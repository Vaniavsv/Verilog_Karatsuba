#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <locale>
#include <cmath>

std::vector<int> gen_num(int bits) {
    std::vector<int> num((bits + 31) / 32, 0);

    for (unsigned int i = 0; i < num.size(); ++i) {
        num[i] = rand();
    }

    int rem_bits = bits % 32;
    if (rem_bits != 0) {
        num.back() &= (1 << rem_bits) - 1;
    }

    return num;
}

std::string fmt_num(const std::vector<int>& num, int bits) {
    std::string ver_str = std::to_string(bits) + "'b";
    bool lead_zero = true;

    for (int i = num.size() - 1; i >= 0; --i) {
        for (int j = 31; j >= 0; --j) {
            int bit = (num[i] >> j) & 1;
            if (bit == 1) {
                lead_zero = false;
            }
            if (!lead_zero) {
                ver_str += bit ? '1' : '0';
            }
        }
    }

    if (ver_str.back() == 'b') {
        ver_str += '0';
    }

    return ver_str;
}

void gen_sm(std::ofstream& file, int N) {
    int sm_bits = 2 * (N / 2) + 1;
    int it_bits = static_cast<int>(ceil(log2(N / 2 + 1)));

    file << "module sm (\n";
    file << "    output reg [" << sm_bits + 1 << ":0] prod,\n";
    file << "    input [" << N / 2 << ":0] a,\n";
    file << "    input [" << N / 2 << ":0] b\n";
    file << ");\n\n";

    file << "    reg [" << sm_bits + 1 << ":0] res;\n";
    file << "    reg [" << it_bits << ":0] i;\n\n";

    file << "    always @(*) begin\n";
    file << "        res = " << sm_bits + 2 << "'b0;\n";
    file << "        for (i = 0; i < " << (N / 2 + 1) << "; i = i + 1) begin\n";
    file << "            if ((b & (1 << i)) != 0) begin\n";
    file << "                res = res + (a << i);\n";
    file << "            end\n";
    file << "        end\n";
    file << "        prod = res;\n";
    file << "    end\n";
    file << "endmodule\n\n";
}

void gen_ver(int orig_N, const std::string& file_name) {
    std::ofstream file(file_name);

    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file for writing." << std::endl;
        return;
    }

    int adj_N = orig_N % 2 == 0 ? orig_N : orig_N + 1;
    gen_sm(file, adj_N);

    int halfN = adj_N / 2;
    int full_bits = 2 * adj_N;

    file << "module karatsuba_mul (\n";
    file << "    input [" << adj_N - 1 << ":0] a,\n";
    file << "    input [" << adj_N - 1 << ":0] b,\n";
    file << "    output reg [" << full_bits - 1 << ":0] prod\n";
    file << ");\n\n";

    file << "    reg [" << halfN << ":0] a_hi, a_lo;\n";
    file << "    reg [" << halfN << ":0] b_hi, b_lo;\n";
    file << "    reg [" << halfN << ":0] sum_a, sum_b;\n";
    file << "    wire [" << (2 * halfN + 2) << ":0] res_lo, res_mid, res_hi;\n";
    file << "    reg [" << full_bits - 1 << ":0] mid, hi;\n\n";

    file << "    always @(*) begin\n";
    file << "        a_hi = {1'b0, a[" << adj_N - 1 << ":" << halfN << "]};\n";
    file << "        a_lo = {1'b0, a[" << halfN - 1 << ":0]};\n";
    file << "        sum_a = a_hi + a_lo;\n\n";

    file << "        b_hi = {1'b0, b[" << adj_N - 1 << ":" << halfN << "]};\n";
    file << "        b_lo = {1'b0, b[" << halfN - 1 << ":0]};\n";
    file << "        sum_b = b_hi + b_lo;\n";
    file << "    end\n\n";

    file << "    sm mult_lo(.a(a_lo), .b(b_lo), .prod(res_lo));\n";
    file << "    sm mult_mid(.a(a_hi), .b(b_hi), .prod(res_mid));\n";
    file << "    sm mult_hi(.a(sum_a), .b(sum_b), .prod(res_hi));\n\n";

    file << "    always @(*) begin\n";
    file << "        mid = res_hi - res_mid - res_lo;\n";
    file << "        hi = res_mid << " << adj_N << ";\n";
    file << "    end\n\n";

    file << "    always @(*) begin\n";
    file << "        prod = hi + (mid << " << halfN << ") + res_lo;\n";
    file << "    end\n";
    file << "endmodule\n";

    file << "\n`timescale 1ns/1ps\n";
    file << "module tb_karatsuba_mul;\n";
    file << "    reg [" << adj_N - 1 << ":0] A;\n";
    file << "    reg [" << adj_N - 1 << ":0] B;\n";
    file << "    wire [" << full_bits - 1 << ":0] res;\n\n";

    file << "    karatsuba_mul uut (\n";
    file << "        .a(A),\n";
    file << "        .b(B),\n";
    file << "        .prod(res)\n";
    file << "    );\n\n";

    file << "    initial begin\n";
    file << "        $display(\"Starting tests...\");\n\n";

    for (int i = 0; i < 10; i++) {
        auto rand_a = gen_num(orig_N);
        auto rand_b = gen_num(orig_N);

        std::string ver_rand_a = fmt_num(rand_a, adj_N);
        std::string ver_rand_b = fmt_num(rand_b, adj_N);

        file << "        A = " << ver_rand_a << "; B = " << ver_rand_b << ";\n";
        file << "        #10;\n";
        file << "        if (res == A * B) begin\n";
        file << "            $display(\"Pass: A=%d, B=%d, res=%d\", A, B, res);\n";
        file << "        end else begin\n";
        file << "            $display(\"Fail: A=%d, B=%d, expected=%d, got=%d\", A, B, A * B, res);\n";
        file << "        end\n\n";
    }

    file << "        $finish;\n";
    file << "    end\n";
    file << "endmodule\n";

    file.close();
}

int main() {
    setlocale(LC_ALL, "Russian");

    srand(static_cast<unsigned int>(time(0)));

    int N;
    std::cout << "Введите разрядность N: ";
    std::cin >> N;

    if (N <= 0) {
        std::cerr << "Ошибка: число N должно быть положительным" << std::endl;
        return 1;
    }

    std::string file_name = "karatsuba_mul_" + std::to_string(N) + ".v";
    gen_ver(N, file_name);

    std::cout << "Verilog-описание сгенерировано в файл: " << file_name << std::endl;
    return 0;
}

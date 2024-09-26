#include <iostream>
#include <fstream>
#include <string>
#include <locale>

void generate_sm_module(std::ofstream& file, int N) {
    int sm_bits = 2 * (N / 2) + 1;
    int iter_bits = static_cast<int>(ceil(log2(N / 2 + 1)));

    file << "module sm (\n";
    file << "    output reg [" << sm_bits + 1 << ":0] prod,\n";
    file << "    input [" << N / 2 << ":0] a,\n";
    file << "    input [" << N / 2 << ":0] b\n";
    file << ");\n\n";

    file << "    reg [" << sm_bits + 1 << ":0] res;\n";
    file << "    reg [" << iter_bits - 1 << ":0] i;\n\n";

    file << "    always @(*) begin\n";
    file << "        res = " << sm_bits + 2 << "'b0;\n";
    file << "        for (i = 0; i < " << (N / 2 + 1) << "; i = i + 1) begin\n";
    file << "            if ((b & (1 << i)) != 0) begin\n";
    file << "                res = res + (a << i); \n";
    file << "            end\n";
    file << "        end\n";
    file << "        prod = res;\n";
    file << "    end\n";
    file << "endmodule\n\n";
}

void generate_verilog(int N, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл для записи." << std::endl;
        return;
    }

    generate_sm_module(file, N);

    int halfN = N / 2;
    int full_bits = 2 * N;

    file << "module karatsuba_multiplier (\n";
    file << "    input [" << N - 1 << ":0] a,\n";
    file << "    input [" << N - 1 << ":0] b,\n";
    file << "    output reg [" << full_bits - 1 << ":0] prod\n";
    file << ");\n\n";

    file << "    reg [" << halfN << ":0] a_hi, a_lo;\n";
    file << "    reg [" << halfN << ":0] b_hi, b_lo;\n";
    file << "    reg [" << halfN << ":0] sum_a, sum_b;\n";
    file << "    wire [" << (2 * halfN + 2) << ":0] res_lo, res_mid, res_hi;\n";
    file << "    reg [" << full_bits - 1 << ":0] mid, hi;\n\n";

    file << "    always @(*) begin\n";
    file << "        a_hi = {1'b0, a[" << N - 1 << ":" << halfN << "]};\n";
    file << "        a_lo = {1'b0, a[" << halfN - 1 << ":0]};\n";
    file << "        sum_a = a_hi + a_lo;\n\n";

    file << "        b_hi = {1'b0, b[" << N - 1 << ":" << halfN << "]};\n";
    file << "        b_lo = {1'b0, b[" << halfN - 1 << ":0]};\n";
    file << "        sum_b = b_hi + b_lo;\n";
    file << "    end\n\n";

    file << "    sm mult_lo(.a(a_lo), .b(b_lo), .prod(res_lo));\n";
    file << "    sm mult_mid(.a(a_hi), .b(b_hi), .prod(res_mid));\n";
    file << "    sm mult_hi(.a(sum_a), .b(sum_b), .prod(res_hi));\n\n";

    file << "    always @(*) begin\n";
    file << "        mid = res_hi - res_mid - res_lo;\n";
    file << "        hi = res_mid << " << N << ";\n";
    file << "    end\n\n";

    file << "    always @(*) begin\n";
    file << "        prod = hi + (mid << " << halfN << ") + res_lo;\n";
    file << "    end\n";
    file << "endmodule\n";
    file.close();
}

int main() {
    setlocale(LC_ALL, "Russian");

    int N;
    std::cout << "Введите разрядность N: ";
    std::cin >> N;
    if (N % 2 != 0) {
        std::cout << "N нечётное, увеличиваю на 1 для чётности.\n";
        N++;
    }
    std::string filename = "karatsuba_multiplier_" + std::to_string(N) + ".v";

    generate_verilog(N, filename);

    std::cout << "Verilog код сгенерирован в файл: " << filename << std::endl;
    return 0;
}

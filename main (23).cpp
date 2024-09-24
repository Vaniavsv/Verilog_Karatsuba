#include <iostream>
#include <fstream>
#include <string>

void generate_verilog(int n) {
    if (n % 2 != 0) {
        n++;
    }

    std::string filename = "karatsuba_multiplier_" + std::to_string(n) + ".v";
    std::ofstream file(filename);

    file << "module karatsuba_multiplier (\n";
    file << "    input  [" << (n-1) << ":0] A,\n";
    file << "    input  [" << (n-1) << ":0] B,\n";
    file << "    output [" << (2*n-1) << ":0] R\n";
    file << ");\n";
    file << "    wire [" << (n/2-1) << ":0] A1 = A[" << (n-1) << ":" << n/2 << "];\n";
    file << "    wire [" << (n/2-1) << ":0] A0 = A[" << (n/2-1) << ":0];\n";
    file << "    wire [" << (n/2-1) << ":0] B1 = B[" << (n-1) << ":" << n/2 << "];\n";
    file << "    wire [" << (n/2-1) << ":0] B0 = B[" << (n/2-1) << ":0];\n\n";

    file << "    wire [" << (n-1) << ":0] P1 = A1 * B1;\n";
    file << "    wire [" << (n-1) << ":0] P2 = A0 * B0;\n";
    file << "    wire [" << (n+1) << ":0] P3 = (A1 + A0) * (B1 + B0);\n\n";

    file << "    assign R = (P1 << " << n << ") + ((P3 - P1 - P2) << " << n/2 << ") + P2;\n";
    file << "endmodule\n";

    file.close();
}

int main() {
    int n;
    std::cout << "Enter N: ";
    std::cin >> n;
    generate_verilog(n);
    std::cout << "Verilog file generated: karatsuba_multiplier_*.v" << std::endl;
    return 0;
}

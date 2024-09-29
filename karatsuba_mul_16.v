module sm (
    output reg [18:0] prod,
    input [8:0] a,
    input [8:0] b
);

    reg [18:0] res;
    reg [4:0] i;

    always @(*) begin
        res = 19'b0;
        for (i = 0; i < 9; i = i + 1) begin
            if ((b & (1 << i)) != 0) begin
                res = res + (a << i);
            end
        end
        prod = res;
    end
endmodule

module karatsuba_mul (
    input [15:0] a,
    input [15:0] b,
    output reg [31:0] prod
);

    reg [8:0] a_hi, a_lo;
    reg [8:0] b_hi, b_lo;
    reg [8:0] sum_a, sum_b;
    wire [18:0] res_lo, res_mid, res_hi;
    reg [31:0] mid, hi;

    always @(*) begin
        a_hi = {1'b0, a[15:8]};
        a_lo = {1'b0, a[7:0]};
        sum_a = a_hi + a_lo;

        b_hi = {1'b0, b[15:8]};
        b_lo = {1'b0, b[7:0]};
        sum_b = b_hi + b_lo;
    end

    sm mult_lo(.a(a_lo), .b(b_lo), .prod(res_lo));
    sm mult_mid(.a(a_hi), .b(b_hi), .prod(res_mid));
    sm mult_hi(.a(sum_a), .b(sum_b), .prod(res_hi));

    always @(*) begin
        mid = res_hi - res_mid - res_lo;
        hi = res_mid << 16;
    end

    always @(*) begin
        prod = hi + (mid << 8) + res_lo;
    end
endmodule

`timescale 1ns/1ps
module tb_karatsuba_mul;
    reg [15:0] A;
    reg [15:0] B;
    wire [31:0] res;

    karatsuba_mul uut (
        .a(A),
        .b(B),
        .prod(res)
    );

    initial begin
        $display("Starting tests...");

        A = 16'b10111011000110; B = 16'b100100101000100;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b11111001010; B = 16'b11011100100111;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b110100101110101; B = 16'b100110111001101;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b11100110101100; B = 16'b100101101001010;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b111011011110000; B = 16'b111000111101100;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b110100011100000; B = 16'b110011100110110;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b11000111001111; B = 16'b111001110100110;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b11000111101011; B = 16'b111000110110001;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b111000011001101; B = 16'b110110110011101;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b10100001110000; B = 16'b100010100011101;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        $finish;
    end
endmodule

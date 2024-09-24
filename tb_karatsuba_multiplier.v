`timescale 1ns/1ps

module tb_karatsuba_multiplier;
    reg [15:0] A;
    reg [15:0] B;
    wire [31:0] R;

    karatsuba_multiplier uut (
        .A(A),
        .B(B),
        .R(R)
    );
    initial begin
        A = 16'd10;
        B = 16'd12;
        #10; 
        $display("A = %d, B = %d, R = %d (Expected: 120)", A, B, R);

        A = 16'd255;
        B = 16'd255;
        #10;
        $display("A = %d, B = %d, R = %d (Expected: 65025)", A, B, R);

        A = 16'd1024;
        B = 16'd512;
        #10;
        $display("A = %d, B = %d, R = %d (Expected: 524288)", A, B, R);

        A = 16'd1234;
        B = 16'd5678;
        #10;
        $display("A = %d, B = %d, R = %d (Expected: 7006652)", A, B, R);

        A = 16'd65535;
        B = 16'd65535;
        #10;
        $display("A = %d, B = %d, R = %d (Expected: 4294836225)", A, B, R);

        $finish;
    end
endmodule

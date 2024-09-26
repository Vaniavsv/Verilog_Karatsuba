`timescale 1ns/1ps

module tb_karatsuba_multiplier;
    reg [15:0] A;
    reg [15:0] B;
    wire [31:0] result;

    karatsuba_multiplier uut (
        .a(A),
        .b(B),
        .prod(result)
    );

    initial begin
        A = 16'd3;
        B = 16'd5;
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 15)", A, B, result);

        A = 16'd12;
        B = 16'd10;
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 120)", A, B, result);

        A = 16'd8;
        B = 16'd7;
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 56)", A, B, result);

        A = 16'd15;
        B = 16'd15;
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 225)", A, B, result);

        A = 16'd0;
        B = 16'd1;
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 0)", A, B, result);

        A = 16'd65535;
        B = 16'd65535; 
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 4294836225)", A, B, result);

        A = 16'd1234;
        B = 16'd5678; 
        #10;
        $display("A = %d, B = %d, result = %d (Expected: 7006652)", A, B, result);

        A = 16'd32768;
        B = 16'd2;
        #10;
        $display("A = %d, B = %d, result = %d (Expected: 65536)", A, B, result);

        A = 16'd1024;
        B = 16'd1024;
        #10;
        $display("A = %d, B = %d, result = %d (Expected: 1048576)", A, B, result);

        $finish;
    end
endmodule

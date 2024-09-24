module karatsuba_multiplier (
    input  [15:0] A,
    input  [15:0] B,
    output [31:0] R
);
    wire [7:0] A1 = A[15:8];
    wire [7:0] A0 = A[7:0];
    wire [7:0] B1 = B[15:8];
    wire [7:0] B0 = B[7:0];

    wire [15:0] P1 = A1 * B1;
    wire [15:0] P2 = A0 * B0;
    wire [17:0] P3 = (A1 + A0) * (B1 + B0);

    assign R = (P1 << 16) + ((P3 - P1 - P2) << 8) + P2;
endmodule

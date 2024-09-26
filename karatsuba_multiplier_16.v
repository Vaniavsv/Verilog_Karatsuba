module sm (
    output reg [18:0] prod,
    input [8:0] a,
    input [8:0] b
);

    reg [18:0] res;
    reg [3:0] i;

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

module karatsuba_multiplier (
    input [15:0] a,
    input [15:0] b,
    output reg [31:0] prod
);

    reg [8:0] a_hi, a_lo;
    reg [8:0] b_hi, b_lo;
    reg [9:0] sum_a, sum_b;
    wire [18:0] res_lo, res_mid, res_hi;
    reg [31:0] mid, hi;

    always @(*) begin
        a_hi = a[15:8];
        a_lo = {1'b0, a[7:0]};
        sum_a = a_hi + a_lo;

        b_hi = b[15:8];
        b_lo = {1'b0, b[7:0]};
        sum_b = b_hi + b_lo;
    end

    sm mult_lo(.a(a_lo), .b(b_lo), .prod(res_lo));
    sm mult_mid(.a(a_hi), .b(b_hi), .prod(res_mid));
    sm mult_hi(.a(sum_a[8:0]), .b(sum_b[8:0]), .prod(res_hi));

    always @(*) begin
        mid = res_hi - res_mid - res_lo;
        hi = res_mid << 16;
    end

    always @(*) begin
        prod = hi + (mid << 8) + res_lo;
    end
endmodule

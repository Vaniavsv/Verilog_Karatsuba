`timescale 1ns/1ps

module tb_karatsuba_multiplier;
    reg [15:0] A;  // Входные данные для умножителя
    reg [15:0] B;  // Входные данные для умножителя
    wire [31:0] result;  // Результат умножения

    // Подключение тестируемого модуля
    karatsuba_multiplier uut (
        .a(A),
        .b(B),
        .prod(result)
    );

    initial begin
        // Установка значений и задержек для тестов
        A = 16'd3;  // 3
        B = 16'd5;  // 5
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 15)", A, B, result);

        A = 16'd12; // 12
        B = 16'd10; // 10
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 120)", A, B, result);

        A = 16'd8;  // 8
        B = 16'd7;  // 7
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 56)", A, B, result);

        A = 16'd15; // 15
        B = 16'd15; // 15
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 225)", A, B, result);

        A = 16'd0;  // 0
        B = 16'd1;  // 1
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 0)", A, B, result);

        A = 16'd65535; // 65535 (максимум для 16-битных чисел)
        B = 16'd65535; // 65535 (максимум для 16-битных чисел)
        #10; 
        $display("A = %d, B = %d, result = %d (Expected: 4294836225)", A, B, result);

        // Дополнительные тесты с разными значениями
        A = 16'd1234; // 1234
        B = 16'd5678; // 5678
        #10;
        $display("A = %d, B = %d, result = %d (Expected: 7006652)", A, B, result);

        A = 16'd32768; // 32768 (высокий бит)
        B = 16'd2;     // 2
        #10;
        $display("A = %d, B = %d, result = %d (Expected: 65536)", A, B, result);

        A = 16'd1024;  // 1024
        B = 16'd1024;  // 1024
        #10;
        $display("A = %d, B = %d, result = %d (Expected: 1048576)", A, B, result);

        // Завершение симуляции
        $finish;
    end
endmodule

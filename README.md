# Verilog_Karatsuba
# Генератор умножителя Карацубы

Этот проект представляет собой программу на языке C++, которая генерирует Verilog-описание умножителя, основанного на алгоритме Карацубы, для двух неотрицательных чисел разрядности `N`. Программа принимает на вход целое число `N` и создает файл с Verilog-описанием, реализующим умножение по алгоритму Карацубы.

## Структура проекта

- **main.cpp**: Программа на C++ для генерации Verilog-описания умножителя Карацубы.
- **Makefile**: Скрипт сборки для компиляции программы и выполнения операций очистки.
- **Verilog-файлы**: Сгенерированные Verilog-файлы имеют имена, такие как `karatsuba_mul_N.v`, где `N` — разрядность входных чисел.

## Описание алгоритма Карацубы

Алгоритм Карацубы предназначен для быстрого умножения больших чисел. Он уменьшает количество операций умножения за счет разбиения чисел на части, вычисления промежуточных произведений и их последующего комбинирования с меньшей сложностью по сравнению с классическим методом умножения.

Для чисел `A` и `B` разрядности `N` произведение `R` вычисляется по следующей формуле:
A * B = P1 * 2^N + (P3 - P1 - P2) * 2^(N/2) + P2
где:
- `P1 = A1 * B1`
- `P2 = A0 * B0`
- `P3 = (A1 + A0) * (B1 + B0)`

## Как собрать и запустить

### Требования

- Операционная система Linux
- Компилятор `g++` с поддержкой стандарта C++17

### Сборка

Для компиляции программы выполните следующую команду в терминале:

```bash
make
```
Это скомпилирует файл main.cpp и создаст исполняемый файл с именем karatsuba_gen.


### Запуск
Чтобы сгенерировать Verilog-файл для заданной разрядности N, выполните программу следующим образом:
```bash
./karatsuba_gen
```
После запуска вас попросят ввести разрядность N. После ввода числа программа сгенерирует Verilog-файл с именем karatsuba_mul_N.v.

### Очистка
Чтобы удалить скомпилированный исполняемый файл и объектные файлы, выполните:

     make clean

Чтобы удалить сгенерированные Verilog-файлы:

     make clean_verilog

Пример сгенерированного Verilog-описания
Для разрядности N = 16 программа генерирует следующий Verilog-модуль:

```bash
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

        A = 16'b11111001100000; B = 16'b101111000011001;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b110011100010100; B = 16'b100010111011111;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b110010110111001; B = 16'b1010100100100;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b100111011101011; B = 16'b10101010110001;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b1100001111011; B = 16'b11011001111101;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b10011001000111; B = 16'b11100110100011;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b101011011010100; B = 16'b11101100100011;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b101010110000010; B = 16'b111111110110110;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b110011110010011; B = 16'b100010100111110;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        A = 16'b101001001101010; B = 16'b100011010001100;
        #10;
        if (res == A * B) begin
            $display("Pass: A=%d, B=%d, res=%d", A, B, res);
        end else begin
            $display("Fail: A=%d, B=%d, expected=%d, got=%d", A, B, A * B, res);
        end

        $finish;
    end
endmodule


```

### Пример вывода тестов
```bash
Starting tests...
Pass: A=15968, B=24089, res= 384653152
Pass: A=26388, B=17887, res= 472002156
Pass: A=26041, B= 5412, res= 140933892
Pass: A=20203, B=10929, res= 220798587
Pass: A= 6267, B=13949, res=  87418383
Pass: A= 9799, B=14755, res= 144584245
Pass: A=22228, B=15139, res= 336509692
Pass: A=21890, B=32694, res= 715671660
Pass: A=26515, B=17726, res= 470004890
Pass: A=21098, B=18060, res= 381029880
```



# Verilog_Karatsuba
# Генератор умножителя Карацубы

Этот проект представляет собой программу на языке C++, которая генерирует Verilog-описание умножителя, основанного на алгоритме Карацубы, для двух неотрицательных чисел разрядности `N`. Программа принимает на вход целое число `N` и создает файл с Verilog-описанием, реализующим умножение по алгоритму Карацубы.

## Структура проекта

- **main.cpp**: Программа на C++ для генерации Verilog-описания умножителя Карацубы.
- **Makefile**: Скрипт сборки для компиляции программы и выполнения операций очистки.
- **tb_karatsuba_multiplier.v**: Тестбенч для проверки сгенерированного Verilog-модуля.
- **Verilog-файлы**: Сгенерированные Verilog-файлы имеют имена, такие как `karatsuba_multiplier_N.v`, где `N` — разрядность входных чисел.

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
После запуска вас попросят ввести разрядность N. После ввода числа программа сгенерирует Verilog-файл с именем karatsuba_multiplier_N.v.

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

```
## Тестирование
Тестбенч tb_karatsuba_multiplier.v используется для тестирования сгенерированного Verilog-модуля. В нем задаются несколько тестов с известными результатами для проверки правильности умножения.

```bash
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
```
### Пример вывода тестов
```bash
A =     3, B =     5, result =         15 (Expected: 15)
A =    12, B =    10, result =        120 (Expected: 120)
A =     8, B =     7, result =         56 (Expected: 56)
A =    15, B =    15, result =        225 (Expected: 225)
A =     0, B =     1, result =          0 (Expected: 0)
A = 65535, B = 65535, result = 4294836225 (Expected: 4294836225)
A =  1234, B =  5678, result =    7006652 (Expected: 7006652)
A = 32768, B =     2, result =      65536 (Expected: 65536)
A =  1024, B =  1024, result =    1048576 (Expected: 1048576)
```



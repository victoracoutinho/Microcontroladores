Para as questões 2 a 5, considere que as variáveis `f`, `g`, `h`, `i` e `j` são do tipo inteiro (16 bits na arquitetura do MSP430), e que o vetor `A[]` é do tipo inteiro. Estas variáveis estão armazenadas nos seguintes registradores:
	f: R4
	g: R5
	h: R6
	i: R7
	j: R8
	A: R9
Utilize os registradores R11, R12, R13, R14 e R15 para armazenar valores temporários.

1. Escreva os trechos de código assembly do MSP430 para:
	(a) Somente setar o bit menos significativo de R5.
  `bis #1, R5`
	(b) Somente setar dois bits de R6: o menos significativo e o segundo menos significativo.
  `bis #3,R6`
	(c) Somente zerar o terceiro bit menos significativo de R7.
  `and.w #4,R7`
	(d) Somente zerar o terceiro e o quarto bits menos significativo de R8.
  `and.w #12,R8`
	(e) Somente inverter o bit mais significativo de R9.
  `xor.w #08000,R9`
	(f) Inverter o nibble mais significativo de R10, e setar o nibble menos significativo de R10. 
```C
  xor.w #0F000, R10
  bis #0000F,R10
```

2. "Traduza" o seguinte trecho de código em C para o assembly do MSP430:

```C
if(i>j) f = g+h+10;
else f = g-h-10;
```
```C
cmp R7,R8
jge lab1
	add R6,R5
	add #10,R6
	mov.w R6, R4
lab1:	sub R6,R5
	sub #10,R6
	mov.w R6, R4
```
3. "Traduza" o seguinte trecho de código em C para o assembly do MSP430:

```C
while(save[i]!=k) i++;
```
```C
while: mov.w R7,R11

```
4. "Traduza" o seguinte trecho de código em C para o assembly do MSP430:

```C
for(i=0; i<100; i++) A[i] = i*2;
```

5. "Traduza" o seguinte trecho de código em C para o assembly do MSP430:

```C
for(i=99; i>=0; i--) A[i] = i*2;
```

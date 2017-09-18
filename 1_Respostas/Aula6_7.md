Para cada questão, escreva funções em C e/ou sub-rotinas na linguagem Assembly do MSP430. Reaproveite funções e sub-rotinas de uma questão em outra, se assim desejar. Leve em consideração que as sub-rotinas são utilizadas em um código maior, portanto utilize adequadamente os registradores R4 a R11. As instruções da linguagem Assembly do MSP430 se encontram ao final deste texto.

1. (a) Escreva uma função em C que calcule a raiz quadrada `x` de uma variável `S` do tipo float, utilizando o seguinte algoritmo: após `n+1` iterações, a raiz quadrada de `S` é dada por

```
x(n+1) = (x(n) + S/x(n))/2
```

```C
unsigned int raiz_quadrada (unsigned int numero){
    ///Cálculo da raiz quadrada do número - INTEIRO
    int i;
    unsigned int resultado[20];
    resultado[0] = numero/2;

    if (numero > 0){
        for (i = 1; i < 20; i++) {
        resultado[i] = (resultado[i-1]+ numero/resultado[i-1])/2;
        //printf("%d | %u\n",i, resultado[i]);
        }
    }else {
        resultado[19] = 0;
    }

    return resultado[19];
}
```

(b) Escreva a sub-rotina equivalente na linguagem Assembly do MSP430. A variável `S` é fornecida pelo registrador R15, e a raiz quadrada de `S` (ou seja, a variável `x`) é fornecida pelo registrador R15 também.
```
; FUNÇÃO RAIZ QUADRADA --------------------------------------------------------
 raiz_quadrada: ; Raiz de R15
        clr.w R14   ;Resultado[i-1]
        clr.w R13   ;Resultado[i]
        clr.w R12; Equivalente ao int i = 0
        ;Dividir R15/2
        push.w R15
        push.w R13
        mov.w #2,R14
        call #divisao
        mov.w R15,R14 ;Resultado da divisão no R13 -- Resultado anterior
        pop.w R13
        pop.w R15
        ; Fim da divisão
for_raiz_quadrada:
        ;Dividir numero/resultado[i-1]
        push.w R15
        push.w R14
        call #divisao ; R15 é o número e R14 e o resultado anterior
        mov.w R15,R13 ;Resultado da divisão no R13 -- numero/resultado[i-1]
        pop.w R14
        pop.w R15
        ; Fim da divisão
        
        add R14,R13    ; resultado[i-1] + numero/resultado[i-1]
        
        ;Dividir (resultado[i-1]+ numero/resultado[i-1])/2
        push.w R15
        push.w R14
        mov.w R13,R15
        mov.w #2, R14
        call #divisao ; R15 é o número e R14 e o resultado anterior
        mov.w R15,R13 ;Resultado da divisão no R13 -- numero/resultado[i-1]
        pop.w R14
        pop.w R15
        ; Fim da divisão
        
        mov.w R13, R14;resultado[i] = (resultado[i-1]+ numero/resultado[i-1])/2;
        inc.w R12     ; i++
        cmp #20, R12
        jl for_raiz_quadrada
        mov.w R14,R15
        ret
 
 ;------------------------------------------------------------------------------
 
; FUNÇÃO DIVISÃO ---------------------------------------------------------------
divisao ; R15 dividido por R14
        clr.w R13         ; Conta quantas vezes R15 é subtraido por R14
verificar:
        cmp R14,R15       ; Até que R15 seja menor que R14
        jge divisao_else
        mov.w R13,R15     ;Aredonda para o menor número
        ret
divisao_else
        sub.w R14, R15
        inc.w R13         ; Contagem
        jmp verificar
;-------------------------------------------------------------------------------
        END
```

2. (a) Escreva uma função em C que calcule `x` elevado à `N`-ésima potência, seguindo o seguinte protótipo: 

```C
int Potencia (int x, int N){
    ///Cálculo de Potência de x elevado a N
    int i;
    float resultado = 1;
    printf("%d\n",N);

    if (N > 0){
        for (i = 0; i < N; i++) {
        resultado = x * resultado;
        }
    }
    return resultado;
}
```

(b) Escreva a sub-rotina equivalente na linguagem Assembly do MSP430. `x` e `n` são fornecidos através dos registradores R15 e R14, respectivamente, e a saída deverá ser fornecida no registrador R15.

```
; FUNÇÃO POTÊNCIA --------------------------------------------------------------

potencia:    ;Função que calcula R15^(R14)
        clr.w R13       ; Contador do for. Equivalente ao int i = 0
        mov.w #1,R12       ; resultado. Fica acumulando.
for_potencia:
        
        push.w R15     ;Multiplicação entre resultado( R12 ) e R15
        push.w R14
        mov.w R12, R14
        call #mult
        mov.w R15, R12 ;Resposta da multiplicação fica R12
        pop.w R14
        pop.w R15
        
        inc.w R13      ; equivalente ao i++
        cmp R14,R13    ; i == N, equivalente ao i<N
        jl for_potencia
        mov.w R12, R15
        ret
;-------------------------------------------------------------------------------


; FUNÇÃO MULTIPLICAÇÃO ---------------------------------------------------------
mult:
        TST R14       ; Verificar se R14 é igual a zero
        JNZ mult_else ; caso não seja, continua decaindo
        CLR.W R15   ;Se for igual a zero, então limpa R15 para colocar as somas
        ret
mult_else
        PUSH.W R15    ;decai o R14 para guardar R15 R14 vezes na pilha
        DEC.W R14     ; até R14 for igual a zero
        CALL #mult    ;No MULT ele vai testar R14
        POP.W R14     ; Reculpera todos R15 que estão na pilha para R14
        ADD R14,R15
        RET
;-------------------------------------------------------------------------------
```

3. Escreva uma sub-rotina na linguagem Assembly do MSP430 que calcula a divisão de `a` por `b`, onde `a`, `b` e o valor de saída são inteiros de 16 bits. `a` e `b` são fornecidos através dos registradores R15 e R14, respectivamente, e a saída deverá ser fornecida através do registrador R15.
```                                    
; FUNÇÃO DIVISÃO ---------------------------------------------------------------
divisao ; R15 dividido por R14
        clr.w R13         ; Conta quantas vezes R15 é subtraido por R14
verificar:
        cmp R14,R15       ; Até que R15 seja menor que R14
        jge divisao_else
        mov.w R13,R15     ;Aredonda para o menor número
        ret
divisao_else
        sub.w R14, R15
        inc.w R13         ; Contagem
        jmp verificar
;-------------------------------------------------------------------------------
```
4. Escreva uma sub-rotina na linguagem Assembly do MSP430 que calcula o resto da divisão de `a` por `b`, onde `a`, `b` e o valor de saída são inteiros de 16 bits. `a` e `b` são fornecidos através dos registradores R15 e R14, respectivamente, e a saída deverá ser fornecida através do registrador R15.
```
; RESTO DA DIVISÃO -------------------------------------------------------------
resto:
        clr.w R13         ; Conta quantas vezes R15 é subtraido por R14
verificar:
        cmp R14,R15       ; Até que R15 seja menor que R14
        jge resto_else
        ;mov.w R13,R15     ;O RESTO fica no R15
        ret
resto_else
        sub.w R14, R15
        inc.w R13         ; Contagem
        jmp verificar
;-------------------------------------------------------------------------------
```

5. (a) Escreva uma função em C que indica a primalidade de uma variável inteira sem sinal, retornando o valor 1 se o número for primo, e 0, caso contrário. Siga o seguinte protótipo:

```C
int Primalidade (unsigned int numero){
    ///Anãlise de paridade
    int i, j;
    //int resultado = 1;
    int auxiliar;

    for (i = 2; i < numero; i++){
            for (j = 1; j< numero; j++){
                auxiliar = i*j; ///verifica se numero é divisível por i
                if (auxiliar == numero){
                    return 0; /// Não é primo!
                }
        }
    }
    /// Se chegou aqui é porque é PRIMO
    return 1;
    }
```

(b) Escreva a sub-rotina equivalente na linguagem Assembly do MSP430. A variável de entrada é fornecida pelo registrador R15, e o valor de saída também.
```
#include "msp430.h"                     ; #define controlled include file

        NAME    main                    ; module name

        PUBLIC  main                    ; make the main label vissible
                                        ; outside this module
        ORG     0FFFEh
        DC16    init                    ; set reset vector to 'init' label

        RSEG    CSTACK                  ; pre-declaration of segment
        RSEG    CODE                    ; place program in 'CODE' segment

init:   MOV     #SFE(CSTACK), SP        ; set up stack

main:   NOP                             ; main program
        MOV.W   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
        
        MOV.W #10,R15
        CALL #primaridade
        MOV.W R15,R6
        
        JMP $                           ; jump to current location '$'
                                        ; (endless loop)

; FUNÇÃO PRIMARIDADE -----------------------------------------------------------
primaridade:
        mov.w #2,R14      ; Equivalente ao int i = 2
        mov.w #1,R13      ; Equivalente ao int j = 1
        clr.w R12         ; Equivalente ao int auxiliar = 0
for_i_primaridade:
        cmp R15,R14
        jge fim_for_i_primaridade
for_j_primaridade:
        cmp R15,R13
        jge fim_for_j_primaridade
          ;Multiplicação
          push.w R15
          push.w R14
          mov.w R13,R15   ; R5 = j e R14 já é igual a i
          call #mult
          mov.w R15,R12   ; R12 = R13*R14 ==> auxiliar = i*j
          pop.w R14
          pop.w R15
          ; Fim da multiplicação
          
          cmp R12,R15      ; Se for i*j for igual ao R15, então ele não é primo
          jeq nao_primo

          inc.w R13       ; j++
          jmp for_j_primaridade

fim_for_j_primaridade
        inc.w R14         ; i++
        jl for_i_primaridade
fim_for_i_primaridade:
        mov.w #1,R15      ; Se chegou aqui é porque não existe nenhum i*j que
        ret               ; igual ao R15
nao_primo:
        mov.w #0,R15      ; teve um i*j igual R15
        ret
;-------------------------------------------------------------------------------

; FUNÇÃO MULTIPLICAÇÃO ---------------------------------------------------------
mult:
        TST R14       ; Verificar se R14 é igual a zero
        JNZ mult_else ; caso não seja, continua decaindo
        CLR.W R15   ;Se for igual a zero, então limpa R15 para colocar as somas
        ret
mult_else
        PUSH.W R15    ;decai o R14 para guardar R15 R14 vezes na pilha
        DEC.W R14     ; até R14 for igual a zero
        CALL #mult    ;No MULT ele vai testar R14
        POP.W R14     ; Reculpera todos R15 que estão na pilha para R14
        ADD R14,R15
        RET
;-------------------------------------------------------------------------------

        END

```
6. Escreva uma função em C que calcula o duplo fatorial de n, representado por n!!. Se n for ímpar, n!! = 1*3*5*...*n, e se n for par, n!! = 2*4*6*...*n. Por exemplo, 9!! = 1*3*5*7*9 = 945 e 10!! = 2*4*6*8*10 = 3840. Além disso, 0!! = 1!! = 1.
O protótipo da função é:

```C
unsigned long long DuploFatorial(unsigned long long n){
    ///Duplo fatorial
    ///Se for par o fatorial fica só com os pares multiplicando
    ///Se for impar o fatorial fica só impares multiplicando
    int i, j;
    int numero = n; // Para não causar um loop infinito na hora de fazer o fatorial
    unsigned long long resultado = 1;
    int paridade;

    ///Verificar se é impar ou par
    if( (n%2) == 0){
        paridade = 1; //PAR
    }else{
        paridade = 0; //IMPAR
    }

    ///Fazer o fatorial duplo
    if (paridade == 1){
        for (i = 2; i <= numero; i+=2){//somente pares
            resultado = i*resultado;
        }
    }else{
        for (i = 1; i <= numero; i+=2){//somente impares
            resultado = i*resultado;
        }
    }
    
   //manda resultado
   return resultado;
    }
```

7. (a) Escreva uma função em C que calcula a função exponencial utilizando a série de Taylor da mesma. Considere o cálculo até o termo n = 20. O protótipo da função é `double ExpTaylor(double x);`
```C
double Fatorial(unsigned int n){
    ///Fatorial
    int i, j;
    double resultado = 1;

    for (i = 1; i <= n; i++){
        resultado = i*resultado;
        }

    //manda resultado
    return resultado;
    }

    double ExpTaylor(double x){
    ///Calcula a exponencial de 'x' a partir da série de Taylor de ordem 20
    int i,j;
    double resultado = 0, auxiliar = 1;

    ///Fazer o fatorial duplo
    for (i = 0; i <= 20; i++){//somente pares
            auxiliar = 1; //reseta o z^n
            for ( j= 1; j<= i; j++){
                auxiliar = ((double) x)*auxiliar; // Faz o x^n
            }
        resultado += auxiliar/Fatorial(i);
        //printf("%lf | %lf | %lf\n", auxiliar, Fatorial(i), resultado);
        }

    //manda resultado
    return resultado;
}
```


(b) Escreva a sub-rotina equivalente na linguagem Assembly do MSP430, mas considere que os valores de entrada e de saída são inteiros de 16 bits. A variável de entrada é fornecida pelo registrador R15, e o valor de saída também.

8. Escreva uma sub-rotina na linguagem Assembly do MSP430 que indica se um vetor esta ordenado de forma decrescente. Por exemplo:

[5 4 3 2 1] e [90 23 20 10] estão ordenados de forma decrescente.

[1 2 3 4 5] e [1 2 3 2] não estão.

O primeiro endereço do vetor é fornecido pelo registrador R15, e o tamanho do vetor é fornecido pelo registrador R14. A saída deverá ser fornecida no registrador R15, valendo 1 quando o vetor estiver ordenado de forma decrescente, e valendo 0 em caso contrário.
```
#include "msp430.h"                     ; #define controlled include file

        NAME    main                    ; module name

        PUBLIC  main                    ; make the main label vissible
                                        ; outside this module
        ORG     0FFFEh
        DC16    init                    ; set reset vector to 'init' label

        RSEG    CSTACK                  ; pre-declaration of segment
        RSEG    CODE                    ; place program in 'CODE' segment

init:   MOV     #SFE(CSTACK), SP        ; set up stack

main:   NOP                             ; main program
        MOV.W   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
        
        MOV.W #0,R15
        MOV.W #255,0(R15)
        MOV.W #135,2(R15)
        MOV.W #45,4(R15)
        MOV.W #1,6(R15)
        MOV.W #4,R14
        CALL #ordem_decrescente
        
        JMP $                           ; jump to current location '$'
                                        ; (endless loop)
; FUNÇÃO VETOR ORDEM DECRESCENTE -------------------------------------------------
ordem_decrescente:        ; Vetor de posição 0 R15 e tamanho R14
                          ;Valores até 255
        dec.w R14;      ;R14 = R14 -1
        add.w R14,R14   ;R14 = 2*(R14-1)
        mov.w #0,R13
        mov.w #2,R12
        push.w R11
        push.w R10
        mov.w #2,R10
        clr.w R11         ;equivalente ao i = 0
        add.w R15,R13     ; primeira posição do vetor
        add.w R13,R1      ; segunda posição do vetor
        sub.w #2,R13      ; Subtrai para poder repor o dois no
        sub.w #2,R12      ; ordem_crescente_else
        
ordem_decrescente_else:
        cmp R11, R14
        jeq ordem_decrescente_fim
        add.w R10,R13    ; posição anterior do vetor
        add.w R10,R12    ; posição atual do vetor
        incd.w R11       ; equivalente ao i+=2
        cmp 0(R12),0(R13)
        jge ordem_decrescente_else
        mov.w #0,R15     ; CRESCENTE
        pop.w R11
        pop.w R10
        ret
ordem_decrescente_fim:
        mov.w #1,R15     ; DECRESCENTE
        pop.w R11
        pop.w R10
        ret
;-------------------------------------------------------------------------------
          
        END

```


9. Escreva uma sub-rotina na linguagem Assembly do MSP430 que calcula o produto escalar de dois vetores, `a` e `b`. O primeiro endereço do vetor `a` deverá ser passado através do registrador R15, o primeiro endereço do vetor `b` deverá ser passado através do registrador R14, e o tamanho do vetor deverá ser passado pelo registrador R13. A saída deverá ser fornecida no registrador R15.

10. (a) Escreva uma função em C que indica se um vetor é palíndromo. Por exemplo:

	[1 2 3 2 1] e [0 10 20 20 10 0] são palíndromos.
	
	[5 4 3 2 1] e [1 2 3 2] não são.
	
Se o vetor for palíndromo, retorne o valor 1. Caso contrário, retorne o valor 0. O protótipo da função é:

```C
int Palindromo(double* vetor, int tamanho){
    ///Diz se um vetor é palindromo, ou seja,
    /// se os valores do vetor são iguais tanto do centro para um lado quanto para outro.
    /// [0,1,3,2,3,1,0] ou [5,2,3,1,1,3,2,5]

    int i = 0, resultado = 1;

    for (i=0; i< tamanho;i++){
        if (vetor[i] == vetor[(tamanho-1) - i]){
            resultado = 1;
            printf("%d | %.2lf | %.2lf\n", i,vetor[i],vetor[(tamanho-1) - i] );
        }else{
            resultado = 0;
            break;
        }
    }

    return resultado;
}
```

(b) Escreva a sub-rotina equivalente na linguagem Assembly do MSP430. O endereço do vetor de entrada é dado pelo registrador R15, o tamanho do vetor é dado pelo registrador R14, e o resultado é dado pelo registrador R15.

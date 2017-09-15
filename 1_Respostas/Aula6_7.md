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

2. (a) Escreva uma função em C que calcule `x` elevado à `N`-ésima potência, seguindo o seguinte protótipo: 

```C
float Potencia (int x, int N){
    ///Cálculo de Potência de x elevado a N
    int i;
    int resultado = 1;
    printf("%d\n",N);

    if (N > 0){
        for (i = 0; i < N; i++) {
        resultado = x * resultado;
        }
    }if (N < 0) {
        for (i = 0; i < N; i++) {
            resultado = 1/((float) x) * resultado;
            printf("Resp: %f\n", resultado);
        }
    } else {
        printf(" else Resp: %f\n", resultado);
        resultado = 1;
    }

    return resultado;
```

(b) Escreva a sub-rotina equivalente na linguagem Assembly do MSP430. `x` e `n` são fornecidos através dos registradores R15 e R14, respectivamente, e a saída deverá ser fornecida no registrador R15.

3. Escreva uma sub-rotina na linguagem Assembly do MSP430 que calcula a divisão de `a` por `b`, onde `a`, `b` e o valor de saída são inteiros de 16 bits. `a` e `b` são fornecidos através dos registradores R15 e R14, respectivamente, e a saída deverá ser fornecida através do registrador R15.

4. Escreva uma sub-rotina na linguagem Assembly do MSP430 que calcula o resto da divisão de `a` por `b`, onde `a`, `b` e o valor de saída são inteiros de 16 bits. `a` e `b` são fornecidos através dos registradores R15 e R14, respectivamente, e a saída deverá ser fornecida através do registrador R15.

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

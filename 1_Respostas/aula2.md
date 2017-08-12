# Questões da aula 2

1. Quais as diferenças entre os barramentos de dados e de endereços?

	Barramento de dados é por onde passa os dados e os endereços são as posições do dados na memória.

2. Quais são as diferenças entre as memórias RAM e ROM?

	Memória RAM é volatil, ela ao ser desligada é apagada sua memória. Já a memoria ROM e não-volátil, ela guarda os dados mesmo sem energia.

3. Considere o código abaixo:

```C
#include <stdio.h>
int main(void)
{
	int i;
	printf("Insira um número inteiro: ");
	scanf("%d", &i);
	if(i%2)
		printf("%d eh impar.\n");
	else
		printf("%d eh par.\n");
	return 0;
}
```
	
- Para este código, responda: 

	a. A variável `i` é armazenada na memória RAM ou ROM? Por quê? 
	
	- Na memória RAM, pois ela será sobreescrevida e não há necessidade de mantê-la após o fim do código.

	b. O programa compilado a partir deste código é armazenado na memória RAM ou ROM? Por quê?
	
	- Na ROM, pois ele não será alterado e haverá necessidade de mantê-lo após o desligar a energia.

4. Quais são as diferenças, vantagens e desvantagens das arquiteturas Harvard e Von Neumann?
	
	A Harvard tem a memória ROM separada da memória RAM.

5. Considere a variável inteira `i`, armazenando o valor `0x8051ABCD`. Se `i` é armazenada na memória a partir do endereço `0x0200`, como ficam este byte e os seguintes, considerando que a memória é: (a) Little-endian; (b) Big-endian.

	(a) `0xCD`em `0x200`, `0xAB`em `0x201`, `0x51`em `0x202`, `0x80`em `0x203`
	
	(b) `0x80`em `0x200`, `0x51`em `0x201`, `0xAB`em `0x202`, `0xCD`em `0x203`

6. Sabendo que o processador do MSP430 tem registradores de 16 bits, como ele soma duas variáveis de 32 bits?

	Soma as duas variaveis usando apenas os 16 bits menos significativos. Depois, soma as duas variáveis usando os 16 bits mais siginificativos mais a flag do carry da antiga soma. Os 16 bits menos siginificativos do resultado seria o resultado da primeira soma e os 16 bits mais significativos são os bits do resultado da segunda operação.

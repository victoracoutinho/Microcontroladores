1. Projete o hardware necessário para o MSP430 controlar um motor DC de 12V e 4A. Utilize transistores bipolares de junção (TBJ) com Vbe = 0,7 V, beta = 100 e Vce(saturação) = 0,2 V. Além disso, considere que Vcc = 3 V para o MSP430, e que este não pode fornecer mais do que 10 mA por porta digital.

Usaria a Montagem de **Par Dalington**, com Rb de **4 kOhms** e o motor seria alimentado por **12,2 V**, considerando que Vce esteja em saturação

2. Projete o hardware necessário para o MSP430 controlar um motor DC de 10V e 1A. Utilize transistores bipolares de junção (TBJ) com Vbe = 0,7 V e beta = 120. Além disso, considere que Vcc = 3,5 V para o MSP430, e que este não pode fornecer mais do que 10 mA por porta digital.

Usar montagem de **Transistor Bipolar**, com Rb de **336 Ohms** (Ib fica **8,33 mA**).

3. Projete o hardware utilizado para controlar 6 LEDs utilizando charlieplexing. Apresente os pinos utilizados no MSP430 e os LEDs, nomeados L1-L6.
```
saida  P1.0|----[resistor]----------------------.-------.---------------.-------.
	   |					|	|		|	|
	   |				       [L1]   ----		|	|
	   |				       ----   [L2]		|	|
	   |					|	|		|	|
saida  P1.1|----[resistor]------.-------.-------|.......|      	       [L3]    ----
	   |			|	|			       ----    [L4]
	   |		      [L5]     ----				|	|
	   |		      ----     [L6]	 			|	|
	   |			|	|				|	|
entradaP1.2|----[resistor]------|-------|-------------------------------|-------|
	   |
	   |

```

4. Defina a função `void main(void){}` para controlar 6 LEDs de uma árvore de natal usando o hardware da questão anterior. Acenda os LEDs de forma que um ser humano veja todos acesos ao mesmo tempo.
```C

#include "io430.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  int i = 0;
  while (1){
    for (i = 0 ; i<6 ; i++){
      switch(i){
      case 0: 
        P1DIR = (BIT0|BIT1);
        P1OUT = BIT0;
        break;
      case 1: 
        P1DIR = (BIT0|BIT1);
        P1OUT = BIT1;
        break;
      case 2: 
        P1DIR = (BIT1|BIT2);
        P1OUT = BIT1;
        break;
      case 3: 
        P1DIR = (BIT1|BIT2);
        P1OUT = BIT2;
        break;
      case 4: 
        P1DIR = (BIT2|BIT0);
        P1OUT = BIT1;
        break;
      case 5: 
        P1DIR = (BIT2|BIT0);
        P1OUT = BIT0;
        break;
      } 
    }
  }

  return 0;
}

```

5. Defina a função `void main(void){}` para controlar 6 LEDs de uma árvore de natal usando o hardware da questão 3. Acenda os LEDs de forma que um ser humano veja os LEDs L1 e L2 acesos juntos por um tempo, depois os LEDs L3 e L4 juntos, e depois os LEDs L5 e L6 juntos.
```C

#include "io430.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  int i = 0,j = 0, jaux = 0;
  while (1){
    for (i = 0 ; i<3 ; i++){
      switch(i){
      case 0:
        for ( j = 0 ; j < 1000 ; j++){
          jaux = j%2;
          switch (jaux){
          case 0:
              P1DIR = (BIT0|BIT1);
              P1OUT = BIT0;
              break;
          case 1:
              P1DIR = (BIT0|BIT1);
              P1OUT = BIT1;
              break;
          }
        }
        break;
      case 1:
        for ( j = 0 ; j < 1000 ; j++){
          jaux = j%2;
          switch (jaux){
          case 0:
              P1DIR = (BIT1|BIT2);
              P1OUT = BIT1;
              break;
          case 1:
              P1DIR = (BIT1|BIT2);
              P1OUT = BIT2;
              break;
          }
        }
        break;
      case 2:
        for ( j = 0 ; j < 1000 ; j++){
          jaux = j%2;
          switch (jaux){
          case 0:
              P1DIR = (BIT2|BIT0);
              P1OUT = BIT2;
              break;
          case 1:
              P1DIR = (BIT2|BIT0);
              P1OUT = BIT0;
              break;
          }
        }
        break;
      } 
    }
  }

  return 0;
}

```

6. Defina a função `void EscreveDigito(volatile char dig);` que escreve um dos dígitos 0x0-0xF em um único display de 7 segmentos via porta P1, baseado na figura abaixo. Considere que em outra parte do código os pinos P1.0-P1.6 já foram configurados para corresponderem aos LEDs A-G, e que estes LEDs possuem resistores externos para limitar a corrente.

```
        ---  ==> A
       |   |
 F <== |   | ==> B
       |   |
        ---  ==> G
       |   |
 E <== |   | ==> C
       |   |
        ---  ==> D
```

7. Multiplexe 2 displays de 7 segmentos para apresentar a seguinte sequência em loop:
	00 - 11 - 22 - 33 - 44 - 55 - 66 - 77 - 88 - 99 - AA - BB - CC - DD - EE - FF







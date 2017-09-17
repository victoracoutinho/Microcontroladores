Data: 26/04/2017

Para todas as questões, utilize os LEDs e/ou os botões da placa Launchpad do MSP430.

1. Escreva um código em C que pisca os LEDs ininterruptamente.
```C

#include <msp430.h>
#define LEDS (BIT6+BIT0)

void atraso (volatile unsigned int i){
  int a = i;
  i = 0;
  while(i<50000){
    i++;
  }
  i = a;
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  //Código que pisca os leds initerruptamente
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = LEDS;
  //P1OUT |= 0xFF; // define pull-up os pinos de entrada. Para não ocorrer problemas.
  int i;
  while (1){
    P1OUT ^= LEDS;
    atraso(i);
  }
  return 0;
}

```

2. Escreva um código em C que pisca os LEDs ininterruptamente. No ciclo que pisca os LEDs, o tempo que os LEDs ficam ligados deve ser duas vezes maior do que o tempo que eles ficam desligados.
```C

#include <msp430.h>
#define LEDS (BIT6+BIT0)

void atraso (volatile unsigned int i){
  int a = i;
  i = 0;
  while(i<50000){
    i++;
  }
  i = a;
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  //Código que pisca os leds initerruptamente com delays diferentes
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = LEDS;
  P1OUT |= 0xFF; // define pull-up os pinos de entrada. Para não ocorrer problemas.
  int i;
  while (1){
    P1OUT ^= LEDS; //Desligado
    atraso(i);
    P1OUT ^= LEDS; //Ligado
    atraso(i);
    atraso(i);
  }
  return 0;
}

```

3. Escreva um código em C que acende os LEDs quando o botão é pressionado.
```C

#include <msp430g2553.h>
//#include <legacymsp430.h>
#define LEDS (BIT6+BIT0)
#define BOTAO BIT3

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  //Código que liga leds quando aperta o botão
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = LEDS;
  P1REN |= BOTAO;
  P1OUT |= BOTAO; // define pull-up os pinos de entrada. Para não ocorrer problemas.
  /*P1IES |= botao;
  P1IE |= botao;
  _BIS_GR(GIE);*/
  while(1){
    if ((P1IN&BOTAO) == 0){
      P1OUT |= LEDS; //LIGAR
    }else{
      P1OUT &= ~LEDS;
    }
  }
  return 0;
}
/*interrupt (PORT1_VECTOR) Interrupcao(void){
  while((P1IFG&botao)== 0){
    P1OUT |= LEDS; //LIGAR
  }
  P1OUT |= ~LEDS; //LIGAR
}*/

```

4. Escreva um código em C que pisca os LEDs ininterruptamente somente se o botão for pressionado.
```C

#include <msp430.h>
#define LEDS (BIT6+BIT0)
#define BOTAO BIT3

void atraso (volatile unsigned int i){
  int a = i;
  i = 0;
  while(i<50000){
    i++;
  }
  i = a;
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  //Código que pisca os leds initerruptamente quando aperta o botão
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = LEDS;
  P1REN |= BOTAO;
  P1OUT |= BOTAO; // define pull-up os pinos de entrada. Para não ocorrer problemas.
  int i;
  while(1){
    while ((P1IN&BOTAO) == 0){
      P1OUT ^= LEDS;
      atraso(i);
    }
    P1OUT &= ~LEDS;
  }
  return 0;
}

```

5. Escreva um código em C que acende os LEDs quando o botão é pressionado. Deixe o MSP430 em modo de baixo consumo, e habilite a interrupção do botão.
```C

```

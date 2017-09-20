1. Escreva uma função em C que faz o debounce de botões ligados à porta P1.
```C

#include "io430.h"
#include <intrinsics.h>
#define LEDS (BIT6+BIT0)
#define BOTAO BIT3
int ativou = 0;

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  //Código que os leds ligam com o botão. Usar Interrupção e baixo consumo.
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = LEDS;
  P1DIR &= ~BOTAO;
  P1REN |= BOTAO;
  P1OUT |= BOTAO; // define pull-up os pinos de entrada.
  P1IES |= BOTAO;
  P1IE |= BOTAO;
  //__bis_SR_register(GIE);
  __enable_interrupt();
  while(1){
    //if (ativou == 1)
      //P1OUT ^= LEDS; //LIGAR
  }

  return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void ISR_botao(void) {
  
  P1IFG &= ~BOTAO;// tira o aviso de interrupção
  volatile int contador = 0;
  while (contador <1000){ // Espera um tempo para estabilizar
    contador ++;
  }
  if ((P1IN&BOTAO)==0){
     P1OUT ^= LEDS; //LIGAR
  }
  P1IFG &= ~BOTAO;// tira o aviso de interrupção
  
}
```

2. Escreva um código em C que lê 9 botões multiplexados por 6 pinos, e pisca os LEDs da placa Launchpad de acordo com os botões. Por exemplo, se o primeiro botão é pressionado, os LEDs piscam uma vez; se o segundo botão é pressionado, os LEDs piscam duas vezes; e assim por diante. Se mais de um botão é pressionado, os LEDs não piscam.

```C

#include "msp430g2553.h"
#define multiplexa (BIT2|BIT1|BIT0)// (1 4 7 | 2 5 8 |3 6 9) - Seletor de faixa
                                   // Seleciona quando for 0 - zero
#define leitor (BIT5|BIT4|BIT3) // (1 2 3 | 4 5 6 | 6 8 9 ) - Leitor de faixa

# define LED BIT6

int main( void )
{
  int i=0, a;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  P1DIR = multiplexa;
  
  P1DIR &= ~leitor;
  P1REN |= leitor;
  P1OUT |= leitor; // define pull-up os pinos de entrada.
  
  //P1IN = 
  
  for ( i = 1 ; i<=3 ; i++ ){
    P1OUT &= ~(00000111); // Zera todas as entradas
    switch (i){
    case 1:
        P1OUT |= BIT2;    // Seta o linha 1 4 7
        if ((P1IN&leitor) == BIT5){
         pisca(1);
        }else if ((P1IN&leitor) == BIT4){
          pisca(4);
        }else if ((P1IN&leitor) == BIT3){
          pisca(7);
        }
        break;
    case 2:
        P1OUT |= BIT1;    // Seta o linha 2 5 8
        if ((P1IN&leitor) == BIT5){
          pisca(2);
        }else if ((P1IN&leitor) == BIT4){
          pisca(5);
        }else if ((P1IN&leitor) == BIT3){
          pisca(8);
        }
        break;
    case 3:
        P1OUT |= BIT0;    // Seta o linha 3 6 9
        if ((P1IN&leitor) == BIT5){
          pisca(3);
        }else if ((P1IN&leitor) == BIT4){
          pisca(6);
        }else if ((P1IN&leitor) == BIT3){
          pisca(9);
        }
        break;
    }
  }
  
  
  return 0;
}

int pisca ( int quantidade){
  //printf (" quant: %d \n", quantidade);
  int i;
  int max = 2*quantidade;
  for  (i = 0 ; i < max ; i++){
   P1OUT ^=LED; 
  }
  return 1;

}

```

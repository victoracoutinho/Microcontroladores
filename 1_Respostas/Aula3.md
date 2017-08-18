1. Dada uma variável `a` do tipo `char` (um byte), escreva os trechos de código em C para:

	(a) Somente setar o bit menos significativo de `a`.
	
	P1OUT = a | 0x01;
	
	(b) Somente setar dois bits de `a`: o menos significativo e o segundo menos significativo.
	
	P1OUT = a | 0x03;
  
	(c) Somente zerar o terceiro bit menos significativo de `a`.
  	
	P1OUT = a & 0x04;
  
	(d) Somente zerar o terceiro e o quarto bits menos significativo de `a`.
  	
	P1OUT = a & 0x0C;
  
	(e) Somente inverter o bit mais significativo de `a`.
  	
	P1OUT = a ^ 0x80;
  
	(f) Inverter o nibble mais significativo de `a`, e setar o nibble menos significativo de `a`. 
  	
	P1OUT = a ^ 0xF0; P1OUT = a & 0x0F;
  

2. Considerando a placa Launchpad do MSP430, escreva o código em C para piscar os dois LEDs ininterruptamente.

```C
	# include <msp430g2553.h>
	# define LEDS 0x03
	# define desligado 0x00
	void main(void){
		WDTCTL = WDTPW | WDTHOLD;
		P1DIR = LEDS;
		while (){
			P1OUT = LEDS; //liga os leds
			P1OUT = desligado; // desliga os leds
		}
	}
```

3. Considerando a placa Launchpad do MSP430, escreva o código em C para piscar duas vezes os dois LEDs sempre que o usuário pressionar o botão.
```C
	# include <msp430g2553.h>
	# define LEDS 0x03
	# define botao 0x04
	# define desligado 0x00
	void main(void){
		int contador = 0;
		WDTCTL = WDTPW | WDTHOLD;
		P1DIR = LEDS|botao;
		while (){
			if ( (P1IN & botao) != 0)
				while (contador < 2){
					P1OUT = LEDS; //liga os leds
					P1OUT = desligado; // desliga os leds
					contador ++;
				}
			} else {
				contador = 0;
			}
		}
	}
```
4. Considerando a placa Launchpad do MSP430, faça uma função em C que pisca os dois LEDs uma vez.

```C
	# include <msp430g2553.h>
	void pisca_dois_leds (){
		P1OUT |= LEDS; //liga os leds
		P1OUT &=~LEDS; // desliga os leds
	}
```

5. Reescreva o código da questão 2 usando a função da questão 4.

```C
	# include <msp430g2553.h>
	# define LEDS 0x03
	# define desligado 0x00
	void main(void){
		WDTCTL = WDTPW | WDTHOLD;
		P1DIR = LEDS;
		while (){
			pisca_dois_leds ();
		}
	}
```

6. Reescreva o código da questão 3 usando a função da questão 4.

```C
	# include <msp430g2553.h>
	# define LEDS 0x03
	# define botao 0x04
	# define desligado 0x00
	void main(void){
		int contador = 0;
		WDTCTL = WDTPW | WDTHOLD;
		P1DIR = LEDS|botao;
		while (){
			if ( (P1IN & botao) != 0)
				while (contador < 2){
					pisca_dois_leds();
					contador ++;
				}
			} else {
				contador = 0;
			}
		}
	}
```

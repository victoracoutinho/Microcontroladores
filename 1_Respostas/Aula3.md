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



3. Considerando a placa Launchpad do MSP430, escreva o código em C para piscar duas vezes os dois LEDs sempre que o usuário pressionar o botão.

4. Considerando a placa Launchpad do MSP430, faça uma função em C que pisca os dois LEDs uma vez.

5. Reescreva o código da questão 2 usando a função da questão 4.

6. Reescreva o código da questão 3 usando a função da questão 4.

1. Defina a função `void Atraso(volatile unsigned int x);` que fornece um atraso de `x` milissegundos. Utilize o Timer_A para a contagem de tempo, e assuma que o SMCLK já foi configurado para funcionar a 1 MHz. Esta função poderá ser utilizada diretamente nas outras questões desta prova.
```C

void Atraso(volatile unsigned int x){
        int aux = 0, i =0;
  	BCSCTL1 = CALBC1_1MHZ;		//MCLK e SMCLK @ 1MHz
	DCOCTL = CALDCO_1MHZ;		//MCLK e SMCLK @ 1MHz
        TACTL |= TACLR;
        TACTL = TASSEL_2 + ID_0 + MC_1; //TASSEL_2 = MCLK ; ID_0 = div por 1
                                        //MC_1 = modo pullup
        if(x<=65){
        TACCR0 = x*1000-1;              // contar 1000*x 
        while(TAIFG&TACTL == 0);
        }else{
          aux = x%65;     //quantas 65 o x tem
          for(i = 0; i < aux; i++){
            Atraso(65);
          }
          Atraso(x-aux*65);
        }
        TACTL = MC_0;
        
}

```

2. Pisque os LEDs da Launchpad numa frequência de 100 Hz.

```C
int main( void )
{
	//int a = 0;
        WDTCTL = WDTPW + WDTHOLD;	// Stop WDT
	
	BCSCTL1 = CALBC1_1MHZ;		//MCLK e SMCLK @ 1MHz
	DCOCTL = CALDCO_1MHZ;		//MCLK e SMCLK @ 1MHz
	P1OUT &= ~LED;
	P1DIR |= LED;
	while(1)
	{
		P1OUT ^= LED;
		Atraso(10); //Atraso de 100Hz
	}
	return 0;
}
```

3. Pisque os LEDs da Launchpad numa frequência de 20 Hz.

```C
int main( void )
{
	//int a = 0;
        WDTCTL = WDTPW + WDTHOLD;	// Stop WDT
	
	BCSCTL1 = CALBC1_1MHZ;		//MCLK e SMCLK @ 1MHz
	DCOCTL = CALDCO_1MHZ;		//MCLK e SMCLK @ 1MHz
	P1OUT &= ~LED;
	P1DIR |= LED;
	while(1)
	{
		P1OUT ^= LED;
		Atraso(50); //Atraso de 100Hz
	}
	return 0;
}
```

4. Pisque os LEDs da Launchpad numa frequência de 1 Hz.

```C
int main( void )
{
	//int a = 0;
        WDTCTL = WDTPW + WDTHOLD;	// Stop WDT
	
	BCSCTL1 = CALBC1_1MHZ;		//MCLK e SMCLK @ 1MHz
	DCOCTL = CALDCO_1MHZ;		//MCLK e SMCLK @ 1MHz
	P1OUT &= ~LED;
	P1DIR |= LED;
	while(1)
	{
		P1OUT ^= LED;
		Atraso(1000); //Atraso de 100Hz
	}
	return 0;
}
```

5. Pisque os LEDs da Launchpad numa frequência de 0,5 Hz.

```C
int main( void )
{
	//int a = 0;
        WDTCTL = WDTPW + WDTHOLD;	// Stop WDT
	
	BCSCTL1 = CALBC1_1MHZ;		//MCLK e SMCLK @ 1MHz
	DCOCTL = CALDCO_1MHZ;		//MCLK e SMCLK @ 1MHz
	P1OUT &= ~LED;
	P1DIR |= LED;
	while(1)
	{
		P1OUT ^= LED;
		Atraso(2000); //Atraso de 100Hz
	}
	return 0;
}
```

6. Repita as questões 2 a 5 usando a interrupção do Timer A para acender ou apagar os LEDs.


```C

```


```C

```


7. Defina a função `void paralelo_para_serial(void);` que lê o byte de entrada via porta P1 e transmite os bits serialmente via pino P2.0. Comece com um bit em nivel alto, depois os bits na ordem P1.0 - P1.1 - … - P1.7 e termine com um bit em nível baixo. Considere um período de 1 ms entre os bits.

8. Faça o programa completo que lê um byte de entrada serialmente via pino P2.0 e transmite este byte via porta P1. O sinal serial começa com um bit em nivel alto, depois os bits na ordem 0-7 e termina com um bit em nível baixo. Os pinos P1.0-P1.7 deverão corresponder aos bits 0-7, respectivamente. Considere um período de 1 ms entre os bits.

9. Defina a função `void ConfigPWM(volatile unsigned int freqs, volatile unsigned char ciclo_de_trabalho);` para configurar e ligar o Timer_A em modo de comparação. Considere que o pino P1.6 já foi anteriormente configurado como saída do canal 1 de comparação do Timer_A, que somente os valores {100, 200, 300, …, 1000} Hz são válidos para a frequência, que somente os valores {0, 25, 50, 75, 100} % são válidos para o ciclo de trabalho, e que o sinal de clock SMCLK do MSP430 está operando a 1 MHz.


















/***********************************************************************
 * Autores:
 * Davi Antônio da Silva Santos
 * Victor Aguiar Coutinho
***********************************************************************/

#include <msp430.h>
#include <legacymsp430.h>

/* O buzzer está em P2.4 */
#define PINO_PWM BIT4
#define TEMPO_SOM_MS 333
/* LED verde está em -P1.6- P2.1 */
#define LED_VERDE BIT1
/* LED vermelho está em P2.0 */
#define LED_VERMELHO BIT0

/* Definindo os botões P1.3, P1.4, P1.5 e P1.7 */
#define BOTAO_01 BIT3
#define BOTAO_02 BIT4
#define BOTAO_03 BIT5
#define BOTAO_04 BIT7
#define BOTOES (BOTAO_01 + BOTAO_02)

/* Definição das constantes de frequência */
/* Frequência do botão 01 é 1,04652 kHz, contar 955 us */
/* Frequência do botão 02 é 1,31852 kHz, contar 758 us */
/* Frequência do botão 03 é 1,56800 kHz, contar 638 us */
/* Frequência do botão 04 é 2,09300 kHz, contar 478 us */
#define FREQUENCIA_01_US 955
#define FREQUENCIA_02_US 758
#define FREQUENCIA_03_US 638
#define FREQUENCIA_04_US 478

/***********************************************************************
 * Protótipos das Funções
 **********************************************************************/

unsigned int configura_lcg(void);
void delay(volatile unsigned int x);
unsigned long int lcg(volatile unsigned long int x0, volatile unsigned int modo);
int ler_portas(void);
void pisca2x(char LED_que_pisca);
void toca_buzzer(volatile unsigned int nLED);
void jogo01(void);
 
/***********************************************************************
 * Início do Código
 **********************************************************************/

int main(void)
{	
	long int x0;
	
	/* Parar o watchdog timer */
	WDTCTL = WDTPW + WDTHOLD;
	
	/* Configurar o pino do PWM como saída da comparação do Timer A1
	 * no canal de comparação 2 */
	P2DIR |= PINO_PWM;
	P2SEL |= PINO_PWM;
	P2SEL2 &= ~PINO_PWM;
	
	/* Configurar os LEDs de saída */
	P2DIR |= LED_VERDE;
	P2DIR |= LED_VERMELHO;
	P2OUT &= ~(LED_VERDE + LED_VERMELHO);
	
	/*Configurar botões como entrada e pulldown*/
	P1DIR &= ~BOTOES;
	P1REN |= BOTOES;
	P1OUT &= ~BOTOES;
	
	/* Congigurar interrupção nos pinos */
	/* Borda de subida pois é pull-down */
	P1IES &= ~BOTOES;
	P1IE |= BOTOES;
	_BIS_SR(GIE);
	
	static int seq_gen[32] = {0};
	static int seq_read[32] = {0};
	unsigned int level = 1;

	unsigned int i = 0, j = 0, cont = 0;
	int fail = 0;
	int continuar_jogo = 0;

	/*Apresenta as frequencias de sons utilizadas*/
	for (cont = 1; cont <= 4; cont++){
		toca_buzzer(cont);
	}
	
	delay(500);
	
	/* Colocar o LCG em modo de configuração */
	x0 = lcg(0, 0);

	/* loop que correrá enquanto o sistema estiver ligado */
	jogo01();

	_BIS_SR(LPM0_bits);
	
	
	return 0;
}

/***********************************************************************
 * Funções
 **********************************************************************/
/* Gerador de números aleatórios (LCG) se modo = 0, todo xn sairá da
 * conversão do ADC10. Assim que modo !=  0 todo xn sairá do LCG */

void jogo01(void)
{
	static long int x0;
	static int seq_gen[32] = {0};
	static int seq_read[32] = {0};
	static unsigned int level = 1;

	static unsigned int i = 0, j = 0, cont = 0;
	static int fail = 0;
	static int continuar_jogo = 0;
	
	/* Desativar interrupção nos pinos */
	P1IE &= ~BOTOES;
	
	continuar_jogo = 0;
	/* Caso uma derrota não resete o programa */
	/*fail = 0;*/
	/* Checar se nível máximo */
	if(level >= 32){
		_BIS_SR(LPM4_bits);
	}
	
	delay(500);
	
	/* Gerar o número aleatório*/
	seq_gen[i] = (lcg(0, 1) % 4) + 1;
	
	/* Piscar os LEDs na sequencia */
	for(j = 0; j < level; j++){
		toca_buzzer(seq_gen[j]);
	}
	
	/* Ler os botões */
	delay(10);
	
	for(j = 0; j < level; j++){
		/* Ler o serial, para debug */
		delay(100);

		seq_read[j] = ler_portas();
	}

	/* Comparar as sequencias */
	for(j = 0; j < level; j++){
		if(seq_gen[j] != seq_read[j]) fail = 1;
	}

	delay(1000);
	/* Checar se passou */
	if(fail != 0){
		pisca2x(LED_VERMELHO);
		level = 1;
		fail = 0;
	}else{
		pisca2x(LED_VERDE);
		level++;
		i++;
	}
	delay(500); 

	/*Deseja continuar?*/
	P2OUT |= LED_VERMELHO;
	P2OUT |= LED_VERDE;
	
	/* Ativar interrupção nos pinos */
	P1IE |= BOTOES;
}

unsigned long int lcg(volatile unsigned long int x0, volatile unsigned int modo)
{
	static long unsigned int xn;
	if(modo){
		xn = (((1103515245*xn + 12345) & 0x7FFFFFFF));
	} else{
		xn = configura_lcg();
	}
	return xn;
}

unsigned int configura_lcg(void)
{
	/* Usar Vcc e Vss como referência;
	 * Usar como tempo de sample e hold 4 clocks;
	 * Habilitar o ADC10 */
	ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON;
	
	/* Selecionar o sensor de temperatura interno como entrada;
	 * A conversão será requisitada quando receber ENC e ADC10SC;
	 * O clock não será dividido;
	 * O clock utilizado é o SMCLK;
	 * Apenas um canal de conversão AD será usado */
	ADC10CTL1 = INCH_10 + SHS_0 + ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_0;
	
	/* Comecar uma conversao */
	ADC10CTL0 |= ENC + ADC10SC;
	
	/* Espera a conversao ficar pronta */
	while((ADC10CTL0 & ADC10IFG)==0);
	
	return ADC10MEM;
}

/*FUNÇÃO PARA LER BOTÕES*/
int ler_portas(void)
{
	while(1){
		/* Aviso visual de modo leitura */
		P2OUT |= LED_VERDE;
		
		int v1 = P1IN & BOTAO_01;
		int v2 = P1IN & BOTAO_02;
		int v3 = P1IN & BOTAO_03;
		int v4 = P1IN & BOTAO_04;

		delay(10);
		
		/*Considerando PULL DOWN => botão ligado no Vcc */
		if(v1 && !(v2) && !(v3) && !(v4)){
			P2OUT &= ~LED_VERDE;
			while(P1IN & BOTAO_01);
			return 1;
		}
		else if(!(v1) && v2 && !(v3) && !(v4)){
			P2OUT&= ~LED_VERDE;
			while(P1IN & BOTAO_02);
			return 2;
		}
		else if(!(v1) && !(v2) &&v3 && !(v4)){
			P2OUT&= ~LED_VERDE;
			while(P1IN & BOTAO_03);
			return 3;
		}
		else if(!(v1) && !(v2) && !(v3) &&v4){
			P2OUT &= ~LED_VERDE;
			while(P1IN & BOTAO_04);
			return 4;
		}
		
		delay(10);
	}
}

/*FUNÇÃO PARA PISCAR LEDS DUAS VEZES*/
void pisca2x( char LED_que_pisca){
	P2OUT &= ~LED_que_pisca;
	delay(100);
	P2OUT |= LED_que_pisca;     
	delay(100);
	P2OUT &= ~LED_que_pisca;
	delay(100);
	P2OUT |= LED_que_pisca;     
	delay(100);
	P2OUT &= ~LED_que_pisca;
	delay(1000);
}

void delay(volatile unsigned int x)
{
	int i;
	
	for(i = 0; i < 1000; i++){ 
	/* 1000 contagens de 1 us = 1 ms, mas conta o zero (up) */
	TA0CCR0 = x - 1;
	
	/* Usar SMCLK, não dividi-lo e contar em modo up */
	TA0CTL = TASSEL_2 + ID_0 + MC_1;
	
	/* Espera terminar contagem */
	while((TA0CTL & TAIFG) == 0);
	}
	
	/* Parar e zerar o timer */
	TA0CTL = MC_0 + TACLR;
	TA0CTL &= ~TAIFG;
}

/* Configurar timer do PWM */
void toca_buzzer(volatile unsigned int nLED)
{
	/*Configurar o pino do PWM como saída da comparação do Timer A1
	 * no canal de comparação 2*/
	
	/* Frequência do botão 01 é 1,04652 kHz, contar 955 us */
	/* Frequência do botão 02 é 1,31852 kHz, contar 758 us */
	/* Frequência do botão 03 é 1,56800 kHz, contar 638 us */
	/* Frequência do botão 04 é 2,09300 kHz, contar 478 us */
	
	/* Reiniciar todos os registradores que serão usados na
	 * comparação e parar o timer */
	TA1CTL = 0;
	TA1CCR0 = 0;
	TA1CCR2 = 0;
	
	/* Configurar o modo de comparação do pino antes de cada
	 * definição de frequência */
	P2SEL |= PINO_PWM;
	P2SEL2 &= ~PINO_PWM;
	
	switch(nLED){
		case 1:
		TA1CCR0 |= FREQUENCIA_01_US - 1;
		TA1CCR2 |= ((FREQUENCIA_01_US - 1) >> 1);
		/* Colocar no modo de contagem up, para facilitar o
		 * trabalho com PWM, pois é só subir o valor do reg de
		 * comparação */
		TA1CCTL2 = OUTMOD_7;
		TA1CTL = TASSEL_2 + ID_0 + MC_1;
	break;

	case 2:
		TA1CCR0 |= FREQUENCIA_02_US - 1;
		TA1CCR2 |= ((FREQUENCIA_02_US - 1) >> 1);
		/* Colocar no modo de contagem up, para facilitar o
		 * trabalho com PWM, pois é só subir o valor do reg de
		 * comparação */
		TA1CCTL2 = OUTMOD_7;
		TA1CTL = TASSEL_2 + ID_0 + MC_1;
	break;

	case 3:
		TA1CCR0 |= FREQUENCIA_03_US - 1;
		TA1CCR2 |= ((FREQUENCIA_03_US - 1) >> 1);
		/* Colocar no modo de contagem up, para facilitar o
		 * trabalho com PWM, pois é só subir o valor do reg de
		 * comparação */
		TA1CCTL2 = OUTMOD_7;
		TA1CTL = TASSEL_2 + ID_0 + MC_1;
	break;

	case 4:
		TA1CCR0 |= FREQUENCIA_04_US - 1;
		TA1CCR2 |= ((FREQUENCIA_04_US - 1) >> 1);
		/* Colocar no modo de contagem up, para facilitar o
		 * trabalho com PWM, pois é só subir o valor do reg de
		 * comparação */
		TA1CCTL2 = OUTMOD_7;
		TA1CTL = TASSEL_2 + ID_0 + MC_1;
	break;

	default:
		TA1CTL = 0;
	}
	
	/* Pequeno delay para estabilizar */
	delay(TEMPO_SOM_MS);
	
	/* Parar o timer */
	TA1CTL = 0;
	
	/* Configurar como saída e colocar em nível baixo para
	 * desligar o buzzer */
	P2SEL &= ~PINO_PWM;
	P2SEL2 &= ~PINO_PWM;
	P2OUT &= ~PINO_PWM;
	
	/* Limpar os registradores da comparação*/
	TA1CCR0 = 0;
	TA1CCR2 = 0;
	
	delay(TEMPO_SOM_MS);
}

/***********************************************************************
 * Rotinas de Interrupção
 **********************************************************************/
/* A interrupção faz o debounce e religa o MSP */
interrupt(PORT1_VECTOR) Interrupcao_botoes(void)
{
	P1IFG = 0;
	
	/* Desativar interrupção nos pinos */
	P1IE &= ~BOTOES;
	
	char continuar_jogo = 0;
	
	/* Zerar o vetor de interrupções para evitar que a interrupção
	 * seja chamada continuamente */	
			continuar_jogo = ler_portas();
			
			if(continuar_jogo == 1){
				P2OUT &= ~LED_VERMELHO;
				P2OUT &= ~LED_VERDE;
				delay(100);
				P2OUT |= LED_VERDE;
				delay(100);
				P2OUT &= ~LED_VERDE;
				delay(100);
				jogo01();
			}else if(continuar_jogo == 2){
				P2OUT &= ~LED_VERDE;
				P2OUT &= ~LED_VERMELHO;
				delay(100);
				P2OUT |= LED_VERMELHO;
				delay(100);
				P2OUT &= ~LED_VERMELHO;
				delay(100);
				/* Não quis continuar, desligar */
				_BIS_SR(LPM4_bits);
			}else{
				continuar_jogo = 0;
			}
			
			/*asm(
			"bic 0x0010, 0(R1) \n"
			"reti"
			);*/
			
}

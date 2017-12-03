/*
Controlar uma matriz de LEDs 8x8 em um MAX7219, usando a USCI/USI de um
MSP430G2553 em modo SPI, sendo que o SPI está em modo 'master'.

Antes de iniciar a SPI, colocar um delay para o CI acompanhar.

* SPI_SIMO = UCA0SIMO = P1.2	(P1SEL |= BIT2, P1SEL2 |= BIT2)
* SPI_CLK  = UCA0CLK  = P1.4	(P1SEL |= BIT4, P1SEL2 |= BIT4)

* Procedimento para ligar a matriz:
* Configurar clock do sistema em 1 MHz
DCOCTL = CALDCO_1MHZ;
BCSCTL1 = CALBC1_1MHZ;

* Configurar as portas usadas pela matriz como saída por segurança,
pois SPI_SIMO e SPI_CLK já tem a direção escolhida pelas configurações da USCI.
P1DIR |= SPI_SIMO + SPI_CLK + PINO_LOAD;

* Para o MAX7219, os dados entram nas bordas de subida do clock, 
independentemente do estado do LOAD.
P1OUT &= ~(PINO_LOAD);

* Inicializar a comunicação SPI através da USCI. Sempre colocar um delay antes
e depois para evitar problemas ao conectar o MSP na alimentação e para dar tempo
do MAX7219 entender que a comunicação iniciou.
__delay_cycles(1000);
inicializar_SPI();
__delay_cycles(1000);

* Definir a intebsidade que quer iniciar a matriz e limpá-la.
configura_matriz(0x00);
limpa_matriz();

* A macro LIGA_MATRIZ torna a matriz disponível para uso.
*/

#include <msp430.h>
#include "graficos_matriz_8x8.h"

/* Configurar os pinos aos quais o CI será conectado */
/* Para o MSP430G2553:
* SPI_SIMO = UCA0SIMO = P1.2	(P1SEL |= BIT2, P1SEL2 |= BIT2)
* SPI_CLK  = UCA0CLK  = P1.4	(P1SEL |= BIT4, P1SEL2 |= BIT4) */
#define SPI_SIMO	BIT2
#define SPI_CLK		BIT4
/* Este é um pino comum, que pode ser conectado a qualquer porta em modo IO. */
#define PINO_LOAD	BIT0 /* P1.0 */

/* Configurar os endereços dos registradores do CI utilizado, tabela 2 do
datasheet*/
#define MAX_NOOP	0x00
#define MAX_DIGIT0	0x01
#define MAX_DIGIT1	0x02
#define MAX_DIGIT2	0x03
#define MAX_DIGIT3	0x04
#define MAX_DIGIT4	0x05
#define MAX_DIGIT5	0x06
#define MAX_DIGIT6	0x07
#define MAX_DIGIT7	0x08
#define MAX_DECODEMODE	0x09
#define MAX_INTENSITY	0x0A
#define MAX_SCANLIMIT	0x0B
#define MAX_SHUTDOWN	0x0C
#define MAX_DISPLAYTEST	0x0F

/* Outras constantes */
#define DELAY_STRING_DIR_ESQ	30000
#define DELAY_FADE_IN		20000
#define DELAY_FADE_OUT		20000
#define DELAY_STRING		100000
#define DELAY_CHAR_ESQ		30000
#define DELAY_CHAR_DIR		30000
#define DELAY_STRING_ESQ	500000

/* Outras macros */
#define LIGA_MATRIZ envia_dados_max_SPI(MAX_SHUTDOWN, 1);
#define DESLIGA_MATRIZ envia_dados_max_SPI(MAX_SHUTDOWN, 0);
#define INTENSIDADE_MAXIMA envia_dados_max_SPI(MAX_INTENSITY, 0x0F);
#define INTENSIDADE_MINIMA envia_dados_max_SPI(MAX_INTENSITY, 0x00);

/*******************************************************************************
Protótipos das funções
*******************************************************************************/
/* Funções básicas */
void inicializar_SPI(void);
void envia_dados_max_SPI(unsigned char endereco, unsigned char dados);
void configura_matriz(unsigned char intensidade);
void limpa_matriz(void);
void mostra_char(volatile unsigned char simbolo);
void mostra_string(unsigned char * string);

/* Efeitos especiais */
void mostra_char_esq(unsigned char simbolo);
void mostra_char_dir(unsigned char simbolo);
void mostra_char_dir_esq(unsigned char simbolo);
void mostra_string_esq(unsigned char * string);
void mostra_string_dir_esq(unsigned char * string);
void mostra_string_dir_esq_v2(unsigned char * string);
void mostra_char_fade_in(unsigned char simbolo, unsigned int intensidade_max);
void limpa_matriz_fade_out(unsigned char intensidade_atual);
void mostra_string_fade_inout(unsigned char * string, unsigned char intensidade);

/* Ligando pela primeira vez, essa função será suficiente */
void liga_1a_vez(void);

/******************************************************************************
Funções
******************************************************************************/

/* Os 16 bits serão enviados para o MAX7219 no formato:
* XXXX AAAA DDDD DDDD ('don´t care', endereço, dados).
* Em seguida, o MAX7219 irá executar o comando quando houver borda de subida do
pino de LOAD. */
void envia_dados_max_SPI(unsigned char endereco, unsigned char dados)
{
	/* Enviar os 4 bits do endereço como um byte e esperar terminar o
	envio. */
	UCA0TXBUF = endereco & 0x0F;
	while (UCA0STAT & UCBUSY);
	
	/* Enviar os dados e esperar terminar o envio */
	UCA0TXBUF = dados;
	while (UCA0STAT & UCBUSY);
	
	/* O MAX7219 guarda os dados recebidos na borda de subida do pino LOAD.
	Deixá-lo em nível baixo para a próxima chamada ocorrer corretamente. */
	P1OUT |= PINO_LOAD;
	P1OUT &= ~(PINO_LOAD);
}

/* Habilitar a comunicação SPI */
void inicializar_SPI()
{
	/* Parar a comunicação colocando a USCI em modo reset, para que possa
	* ser configurada. */
	UCA0CTL1 |= UCSWRST;
	
	/* Para comunicar conforme descrito no datasheet do MAX7219:
	* Os dados são adquiridos na primeira borda de clock do sinal utilizado
	pelo módulo e são atualizados na borda seguinte;
	* O MSB é enviado primeiro;
	* O MSP é o mestre (gera o clock), e o MAX7219, o escravo;
	* Operação do módulo em modo síncrono, comunicação por SPI.*/
	UCA0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC;
	
	/* O clock utilizado pelo módulo será o SMCLK */
	UCA0CTL1 |= UCSSEL_2;
	
	/* A comunicação ocorrerá com o mesmo clock de SMCLK. Divisão poderia
	ser até 2^16. */
	UCA0BR0 |= 0x01;
	UCA0BR1 = 0;
	
	/* Configurar os pinos SIMO (saída de dados do MSP) e CLK. */
	P1SEL |= SPI_SIMO + SPI_CLK;
	P1SEL2 |= SPI_SIMO + SPI_CLK;
	
	/* O módulo pode comunicar-se, sair do estado reset. */
	UCA0CTL1 &= ~UCSWRST;
}

void configura_matriz(unsigned char intensidade)
{	
	/*if(intensidade > 0x0F) intensidade = 0x0F;*/
	
	/* O CI deve ser configurado para trabalhar com uma matriz de 8x8:
	* limpar o registrador de deslocamento, ordenado que não se execute
	nennhuma operação;
	* Habilitar todas as 8 colunas (dígitos caso display de 7 segmentos como
	no datasheet.
	* Configurar a intensidadde do display, 16 disponíveis (0x00a 0x0F).
	* Desativar a decodificação de BCD, pois trabalharemos com uma matriz
	e enviaremos em binário os LEDs de cada coluna que devem ser acesos.
	*/
	envia_dados_max_SPI(MAX_NOOP, 0x00);
	envia_dados_max_SPI(MAX_NOOP, 0x00);
	
	envia_dados_max_SPI(MAX_SCANLIMIT, 0x07);
	envia_dados_max_SPI(MAX_INTENSITY, intensidade);
	envia_dados_max_SPI(MAX_DECODEMODE, 0);
}

void limpa_matriz(void)
{	
	
	/*envia_dados_max_SPI(MAX_DIGIT0, 0);
	envia_dados_max_SPI(MAX_DIGIT1, 0);
	envia_dados_max_SPI(MAX_DIGIT2, 0);
	envia_dados_max_SPI(MAX_DIGIT3, 0);
	envia_dados_max_SPI(MAX_DIGIT4, 0);
	envia_dados_max_SPI(MAX_DIGIT5, 0);
	envia_dados_max_SPI(MAX_DIGIT6, 0);
	envia_dados_max_SPI(MAX_DIGIT7, 0);*/
	
	asm(
	"clr.w R14 \n"
	"mov.b	#1, R15 \n"
	"call	#envia_dados_max_SPI \n" 
	"mov.b	#2, R15 \n"
	"call	#envia_dados_max_SPI \n" 
	"mov.b	#3, R15	\n"
	"call	#envia_dados_max_SPI \n"
	"mov.b	#4, R15	\n"
	"call	#envia_dados_max_SPI \n"
	"mov.b	#5, R15 \n"
	"call	#envia_dados_max_SPI \n"
	"mov.b	#6, R15	\n"
	"call	#envia_dados_max_SPI \n" 
	"mov.b	#7, R15	\n"
	"call	#envia_dados_max_SPI \n" 
	"mov.b	#8, R15	\n"
	"call	#envia_dados_max_SPI \n"
	"pop.w R4 \n"
	"ret"
	);
}

void mostra_char(volatile unsigned char simbolo)
{
	unsigned int i;
	
	for(i = 0; i < 8; i++)
		envia_dados_max_SPI(MAX_DIGIT0 + i, fonte_8x8[(unsigned int) \
		(simbolo*8) + i]);
}

void mostra_string(unsigned char * string)
{
	unsigned int i = 0;
	while(string[i] != '\0'){
		mostra_char(string[i]);
		i++;
		__delay_cycles(DELAY_STRING);
	}
}

/* Efeitos especiais */
void mostra_char_esq(unsigned char simbolo)
{
	unsigned int i, j;
	
	for(j = 0; j <= 8; j++){ 
		for(i = 0; i < 8; i++)
			envia_dados_max_SPI(MAX_DIGIT0 + i, fonte_8x8[(unsigned
			int)(simbolo*8) + i] << j);
		__delay_cycles(DELAY_CHAR_ESQ);
	}
}

void mostra_char_dir(unsigned char simbolo)
{
	unsigned int i, j;
	
	for(j = 0; j <= 8; j++){ 
		for(i = 0; i < 8; i++)
			envia_dados_max_SPI(MAX_DIGIT0 + i, fonte_8x8[(unsigned
			int)(simbolo*8) + i] >> (8-j));
		__delay_cycles(DELAY_CHAR_DIR);
	}
}

void mostra_char_dir_esq(unsigned char simbolo)
{
	mostra_char_dir(simbolo);
	mostra_char_esq(simbolo);
}

void mostra_string_dir_esq(unsigned char * string)
{
	unsigned int i=0, j, k;
	
	/*Enquanto não achar a última posição*/
	
	while(string[i] != '\0'){
		for(j = 0; j <= 8; j++){ 
			/* Para o primeiro caractere, fazer apenas aparecer
			da direita para a esquerda*/
			if(i == 0){
				for(k = 0; k < 8; k++){
					envia_dados_max_SPI(MAX_DIGIT0 + k, 
					fonte_8x8[(unsigned int)(string[i]*8)
					 + k] >> (8-j));
					}
			/* Para as demais posições, será enviado o caractere
			concatenado com o próximo, de modo que quando o 1o
			esteja no meio da tela o 2o comece a aparecer na mesma
			proporção. */
			}else if (i > 0) {
				for(k = 0; k < 8; k++){
					envia_dados_max_SPI(MAX_DIGIT0 + k, 
					(fonte_8x8[(unsigned int)(string[i-1]*8)
					 + k] << (j+1)) | (fonte_8x8[(unsigned
					int)(string[i]*8) + k] >> (8 - j)));
					}
				}
				__delay_cycles(50000);
		}
		i++;
	}
	mostra_char_esq(string[i-1]);
}


void mostra_string_dir_esq_v2(unsigned char * string)
{
	unsigned int i=0, j, k;
	
	/*Enquanto não achar a última posição*/
	
	while(string[i] != '\0'){
		/* Para o primeiro caractere, fazer apenas aparecer da direita 
		para a esquerda*/
		if(i == 0){
			mostra_char_dir(string[0]);
		}else if(i > 0){
			for(j = 0; j <= 8; j++){ 
				/* Para as demais posições, será enviado o 
				caractere concatenado com o próximo, de modo que
				quando o 1o esteja no meio da tela o 2o comece
				a aparecer na mesma proporção. */
				for(k = 0; k < 8; k++){
					envia_dados_max_SPI(MAX_DIGIT0
					+ k, (fonte_8x8[(unsigned int)
					(string[i-1]*8) + k] << (j+1))| 
					(fonte_8x8[(unsigned int)
					(string[i]*8) + k] >> (8 - j)));
					}
				__delay_cycles(DELAY_STRING_DIR_ESQ);
			}
		}
		i++;
	}
	mostra_char_esq(string[i-1]);
}


void mostra_string_esq(unsigned char * string)
{
	unsigned int i = 0;
	
	while(string[i] != '\0'){
		mostra_char_esq(string[i]);
		i++;
		__delay_cycles(DELAY_STRING_ESQ);
	}
}

void mostra_char_fade_in(unsigned char simbolo, unsigned int intensidade_max)
{
	unsigned char i;
	
	limpa_matriz();
	envia_dados_max_SPI(MAX_INTENSITY, 0x00);
	mostra_char(simbolo);
	
	for(i = 0x00; i < intensidade_max; i++){
		envia_dados_max_SPI(MAX_INTENSITY, i);
		__delay_cycles(DELAY_FADE_IN);
	}
}

void limpa_matriz_fade_out(unsigned char intensidade_atual)
{
	while((intensidade_atual >= 0x00) && (intensidade_atual <= 0x0F)){
		envia_dados_max_SPI(MAX_INTENSITY, intensidade_atual);
		intensidade_atual--;
		__delay_cycles(DELAY_FADE_OUT);
	}
	limpa_matriz();
}

void mostra_string_fade_inout(unsigned char * string, unsigned char intensidade)
{
	unsigned int i = 0;
	
	while(string[i] != '\0'){
		mostra_char_fade_in(string[i], intensidade);
		limpa_matriz_fade_out(intensidade);
		i++;
	}
	
	limpa_matriz();
	envia_dados_max_SPI(MAX_INTENSITY, intensidade);
}

void liga_1a_vez(void)
{
	/* Configurar as portas usadas pela matriz como saída por segurança,
	pois SPI_SIMO e SPI_CLK já tem a direção escolhida pelas configurações
	da USCI. */
	P1DIR |= SPI_SIMO + SPI_CLK + PINO_LOAD;
	
	/* Para o MAX7219, os dados entram nas bordas de subida do clock,
	independentemente do estado do LOAD*/
	P1OUT &= ~(PINO_LOAD);

	/* Inicializar a comunicação SPI através da USCI */
	__delay_cycles(1000);
	inicializar_SPI();
	__delay_cycles(1000);
	
	configura_matriz(0x00);
	limpa_matriz();
	
	/* Pode ligar o display */
	LIGA_MATRIZ;
}

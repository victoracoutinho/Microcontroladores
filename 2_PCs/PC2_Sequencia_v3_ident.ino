/*
 * Autores:
 * Davi Antônio da Silva Santos
 * Victor Coutinho Aguiar
*/

#include <msp430.h>

#define PPWM P2_4
#define TSOMMS 333
#define LEDverde P1_6
#define LEDvermelho P2_0

/*FUNÇÃO PARA LER OS BOTÕES*/
int ler_portas(void)
{
	Serial.println("ler_portas");

	while(1){
	/* Aviso visual de modo leitura */
	digitalWrite(LEDverde, HIGH);

	int v1 = digitalRead(P1_3);
	int v2 = digitalRead(P1_4);
	int v3 = digitalRead(P1_5);
	int v4 = digitalRead(P1_7);

	delay(100);

	/*Considerando PULL DOWN => botão ligado no Vcc */
	if(v1 && !(v2) && !(v3) && !(v4)){
	digitalWrite(LEDverde, LOW);
	return 1;
	}
	else if(!(v1) && v2 && !(v3) && !(v4)){
	digitalWrite(LEDverde, LOW);
	return 2;
	}
	else if(!(v1) && !(v2) &&v3 && !(v4)){
	digitalWrite(LEDverde, LOW);
	return 3;
	}
	else if(!(v1) && !(v2) && !(v3) &&v4){
	digitalWrite(LEDverde, LOW);
	return 4;
	}
	delay(10);
	}
}

/*FUNÇÃO QUE TOCARÁ O BUZER EM FREQUENCIAS DETERMINADAS*/
void toca_buzzer(int nLED)
{
	float frequencias[4] = {4*261.63,4*329.63,4*392.00,4*523.25};
	switch(nLED){
	case 1:
		tone(PPWM, frequencias[nLED-1], TSOMMS);
		delay(TSOMMS);
		noTone(PPWM);
		delay(TSOMMS);
	break;

	case 2:
		tone(PPWM, frequencias[nLED-1], TSOMMS);
		delay(TSOMMS);
		noTone(PPWM);
		delay(TSOMMS);
	break;

	case 3:
		tone(PPWM, frequencias[nLED-1], TSOMMS);
		delay(TSOMMS);
		noTone(PPWM);
		delay(TSOMMS);
	break;

	case 4:
		tone(PPWM, frequencias[nLED-1], TSOMMS);
		delay(TSOMMS);
		noTone(PPWM);
		delay(TSOMMS);
	break;

	default:
		Serial.print("Escrita invalida no pino: ");
		Serial.print(nLED);
		Serial.println("");
	}
}

/*FUNÇÃO PARA PISCAR LEDS DUAS VEZES*/
void pisca2x( int LED_que_pisca){
	digitalWrite(LED_que_pisca, LOW);
	delay(100);
	digitalWrite(LED_que_pisca, HIGH);     
	delay(100);
	digitalWrite(LED_que_pisca, LOW);
	delay(100);
	digitalWrite(LED_que_pisca, HIGH);     
	delay(100);
	digitalWrite(LED_que_pisca, LOW);
	delay(1000);
}

/*FUNÇÃO DO SETUP DO PINOS*/
void setup()
{
	/*Definindo pinos como entrada e pulldown*/
	pinMode(P1_3, INPUT_PULLDOWN);
	pinMode(P1_4, INPUT_PULLDOWN);
	pinMode(P1_5, INPUT_PULLDOWN);
	pinMode(P1_7, INPUT_PULLDOWN);
	/*Definindo pinos como saída*/
	pinMode(PPWM, OUTPUT);
	pinMode(LEDverde, OUTPUT);
	pinMode(LEDvermelho, OUTPUT);
	/* Começar todos os LEDs em nivel baixo */
	digitalWrite(PPWM, LOW); /* Buzzer */
	digitalWrite(LEDverde, LOW); /* Led verde: Pode digitar */
	digitalWrite(LEDvermelho, LOW); /* Led vermelho: Erro */
	/* Iniciar o gerador randômico */
	randomSeed(analogRead(A10));
	/* Inicializar comunicação serial, para debug */
	Serial.begin(9600); /* msp430g2231 must use 4800 */
	/* Pequeno delay para estabilizar */
	delay(10);
	/* Verificar se o serial irá funcionar */
	if(Serial.available() == 0){
		Serial.println("Comunicacao OK");
		delay(10);
	} else{
		while(1) Serial.println("NOK");
	}
}

/*FUNÇÃO PRINCIPAL*/
void loop()
{

	static int seq_gen[32] = {0};
	static int seq_read[32] = {0};
	unsigned int level = 1;

	int i = 0, j = 0, cont = 0;
	int fail = 0;
	int continuar_jogo = 0;

	Serial.println("OK, variaveis setadas");

	/*Apresenta as frequencias de sons utilizadas*/
	for (cont = 1; cont <= 4; cont++){
		toca_buzzer(cont);
		Serial.println(cont);
	}
	delay(2000);

	/* loop que correrá enquanto o sistema estiver ligado */
	while(1){
	continuar_jogo = 0;
	/* Caso uma derrota não resete o programa */
	/*fail = 0;*/
	/* Checar se nível máximo */
	if(level >= 32){
		Serial.println("YOU ARE A SUPER PLAYER");
	while(1);
	}

	delay(500);
	Serial.print("O nivel e: ");
	Serial.println(level);

	/* Gerar o número aleatório*/
	seq_gen[i] = (random(0, 65535) % 4) + 1;

	Serial.print("O numero aleatorio e: ");
	Serial.println(seq_gen[i]);

	/* Piscar os LEDs na sequencia */
	for(j = 0; j < level; j++){
		Serial.print("O LED e: ");
		Serial.println(seq_gen[j]);
		toca_buzzer(seq_gen[j]);
	}

	/* Ler os botões */
	Serial.println("Ler botoes");
	delay(100);
	Serial.flush();

	for(j = 0; j < level; j++){
	/* Ler o serial, para debug */
	//while(Serial.available() < 1);
	/* Converter do ASCII para int */
	//seq_read[j] = Serial.read() - 48;
		delay(200);
		seq_read[j] = ler_portas();
	}

	/* Comparar as sequencias */
	for(j = 0; j < level; j++){
		Serial.print(seq_gen[j]);
		Serial.print(" = ");
		Serial.print(seq_read[j]);
		Serial.println("?");

		if(seq_gen[j] != seq_read[j]) fail = 1;
	}

	delay(1000);
	/* Checar se passou */
	if(fail != 0){
		Serial.println("ERRO DE DIGITACAO");
		Serial.print("Nivel maximo: ");
		Serial.println(level);
		Serial.println("G A M E  O V E R");
		pisca2x(LEDvermelho);
		level = 1;
		fail = 0;
	}else{
		Serial.println("DIGITADO CORRETAMENTE");
		pisca2x(LEDverde);
		level++;
		i++;
	}
	delay(500); 

	/*Deseja continuar?*/
	digitalWrite(LEDverde,HIGH);
	digitalWrite(LEDvermelho,HIGH);
	while(continuar_jogo == 0){
		continuar_jogo = ler_portas();
		if(continuar_jogo == 1){
			digitalWrite(LEDvermelho, LOW);
			digitalWrite(LEDverde, LOW);
			delay(100);
			digitalWrite(LEDverde, HIGH);
			delay(100);
			digitalWrite(LEDverde, LOW);
			delay(100);
			Serial.println("VAMOS CONTINUAR JOGANDO!");
		}else if(continuar_jogo == 2){
			digitalWrite(LEDverde, LOW);
			digitalWrite(LEDvermelho, LOW);
			delay(100);
			digitalWrite(LEDvermelho, HIGH);
			delay(100);
			digitalWrite(LEDvermelho, LOW);
			delay(100);
			Serial.println("FIM DE JOGO!");
			while(1); /*Não quis continuar*/
		}else{
			continuar_jogo = 0;
		}
	}


	}
}

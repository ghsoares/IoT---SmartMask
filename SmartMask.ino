#include <Ultrasonic.h>	// Biblioteca do sensor ultrassônico

/*
Sensor ultrssônico, é utilizado para detectar a proximidade da
mão em relação á máscara
*/
HC_SR04 sensorDistancia(2, 3);
/*
Pino do led, é um pino pwm, isso significa que é um pino digital,
mas com um recurso adicional para modulação de frequência, isso gera
uma saída analógica, utilizado para regular o brilho do led
*/
int pinLed = 5;
/*
Pino do sensor de temperatura baseado em resistência
*/
int pinTherm = A0;
// Distância mínima detectada em centímetros
int distanceMin = 2;		
// Distância máxima em centímetros
int distanceMax = 30;		

int maxIntensity = 125;

void setup() {
	// Inicia o serial
  	Serial.begin(9600); 
	// Define o pino do led como saída   
	pinMode(pinLed, OUTPUT);
}

void loop() {
	// Distância atual em centímetros
	int distMao = sensorDistancia.distance();
	
	// Temperatura atual
	int temp = analogRead(pinTherm);
	
	// [Debug] Mostra no monitor serial a distância atual
	Serial.println(String("Distância: ") + distMao + String(" cm"));
	
	// [Debug] Mostra no monitor serial a temperatura atual
	Serial.println(String("Temperatura: ") + temp + String(""));
	
	// Limita a distância para evitar overflow do método map
	distMao = constrain(distMao, distanceMin, distanceMax);
	
	/*
	Mapeia a intensidade (brilho) do led utilizando a distância do sensor,
	sendo que uma distância próxima da distância mínima deixa o led com mais
	intensidade que uma distância próxima ou maior que a distância máxima,
	255 - Intensidade máxima
	0 - Intensidade Mínima
	*/
	int ledIntensity = map(distMao, distanceMin, distanceMax, maxIntensity, 0);
	
	/*
	Escreve a saída analógica do pino do led, é usado o pino pwm
	para modulação do sinal para melhor controle da intensidade do led
	*/
	analogWrite(pinLed, ledIntensity);
	
	// Espera um décimo de segundo
	delay(100);
}

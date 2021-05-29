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
/*
Pino do sensor de umidade (resistência)
*/
int pinUmidade = A1;
// Distância mínima detectada em centímetros
int distanceMin = 2;		
// Distância máxima em centímetros
int distanceMax = 30;		

// Intensidade máxima do led
int maxIntensity = 125;

// Resistência do termistor
float tempR = 10000;
// Temperatura atual
float temp;
// umidade relativa atual
float umid;

void setup() {
	// Inicia o serial
  	Serial.begin(9600); 
	// Define o pino do led como saída   
	pinMode(pinLed, OUTPUT);
}

void loop() {
	// Distância atual em centímetros
	int distMao = sensorDistancia.distance();
	
	// Lê a temperatura atual
	temp = readTemp(pinTherm, tempR);
	
	// Lê a umidade relativa atual
	umid = (float)analogRead(pinUmidade);
	
	// Mapeia a umidade relativa (0-1023 para 0%-100%)
	umid = map(umid, 0, 1023, 0, 100);
	
	// [Debug] Mostra no monitor serial a distância atual
	Serial.println(String("Distância: ") + distMao + String(" cm"));
	
	// [Debug] Mostra no monitor serial a temperatura atual
	Serial.println(String("Temperatura: ") + temp + String(""));
	
	// [Debug] Mostra no monitor serial a umidade relativa atual
	Serial.println(String("Umidade: ") + umid + String("%"));
	
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
	
	// Envia os dados dos sensores para o aplicativo (react-native) dados como temperatura, umidade, etc.
	// TODO: Enviar dados do sensor para o aplicativo
	
	// Espera um décimo de segundo
	delay(1000);
}

//função que faz leitura da temperatura e retorna o valor em graus celcius
float readTemp(int pin, float resistencia){
	//valores constante para calculo da temperatura
	float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07; 

	// Lê a entrada
  float tempOut = (float)analogRead(pin);
//calculo R2, demonstração no arquivo pdf da aula
  tempOut = resistencia * (1023.0 / tempOut - 1.0);
  tempOut = log(tempOut);
// Equação de Steinhart–Hart 
  tempOut = (1.0 / (c1 + c2*tempOut + c3*tempOut*tempOut*tempOut));// Equação de Steinhart–Hart 
  tempOut = tempOut - 273.15; //temperatura em Graus celcius
  //Tf = (Tc * 9.0)/ 5.0 + 32.0; // temp em fahrenheit
  return tempOut;
}

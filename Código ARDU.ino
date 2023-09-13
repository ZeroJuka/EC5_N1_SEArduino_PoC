#include <LiquidCrystal.h>
#include "dht.h"
#include <Wire.h> //INCLUSÃO DA BIBLIOTECA
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA
#include <EEPROM.h>

#define PINBUZZER 9
#define FORA_DOS_LIMITES  !((m_temperatura >= 15)&&(m_temperatura <= 25)) || !((m_umidade >= 30)&&(m_umidade <= 50)) || !((m_luminosidade >= 0)&&(m_luminosidade <= 30))

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
const int pinoDHT11 = A2; //PINO ANALÓGICO UTILIZADO PELO DHT11
dht DHT; //VARIÁVEL DO TIPO DHT
RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231 REAL TIME CLOCK


//CRIANDO OS BYTES PARA ANIMAÇÃO
byte PAKMAIN_ALIMENTAR[8] = {
B01110,
B11111,
B11101,
B11111,
B11100,
B11000,
B11100,
B01111
};

byte PAKMAIN_ALIMENTAR_INVERSO[8] = {
B01110,
B11111,
B10111,
B11111,
B00111,
B00011,
B00111,
B11110
};

byte PAKMAIN_COMIDO[8] = {
B01110,
B11111,
B11101,
B11111,
B11111,
B11111,
B11111,
B01110
};

byte PAKMAIN_COMIDO_INVERSO[8] = {
B01110,
B11111,
B10111,
B11111,
B11111,
B11111,
B11111,
B01110
};

byte GHOST_FANTASMA[8] = {
B01110,
B11111,
B10101,
B11111,
B11111,
B11111,
B11111,
B10101
};

byte VAZIO[8] = {
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00000
};


void setup()
{
  pinMode(PINBUZZER, OUTPUT); //Setup do Buzzer
  pinMode(7, OUTPUT); //SETUP do LED
  
  //SERIAL
  Serial.begin(9600); //INICIALIZA A SERIAL
  
  //LCD SETUP
  lcd.begin(16,2);              
  lcd.clear();                  
  lcd.setCursor(0,0);           
  lcd.print("Lumi-Temp-Umid");    
  lcd.setCursor(0,1);           
  lcd.print("--Date--Time--");


// To do, criar enum para cada estado
  lcd.createChar(1, PAKMAIN_ALIMENTAR);
  lcd.createChar(2, PAKMAIN_COMIDO);
  lcd.createChar(3, GHOST_FANTASMA);
  lcd.createChar(4, VAZIO);
  lcd.createChar(5, PAKMAIN_ALIMENTAR_INVERSO);
  lcd.createChar(6,PAKMAIN_COMIDO_INVERSO);
  
  //RTC SETUP
  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
  if(rtc.lostPower()){
    Serial.println("DS3231 OK!"); 
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    //rtc.adjust(DateTime(2023, 8, 29, 21, 26, 00)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }
  delay(2000);
  lcd.clear();
  //Animação
    int pacman_status = 2;
  
    for(int i = 0; i < 16; ++i)
  {
    lcd.setCursor(i,0);
    if(pacman_status % 2 == 0)
    {
      pacman_status = 1;
      lcd.write(pacman_status);
    }
    else
    {
      pacman_status = 2;
      lcd.write(pacman_status);
    }
    lcd.setCursor(i+2,0);
    lcd.write(3);
    lcd.setCursor(i+3,0);
    lcd.write(3);
    lcd.setCursor(i+4,0);
    lcd.write(3); 
    delay(350);
    lcd.clear();   
  } 

  pacman_status = 6;
  for(int i = 16; i > -5; --i)
  {
    lcd.setCursor(i+2,1);
    lcd.write(3);
    lcd.setCursor(i+3,1);
    lcd.write(3);
    lcd.setCursor(i+4,1);
    lcd.write(3);
    lcd.setCursor(i,1);
    if(pacman_status % 2 == 0)
    {
      pacman_status = 5;
      lcd.write(pacman_status);
    }
    else
    {
      pacman_status = 6;
      lcd.write(pacman_status);
    } 
    delay(250);
    lcd.clear();   
  } 
  delay(1000);
}

//Declarando variáveis necessárias para calcular a media e exibir os dados da EEPROM no Serial
int cont = 0;
unsigned long i = 0;

void loop()
{ 
  //RTC CONFIG
  DateTime dt = rtc.now(); //CHAMADA DE FUNÇÃO dt.day, dt.year, dt.month
  //LCD CONFIG
  delay(250);
  lcd.noDisplay();
  lcd.clear();
  delay(250);
  lcd.display();
  lcd.print("Calculando...");

  //LDR (LUMINOSIDADE)
  int sensorValue = analogRead(A0);   // Ler o pino Analógico A0 onde está o LDR, lembrando que o divisor de tensão é Vin = Vout (R2/(R1 + R2))

  //CALCULA AS MEDIAS DOS VALORES AO FINAL DE 10 SEGUNDOS
  unsigned long tempo_inicio = millis();
  Serial.print("millis:");
  Serial.println(tempo_inicio);
  cont = 0;
  float m_umidade = 0; //Média da Umidade
  float m_luminosidade = 0; //Media da Luminosidade
  float m_temperatura = 0; //Media da Temperatura
  while((millis() - tempo_inicio) <= 10000 ) //durante 10 segundos
  {
    //DENTRO DESTE WHILE ELE REALIZA O CÁLCULO DAS MEDIDAS DURANTE 10 SEGUNDOS.
    //AO FINAL DOS 10 SEGUNDOS, ELE EXIBE A MÉDIA DOS VALORES MEDIDOS POR 50 SEGUNDOS, TOTALIZANDO 1 MINUTO DE ITERAÇÃO
    cont += 1;
    
    //MEDICOES
    DHT.read11(pinoDHT11); //Mede Umidade e Temperatura pelo DHT
    m_umidade += DHT.humidity;
    m_temperatura += DHT.temperature;

    sensorValue = analogRead(A0);
    m_luminosidade += map(sensorValue, 0, 1023, 0, 100);  // Normalizar o valor da luminosidade entre 0% e 100%
  }
  Serial.print(String(cont) + '\n');
  Serial.print("");
  m_umidade /= cont; //Calcula a média
  m_temperatura /= cont; //Calcula a media
  m_luminosidade /= cont; //Calcula a media

  // Verifica se os valores estão dentro dos limites
  
  if ( FORA_DOS_LIMITES )
  {
    // EEPROM
    String log_eeprom = "";
    log_eeprom += String(m_umidade) + "%|";
    log_eeprom += String(m_temperatura) + "*C|";
    log_eeprom += String(m_luminosidade) + "%";
    log_eeprom += "-" + String(dt.day(), DEC) +"/"+ String(dt.month(), DEC) +"/"+ String(dt.year(), DEC) + ',';
    //TEXTO DE LOG É INSERIDO NA EEPROM EM CASO DE "IRREGULARIDADE"
    digitalWrite(7, HIGH); //A LED Acende
    tone(PINBUZZER, 1660, 3000); //O Buzzer Toca
    delay(3000);
    digitalWrite(7, LOW); //A LED Apaga
    
    //Gravar na eeprom
    Serial.println("i="+String(i));
    for (i; i < 1024; ++i)
    {
      EEPROM.write(i, log_eeprom[i]);
    }
  }
  
  //EXIBIÇÃO LCD
  lcd.setCursor(0,0);
  lcd.print(m_umidade); //IMPRIME O VALOR DE UMIDADE MEDIDO
  lcd.print("%|"); //ESCREVE O TEXTO 
  lcd.print(m_temperatura, 0); //IMPRIME O VALOR DE UMIDADE MEDIDO E REMOVE A PARTE DECIMAL
  lcd.print("*C|"); //IMPRIME O TEXTO
  lcd.print(round(m_luminosidade)); //ESCREVE A LUMINOSIDADE 
  lcd.print("%"); //ESCREVE O TEXTO EM SEGUIDA
  
  lcd.setCursor(0,1); //PASSA PRA PRÓXIMA LINHA
  lcd.print(dt.day(), DEC); //DIA
  lcd.print('/'); 
  lcd.print(dt.month(), DEC); //MES
  lcd.print('/');
  lcd.print(dt.year(), DEC); //ANO

  lcd.print("-");
  lcd.print(dt.hour(), DEC); //HORA
  lcd.print(':');
  lcd.print(dt.minute(), DEC); //MINUTO

  // Ler a EEPROm

  String retorno = "";
  for(int i = 0; i < EEPROM.length(); ++i)
  {
    retorno += char(EEPROM.read(i));
  }
  //Todo o dado da EEMPROM é exibido no SERIAL
  Serial.print(retorno);

  delay(20000);
}

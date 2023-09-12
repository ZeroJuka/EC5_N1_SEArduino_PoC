# Readme.md
> Trabalho de N1 de Sistemas Embarcados utilizando Arduino UNO e diversos sensores para medir a temperatura, umidade e luminosidade de um ambiente (referir a [Integrantes.md](main/Integrantes.md) para conhecer os integrantes deste grupo.) - Video: [Youtube](https://youtu.be/1VAtpi4HaL8)
### Lista de componentes utilizados:
  -1 MCU (Atmega 328P) - Arduino Uno R3 <br/>
  -1 LDR + Resistor 1KOhm <br/>
  -1 DHT-11 (Sensor de temperatura e umidade) <br/>
  -1 LCD 16x2 <br/>
  -1 Bateria de 9V <br/>
  -1 RTC (Real Time Clock) <br/>
  -1 protoboard <br/>
  -Cabos (Jumpers) <br/>
  -1 LED <br/>
  -Resistores <br/>
  -1 Suporte para bateria <br/>

### Funcionamento
  O nosso projeto deve medir, através dos sensores DHT11 e LDR, a Temperatura, Umidade e Luminosidade do ambiente onde o protótipo está posicionado. Durante 10 segundos desde o início de seu funcionamento ocorre a medição contínua e repetidamente pelos sensores citados para maximizar a precisão do equipamento, após essa medição, é feita uma média dos valores medidos para ser apresentada com alta precisão no LCD (Liquid Crystal Display) para uma futura análise, na qual as médias dos valores são comparadas a **normas pré-estabelecidas de Níveis Adequados**, são essas:
 > Temperatura: 15 < t < 25 ºC <br/>
 > Luminosidade: 0 < l < 30% <br/>
 > Umidade: 30% < u < 50% <br/>
 
No caso de aprovação dos dados (quando esses correspondem aos níveis adequados), nenhum alerta é oferecido. Todavia, se algum dos valores não cumprir tal condição, aquela iteração das variáveis locais medidas serão salvos na memória interna do Arduino Uno (EEMPROM) e além disso, um LED acenderá e um Buzzer soará, alertando sobre a inconsistência naquela iteração.


![image](https://github.com/ZeroJuka/EC5_N1_SEArduino_PoC/assets/144825939/05939933-8abf-4c5e-8dee-5aede2f34c9a)

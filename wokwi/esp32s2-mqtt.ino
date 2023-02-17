/*
  Configure o cliente do navegador HiveMQ para visualizar as
  mensagens MQTT publicadas pelo cliente MQTT.

  1) Vá para a URL abaixo e clique no botão conectar
     http://www.hivemq.com/demos/websocket-client/

  2) Adicione os tópicos de assinatura um para cada tópico que o ESP32 usa:
     topic_on_off_led/#
     topic_sensor_temperature/#
     topic_sensor_humidity/#

  3) Experimento publicar no topico topic_on_off_led com a mensagem 1 e 0
     para ligar e desligar o LED"

  IMPORTANTE: É necessário rodar o Wokwi Gateway e habilitar a opção
  "Enable Private Wokwi IoT Gateway" através da tecla de atalho F1 no editor de
  código.

  Para baixar o Wokwi IoT Network Gateway acesse o seguinte link:
    https://github.com/wokwi/wokwigw/releases/
*/

#include <DHTesp.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define PIN_LED 15  // GPIO que está ligado o LED
#define PIN_DHT 11  // GPIO que está ligado o pino do sensor

/* Configura os tópicos do MQTT */
#define TOPIC_SUBSCRIBE_LED "topic_on_off_led"
#define TOPIC_PUBLISH_TEMPERATURE "topic_sensor_temperature"
#define TOPIC_PUBLISH_HUMIDITY "topic_sensor_humidity"

#define PUBLISH_DELAY 2000  // Atraso da publicação (2 segundos)

#define ID_MQTT "esp32_mqtt"  // id mqtt (para identificação de sessão)

// IMPORTANTE: Este deve ser único no broker, ou seja, se um client MQTT
// tentar entrar com o mesmo id de outro já conectado ao broker,
// o broker irá fechar a conexão de um deles.

/* Variaveis, constantes e objetos globais */
DHTesp dht;

const char *SSID =
    "Wokwi-GUEST";          // SSID / nome da rede WI-FI que deseja se conectar
const char *PASSWORD = "";  // Senha da rede WI-FI que deseja se conectar

// URL do broker MQTT que se deseja utilizar
const char *BROKER_MQTT = "broker.hivemq.com";

int BROKER_PORT = 1883;  // Porta do Broker MQTT

unsigned long publishUpdate;

static char strTemperature[10] = {0};
static char strHumidity[10] = {0};

// Variáveis e objetos globais
WiFiClient espClient;  // Cria o objeto espClient
PubSubClient MQTT(
    espClient);  // Instancia o Cliente MQTT passando o objeto espClient

/* Prototypes */
float getTemperature(void);
float getHumidity(void);

void initWiFi(void);
void initMQTT(void);
void callbackMQTT(char *topic, byte *payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void checkWiFIAndMQTT(void);

/* Faz a leitura da temperatura (sensor DHT22)
   Retorno: temperatura (graus Celsius) */
float getTemperature(void) {
  TempAndHumidity data = dht.getTempAndHumidity();

  if (!(isnan(data.temperature)))
    return data.temperature;
  else
    return -99.99;
}

/* Faz a leitura da umidade relativa do ar (sensor DHT22)
   Retorno: umidade (0 - 100%) */
float getHumidity(void) {
  TempAndHumidity data = dht.getTempAndHumidity();

  if (!(isnan(data.humidity)))
    return data.humidity;
  else
    return -99.99;
}

/* Inicializa e conecta-se na rede WI-FI desejada */
void initWiFi(void) {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");

  reconnectWiFi();
}

/* Inicializa os parâmetros de conexão MQTT(endereço do broker, porta e seta
  função de callback) */
void initMQTT(void) {
  MQTT.setServer(
      BROKER_MQTT,
      BROKER_PORT);  // Informa qual broker e porta deve ser conectado
  MQTT.setCallback(callbackMQTT);  // Atribui função de callback (função chamada
                                   // quando qualquer informação de um dos
                                   // tópicos subescritos chega)
}

/* Função de callback  esta função é chamada toda vez que uma informação
   de um dos tópicos subescritos chega */
void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  String msg;

  // Obtem a string do payload recebido
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }

  Serial.printf("Chegou a seguinte string via MQTT: %s do topico: %s\n", msg,
                topic);

  /* Toma ação dependendo da string recebida */
  if (msg.equals("1")) {
    digitalWrite(PIN_LED, HIGH);
    Serial.print("LED ligado por comando MQTT");
  }

  if (msg.equals("0")) {
    digitalWrite(PIN_LED, LOW);
    Serial.print("LED desligado por comando MQTT");
  }
}

/* Reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a
   conexão cair) em caso de sucesso na conexão ou reconexão, o subscribe dos
   tópicos é refeito. */
void reconnectMQTT(void) {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPIC_SUBSCRIBE_LED);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Nova tentativa de conexao em 2 segundos.");
      delay(2000);
    }
  }
}

/* Verifica o estado das conexões WiFI e ao broker MQTT.
  Em caso de desconexão (qualquer uma das duas), a conexão é refeita. */
void checkWiFIAndMQTT(void) {
  if (!MQTT.connected())
    reconnectMQTT();  // se não há conexão com o Broker, a conexão é refeita

  reconnectWiFi();  // se não há conexão com o WiFI, a conexão é refeita
}

void reconnectWiFi(void) {
  // se já está conectado a rede WI-FI, nada é feito.
  // Caso contrário, são efetuadas tentativas de conexão
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(SSID, PASSWORD);  // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  // Configura o pino do LED como output e inicializa em nível baixo
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  // Inicializacao do sensor de temperatura
  dht.setup(PIN_DHT, DHTesp::DHT22);

  // Inicializa a conexao Wi-Fi
  initWiFi();

  // Inicializa a conexao ao broker MQTT
  initMQTT();
}

void loop() {
  /* Repete o ciclo após 2 segundos */
  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();
    // Verifica o funcionamento das conexões WiFi e ao broker MQTT
    checkWiFIAndMQTT();

    // Formata as strings a serem enviadas para o dashboard (campos texto)
    sprintf(strTemperature, "%.2fC", getTemperature());
    sprintf(strHumidity, "%.2f", getHumidity());

    // Envia as strings ao dashboard MQTT
    MQTT.publish(TOPIC_PUBLISH_TEMPERATURE, strTemperature);
    MQTT.publish(TOPIC_PUBLISH_HUMIDITY, strHumidity);

    // Keep-alive da comunicação com broker MQTT
    MQTT.loop();
  }
}

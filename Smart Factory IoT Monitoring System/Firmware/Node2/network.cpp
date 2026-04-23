#include "network.h"
#include "rpc.h"

static PubSubClient* client;
static void (*onConnectCallback)(void);
static unsigned long lastReconnect = 0;

void network_begin(PubSubClient* mqtt, void (*onConnected)(void))
{
  static byte mac[] = NODE_MAC;

  client = mqtt;
  onConnectCallback = onConnected;

  Serial.println("[NET] Starting Ethernet...");

  while (Ethernet.begin(mac) == 0) {
    Serial.println("[NET] DHCP failed, retrying...");
    delay(2000);
  }

  Serial.print("[NET] IP: ");
  Serial.println(Ethernet.localIP());

  client->setServer(TB_SERVER, TB_PORT);

  // 🔥 RPC callback
  client->setCallback(rpc_mqttCallback);
}

void network_maintain(void)
{
  if (!client->connected())
  {
    if (millis() - lastReconnect > RECONNECT_INTERVAL)
    {
      lastReconnect = millis();

      Serial.println("[MQTT] Connecting...");

      if (client->connect(CLIENT_ID, ACCESS_TOKEN, ""))
      {
        Serial.println("[MQTT] Connected ✅");

        client->subscribe(TOPIC_RPC_REQUEST);

        if (onConnectCallback)
          onConnectCallback();
      }
      else
      {
        Serial.print("[MQTT] Failed rc=");
        Serial.println(client->state());
      }
    }
  }

  client->loop(); // 🔥 mandatory
}

uint8_t network_isConnected(void)
{
  return client->connected();
}

uint8_t network_publish(const char* topic, const char* payload, uint8_t retain)
{
  return client->publish(topic, payload, retain);
}

uint8_t network_subscribe(const char* topic)
{
  return client->subscribe(topic);
}
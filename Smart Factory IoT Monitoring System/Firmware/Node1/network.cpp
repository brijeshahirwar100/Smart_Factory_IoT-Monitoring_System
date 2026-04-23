#include "network.h"
#include "rpc.h"   // 🔥 REQUIRED for RPC

static PubSubClient* client;
static void (*onConnectCallback)(void);

static unsigned long lastReconnect = 0;

// ─────────────────────────────────────────────
// INIT NETWORK + MQTT
// ─────────────────────────────────────────────
void network_begin(PubSubClient* mqtt, void (*onConnected)(void))
{
    static byte mac[] = NODE_MAC;

    client = mqtt;
    onConnectCallback = onConnected;

    Serial.println("[NET] Starting Ethernet...");

    if (Ethernet.begin(mac) == 0) {
        Serial.println("[NET] DHCP failed ❌");
    } else {
        Serial.print("[NET] IP: ");
        Serial.println(Ethernet.localIP());
    }

    delay(1000);

    client->setServer(TB_SERVER, TB_PORT);

    Serial.print("[NET] MQTT server: ");
    Serial.print(TB_SERVER);
    Serial.print(":");
    Serial.println(TB_PORT);
}

// ─────────────────────────────────────────────
// MAINTAIN CONNECTION
// ─────────────────────────────────────────────
void network_maintain(void)
{
    if (!client->connected())
    {
        if (millis() - lastReconnect > RECONNECT_INTERVAL)
        {
            lastReconnect = millis();

            Serial.println("[MQTT] Connecting...");

            // username = ACCESS_TOKEN
            bool ok = client->connect(CLIENT_ID, ACCESS_TOKEN, "");

            if (ok)
            {
                Serial.println("[MQTT] Connected ✅");

                // 🔥 VERY IMPORTANT: SET CALLBACK
                client->setCallback(rpc_mqttCallback);

                // 🔥 SUBSCRIBE TO RPC
                client->subscribe(TOPIC_RPC_REQUEST);

                Serial.println("[MQTT] RPC Subscribed");

                if (onConnectCallback) {
                    onConnectCallback();
                }
            }
            else
            {
                Serial.print("[MQTT] Failed ❌ rc=");
                Serial.println(client->state());
            }
        }
    }

    client->loop();  // keep MQTT alive
}

// ─────────────────────────────────────────────
// STATUS CHECK
// ─────────────────────────────────────────────
uint8_t network_isConnected(void)
{
    return client->connected();
}

// ─────────────────────────────────────────────
// PUBLISH
// ─────────────────────────────────────────────
uint8_t network_publish(const char* topic, const char* payload, uint8_t retain)
{
    if (!client->connected()) {
        Serial.println("[PUB] MQTT NOT CONNECTED ❌");
        return 0;
    }

    Serial.print("[PUB] Topic: ");
    Serial.println(topic);

    Serial.print("[PUB] Payload: ");
    Serial.println(payload);

    uint8_t ok = client->publish(topic, payload, retain);

    Serial.print("[PUB] Status: ");
    Serial.println(ok);  // 1 = success

    return ok;
}

// ─────────────────────────────────────────────
// SUBSCRIBE (OPTIONAL UTILITY)
// ─────────────────────────────────────────────
uint8_t network_subscribe(const char* topic)
{
    if (!client->connected()) return 0;

    uint8_t ok = client->subscribe(topic);

    Serial.print("[SUB] ");
    Serial.print(topic);
    Serial.print(" -> ");
    Serial.println(ok);

    return ok;
}
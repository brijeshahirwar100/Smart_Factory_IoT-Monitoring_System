#include "rpc.h"
#include "actuator.h"
#include <string.h>

// 🔥 Use external override flag from main
extern uint8_t manualOverride;

static char* rpcBuf;
static uint8_t bufSize;

void rpc_init(PubSubClient* client, char* buf, uint8_t size)
{
  (void)client; // not used directly
  rpcBuf = buf;
  bufSize = size;
}

void rpc_mqttCallback(char* topic, byte* payload, unsigned int length)
{
  if (length >= bufSize) length = bufSize - 1;

  memcpy(rpcBuf, payload, length);
  rpcBuf[length] = '\0';

  // 🔥 SIMPLE PARSE (works for ThingsBoard switch)
  if (strstr(rpcBuf, "setRelay"))
  {
    if (strstr(rpcBuf, "true"))
      actuators_setRelay(1);
    else
      actuators_setRelay(0);

    manualOverride = 1;  // 🔥 IMPORTANT
  }
}
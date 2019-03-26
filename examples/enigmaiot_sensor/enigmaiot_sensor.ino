#include <EnigmaIOTSensor.h>
#include <espnow_hal.h>
#include <CayenneLPP.h>
#define BLUE_LED 2
uint8_t gateway[6] = { 0x5E, 0xCF, 0x7F, 0x80, 0x34, 0x75 };

#define SLEEP_TIME 10000000

ADC_MODE (ADC_VCC);

void connectEventHandler () {
    Serial.println ("Connected");
}

void disconnectEventHandler () {
    Serial.println ("Disconnected");
}

void processRxData (const uint8_t* mac, const uint8_t* buffer, uint8_t length) {
    char macstr[18];
    mac2str (mac, macstr);
    Serial.println ();
    Serial.printf ("Data from %s --> %s\n", macstr, printHexBuffer (buffer, length));
    for (int i = 0; i < length; i++) {
        Serial.print ((char)buffer[i]);
    }
    Serial.println ();
    Serial.println ();
}

void setup () {
    CayenneLPP msg (20);

    Serial.begin (115200); Serial.println (); Serial.println ();

    EnigmaIOTSensor.setLed (BLUE_LED);
    EnigmaIOTSensor.onConnected (connectEventHandler);
    EnigmaIOTSensor.onDisconnected (disconnectEventHandler);
    EnigmaIOTSensor.onDataRx (processRxData);
    EnigmaIOTSensor.begin (&Espnow_hal, gateway);

    msg.addAnalogInput (0, (float)(ESP.getVcc ())/1000);
    Serial.printf ("Vcc: %f\n", (float)(ESP.getVcc ()) / 1000);
    msg.addTemperature (1, 20.34);
    //char *message = "Hello World!!!";

    Serial.printf ("Trying to send: %s\n", printHexBuffer (msg.getBuffer (), msg.getSize ()));
    //EnigmaIOTSensor.sendData ((uint8_t *)message, strlen (message));
    EnigmaIOTSensor.sendData (msg.getBuffer (), msg.getSize ());

    EnigmaIOTSensor.sleep (SLEEP_TIME);
}

void loop () {

    EnigmaIOTSensor.handle ();

}
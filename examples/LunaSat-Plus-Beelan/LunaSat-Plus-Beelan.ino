/**
 * Based on Beelan LoraWAN Class A example - Ivan Moreno, Eduardo Contreras June 2019
 */
#include <lorawan.h>   
#include "TMP117.h"
#include "MLX90393.h"
#include "MPU6000.h"
#include "CAP.h"
#include "TPIS1385.h"
#include "GLEE_Radio.h"

LunaRadio Rad;

// Sensor Objects (Debugging defaults to false)
TMP117 thermometer(1);
MPU6000 accelerometer(2);
MLX90393 magnetometer(3);
TPIS1385 thermopile(4);
CAP capacitive(5);

// Sensor Data Variables
float tempSample;
sensor_float_vec_t accSample;
mlx_sample_t magSample;
TPsample_t thermSample;
int capSample;

// Individual string buffers for memory optimization
char timeStamp_str[8] = {' '};
char temp_str[8] = {' '} ;
char accX_str[8] = {' '} ;
char accY_str[8] = {' '} ;
char accZ_str[8] = {' '} ;
char magX_str[7] = {' '} ;
char magY_str[7] = {' '} ;
char magZ_str[7] = {' '} ;
char thermObj_str[6] = {' '} ;
char thermAmb_str[6] = {' '} ;
char cap_str[5] = {' '};

// Transmission Buffer
char data_buffer[80]; 

// Output Formatting Variables
String printOut;
char sep = ",";
unsigned long timeStamp;

//ABP Credentials 
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

const unsigned long interval = 10000;    // 10 s interval to send message
unsigned long previousMillis = 0;  // will store last time message sent
unsigned int counter = 0;     // message counter

char myStr[50];
char outStr[255];
byte recvStatus = 0;

const sRFM_pins RFM_pins = {
  .CS = 10,
  .RST = 9,
  .DIO0 = 2,
  .DIO1 = 3,
  .DIO2 = -1,
  .DIO5 = -1,
};

void setup() {
  // Setup loraid access
  Serial.begin(9600);
  while(!Serial);
  if(!lora.init()){
    Serial.println("SX1272 Initialization failed");
    delay(5000);
    return;
  }

  // Set LoRaWAN Class change CLASS_A or CLASS_C
  lora.setDeviceClass(CLASS_A);

  // Set Data Rate
  lora.setDataRate(SF12BW250);

  // set channel to random
  lora.setChannel(MULTI);
  
  // Put ABP Key and DevAddress here
  lora.setNwkSKey(nwkSKey);
  lora.setAppSKey(appSKey);
  lora.setDevAddr(devAddr);
}

void loop() {
  // Check interval overflow
  if(millis() - previousMillis > interval) {
    previousMillis = millis(); 

    sprintf(myStr, "Counter-%d", counter); 

    Serial.print("Sending: ");
    Serial.println(myStr);
    
    lora.sendUplink(myStr, strlen(myStr), 0, 1);
    counter++;
  }

  recvStatus = lora.readData(outStr);
  if(recvStatus) {
    Serial.println(outStr);
  }
  
  // Check Lora RX
  lora.update();
}
//Sketch based on work done by Salvador AC.

//Este es un programa para un escaner galvo con el protocolo XY2_100.h para Marlin 1.0.6 

#include <XY2_100.h>
#define NUM_AXIS 2
float fraction ;
float steps = 200.0;  ///a 200 semueve vien sin ruido 0.05 configuracion 0 delay
float t = (1.0);
const byte buffSize = 15;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;
//float  velServoAngle ;
#include <SoftwareSerial.h>
SoftwareSerial mySerial(0, 1);
XY2_100 galvo;
float difference[NUM_AXIS]={  0.0, 0.0, };
static float destination[NUM_AXIS] = {  0.0, 0.0, };
static float current_position[NUM_AXIS] = {  0.0, 0.0, };
//=============

void setup() {
 // Serial.begin(115200);
  mySerial.begin(115200);
   galvo.begin();
   galvo.setXY(0,0);

}

//=============

void loop() {

  //Serial.print("<");
  getDataFromPC();
  
  replyToPC();

}

//=============

void getDataFromPC() {

    // receive data from PC and save it into inputBuffer
    
  if(mySerial.available() > 0) {

    char x = mySerial.read();              //read char from serial
    //  Serial.print(mySerial.read());
    if (x == endMarker) {                //look for end marker
      readInProgress = false;            //if found, set read in progress true (will stop adding new byte to buffer)
      newDataFromPC = true;              //let arduino know that new data is available
      inputBuffer[bytesRecvd] = 0;       //clear input buffer
      processData();                      // process data in buffer
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;      //populate input buffer with bytes
      bytesRecvd ++;                    //increment index
      if (bytesRecvd == buffSize) {     //when buffer is full
        bytesRecvd = buffSize - 1;      //keep space for end marker
      }
    }

    if (x == startMarker) {              // look for start maker
      bytesRecvd = 0;                    // if found, set byte received to 0
      readInProgress = true;             // set read in progress true
    }
  }
}

//=============

void processData() // for data type "<float, float, int>" 
{
  char * strtokIndx; // this is used by strtok() as an index  <1233,3223>

   strtokIndx = strtok(inputBuffer,",");      // get the first part
   destination[0] = atof(strtokIndx);         // convert this part to a float

   strtokIndx = strtok(NULL,",");          // get the second part(this continues where the previous call left off)
   destination[1] = atof(strtokIndx);     // convert this part to a float

   //strtokIndx = strtok(NULL, ",");      // get the last part
   //velServoAngle = atoi(strtokIndx);        // convert this part to an <1233,3223>integer (string to int)
}

//=============

void replyToPC() {

  if (newDataFromPC) {
    newDataFromPC = false;
 //  if( velServoAngle == 1){
    t = 1 ;
    for(int i=0; i < t ; i++) {
    
  //Serial.print("<>");
  //Serial.print(destination[0]);
  //Serial.print(",");
  //Serial.print(destination[1]);
  //Serial.println("<>");
    }
 for (int8_t i=0; i < NUM_AXIS; i++) {
    difference[i] = destination[i] - current_position[i];
   // Serial.println("dif");
     //  Serial.print(destination[i]);

  //Serial.print(difference[i]);
  //  Serial.println(",");
  }
   for (int s = 1; s <= steps; s++) {
   // Serial.println("paso");
    
   fraction = 1 / steps;
   
    
    for(int8_t i=0; i < NUM_AXIS; i++) {
      destination[i] = current_position[i] + difference[i] * fraction;
     //Serial.println("dety");
  //Serial.println(destination[i]);

  //Serial.print(destination[1]);
   // Serial.println(">");
      
     // Serial.println("out");
    }         
   for(int i=0; i < NUM_AXIS; i++) {
   current_position[i]= destination[i] ;
   // Serial.println("curret");
}
   //       ///////////////////////////////////////
   //Serial.println("set galvo");
   // Serial.println(destination[0]);
   // Serial.println(destination[1]);
    galvo.setSignedXY( destination[0] , destination[1]);
    
  } 
  } 
  
 // galvo.setSignedXY( destination[0] , destination[1]);
  //delay(2.5);
//}
}


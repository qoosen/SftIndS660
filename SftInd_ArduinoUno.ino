// HONDA S660 (JW5) Shift indicator by qoosen
// CAN serial read for Arduino
// https://github.com/qoosen/ShiftIndicator
// File:SftInd_Ard.ino
// Arduino MPC2515 Library "MCP_CAN_lib / https://github.com/coryjfowler/MCP_CAN_lib" is required.


#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

//ID:0x255
unsigned int count_0x255 = 0;
unsigned int WhSpdFL_kph;   //Front Left wheel speed [1km/h / LSB] @165/55R15 Dia:563mm
unsigned int WhSpdFR_kph;   //Front Right wheel speed [1km/h / LSB]
unsigned int WhSpdRL_kph;   //Rear Left wheel speed [1km/h / LSB] @195/45R16 Dia:582mm
unsigned int WhSpdRR_kph;   //Rear Right wheel speed [1km/h / LSB]

//ID:0x17C
long unsigned int Ne;       //Engine speed [1rpm / LSB]
long unsigned int AccPos;   //Accelerator pedal position [% / LSB]
unsigned int GearPos;       //Estimated gear position [Speed / LSB]

//Gear position cal.
long unsigned int GearRatio;  //Total gear ratio [(kph/rpm) / 10^6 LSB]

#define CAN3_INT 2                              // Set INT to pin 2
MCP_CAN CAN3(10);                               // Set CS to pin 10

void setup()
{
  Serial.begin(115200);
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN3.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN3.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN3_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");
}

void loop()
{
  if(!digitalRead(CAN3_INT)) {                // If CAN3_INT pin is low, read receive buffer
    CAN3.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
  //  sprintf(msgString, "%.3X",rxId);
  //  Serial.print(msgString);

    if (rxId == 0x255) {
      if (count_0x255 == 0) {
        WhSpdFL_kph = rxBuf[0];
        WhSpdFR_kph = rxBuf[1];
        WhSpdRL_kph = rxBuf[2];
        WhSpdRR_kph = rxBuf[3];

        GearRatio = (WhSpdRL_kph + WhSpdRR_kph) * 1000000 / 2 / Ne;
        if (GearRatio < 2000) {
        GearPos = 0;
        }
        else if  (GearRatio < 8231) {
        GearPos = 1;
        }
        else if (GearRatio < 12454) {
          GearPos = 2;
        }
        else if (GearRatio < 17202) {
          GearPos = 3;
        }
        else if (GearRatio < 22810) {
          GearPos = 4;
        }
        else if (GearRatio < 29451) {
          GearPos = 5;
        }
        else {
          GearPos = 6;
        }   
        //Gear position send
        //sprintf(msgString, "%.1d", GearPos);    
        //Serial.print(msgString);

        //Message send
        sprintf(msgString, "%03d %03d %03d %03d %01d %04ld %03ld", 
        WhSpdFL_kph,
        WhSpdFR_kph,
        WhSpdRL_kph,
        WhSpdRR_kph,
        GearPos,
        Ne,
        AccPos);   
        Serial.print(msgString);

        // For debug  
        //  sprintf(msgString,"  %.10d",GearRatio);
        //  Serial.print(msgString);      
      
        //  sprintf(msgString,"  %.10d",AccPos);
        //  Serial.print(msgString);

        //  sprintf(msgString,"  %.10d",GearPos);
        //  Serial.print(msgString);
        
        Serial.println();
        count_0x255 = 0;
      }
      else {
        count_0x255--;
      }
    }
    else if (rxId == 0x17C) {
      //Engine speed Cal.
      Ne = (rxBuf[2]*256) + rxBuf[3];
      //Engine speed send
      //sprintf(msgString, "%.4d", Ne);
      //Serial.print(msgString);
      //Accelerator pedal position cal.
      AccPos = (rxBuf[0]*100) / 255;
      //Accelerator pedal position send      
      //sprintf(msgString, "%.3d", AccPos);
      //Serial.print(msgString);
      //Gear position cal.
    }
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
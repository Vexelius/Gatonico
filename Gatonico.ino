#include <MD_MAX72xx.h>
#include <SPI.h>

// Turn on debug statements to the serial output
#define  DEBUG  0

#if  DEBUG
#define  PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTD(x) Serial.println(x, DEC)

#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTD(x)

#endif

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define MAX_DEVICES 3

#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(CS_PIN, MAX_DEVICES);
// Arbitrary pins
// MD_MAX72XX mx = MD_MAX72XX(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// We always wait a bit between updates of the display
#define  DELAYTIME  100  // in milliseconds

// Sprite set definition
  uint8_t stdMouth[COL_SIZE] =
  {
    0b01100000,
    0b10010000,
    0b10000000,
    0b01000000,
    0b01000000,
    0b10000000,
    0b10010000,
    0b01100000
  };

  uint8_t tlkMouth[COL_SIZE] =
  {
    0b00100000,
    0b01010000,
    0b01000000,
    0b10100000,
    0b10100000,
    0b01000000,
    0b01010000,
    0b00100000
  };

    uint8_t blnkREye[COL_SIZE] =
  {
    0b00001100,
    0b00001110,
    0b00000110,
    0b00000110,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00000000
  };

    uint8_t blnkLEye[COL_SIZE] =
  {
    0b00000000,
    0b00011000,
    0b00001100,
    0b00000110,
    0b00000110,
    0b00000110,
    0b00001110,
    0b00001100
  };

    uint8_t idleREye[COL_SIZE] =
  {
    0b00000000,
    0b00111110,
    0b01111111,
    0b01100011,
    0b01111111,
    0b00111110,
    0b00000000,
    0b00000000
  };

    uint8_t idleLEye[COL_SIZE] =
  {
    0b00000000,
    0b00000000,
    0b00111110,
    0b01111111,
    0b01100011,
    0b01111111,
    0b00111110,
    0b00000000
  };

    uint8_t hrtREyeA[COL_SIZE] =
  {
    0b00000000,
    0b00001100,
    0b00011010,
    0b00111100,
    0b00011110,
    0b00001100,
    0b00000000,
    0b00000000
  };

    uint8_t hrtLEyeA[COL_SIZE] =
  {
    0b00000000,
    0b00000000,
    0b00001100,
    0b00011110,
    0b00111100,
    0b00011010,
    0b00001100,
    0b00000000
  };

    uint8_t hrtREyeB[COL_SIZE] =
  {
    0b00001110,
    0b00011111,
    0b00110111,
    0b01111010,
    0b00110111,
    0b00011111,
    0b00001110,
    0b00000000
  };

    uint8_t hrtLEyeB[COL_SIZE] =
  {
    0b00000000,
    0b00001110,
    0b00011111,
    0b00110111,
    0b01111010,
    0b00110111,
    0b00011111,
    0b00001110
  };

    uint8_t hrtMouth[COL_SIZE] =
  {
    0b01000000,
    0b10000000,
    0b10000000,
    0b01000000,
    0b01000000,
    0b10000000,
    0b10000000,
    0b01000000
  };

    uint8_t sadREye[COL_SIZE] =
  {
    0b00011000,
    0b00111000,
    0b00110000,
    0b00110000,
    0b00110000,
    0b00011000,
    0b00001100,
    0b00000000
  };

    uint8_t sadLEye[COL_SIZE] =
  {
    0b00000000,
    0b00001100,
    0b00011000,
    0b00110000,
    0b00110000,
    0b00110000,
    0b00111000,
    0b00011000
  };

    uint8_t sadMoutA[COL_SIZE] =
  {
    0b01000000,
    0b10000000,
    0b10000000,
    0b01000000,
    0b01000000,
    0b10000000,
    0b10000000,
    0b01000000
  };

    uint8_t sadMoutB[COL_SIZE] =
  {
    0b00100000,
    0b01000000,
    0b11000000,
    0b10100000,
    0b10100000,
    0b11000000,
    0b01000000,
    0b00100000
  };


//  Variables
int PulseSensorPurplePin = 0;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;            // Determine which Signal to "count as a beat", and which to ingore.


// The SetUp Function:
void setup() {
  mx.begin();
  Serial.begin(9600);         // Set's up Serial Communication at certain speed.
  mx.clear();

  mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, blnkLEye);  //Load Left Eye into 3rd led matrix
  mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, stdMouth);  //Load mouth into 2nd led matrix
  mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, blnkREye);  //Load Right Eye into 3rd led matrix
  delay(800);
  mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, blnkLEye);  //Load Left Eye into 3rd led matrix
  mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, tlkMouth);  //Load mouth into 2nd led matrix
  mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, blnkREye);  //Load Right Eye into 3rd led matrix
  delay(500);
  mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, idleLEye);  //Load Left Eye into 3rd led matrix
  mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, stdMouth);  //Load mouth into 2nd led matrix
  mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, idleREye);  //Load Right Eye into 3rd led matrix
  delay(500);
  mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, blnkLEye);  //Load Left Eye into 3rd led matrix
  mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, stdMouth);  //Load mouth into 2nd led matrix
  mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, blnkREye);  //Load Right Eye into 3rd led matrix
  delay(300);
  mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, idleLEye);  //Load Left Eye into 3rd led matrix
  mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, stdMouth);  //Load mouth into 2nd led matrix
  mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, idleREye);  //Load Right Eye into 3rd led matrix
  delay(500);
  
  scrollText("Hola!        ");

}

// The Main Loop Function
void loop() {

  Signal = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value.
                                              // Assign this value to the "Signal" variable.

   Serial.println(Signal);                    // Send the Signal value to Serial Plotter.

   mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);


   if(Signal > Threshold){                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
     //digitalWrite(LED13,HIGH);
     //mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, hrtLEyeB);
     //mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, stdMouth);
     //mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, hrtREyeB);
     mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
   } else {
     //digitalWrite(LED13,LOW);                //  Else, the sigal must be below "550", so "turn-off" this LED.
     //mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, hrtLEyeA);
     //mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, hrtMouth);
     //mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, hrtREyeA);
     mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
   }


delay(10);


}


// Text scroll function
void scrollText(char *p)
{
  uint8_t  charWidth;
  uint8_t cBuf[8];  // this should be ok for all built-in fonts

  mx.clear();

  while (*p != '\0')
  {
    charWidth = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);

    for (uint8_t i=0; i<charWidth + 1; i++) // allow space between characters
    {
      mx.transform(MD_MAX72XX::TSL);
      if (i < charWidth)
        mx.setColumn(0, cBuf[i]);
      delay(DELAYTIME);
    }
  }
}

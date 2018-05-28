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
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 

int BPMCounter = 0;
int AccurateBPM = 123;
char BPMTxt[4];
bool DemoMode = true;
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


// The SetUp Function:
void setup() {
  mx.begin();
  Serial.begin(9600);         // Set's up Serial Communication at certain speed.
  mx.clear();

    // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  //pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }

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
  mx.clear();
  

}

// The Main Loop Function
void loop() {

  if(DemoMode == true)
  {
      //Blink
      for(int exptst=0;exptst<3;exptst++)
      {
      mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, blnkLEye);  //Load Left Eye into 3rd led matrix
      mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, stdMouth);  //Load mouth into 2nd led matrix
      mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, blnkREye);  //Load Right Eye into 3rd led matrix
      delay(DELAYTIME*5);
      mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, idleLEye);  //Load Left Eye into 3rd led matrix
      mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, stdMouth);  //Load mouth into 2nd led matrix
      mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, idleREye);  //Load Right Eye into 3rd led matrix
  
      delay(DELAYTIME*10);
      }

      scrollText("Estoy listo!!!         ");
      DemoMode = false;
  
  }
  else
  {
  checkBPM();
  delay(20);  // considered best practice in a simple sketch.

  if(BPMCounter == 17)
  {
  Serial.print("Accurate BPM: ");                        // Print phrase "BPM: " 
  Serial.println(AccurateBPM);                        // Print the value inside of myBPM. 

  sprintf (BPMTxt, "%03i", AccurateBPM);
  scrollText(BPMTxt);
  delay(1200);
  scrollText("latidos/minuto          ");
  mx.clear();

  if(AccurateBPM <= 100)
  {
  for(int exptst=0;exptst<3;exptst++)
    {
  mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, hrtLEyeA);  //Load Left Eye into 3rd led matrix
  mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, stdMouth);  //Load mouth into 2nd led matrix
  mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, hrtREyeA);  //Load Right Eye into 3rd led matrix
  delay(DELAYTIME*10);
  mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, hrtLEyeB);  //Load Left Eye into 3rd led matrix
  mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, hrtMouth);  //Load mouth into 2nd led matrix
  mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, hrtREyeB);  //Load Right Eye into 3rd led matrix
  delay(DELAYTIME*10);
    }
    scrollText("Hasta pronto!        ");
    DemoMode = true;
  }

  if(AccurateBPM > 100)
  {
  for(int exptst=0;exptst<2;exptst++)
    {
  mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, sadLEye);  //Load Left Eye into 3rd led matrix
  mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, sadMoutA);  //Load mouth into 2nd led matrix
  mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, sadREye);  //Load Right Eye into 3rd led matrix
  delay(DELAYTIME*10);
  mx.setBuffer((1*COL_SIZE)-1, COL_SIZE, sadLEye);  //Load Left Eye into 3rd led matrix
  mx.setBuffer((2*COL_SIZE)-1, COL_SIZE, sadMoutB);  //Load mouth into 2nd led matrix
  mx.setBuffer((3*COL_SIZE)-1, COL_SIZE, sadREye);  //Load Right Eye into 3rd led matrix
  delay(DELAYTIME*10);
    }
    scrollText("Necesitas relajarte!        ");
    DemoMode = true;
  }
  
  
  BPMCounter = 0;
  delay(20);
  }
  }

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

void checkBPM()
{
   int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened". 
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);                        // Print the value inside of myBPM.
 BPMCounter++;
 if(BPMCounter == 17)
 {
  AccurateBPM = myBPM;
 }
}

}



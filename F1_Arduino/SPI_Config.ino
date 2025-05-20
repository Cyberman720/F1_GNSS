/*

Each address is labled 0-15, these corrospond to 0000, 0001 ect
for each address, we have a 32bit word that we need to put in it's place.
In theroy we can use defaults from the spec sheet to do this.

 The circuit:

  * CS - to digital pin 10  (CS pin)

  * DATA - to digital pin 11 (SDATA pin)

  * CLK - to digital pin 13 (SCK pin)
*/


//Commands per segment
/*
Address:0x0; Command:0xA2931A3; // LNA2 on, Antenna filter off
Address:0x1; Command:0x55128C; // CMOS logic, 2 bit I only
Address:0x2; Command:0xEAFE1DC; // Basic
Address:0x3; Command:0x9EC0008; // PLL ON
Address:0x4; Command:0x0C00080; //interger PLL
Address:0x5; Command:0x8000070; //Fractional-N PLL 16.368mhz, 1536
Address:0x6; Command:0x8000000; //Reserved
Address:0x7; Command:0x10061B2;
*/

// inslude the SPI library:
#include <SPI.h>

// set pin 10 as the chip select for the digital pot:
const int chipSelectPin = 10;

void setup() { 
  Serial.begin(9600);
  // set the chipSelectPin as an output:
  pinMode(chipSelectPin, OUTPUT);
  // pull it high
  digitalWrite(chipSelectPin, HIGH);
  // initialize SPI:
  SPI.begin();
}
 

void loop() {
    // set the target address
    uint8_t address = 0x7;
    // change the message on this address to the desired message:
    unsigned long message = 0x10061B2; // PLL int dev:  PLL ref ratio:
    Set_register(address, message);
    delay(100);
    Serial.println("Command Set");
    delay(10000);
  }


void Set_register(uint8_t addr, uint32_t value)
{
  // set message, shift, add address at end
  uint32_t data = (value << 4) | (addr);
  Serial.println(data);
  //convert hex to binary
  uint8_t bitsCount = sizeof( data ) * 8;
  char binary_string[ bitsCount + 1 ];
  itoa( data, binary_string, 2 );
  // create message
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
  digitalWrite(chipSelectPin, LOW);
  delay(1);
  Serial.println("chip Low");

  for(byte i = 0; i < 32; i++) { // per bit for whole register.
    SPI.transfer(binary_string[i]);
  }

  delay(1);
  digitalWrite(chipSelectPin, HIGH);
  Serial.println("chip High");
  SPI.endTransaction();

}


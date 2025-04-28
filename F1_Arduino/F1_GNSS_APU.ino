#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

int LD = A0; // potentiometer wiper (middle terminal) connected to analog pin 3
int I0 = A2;
int I1 = A3;
int Q0 = A4;
int Q1 = A5;
                    // outside leads to ground and +5V
int I0_read = 0;
int I1_read = 0;
int Q0_read = 0;
int Q1_read = 0;
int LD_read = 0;  // variable to store the value read

int I0_visual = 0;
int I1_visual = 0;
int Q0_visual = 0;
int Q1_visual = 0;
int LD_visual = 0;

float fractional_reference = 0;

void setup() {
  Serial.begin(9600);           //  setup serial
  matrix.begin();               // begins the Matrix LED
}

uint8_t frame[8][12] = {

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void loop() {
  LD_read = analogRead(LD);  // read the input pin
  I0_read = analogRead(I0);
  I1_read = analogRead(I1);
  Q0_read = analogRead(Q0);
  Q1_read = analogRead(Q1);
  lineloop(LD_read, 3);      // run the LED loop (value in, line on)
  lineloop(I0_read, 4);
  lineloop(I1_read, 5);
  lineloop(Q0_read, 6);
  lineloop(Q1_read, 7);
  matrix.renderBitmap(frame, 8, 12); // render
  Serial.println(LD_read);
}


void lineloop(int Read_value, int frame_x) {
  // function to have each read value be a bar on the board.
  fractional_reference = Read_value/1023; //become percentage of total value
  int visual = 12*fractional_reference;  //number of LED as percentage
  for (int i = 0; i < 12; i++) {
    if (i <= visual) {
      frame[frame_x][i] = 1;
    }
    else {
      frame[frame_x][i] = 0;
    }
  }
}

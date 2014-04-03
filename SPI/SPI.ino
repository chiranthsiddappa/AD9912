#include <Wire.h>
#include <string.h>
#include <SPI.h>

#define PB4 7
#define SPISCK PB4

#define PB6 23
#define SPIMISO PB6

#define PB7 24
#define SPIMOSI PB7

#define PB5 2
#define SPICS PB5

//test pin
#define curr_test_pin 4

// global type variables
unsigned char spi_data[4];
int serial_test = 25;
void setup()
{
  // put your setup code here, to run once:
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  Serial.begin(9600);
  // set pins for SPI bus here
  pinMode(SPISCK, OUTPUT);
  pinMode(SPIMISO, INPUT);
  pinMode(SPIMOSI, OUTPUT);
  pinMode(SPICS, OUTPUT);
  //other pin testing
  pinMode(curr_test_pin, INPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  //toggle blinky first
  digitalWrite(SPISCK, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(SPISCK, HIGH);
  digitalWrite(BLUE_LED, HIGH);
}

void flash_blue() {
  
  digitalWrite(BLUE_LED, LOW);
  delay(20);
  digitalWrite(BLUE_LED, HIGH);
  delay(20);
  digitalWrite(BLUE_LED, LOW);
}

void flash_red() {
  digitalWrite(RED_LED, LOW);
  delay(20);
  digitalWrite(RED_LED, HIGH);
  delay(20);
  digitalWrite(RED_LED, LOW);
}

void flash_green() {
  digitalWrite(GREEN_LED, LOW);
  delay(20);
  digitalWrite(GREEN_LED, HIGH);
  delay(20);
  digitalWrite(GREEN_LED, LOW);
}

void spi_transfer_test() {
  SPI.transfer('4');
}

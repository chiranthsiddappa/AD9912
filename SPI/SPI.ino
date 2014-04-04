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
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  //All done
  flash_green();
}

void loop()
{
  // put your main code here, to run repeatedly:
  //toggle blinky first
  digitalWrite(SPISCK, LOW);
  digitalWrite(SPISCK, HIGH);
  if(digitalRead(PUSH1) == LOW) {
    flash_red();
    delay(200);
    maxInt();
  }
  if(digitalRead(PUSH2) == LOW) {
    flash_red();
    delay(200);
    maxShort();
  }
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

void maxInt() {
  unsigned int maxInt = 0;
  Serial.println("\nMax Int: ");
  delay(20);
  maxInt = 0;
  maxInt -= 1;
  Serial.print(maxInt, HEX);
  Serial.println("");
  delay(10);
}

void maxShort() {
  unsigned short maxShort;
  Serial.print("\nMax Short: ");
  delay(20);
  maxShort = 0;
  maxShort -= 1;
  Serial.print(maxShort, HEX);
  Serial.println("");
  delay(10);
}

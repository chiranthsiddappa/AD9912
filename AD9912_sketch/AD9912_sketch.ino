#include <Wire.h>
#include <stdint.h>
#include <inttypes.h>
#include <SPI.h>
#include "AD9912.h"
#include "LiquidCrystal_I2C.h"
#include <math.h>
#include "AmplitudeCalibrationData.h"

#define PB4 0x7
#define SPISCK PB4

#define PB6 23
#define SPIMISO PB6

#define PB7 0xF
#define SPIMOSI PB7

#define PB5 0x2
#define SPICS PB5

#define PF3 GREEN_LED
#define IO_update PF3

#define PD0 0x17
#define PD1 0x18
#define I2C2SLA PD1
#define I2C2SCL PD0

#define GREEN_BUTTON PB_0
#define RED_BUTTON PD_2
#define UP_BUTTON PD_3
#define DOWN_BUTTON PE_1
#define LEFT_BUTTON PE_2
#define RIGHT_BUTTON PE_3

uint16_t partID_res;
uint16_t DAC_fsc_read;
uint16_t DAC_fsc_set;
uint32_t userFreq = 1;
uint16_t userDAC = 0;
uint32_t freqCursPos = 0;
uint32_t DACCursPos = 0;
uint8_t menuCursPos = 0;
uint8_t green_mode = 0;
uint8_t menu_mode = 0;
uint32_t resetFreq;
uint16_t resetDAC;
uint16_t demoState = 0x0;

#define green_freq_mode 0x0
#define green_dac_mode 0x1
#define menu_init 0x0
#define menu_ff 0x1
#define menu_demo 0x2
#define demo_freq_1_uc 0x1
#define demo_freq_2_uc 0x2
#define demo_freq_3_uc 0x3
#define demo_freq_4_uc 0x4
#define demo_freq_1_c 0x5
#define demo_freq_2_c 0x6
#define demo_freq_3_c 0x7
#define demo_freq_4_c 0x8
#define demo_DBM_match -4.904418

AD9912 ad9912;
LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup()
{
  delay(200);  
  //UI pins
  pinMode(GREEN_BUTTON, INPUT);
  pinMode(RED_BUTTON, INPUT);
  pinMode(UP_BUTTON, INPUT);
  pinMode(DOWN_BUTTON, INPUT);
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
  // I2C pins
  pinMode(I2C2SLA, INPUT_PULLUP);
  pinMode(I2C2SCL, INPUT_PULLUP);
  Wire.begin();
  Wire.setModule(3);
  lcd.init();
  lcd.backlight();
  lcd.print("Initializing...");
  // put your setup code here, to run once:
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  //Initialize other DDS pins
  pinMode(IO_update, OUTPUT);
  pinMode(SPISCK, OUTPUT);
  //  pinMode(SPIMISO, INPUT);
  pinMode(SPIMOSI, OUTPUT);
  pinMode(SPICS, OUTPUT);
  digitalWrite(SPICS, HIGH);
  digitalWrite(SPISCK, LOW);
  digitalWrite(SPIMOSI, LOW);
  ad9912.init(SPICS, SPISCK, SPIMOSI, SPIMISO, IO_update, 1000000000, 10000);
  lcd.clearRow(0);
  lcd.print("AD9912 Module Init");
  //push buttons
  pinMode(PUSH2, INPUT_PULLUP);
  lcd.setCursor(0,1);
  if(ad9912.read_PartID() == 0x1902) {
    lcd.print("Chip Found");
    flash_green();
    userFreq = ad9912.getFrequency();
    userDAC = ad9912.DAC_read();
    resetFreq = userFreq;
    resetDAC = userDAC;
    displayMenu();
  }
  else {
    lcd.print("Chip Not Found");
    flash_red();
  }
}

void loop()
{
  if(menu_mode == menu_init) {
    displayMenuCursor();
    if(digitalRead(GREEN_BUTTON) == HIGH) {
      if(menuCursPos == 0x0) {
	menu_mode = menu_ff;
	for(int i = 0; i < 4; i++) {
	  lcd.clearRow(i);
	}
	printUserFreq();
	printUserDAC();
	printUserDBM();
	printUserVpp();
      }
      else if (menuCursPos == 0x1) {
	menu_mode = menu_demo;
	resetFreq = userFreq;
	resetDAC = userDAC;
      }
    }
    else if(digitalRead(UP_BUTTON) == HIGH) {
      incMenuCursor();
      delay(200);
    }
    else if(digitalRead(DOWN_BUTTON) == HIGH) {
      decMenuCursor();
      delay(200);
    }
  }
  else if (menu_mode == menu_demo) {
    // demo stuff here
    if(digitalRead(RED_BUTTON) == HIGH) {
      menu_mode = menu_init;
      displayMenu();
      userFreq = resetFreq;
      userDAC = resetDAC;
      ad9912.setFrequency(userFreq);
      ad9912.DAC_write(userDAC);
      delay(200);
    }
    else if (digitalRead(GREEN_BUTTON) == HIGH) {
      if(demoState < 0x8) {
	++demoState;
      } else 
	demoState = demo_freq_1_uc;
      if(demoState == demo_freq_1_uc) {
	userFreq = 195 * pow(10 , 6);
	userDAC = 0x1FF;
      } else if (demoState == demo_freq_2_uc) {
	userFreq = 362 * pow(10 , 6);
	userDAC = 0x1FF;
      } else if (demoState == demo_freq_3_uc) {
	userFreq = 385 * pow(10 , 6);
	userDAC = 0x1FF;
      }
      else if (demoState == demo_freq_4_uc) {
	userFreq = 400 * pow(10 , 6);
	userDAC = 0x1FF;
      }
      else if (demoState == demo_freq_1_c) {
	userFreq = 195 * pow(10 , 6);
	//	userDAC = findBestDACMatch(userFreq , demo_DBM_match);
	userDAC = 180;
      }
      else if (demoState == demo_freq_2_c) {
	userFreq = 362 * pow(10 , 6);
	//	userDAC = findBestDACMatch(userFreq , demo_DBM_match);
	userDAC = 820;
      }
      else if (demoState == demo_freq_3_c) {
	userFreq = 385 * pow(10 , 6);
	//	userDAC = findBestDACMatch(userFreq , demo_DBM_match);
	userDAC = 590;
      }
      else if (demoState == demo_freq_4_c) {
	userFreq = 400 * pow(10 , 6);
	//	userDAC = findBestDACMatch(userFreq , demo_DBM_match);
	userDAC = 511;
      }
      // Constant display stuff here
      ad9912.setFrequency(userFreq);
      ad9912.DAC_write(userDAC);
      printUserFreq();
      printUserDAC();
      printUserDBM();
      printUserVpp();
      delay(150);
    }
  }
  else if (menu_mode == menu_ff) {
    //display cursor
    if(green_mode == green_freq_mode)
      dispFreqCurs();
    else
      dispDACCurs();
    //check for the green mode button
    if(digitalRead(GREEN_BUTTON) == HIGH) {
      green_mode = (green_mode == green_freq_mode) ? green_dac_mode : green_freq_mode;
      delay(200);
    }
    // ruld buttons
    if(green_mode == green_freq_mode) {
      if(digitalRead(UP_BUTTON) == HIGH) {
	incUserFreq();
	printUserFreq();
	ad9912.setFrequency(userFreq);
	printUserDBM();
	printUserVpp();
	delay(150);
      }
      if(digitalRead(DOWN_BUTTON) == HIGH) {
	decUserFreq();
	printUserFreq();
	ad9912.setFrequency(userFreq);
	printUserDBM();
	printUserVpp();
	delay(150);
      }
      if(digitalRead(LEFT_BUTTON) == HIGH) {
	incFreqCurs();
	delay(200);
      }
      if(digitalRead(RIGHT_BUTTON) == HIGH) {
	decFreqCurs();
	delay(200);
      }
    }
    else {
      if(digitalRead(UP_BUTTON) == HIGH) {
	incUserDAC();
	printUserDAC();
	ad9912.DAC_write(userDAC);
	printUserDBM();
	printUserVpp();
	delay(150);
      }
      if(digitalRead(DOWN_BUTTON) == HIGH) {
	decUserDAC();
	printUserDAC();
	ad9912.DAC_write(userDAC);
	printUserDBM();
	printUserVpp();
	delay(150);
      }
      if(digitalRead(LEFT_BUTTON) == HIGH) {
	incDACCurs();
	delay(200);
      }
      if(digitalRead(RIGHT_BUTTON) == HIGH) {
	decDACCurs();
	delay(200);
      }
    }
    if(digitalRead(RED_BUTTON) == HIGH) {
      menu_mode = menu_init;
      displayMenu();
    }
  }
}

void flash_green() {
  digitalWrite(GREEN_LED, LOW);
  delay(20);
  digitalWrite(GREEN_LED, HIGH);
  delay(20);
  digitalWrite(GREEN_LED, LOW);
}

void flash_red() {
  digitalWrite(RED_LED, LOW);
  delay(20);
  digitalWrite(RED_LED, HIGH);
  delay(20);
  digitalWrite(RED_LED, LOW);
}

uint ad9912_frequency_sweep() {
  uint64_t orig_FTW = ad9912.FTW_read();
  uint32_t FTW_set;
  for(FTW_set = 0x0; FTW_set <= 0xFFFFFFFFFFEul; FTW_set += 536870912l) {
    ad9912.FTW_write(FTW_set);
    if(digitalRead(PUSH2) == LOW) {
      ad9912.FTW_write(orig_FTW);
      return 0;
    }
  }
  ad9912.FTW_write(orig_FTW);
  return 1;
}

void LCDDisplayCurrentSettings() {
  uint8_t i;
  uint32_t freq = ad9912.getFrequency();
  float current = ad9912.getCurrent() * 1000;
  uint32_t DAC = ad9912.DAC_read();
  lcd.noCursor();
  lcd.noBlink();
  for(i = 0; i < 3; i++) {
    lcd.clearRow(i);
  }
  lcd.setCursor(0,0);
  lcd.print("Freq: ");
  for(i = 0; i < 9 - length(freq); i++) {
    lcd.print(" ");
  }
  lcd.print(freq);
  lcd.print("Hz");
  lcd.setCursor(0,1);
  lcd.print("DAC: ");
  for(i = 0; i < 4 - length(DAC); i++) {
    lcd.print(" ");
  }
  lcd.print(DAC);
  lcd.setCursor(0,2);
  lcd.print("Curr: ");
  for(i = 0; i < 2 - length(current); i++) {
    lcd.print(" ");
  }
  lcd.print(current, 4);
  lcd.print("mA");
}

uint32_t length(uint64_t x) {
  float flength = floor(log10(abs(x))) + 1;
  return (uint32_t) flength;
}

void printUserFreq() {
  uint8_t i;
  lcd.noCursor();
  lcd.clearRow(0);
  lcd.setCursor(0,0);
  lcd.print("Freq: ");
  for(i = 0; i < 9 - length(userFreq); i++) {
    lcd.print("0");
  }
  lcd.print(userFreq);
  lcd.print("Hz");
}

void printUserDAC() {
  uint8_t i;
  lcd.noCursor();
  lcd.clearRow(1);
  lcd.setCursor(0,1);
  lcd.print("DAC: ");
  for(i = 0; i < 4 - length(userDAC); i++) {
    lcd.print("0");
  }
  lcd.print(userDAC);
}

void printUserDBM() {
  lcd.noCursor();
  lcd.clearRow(2);
  if(userFreq >= ACD_start * pow(10 , 6) && userFreq <= ACD_stop * pow(10 , 6)) {
    lcd.setCursor(0,2);
    lcd.print("dBM: ");
    lcd.print(getDBM(userFreq , userDAC) , 4);
    lcd.print("dBM");
  }
}

void printUserVpp() {
  lcd.noCursor();
  lcd.clearRow(3);
  if(userFreq >= ACD_start * pow(10 , 6) && userFreq <= ACD_stop * pow(10 , 6)) {
    lcd.setCursor(0,3);
    lcd.print("Vpp: ");
    lcd.print(getVpp(userFreq , userDAC) , 4);
    lcd.print("V");
  }
}

void dispFreqCurs() {
  lcd.setCursor(14 - freqCursPos, 0);
  lcd.cursor();
}

void dispDACCurs() {
  lcd.setCursor(8 - DACCursPos, 1);
  lcd.cursor();
}

void incFreqCurs() {
  if(freqCursPos < 8) {
    ++freqCursPos;
  } else
    freqCursPos = 0;
}

void decFreqCurs() {
  if(freqCursPos > 0) {
    --freqCursPos;
  } else
    freqCursPos = 8;
}

void incDACCurs() {
  if(DACCursPos < 3) {
    ++DACCursPos;
  } else
    DACCursPos = 0;
}

void decDACCurs() {
  if(DACCursPos > 0) {
    --DACCursPos;
  } else
    DACCursPos = 3;
}

void incUserFreq() {
  uint32_t incAmount = (uint32_t) pow(10, freqCursPos);
  if(userFreq + incAmount <= 450000000)
    userFreq += incAmount;
}

void decUserFreq() {
  uint32_t decAmount = (uint32_t) pow(10, freqCursPos);
  if(decAmount <= userFreq)
    userFreq -= decAmount;
}

void incUserDAC() {
  uint32_t incAmount = (uint32_t) pow(10, DACCursPos);
  if(userDAC + incAmount <= 1023) 
    userDAC += incAmount;
}

void decUserDAC() {
  uint32_t decAmount = (uint32_t) pow(10, DACCursPos);
  if(decAmount <= userDAC)
    userDAC -= decAmount;
}

void displayMenu() {
  lcd.noCursor();
  lcd.noBlink();
  for(int i = 0; i <= 3; i++) {
    lcd.clearRow(i);
  }
  lcd.setCursor(0,0);
  lcd.print(" Full Function");
  lcd.setCursor(0,1);
  lcd.print(" Demo");
  displayMenuCursor();
}

void displayMenuCursor() {
  lcd.cursor();
  lcd.setCursor(0 , menuCursPos);
}

void incMenuCursor() {
  if(menuCursPos < 0x1) {
    ++menuCursPos;
  }
  else 
    menuCursPos = 0x0;
}

void decMenuCursor() {
  if(menuCursPos > 0x0) 
    --menuCursPos;
  else 
    menuCursPos = 0x1;
}

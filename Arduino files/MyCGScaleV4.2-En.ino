// Size of EEPROM
#define EESIZE 1024

#define TRUE 1
#define FALSE 0

//Include the graphics library.
#include "U8glib.h"
#include <AdKeyboard.h>
#include <HX711_ADC.h>
#include <EEPROM.h>
#include <Wire.h>

// pin for keyboard
AdKeyboard adKeyboard(6);

//HX711 constructor (dout pin, sck pin)
HX711_ADC ScaleFront(A2, A3); //HX711 pins front sensor (DOUT, PD_SCK)
HX711_ADC ScaleBack(A0, A1); //HX711 pins rear sensor (DOUT, PD_SCK)

//Initialize display.
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

//define values for scale
float SW_front = 10.0; //ScaleWeight
float SW_back = 10.0;
float SF_front = 3000.00; //ScaleFactor
float SF_back = 3000.00;
float SF_front_default = 3000.00;
float SF_back_default = 3000.00;
byte calibFrontOK;
byte calibBackOK;

// wings support
int DistSupPins;
int DistStopSup;
int CGoffset;
byte supportSelected;


//values for display with U8g font with helvR10 (11 pixels high font) , we have 64 pixels high
byte abst = 17 ;//number of pixels between the lines, including the interligne space (4 pixels)
byte line1 = 11;
byte line2 = line1 + 1 * abst ;
byte line3 = line1 + 2 * abst ;
byte line4 = line1 + 3 * abst ;

// declaration of the subroutine
void toOLED(const char* row1, const char* row2, const char* row3, const char* row4); //output to OLED using u8g lib
void displayMenu(byte numMenu, byte selectedItem); // to display the setup menu
int readBattVoltage(boolean *bWarn); //check batt value
int scaleFactorSupport (int supportSelected); // return CGoffset regarding the support selected


// menu configuration
byte selectedItem = 3; // default value = Exit menu
byte numMenu; // menu number
byte setupIsFinish = 0;

//define values for eeprom
byte adrSF_front = 0;  //adresses for EEPROM values
byte adrSF_back = 5;
byte adrTotalWeight = 10;
byte adrcalibFrontOK = 15;
byte adrcalibBackOK = 20;
byte adrSupportSelected = 25;

//various stuff
int batRefPin = A7;// pin to check BAT value
boolean bBatWarning = FALSE;
byte loc = 0;

// tare weight definition
word weight;
word incr = 1;
char str_weight[6] = "";
char str_incr[6] = {"1g"};

void setup(void)
{
  byte ScaleFront_rdy = 0;
  byte ScaleBack_rdy = 0;
  int stabilisingtime = 3000;

  //Serial.begin(9600);
  u8g.setFont(u8g_font_helvR10);

  // Reading the eeprom to get stored values
  EEPROM.get(adrTotalWeight, weight); // reference weight for calibration
  EEPROM.get(adrSupportSelected, supportSelected); // type of support selected
  EEPROM.get(adrcalibFrontOK, calibFrontOK);
  EEPROM.get(adrcalibBackOK, calibBackOK);
  if (calibFrontOK != 0) {
    EEPROM.get(adrSF_front, SF_front); // scale factor for Front
  };
  if (calibBackOK != 0) {
    EEPROM.get(adrSF_back, SF_back);   // scale factor for Back
  };
  //Serial.println((String)"L89  ref. weight=" + weight + "   supportSelected=" + supportSelected + "   calibFrontOK=" + calibFrontOK + "   calibBackOK=" + calibBackOK + "   SF_front=" + SF_front + "  SF_back=" + SF_back + "  CGoffset=" + CGoffset);

  // subroutine to listen the keyboard
  adKeyboard.setClickCallback(clickHandler);
  adKeyboard.setPressCallback(pressHandler);

  // starting the scales
  toOLED("CG SCALE", "", "Initialization", "in progress ...");
  ScaleFront.begin(); //start the Scales
  ScaleBack.begin();
  ScaleFront.start(2000); //Stabilize Scales
  ScaleBack.start(2000);

  while ((ScaleFront_rdy + ScaleBack_rdy) < 2) { //run startup, stabilisation and tare, both modules simultaneously
    if (!ScaleFront_rdy) ScaleFront_rdy = ScaleFront.startMultiple(stabilisingtime);
    if (!ScaleBack_rdy) ScaleBack_rdy = ScaleBack.startMultiple(stabilisingtime);
  }

  numMenu = 1;

  // menu loop
  while (setupIsFinish != 1) {
    adKeyboard.update();
    //Serial.println((String)"numMenu=" + numMenu);
    switch (numMenu) {
      case 0: // main loop, scale running
        CGoffset = scaleFactorSupport(supportSelected);  // run once setup is finish
        setupIsFinish = 1;
        break;

      case 1: // main menu
        displayMenu(numMenu, selectedItem);
        break;

      case 2: // support menu
        displayMenu(numMenu, selectedItem);
        break;

      case 3: // calibration menu
        displayMenu(numMenu, selectedItem);
        break;

      case 4: // tare definition menu
        toOLED("Tare value:", str_weight, "increment:", str_incr);
        break;

      case 5:
        break;

      case 6:
        break;
    } // end of switch
  } // end of while

  // finishing setting the scales
  ScaleFront.setCalFactor(SF_front); // set calibration factor
  ScaleBack.setCalFactor(SF_back); // set calibration factor
  ScaleFront.tareNoDelay();  //Calibweight has to be removed beforehand
  ScaleBack.tareNoDelay();
  toOLED("End of", "configuration", "Ready to weight", "a model.");
  delay(2000);


} // end of setup loop

void loop(void) {
  float CGratio;
  long int CG;
  long int weightTot;
  int printInterval = 500; // LCD/Serial refresh interval
  char weightLCD[17] = "displ";
  char CGtoLCD[17] = "displ";
  char batToLCD[17] = "";
  int t1;
  // u8g.setFont(u8g_font_unifont);

  ScaleFront.update();
  ScaleBack.update();

  if (t1 < millis()) {  //start calculation
    t1 = millis() + printInterval;
    SW_front = ScaleFront.getData();
    SW_back = ScaleBack.getData();

    weightTot = (SW_back + SW_front) * 100;

    if (SW_front > 5 && SW_back > 5) {
      SW_front = SW_front / 10;
      SW_back = SW_back / 10;
      CGratio = (((SW_back * 10000) / (SW_front + SW_back)));
      CG = ((((DistSupPins) * CGratio) / 1000) - ((DistSupPins * 10) / 2) + CGoffset);
    }
    else
      CG = 0;

    if (weightTot < 0 && weightTot >= -100)
      weightTot = 0;

    if (weightTot < -100)
      sprintf(weightLCD, "Error");
    else
      sprintf (weightLCD, "Weight: %ld g", weightTot / 100);

    if (CG != 0)
      sprintf(CGtoLCD, "CG: %ld mm", CG / 100);
    else
      sprintf(CGtoLCD, "No CG");

    // Battery value display
    int batVal = readBattVoltage(&bBatWarning);
    if (bBatWarning)
      sprintf(batToLCD, "Bat low: %d.%d V", (batVal / 1000), ((batVal % 1000) / 100), ((batVal % 100) / 10));
    else
      sprintf(batToLCD, "Bat ok: %d.%d V", (batVal / 1000), ((batVal % 1000) / 100), ((batVal % 100) / 10));

    //BatVal, Weight and Cg OLED display
    //Serial.println((String)"Bat: " + bat + " Weight: " + weightTot + " CG: " + CG);
    toOLED(batToLCD, "", weightLCD, CGtoLCD);

  } //end if...t1 millis
}


void clickHandler(int key) {

  if (key == 0 && numMenu != 4) {
    numMenu = 1;
  };

  // Main menu
  if (numMenu == 0) {
    switch (key) {
      case 0: // Left
        //Serial.println((String)"210: menu = " + numMenu + "   selectedItem = " + selectedItem);
        numMenu = 1;
        // selectedItem=1;
        // displayMenu(numMenu, selectedItem);
        break;
    };
  };

  if (numMenu == 1) {
    //Serial.println((String)"222: menu = " + numMenu + "   selectedItem = " + selectedItem);
    switch (key) {

      case 0: // Left
        // nothing to do
        break;

      case 1: // Up
        if (selectedItem > 1) {
          selectedItem = selectedItem - 1;
          //Serial.println((String)"149: menu = " + numMenu + "   selectedItem = " + selectedItem);
        };
        break;

      case 2: // Down
        if (selectedItem < 3) {
          selectedItem = selectedItem + 1;
          //Serial.println((String)"155: menu = " + numMenu + "   selectedItem = " + selectedItem);
        };
        break;

      case 3: // Right
        break;

      case 4: // Enter
        if (selectedItem == 1) { // menu support selected
          numMenu = 2;
          //Serial.println((String)"201: Going to the support menu");
        };
        if (selectedItem == 2) { // menu calibration selected
          numMenu = 3;
          //Serial.println((String)"205: Going to the calibration menu");
        };
        if (selectedItem == 3) { // quit the menu
          numMenu = 0;
          // setupIsFinish = 1;
          //Serial.println((String)"271: going to the main loop");
        };
    };
  };

  // support menu
  if (numMenu == 2) {
    //Serial.println((String)"264: menu = " + numMenu + "   selectedItem = " + selectedItem);
    switch (key) {

      case 0: // Left
        // nothing to do
        break;

      case 1: // Up
        if (selectedItem > 1) {
          selectedItem = selectedItem - 1;
          //Serial.println((String)"187: menu = " + numMenu + "   selectedItem = " + selectedItem);
        };
        break;

      case 2: // Down
        if (selectedItem < 3) {
          selectedItem = selectedItem + 1;
          //Serial.println((String)"193: menu = " + numMenu + "   selectedItem = " + selectedItem);
        };
        break;

      case 3: // Right
        // nothing to do
        break;

      case 4: // Enter
        if (selectedItem == 1) { // support 'petit' selected
          //Serial.println("201: Support selected : petit");
          supportSelected = 1;
        };
        if (selectedItem == 2) { // support 'moyen' selected
          //Serial.println("206: Support selected : moyen");
          supportSelected = 2;
        };
        if (selectedItem == 3) { // support 'grand' selected
          //Serial.println("211: Support selected : grand");
          supportSelected = 3;
        };
        EEPROM.put(adrSupportSelected, supportSelected);
        CGoffset = scaleFactorSupport(supportSelected);
    }
  };

  // Calibration menu
  // Serial.println((String)"menu = " + numMenu + "   selectedItem = " + selectedItem);
  if (numMenu == 3) {
    switch (key) {

      case 0: // Left
        // quit the calibration menu
        numMenu = 1;
        break;

      case 1: // Up
        if (selectedItem > 1) {
          selectedItem = selectedItem - 1;
          //Serial.println((String)"menu = " + numMenu + "   selectedItem = " + selectedItem);
        };
        break;

      case 2: // Down
        if (selectedItem < 3) {
          selectedItem = selectedItem + 1;
          //Serial.println((String)"menu = " + numMenu + "   selectedItem = " + selectedItem);
        };
        break;

      case 3: // Right
        if (selectedItem == 1) { // Tare configuration procedure
          //Serial.println((String)"Going to the tare configuration procedure");
          sprintf(str_weight, "%d%s", weight, "g");
          numMenu = 4;
        };
        if (selectedItem == 2) { // Front calibration procedure
          toOLED("Put the tare on", "the front scale", "Press 'Enter' 5s", "to start.");
          numMenu = 5;
        };
        if (selectedItem == 3) { // Back calibration procedure
          toOLED("Put the tare on", "the back scale", "Press 'Enter' 5s", "to start.");
          numMenu = 6;
        };
        break;

      case 4: // Enter
        // nothing to do for now
        break;
    }
  };

  if (numMenu == 4) {     // Tare weight definition menu
    //Serial.println((String)"253: weight=" + str_weight + "  incr=" + str_incr);
    switch (key) {

      case 0 : // Left
        // decrease increment
        switch (incr) {
          case 1:
            // nothing to do, already at the minimum
            break;
          case 10:
            incr = 1;
            sprintf(str_incr, "%d%s", incr, "g");
            break;
          case 100:
            incr = 10;
            sprintf(str_incr, "%d%s", incr, "g");
            break;
          case 1000:
            incr = 100;
            sprintf(str_incr, "%d%s", incr, "g");
            break;
            //Serial.println((String)"273: weight=" + str_weight + "  incr=" + str_incr);
        }
        break;

      case 1: // Up
        weight = weight + incr;
        sprintf(str_incr, "%d%s", incr, "g");
        sprintf(str_weight, "%d%s", weight, "g");
        //Serial.println((String)"337: weight=" + str_weight + "  incr=" + str_incr);
        break;

      case 2: // Down
        weight = weight - incr;
        sprintf(str_incr, "%d%s", incr, "g");
        sprintf(str_weight, "%d%s", weight, "g");
        //Serial.println((String)"343: weight=" + str_weight + "  incr=" + str_incr);
        break;

      case 3 : // Right
        // increase increment
        switch (incr) {
          case 1:
            incr = 10;
            sprintf(str_incr, "%d%s", incr, "g");
            break;
          case 10:
            incr = 100;
            sprintf(str_incr, "%d%s", incr, "g");
            break;
          case 100:
            incr = 1000;
            sprintf(str_incr, "%d%s", incr, "g");
            break;
          case 1000:
            // nothing to do, already at the maximum
            break;
            //Serial.println((String)"301: weight=" + str_weight + "  incr=" + str_incr);
        }
        break;

      case 4: // Enter
        break;
    } // end switch key

  } // end of if numMenu=4

} // end of clickHandler subroutine


void pressHandler(int key) {

  if (numMenu == 4 && key == 4) {
    toOLED("New weight", "of the tare", str_weight, "");
    EEPROM.put(adrTotalWeight, weight);
    delay(4000);
    numMenu = 3;
    selectedItem = 2;
  }

  if (numMenu == 5) {   // menu 5 => front scale calibration procedure

    switch (key) {
      case 0: // Left
        break;
      case 1: // Up
        break;
      case 2: // Down
        break;
      case 3: // Right
        break;
      case 4: // Enter
        int i = 0;
        ScaleFront.setCalFactor(SF_front_default); //set default to be faster
        ScaleFront.update();
        delay(200);
        SW_front = ScaleFront.getData();
        //Serial.println((String)"403: SW_front=" + SW_front);
        SF_front = ScaleFront.getCalFactor();
        //Serial.println((String)"405: SF_front=" + SF_front);
        int itr = 10;
        word diff;

        while (abs(SW_front - weight) >= 0.25) {
          ScaleFront.update();
          delay(200);
          diff = abs(SW_front - weight);
          if (i == 0) { //just to see on display that programm is not hanging
            toOLED("Front scale", "", "calibration", "in progress   |");
            toOLED("Front scale", "", "calibration", "in progress   /");
            i = 1;
          }
          else {
            i = 0;
            toOLED("Front scale", "", "calibration", "in progress   -");
            toOLED("Front scale", "", "calibration", "in progress   \\");
          }// ende else (i==0)

          if (diff >= 1000) {
            itr = 500;
          }
          if (diff >= 100 && diff < 1000 ) {
            itr = 100;
          }
          if (diff >= 50 && diff < 100 ) {
            itr = 25;
          }
          if (diff >= 7 && diff < 50) {
            itr = 5;
          }
          if (diff < 7 ) {
            itr = 1;
          }

          // Serial.println((String)"diff=" + diff + "  itr=" + itr + "  SW_front=" + SW_front + "  factor=" + SF_front);

          if (SW_front < weight) { //start autocalib
            //diff = abs(SW_front - weight);
            SF_front = SF_front - itr;
            ScaleFront.setCalFactor(SF_front);
          }
          if (SW_front > weight) {
            SF_front = SF_front + itr;
            ScaleFront.setCalFactor(SF_front);
          }
          SW_front = ScaleFront.getData();
        } // end while

        if (SF_front != SF_front_default) {
          // Serial.println((String)"460: we have values SF_front=" + SF_front + "   SW_front=" + SW_front);
          EEPROM.put(adrSF_front, SF_front);  //write calibrated value to EEPROM
          toOLED("Front scale", "calibration", "finished.", "");
          calibFrontOK = 1;
          EEPROM.put(adrcalibFrontOK, calibFrontOK);
          delay(2000);
        };

        break;
    } // end switch key

    numMenu = 3;
    selectedItem = 3;

  }// end menu 5


  if (numMenu == 6) {   // menu 6 => back scale calibration procedure

    switch (key) {
      case 0: // Left
        break;
      case 1: // Up
        break;
      case 2: // Down
        break;
      case 3: // Right
        break;
      case 4: // Enter
        int i = 0;
        ScaleBack.setCalFactor(SF_back_default); //set default to be faster
        ScaleBack.update();
        delay(200);
        SW_back = ScaleBack.getData();
        SF_back = ScaleBack.getCalFactor();
        int itr = 10;
        word diff;

        while (abs(SW_back - weight) >= 0.25) {
          ScaleBack.update();
          delay(200);
          diff = abs(SW_back - weight);
          if (i == 0) { //just to see on display that programm is not hanging
            toOLED("Back scale", "", "calibration", "in progress   |");
            toOLED("Back scale", "", "calibration", "in progress   /");
            i = 1;
          }
          else {
            i = 0;
            toOLED("Back scale", "", "calibration", "in progress   -");
            toOLED("Back scale", "", "calibration", "in progress   \\");
          }// ende else (i==0)

          if (diff >= 1000) {
            itr = 500;
          }
          if (diff >= 100 && diff < 1000 ) {
            itr = 100;
          }
          if (diff >= 50 && diff < 100 ) {
            itr = 25;
          }
          if (diff >= 7 && diff < 50) {
            itr = 5;
          }
          if (diff < 7 ) {
            itr = 1;
          }
          //Serial.println((String)"diff=" + diff + "  itr=" + itr + "  SW_back=" + SW_back + "  factor=" + SF_back);

          if (SW_back < weight) { //start autocalib
            SF_back = SF_back - itr;
            ScaleBack.setCalFactor(SF_back);
          }
          if (SW_back > weight) {
            SF_back = SF_back + itr;
            ScaleBack.setCalFactor(SF_back);
          }
          SW_back = ScaleBack.getData();
        }// end while

        if (SF_back != SF_back_default) {
          EEPROM.put(adrSF_back, SF_back);  //write calibrated value to EEPROM
          toOLED("Back scale", "calibration", "finished.", "");
          calibBackOK = 1;
          EEPROM.put(adrcalibBackOK, calibBackOK);
          delay(2000);
        };
        break;
    } // end switch key

    numMenu = 1;
    selectedItem = 3; // ready to quit the main menu

  } // end menu 6

}


void displayMenu(byte num, byte selectedItem) {
  char const *menuItem10 = "Configuration";
  char const *menuItem20 = " Support option";
  char const *menuItem30 = " Calibration";
  char const *menuItem21 = "Support : ";
  char const *menuItem22 = " Small";
  char const *menuItem23 = " Medium";
  char const *menuItem24 = " Large";
  char const *menuItem31 = " Tare config.";
  char const *menuItem32 = " Front calibration";
  char const *menuItem33 = " Back calibration";
  char const *menuItem40 = " Quit";
  char selected[20] = {""};
  char support[20] = {""};
  char selector[] = {'>'};
  char r1[22] ;
  char r2[22] ;
  char r3[22] ;
  char r4[22] ;
  if (supportSelected == 1) {
    strcpy(support, menuItem21);
    strcat(support, "small");
  }
  if (supportSelected == 2) {
    strcpy(support, menuItem21);
    strcat(support, "medium");
  }
  if (supportSelected == 3) {
    strcpy(support, menuItem21);
    strcat(support, "large");
  }

  switch (num) {

    case 1: // main menu
      //r1 = menuItem10;
      //r2 = menuItem20;
      //r3 = menuItem30;
      //r4 = menuItem40;
      strcpy(r1, menuItem10);
      strcpy(r2, menuItem20);
      strcpy(r3, menuItem30);
      strcpy(r4, menuItem40);
      break;

    case 2: // support menu
      //r1 = support;
      //r2 = menuItem22;
      //r3 = menuItem23;
      //r4 = menuItem24;
      strcpy(r1, support);
      strcpy(r2, menuItem22);
      strcpy(r3, menuItem23);
      strcpy(r4, menuItem24);
      break;

    case 3: // calibration menu
      //r1 = menuItem30;
      //r2 = menuItem31;
      //r3 = menuItem32;
      //r4 = menuItem33;
      strcpy(r1, menuItem30);
      strcpy(r2, menuItem31);
      strcpy(r3, menuItem32);
      strcpy(r4, menuItem33);
      break;
  }
  switch (selectedItem) {
    case 1:
      strcpy(selected, r2);
      selected[0] = '>';
      toOLED(r1, selected , r3, r4);
      break;
    case 2:
      strcpy(selected, r3);
      selected[0] = '>';
      toOLED(r1, r2 , selected, r4);
      break;
    case 3:
      strcpy(selected, r4);
      selected[0] = '>';
      toOLED(r1, r2 , r3, selected);
      break;
  }
} // end void displayMenu

void toOLED(const char* row1, const char* row2, const char* row3, const char* row4) {
  u8g.firstPage();
  do {
    u8g.drawStr(0, line1, row1);
    u8g.drawStr(0, line2, row2);
    u8g.drawStr(0, line3, row3);
    u8g.drawStr(0, line4, row4);
  } while (u8g.nextPage());
}//end void toOLED

int readBattVoltage(boolean * bWarn) { // read battery voltage
  long battvalue = 0;
  /*--- Double read to increase the stability ---*/
  battvalue += analogRead(batRefPin);
  battvalue += analogRead(batRefPin);
  /*--- Simple cross product and bride resistor divider value * 2 because we have done two analog read ---*/
  battvalue *= 4883L; // analog reading * (5.00V*1000000)/1024 (adjust value if VCC is not 5.0V)
  battvalue /= 640L; // this number comes from the resistor divider value ((R2/(R1+R2))*1000)/noof analogreadings (adjust value if required)
  /*--- Warning if battvalue < 5,5 V. Old Value 7,5 V ----*/
  if (battvalue < 5500) {
    *bWarn = TRUE;
  }

  return battvalue;
}

int scaleFactorSupport (int support) {
  switch (support) {
    case 1: // little support
      DistSupPins = 1450; //calibration value in 1/10mm, between the support pins
      DistStopSup = 285; //calibration value 1/10mm, projected distance from front wing support point to stopper pin
      break;
    case 2: // medium support
      DistSupPins = 1200;
      DistStopSup = 300;
      break;
    case 3: // big support
      DistSupPins = 1715;
      DistStopSup = 275;
      break;
  } // end switch
  CGoffset = ((DistSupPins / 2) + DistStopSup) * 10;  //offset calaculation as plane is not centrally above support points of scale
  //Serial.println((String)"722: CGoffset=" + CGoffset);
  return CGoffset;
}

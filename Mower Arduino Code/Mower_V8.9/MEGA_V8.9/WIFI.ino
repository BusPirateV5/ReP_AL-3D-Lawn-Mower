// Blynk TX and RX functions to communicate with the MEGA and NODEMCU
// Each transmission has a label n or p or q or 

// USE V5 Blynk  12th Feb 2019

void Get_WIFI_Commands() {
  Receive_Data_From_NODEMCU(); 
  delay(5); 
  Transmit_All_To_NODEMCU(); 
  }


void Receive_Data_From_NODEMCU() {
  while(Serial2.available()>0){
      val_WIFI = Serial2.parseInt();
      if(Serial2.read()== '\p'){
        delay(1000);
        Execute_Blynk_Command_To_Mower();
        }
  }
 }

void Receive_WIFI_Manuel_Commands() {
  while(Serial2.available()>0){
      val_WIFI = Serial2.parseInt();
      if(Serial2.read()== '\p'){
        delay(5);
        Execute_Manuel_Blynk_Command_To_Mower();
        }
  }
 }

void Transmit_All_To_NODEMCU() {
  delay(5);
  Serial2.print(Volts);
  Serial2.println("\g");
  delay(5);
  Serial2.print(Loop_Cycle_Mowing);
  Serial2.println("\c");
  delay(5);
  Serial2.print(Mower_Docked);
  Serial2.println("\d");
  delay(5);
  Serial2.print(Mower_Running);
  Serial2.println("\z");
  delay(5);
  Serial2.print(Mower_Parked);
  Serial2.println("\y");
  delay(5);
  Serial2.println(Charging);
  Serial2.println("\o");
  delay(5);
  Serial2.println(Tracking_Wire);
  Serial2.println("\m");   
  delay(5);
  Serial2.println(Update_APP_Buttons);
  Serial2.println("\b");   
  delay(5);   

  Serial.print(F("|BtnS"));
  Serial.print(Update_APP_Buttons);   
  }


void Transmit_APP_Buttons_Status() {

  delay(200);
  int Compass_Activate_TX = Compass_Activate + 2;
  int Compass_Heading_Hold_Enabled_TX = Compass_Heading_Hold_Enabled + 2;
  int GYRO_Enabled_TX = GYRO_Enabled + 2;
  
  Serial.println(F(""));
  Serial.print(F("|Updating APP Buttons"));
  delay(5);
  Serial2.print(Compass_Activate_TX);
  Serial2.println("\h");
  delay(10);
  Serial2.print(Compass_Heading_Hold_Enabled_TX);
  Serial2.println("\r");
  delay(10);
  Serial2.print(GYRO_Enabled_TX);
  Serial2.println("\w");
  delay(10);

  Serial.print(F("|Compass_Activate:"));
  Serial.print(Compass_Activate_TX);
  Serial.print("/");
  Serial.print(Compass_Activate);
  Serial.print("|");

  Serial.print(F("Heading Hold:"));
  Serial.print(Compass_Heading_Hold_Enabled_TX);
  Serial.print("/");
  Serial.print(Compass_Heading_Hold_Enabled);  
  Serial.print("|");  

  
  Serial.print(F("GYRO:"));
  Serial.print(GYRO_Enabled_TX);
  Serial.print("/");
  Serial.print(GYRO_Enabled);
  Serial.print("|"); 
  }

void Execute_Blynk_Command_To_Mower() {

// Updates the Serial Monitor with the latest Blynk Commands and can be used to start
// functions on the mower when the command is recieved.
delay(30);


// Exit Dock to Zone 1
 if (val_WIFI == 14) {
   if (Mower_Docked == 1)   {  

   // Update the TFT Display
   Serial.println(F(""));
   Serial.println(F("Updating TFT Screen: WIFI Command to Exit dock"));
   Exiting_Dock = 1;
   Mower_Error_Value = 0;
   Send_Mower_Docked_Data();                                   // Send the Docked TX Data package to the mower.
     
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.print(F("Exit Dock| "));
   Serial.println(F("Zone 1"));
   lcd.clear();
   lcd.print(F("WIFI Start"));
   lcd.setCursor(0,1);
   lcd.print(F("Exit Dock Z1"));
   delay(500);
   lcd.clear();
   Exit_Zone = 1;
   Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
   Manouver_Exit_To_Zone_X();
   }
   }

 // Quick Start Button in Blynk App
 if (val_WIFI == 13) {
   Serial.println("");
   Serial.print(F("WIFI Command:")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Quick Start"));
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(F("WIFI Start"));
   Serial.println(F("Quick Start"));
   if (Mower_Docked == 0) { 
     Manouver_Start_Mower();
     lcd.clear();    
     if (TFT_Screen_Menu == 1) Send_Mower_Docked_Data();    // Send the Docked TX Data package to the mower.
     }

     
   else Serial.println(F("Mower Docked - Quick Start not possible"));   
   }

// Go To Dock Button in Blynk App
 if (val_WIFI == 12) {
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Go To Dock"));
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(F("WIFI Go To Dock"));
   Menu_Mode_Selection = 0;                                      // Releases the loop in the membrane button section.
   Motor_Action_Stop_Spin_Blades();
   Motor_Action_Stop_Motors();
   delay(1000);
   lcd.clear();
   Manouver_Go_To_Charging_Station();    

   
   }

// STOP / Cancel Button in Blynk App
 if (val_WIFI == 11)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Pause/Stop"));
   Manouver_Park_The_Mower(); 
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

// Manuel Button in Blynk App
 if (val_WIFI == 15)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Manuel Mode"));
   Manouver_Park_The_Mower(); 
   delay(1000);
   Manouver_Manuel_Mode(); 
   Turn_On_Relay(); 
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

// Automatic RANDOM Button in Blynk App
 if (val_WIFI == 16)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Automatic Mode RANDOM"));
   lcd.clear();
   lcd.print("Auto Random");
   lcd.setCursor(0,1);
   lcd.print("Pattern");
   delay(200);
   lcd.clear();
   if (Mower_Running == 0) {
    Manouver_Park_The_Mower(); 
    Turn_On_Relay(); 
    }
   Pattern_Mow = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

// Automatic SPIRAL Button in Blynk App
 if (val_WIFI == 21)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Automatic Mode SPIRAL"));
   lcd.clear();
   lcd.print("Auto Spiral");
   lcd.setCursor(0,1);
   lcd.print("Pattern");
   delay(200);
   lcd.clear();
   if (Mower_Running == 0) {
    Manouver_Park_The_Mower(); 
    Turn_On_Relay(); 
    }
   Pattern_Mow = 2;

   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

// Automatic SPIRAL Button in Blynk App
 if (val_WIFI == 22)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Automatic Mode PARALLEL"));
   lcd.clear();
   lcd.print("Auto Parallel");
   lcd.setCursor(0,1);
   lcd.print("Pattern");
   delay(200);
   lcd.clear();
   if (Mower_Running == 0) {
    Manouver_Park_The_Mower(); 
    Turn_On_Relay(); 
    }
   Pattern_Mow = 1;

   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

// SETTINGS MOWER

// Compass Setting Via WIFI.
 if (val_WIFI == 40)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Compass ON"));
   Compass_Activate = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }
 if (val_WIFI == 41)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Compass OFF"));
   Compass_Activate = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

// Heading Hold
 if (val_WIFI == 42)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Heading Hold ON"));
   Compass_Heading_Hold_Enabled = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }
 if (val_WIFI == 43)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Heading Hold OFF"));
   Compass_Heading_Hold_Enabled = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

// GYRO
 if (val_WIFI == 44)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|GYRO ON"));
   GYRO_Enabled = 1;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }
 if (val_WIFI == 45)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|GYRO OFF"));
   GYRO_Enabled = 0;
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

}

void Execute_Manuel_Blynk_Command_To_Mower() {
// insert wheel motions here.

 if (val_WIFI == 16)  {    
   Serial.println("");
   Serial.print(F("WIFI Command: ")); 
   Serial.print(val_WIFI);
   Serial.println(F("|Automatic Mode"));
   Manouver_Park_The_Mower(); 
   Turn_On_Relay(); 
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

 if (val_WIFI == 17)  {    
   Serial.print(F("WIFI")); 
   Serial.print(val_WIFI);
   Serial.print(F("|Wheel Forward"));
   SetPins_ToGoForwards(); 
   Motor_Action_Go_Full_Speed();
   delay(300);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

 if (val_WIFI == 18)  {    
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.print(F("|Wheel Reverse"));
   SetPins_ToGoBackwards(); 
   Motor_Action_Go_Full_Speed();
   delay(300);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

 if (val_WIFI == 19)  {    
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.print(F("|Wheel Left"));
   SetPins_ToTurnLeft();
   Motor_Action_Go_Full_Speed();
   delay(200);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }

 if (val_WIFI == 20)  {    
   Serial.print(F("WIFI:")); 
   Serial.print(val_WIFI);
   Serial.print(F("|Wheel Right"));
   SetPins_ToTurnRight();
   Motor_Action_Go_Full_Speed();
   delay(200);
   Motor_Action_Stop_Motors();
   val_WIFI = 0;   // restes val2 to zero so the command is only executed once
  }
  
}


void Update_Blynk_Start_Conditions() {

  Serial2.print(Compass_Activate);
  Serial2.println("\a");
  delay(5);
  Serial2.print(Loop_Cycle_Mowing);
  Serial2.println("\b");
  delay(5);
  Serial2.print(Mower_Docked);
  Serial2.println("\c");
  delay(5);
  Serial2.print(Mower_Running);
  Serial2.println("\d");
  delay(5);
  Serial2.print(Mower_Parked);
  Serial2.println("\e");
  delay(5);
  Serial2.println(Charging);
  Serial2.println("\f");
  delay(5);
  Serial2.println(Tracking_Wire);
  Serial2.println("\g"); 

  

} 

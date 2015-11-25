/*
  History Changes
------------------------------------------------------------------------------------------------------
1.0     | MoCe  | Initial Revision.
1.1     | MoCe  | 
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Tone.h>
//#include <SD.h>
//include<stdlib.h>

#define SS_PIN 10    //Arduino Uno
//#define SS_PIN 7    //Arduino MEga
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.

int cardIsonPosition= 1;
String choferID = "";
String unidadID = "";
String SWver = "v0.5";
String diagCmd ="";
byte buffer[34];  
byte block;
byte status, len;
String option;
char optionFirstChar[2];
byte blockAddr;
byte size = sizeof(buffer);
int i;
boolean stringComplete;
Tone notePlayer[1];
MFRC522::MIFARE_Key key;

void setup() {
        Serial.begin(9600);        // Initialize serial communications with the PC
        SPI.begin();                // Init SPI bus
        mfrc522.PCD_Init();        // Init MFRC522 card
        Serial.println("          ##### PROGRAMADOR DE ID'S PARA TARJETAS DE CHOFER Y UNIDADES #####            ");

  /*notePlayer[0].begin(6);
  notePlayer[0].play(NOTE_D4);
  delay(200);
  notePlayer[0].play(NOTE_B4);
  delay(180);
  notePlayer[0].stop();
  delay(1000);
  notePlayer[0].play(NOTE_B2);
  delay(300);
  notePlayer[0].stop();
  delay(100);
  notePlayer[0].play(NOTE_B2);
  delay(180);
  notePlayer[0].stop();*/


}

void loop() {

  serialRequest();

  if (stringComplete) {
    if(diagCmd =="swver\n")
    {
     Serial.println(SWver);
      
    }
    /*if(diagCmd == "ls\n")
    {
       root = SD.open("/");
       printDirectory(root, 0);
    }*/

    if(diagCmd == "open log\n")
    {
     // readFile("ODOLOG.TXT");
    }

    if(diagCmd == "rm log\n")
    {
//      SD.remove("ODOLOG.TXT");
      Serial.println("ODOLOG.TXT removed");
    }
   // Serial.println(diagCmd);
    // clear the string:
    diagCmd = "";
    stringComplete = false;
  }


  
      
     /*   if(cardIsonPosition)
        {
          Serial.println("Coloca la tarjeta a programar cerca del lector :) ");
        }
        //mfrc522.PCD_Init();        // Init MFRC522 card
        cardIsonPosition = 0;    //Desactiva la visualizacion de la colocacion de la tarjeta
        
        // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        
        for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
        
        // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
                return;
        }

        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial())    return;
        
        Serial.print("Card UID:");    //Dump UID
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
        } 
        Serial.print(" PICC type: ");   // Dump PICC type
        byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
        Serial.println(mfrc522.PICC_GetTypeName(piccType));
         
      
        
        Serial.setTimeout(20000) ;     // wait until 20 seconds for input from serial
        Serial.println("ENTER ID");
        option = Serial.readStringUntil('\n');
        Serial.println(option[0]);  //DEBUG line
        
        
        
        if(option[0] == 'C')
        {
              eraseMemory();
              //Serial.setTimeout(20000);
            //  Serial.println("Teclead ID de chofer para esta tarjeta, despues Enter");
              //len=Serial.readBytesUntil('\n', (char *) buffer, 30) ; // read Chofer ID from serial
              len = option.length();
              option.getBytes(buffer,len+1);
              for (byte i = len+1; i < 30; i++) buffer[i] = '0';     // Complete Block with 0s
              
              block = 1;
              //Serial.println("Authenticating using key A...");
              status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
              if (status != MFRC522::STATUS_OK) 
              {
                 Serial.print("PCD_Authenticate() failed: :(");
                 Serial.println(mfrc522.GetStatusCodeName(status));
                 return;
              }
              
              // Write block
      	      status = mfrc522.MIFARE_Write(block, buffer, 16);
              if (status != MFRC522::STATUS_OK) 
              {
      	        Serial.print("MIFARE_Write() failed: :(");
      	        Serial.println(mfrc522.GetStatusCodeName(status));
                notePlayer[0].play(NOTE_B2);
                delay(300);
                notePlayer[0].stop();
                delay(100);
                notePlayer[0].play(NOTE_B2);
                delay(180);
                notePlayer[0].stop();
                return;
      	      }
              else// Serial.println("MIFARE_Write() success: :D ");
      
              block = 2;
              //Serial.println("Authenticating using key A...");
              status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
              if (status != MFRC522::STATUS_OK) 
              {
                 Serial.print("PCD_Authenticate() failed: :(");
                 Serial.println(mfrc522.GetStatusCodeName(status));
                 notePlayer[0].play(NOTE_B2);
                delay(300);
                notePlayer[0].stop();
                delay(100);
                notePlayer[0].play(NOTE_B2);
                delay(180);
                notePlayer[0].stop();
                 return;
              }
              
              // Write block
      	status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
      	if (status != MFRC522::STATUS_OK) 
      	{
      	    Serial.print("MIFARE_Write() failed: :(");
      	    Serial.println(mfrc522.GetStatusCodeName(status));
            notePlayer[0].play(NOTE_B2);
            delay(300);
            notePlayer[0].stop();
            delay(100);
            notePlayer[0].play(NOTE_B2);
            delay(180);
            notePlayer[0].stop();
            return;
      	}
              else
              {
                Serial.println("Chofer grabado");
                  notePlayer[0].play(NOTE_D4);
                  delay(200);
                  notePlayer[0].play(NOTE_B4);
                  delay(180);
                  notePlayer[0].stop();
              }
              
       cardIsonPosition= 1; //Visualiza de nuevo el letrero para colocar tarjeta
       
       //READ Chofer Block(1);
       
       blockAddr= 1;
       
        status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
          
       if (status != MFRC522::STATUS_OK) 
       {
        Serial.print("MIFARE_Read() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        notePlayer[0].play(NOTE_B2);
        delay(300);
        notePlayer[0].stop();
        delay(100);
        notePlayer[0].play(NOTE_B2);
        delay(180);
        notePlayer[0].stop();
       }
        for(i = 0; i < 4; i++)      //Vamos de la posicion 0 a la 3(4 bytes) para leer el ID del chofer GRabado
        {
          choferID += (char)buffer[i];
        }
        Serial.println("ID Grabado del Chofer:");
        Serial.println(choferID);
        
        choferID = "";
       }
        
        else if(option[0] == 'U')
        {
                        eraseMemory();
                         len = option.length();
                         option.getBytes(buffer,len+1);
                        // Ask personal data: ID de Unidad
                       // Serial.println("Teclea el ID de la unidad y presiona Enter");
                        //len=Serial.readBytesUntil('\n', (char *) buffer, 20) ; // read first name from serial
                        for (byte i = len+1; i < 20; i++) buffer[i] = '0';     // pad with spaces
                        
                        block = 4;
                        //Serial.println("Authenticating using key A...");
                        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
                        if (status != MFRC522::STATUS_OK) {
                           Serial.print("PCD_Authenticate() failed: ");
                           Serial.println(mfrc522.GetStatusCodeName(status));
                           notePlayer[0].play(NOTE_B2);
                            delay(300);
                            notePlayer[0].stop();
                            delay(100);
                            notePlayer[0].play(NOTE_B2);
                            delay(180);
                            notePlayer[0].stop();
                           return;
                        }
                        
                        // Write block
                	status = mfrc522.MIFARE_Write(block, buffer, 16);
                	if (status != MFRC522::STATUS_OK) {
                	    Serial.print("MIFARE_Write() failed: ");
                	    Serial.println(mfrc522.GetStatusCodeName(status));
                      notePlayer[0].play(NOTE_B2);
                      delay(300);
                      notePlayer[0].stop();
                      delay(100);
                      notePlayer[0].play(NOTE_B2);
                      delay(180);
                      notePlayer[0].stop();
                      return;
                	}
                       // else Serial.println("Unidad grabada");
                
                        block = 5;
                        //Serial.println("Authenticating using key A...");
                        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
                        if (status != MFRC522::STATUS_OK) {
                           Serial.print("PCD_Authenticate() failed: ");
                           Serial.println(mfrc522.GetStatusCodeName(status));
                           notePlayer[0].play(NOTE_B2);
                          delay(300);
                          notePlayer[0].stop();
                          delay(100);
                          notePlayer[0].play(NOTE_B2);
                          delay(180);
                          notePlayer[0].stop();
                           return;
                        }
                        
                        // Write block
                	status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
                	if (status != MFRC522::STATUS_OK) {
                	    Serial.print("MIFARE_Write() failed: ");
                	    Serial.println(mfrc522.GetStatusCodeName(status));
                      notePlayer[0].play(NOTE_B2);
                      delay(300);
                      notePlayer[0].stop();
                      delay(100);
                      notePlayer[0].play(NOTE_B2);
                      delay(180);
                      notePlayer[0].stop();
                      return;
                	}
                        else
                        {
                            Serial.println("Unidad grabada");
                            notePlayer[0].play(NOTE_D4);
                            delay(200);
                            notePlayer[0].play(NOTE_B4);
                            delay(180);
                            notePlayer[0].stop();
                        }
                        
        cardIsonPosition= 1;  //Visualiza de nuevo el letrero para colocar tarjeta
        
               //READ Vehicle Block(4);
       
       blockAddr= 4;
       
        status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
          
       if (status != MFRC522::STATUS_OK) 
       {
        Serial.print("MIFARE_Read() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        notePlayer[0].play(NOTE_B2);
        delay(300);
        notePlayer[0].stop();
        delay(100);
        notePlayer[0].play(NOTE_B2);
        delay(180);
        notePlayer[0].stop();
       }
        for(i = 0; i < 4; i++)      //Vamos de la posicion 0 a la 3(4 bytes) para leer el ID del chofer GRabado
        {
          unidadID += (char)buffer[i];
        }
        Serial.println("ID Grabado de la Unidad:");
        Serial.println(unidadID);
        unidadID = "";
        
       }
        
        else 
        {
          Serial.println("Selecciona una opcion valida");
          option="";

        }

        Serial.println(" ");
        mfrc522.PICC_HaltA(); // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
       */
}


bool eraseMemory()
{
  //Erase memory sequence, this will erase Unidad ID and Chofer ID to be sure none of them is available before writing to memory
 
 //-------------------Erasing Chofer ID---------------------

 
 for (byte i = 0; i < 30; i++) buffer[i] = '0';     // Complete Block with 0s
  
  block = 1;
              //Serial.println("Authenticating using key A...");
              status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
              if (status != MFRC522::STATUS_OK) 
              {
                 Serial.print("PCD_Authenticate() failed: :(");
                 Serial.println(mfrc522.GetStatusCodeName(status));
                 notePlayer[0].play(NOTE_B2);
                  delay(300);
                  notePlayer[0].stop();
                  delay(100);
                  notePlayer[0].play(NOTE_B2);
                  delay(180);
                  notePlayer[0].stop();
                 return 0;
              }
              
              // Write block
              status = mfrc522.MIFARE_Write(block, buffer, 16);
              if (status != MFRC522::STATUS_OK) 
              {
                Serial.print("MIFARE_Write() failed: :(");
                Serial.println(mfrc522.GetStatusCodeName(status));
                notePlayer[0].play(NOTE_B2);
                delay(300);
                notePlayer[0].stop();
                delay(100);
                notePlayer[0].play(NOTE_B2);
                delay(180);
                notePlayer[0].stop();
                return 0;
              }
           //   else Serial.println("MIFARE_Write() success: :D ");
      
              block = 2;
              //Serial.println("Authenticating using key A...");
              status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
              if (status != MFRC522::STATUS_OK) 
              {
                 Serial.print("PCD_Authenticate() failed: :(");
                 Serial.println(mfrc522.GetStatusCodeName(status));
                 notePlayer[0].play(NOTE_B2);
                delay(300);
                notePlayer[0].stop();
                delay(100);
                notePlayer[0].play(NOTE_B2);
                delay(180);
                notePlayer[0].stop();
                 return 0;
              }
              
              // Write block
        status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
        if (status != MFRC522::STATUS_OK) {
            Serial.print("MIFARE_Write() failed: :(");
            Serial.println(mfrc522.GetStatusCodeName(status));
            notePlayer[0].play(NOTE_B2);
            delay(300);
            notePlayer[0].stop();
            delay(100);
            notePlayer[0].play(NOTE_B2);
            delay(180);
            notePlayer[0].stop();
            return 0;
        }
              else Serial.println("Chofer Borrado");
              //return 1;

//-------------------Erasing Unidad ID---------------------

                     for (byte i = 0; i < 20; i++) buffer[i] = '0';     // pad with spaces
                        
                        block = 4;
                        //Serial.println("Authenticating using key A...");
                        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
                        if (status != MFRC522::STATUS_OK) {
                           Serial.print("PCD_Authenticate() failed: ");
                           Serial.println(mfrc522.GetStatusCodeName(status));
                           notePlayer[0].play(NOTE_B2);
                            delay(300);
                            notePlayer[0].stop();
                            delay(100);
                            notePlayer[0].play(NOTE_B2);
                            delay(180);
                            notePlayer[0].stop();
                           return 0;
                        }
                        
                        // Write block
                  status = mfrc522.MIFARE_Write(block, buffer, 16);
                  if (status != MFRC522::STATUS_OK) {
                      Serial.print("MIFARE_Write() failed: ");
                      Serial.println(mfrc522.GetStatusCodeName(status));
                      notePlayer[0].play(NOTE_B2);
                      delay(300);
                      notePlayer[0].stop();
                      delay(100);
                      notePlayer[0].play(NOTE_B2);
                      delay(180);
                      notePlayer[0].stop();
                      return 0;
                  }
                     //   else Serial.println("MIFARE_Write() success: ");
                
                        block = 5;
                        //Serial.println("Authenticating using key A...");
                        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
                        if (status != MFRC522::STATUS_OK) {
                           Serial.print("PCD_Authenticate() failed: ");
                           Serial.println(mfrc522.GetStatusCodeName(status));
                           notePlayer[0].play(NOTE_B2);
                            delay(300);
                            notePlayer[0].stop();
                            delay(100);
                            notePlayer[0].play(NOTE_B2);
                            delay(180);
                            notePlayer[0].stop();
                           return 0;
                        }
                        
                        // Write block
                  status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
                  if (status != MFRC522::STATUS_OK) {
                      Serial.print("MIFARE_Write() failed: ");
                      Serial.println(mfrc522.GetStatusCodeName(status));
                      notePlayer[0].play(NOTE_B2);
                      delay(300);
                      notePlayer[0].stop();
                      delay(100);
                      notePlayer[0].play(NOTE_B2);
                      delay(180);
                      notePlayer[0].stop();
                      return 0;
                  }
                       else{
                        Serial.println("Unidad Borrado");
                        return 1;}
        
}


void serialRequest()
{
 // Serial.println("H");
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    diagCmd += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }

}

boolean r(int dir, char str[])
{




}

boolean w(int dir, char str[])
{


  
}



/*
 * Write personal data of a MIFARE RFID card using a RFID-RC522 reader
 * Uses MFRC522 - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT. 
 ----------------------------------------------------------------------------- 
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               52                MOSI
 * SPI MISO   12               51                MISO
 * SPI SCK    13               50                SCK
 *
 * Hardware required:
 * Arduino
 * PCD (Proximity Coupling Device): NXP MFRC522 Contactless Reader IC
 * PICC (Proximity Integrated Circuit Card): A card or tag using the ISO 14443A interface, eg Mifare or NTAG203.
 * The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com. 
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 7    //Arduino Uno
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.

int cardIsonPosition= 1;
String choferID = "";
String unidadID = "";

void setup() {
        Serial.begin(9600);        // Initialize serial communications with the PC
        SPI.begin();                // Init SPI bus
        mfrc522.PCD_Init();        // Init MFRC522 card
        Serial.println("          ##### PROGRAMADOR DE ID'S PARA TARJETAS DE CHOFER Y UNIDADES #####            ");
}

void loop() {
      
        if(cardIsonPosition)
        {
          Serial.println("Coloca la tarjeta a programar cerca del lector :) ");
        }
        //mfrc522.PCD_Init();        // Init MFRC522 card
        cardIsonPosition = 0;    //Desactiva la visualizacion de la colocacion de la tarjeta
        
        // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        MFRC522::MIFARE_Key key;
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
         
        byte buffer[34];  
        byte block;
        byte status, len;
        String option;
        byte blockAddr;
        byte size = sizeof(buffer);
        int i;
        
        Serial.setTimeout(20000) ;     // wait until 20 seconds for input from serial
        Serial.println("Teclea la opcion deseada: \n[1]Grabar ID Chofer \n[2]Grabar ID Unidad \nPresiona Enter al finalizar");
        option = Serial.readStringUntil('\n');
       // Serial.println(option);  //DEBUG line
        
        if(option == "1")
        {
              Serial.setTimeout(20000);
              Serial.println("Teclead ID de chofer para esta tarjeta, despues Enter");
              len=Serial.readBytesUntil('\n', (char *) buffer, 30) ; // read family name from serial
              for (byte i = len; i < 30; i++) buffer[i] = '0';     // Complete Block with 0s
              
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
                return;
      	      }
              else Serial.println("MIFARE_Write() success: :D ");
      
              block = 2;
              //Serial.println("Authenticating using key A...");
              status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
              if (status != MFRC522::STATUS_OK) 
              {
                 Serial.print("PCD_Authenticate() failed: :(");
                 Serial.println(mfrc522.GetStatusCodeName(status));
                 return;
              }
              
              // Write block
      	status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
      	if (status != MFRC522::STATUS_OK) {
      	    Serial.print("MIFARE_Write() failed: :(");
      	    Serial.println(mfrc522.GetStatusCodeName(status));
                  return;
      	}
              else Serial.println("MIFARE_Write() success: :D");
              
       cardIsonPosition= 1; //Visualiza de nuevo el letrero para colocar tarjeta
       
       //READ Chofer Block(1);
       
       blockAddr= 1;
       
        status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
          
       if (status != MFRC522::STATUS_OK) 
       {
        Serial.print("MIFARE_Read() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
       }
        for(i = 0; i < 4; i++)      //Vamos de la posicion 0 a la 3(4 bytes) para leer el ID del chofer GRabado
        {
          choferID += (char)buffer[i];
        }
        Serial.println("ID Grabado del Chofer:");
        Serial.println(choferID);
        
        choferID = "";
       }
        
        else if(option == "2")
        {
                        // Ask personal data: First name
                        Serial.println("Teclea el ID de la unidad y presiona Enter");
                        len=Serial.readBytesUntil('\n', (char *) buffer, 20) ; // read first name from serial
                        for (byte i = len; i < 20; i++) buffer[i] = '0';     // pad with spaces
                        
                        block = 4;
                        //Serial.println("Authenticating using key A...");
                        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
                        if (status != MFRC522::STATUS_OK) {
                           Serial.print("PCD_Authenticate() failed: ");
                           Serial.println(mfrc522.GetStatusCodeName(status));
                           return;
                        }
                        
                        // Write block
                	status = mfrc522.MIFARE_Write(block, buffer, 16);
                	if (status != MFRC522::STATUS_OK) {
                	    Serial.print("MIFARE_Write() failed: ");
                	    Serial.println(mfrc522.GetStatusCodeName(status));
                            return;
                	}
                        else Serial.println("MIFARE_Write() success: ");
                
                        block = 5;
                        //Serial.println("Authenticating using key A...");
                        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
                        if (status != MFRC522::STATUS_OK) {
                           Serial.print("PCD_Authenticate() failed: ");
                           Serial.println(mfrc522.GetStatusCodeName(status));
                           return;
                        }
                        
                        // Write block
                	status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
                	if (status != MFRC522::STATUS_OK) {
                	    Serial.print("MIFARE_Write() failed: ");
                	    Serial.println(mfrc522.GetStatusCodeName(status));
                            return;
                	}
                        else Serial.println("MIFARE_Write() success: ");
                        
        cardIsonPosition= 1;  //Visualiza de nuevo el letrero para colocar tarjeta
        
               //READ Vehicle Block(4);
       
       blockAddr= 4;
       
        status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
          
       if (status != MFRC522::STATUS_OK) 
       {
        Serial.print("MIFARE_Read() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
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
          Serial.println("Selecciona una opcion valida");

        Serial.println(" ");
        mfrc522.PICC_HaltA(); // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
       
}

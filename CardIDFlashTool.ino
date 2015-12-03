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


    byte dataBlock[]    = {
        0x00, 0x00, 0x00, 0x00, //  0, 0, 0, 0,
        0x00, 0x00, 0x00, 0x00, //  0, 0, 0, 0,
        0x00, 0x00, 0x00, 0x00, //  0, 0, 0, 0,
        0x00, 0x00, 0x00, 0x00  //  0, 0, 0, 0
    };
byte sector;
byte trailerBlock;
int BlockNumber;
int cardIsonPosition= 1;
char data_to_write[16];
String SWver = "v2.8 03/12/15";
String strCmd = "";
String diagCmd = "";
//byte buffer[34];  
//byte status, len;
byte blockAddr;
//byte size = sizeof(buffer);
int i;
boolean stringComplete;
Tone notePlayer[1];
MFRC522::MIFARE_Key key;

void setup() {
      Serial.begin(9600); // Initialize serial communications with the PC
    SPI.begin();        // Init SPI bus
      //  mfrc522.PCD_Init(); // Init MFRC522 card

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

   // Serial.println("Scan a MIFARE Classic PICC to demonstrate read and write.");
   // Serial.print("Using key (for A and B):");
   // dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
  //  Serial.println();
    
   // Serial.println("BEWARE: Data will be written to the PICC, in sector #1");
        Serial.println(F("          ##### RFID TAG READER/WRITER #####            "));

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

    if(diagCmd[0] == 'r' && diagCmd[1]== '(')  //We received a read Block Command
    {
      i=2;                                     //Start in Block number byte(2)
      mfrc522.PCD_Init(); // Init MFRC522 card
      //Serial.println(F("Read command"));          //Debug
      while(diagCmd[i] != ')' )                 //Read Block number until ')' is detected
      {
        strCmd = strCmd + diagCmd[i];
        i++;
        

        if(i > 4)
        {
         // Serial.println(i);
          Serial.println(F("Error 501"));          //Error 501 means parenthesis is not closed in function or numer exceeds the 2 digits limit
          break;
          //Serial.println(diagCmd);
        }
      }
      
      //Serial.println(strCmd);
      BlockNumber = strCmd.toInt();

      if(BlockNumber < 4)
      {
        sector = 0;
      }

      else if(BlockNumber < 8)
      {
        sector = 1;
      }

      else if(BlockNumber < 12)
      {
        sector = 2;
      }

      else if(BlockNumber < 16)
      {
        sector = 3;
      }

      else if(BlockNumber < 20)
      {
        sector = 4;
      }

      else if(BlockNumber < 24)
      {
        sector = 5;
      }

      else if(BlockNumber < 28)
      {
        sector = 6;
      }

      else if(BlockNumber < 32)
      {
        sector = 7;
      }

      else if(BlockNumber < 36)
      {
        sector = 8;
      }

      else if(BlockNumber < 40)
      {
        sector = 9;
      }

      else if(BlockNumber < 44)
      {
        sector = 10;
      }

      else if(BlockNumber < 48)
      {
        sector = 11;
      }

      else if(BlockNumber < 52)
      {
        sector = 12;
      }

      else if(BlockNumber < 56)
      {
        sector = 13;
      }

      else if(BlockNumber < 60)
      {
        sector = 14;
      }

      else if(BlockNumber < 64)
      {
        sector = 15;
      }
      /*Serial.print("Sector: ");
      Serial.println(sector);
      Serial.print("Block: ");
      Serial.println(BlockNumber);*/
    
      r(BlockNumber);
        strCmd = "";
      i = 0;  //Resetting counter
      
      
    }

    if(diagCmd[0] == 'w' && diagCmd[1] == '(')
    {
        mfrc522.PCD_Init(); // Init MFRC522 card
      //Serial.println(F("Write Command"));
      i=2;                                     //Start in Block number byte(2)
       while(diagCmd[i] != ',' )                 //Read Block number until ')' is detected
      {
        strCmd = strCmd + diagCmd[i];
        i++;
        

        if(i > 4)
        {
         // Serial.println(i);
          Serial.println(F("Error 501"));          //Error 501 means parenthesis is not closed in function or numer exceeds the 2 digits limit
          break;
          //Serial.println(diagCmd);
        }
      }
      
      //Serial.println(strCmd);
      BlockNumber = strCmd.toInt();

  if(BlockNumber < 4)
      {
        sector = 0;
      }

      else if(BlockNumber < 8)
      {
        sector = 1;
      }

      else if(BlockNumber < 12)
      {
        sector = 2;
      }

      else if(BlockNumber < 16)
      {
        sector = 3;
      }

      else if(BlockNumber < 20)
      {
        sector = 4;
      }

      else if(BlockNumber < 24)
      {
        sector = 5;
      }

      else if(BlockNumber < 28)
      {
        sector = 6;
      }

      else if(BlockNumber < 32)
      {
        sector = 7;
      }

      else if(BlockNumber < 36)
      {
        sector = 8;
      }

      else if(BlockNumber < 40)
      {
        sector = 9;
      }

      else if(BlockNumber < 44)
      {
        sector = 10;
      }

      else if(BlockNumber < 48)
      {
        sector = 11;
      }

      else if(BlockNumber < 52)
      {
        sector = 12;
      }

      else if(BlockNumber < 56)
      {
        sector = 13;
      }

      else if(BlockNumber < 60)
      {
        sector = 14;
      }

      else if(BlockNumber < 64)
      {
        sector = 15;
      }
    //  Serial.print(F("Sector: "));
      //Serial.println(sector);
      //Serial.print(F("Block: "));
      //Serial.println(BlockNumber);
      trailerBlock =sector+3*(sector+1);
      //Serial.print("TRailer Block: ");
      //Serial.println(trailerBlock);
      if(BlockNumber == trailerBlock)
      {
        Serial.println(F("TR_NA"));
        play_wrong();
      }
      else
      {
        //Serial.print(F("Byte i:"));
        //Serial.println(i);
        strCmd = "";
        i++;
        while(diagCmd[i] != ')' )                 //Read Block number until ')' is detected
        {
          strCmd = strCmd + diagCmd[i];
          i++;
          
  
          if(i > 20)
          {
           // Serial.println(i);
            Serial.println(F("Error 501"));          //Error 501 means parenthesis is not closed in function or numer exceeds the 16 bytes of data(Limit of Block is 16 bytes)
            Serial.println(F("W_NOK"));
            play_wrong();
            break;
            //Serial.println(diagCmd);
          }
        }
       // Serial.print(F("Data to be written: "));
        
        strCmd.toCharArray(data_to_write,16);
        //Serial.println(data_to_write);
        w(BlockNumber,data_to_write);
      }

       strCmd = "";
      i = 0;  //Resetting counter
    }

    diagCmd = "";
    stringComplete = false;
  }

 

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

boolean r(int dir)
{

 // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
        play_wrong();
        return 0;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
        play_wrong();
        return 0;
    }
    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Check for compatibility
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("This sample only works with MIFARE Classic cards."));
        return 0;
    }

    // In this sample we use the second sector,
    // that is: sector #1, covering block #4 up to and including block #7

    byte blockAddr      = dir;


    byte status;
    byte buffer[18];
    byte size = sizeof(buffer);

    // Authenticate using key A
    Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return 0;
    }

    // Show the whole sector as it currently is
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    // Read data from the block
    Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(" ...");
    status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(":");
    dump_byte_array(buffer, 16); Serial.println();
    Serial.println();

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
       
    Serial.println(F("R_OK"));
    return 1; 
}

boolean w(int dir, char str[])
{
  

 // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
        play_wrong();
        Serial.println(F("W_NOK"));
        return 0;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
        play_wrong();
        Serial.println(F("W_NOK"));
        return 0;
    }

    // Show some details of the PICC (that is: the tag/card)
    //Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    //Serial.println();
    //Serial.print(F("PICC type: "));
    byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
   // Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Check for compatibility
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
     //   Serial.println(F("This sample only works with MIFARE Classic cards."));
        return 0;
    }

    // In this sample we use the second sector,
    // that is: sector #1, covering block #4 up to and including block #7

    byte blockAddr      = dir;
    //Serial.print(F("Block ADDRESS: "));
    //Serial.println(blockAddr);

    byte status;
    byte buffer[18];
    byte size = sizeof(buffer);

    // Authenticate using key A
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        play_wrong();
        Serial.println(F("W_NOK"));
        return 0;
    }

    
   // Authenticate using key B
    //Serial.println(F("Authenticating again using key B..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        play_wrong();
        Serial.println(F("W_NOK"));
        return 0;
    }

    // Write data to the block
  //
   for(i=0;i< strlen(str);i++)
    {
      buildBlock(i,str[i]);
    //  Serial.println(str[i]);
    }

   // Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
    //Serial.println(" ...");
    //dump_byte_array(dataBlock, 16); Serial.println();
    status = mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {
      //  Serial.print(F("MIFARE_Write() failed: "));
        play_wrong();
        Serial.println(F("W_NOK"));
    //    Serial.println(mfrc522.GetStatusCodeName(status));
    return 0;
    }
    //Serial.println();

    // Read data from the block (again, should now be what we have written)
    //Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
   // Serial.println(" ...");
    status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
      play_wrong();
      Serial.println(F("W_NOK"));
      //  Serial.print(F("MIFARE_Read() failed: "));
        //Serial.println(mfrc522.GetStatusCodeName(status));
        return 0;
    }
   // Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(":");
   // dump_byte_array(buffer, 16); Serial.println();
        
    // Check that data in block is what we have written
    // by counting the number of bytes that are equal
   // Serial.println(F("Checking result..."));
    byte count = 0;
    for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what we've read) with dataBlock (= what we've written)
        if (buffer[i] == dataBlock[i])
            count++;
    }
   // Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    if (count == 16) {
     //   Serial.println(F("Success :-)"));
        play_OK();
    } else {
      play_wrong();
      Serial.println(F("W_NOK"));
       // Serial.println("Failure, no match :-(");
       return 0;
        //Serial.println("  perhaps the write didn't work properly...");
    }
   // Serial.println();
        
    // Dump the sector data
    //Serial.println(F("Current data in sector:"));
   // mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
   // Serial.println();

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

    Serial.println(F("W_OK"));
    
    erase_dataBlock();
    return 1; 

 


}

void buildBlock(int byteNo, char data)
{
  if(byteNo < 16 && byteNo >= 0)
  {
    dataBlock[byteNo] = data;
   // Serial.write(dataBlock[byteNo]);
  }
  //dump_byte_array(dataBlock,16);
  
}

void erase_dataBlock()
{
  int byteNo = 0;
  for (byteNo=0; byteNo < 16; byteNo++)
  {
    dataBlock[byteNo] = 0x00;
  }
  
}


/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
      //  Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        //Serial.print(buffer[i], HEX);
    }
}

void play_wrong()
{
  notePlayer[0].begin(6);
  notePlayer[0].play(NOTE_B2);
  delay(300);
  notePlayer[0].stop();
  delay(100);
  notePlayer[0].play(NOTE_B2);
  delay(180);
  notePlayer[0].stop();
}

void play_OK()
{
  notePlayer[0].begin(6);
    notePlayer[0].play(NOTE_D4);
    delay(200);
    notePlayer[0].play(NOTE_B4);
    delay(180);
    notePlayer[0].stop();
}


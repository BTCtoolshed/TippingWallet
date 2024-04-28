//To do : multisig wallet creation

//NEW: print/no print options

// esp32 only
// Many thanks to Limor Fried/LadyAda, ricmoo, but especially to Stepan Snigirev and my anonymous friend for their information and help

// Obviously you need to install libraries like ricmoo's qrcode, uBitcoin, SdFat (not the forks), and various Adafruit libraries for this to work
// Read the comment next to the #include qrcoderm.h library. It is very important.
// Printer Driver has been updated. Import zip file to Arduino library from here https://github.com/AndersV209/Pos-Printer-Library

// To use the EMF Detector function:
// You may need to buy a special resistor (pick a value between 1 megOhm - 3.3 megOhms) to connect ground and A2 (GPIO Pin 34)
// You may need to solder an antenna/piece of wire to the analog pin side of the resistor


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */

#include <Adafruit_NeoPixel.h> //MIT License, attribute to Limor Fried
Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);
int led = LED_BUILTIN;

#include <bootloader_random.h> //necessary for true random number generator - brings in entropy from WIFI, but shuts off after random numbers are generated

#include <Bitcoin.h> //MIT License, attribute to Stepan Snigerev

#include <PSBT.h> //MIT License, attribute to Stepan Snigerev

#include <qrcoderm.h> //CRITICAL see instructions for renaming files here or QR code will not work https://github.com/ricmoo/QRCode/issues/35#issuecomment-1179311130
//qrcode MIT license, attribute to Richard Moore and Project Nayuki

#include <SPI.h>

#include <SdFat.h>
SdFat SD;
File myFile;

String finalString;
String psbtString;
PSBT psbt;
HDPrivateKey root;
HDPublicKey pub;

String walletamount = "\n\n LEFTOVER IN WALLET ->\n0.00";
String transaction = "";

//float ONE;
//float TWO;
//float THREE;
//float FOUR;

#include "Adafruit_ThinkInk.h" //MIT License, attribute to Limor Fried

#define SD_CS       14   // Reference https://forums.adafruit.com/viewtopic.php?f=22&t=171897
#define EPD_DC      33 // 
#define EPD_CS      15  // 
#define EPD_BUSY    -1  // 
#define SRAM_CS     32  // 
#define EPD_RESET   -1  // 

#define BUTTONA 27
#define BUTTONB 12
#define BUTTONC 13

ThinkInk_290_Grayscale4_T5 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);
//ThinkInk_290_Tricolor_Z10 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);
#define DEBUG

#define COLOR1 EPD_BLACK
#define COLOR2 EPD_LIGHT

//#include "Adafruit_Thermal.h" //CRITICAL edit this file to increase the heat of the printer. My settings: setHeatConfig(uint8_t dots=4, uint8_t time=255, uint8_t interval=255)
////Print Library MIT License, attribute to Limor Fried
//Adafruit_Thermal printer(&Serial1);

#include "Pos_Printer.h"
Pos_Printer printer(&Serial1); //edit the cpp and h file where necessary to set heat settings : dots=4, time = 255, interval = 255


//////////////////////////////////////////////////////////
int sample = 1000;
unsigned int state = 0;
unsigned int state1 = 0;
unsigned int state2 = 0;



int countdown = 6000; //
int randomchoice;
const char* QUOTE1 = "\nWe can't solve problems by using the same kind\nof thinking we used whenwe created them.\n\n      - Albert Einstein"; //
const char* QUOTE2 = "\nOh, you should never,\nnever doubt what nobody is sure about.\n\n      - Hilaire Belloc"; //
const char* QUOTE3 = "\nIf I had more time, I\nwould have written a\nshorter letter.\n\n      - Blaise Pascal"; //
const char* QUOTE4 = "\nIf I have seen further\nthan others, it is by\nstanding upon the\nshoulders of giants.\n\n      - Isaac Newton"; //
const char* QUOTE5 = "\nA little nonsense now\nand then is relished by the wisest men.\n\n      - Roald Dahl"; //
const char* QUOTE6 = "\nNow is the time to\nunderstand more, so thatwe may fear less.\n\n      - Marie Curie"; //
const char* QUOTE7 = "\nHe will win who knows\nwhen to fight and when\nnot to fight.\n\n      - Sun Tzu"; //
const char* QUOTE8 = "\nI shall allow no man to belittle my soul by\nmaking me hate him.\n\n  - Booker T Washington"; //
const char* QUOTE9 = "\nKeep your face always\ntoward the sunshine and shadows will fall behindyou.\n\n      - Walt Whitman"; //
const char* QUOTE10 = "\nSometimes life hits you in the head with a\nbrick. Don't lose faith.\n\n      - Steve Jobs"; //
const char* QUOTE11 = "\nWhen something is\nimportant enough, you doit even if the odds\nare not in your favor.\n\n      - Elon Musk"; //
const char* QUOTE12 = "\nIf everyone is moving\nforward together, then\nsuccess takes care of\nitself.\n\n      - Henry Ford";

const char* mainmenutext = "\n\n\n\nPress Top Left Button to Sign Transaction\n\n\n\nPress Middle Top Button to Generate New Wallet\n\n\n\nPress Top Right Button for Flashlight";

PrivateKey pk;
PublicKey pubk;
bool no_wallet_created_so_far = true;
String pkstr = "", pkwif = "", pubkstr = "";
const char* PRIVATE;
const char* PUBLIC;
const char* PHR;
const char* PHRS;

const char* P1;
const char* P2;
const char* P3;
const char* P4;

String PB1;
String PB2;
String PB3;
String PB4;
String finger;
const char* fing;
String BWxpub;

const char* PBL1;
const char* PBL2;
const char* PBL3;
const char* PBL4;
const char* XXPUB;
const char* XXPUBS;

String pu1;
String pu2;
String pu3;
String pu4;
String phra;
String phrase;
String ROOOT;
String pubx;

int RAND1;
int RANDO;

////////////////////////////////////////////////////////////

void testdrawtext(const char *text, uint16_t color) {
  display.setCursor(0, 0);
  display.setTextColor(color);
  display.setTextWrap(true);
  display.print(text);
}


int calc_buffer_size(QRCode *qrcode, int scale) {

  int size = qrcode->version * 5 + 7;
  return ((size * scale + 7) / 8) * size * scale;
}
byte x;
byte y;
QRCode qrcode;
uint8_t blockValue = qrcode_getModule(&qrcode, x, y);

void buffer_qr(QRCode *qrcode, uint8_t buffer[], int scale)
{
  int size = qrcode->version * 5 + 7;
  int gridByteSize = (size * scale + 7) / 8;
  int gridSize = size * scale;

  int pixelNum=0, byteNum, x, y, b, i;

  for(byteNum=y=0; y<gridSize; y++) { // Each row...
    for(x=0; x<gridByteSize; x++) { // Each 8-pixel block within row...
      b = 0;
      for(i=0; i < 8; i++) {// each bit in byte
        b = b << 1;
        b += qrcode_getModule(qrcode, (x * 8 + i) / scale, y / scale);
      }
      buffer[byteNum] = b;
      byteNum++;
    }
  }
}

uint8_t generate_random_8bit()
{
    RANDO = esp_random();
//    ONE = esp_random();
//    TWO = esp_random();
//    THREE = esp_random();
//    FOUR = esp_random();
//
//    RAND1 = random(1,8);
//    if (RAND1 == 1)
//    {RANDO = int(ONE/TWO);};
//    if (RAND1 == 2)
//    {RANDO = int(ONE*TWO);};
//    if (RAND1 == 3)
//    {RANDO = int(TWO/FOUR);};
//    if (RAND1 == 4)
//    {RANDO = int(THREE+ONE);};
//    if (RAND1 == 5)
//    {RANDO = int(THREE*ONE);};
//    if (RAND1 == 6)
//    {RANDO = int(TWO+ONE);};
//    if (RAND1 == 7)
//    {RANDO = int(FOUR+TWO);};
//    if (RAND1 == 8)
//    {RANDO = int(TWO-THREE);};
    
    //add in EMF entropy
    return (uint8_t)RANDO;
}


//////////////////////////////////////////////////////////////////////////////
void  quoter()
{
delay(100);
Serial.println("Quoter");
randomchoice = random(1,13);

      if(randomchoice == 1)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE1, COLOR1);
        display.display();
        }

      if(randomchoice == 2)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE2, COLOR1);
        display.display();
        }

      if(randomchoice == 3)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE3, COLOR1);
        display.display();
        }

      if(randomchoice == 4)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE4, COLOR1);
        display.display();
        }

      if(randomchoice == 5)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE5, COLOR1);
        display.display();
        }

      if(randomchoice == 6)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE6, COLOR1);
        display.display();
        }


      if(randomchoice == 7)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE7, COLOR1);
        display.display();
        }

      if(randomchoice == 8)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE8, COLOR1);
        display.display();
        }

      if(randomchoice == 9)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE9, COLOR1);
        display.display();
        }

      if(randomchoice == 10)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE10, COLOR1);
        display.display();
        }

      if(randomchoice == 11)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE11, COLOR1);
        display.display();
        }

      if(randomchoice == 12)
      {
        display.clearBuffer();
        display.setTextSize(2);
        testdrawtext(QUOTE12, COLOR1);
        display.display();
        }


}
///////////////////////////////////////////////////////////////////////////////////////////// SUBMENUS
void strobe()
{
countdown = 1100;
  while (countdown >= (300))
  {delay(50); pixels.setPixelColor(0, pixels.Color(255,255,255));pixels.show();delay(50); pixels.setPixelColor(0, pixels.Color(0,0,0));pixels.show();if (digitalRead(BUTTONA) == LOW){pixels.setPixelColor(0, pixels.Color(255,255,255));pixels.show();EMF();} if ( digitalRead(BUTTONB) == LOW){strobe();} if ( digitalRead(BUTTONC) == LOW){{pixels.setPixelColor(0, pixels.Color(0,0,0));pixels.show();loop();}} --countdown;};
  while (countdown >= (0))
  {delay(50); pixels.setPixelColor(0, pixels.Color(255,0,0));pixels.show();pixels.show();delay(50); pixels.setPixelColor(0, pixels.Color(0,0,0));pixels.show();if (digitalRead(BUTTONA) == LOW){pixels.setPixelColor(0, pixels.Color(255,255,255));pixels.show();EMF();}if ( digitalRead(BUTTONB) == LOW){strobe();} if ( digitalRead(BUTTONC) == LOW){{pixels.setPixelColor(0, pixels.Color(0,0,0));pixels.show();loop();}} --countdown;};

}

//void generate_multisig()
//{
//  display.clearBuffer();
//  display.setTextSize(2);
//  testdrawtext("COMING SOON!", COLOR1);
//  display.display();
//}

void generate_wallet()
{
  const uint8_t pklen = 32;
    uint8_t pkbytes[pklen];

    bootloader_random_enable(); /*key to true random number generation, uses analog noise and other sources for entropy*/
    for (uint8_t i = 0; i < pklen; i++)
    {
        pkbytes[i] = generate_random_8bit();
    }
    bootloader_random_disable(); /*protects from wireless attack*/
    
    pkstr = "";
    for (uint8_t i = 0; i < pklen; i++)
    {
        if (pkbytes[i] <= 0x0f)
        {
            pkstr += "0";
        }
        pkstr += String(pkbytes[i], HEX);
    }
  
  
  String entropy = pkstr;
  phrase = generateMnemonic(12, entropy);
//  Serial.println(phrase);

phra = String("Private Recovery Phrase:\n\n")+phrase;
PRIVATE = phrase.c_str();
PHR = phra.c_str();

HDPrivateKey root(phrase, ""); // using default empty password, fill quotes if you want a password added

ROOOT = root;
finger = root.fingerprint();
fing = finger.c_str();
HDPrivateKey account = root.derive("m/84'/0'/0'");
//Serial.println(root);
//Serial.println(root.fingerprint());
//Serial.println(account);
//Serial.println(account.xpub());



HDPublicKey xpub = account.xpub();
pubx = account.xpub();
//Serial.println(pubx);
BWxpub = "["+finger+"/84h/0h/0h"+"]"+pubx;
//Serial.println(BWxpub);
XXPUB = BWxpub.c_str();

      String desc = ""; //watch wallet for Bitcoin Core
      desc = "wpkh([";
      // add fingerprint
      desc += root.fingerprint();
      // add derivation path. We need to remove leading `m`
      desc += "/84h/0h/0h]";
      // now pub xpub in normal form
      xpub.type = UNKNOWN_TYPE;
      desc += xpub.toString();
      desc += "/0/*)";
      // and add a checksum
      desc += String("#")+descriptorChecksum(desc);
      //Serial.println(desc);

      String desc2 = "";
      desc2 = "wpkh([";
      // add fingerprint
      desc2 += root.fingerprint();
      // add derivation path. We need to remove leading `m`
      desc2 += "/84h/0h/0h]";
      // now pub xpub in normal form
      xpub.type = UNKNOWN_TYPE;
      desc2 += xpub.toString();
      desc2 += "/1/*)";
      // and add a checksum
      desc2 += String("#")+descriptorChecksum(desc2);
      //Serial.println(desc2);

HDPublicKey pub1;
HDPublicKey pub2;
HDPublicKey pub3;
HDPublicKey pub4;

pub1 = xpub.child(0).child(1); //this will show as Address 2
//Serial.println(pub1.address());

pub2 = xpub.child(0).child(2); //this will show as Address 3
//Serial.println(pub2.address());

pub3 = xpub.child(0).child(3); //this will show as Address 4
//Serial.println(pub3.address());

pub4 = xpub.child(0).child(0); //this will show as Address 1 -- I KNOW THIS IS DUMB, BUT I DID THIS IN THE BEGINNING, AND I AM TOO LAZY TO CARE NOW.
//Serial.println(pub4.address());

pu1 = String("Public Address 2:\n\n")+String(pub1.address());
pu2 = String("Public Address 3:\n\n")+String(pub2.address());
pu3 = String("Public Address 4:\n\n")+String(pub3.address());
pu4 = String("Public Address 1:\n\n")+String(pub4.address());

P1 = pu1.c_str();
P2 = pu2.c_str();
P3 = pu3.c_str();
P4 = pu4.c_str();

PB1 = pub1.address();
PBL1 = PB1.c_str();

PB2 = pub2.address();
PBL2 = PB2.c_str();

PB3 = pub3.address();
PBL3 = PB3.c_str();

PB4 = pub4.address();
PBL4 = PB4.c_str();



////////////eInk text print//////////////////


QRCode qrcode;
uint8_t qrVersion = 10;
uint8_t qrErrorLevel = ECC_MEDIUM;
byte qrcodeBytes[qrcode_getBufferSize(qrVersion)];
qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, XXPUB);
uint8_t scale=3;
int buffer_size = calc_buffer_size(&qrcode, scale);
int width = ((qrcode.version * 5 + 7) * scale);
uint8_t pixels[buffer_size];

  

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  

  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("IN CHRIST ALONE WE TRUST\n\nBITCOIN WALLET GENERATOR v4.0\n\n\n1) Press Top Left Button for Address 1\n\n2) Press Top Middle Button for XPUB\n   & Optional SD Card Backup\n\n3) Press Top Right Button for Recovery Phrase\n   & Optional SD Card Backup", COLOR1);
  display.display();
  delay(1000);
countdown = 750;
///////////////////////////////////////////////////////////////////// innermost menu
while (countdown >= 0 )
{ delay(100);


//////////////////////////////////////////////////////restate Address
  if ( digitalRead(BUTTONA) == LOW )
  {
  Serial.println("ButtonA");
  delay(100);
  display.clearBuffer();
  display.setTextSize(2);
  testdrawtext(P4, COLOR1);
  display.display();
  ///////////////////
  countdown = 250;
   while (countdown >= (0)) 
   {delay(250);
   //try to connect for a period of time
   if (digitalRead(BUTTONA) != LOW) 
  {
    delay(250);
    String countprint = String(countdown);
//    Serial.println(countprint);
    --countdown;}
      else {Serial.println("ButtonA"); break;}
   }
    Serial.println("Exit Address A");
    countdown = 750;
  ///////////////////
  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("IN CHRIST ALONE WE TRUST\n\nBITCOIN WALLET GENERATOR v4.0\n\n\nPress button near SD slot to reset.\n\n1) Press Top Left Button for Address 1 \n\n2) Press Top Middle Button for XPUB\n   & Optional SD Card Backup\n\n3) Press Top Right Button for Recovery Phrase \n   & Optional SD Card Backup", COLOR1);
  display.display();}
 

//////////////////////////////////////////////////////restate XPUB
  if ( digitalRead(BUTTONB) == LOW )
  {

//////////////////////////////////////////
if(!SD.begin(SD_CS, SD_SCK_MHZ(16)))
{Serial.println("SD did not initialize.");

  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("Check for SD Fat format, not Fat32. If formatted correctly, you may need to reseat the SD Card\nand try again.\n\n\nRevealing XPUB in 5 seconds.", COLOR1);
  display.display();

  delay(6000);
  }

else {
  SD.remove("XPUB.txt"); //This is the format for importing to Blue Wallet
  myFile = SD.open("XPUB.txt", FILE_WRITE);
  myFile.print(BWxpub);
  myFile.close();
  Serial.println("XPUB text written to SD");

  SD.remove("COREWATCH.txt"); //This is the format for importing to Bitcoin Core
  myFile = SD.open("COREWATCH.txt", FILE_WRITE);
  myFile.println(desc);
  myFile.println(desc2);
  myFile.close();
  Serial.println("COREWATCH text written to SD");
}



    
   Serial.println("ButtonB");
   delay(100);


/////////////qrcode XPUB/////////////////
  // Create the QR code
  //QRCode qrcode;
  // See table at https://github.com/ricmoo/QRCode
  // or https://www.qrcode.com/en/about/version.html for
  // calculation of data capacity of a QR code. Current
  // settings will support a string of about 100 bytes:
  qrVersion = 10;
  // can be ECC_LOW, ECC_MEDIUM, ECC_QUARTILE and ECC_HIGH (0-3, respectively):
  qrErrorLevel = ECC_MEDIUM;
  // allocate QR code memory: 
  qrcodeBytes[qrcode_getBufferSize(qrVersion)];
  qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, XXPUB);
  // QR Code block characteristics will depend on the display:
  // QR code needs a "quiet zone" of background color around it, hence the offset:
  int offset = 10;
  int blockSize = ((display.height() - (offset * 2)) / qrcode.size)*2;
//////// fill with the background color:
  display.clearBuffer();
  display.fillScreen(COLOR2);
  // read the bytes of the QR code and set the blocks light or dark, accordingly:
  // vertical loop:
  for (byte y = 0; y < qrcode.size; y++) {
    // horizontal loop:
    for (byte x = 0; x < qrcode.size; x++) {
      // caculate the block's X and Y positions:
      int blockX = (x * blockSize) + offset;
      int blockY = (y * blockSize) + offset;
      // read the block value from the QRcode:
      int blockValue = qrcode_getModule(&qrcode, x, y);
      // set the default block color:
      int blockColor = COLOR2;
      // if the block value is 1, set color to foreground color instead:
      if (blockValue == 1) {
        blockColor = COLOR1;
      }
      // display the block on the screen:
      display.fillRect(blockX, blockY, blockSize, blockSize, blockColor);
    }
  }
  // print the message and display it:
  Serial.println(XXPUB);
  display.display();
  ///////////////////
  countdown = 250;
   while (countdown >= (0)) 
   {delay(250);
   //try to connect for a period of time
   if (digitalRead(BUTTONB) != LOW) 
    {
    delay(250);
    String countprint = String(countdown);
//    Serial.println(countprint);
    --countdown;}
      else {Serial.println("ButtonB");Serial.println("Exit XPUB"); break;}
   }


   
  display.clearBuffer();
  display.setTextSize(2);
  testdrawtext(XXPUB,COLOR1);
  display.display();
  ///////////////////
  countdown = 250;
   while (countdown >= (0)) 
   {delay(250);
   //try to connect for a period of time
   if (digitalRead(BUTTONB) != LOW) 
  {
    delay(250);
    String countprint = String(countdown);
//    Serial.println(countprint);
    --countdown;}
      else {Serial.println("ButtonB");Serial.println("Exit XPUB"); break;}
   }



  
  ///////////////////
  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("IN CHRIST ALONE WE TRUST\n\nBITCOIN WALLET GENERATOR v4.0\n\n\nPress button near SD slot to reset.\n\n1) Press Top Left Button for Address 1 \n\n2) Press Top Middle Button for XPUB\n   & Optional SD Card Backup\n\n3) Press Top Right Button for Recovery Phrase \n   & Optional SD Card Backup", COLOR1);
  display.display();
   
//   countdown = 250;
//   while (countdown >= (0)) 
//   {delay(250);
//   //try to connect for a period of time
//   if (digitalRead(BUTTONB) != LOW) 
//  {
//    delay(250);
//    String countprint = String(countdown);
////    Serial.println(countprint);
//    --countdown;}
//      else {Serial.println("ButtonB");Serial.println("Exit XPUB"); break;}
//   }
//  
  countdown = 750;
--countdown;
  }



if ( digitalRead(BUTTONC) == LOW ) ///////////// SD write and recovery phrase restate
  {

  countdown=250;
  Serial.println("ButtonC");
 /////////////////////////////////////////
if(!SD.begin(SD_CS, SD_SCK_MHZ(16)))
{Serial.println("SD did not initialize.");

  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("Check for SD Fat format, not Fat32. If formatted correctly, you may need to reseat the SD Card\nand try again.\n\n\nRevealing Recovery Phrase in 5 seconds.", COLOR1);
  display.display();

  delay(6000);
  }

else {
  SD.remove("PUBLIC.txt");
  myFile = SD.open("PUBLIC.txt", FILE_WRITE);
  myFile.print(PB4);
  myFile.close();
  Serial.println("PUBLIC text written to SD");

  SD.remove("XPUB.txt");
  myFile = SD.open("XPUB.txt", FILE_WRITE);
  myFile.print(BWxpub);
  myFile.close();
  Serial.println("XPUB text written to SD");

  SD.remove("PHRASE.txt");
  myFile = SD.open("PHRASE.txt", FILE_WRITE);
  myFile.print(phrase);
  myFile.close();
  Serial.println("PHRASE text written to SD");

  SD.remove("COREWATCH.txt");
  myFile = SD.open("COREWATCH.txt", FILE_WRITE);
  myFile.println(desc);
  myFile.println(desc2);
  myFile.close();
  Serial.println("COREWATCH text written to SD");
}

    



/////////////qrcode XPUB/////////////////
  // Create the QR code
  //QRCode qrcode;
  // See table at https://github.com/ricmoo/QRCode
  // or https://www.qrcode.com/en/about/version.html for
  // calculation of data capacity of a QR code. Current
  // settings will support a string of about 100 bytes:
  qrVersion = 10;
  // can be ECC_LOW, ECC_MEDIUM, ECC_QUARTILE and ECC_HIGH (0-3, respectively):
  qrErrorLevel = ECC_MEDIUM;
  // allocate QR code memory: 
  qrcodeBytes[qrcode_getBufferSize(qrVersion)];
  qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, PRIVATE);
  // QR Code block characteristics will depend on the display:
  // QR code needs a "quiet zone" of background color around it, hence the offset:
  int offset = 10;
  int blockSize = ((display.height() - (offset * 2)) / qrcode.size)*2;
//////// fill with the background color:
  display.clearBuffer();
  display.fillScreen(COLOR2);
  // read the bytes of the QR code and set the blocks light or dark, accordingly:
  // vertical loop:
  for (byte y = 0; y < qrcode.size; y++) {
    // horizontal loop:
    for (byte x = 0; x < qrcode.size; x++) {
      // caculate the block's X and Y positions:
      int blockX = (x * blockSize) + offset;
      int blockY = (y * blockSize) + offset;
      // read the block value from the QRcode:
      int blockValue = qrcode_getModule(&qrcode, x, y);
      // set the default block color:
      int blockColor = COLOR2;
      // if the block value is 1, set color to foreground color instead:
      if (blockValue == 1) {
        blockColor = COLOR1;
      }
      // display the block on the screen:
      display.fillRect(blockX, blockY, blockSize, blockSize, blockColor);
    }
  }
  // print the message and display it:
  display.display();
  ///////////////////
  countdown = 250;
   while (countdown >= (0)) 
   {delay(250);
   //try to connect for a period of time
   if (digitalRead(BUTTONC) != LOW) 
    {
    delay(250);
    String countprint = String(countdown);
//    Serial.println(countprint);
    --countdown;}
      else {Serial.println("ButtonC");Serial.println("Exit PHRASE"); break;}
   }  







  display.clearBuffer();
  display.setTextSize(2);
  testdrawtext(PHR, COLOR1);
  display.display();
  ///////////////////
  countdown = 250;
   while (countdown >= (0)) 
   {delay(250);
   //try to connect for a period of time
   if (digitalRead(BUTTONC) != LOW) 
  {
    delay(250);
    String countprint = String(countdown);
//    Serial.println(countprint);
    --countdown;}
      else {Serial.println("ButtonC"); break;}
   }
    Serial.println("Exit Phrase");
  
  countdown = 750;
  ///////////////////
  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("IN CHRIST ALONE WE TRUST\n\nBITCOIN WALLET GENERATOR v4.0\n\n\nPress button near SD slot to reset.\n\n1) Press Top Left Button for Address 1 \n\n2) Press Top Middle Button for XPUB\n   & Optional SD Card Backup\n\n3) Press Top Right Button for Recovery Phrase \n   & Optional SD Card Backup", COLOR1);
  display.display();}

  --countdown;
  //Serial.println(countdown);
 }


  
 --countdown;
 //Serial.println(countdown);
 loop();
}
//////////////////////////////////////////////////////////////////////////////////////////////

void generate_wallet_print()
{

  const uint8_t pklen = 32;
    uint8_t pkbytes[pklen];

    bootloader_random_enable(); /*key to true random number generation, uses analog noise and other sources for entropy*/
    for (uint8_t i = 0; i < pklen; i++)
    {
        pkbytes[i] = generate_random_8bit();
    }
    bootloader_random_disable(); /*protects from wireless attack*/
    
    pkstr = "";
    for (uint8_t i = 0; i < pklen; i++)
    {
        if (pkbytes[i] <= 0x0f)
        {
            pkstr += "0";
        }
        pkstr += String(pkbytes[i], HEX);
    }
  
  
  String entropy = pkstr;
  phrase = generateMnemonic(12, entropy);
//  Serial.println(phrase);

phra = String("Private Recovery Phrase:\n\n")+phrase;
PRIVATE = phrase.c_str();
PHR = phra.c_str();

HDPrivateKey root(phrase, ""); // using default empty password, fill quotes if you want a password added

ROOOT = root;
finger = root.fingerprint();
fing = finger.c_str();
HDPrivateKey account = root.derive("m/84'/0'/0'");
//Serial.println(root);
//Serial.println(root.fingerprint());
//Serial.println(account);
//Serial.println(account.xpub());



HDPublicKey xpub = account.xpub();
pubx = account.xpub();
//Serial.println(pubx);
BWxpub = "["+finger+"/84h/0h/0h"+"]"+pubx;
//Serial.println(BWxpub);
XXPUB = BWxpub.c_str();

      String desc = "";
      desc = "wpkh([";
      // add fingerprint
      desc += root.fingerprint();
      // add derivation path. We need to remove leading `m`
      desc += "/84h/0h/0h]";
      // now pub xpub in normal form
      xpub.type = UNKNOWN_TYPE;
      desc += xpub.toString();
      desc += "/0/*)";
      // and add a checksum
      desc += String("#")+descriptorChecksum(desc);
      //Serial.println(desc);

      String desc2 = "";
      desc2 = "wpkh([";
      // add fingerprint
      desc2 += root.fingerprint();
      // add derivation path. We need to remove leading `m`
      desc2 += "/84h/0h/0h]";
      // now pub xpub in normal form
      xpub.type = UNKNOWN_TYPE;
      desc2 += xpub.toString();
      desc2 += "/1/*)";
      // and add a checksum
      desc2 += String("#")+descriptorChecksum(desc2);
      //Serial.println(desc2);

HDPublicKey pub1;
HDPublicKey pub2;
HDPublicKey pub3;
HDPublicKey pub4;

pub1 = xpub.child(0).child(1); //this will show as Address 2
//Serial.println(pub1.address());

pub2 = xpub.child(0).child(2); //this will show as Address 3
//Serial.println(pub2.address());

pub3 = xpub.child(0).child(3); //this will show as Address 4
//Serial.println(pub3.address());

pub4 = xpub.child(0).child(0); //this will show as Address 1
//Serial.println(pub4.address());

pu1 = String("Public Address 2:\n\n")+String(pub1.address());
pu2 = String("Public Address 3:\n\n")+String(pub2.address());
pu3 = String("Public Address 4:\n\n")+String(pub3.address());
pu4 = String("Public Address 1:\n\n")+String(pub4.address());

P1 = pu1.c_str();
P2 = pu2.c_str();
P3 = pu3.c_str();
P4 = pu4.c_str();

PB1 = pub1.address();
PBL1 = PB1.c_str();

PB2 = pub2.address();
PBL2 = PB2.c_str();

PB3 = pub3.address();
PBL3 = PB3.c_str();

PB4 = pub4.address();
PBL4 = PB4.c_str();


  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("IN CHRIST ALONE WE TRUST\n\nBITCOIN WALLET GENERATOR v4.0\n\n\n\n\nPlease wait 1 minute for options to recall \naddresses & recovery phrase as generator \nattempts to print", COLOR1);
  display.display();

printer.justify('L');
printer.feed();
printer.println(F(PHR));

//print QR Code for Phrase
printer.feed(1);
QRCode qrcode;
uint8_t qrVersion = 10; 
uint8_t qrErrorLevel = ECC_MEDIUM;
byte qrcodeBytes[qrcode_getBufferSize(qrVersion)];
qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, PRIVATE);
uint8_t scale=3;
int buffer_size = calc_buffer_size(&qrcode, scale);
int width = ((qrcode.version * 5 + 7) * scale);
uint8_t pixels[buffer_size];
buffer_qr(&qrcode, pixels, scale);
printer.feed(1);
printer.printBitmap(width, width, pixels);
  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////BLUE WALLET FOOTER
printer.justify('L');
printer.feed(1);printer.println(F("---------------------------"));
printer.feed(1);
printer.println(F("Import Recovery Phrase to"));
printer.feed(1);
printer.println(F("BlueWallet for ease of use:"));
printer.feed(1);
printer.println(F("https://bluewallet.io"));
printer.feed(1);
//qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, "https://bluewallet.io/");
printer.feed(1);
printer.println(F("Step 1) Go to create new wallet; click but don't create yet"));
printer.feed(1);
printer.println(F("Step 2) Scroll down & click"));
printer.feed(1);
printer.println(F("      IMPORT"));
printer.feed(1);
printer.println(F("Step 3) Enter recovery phrase & continue. Enjoy!"));
printer.feed(1);
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////BLUE WALLET FOOTER
printer.justify('L');
printer.feed(1);printer.println(F("---------------------------"));
printer.feed(1);
printer.println(F("Spend Using :"));
printer.feed(1);
printer.println(F("thebitcoincompany.com"));
printer.feed(1);
printer.println(F("Buy More Using :"));
printer.feed(1);
printer.println(F("https://strike.me/"));
//////////////////////////////////////////////////////////////////
  
////////////////////////////////////////
//////// XPUB /////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////
qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, XXPUB);
scale=3;
buffer_size = calc_buffer_size(&qrcode, scale);
width = ((qrcode.version * 5 + 7) * scale);
pixels[buffer_size];
printer.justify('L');
printer.feed(1);
printer.feed(1);printer.println(F("---------------------------"));
printer.println(F("ADVANCED FEATURES..."));
printer.feed(1);
printer.print(F("FINGERPRINT: "));
printer.println(F(fing));
printer.println(F("WATCH WALLET: "));

buffer_qr(&qrcode, pixels, scale);
printer.feed(1);
printer.printBitmap(width, width, pixels);

//printer.printf(XXPUB);
printer.feed(1);
printer.feed(1);printer.println(F("---------------------------"));
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
////////////////////////////////////////
//////// PUBLIC ADDRESS 1 /////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////

qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, PBL4);

printer.feed(1);
printer.justify('L');
printer.println(F("Public Address 1: "));
printer.feed(1);
printer.print(F(PBL4));

////////////////////////////////////////////////////////////////////
// try scale=7, try lower values if you run out of memory
scale=3;
buffer_size = calc_buffer_size(&qrcode, scale);
width = ((qrcode.version * 5 + 7) * scale);
pixels[buffer_size];

// for each qr code use this code to print it - use the same pixel buffer to save memory
buffer_qr(&qrcode, pixels, scale);
printer.feed(1);
printer.printBitmap(width, width, pixels);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////
////////// PUBLIC ADDRESS 2/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////
//
//  qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, PBL2);
//
//
//printer.feed(3);
////printer.justify('L');
////printer.println(F("IN CHRIST ALONE WE TRUST"));
////
////printer.feed(1);
//printer.justify('L');
//printer.println(F("Public Address 2: "));
//printer.feed(1);
//printer.printf(PBL1);
//
////////////////////////////////////////////////////////////////////
//
//// try scale=5, try lower values if you run out of memory
//scale=3;
//buffer_size = calc_buffer_size(&qrcode, scale);
//width = ((qrcode.version * 5 + 7) * scale);
//pixels[buffer_size];
//
//// for each qr code use this code to print it - use the same pixel buffer to save memory
//buffer_qr(&qrcode, pixels, scale);
//printer.feed(1);
//printer.printBitmap(width, width, pixels);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
printer.feed(1);
printer.feed(1);printer.println(F("---------------------------"));
printer.println(F("Learn More At:"));
printer.feed(1);
qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, "https://10hoursofbitcoin.com/");
scale=3;
buffer_size = calc_buffer_size(&qrcode, scale);
width = ((qrcode.version * 5 + 7) * scale);
pixels[buffer_size];
buffer_qr(&qrcode, pixels, scale);
printer.printBitmap(width, width, pixels);

printer.feed();
printer.feed();
printer.feed();
printer.feed();
printer.feed();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////print phrase again
//printer.feed(1);
//printer.justify('L');
//printer.println("---------------------------");
//printer.printf(PHR);
//printer.feed(3);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  

  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("IN CHRIST ALONE WE TRUST\n\nBITCOIN WALLET GENERATOR v4.0\n\n\n1) Press Top Left Button for Address 1\n\n2) Press Top Middle Button for XPUB\n   & Optional SD Card Backup\n\n3) Press Top Right Button for Recovery Phrase\n   & Optional SD Card Backup\n\n\n If SD Fat card present, it now has wallet info.", COLOR1);
  display.display();
  delay(1000);
countdown = 750;
///////////////////////////////////////////////////////////////////// innermost menu
while (countdown >= 0 )
{ delay(100);


//////////////////////////////////////////////////////restate Address
  if ( digitalRead(BUTTONA) == LOW )
  {
  Serial.println("ButtonA");
  delay(100);
  display.clearBuffer();
  display.setTextSize(2);
  testdrawtext(P4, COLOR1);
  display.display();
  ///////////////////
  countdown = 250;
   while (countdown >= (0)) 
   {delay(250);
   //try to connect for a period of time
   if (digitalRead(BUTTONA) != LOW) 
  {
    delay(250);
    String countprint = String(countdown);
//    Serial.println(countprint);
    --countdown;}
      else {Serial.println("ButtonA"); break;}
   }
    Serial.println("Exit Address A");
    countdown = 750;
  ///////////////////
  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("IN CHRIST ALONE WE TRUST\n\nBITCOIN WALLET GENERATOR v4.0\n\n\nPress button near SD slot to reset.\n\n1) Press Top Left Button for Address 1 \n\n2) Press Top Middle Button for XPUB\n   & Optional SD Card Backup\n\n3) Press Top Right Button for Recovery Phrase \n   & Optional SD Card Backup", COLOR1);
  display.display();}
 

//////////////////////////////////////////////////////restate XPUB
  if ( digitalRead(BUTTONB) == LOW )
  {

//////////////////////////////////////////
if(!SD.begin(SD_CS, SD_SCK_MHZ(16)))
{Serial.println("SD did not initialize.");

  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("Check for SD Fat format, not Fat32. If formatted correctly, you may need to reseat the SD Card\nand try again.\n\n\nRevealing XPUB in 5 seconds.", COLOR1);
  display.display();

  delay(6000);
  }

else {
  SD.remove("XPUB.txt");
  myFile = SD.open("XPUB.txt", FILE_WRITE);
  myFile.print(BWxpub);
  myFile.close();
  Serial.println("XPUB text written to SD");

  SD.remove("COREWATCH.txt");
  myFile = SD.open("COREWATCH.txt", FILE_WRITE);
  myFile.println(desc);
  myFile.println(desc2);
  myFile.close();
  Serial.println("COREWATCH text written to SD");
}



    
   Serial.println("ButtonB");
   delay(100);


/////////////qrcode XPUB/////////////////
  // Create the QR code
  //QRCode qrcode;
  // See table at https://github.com/ricmoo/QRCode
  // or https://www.qrcode.com/en/about/version.html for
  // calculation of data capacity of a QR code. Current
  // settings will support a string of about 100 bytes:
  qrVersion = 10;
  // can be ECC_LOW, ECC_MEDIUM, ECC_QUARTILE and ECC_HIGH (0-3, respectively):
  qrErrorLevel = ECC_MEDIUM;
  // allocate QR code memory: 
  qrcodeBytes[qrcode_getBufferSize(qrVersion)];
  qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, XXPUB);
  // QR Code block characteristics will depend on the display:
  // QR code needs a "quiet zone" of background color around it, hence the offset:
  int offset = 10;
  int blockSize = ((display.height() - (offset * 2)) / qrcode.size)*2;
//////// fill with the background color:
  display.clearBuffer();
  display.fillScreen(COLOR2);
  // read the bytes of the QR code and set the blocks light or dark, accordingly:
  // vertical loop:
  for (byte y = 0; y < qrcode.size; y++) {
    // horizontal loop:
    for (byte x = 0; x < qrcode.size; x++) {
      // caculate the block's X and Y positions:
      int blockX = (x * blockSize) + offset;
      int blockY = (y * blockSize) + offset;
      // read the block value from the QRcode:
      int blockValue = qrcode_getModule(&qrcode, x, y);
      // set the default block color:
      int blockColor = COLOR2;
      // if the block value is 1, set color to foreground color instead:
      if (blockValue == 1) {
        blockColor = COLOR1;
      }
      // display the block on the screen:
      display.fillRect(blockX, blockY, blockSize, blockSize, blockColor);
    }
  }
  // print the message and display it:
  Serial.println(XXPUB);
  display.display();
  ///////////////////
  countdown = 250;
   while (countdown >= (0)) 
   {delay(250);
   //try to connect for a period of time
   if (digitalRead(BUTTONB) != LOW) 
    {
    delay(250);
    String countprint = String(countdown);
//    Serial.println(countprint);
    --countdown;}
      else {Serial.println("ButtonB");Serial.println("Exit XPUB"); break;}
   }


   
  display.clearBuffer();
  display.setTextSize(2);
  testdrawtext(XXPUB,COLOR1);
  display.display();
  ///////////////////
  countdown = 250;
   while (countdown >= (0)) 
   {delay(250);
   //try to connect for a period of time
   if (digitalRead(BUTTONB) != LOW) 
  {
    delay(250);
    String countprint = String(countdown);
//    Serial.println(countprint);
    --countdown;}
      else {Serial.println("ButtonB");Serial.println("Exit XPUB"); break;}
   }



  
  ///////////////////
  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("IN CHRIST ALONE WE TRUST\n\nBITCOIN WALLET GENERATOR v4.0\n\n\nPress button near SD slot to reset.\n\n1) Press Top Left Button for Address 1 \n\n2) Press Top Middle Button for XPUB\n   & Optional SD Card Backup\n\n3) Press Top Right Button for Recovery Phrase \n   & Optional SD Card Backup", COLOR1);
  display.display();
   
//   countdown = 250;
//   while (countdown >= (0)) 
//   {delay(250);
//   //try to connect for a period of time
//   if (digitalRead(BUTTONB) != LOW) 
//  {
//    delay(250);
//    String countprint = String(countdown);
////    Serial.println(countprint);
//    --countdown;}
//      else {Serial.println("ButtonB");Serial.println("Exit XPUB"); break;}
//   }
//  
  countdown = 750;
--countdown;
  }



if ( digitalRead(BUTTONC) == LOW ) ///////////// SD write and recovery phrase restate
  {

  countdown=250;
  Serial.println("ButtonC");
 /////////////////////////////////////////
if(!SD.begin(SD_CS, SD_SCK_MHZ(16)))
{Serial.println("SD did not initialize.");

  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("Check for SD Fat format, not Fat32. If formatted correctly, you may need to reseat the SD Card\nand try again.\n\n\nRevealing Recovery Phrase in 5 seconds.", COLOR1);
  display.display();

  delay(6000);
  }

else {
  SD.remove("PUBLIC.txt");
  myFile = SD.open("PUBLIC.txt", FILE_WRITE);
  myFile.print(PB4);
  myFile.close();
  Serial.println("PUBLIC text written to SD");

  SD.remove("XPUB.txt");
  myFile = SD.open("XPUB.txt", FILE_WRITE);
  myFile.print(BWxpub);
  myFile.close();
  Serial.println("XPUB text written to SD");

  SD.remove("PHRASE.txt");
  myFile = SD.open("PHRASE.txt", FILE_WRITE);
  myFile.print(phrase);
  myFile.close();
  Serial.println("PHRASE text written to SD");

  SD.remove("COREWATCH.txt");
  myFile = SD.open("COREWATCH.txt", FILE_WRITE);
  myFile.println(desc);
  myFile.println(desc2);
  myFile.close();
  Serial.println("COREWATCH text written to SD");
}

    



/////////////qrcode XPUB/////////////////
  // Create the QR code
  //QRCode qrcode;
  // See table at https://github.com/ricmoo/QRCode
  // or https://www.qrcode.com/en/about/version.html for
  // calculation of data capacity of a QR code. Current
  // settings will support a string of about 100 bytes:
  qrVersion = 10;
  // can be ECC_LOW, ECC_MEDIUM, ECC_QUARTILE and ECC_HIGH (0-3, respectively):
  qrErrorLevel = ECC_MEDIUM;
  // allocate QR code memory: 
  qrcodeBytes[qrcode_getBufferSize(qrVersion)];
  qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, PRIVATE);
  // QR Code block characteristics will depend on the display:
  // QR code needs a "quiet zone" of background color around it, hence the offset:
  int offset = 10;
  int blockSize = ((display.height() - (offset * 2)) / qrcode.size)*2;
//////// fill with the background color:
  display.clearBuffer();
  display.fillScreen(COLOR2);
  // read the bytes of the QR code and set the blocks light or dark, accordingly:
  // vertical loop:
  for (byte y = 0; y < qrcode.size; y++) {
    // horizontal loop:
    for (byte x = 0; x < qrcode.size; x++) {
      // caculate the block's X and Y positions:
      int blockX = (x * blockSize) + offset;
      int blockY = (y * blockSize) + offset;
      // read the block value from the QRcode:
      int blockValue = qrcode_getModule(&qrcode, x, y);
      // set the default block color:
      int blockColor = COLOR2;
      // if the block value is 1, set color to foreground color instead:
      if (blockValue == 1) {
        blockColor = COLOR1;
      }
      // display the block on the screen:
      display.fillRect(blockX, blockY, blockSize, blockSize, blockColor);
    }
  }
  // print the message and display it:
  display.display();
  ///////////////////
  countdown = 250;
   while (countdown >= (0)) 
   {delay(250);
   //try to connect for a period of time
   if (digitalRead(BUTTONC) != LOW) 
    {
    delay(250);
    String countprint = String(countdown);
//    Serial.println(countprint);
    --countdown;}
      else {Serial.println("ButtonC");Serial.println("Exit PHRASE"); break;}
   }  







  display.clearBuffer();
  display.setTextSize(2);
  testdrawtext(PHR, COLOR1);
  display.display();
  ///////////////////
  countdown = 250;
   while (countdown >= (0)) 
   {delay(250);
   //try to connect for a period of time
   if (digitalRead(BUTTONC) != LOW) 
  {
    delay(250);
    String countprint = String(countdown);
//    Serial.println(countprint);
    --countdown;}
      else {Serial.println("ButtonC"); break;}
   }
    Serial.println("Exit Phrase");
  
  countdown = 750;
  ///////////////////
  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("IN CHRIST ALONE WE TRUST\n\nBITCOIN WALLET GENERATOR v4.0\n\n\nPress button near SD slot to reset.\n\n1) Press Top Left Button for Address 1 \n\n2) Press Top Middle Button for XPUB\n   & Optional SD Card Backup\n\n3) Press Top Right Button for Recovery Phrase \n   & Optional SD Card Backup", COLOR1);
  display.display();}

  --countdown;
  //Serial.println(countdown);
 }


  
 --countdown;
 //Serial.println(countdown);
 loop();
}



void signthis()
{
      //////////////////////////////////////////
      if(!SD.begin(SD_CS, SD_SCK_MHZ(16)))
      {
      Serial.println("SD did not initialize.");
      display.clearBuffer();
      display.setTextSize(1);
      testdrawtext("Check for SD Fat format, not Fat32. If formatted correctly, you may need to reseat the SD Card.", COLOR1);
      display.display();
      delay(6000);
      loop();
      } //Check for SD Fat format, not Fat32. If formatted correctly, you may need to reseat the SD Card.
      //////////////////////////////////////////
      
      else
      {
            delay(100);
            myFile = SD.open("PHRASE.txt");
            while (myFile.available())
            {
              finalString += (char)myFile.read();
            }
            myFile.close();
            
            int strlength = finalString.length();
            String finalfinal = finalString.substring(0,(strlength-0));
            delay(100);
            Serial.println(finalfinal);
            PHRS = finalfinal.c_str();
            
            strlength=0;

            //determine if using an xprv or a bip39 phrase
            if(finalfinal.indexOf(" ")>-1)
            {root = HDPrivateKey(PHRS,"");}
            else{HDPrivateKey root(PHRS);}
            
            finalString="";
            HDPrivateKey account = root.derive("m/84'/0'/0'");
//            Serial.println(root);
//            Serial.println(root.fingerprint());
            finger = root.fingerprint();
//            Serial.println(account);
//            Serial.println(account.xpub());
            
            HDPublicKey xpub = account.xpub();
            pubx = account.xpub();
//            Serial.println(pubx);
            XXPUBS = pubx.c_str();
//            Serial.println(XXPUBS);


                  String desc = "";
                  desc = "wpkh([";
                  // add fingerprint
                  desc += root.fingerprint();
                  // add derivation path. We need to remove leading `m`
                  desc += "/84h/0h/0h]";
                  // now pub xpub in normal form
                  xpub.type = UNKNOWN_TYPE;
                  desc += xpub.toString();
                  desc += "/0/*)";
                  // and add a checksum
                  desc += String("#")+descriptorChecksum(desc);
                  //Serial.println(desc);
            
                  String desc2 = "";
                  desc2 = "wpkh([";
                  // add fingerprint
                  desc2 += root.fingerprint();
                  // add derivation path. We need to remove leading `m`
                  desc2 += "/84h/0h/0h]";
                  // now pub xpub in normal form
                  xpub.type = UNKNOWN_TYPE;
                  desc2 += xpub.toString();
                  desc2 += "/1/*)";
                  // and add a checksum
                  desc2 += String("#")+descriptorChecksum(desc2);
                  //Serial.println(desc2);

                  if(!SD.begin(SD_CS, SD_SCK_MHZ(16)))
                  {Serial.println("whatever");}
                  else 
                  {
                  delay(100);
                  SD.remove("COREWATCH.txt");
                  myFile = SD.open("COREWATCH.txt", FILE_WRITE);
                  myFile.println(desc);
                  myFile.println(desc2);
                  myFile.print(XXPUBS);
                  myFile.close();
                  Serial.println("COREWATCH text written to SD");
                  }
      }

  delay(100);
  display.clearBuffer();
  display.setTextSize(1);
  String fingers = "\nCheck Fingerprint for Verification:  "+finger+"\n\nStep 1: Remove SD card with Recovery Phrase\n\n\Step 2: Press Top Left Button to continue";
  const char* fingersconst = fingers.c_str();
  testdrawtext(fingersconst, COLOR1);
  display.display();

  fingers = "";

  countdown = 1100;
  while (countdown >= 0)
  {
  delay(100); 
  if ( digitalRead(BUTTONA) == LOW)
    {break;}
  --countdown;
  }
  
  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("Step 1: Insert SD card with PSBT\n        from External Wallet\n\n\nStep 2: Press Top Left Button to continue", COLOR1);
  display.display();

  countdown = 1100;
  while (countdown >= 0)
  {
  delay(100); 
  if ( digitalRead(BUTTONA) == LOW)
    {break;}
  --countdown;
  }

//  //////////////READ PSBT FROM BLUEWALLET & SIGN IT AND WRITE IT TO SD
//  ////////////////////////////////////////////////////need to put a button step here to allow SD Swapping
//      //////////////////////////////////////////
      if(!SD.begin(SD_CS, SD_SCK_MHZ(16)))
      {
      Serial.println("SD did not initialize.");
      display.clearBuffer();
      display.setTextSize(1);
      testdrawtext("Check for SD Fat format, not Fat32. If formatted correctly, you may need to reseat the SD Card.", COLOR1);
      display.display();
      delay(6000);
      loop();
      } //Check for SD Fat format, not Fat32. If formatted correctly, you may need to reseat the SD Card.
      //////////////////////////////////////////
      
      else
      {
            delay(100);
            myFile = SD.open("This.psbt");
            while (myFile.available())
            {
              psbtString += (char)myFile.read();
            }
            myFile.close();}
delay(100);
int strlength = psbtString.length();
String finalpsbt = psbtString;
Serial.println(finalpsbt);
const char* signedpsbt = finalpsbt.c_str();
psbtString="";
strlength=0;

////////////////////////
//psbt.parseBase64("");
//psbt.parseBase64("");
psbt.parseBase64(signedpsbt);
Serial.print("ROOTCHECK: ");
Serial.println(root);
if(!psbt){
    Serial.println("Failed parsing transaction");
      display.clearBuffer();
      display.setTextSize(1);
      testdrawtext("PSBT format incorrect. Check wallet settings for fingerprint and derivation. Resetting now.", COLOR1);
      display.display();
      delay(6000);
    loop();
  }
   uint8_t zero[4] = { 0 };
    for(unsigned int i = 0; i < psbt.tx.inputsNumber; i++){
        for(unsigned int j = 0; j < psbt.txInsMeta[i].derivationsLen; j++){
            // check if psbt has zero fingerprint, if so - fill ours instead
            if(memcmp(psbt.txInsMeta[i].derivations[j].fingerprint, zero, 4) == 0){
                root.fingerprint(psbt.txInsMeta[i].derivations[j].fingerprint);
            }
        }
    }


  Serial.println("Transactions details:");
  // going through all outputs to print info
  Serial.println("Outputs:");
  for(int i=0; i<psbt.tx.outputsNumber; i++){
    if(i==0)
    { transaction += "RECEIVING ADDRESS:\n";};
    if(i==1)
    { transaction += "\nWALLET ADDRESS:\n";};
    // print addresses
    Serial.println(i);
    Serial.print(psbt.tx.txOuts[i].address(&Mainnet));
    transaction += String(psbt.tx.txOuts[i].address(&Mainnet));
    transaction += "\n\n";
    if(psbt.txOutsMeta[i].derivationsLen > 0){ // there is derivation path
      // considering only single key for simplicity
      PSBTDerivation der = psbt.txOutsMeta[i].derivations[0];
      HDPublicKey pub = root.derive(der.derivation, der.derivationLen).xpub();
      if(pub.address() == psbt.tx.txOuts[i].address()){
        Serial.print(" (change) ");
      }
    }
    Serial.print(" -> ");
    Serial.print(psbt.tx.txOuts[i].btcAmount()*1e3);
    Serial.println(" mBTC");
//    Serial.println(psbt.tx.txOuts[i].btcAmount()*1000000);
    if(i==0)
    {transaction += "SEND AMOUNT -> ";
    transaction += String(psbt.tx.txOuts[i].btcAmount()*1000000*100,0);
    transaction += " Sats\n\n";}

    if(i==1)
    {
      if(pub.address() == psbt.tx.txOuts[i].address()){
      transaction += "LEFTOVER IN WALLET -> ";
      transaction += String(psbt.tx.txOuts[i].btcAmount()*1000000*100,0);
      transaction += " Sats\n";
        }
      else {transaction += "LEFTOVER IN WALLET -> ";
      transaction += String(psbt.tx.txOuts[i].btcAmount()*1000000*100,0);
      transaction += " Sats\n";}
    }
  }
  Serial.print("Fee: ");
  Serial.print(float(psbt.fee())/100); // Arduino can't print 64-bit ints
  Serial.println(" bits");


if(transaction.indexOf("LEFTOVER") == -1)
{transaction += "LEFTOVER IN WALLET ->  0 Sats\n";}

transaction += "\nFEE: ";
transaction += String(float(psbt.fee())/100);
transaction += " bits\n";

transaction += "\n-------------------------\n\n-Accept? Wait 3 Minutes or Press Top Left Button\n\n-Deny? Press Top Middle Button";

//Serial.println(transaction);
const char* PSBTdetails = transaction.c_str();
//Serial.println(PSBTdetails);

      display.clearBuffer();
      display.setTextSize(1);
      testdrawtext(PSBTdetails, COLOR1);
      display.display();
      delay(3000);

countdown = 1100;
while (countdown >= 0)
{
delay(100); 
if ( digitalRead(BUTTONA) == LOW)
  {break;}
if ( digitalRead(BUTTONB) == LOW)
  {loop();}
//Serial.println(countdown);
--countdown;
}


 
  delay(1000);
  psbt.sign(root);
  Serial.println(psbt.toBase64());

  String signedthat = psbt.toBase64();
  Serial.println(signedthat);
  /////////////////////////////////////////////////////////////////////////////////////////////
  SD.remove("that.psbt");
  myFile = SD.open("that.psbt", FILE_WRITE);
  myFile.print(signedthat);
  myFile.close();
  Serial.println("signed psbt text written to SD");

  display.clearBuffer();
  display.setTextSize(2);
  testdrawtext("\n\nRemove SD card\nwith signed PSBT.", COLOR1);
  display.display();
  
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
esp_deep_sleep_start();
}


/////////////////////////////////////////////////////////////////////////////////////////////MAIN MENU + CHOICES
void signer()
{

  delay(100);
  Serial.println("Signer");

  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("Step 1: Make sure SD card slot is empty.\n\n\nStep 2: Insert SD card with Recovery Phrase\n\n\nStep 3: Press Top Left Button to continue", COLOR1);
  display.display();
  delay(500);
  
  countdown = 1100;
  while (countdown >= (0))
  {delay(100); if ( digitalRead(BUTTONA) == LOW){signthis();}--countdown;}
}





void generate()
{
  countdown = 60000;
  delay(1000);
  Serial.println("Generate");
  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("\n\n\n-Top Middle Button to Continue SingleSig\n\n\n-Top Right Button to Continue AND Print\n\n\n-Press Reset to Exit.",COLOR1);
  display.display();
  delay(500);
  while (countdown >= (0))
  {delay(100); 
  --countdown;
//  if ( digitalRead(BUTTONA) == LOW){generate_multisig();};
  if ( digitalRead(BUTTONB) == LOW){generate_wallet();};
  if ( digitalRead(BUTTONC) == LOW){generate_wallet_print();};
  }
}


void EMF()
{ delay(100);
  countdown = 1100;
  Serial.println("EMF");
  display.clearBuffer();
  display.setTextSize(1);
  testdrawtext("\n3 Minutes\n\n\n-Hold Top Left Button for Flashlight-EMF\n (Red = Source)\n\n\n-Hold Top Middle Button for Strobe\n (Red = Time Almost Up)\n\n\n-Press Top Right Button to Exit", COLOR1);
  display.display();
  delay(500);


countdown = 1100;
  while (countdown >= (0))
{
            state = 0; 
            for(int i = 0; i < sample; i++)
            {           
               state1 = analogRead(34);
               state2 = analogRead(34);
               if (state1 > state2 and (state1 > state))
               {state = state1;};
               if (state2 > state1 and (state2 > state))
               {state = state2;};
            }                     
            pixels.setPixelColor(0, pixels.Color(0,0,0)); pixels.show();                                        
            state = constrain(state, 0, 100);  
//            Serial.println(state);
            if (state == 0)
            {pixels.setPixelColor(0, pixels.Color(255,255,255));pixels.show();sample == 1000;}
              else{pixels.setPixelColor(0, pixels.Color(255,255,255));pixels.show();sample == 1000;};
            if (state > 0 and state < 21)
            {pixels.setPixelColor(0, pixels.Color(255,255,255));pixels.show();sample == 1000;}
              else{sample == 1000;};     
            if (state > 20 and state < 31)
            {pixels.setPixelColor(0, pixels.Color(0,0,255));pixels.show();sample == 900;}
              else{sample == 900;};      
            if (state > 30 and state < 41)
            {pixels.setPixelColor(0, pixels.Color(0,255,0));pixels.show();sample == 750;}
              else{sample == 750;};        
            if (state > 40 and state < 51)
            {pixels.setPixelColor(0, pixels.Color(0,0,255));pixels.show();sample == 500;}
              else{sample == 500;};        
            if (state > 50 and state < 61)
            {pixels.setPixelColor(0, pixels.Color(255,0,255));pixels.show();sample == 250;}
              else{sample == 250;};        
            if (state > 60 and state < 101)
            {pixels.setPixelColor(0, pixels.Color(255,0,0));pixels.show();sample == 100;}
              else{sample == 100;};        
            
            //
            //if (state == 0)
            //{pixels.setPixelColor(0, pixels.Color(255,255,255));pixels.show();};     
            //if (state > 0 and state < 11)
            //{pixels.setPixelColor(0, pixels.Color(0,0,255));pixels.show();};      
            //if (state > 10 and state < 21)
            //{pixels.setPixelColor(0, pixels.Color(0,255,0));pixels.show();};      
            //if (state > 20 and state < 41)
            //{pixels.setPixelColor(0, pixels.Color(0,0,255));pixels.show();};      
            //if (state > 40 and state < 61)
            //{pixels.setPixelColor(0, pixels.Color(255,0,255));pixels.show();};      
            //if (state > 60 and state < 101)
            //{pixels.setPixelColor(0, pixels.Color(255,0,0));pixels.show();}; 
if ( digitalRead(BUTTONA) == LOW){EMF();} if ( digitalRead(BUTTONB) == LOW){strobe();} if ( digitalRead(BUTTONC) == LOW){{pixels.setPixelColor(0, pixels.Color(0,0,0));pixels.show();loop();}} --countdown;};
  


countdown = 1100;
pixels.setPixelColor(0, pixels.Color(0,0,0));pixels.show();
delay(100);
loop();
}






void menu()

{Serial.println("MENU");
countdown = 1100;
while (countdown >= (0))
  {   delay(100);
      if ( (digitalRead(BUTTONA) == LOW) && (digitalRead(BUTTONB) == LOW) && (digitalRead(BUTTONC) == LOW) ) // must press all three buttons to reveal hidden menu
            { Serial.println("hidden menu reveal");
              display.clearBuffer();
              display.setTextSize(1);
              testdrawtext(mainmenutext, COLOR1);
              display.display();
              countdown = 1100;
              while (countdown >= (0))
              {
              if ( digitalRead(BUTTONA) == LOW ){signer();};
              if ( digitalRead(BUTTONB) == LOW ){generate();};
              if ( digitalRead(BUTTONC) == LOW ){EMF();};
              delay(100);
              --countdown;
              }
            
      }

--countdown;
//Serial.println(countdown);
}
}

//////////////////////////////////////////////

void setup() {

//Serial.begin(115200);
  delay(100);
Serial1.begin(9600);  // Initialize HardwareSerial for Printer


  delay(100);
  printer.begin();        // Init printer (same regardless of serial type)
  delay(100);
  pinMode(BUTTONA, INPUT_PULLUP);
  pinMode(BUTTONB, INPUT_PULLUP);
  pinMode(BUTTONC, INPUT_PULLUP);
  
  display.begin(THINKINK_MONO);
  pixels.begin();

delay(500);

PB1.reserve(257);
PB2.reserve(257);
PB3.reserve(257);
PB4.reserve(257);
finger.reserve(257);
BWxpub.reserve(257);
pu1.reserve(257);
pu2.reserve(257);
pu3.reserve(257);
pu4.reserve(257);
phra.reserve(257);
phrase.reserve(257);
ROOOT.reserve(257);
pubx.reserve(257);
finalString.reserve(257);
psbtString.reserve(257);

}





void loop() 
{ 
  delay(250);
  quoter();
  menu();

  delay(100);
  quoter();
  menu();

  delay(100);
  quoter();
  menu();

  pinMode(led, OUTPUT);
  digitalWrite(led,HIGH);
  digitalWrite(led,LOW);

  delay(1000);
  
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);
  
  esp_deep_sleep_start();
}

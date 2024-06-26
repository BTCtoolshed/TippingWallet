# TippingWallet
Create and print temporary Bitcoin wallets using a ESP32 microcontroller and Thermal Printer. 

# READ this whole page and all instructions before proceeding!!!

<br><br>
# Shopping List
![Shopping List](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/shoppingList.png?raw=true)
* A. Adafruit FeatherWing eINK 2.9" Grayscale (preferred) or Tricolor IL0373 (laggy, may require multiple button pushes)
* B. Adafruit Feather ESP32 v2 (choose assembled headers option if you dont want to solder pins on) Product ID 5900 or 5400
* C. USB-C Data Cable for flashing (Not a regular charging cable! This is specifically a data cable.)
* D. Adafruit Nano Thermal Printer (If unavailable, the newest version of my software also supports QR 204 Thermal Printers over serial, scroll to bottom of this page for instructions for QR204)
* E. 1 Amp or 2 Amp 9V Power Supply (Either one works)
* F. Terminal Block that fits Power Supply Jack (Female)
* G. Small breadboard
* H. Jumper wires for microcontrollers/small electronics (Male endings on both ends)
* *Optional: 2 SD Cards (smallest size in MB possible) and Adafruit Feather Lipo Battery
* You also need a computer with a USB port and with Arduino IDE installed

<br><br><br>
**1st installation option : Flashing Binary to the ESP32**<br>
*Warning : By using this option you are trusting I am not a malicious actor and that the software I used to compile this bin was not compromised!!!*
1) Download the "Tipping Wallet v1_2 Grayscale.bin" or "Tipping Wallet v1_2 Tricolor.bin" file from this github repository, depending on which Featherwing you have.<br>
2) You will need to install Python on your computer if it isn't already installed.<br>
3) Then you will need to install esptool (see here https://docs.espressif.com/projects/esptool/en/latest/esp32/installation.html)<br>
5) Plug in your USB data cable to your esp32 device and connect to your computer's USB.<br>
6) Identify the comms port (Windows example COM9) or device path (Linux/Mac example /dev/ttyUSB0) of your esp32. Google this if you don't know how.<br>
7) Open CMD (command prompt). Navigate to the folder in which you installed esptool.py <br>
8) Type: python esptool.py -p YOURCOMPORTorDEVICEPATH -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x10000 FULLPATHTOWHEREYOUDOWNLOADEDtheBINFILE <br>
9) Hit enter. <br>
10) Device should flash in under a minute and be ready for use. <br><br>

Example of an esp32 device connected via COM port 9: python esptool.py -p COM9 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x10000 C:\Users\YourUser\Desktop\TippingWallet.bin

<br><br><br>
**2nd installation option : The way of pain in the Arduino IDE**
Read the .ino file appropriate for your Featherwing in the repository, especially the comments in the header. I do not have the time to support other boards or setups, unfortunately, other than the Adafruit ESP32 v2 Feather and eInk FeatherWing 2.9". If you are new to Arduino IDE, checkout https://randomnerdtutorials.com/ for ESP32 programming tutorials. You will need to know how to install new libraries and program an ESP32 for this project. Why am I not making this easier? You should have to read code in order to trust what you are doing when it comes to Hardware Wallets.

Make sure you also view Adafruit for basic instructions on installing libraries for the ThinkInk feather and/or Nano Thermal Printer. I do not follow Adafruit's power instructions on the thermal printer though, preferring a 1amp DC power source instead of a 2amp that would require a logic stepdown chip to prevent burning out the 3.3v ESP32 board. Instead you can use the 1amp power source but adjust certain settings in the libraries as noted in the .ino file and also plug the thermal printer in for 15 minutes or so to let it heat up before printing.

Thermal printing is great for setting up burner wallets - for tipping servers for example. This wallet will produce airgapped, truly random private bitcoin keys. Although ESP32s have a native function for generating true random numbers that has been evaluated by others as being truly random, I advise you to only use this for creating temporary use wallets. Bitcoin Core remains the gold standard in random key generation for bitcoin wallets.

In full transparency, this is where the private keys are generated. esp_random is a native function in ESP32:
![Randomization](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Random.png?raw=true)
![Randomization](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Random2.png?raw=true)



<br><br><br><br>
# Hardware Assembly
Assembling the ESP32 Feather and eInk FeatherWing is simple and straightforward.<br>
![Assembly Step 1](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Assembly01.png?raw=true)

If you have a battery, place it in the middle of the FeatherWing thus.<br>
![Assembly Step 2](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Assembly02.png?raw=true)

Seat the Feather into the FeatherWing. Be gentle pushing the pins in, and make sure you are lining them up properly or when power is applied you will destroy the board.<br>
![Assembly Step 3](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Assembly03.png?raw=true)

Seat the battery plug if applicable. You can power the device/charge the battery with the usb-C data cable.<br>
![Assembly Step 4](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Assembly04.png?raw=true)





<br><br><br><br>
**The printer is a little more complicated.**

Hook up your jumper wires in the printer's interface. I prefer to match the colors from the printer's interface for my own sanity.<br>
![Printer Assembly Step 1](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Print01.png?raw=true)

Insert the red and black jumper wires into the breadboard like below. Then take your Terminal Block and screw in red and black jumper wires. Place these into the breadboard in the same manner as below. Finally insert a yellow jumper wire into the black wires' row; these represents the Ground that the Printer and the ESP32 must share.<br>
![Printer Assembly Step 2](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Print02.png?raw=true)

Insert the DC Jack from the 1 Amp 9 Volt Power Supply into the Terminal Block. The final setup of the printer should look like this.<br>
![Printer Assembly Step 3](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Print03.png?raw=true)

Lastly, place the yellow ground jumper wire into the GND slot of the FeatherWing (should be 4th from the left in this view). Then place the blue jumper wire into the TX slot (2nd from right) and the green jumper wire into the RX slot (3rd from right).<br>
![Printer Assembly Step 4](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Print04.png?raw=true)






<br><br><br><br>
# Printing a Wallet
A submenu sits behind a random quote generator.
To access the submenu, click all 3 top buttons on the eInk Feather Wing at the same time to enter the submenu.<br>
![Print Step 1](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint01.png?raw=true)

Pick the Middle button to print a wallet. The eInk display will give you clear step by step instructions. There is an option to save the Xpub and Private Key Phrase (BIP39) to separate SD cards after the printing has finished.<br>

![Print Step 3](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint02.png?raw=true)

If you wish to save all wallet information to an SD card, insert it now before clicking the top middle button.<br>

![Print Step 4](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint03.png?raw=true)

If you did not insert an SD card, you will receive this error, but that is okay. Sometimes the wallet memory fails at this step and you have to restart the process. This is normal.<br>

![Print Step 5](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint04.png?raw=true)

This is what the current burner wallet looks like printed. You can rewrite the text by editing the ino file if you wish (only if you are able to install with Option 2 via Arduino IDE).<br>

![Print Step 13](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint12.png?raw=true)

After the ESP32 attempts to print (and it will always attempt to print even if the printer is not hooked up), you can access the Bitcoin wallet's Xpub, First receiving address and Private Phrase/Key. If you want you can insert an SD card before choosing option 2 or 3. Option 2 will just save the XPUB info to the SD card. Option 3 will save the all wallet information to the SD card.<br>
![Print Step 2](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrintMenu.png?raw=true)

After pressing the upper left button, the 1st BTC receiving address is displayed. You can use this is to verify if an XPUB was entered correctly. You can hold the upper left button to escape back to the Wallet menu.<br>

![Print Step 6](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint05.png?raw=true)

On the Wallet menu, after pressing the upper middle button, the XPUB QR Code is displayed. You can use this to load a watch only wallet. For example, you can scan this easily into the BlueWallet app for a watch-only wallet. To escape to the next screen, hold the upper middle button.<br>

![Print Step 7](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint06.png?raw=true)

![Print Step 8](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint07.png?raw=true)

This displays all the XPUB watch only wallet information. You can write this down if you wish.<br>

![Print Step 9](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint08.png?raw=true)

On the Wallet menu, after pressing the upper right button, the Private Phrase (BIP39) QR Code is displayed. You can use this to load a hot wallet. For example, you can scan this easily into the BlueWallet app through the import function. To escape to the next screen, hold the upper right button.<br>

![Print Step 10](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint09.png?raw=true)

![Print Step 11](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint10.png?raw=true)

This displays all the private key/phrase wallet information. You can write this down if you wish. Even if I am giving this as a tip, I prefer to keep the private keys for myself in case the person never uses it. I try to tell people they need to transfer the bitcoin within 24 hours or I will retake it.<br>

![Print Step 12](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint11.png?raw=true)

<br><br><br><br>
# Airgapped Transaction Signing
A submenu sits behind a random quote generator.
To access the submenu, click all 3 top buttons on the eInk Feather Wing at the same time to enter the submenu.
Pick the Left button to begin signing a transaction. You will need an SD card with the Private Key Phrase to start the process. 

You will also need another SD card on which you have saved a Partially Signed Bitcoin Transaction. You can do this with BlueWallet or other wallets. Save the PSBT as this name: "This.psbt" to the SD card. 

Follow the instructions on the eInk display for signing a transaction. After the transaction is signed, the full transaction will be listed in the file "THAT.psbt" . You can broadcast the transaction in Bitcoin Core or BlueWallet or any other wallet that can accept Bitcoin raw transactions.

<br><br><br><br>
# For Fun: Flashlight, Strobe Light and EMF Detector
A submenu sits behind a random quote generator.
To access the submenu, click all 3 top buttons on the eInk Feather Wing at the same time to enter the submenu.
Pick the Right button to access the Flashlight functions.

The flashlight stays lit for 3 minutes and then return to the main menu, unless you click the left button to reset that 3 minutes. To use the EMF detector, just hold the wallet close to possible EMF sources. If the light turns red it means there is a heavy EMF source. The EMF detector is just a fun tool, not a serious one.

You can also click the middle button for a strobe.

<br><br><br><br>
# QR204 Thermal Printer
Since Adafruit no longer sells thermal printers, I added support for the QR204 thermal printer - available on Amazon and other retailers.

The setup is very similar, but the QR204 uses two separate jumper connections. One is for power and the other for serial connection. The connector with a 2-wire female jack is for power: red is for vin (positive/power) and black is for ground. The other connector is a 5-wire jack. You will need to connect the ground to the same ground as the 2-wire connector and also the ESP32's ground connection. The RX and TX on the 5-wire jack connect to the RX and TX of the ESP32. In the photo example of the thermal printer connection in the sections above, you would just need to add an extra jumper wire for the ground in the same row as the yellow jumper wire and connect that to the "extra" ground connection for the QR204. The Adafruit printer only has 1 ground connection; the QR204 has 2 ground connections.

If the printer does not print from the ESP32 on its first test, you may have the TX and RX reversed. It won't harm the controller if you have them switched, it just won't work until you switch them to the correct slots.

<br><br><br><br>
# BETA version 1.3 Hidden One Time Pad printer
Only found in the BETA folder...

If you jump to the screen where the xpub QR code is displayed in the SingleSig non-print wallet generation option, hold the upper LEFT button for 2 seconds and it will print two One Time Pads. They are alphabetical only. You can use the OTP encryption and decryption method by assigning positive numbers 0 through +25 to A-Z (A = 0, B = 1, so on and so forth. Don't change that. They are A=0, B=1, so on and so forth). a-z lowercase can be assigned to negative numbers 0 through -25.

Example : Let's encrypt and decrypt "HELLO"
1) "HELLO" when translated to numbers is always going to be 7 4 11 11 14
2) Let's say the first five letter on the OTP are "JTNDI", which is 9 19 13 3 8
3) Subtract "JTNDI" from "HELLO" = 7-9, 4-19, 11-13, 11-3, 14-8
4) This gives us -2, -15, -2, 8, 6 or "cpcIG". Send this to your recipient. This is the encoded message.
5) Decode = Add "JTNDI" to "cpcIG" = -2+9, -15+19, -2+13, 8+3, 6+8
6) This gives us 7 4 11 11 14, which is "HELLO"

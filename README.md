# TippingWallet
Create and print temporary Bitcoin wallets using a ESP32 microcontroller and Thermal Printer. 

# READ this whole page and all instructions before proceeding!!!
From me ........ Read the .ino file, especially the comments in the header. I do not have the time to support other boards or setups, unfortunately. If you are new to Arduino IDE, checkout https://randomnerdtutorials.com/ for ESP32 programming tutorials. You will need to know how to install new libraries and program an ESP32 for this project. Why am I not making this easier? You should have to read code in order to trust what you are doing when it comes to Hardware Wallets.

Make sure you also view Adafruit for basic instructions on installing libraries for the ThinkInk feather and/or Nano Thermal Printer. I do not follow Adafruit's power instructions on the thermal printer though, preferring a 1amp DC power source instead of a 2amp that would require a logic stepdown chip to prevent burning out the 3.3v ESP32 board. Instead you can use the 1amp power source but adjust certain settings in the libraries as noted in the .ino file and also plug the thermal printer in for 15 minutes or so to let it heat up before printing.

Thermal printing is great for setting up burner wallets - for tipping servers for example. This wallet will produce airgapped, truly random private bitcoin keys. Although ESP32s have a native function for generating true random numbers that has been evaluated by others as being truly random, I advise you to only use this for creating temporary use wallets. Bitcoin Core remains the gold standard in random key generation for bitcoin wallets.

# Shopping List
![Shopping List](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/shoppingList.png?raw=true)
* A. Adafruit FeatherWing eINK 2.9"
* B. Adafruit Feather ESP32 v2
* C. USB-C Data Cable for flashing
* D. Adafruit Nano Thermal Printer
* E. 1 Amp 9V Power Supply
* F. Terminal Block that fits Power Supply Jack (Female)
* G. Small breadboard
* H. Jumper wires for microcontrollers/small electronics
* *Optional: 2 SD Cards (smallest size in MB possible) and Adafruit Feather Lipo Battery
* You also need a computer with a USB port and with Arduino IDE installed

# Hardware Assembly
Assembling the ESP32 Feather and eInk FeatherWing is simple and straightforward.
![Assembly Step 1](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Assembly01.png?raw=true)

If you have a battery, place it in the middle of the FeatherWing thus.
![Assembly Step 2](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Assembly02.png?raw=true)

Seat the Feather into the FeatherWing. It is impossible to misplace the pins, but be gentle.
![Assembly Step 3](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Assembly03.png?raw=true)

Seat the battery plug if applicable.
![Assembly Step 4](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Assembly04.png?raw=true)


**The printer is a little more complicated.**

Hook up your jumper wires in the printer's interface. I prefer to match the colors from the printer's interface for my own sanity.

![Printer Assembly Step 1](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Print01.png?raw=true)

Insert the red and black jumper wires into the breadboard like below. Then take your Terminal Block and screw in red and black jumper wires. Place these into the breadboard in the same manner as below. Finally insert a yellow jumper wire into the black wires' row; these represents the Ground that the Printer and the ESP32 must share.

![Printer Assembly Step 2](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Print02.png?raw=true)

Insert the DC Jack from the 1 Amp 9 Volt Power Supply into the Terminal Block. The final setup of the printer should look like this.

![Printer Assembly Step 3](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Print03.png?raw=true)

Lastly, place the yellow ground jumper wire into the GND slot of the FeatherWing (should be 4th from the left in this view). Then place the blue jumper wire into the TX slot (2nd from right) and the green jumper wire into the RX slot (3rd from right).

![Printer Assembly Step 4](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/Print04.png?raw=true)

# Printing a Wallet
A submenu sits behind a random quote generator.
To access the submenu, click all 3 top buttons on the eInk Feather Wing at the same time to enter the submenu.
![Print Step 1](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint01.png?raw=true)

Pick the Middle button to print a wallet. The eInk display will give you clear step by step instructions. There is an option to save the Xpub and Private Key Phrase (BIP39) to separate SD cards after the printing has finished.

![Print Step 3](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint02.png?raw=true)

If you wish to save all wallet information to an SD card, insert it now before clicking the top middle button.

![Print Step 4](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint03.png?raw=true)

If you did not insert an SD card, you will receive this error, but that is okay. Sometimes the wallet memory fails at this step and you have to restart the process. This is normal.

![Print Step 5](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint04.png?raw=true)

This is what the current burner wallet looks like printed. You can rewrite the text by editing the ino file if you wish.

![Print Step 13](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint12.png?raw=true)

After the ESP32 attempts to print (and it will always attempt to print even if the printer is not hooked up), you can access the Bitcoin wallet's Xpub, First receiving address and Private Phrase/Key
![Print Step 2](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrintMenu.png?raw=true)



![Print Step 6](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint05.png?raw=true)

![Print Step 7](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint06.png?raw=true)

![Print Step 8](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint07.png?raw=true)

![Print Step 9](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint08.png?raw=true)

![Print Step 10](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint09.png?raw=true)

![Print Step 11](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint10.png?raw=true)

![Print Step 12](https://github.com/BTCtoolshed/TippingWallet/blob/main/Photos/wPrint11.png?raw=true)


# Airgapped Transaction Signing
A submenu sits behind a random quote generator.
To access the submenu, click all 3 top buttons on the eInk Feather Wing at the same time to enter the submenu.
Pick the Left button to begin signing a transaction. You will need an SD card with the Private Key Phrase to start the process. 

You will also need another SD card on which you have saved a Partially Signed Bitcoin Transaction. You can do this with Bitcoin Core or with BlueWallet or other wallets. Save the PSBT as this name: "This.psbt" to the SD card. 

Follow the instructions on the eInk display for signing a transaction. After the transaction is signed, the full transaction will be listed in the file "THAT.psbt" . You can broadcast the transaction in Bitcoin Core or BlueWallet or any other wallet that can accept Bitcoin raw transactions.

# For Fun: Flashlight, Strobe Light and EMF Detector
A submenu sits behind a random quote generator.
To access the submenu, click all 3 top buttons on the eInk Feather Wing at the same time to enter the submenu.
Pick the Right button to access the Flashlight functions.

The flashlight stays lit for 3 minutes and then return to the main menu, unless you click the left button to reset that 3 minutes. To use the EMF detector, just hold the wallet close to possible EMF sources. If the light turns red it means there is a heavy EMF source. The EMF detector is just a fun tool, not a serious one.

You can also click the middle button for a strobe.

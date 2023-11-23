# TippingWallet
Create and print temporary Bitcoin wallets using a ESP32 microcontroller and Thermal Printer. 

# READ this whole page and all instructions before proceeding!!!
From me ........ Read the .ino file, especially the comments in the header. I do not have the time to support other boards or setups, unfortunately. If you are new to Arduino IDE, checkout https://randomnerdtutorials.com/ for ESP32 programming tutorials. You will need to know how to install new libraries and program an ESP32 for this project. Why am I not making this easier? You should have to read code in order to trust what you are doing when it comes to Hardware Wallets.

Make sure you also view Adafruit for basic instructions on installing libraries for the ThinkInk feather and/or Nano Thermal Printer. I do not follow Adafruit's power instructions on the thermal printer though, preferring a 1amp DC power source instead of a 2amp that would require a logic stepdown chip to prevent burning out the 3.3v ESP32 board. Instead you can use the 1amp power source but adjust certain settings in the libraries as noted in the .ino file and also plug the thermal printer in for 15 minutes or so to let it heat up before printing.

Thermal printing is great for setting up burner wallets - for tipping servers for example. This wallet will produce airgapped, truly random private bitcoin keys. Although ESP32s have a native function for generating true random numbers that has been evaluated by others as being truly random, I advise you to only use this for creating temporary use wallets. Bitcoin Core remains the gold standard in random key generation for bitcoin wallets.

# Shopping List
![Shopping List](https://github.com/BTCtoolshed/TippingWallet/Photos/blob/main/shoppingList.png?raw=true)
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

The printer is a little more complicated.

# Printing a Wallet
A submenu sits behind a random quote generator.
To access the submenu, click all 3 top buttons on the eInk Feather Wing at the same time to enter the submenu.
Pick the Middle button to print a wallet. The eInk display will give you clear step by step instructions. There is an option to save the Xpub and Private Key Phrase (BIP39) to separate SD cards after the printing has finished.

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

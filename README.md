# TippingWallet
Create and print temporary Bitcoin wallets using a ESP32 microcontroller and Thermal Printer. 

# READ ALL INSTRUCTIONS BEFORE PROCEEDING
From me ........ Read the .ino file, especially the comments in the header. I do not have the time to support other boards or setups, unfortunately. If you are new to Arduino IDE, checkout https://randomnerdtutorials.com/ for ESP32 programming tutorials. You will need to know how to install new libraries and program an ESP32 for this project. Why am I not making this easier? You should have to read code in order to trust what you are doing when it comes to Hardware Wallets.

Make sure you also view Adafruit for basic instructions on installing libraries for the ThinkInk feather and/or Nano Thermal Printer. I do not follow Adafruit's power instructions on the thermal printer though, preferring a 1amp DC power source instead of a 2amp that would require a logic stepdown chip to prevent burning out the 3.3v ESP32 board. Instead you can use the 1amp power source but adjust certain settings in the libraries as noted in the .ino file and also plug the thermal printer in for 15 minutes or so to let it heat up before printing.

Thermal printing is great for setting up burner wallets - for tipping servers for example. This wallet will produce airgapped, truly random private bitcoin keys. Although ESP32s have a native function for generating true random numbers that has been evaluated by others as being truly random, I advise you to only use this for creating temporary use wallets. Bitcoin Core remains the gold standard in random key generation for bitcoin wallets.

# Shopping List
* Adafruit FeatherWing eINK
* Adafruit Feather ESP32 v2 + USB-C Data Cable for flashing
* Adafruit Nano Thermal Printer
* 1 Amp 9V Power Supply
* Terminal Block that fits Power Supply Jack (Female)
* Half breadboard
* Jumper wires for microcontrollers/small electronics
* Waterproof notepad (for keeping private keys safe)
* Optional: 2 SD Cards (smallest size in MB possible) and Adafruit Feather Lipo Battery

# Hardware Assembly
# Printing a Wallet
# Airgapped Transaction Signing
# For Fun: Flashlight, Strobe Light and EMF Detector

# TippingWallet
Create and print temporary Bitcoin wallets using a ESP32 microcontroller and Thermal Printer

ESP32-BITCOIN-WALLET_EMF_wFeeder is most up to date version 5.24.23.

From me ........ Read the .ino file, especially the comments in the header. I do not have the time to support other boards or setups, unfortunately.

Make sure you also view Adafruit for basic instructions on installing libraries for the ThinkInk feather and/or Nano Thermal Printer. I do not follow Adafruit's power instructions on the thermal printer though, preferring a 1amp DC power source instead of a 2amp that requires a logic stepdown chip to prevent burning out the 3.3v ESP32 board.

Thermal printing is great for setting up burner wallets - for tipping servers for example.

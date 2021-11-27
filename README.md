# CatFeeder Timer
An arduino project to more accurately control a cat feeder. By removing the inaccurate analog timer in a Cat Mate C50 and putting a servo in its place you can create a poor mans version of the more recent digital C500.

## Parts
 - Arduino micro
 - Micro servo
 - 4 buttons
 - Display (2 row)
 - Power wart (any voltage that the micro will take is fine)
 - Barrel connector for the power connection

## Setup
As each servo wil require different values to trigger each bin, first flash the calibration project to the micro and use the 4 buttons to find the correct values for a neutral position and the 4 trigger positions. Once you find them, note them down in the NEUTRAL variable and the STEP array. Upon reflashing the calibration project it'll attempt to open the 4 bins so you can test the opening sequence.

Once you are happy with the values, copy them into catfeedertimer.ino and flash that to the micro.

## Usage
Upon booting, use the 4 buttons to set the current time (arduino has no internal clock) and the times the 4 bins should open. Menu text is in Dutch, feel free to translate.
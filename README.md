# README

## Introduction
This repository is a small project of mine which I wanted to publish. This project is a small coffee asisstant helper, helping you get the perfect shot of espresso or lungo from you espressomachine.
The user is able to input the:
- Kind of drink (espresso or lungo)
- Strength of Espresso (lungo is always 'strong)
- Dose of your coffee (7, 8, 9 for single or 14, 15, 16 for double. My machine can't fit above 16gr in one portafilter that's why)
- Grind size of the beangrinder

Then it will calculate the amount of ouput your drink has to be (in grams) and the extraction time. Once you start brewing a timer starts and you can press stop to stop the timer. From there the device will give you feedback with your time. 
This means always try to get the right amount of output of your drink, once that is reached stop the timer. Once extraction is done the display will give you feedback about your shot and what grind to change.

## Hardware
- ESP32 Wroom-32
- 1.8 inch TFT ST7735 Display SPI
- 2 Buttons

## Software
I've used Arduino IDE instead of ESP-IDF since it's easier using external libaries, this project is using Adafruit ST7735 library for the display.

## Future
More features might be added in the future

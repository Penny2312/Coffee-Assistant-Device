#pragma once
#include <Adafruit_ST7735.h>
#include "BrewCalculator.h"
#include "Buttons.h"

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define COFFEE_BROWN  0x79E0   // warm brown
#define COFFEE_YELLOW 0xFFE0   // golden crema
#define COFFEE_GREEN  0x07E0   // selection (keep green)
#define COFFEE_DARK   0x4208   // dark grey/brown

enum UIState {
  BOOT,
  SELECT_DRINK,
  SELECT_STRENGTH,
  SHOW_DOSE,
  SET_GRIND,
  SUMMARY,
  BREWING,
  RESULT,
  ADVICE
};

class UI {
public:
  UI();
  void init();
  void update();

private:
  Adafruit_ST7735 tft;
  Buttons buttons;

  UIState state;
  UIState lastState;

  DrinkType drink = ESPRESSO;

  int drinkIndex = 0;
  BrewStrength strength = MEDIUM;
  int grindSize = 1;

  float shotTime = 0.0f;
  unsigned long lastSecondTick = 0;
  float lastShownTime = -1.0f;
  int doseIndex;
  int dose = 0;

  bool redraw = false;

  BrewResult lastResult;
  CoffeeCalculator calculator;

  void drawBoot(bool redraw);
  void drawDrinkSelect(bool redraw);
  void drawStrengthSelect(bool redraw);
  void drawDose(bool redraw);
  void drawGrind(bool redraw);
  void drawSummary(bool redraw);
  void drawBrewing(bool redraw);
  void drawResult(bool redraw);
  void drawAdvice(bool redraw);
  void printCentered(const char* text, int y, uint16_t color, uint8_t size);
  const char* currentRatioText();
};

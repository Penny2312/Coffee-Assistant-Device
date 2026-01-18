#include "UI.h"

#define TFT_CS   5
#define TFT_DC   16
#define TFT_RST  17
#define TFT_MOSI 21
#define TFT_SCLK 22

const int doses[] = {7, 8, 9, 14, 15, 16};

UI::UI() : tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST) {}

void UI::init() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(0);

  buttons.init();
  state = BOOT;
  lastState = (UIState)-1;
}

void UI::update() {
  if (state != lastState) {
    tft.fillScreen(COFFEE_BROWN);   // BACKGROUND CHANGED
    lastState = state;
    redraw = true;
  } else {
    redraw = false;
  }

  switch (state) {
    case BOOT: drawBoot(redraw); break;
    case SELECT_DRINK: drawDrinkSelect(redraw); break;
    case SELECT_STRENGTH: drawStrengthSelect(redraw); break;
    case SHOW_DOSE: drawDose(redraw); break;
    case SET_GRIND: drawGrind(redraw); break;
    case SUMMARY: drawSummary(redraw); break;
    case BREWING: drawBrewing(redraw); break;
    case RESULT: drawResult(redraw); break;
    case ADVICE: drawAdvice(redraw); break;
  }
}

/* ---------- CENTERED TEXT HELPER ---------- */
void UI::printCentered(const char* text, int y, uint16_t color, uint8_t size) {
  int16_t x1, y1;
  uint16_t w, h;

  tft.setTextSize(size);
  tft.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  int x = (tft.width() - w) / 2;

  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.print(text);
}

const char* UI::currentRatioText() {
  if (drink == ESPRESSO) {
    return (strength == STRONG) ? "RAT 1:1.5" : "RAT 1:2";
  } else {
    return "RATIO 1:3";
  }
}

/* ---------- BOOT ---------- */
void UI::drawBoot(bool redraw) {
  if (!redraw) return;
  tft.fillScreen(COFFEE_BROWN); // BACKGROUND CHANGED
  printCentered("ESPRESSO", 50, COFFEE_YELLOW, 2);
  printCentered("ASSIST", 75, COFFEE_YELLOW, 2);
  delay(1000);
  state = SELECT_DRINK;
}

/* ---------- DRINK SELECT ---------- */
void UI::drawDrinkSelect(bool redraw) {
  static int lastDrink = -1;
  const char* drinks[] = {"ESPRESSO", "LUNGO"};

  if (redraw) {
    printCentered("SELECT", 10, COFFEE_YELLOW, 2);
    printCentered("DRINK", 30, COFFEE_YELLOW, 2);
    lastDrink = -1;
  }

  int baseY = 55;
  int lineH = 22;

  if (drink != lastDrink) {
    for (int i = 0; i < 2; i++) {
      uint16_t bg = (i == drink) ? COFFEE_GREEN : COFFEE_BROWN;  // BACKGROUND CHANGED
      uint16_t fg = (i == drink) ? BLACK : WHITE;               // SELECTABLE TEXT = WHITE
      tft.fillRect(0, baseY + i * lineH - 2, tft.width(), lineH, bg);
      printCentered(drinks[i], baseY + i * lineH, fg, 2);
    }
    lastDrink = drink;
  }

  if (buttons.nextPressed()) {
    drink = (drink == ESPRESSO) ? LUNGO : ESPRESSO;
    delay(150);
  }

  if (buttons.selectPressed()) {
    state = SELECT_STRENGTH;
    delay(150);
  }
}

/* ---------- STRENGTH ---------- */
void UI::drawStrengthSelect(bool redraw) {
  // Lungo only supports MEDIUM strength
  if (drink == LUNGO && strength == STRONG) {
    strength = MEDIUM;
  }

  static int lastStrength = -1;
  const char* labels[] = {"MEDIUM", "STRONG"};

  if (redraw) {
    printCentered("STRENGTH", 10, COFFEE_YELLOW, 2);
    lastStrength = -1;
    printCentered(currentRatioText(), 100, COFFEE_YELLOW, 2);
  }

  int baseY = 45;
  if (drink == ESPRESSO) {
    if (strength != lastStrength) {
      for (int i = 0; i < 2; i++) {
        uint16_t bg = (i == strength) ? COFFEE_GREEN : COFFEE_BROWN;  // BACKGROUND CHANGED
        uint16_t fg = (i == strength) ? BLACK : WHITE;               // SELECTABLE TEXT = WHITE
        tft.fillRect(0, baseY + i * 25 - 2, tft.width(), 22, bg);
        printCentered(labels[i], baseY + i * 25, fg, 2);
      }
      lastStrength = strength;
    }
  } else if (redraw) {
    tft.fillRect(0, 40, tft.width(), 22, COFFEE_GREEN);
    printCentered("STRONG", baseY, BLACK, 2);
  }

  if (buttons.nextPressed() && drink == ESPRESSO) {
    strength = (strength == MEDIUM) ? STRONG : MEDIUM;
    delay(150);
  }

  if (buttons.selectPressed()) {
    state = SHOW_DOSE;
    delay(150);
  }
}

/* ---------- DOSE (SINGLE / DOUBLE) ---------- */
void UI::drawDose(bool redraw) {
  static int lastDoseIndex = -1;
  const int single[] = {7, 8, 9};
  const int dbl[] = {14, 15, 16};

  if (redraw) {
    printCentered("DOSE", 10, COFFEE_YELLOW, 2);

    tft.setTextSize(2);
    tft.setTextColor(WHITE);          // SELECTABLE TEXT = WHITE
    tft.setCursor(10, 40); tft.print("SIN");
    tft.setCursor(79, 40); tft.print("DBL");

    lastDoseIndex = -1;
  }

  if (doseIndex != lastDoseIndex) {
    for (int i = 0; i < 3; i++) {
      bool selS = (doseIndex == i);
      bool selD = (doseIndex == i + 3);

      tft.fillRect(5, 65 + i * 22, 45, 20, selS ? COFFEE_GREEN : COFFEE_BROWN); // BG CHANGED
      tft.fillRect(74, 65 + i * 22, 45, 20, selD ? COFFEE_GREEN : COFFEE_BROWN); // BG CHANGED

      tft.setTextSize(2);
      tft.setTextColor(selS ? BLACK : WHITE); // SELECTABLE TEXT = WHITE
      tft.setCursor(10, 68 + i * 22); tft.print(single[i]); tft.print("g");

      tft.setTextColor(selD ? BLACK : WHITE); // SELECTABLE TEXT = WHITE
      tft.setCursor(79, 68 + i * 22); tft.print(dbl[i]); tft.print("g");
    }
    lastDoseIndex = doseIndex;
  }

  if (buttons.nextPressed()) {
    doseIndex = (doseIndex + 1) % 6;
    delay(150);
  }

  if (buttons.selectPressed()) {
    dose = doses[doseIndex];
    state = SET_GRIND;
    delay(150);
  }
}

/* ---------- GRIND SIZE ---------- */
void UI::drawGrind(bool redraw) {
  static int lastGrind = 0;

  if (redraw) {
    printCentered("GRIND SIZE", 10, COFFEE_YELLOW, 2);
    printCentered("1 - 20", 30, WHITE, 2);  // SELECTABLE TEXT = WHITE
    lastGrind = 0;
  }

  if (grindSize != lastGrind) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", grindSize);
    tft.fillRect(0, 65, tft.width(), 30, COFFEE_BROWN); // BG CHANGED
    printCentered(buf, 70, COFFEE_GREEN, 2);
    lastGrind = grindSize;
  }

  if (buttons.nextPressed()) {
    grindSize = (grindSize + 1) % 21;
    delay(150);
  }

  if (buttons.selectPressed()) {
    state = SUMMARY;
    delay(150);
  }
}

/* ---------- SUMMARY ---------- */
void UI::drawSummary(bool redraw) {
  if (redraw) {
    BrewResult r = calculator.calculate(dose, strength, drink, grindSize, shotTime);

    printCentered("READY", 10, COFFEE_YELLOW, 2);

    char buf[32];
    snprintf(buf, sizeof(buf), "DOSE: %dg", dose);
    printCentered(buf, 45, WHITE, 2);   // SELECTABLE TEXT = WHITE

    snprintf(buf, sizeof(buf), "OUT: %dg", (int)r.targetOutput);
    printCentered(buf, 65, WHITE, 2);   // SELECTABLE TEXT = WHITE

    printCentered(currentRatioText(), 85, WHITE, 2); // SELECTABLE TEXT = WHITE

    snprintf(buf, sizeof(buf), "TIME: %ds", r.targetTime);
    printCentered(buf, 105, COFFEE_YELLOW, 2);

    printCentered("START BREW", 135, COFFEE_GREEN, 2);

  }

  if (buttons.selectPressed()) {
    shotTime = 0.0f;
    lastSecondTick = millis();
    state = BREWING;
    delay(150);
  }
}

/* ---------- BREWING ---------- */
void UI::drawBrewing(bool redraw) {
  if (redraw) {
    tft.fillScreen(COFFEE_BROWN); // BACKGROUND CHANGED
    printCentered("BREWING...", 10, COFFEE_YELLOW, 2);
    lastShownTime = -1.0f;   // force first draw
    printCentered("TIME", 50, WHITE, 2); // SELECTABLE TEXT = WHITE
  }
  // update time using millis()
  unsigned long now = millis();
  if (now - lastSecondTick >= 100) {   // 0.1s resolution
    lastSecondTick = now;
    shotTime += 0.1f;
  }
  // redraw only if value changed
  if ((int)(shotTime * 10) != (int)(lastShownTime * 10)) {
    lastShownTime = shotTime;
    tft.fillRect(0, 70, tft.width(), 30, COFFEE_BROWN); // BG CHANGED
    char buf[16];
    snprintf(buf, sizeof(buf), "%.1fs", shotTime);
    printCentered(buf, 75, COFFEE_GREEN, 2);
  }

  if (buttons.selectPressed()) {
    lastResult = calculator.calculate(dose, strength, drink, grindSize, (int)shotTime);
    state = RESULT;
    delay(150);
  }
}

/* ---------- RESULT ---------- */
void UI::drawResult(bool redraw) {
  if (redraw) {
    printCentered("RESULT", 10, COFFEE_YELLOW, 2);

    char buf[32];
    snprintf(buf, sizeof(buf), "%.1fs", shotTime);
    printCentered(buf, 55, COFFEE_GREEN, 2);

    snprintf(buf, sizeof(buf), "TARG: %ds", lastResult.targetTime);
    printCentered(buf, 75, WHITE, 2); // SELECTABLE TEXT = WHITE
  }

  if (buttons.selectPressed()) {
    state = ADVICE;
  }
}

/* ---------- ADVICE ---------- */
void UI::drawAdvice(bool redraw) {
  if (redraw) {
    printCentered("ADVICE", 10, COFFEE_YELLOW, 2);

    if (lastResult.grindAdjustment < 0) {
      printCentered("TOO FAST", 50, RED, 2);
      printCentered("GRIND", 80, WHITE, 2);   // SELECTABLE TEXT = WHITE
      printCentered("FINER", 100, WHITE, 2);  // SELECTABLE TEXT = WHITE
    }
    else if (lastResult.grindAdjustment > 0) {
      printCentered("TOO SLOW", 50, RED, 2);
      printCentered("GRIND", 80, WHITE, 2);   // SELECTABLE TEXT = WHITE
      printCentered("COARSER", 100, WHITE, 2); // SELECTABLE TEXT = WHITE
    }
    else {
      printCentered("PERFECT", 65, COFFEE_GREEN, 2);
      printCentered("SHOT", 85, COFFEE_GREEN, 2);
    }
  }

  if (buttons.selectPressed()) {
    state = SELECT_DRINK;
  }
}

#pragma once

enum BrewStrength {
    MEDIUM,
    STRONG
};

enum DrinkType {
  ESPRESSO,
  LUNGO
};

struct BrewResult {
    float targetOutput;
    int targetTime;
    int grindAdjustment;
};

class CoffeeCalculator {
public:
    BrewResult calculate(
        float dose,
        BrewStrength strength,
        DrinkType drink,
        int currentGrind,
        float actualTime
    );
};
#include "BrewCalculator.h"

BrewResult CoffeeCalculator::calculate(
    float dose,
    BrewStrength strength,
    DrinkType drink,
    int currentGrind,
    float actualTime
) {
    BrewResult r;

    float ratio;
    int targetTime;

    if (drink == ESPRESSO) {
        ratio = (strength == STRONG) ? 1.5f : 2.0f;
        targetTime = 27;
    } else { // LUNGO
        ratio = (strength == STRONG) ? 2.5f : 3.0f;
        targetTime = 40;
    }

    r.targetOutput = dose * ratio;
    r.targetTime = targetTime;

    if (actualTime < targetTime - 3)
        r.grindAdjustment = -1;   // too fast → grind finer
    else if (actualTime > targetTime + 3)
        r.grindAdjustment = +1;   // too slow → grind coarser
    else
        r.grindAdjustment = 0;    // good shot

    return r;
}

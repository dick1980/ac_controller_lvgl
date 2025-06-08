#ifndef AC_UNITS_CONFIG_H
#define AC_UNITS_CONFIG_H

#include "../src/ac_controller_lvgl.h"

// Define all AC units based on the current configuration
ACUnit acUnits[] = {
  {
    "Centrale ruimte",
    "ac_grote_ruimte_1",
    22.0, false, 0, 0, 0, 22.0, 22.0
  },
  {
    "Bestuurskamer",
    "ac_bestuurskamer",
    22.0, false, 0, 0, 0, 22.0, 22.0
  },
  {
    "Vergaderzaal boven",
    "ac_vergaderzaal_boven",
    22.0, false, 0, 0, 0, 22.0, 22.0
  },
  {
    "EHBO",
    "ac_ehbo",
    22.0, false, 0, 0, 0, 22.0, 22.0
  },
  {
    "Kleedkamer dames",
    "ac_kleedkamer_dames",
    22.0, false, 0, 0, 0, 22.0, 22.0
  },
  {
    "Kleedkamer dames gasten BSO",
    "ac_kleedkamer_dames_gasten_bso",
    22.0, false, 0, 0, 0, 22.0, 22.0
  },
  {
    "Kleedkamer heren",
    "ac_kleedkamer_heren",
    22.0, false, 0, 0, 0, 22.0, 22.0
  },
  {
    "Kleedkamer heren gasten",
    "ac_kleedkamer_heren_gasten",
    22.0, false, 0, 0, 0, 22.0, 22.0
  },
  {
    "Scheidsrechters",
    "ac_scheidsrechters",
    22.0, false, 0, 0, 0, 22.0, 22.0
  },
  {
    "Materiaalhok binnen",
    "ac_materiaalhok_binnen",
    22.0, false, 0, 0, 0, 22.0, 22.0
  },
  {
    "Materiaalhok buiten",
    "ac_materiaalhok_buiten",
    22.0, false, 0, 0, 0, 22.0, 22.0
  }
};

const int numUnits = sizeof(acUnits) / sizeof(acUnits[0]);

// Mode names - Order must match YAML hvac_mode_register values:
// 0=COOL, 1=HEAT, 2=FAN_ONLY, 3=AUTO, 4=DRY
const char* modeNames[] = {"Koelen", "Verwarmen", "Ventilatie", "Auto", "Drogen"};
// English mode names for MQTT communication
const char* modeNamesEN[] = {"COOL", "HEAT", "FAN", "AUTO", "DRY"};

// Fan speed names - Order must match YAML fan_mode_register values:
// 0=LOW, 1=MEDIUM, 2=HIGH, 3=POWERFUL  
const char* fanNames[] = {"Laag", "Gemiddeld", "Hoog", "Krachtig"};
// English fan names for MQTT communication
const char* fanNamesEN[] = {"LOW", "MEDIUM", "HIGH", "POWERFUL"};

// Swing mode names - Order must match YAML swing_mode values:
// 0=swing, 1=position_1, 2=position_2, 3=position_3, 4=position_4
const char* swingNames[] = {"Swing", "Positie 1", "Positie 2", "Positie 3", "Positie 4"};
// English swing names for MQTT communication
const char* swingNamesEN[] = {"swing", "position_1", "position_2", "position_3", "position_4"};

#endif // AC_UNITS_CONFIG_H
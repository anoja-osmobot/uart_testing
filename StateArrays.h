// Node State Array
// Decimal indices into State Array
#define _FIRMVERS 0 // 2 bytes
#define _HARDVERS 2 // 2 bytes
#define _NODEID 4 // 3 bytes
#define _NODEADD 7 // 1 byte
#define _SLIDEID 8 // 4 bytes
#define _ADCAVG 12 // 1 byte
#define _LSAVG 13 // 1 byte
#define _AMBCORRECT 14 // 1 byte
#define _LSINTSTEPS 15 // 8 bytes
#define _LSGAINS 23 // 8 bytes
#define PERSISTLEN 31 // The first byte after the power-cycle-proof variables

#define _UPTIME PERSISTLEN + 0 // 4 bytes
#define _ADC0 PERSISTLEN + 4 // 2 bytes
#define _ADC1 PERSISTLEN + 6 // 2 bytes
#define _ADC2 PERSISTLEN + 8 // 2 bytes
#define _ADC3 PERSISTLEN + 10 // 2 bytes
#define _ADC4 PERSISTLEN + 12 // 2 bytes
#define _ADC5 PERSISTLEN + 14 // 2 bytes
#define _ADC6 PERSISTLEN + 16 // 2 bytes
#define _ADC7 PERSISTLEN + 18 // 2 bytes
#define _LS0 PERSISTLEN + 20 // 8 bytes: CCRRGGBB
#define _LS1 PERSISTLEN + 28 // 8 bytes: CCRRGGBB
#define _LS2 PERSISTLEN + 36 // 8 bytes: CCRRGGBB
#define _LS3 PERSISTLEN + 42 // 8 bytes: CCRRGGBB
#define _LS4 PERSISTLEN + 50 // 8 bytes: CCRRGGBB
#define _LS5 PERSISTLEN + 58 // 8 bytes: CCRRGGBB
#define _LS6 PERSISTLEN + 64 // 8 bytes: CCRRGGBB
#define _LS7 PERSISTLEN + 72 // 8 bytes: CCRRGGBB
#define VARIABLELEN 80 // 31+80 = 111 Must be less than STATELEN
#define STATELEN 128

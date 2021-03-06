#ifndef __STATE_HELPER__
#define __STATE_HELPER__

typedef enum { G_OFF = 0, ON = 1 } GasInjection;
typedef enum { V_OFF = 0, VL1 = 1, VL2 = 2 } Ventilation;
typedef enum { A_OFF = 0, AL1 = 1, AL2 = 2, AL3 = 3 } Aeration;
typedef enum { AG_OFF = 0, AGXL = 1, AGXM = 2, AGXH = 3 } AlarmLevel;


static const char *GazInjectionStrings[] = {"AIG","IG"};
static const char *VentilationStrings[] = {"VN","VL1","VL2"};
static const char *AerationStrings[] = {"AN","AL1","AL2","AL3"};
static const char *AlarmStrings[] = {"","L","M","H"};

extern struct ControlModel
{
    GasInjection GasInjection;
    Ventilation ventilation;
    Aeration aeration;
    AlarmLevel alarm;
    int min;
    int max;
};

struct ControlModel GetGazControlState(int gasValue);

Ventilation GetGlobalVentilation( struct ControlModel gas1, struct ControlModel gas2, struct ControlModel gas3);
Aeration GetGlobalAertation( struct ControlModel gas1, struct ControlModel gas2, struct ControlModel gas3);

#endif
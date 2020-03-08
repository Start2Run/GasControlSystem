#ifndef __STATE_HELPER__
#define __STATE_HELPER__

typedef enum { G_OFF = 0, ON = 1 } GasAnulation;
typedef enum { V_OFF = 0, VL1 = 1, VL2 = 2 } Ventilation;
typedef enum { A_OFF = 0, AL1 = 1, AL2 = 2, AL3 = 3 } Aeration;

extern struct ControlModel
{
    GasAnulation gasAnulation;
    Ventilation ventilation;
    Aeration aeration;
    int min;
    int max;
};

struct ControlModel GetGazControlState(int gasValue);

Ventilation GetGlobalVentilation( struct ControlModel gas1, struct ControlModel gas2, struct ControlModel gas3);
Aeration GetGlobalAertation( struct ControlModel gas1, struct ControlModel gas2, struct ControlModel gas3);

#endif
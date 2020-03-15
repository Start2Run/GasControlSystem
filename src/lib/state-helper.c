#include <stdio.h>
#include "state-helper.h"

#define controlStates_Size = 24;

static struct ControlModel controlModelArray[24] =
{
    {G_OFF,V_OFF,A_OFF,AG_OFF,0,0},
    {G_OFF,V_OFF,AL1,AG_OFF,1,5},
    {G_OFF,V_OFF,AL2,AGXL,6,8},
    {G_OFF,V_OFF,AL3,AGXL,9,12},
    {G_OFF,VL1,A_OFF,AGXL,13,16},
    {G_OFF,VL1,AL1,AGXL,17,20},
    {G_OFF,VL1,AL2,AGXM,21,24},
    {G_OFF,VL1,AL3,AGXM,25,28},
    {G_OFF,VL2,A_OFF,AGXM,29,32},
    {G_OFF,VL2,AL1,AGXM,33,36},
    {G_OFF,VL2,AL2,AGXM,37,40},
    {G_OFF,VL2,AL3,AGXM,41,44},
    {ON,V_OFF,A_OFF,AGXM,45,48},
    {ON,V_OFF,AL1,AGXM,49,50},
    {ON,V_OFF,AL2,AGXH,51,56},
    {ON,V_OFF,AL3,AGXH,57,60},
    {ON,VL1,A_OFF,AGXH,61,64},
    {ON,VL1,AL1,AGXH,65,68},
    {ON,VL1,AL2,AGXH,69,72},
    {ON,VL1,AL3,AGXH,73,76},
    {ON,VL2,A_OFF,AGXH,77,80},
    {ON,VL2,AL1,AGXH,81,84},
    {ON,VL2,AL2,AGXH,85,88},
    {ON,VL2,AL3,AGXH,89,100},
};

struct ControlModel GetGazControlState(int gasRead)
{
    for(int i=0;i<sizeof(controlModelArray);i++)
    {
        struct ControlModel controlModel = controlModelArray[i];
        if(controlModel.min <= gasRead && controlModel.max >= gasRead)
        {
            return controlModel;
        }
    }
}

//Get the biggest ventilation value between all gas control models
Ventilation GetGlobalVentilation( struct ControlModel gas1, struct ControlModel gas2, struct ControlModel gas3)
{
    if((gas1.ventilation > gas2.ventilation) && (gas1.ventilation > gas3.ventilation))
    {
        return gas1.ventilation;
    }
    else if(gas2.ventilation > gas3.ventilation)
    {
        return gas2.ventilation;
    }
    else
    {
        return gas3.ventilation;
    }
}

//Get the biggest aeration value affected by the ventilation indice for all gas control models
Aeration GetGlobalAertation( struct ControlModel gas1, struct ControlModel gas2, struct ControlModel gas3)
{
    //Example
    // Given the:
    //               Gas1 ventilation=2 aeration=1
    //               Gas2 ventilation=1 aeration=3
    //               Gas3 ventilation=0 aeration=2
    // The global ventilation should be 2 and the aeration should be 1 because is the control mode that will have the most effect on the system

    int ventilationInfluenceIndice = 3;
    int aeration1 = gas1.ventilation * ventilationInfluenceIndice + gas1.aeration;
    int aeration2 = gas2.ventilation * ventilationInfluenceIndice + gas2.aeration;
    int aeration3 = gas3.ventilation * ventilationInfluenceIndice + gas3.aeration;

    if((aeration1 > aeration2) && (aeration1 > aeration3))
    {
        return gas1.aeration;
    }
    else if(aeration2 > aeration3)
    {
        return gas2.aeration;
    }
    else
    {
        return gas3.aeration;
    }
}
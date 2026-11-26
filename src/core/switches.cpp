#include "switches.h"
#include<physics.h>
#include "util.h"

bool canUseLift1(int fromFloor, int toFloor)
{
    if((fromFloor==1 && toFloor==2)||(fromFloor==2 && toFloor==1))
    return 1;
return 0;
}
bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft)
{
bool isvalidfloor=(fromFloor==2 || fromFloor==4 || fromFloor==5)&&(toFloor==2 || toFloor==4 || toFloor==5)&&(fromFloor!=toFloor);
if(!isvalidfloor)
return 0;
if(lift2Operational)
return 1;
if(emergencyTripsLeft>0)
return 1;
return false;
}
//confusion
bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount)
{
    for(int i=0;i<inventoryCount;i++)
    {
        if(inventory[i]=='<')
        return 1;
    }
    return 0;
}
//later
bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                             bool objectives[NUM_OBJECTIVES], int& score)
{
   if(!objectives[OBJ_FUSE_COLLECTED] || objectives[OBJ_FUSE_INSTALLED])
   return 0;
if(!removeItem(inventory,inventoryCount,'*'))
return 0;
objectives[OBJ_FUSE_INSTALLED]=true;
objectives[OBJ_LIFT2_ACTIVE]=true;
applyScoreDelta(score,SCORE_INSTALL_FUSE);
return 1;
}
bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES])
{
if((!objectives[OBJ_CLUE_4])||(!objectives[OBJ_CLUE_2])||(!objectives[OBJ_CLUE_7]))
return 0;
if(stringEquals(enteredCode,"427"))
return 1;
return 0;
}
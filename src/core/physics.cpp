#include<config.h>
#include<stdlib.h>
#include<iostream>
#include "physics.h"
#include "util.h"
using namespace std;


bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType){
        if(fromF < 1 || fromR < 1 || toF < 1 || toR < 1 ||
         fromF > NUM_FLOORS || toF > NUM_FLOORS || toR > ROOMS_PER_FLOOR || fromR > ROOMS_PER_FLOOR)
                            return false;
        if(fromF == toF && fromR == toR) return false;
        if(fromF == toF){
            if(abs(fromR-toR) != 1) return false;
            int floorI = fromF -1;
            int roomI = min(fromR,toR) - 1;
            int con = horizontalConn[floorI][roomI];
            outConnType = con;
            return true;
        }
        if(fromF != toF){
            if((fromR == 1 && toR==1)){    
                if(abs(fromF - toF) == 1 && (fromF != 3 && toF !=4)){outConnType = 'S'; return true;  }// Staircases
            }
            if(abs(fromF - toF) == 1 && (fromF < 3 && toF < 3) && (fromR == 2 || fromR == 1) && (toR == 2 || toR == 1)){ outConnType = '1'; return true;}
            if(fromR == 4 && toR == 4 && toF!=3 && fromF!=3 && (fromF != 1 && toF != 1)){
                outConnType = '2';
                return true;
            }
        }
        return false;
}

bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational)
{
    switch(connType){
        case 'D':
        case 'S':
        case 'W':
        case '1':
            return true;
        case 'L':{
                bool hasitem = hasItem(inventory,inventoryCount,'<');
                if(hasitem) return true;
                else return false;
        }
        case '2':
                if(lift2Operational)
                    return hasItem(inventory,inventoryCount,'(');
                else    
                    return hasItem(inventory,inventoryCount,'$') && emergencyTripsLeft>0;
    }
    return false;
}

void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime){
    energy += deltaEnergy;
    time += deltaTime;
    energy = clampInt(energy,0,100);
    time = clampInt(time,0,100);
}
void applyScoreDelta(int& score, int deltaScore){
    score = score + deltaScore;
    if(score < 0) score = 0;
}

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol){
    for(int i = 0; i< inventoryCount; i++){
        if(inventory[i] == itemSymbol) return true;
    }
    return false;
}
bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol){
    inventory[inventoryCount++] = itemSymbol;
    return true;
}
bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol){
    bool found = false;
    for(int i = 0; i< inventoryCount; i++){
        if(inventory[i] == itemSymbol){ inventory[i] = -1; found = true;  break;}
    }
    if(found) return false;
    int insert = 0;
    for(int i = 0; i< inventoryCount; i++){
        if(inventory[i] != -1){
            inventory[insert] = inventory[i];
            insert++;
        }
    }
    inventoryCount = insert;
    return true;
}   



// Movement validation and resource adjustments without structures

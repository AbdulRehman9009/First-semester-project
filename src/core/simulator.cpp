#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include<iostream>
#include<string.h>
#include<fstream>
#include "util.h"
using namespace std;

void runSimulation(int& playerFloor, int& playerRoom,
                   int& energy, int& time, int& score,
                   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                   char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    renderAsciiBuilding(playerFloor,playerRoom);
    string command;
    while(!checkEndConditions(energy,time,objectives)){
        
        renderDashboard(playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,emergencyTripsLeft);
        cout <<"Enter command: move inspect collect use unlock room objectives load save map quit\n";
        if(!(cin >> command)){break;}
        if(command == "move"){
            int tfloor = 0, toroom = 0;
            while(1){
                cout << "Enter floor and room respectively: ";
                if((cin >> tfloor >> toroom) || ((tfloor > 0 || tfloor<6) && (toroom <0 || toroom > 5))) break; 
            }
            bool lift2operational = objectives[OBJ_LIFT2_ACTIVE] || objectives[OBJ_FUSE_INSTALLED];
            if(!handleMoveCommand(tfloor,toroom,playerFloor,playerRoom,energy,time,horizontalConn,inventory,inventoryCount,emergencyTripsLeft,lift2operational)){
                cout <<"\033[31mCant move to this floor\033[0m\n";
            }
        }
        else if(command =="inspect"){
            if(!handleInspectCommand(playerFloor,playerRoom,energy,time,roomEntities,entityCounts[playerFloor-1][playerRoom-1],objectives,score)){
                cout <<"Something went wrong\n";
            }
        }
        else if(command == "collect"){
            char symbol;
            while(1){
                if(cin >> symbol) break;
            }
            if(!handleCollectCommand(symbol,playerFloor,playerRoom,energy,time,score,roomEntities,entityCounts[playerFloor-1][playerRoom-1],inventory,inventoryCount,objectives)){
                cout <<"Something went wrong\n";
            }
        }
        else if(command =="use"){
            char symbol;
            while(1) if(cin >> symbol) break;
            if(!handleUseCommand(symbol,playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives)){
                cout <<"Something went wrong";
            }
        }
        else if(command == "unlock"){
            char code[4];
            while(1){
                cout <<"Enter code: ";
                if(cin >> code) break;
            }
            if(!(handleUnlockCabinetCommand(code,playerFloor,playerRoom,objectives,score,roomEntities,entityCounts[playerFloor-1][playerRoom-1]))){
                cout <<"Something went wrong\n";
            }
        }
        else if(command == "save"){
            char filename[] = "./storage/gamefile.txt";
            ofstream file(filename);
            if(saveGameStateToFile(filename,playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,emergencyTripsLeft,roomEntities,entityCounts)){
                cout <<"Successfully saved game to file";
            }
        }
        else if(command == "load"){
            char filename[] = "./storage/gamefile.txt";
            if(loadGameStateFromFile(filename,playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,emergencyTripsLeft,roomEntities,entityCounts)){
                cout <<"Successfully loaded the game\n";
            }
        }
        else if(command == "objectives") renderObjectivesChecklist(objectives);
        else if(command == "map") renderAsciiBuilding(playerFloor,playerRoom);
        else if(command == "room") renderRoomDetails(playerFloor,playerRoom,roomEntities,entityCounts[playerFloor-1][playerRoom-1]);
        else if(command == "quit" || command == "exit") break;
        else {
            cout <<"Unknown command\n";
        }
    }
    bool isWon = objectives[OBJ_MISSION_WON];
    if(isWon){
        applyScoreDelta(score,SCORE_MISSION_VICTORY);
        cout <<"You won the game\n";
    }else {
        objectives[OBJ_MISSION_FAILED] = true;
        cout <<"You lose the game\n";
    }
    writeGameSummaryReport("./storage/report",score,energy,time,objectives,isWon);
}


bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational)
{
    char conType = '\0';
    if(!isDirectlyConnected(currentFloor,currentRoom,targetFloor,targetRoom,horizontalConn,conType)){
        return false;
    }
    if(!canTraverse(targetFloor,targetRoom,conType,inventory,inventoryCount,emergencyTripsLeft,lift2Operational)){
        return false;
    }
    int deltaEnergy = 0;
    int deltaTime = 0;
    switch (conType)
    {
    case 'D':
    case 'S':
    case 'L':
        deltaEnergy = -2;
        deltaTime = -1;
    break;
    case 'W': 
        deltaEnergy = -3;
        deltaTime = -2;
        break;
    case '1':
    case '2':
        deltaEnergy = -4;
        deltaTime = -3;
        break;    
    }
    currentFloor = targetFloor;
    currentRoom = targetRoom;
    applyResourceDelta(energy,time,deltaEnergy,deltaTime);
    if(conType == '2' && !lift2Operational) emergencyTripsLeft--;
    return true;
}
bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score)
{
    if(currentFloor < 1 || currentFloor > NUM_FLOORS || currentRoom < 1 || currentRoom > ROOMS_PER_FLOOR) return false;
    applyResourceDelta(energy,time,COST_INSPECT_ENERGY,COST_INSPECT_TIME);
    int fidx = currentFloor - 1, ridx = currentRoom -1;
    bool clue = false;
    for(int i = 0; i<entityCount; i++){
        const char symbol = roomEntities[fidx][ridx][i];
        cout << symbol << " ";
        if(symbol == '4' && !objectives[OBJ_CLUE_4]){
            objectives[OBJ_CLUE_4] = true;
            applyScoreDelta(score,SCORE_CLUE_FOUND);
            cout <<"Objective 4 completed";
            clue = true;
        }
        else if(symbol == '7' && !objectives[OBJ_CLUE_7]){
            objectives[OBJ_CLUE_7] = true;
            applyScoreDelta(score,SCORE_CLUE_FOUND);
            cout <<"Obbjectives 7 completed";
            clue = true;
        }
        else if(symbol == '2' && !objectives[OBJ_CLUE_2]){
            objectives[OBJ_CLUE_2] = true;
            applyScoreDelta(score,SCORE_CLUE_FOUND);
            cout <<"Objective 2 completed";
            clue = true;
        }
        cout <<"\n";
    }
    if(!clue) cout <<"No clue found.\n";
    return true;
}

bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES])
{
    if(currentFloor < 1 || currentFloor > NUM_FLOORS || currentRoom < 1 || currentRoom > ROOMS_PER_FLOOR) return false;
    const int size = 7;
    char arr[size] = {ENT_CAT,ENT_CLUE_2,ENT_CLUE_4,ENT_CLUE_7,ENT_CONTROL_PANEL,ENT_PASSENGER};
    for(int i = 0; i<size; i++){
        if(itemSymbol == arr[i]) return false;
    }
    if(itemSymbol == ENT_CABINET_TRAIN_KEY && !objectives[OBJ_TRAIN_KEY]){
        cout <<"The cabinet is locked unloack it with code first\n";
        return false;
    }
    int found = -1;
    for(int i = 0; i<entityCount; i++){
        char symbol = roomEntities[currentFloor-1][currentRoom-1][i];
        if(itemSymbol == symbol){ found = i;break;}
    }
    if(found == -1) return false;
    bool consume = false;


    if(itemSymbol == ENT_ENERGY_PACK || itemSymbol == ENT_OBSERVATION_TOKEN || itemSymbol == ENT_EMERGENCY_TOOLKIT) consume = true;
    if(!consume && inventoryCount >= MAX_INVENTORY_CAPACITY) return false;

    for(int i = found; i<entityCount - 1; i++){
        roomEntities[currentFloor-1][currentRoom-1][i] = roomEntities[currentFloor-1][currentRoom-1][i+1];
    }
    roomEntities[currentFloor][currentRoom][entityCount-1] = '\0';
    entityCount--;
    if(consume){
        applyResourceDelta(energy,time,COST_ITEM_PICKUP_ENERGY,COST_ITEM_PICKUP_TIME);
    switch(itemSymbol){
        case ENT_ENERGY_PACK:
            applyResourceDelta(energy,time,BONUS_ENERGY_PACK,0);
            cout <<"Collected Energy pack\n";
            return true;
        case ENT_OBSERVATION_TOKEN:
            applyResourceDelta(energy,time,0,BONUS_OBSERVATION_TOKEN);
            cout <<"Collected Observation token.\n";
            return true;
        case ENT_EMERGENCY_TOOLKIT:
            applyResourceDelta(energy,time,BONUS_EMERGENCY_TOOLKIT,0);
            cout <<"Collected Emergency toolkit\n";
            return true;
    }
    }else {
        applyResourceDelta(energy,time,COST_ITEM_PICKUP_ENERGY,COST_ITEM_PICKUP_TIME);
        inventory[inventoryCount] = itemSymbol;
        inventoryCount++;
        if(itemSymbol == ENT_MASTER_KEY){
            objectives[OBJ_MASTER_KEY] = true;
            applyScoreDelta(score,SCORE_KEY_COLLECT);
            cout <<"Collected Master key +10 points\n";
            return true;
        }else if(itemSymbol == ENT_LIFT_PASS){
            objectives[OBJ_LIFT_PASS] = true;
            applyScoreDelta(score,SCORE_LIFT_PASS);
            cout <<"Collected Lift pass +20 points\n";
            return true;
        }else if(itemSymbol == ENT_CABINET_TRAIN_KEY){
            objectives[OBJ_TRAIN_KEY] = true;
            applyScoreDelta(score,SCORE_TRAIN_KEY);
            cout <<"Collected Tran key +10 points\n";
            return true;
        }else if(itemSymbol == ENT_CAT_CARRIER){
            objectives[OBJ_CAT_CARRIER] = true;
            cout <<"Successfully collected cat carrier\n";
            return true;
        }else if(itemSymbol == ENT_FUSE){
            objectives[OBJ_FUSE_COLLECTED] = true;
            cout <<"Successfully collected fuse *.\n";
            return true;
        }
    }
    cout <<"Item not found\n";
    return false;
}


bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES])
{
    if(currentFloor <0 || currentFloor > NUM_FLOORS || currentRoom <0 || currentRoom > ROOMS_PER_FLOOR) return false;

    if(itemSymbol == ENT_CABINET_TRAIN_KEY){
        if(currentFloor != 5 || currentRoom != 1) return false;
        if(!hasItem(inventory,inventoryCount,itemSymbol) || objectives[OBJ_TRAIN_PREPARED]) return false;
        if(!objectives[OBJ_PASSENGER_RESCUED] || !objectives[OBJ_CAT_SECURED]){ cout<<"You must rescued the passenger and Cat before unlocking cabinet\n"; return false;}

        objectives[OBJ_TRAIN_PREPARED] = true;
        objectives[OBJ_MISSION_WON] =true;
        applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
        applyScoreDelta(score,SCORE_PREPARE_TRAIN);
        return true;

    }else if(itemSymbol == ENT_CAT_CARRIER){
        if(currentFloor != 3 || currentRoom != 3) return false;
        if(!hasItem(inventory,inventoryCount,itemSymbol) || objectives[OBJ_CAT_SECURED]) return false;
        removeItem(inventory,inventoryCount,itemSymbol);
        objectives[OBJ_CAT_SECURED] = true;
        applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
        applyScoreDelta(score,SCORE_SECURE_CAT);
        return true;
    }else if(itemSymbol == ENT_MEDICAL_KIT){
        if(currentFloor != 4 || currentRoom !=1) return false;
        if(!hasItem(inventory,inventoryCount,itemSymbol) || objectives[OBJ_PASSENGER_RESCUED]) return false;
        removeItem(inventory,inventoryCount,itemSymbol);
        objectives[OBJ_PASSENGER_RESCUED] = true;
        applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
        applyScoreDelta(score,SCORE_RESCUE_PASSENGER);
        return true;
    }else if(itemSymbol == ENT_FUSE){
        if(currentFloor != 3 || currentRoom !=4) return false;
        if(!hasItem(inventory,inventoryCount,itemSymbol)|| objectives[OBJ_FUSE_INSTALLED]) return false;
        if(installFuseControlPanel(inventory,inventoryCount,objectives,score)){
            applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
            cout <<"Successfully installed fuse.";
            return true;
        }
    }
    return false;
}


bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,
                                bool objectives[NUM_OBJECTIVES], int& score,
                                char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                                int& entityCount)
{
    if(currentFloor != 5 && currentRoom !=1) return false;
    if(objectives[OBJ_CABINET_UNLOCKED]) return false;
    if(!verifyCabinetCode(code,objectives)){
        cout <<"Invalid cabinet code or objectives not completed\n";
        return false;
    }
    objectives[OBJ_CABINET_UNLOCKED] = true;
    applyScoreDelta(score,SCORE_CABINET_UNLOCK);
    cout <<"";
    bool found = false;
    for(int i = 0; i<entityCount; i++){
        if(roomEntities[currentFloor-1][currentRoom-1][i] == ENT_CABINET_TRAIN_KEY) found = true;
    }
    if(!found && entityCount <MAX_ENTITIES_PER_ROOM){
        roomEntities[currentFloor-1][currentRoom-1][entityCount]=ENT_CABINET_TRAIN_KEY;
        entityCount++;
    }
    cout <<"You have successfully unlocked the cabinet code train key ^ is now accessible. +30 score points\n";
    return true;
}
bool checkEndConditions(int energy, int time, const bool objectives[NUM_OBJECTIVES]){
    if(objectives[OBJ_MISSION_WON] && objectives[OBJ_CAT_SECURED] && objectives[OBJ_FUSE_INSTALLED]
        && objectives[OBJ_TRAIN_PREPARED]) return true;
    if(energy <=0 && time<=0) return true;
    return false;
}
// Modular simulation loop using primitive variables

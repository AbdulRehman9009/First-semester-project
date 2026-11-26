#include "viewer_iface.h"
#include "util.h"
#include<fstream>
#include<iostream>
using namespace std;


void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft)
{
    cout<<"------------------------------"<<endl;
    cout<<"        GAME DASHBOARD        "<<endl;
    cout<<"------------------------------"<<endl;
    cout<<"PLAYER FLOOR:"<<playerFloor<<endl;
    cout<<"PLAYER ROOM:"<<playerRoom<<endl;
    cout<<"ENERGY"<<energy<<endl;
    cout<<"REMAINING TIME:"<<time<<endl;
    cout<<"SCORE:"<<score<<endl;
    cout<<"INVENTORY:";
    if(inventoryCount==0)
    {
        cout<<"EMPTY";
    }
else
{
    for(int i=0;i<inventoryCount;i++)
    {
        cout<<inventory[i]<<" ";
    }
}
cout<<endl;
cout<<"EMERGENCY TRIPS LEFT:"<<emergencyTripsLeft<<endl;
cout<<"-------------------------------\n";
}
void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount)
{
    cout<<"------------------------------"<<endl;
    cout<<"         ROOM DETAILS         "<<endl;
    cout<<"------------------------------"<<endl;
    cout<<"FLOOR:"<<floor+1<<endl;
    cout<<"ROOM:"<<room+1<<endl;
    cout<<"ENTITIES IN THIS ROOM:"<<endl;
    if(entityCount==0)
    cout<<"NO ENTITIES ARE IN THIS ROOM"<<endl;
else
{
    for(int i=0;i<entityCount;i++)
    {
        cout<<i+1<<") "<<roomEntities[floor][room][i]<<endl;
    }
}
cout<<"------------------------------";
}


void renderAsciiBuilding(int playerFloor, int playerRoom){
    const char filename[] = "./data/building.txt";
    ifstream file(filename);
    if(!file || file.fail()){
        cout <<"Cant open file";
        return;
    }
    const int targetSize = 5;
    char* targetStr = new char[targetSize + 1] {'F', static_cast<char>('0' + playerFloor), '-', 'R', static_cast<char>('0' + playerRoom), '\0'};
    char line[MAX_LINE_LEN] = {0};
    int lines = 0;
    while(file.getline(line,MAX_LINE_LEN) && lines < 17){
        int colorS = -1;
        for(int i = 0; i<MAX_LINE_LEN-targetSize; i++){
            char is[targetSize+1];
            for(int k=0; k<targetSize;k++) is[k] = line[k+i];
            is[targetSize] = '\0';
            if(stringEquals(is,targetStr)){
                colorS = i+targetSize + 1;
                line[colorS] = '[';
                line[colorS+1] = 'P';
                line[colorS+2] = ']';
                break;
            }
        }
        for(int i = 0; line[i] != '\0'; i++){
            if(i == colorS){
                cout << "\033[1;32m";
            }
            cout << line[i];
            if(colorS != -1 && i == colorS + 2){
                cout << "\033[0m";
            }
        }
        cout <<"\n";
        lines++;
    }
    delete[] targetStr;
    targetStr = nullptr;
}
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]){
    const char* objnames[NUM_OBJECTIVES] = {"OBJ_MASTER_KEY","OBJ_CLUE_4", "OBJ_CLUE_2", "OBJ_CLUE_7", "OBJ_CABINET_UNLOCKED", "OBJ_TRAIN_KEY",
    "OBJ_FUSE_COLLECTED", "OBJ_FUSE_INSTALLED", "OBJ_LIFT_PASS", "OBJ_LIFT2_ACTIVE", "OBJ_PASSENGER_RESCUED", "OBJ_CAT_CARRIER", "OBJ_CAT_SECURED", "OBJ_TRAIN_PREPARED",
    "OBJ_MISSION_WON","OBJ_MISSION_FAILED" };
    for(int i = 0; i<NUM_OBJECTIVES; i++){
        cout << objnames[i] << ": " << (objectives[i]?"\033[1;32mCompleted\033[0m":"\033[31mNot completed\033[0m") <<"\n";
    }
}

// Terminal UI and dashboard rendering using primitive parameters

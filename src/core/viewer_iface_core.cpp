#include "viewer_iface.h"
#include "util.h"
#include<fstream>
#include<iostream>
using namespace std;







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
        for(int i = 0; i<MAX_LINE_LEN-targetSize; i++){
            char is[targetSize+1];
            for(int k=0; k<targetSize;k++) is[k] = line[k+i];
            is[targetSize] = '\0';
            if(stringEquals(is,targetStr)){
                line[i+targetSize+1] = '[';
                line[i+targetSize+2] = 'P';
                line[i+targetSize+3] = ']';
                break;
            }
        }
        cout << line <<"\n";
        lines++;
    }
}
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]){
    const char* objnames[NUM_OBJECTIVES] = {"OBJ_MASTER_KEY","OBJ_CLUE_4", "OBJ_CLUE_2", "OBJ_CLUE_7", "OBJ_CABINET_UNLOCKED", "OBJ_TRAIN_KEY",
    "OBJ_FUSE_COLLECTED", "OBJ_FUSE_INSTALLED", "OBJ_LIFT_PASS", "OBJ_LIFT2_ACTIVE", "OBJ_PASSENGER_RESCUED", "OBJ_CAT_CARRIER", "OBJ_CAT_SECURED", "OBJ_TRAIN_PREPARED",
    "OBJ_MISSION_WON","OBJ_MISSION_FAILED" };
    for(int i = 0; i<NUM_OBJECTIVES; i++){
        cout << objnames[i] << ": " << (objectives[i]?"Completed":"Not completed") <<"\n";
    }
}

// Terminal UI and dashboard rendering using primitive parameters

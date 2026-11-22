#include "level.h"
#include "util.h"
#include<iostream>
#include<fstream>
#include<string>

using namespace std;
// Modular implementation using primitive arrays for map parsing and verification


 bool loadBuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    ifstream mfile(filename);  
    if(!mfile || mfile.fail()){
        return false;
    }
    for(int f = 0; f < NUM_FLOORS; f++){
        for(int c = 0; c < ROOMS_PER_FLOOR - 1; c++){
            horizontalConn[f][c] = '#';
        }
    }
    string line;
    int flooridx = -1;
    int floorFound = 0;
    while(getline(mfile,line)){
        if(line.find("D =")!= string::npos){
            break;
        }
        int rpos = line.find("R1");
        if(rpos != string::npos && rpos >=2){
            int floor = line[rpos - 2] - '1';
            if(floor < 0 || floor >= NUM_FLOORS){
                flooridx = -1;
                continue;
            }
            flooridx = floor;
            floorFound++;
        const char* rstr[3] = {"-R2", "-R3","-R4"};
        for(int i = 0; i<ROOMS_PER_FLOOR - 1; i++){
            int fidx = line.find(rstr[i]);
            if(fidx != string::npos){
                const char con = line[fidx-4];
                if(con == 'D' || con=='W' || con =='L' || con=='#'){
                    horizontalConn[flooridx][i] = con;
                }
            }
        }
        continue;
    }
        if(flooridx != -1){
            int totalPipes = 0;
            for(int i = 0; i<line.length(); i++){
                if(line[i] == '|') totalPipes++;;
            }
            const int size = 18;
            char entityArray[size] = {'<',')','\\','4','*','~',';','(','!','%','$','2','7','^','@','&','-'};
            for(int i = 0; i<line.length(); i++){
                char curentChar = line[i];
                bool isEntity = false;
                for(int j = 0; j<size; j++){
                    if(curentChar == entityArray[j]){ isEntity = true; break;}
                }
                if(isEntity){
                    int pipeCount = 0;
                    for(int k= 0; k<i; k++){
                        if(line[k] == '|') pipeCount++;
                    }
                    int o = (totalPipes >= 6)? 2:1;
                    int room = pipeCount - o;
                    if(room>=0 && room < ROOMS_PER_FLOOR){
                        int s = entityCounts[flooridx][room];
                        if(s<MAX_ENTITIES_PER_ROOM){
                            roomEntities[flooridx][room][s] = curentChar;
                            entityCounts[flooridx][room]++;
                        }
                    }
                }
            }
        }
        flooridx = -1;
    }
    return floorFound == NUM_FLOORS;
}



bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft){
    ifstream file(filename);
    if(!file) return false;                    
    inventoryCount = 0;

    for(int i = 0; i< NUM_OBJECTIVES; i++)
            objectives[i] = false;
    for(int i =  0; i<MAX_INVENTORY_CAPACITY; ++i)
            inventory[i] = ENT_NONE;

    string line;
    while(getline(file, line)){
        int colon = line.find(':');
        if(colon == string::npos) continue;
        char key[128];
        int i;
        for(i=0; i<colon && i <127; i++)
            key[i] = line[i];
        key[i] = '\0';
        trimWhitespace(key);

        char value[128];
        int vidx = 0;
        for(int j = colon+1; j<line.length(); j++, vidx++){
            value[vidx] = line[j];
        }
        value[vidx] = '\0';
        trimWhitespace(value);

        if(stringEquals(key, "INITIAL_ENERGY")){
            energy = stoi(value);
        }else if(stringEquals(key, "INITIAL_TIME")){
            time = stoi(value);
        }else if(stringEquals(key,"INITIAL_SCORE")){
            score = stoi(value);
        }else if(stringEquals(key,"START_ROOM") && stringEquals(value,"PLAYER_SELECTS")){
            playerFloor = 1;
            playerRoom = 1;
        }else if(stringEquals(key,"PASSENGER_RESCUED")){
            emergencyTripsLeft = 0;
        }
    }

    return energy >=0 && time >=0 && score >=0 && playerFloor>=1 && playerRoom>=1 && emergencyTripsLeft>=0;
}


bool validateStartingRoom(int startFloor, int startRoom,
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])
{
    if(startFloor != 1 || startRoom != 1) return false;
    return true;
}

const char* getEntityName(char entitySymbol){
    switch(entitySymbol){
        case '<': return "Master Key";
        case ')': return "Energy Packs";
        case '\\': return "Emergency Toolkit";
        case '4': return "Clue digit 4";
        case '*': return "Fuse";
        case '~': return "Cat";
        case ';': return "Control Panel";
        case '(': return "Lift Pass";
        case '!': return "Passenger";
        case '%': return "Medical Kit";
        case '$': return "Emergency Access Card";
        case '2': return "Clue digit 2";
        case '7': return "Clue Digit 7";
        case '^': return "Cabinet";
        case '@': return "Cat Carrier";
        case '&': return "Observation Token";
        default: return "Unknown";
    }
}


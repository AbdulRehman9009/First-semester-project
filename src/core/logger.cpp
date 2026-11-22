#include "logger.h"
#include "util.h"
#include "fstream"
#include<iostream>
using namespace std;


bool saveGameStateToFile(const char* filename,
                         int playerFloor, int playerRoom,
                         int energy, int time, int score,
                         const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                         const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft,
                         const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                         const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
                         {
if(filename==nullptr)
return 0;
if(inventoryCount<0 || inventoryCount>MAX_INVENTORY_CAPACITY)
return 0;
ofstream fout(filename);
if(!fout)
{
    cout<<"unable to create file!";
    return 0;
}
fout<<playerFloor<<endl;
fout<<playerRoom<<endl;
fout<<energy<<endl;
fout<<score<<endl;
fout<<inventoryCount<<endl;
for(int i=0;i< inventoryCount;i++)
{
    fout<<inventory[i]<<endl;
}
for(int i=0;i<NUM_OBJECTIVES;i++)
{
    fout<<objectives[i]<<endl;
}
fout<<emergencyTripsLeft<<endl;
for(int f=0;f<NUM_FLOORS;f++)
{
for(int r=0;r<ROOMS_PER_FLOOR;r++)
{
    if(entityCounts[f][r]<0 || entityCounts[f][r]>MAX_ENTITIES_PER_ROOM)
    {
        fout.close();
        return 0;
    }
    fout<<entityCounts[f][r]<<endl;
    for(int e=0;e<entityCounts[f][r];e++)
{
    fout<<roomEntities[f][r][e]<<endl;
}
}
}
fout.close();
return 1;
                         }




bool loadGameStateFromFile(const char* filename,
                           int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    ifstream fin(filename);
    if(!fin)
    {
        cout<<"unable to open file";
        return 0;
    }
    fin>>playerFloor>>playerRoom;
    fin>>energy>>time>>score;
    fin>>inventoryCount;
    for(int i=0;i<inventoryCount;i++)
    {
        fin>>inventory[i];
    }
    for(int o=0;o<NUM_OBJECTIVES;o++)
    {
        fin>>objectives[o];
    }
    fin>>emergencyTripsLeft;
    for(int i=0;i<NUM_FLOORS;i++)
    {
        for(int j=0;j<ROOMS_PER_FLOOR;j++)
        {
            fin>>entityCounts[i][j];
            for(int k=0;k<entityCounts[i][j];k++)
            {
                fin>>roomEntities[i][j][k];
            }
        }
    }
    fin.close();
    return 1;
}




void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory)
{
 ofstream fout(filename);
 if(!fout)
 {
    cout<<"unable to create report file!";
    return;
 }   
 fout<<"========== GAME SUMMARY REPORT =========="<<endl;
 fout<<"Final score"<<score<<endl;
 fout<<"Remaining Energy"<<remainingEnergy<<endl;
 fout<<"Remaining Time"<<remainingTime<<endl;
 fout<<"Objectives Status"<<endl;
 for(int i=0;i<NUM_OBJECTIVES;i++)
 {
  fout<<"Objective"<<i+1<<":";
  if(objectives[i])
  fout<<"Completed"<<endl;
else 
  fout<<"Incomplete"<<endl;
 }
fout<<"Game Result:";
if(victory)
    fout<<"Victory"<<endl;
else 
    fout<<"Defeat"<<endl;
fout.close();
}          
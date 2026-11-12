#ifndef VIEWER_IFACE_H
#define VIEWER_IFACE_H

#include "state.h"

// Terminal Visualization using primitive variables
void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft);

void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount);

void renderAsciiBuilding(int playerFloor, int playerRoom);
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]);

#endif // VIEWER_IFACE_H

#pragma once
#include <map>
#include <set>
#include <unordered_set>

#include "dungeon.h"
#include <unordered_map>
#include <stdexcept>

struct RoomInfo{
    bool done = false;
    bool visited = false;
    bool keys_collected = false;
};

Room* FindFinalRoom(Room* starting_room) {
   std::unordered_map<Room*, RoomInfo> info;
   std::unordered_set<std::string> keys;keys.reserve(1000);
   std::vector<Room*> queue; queue.reserve(1000);
   queue.push_back(starting_room);if(starting_room->IsFinal()){return starting_room;}
   while(!queue.empty()){
        Room* cur = queue.back();
        if(info[cur].keys_collected == false)
        {
            auto num_keys = cur->NumKeys();
            for(size_t i = 0; i < num_keys;i++){
                keys.insert(cur->GetKey(i));
            }
            info[cur].keys_collected = true;
        }
        bool all_doors_opened = true;
        auto num_doors = cur->NumDoors();
        for(size_t i = 0; i < num_doors; i++){
            auto door = cur->GetDoor(i);
            if(door->IsOpen() && info[door->GoThrough()].visited == false){
                queue.push_back(door->GoThrough());
                info[door->GoThrough()].visited = true;
            }
            else{
                bool opened = false;
                for(auto my_key: keys){
                    opened = door->TryOpen(my_key);
                    if(opened){
                        auto next_door =  door->GoThrough();
                        if(next_door->IsFinal()){
                            return next_door;
                        }
                        if(info.count(next_door) && info[next_door].visited == false)
                        {
                            queue.push_back(next_door);break;
                            info[next_door].visited = true;
                        }
                    }
                }
                if(opened == false){
                    all_doors_opened = false;
                }
            }
        }
        if(all_doors_opened == true){
            info[cur].done = true;
        }
   }
   return nullptr;
}

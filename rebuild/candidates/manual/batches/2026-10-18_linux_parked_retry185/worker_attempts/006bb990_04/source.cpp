// CGameTimeManager::Update(float) PDB006F71F0; no repository class header.
// Leaf: Update
#include "rebuild_abi.h"
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld
// Actual CWorld header retained; this method only carries its opaque pointer and constructor reference.
struct CWorld;struct CMainGameComponent;
struct StopTimeSet {void*Head;unsigned long Size;unsigned long Allocator;}; //native tree12 vs donor16
struct CGameTimeManager {CGameTimeManager(CMainGameComponent&,CWorld&); // actual PDB006F7540
 CWorld*World;CMainGameComponent*GameComponent;float Time,PrevTime,TimeStep;long DayCount;bool UsingVillageTime;float VillageTime;bool VillageDay,FastForwardingToTime;float TimeFastForwardingTo,SecondsLeftToFastForwardTime;StopTimeSet StopTimeIndexes;long NextStopTimeIndex;
 bool IsTimeBetween(float,float,float)const; //006BB5B0/PDB006F6E20
 void Update(float);
};
void CGameTimeManager::Update(float delta){
 PrevTime=Time;
 if(FastForwardingToTime){
 float amount=Time<=TimeFastForwardingTo?TimeFastForwardingTo-Time:1.0f-Time+TimeFastForwardingTo;
 if(delta>SecondsLeftToFastForwardTime){Time+=amount;SecondsLeftToFastForwardTime=0;FastForwardingToTime=false;}
 else{Time+=(delta/SecondsLeftToFastForwardTime)*amount;SecondsLeftToFastForwardTime-=delta;}
 }else if(StopTimeIndexes.Size==0){
 if(UsingVillageTime){float advance=delta*TimeStep;VillageTime+=advance;
 if(VillageDay){if(IsTimeBetween(Time,0.375f,0.708333313f))Time+=advance;}
 else if(IsTimeBetween(Time,0.8125f,0.229166672f))Time+=advance;
 }else{Time+=delta*TimeStep;VillageTime=Time;}
 }
 if(Time>=1.0f){Time-=static_cast<long>(Time);++DayCount;}
 if(VillageTime>=1.0f)VillageTime-=static_cast<long>(VillageTime);
}

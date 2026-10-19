// CHeroBoastsScript::BoastHobbeContestKillMoreHobbes(CCharString) -- wait for quest/boast outcome
// Native 00ce5620; callee00CB7940 termination query and99EAE0 by-value string destructor.
// Leaf: BoastHobbeContestKillMoreHobbes
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
// CHeroBoastsScript and CQ_SunnyvaleMasterData have no generated engine headers.
// Native CLevelScriptBase Game+40/MasterScriptData+44; donor Game+54/Master+58.
// Named master flags have identical native/PDB offsets; untouched ranges remain opaque.
struct SunnyvaleMasterData {
 unsigned char BeforeBounty[0xd9]; bool BountyHuntDecapitation;
 unsigned char BeforeHobbe[0x33]; bool HobbeCavePerpertratorKilled;
 unsigned char BeforeContest[0xd]; bool HobbeContestKillMoreHobbes; bool GraveyardKillAllInnocents;
 unsigned char BeforeFireHeart[0x12]; bool FireHeartFreeAllPrisoners;
};
struct GameMethods:CGameScriptInterface {};
union GameSlot {
 void* Native;
 void(GameMethods::*NewScriptFrame)() const;
 bool(GameMethods::*QuestMessage)(const CCharString&) const;
 void(GameMethods::*SetBoast)(long,const CCharString&) const;
};
// Native vtable01260F0C:7=6E7410,28=893730,29=8937D0,305=8915B0,306=8915E0.
// PMF union restores flattened header's real virtual ABI without fabricated virtual slots.
struct CHeroBoastsScript {
 unsigned char ScriptBaseState[0x40]; GameMethods* Game; SunnyvaleMasterData* MasterScriptData;
 bool IsActiveThreadTerminating()const; //00CB7940: active thread+2C, thread terminating byte+5
 void BoastHobbeContestKillMoreHobbes(CCharString quest);
 __forceinline bool Failed(const CCharString& q){return (Game->*reinterpret_cast<GameSlot*>(Game->__vftable)[29].QuestMessage)(q);}
 __forceinline bool Completed(const CCharString& q){return (Game->*reinterpret_cast<GameSlot*>(Game->__vftable)[28].QuestMessage)(q);}
 __forceinline void Frame(){(Game->*reinterpret_cast<GameSlot*>(Game->__vftable)[7].NewScriptFrame)();}
 __forceinline void Success(long id,const CCharString& q){(Game->*reinterpret_cast<GameSlot*>(Game->__vftable)[306].SetBoast)(id,q);}
 __forceinline void Failure(long id,const CCharString& q){(Game->*reinterpret_cast<GameSlot*>(Game->__vftable)[305].SetBoast)(id,q);}
};
FABLE_STATIC_ASSERT(sizeof(GameSlot)==4);
FABLE_STATIC_ASSERT(offsetof(CHeroBoastsScript,Game)==0x40);
FABLE_STATIC_ASSERT(offsetof(CHeroBoastsScript,MasterScriptData)==0x44);
FABLE_STATIC_ASSERT(offsetof(SunnyvaleMasterData,BountyHuntDecapitation)==0xd9);
FABLE_STATIC_ASSERT(offsetof(SunnyvaleMasterData,HobbeCavePerpertratorKilled)==0x10d);
FABLE_STATIC_ASSERT(offsetof(SunnyvaleMasterData,HobbeContestKillMoreHobbes)==0x11b);
FABLE_STATIC_ASSERT(offsetof(SunnyvaleMasterData,GraveyardKillAllInnocents)==0x11c);
FABLE_STATIC_ASSERT(offsetof(SunnyvaleMasterData,FireHeartFreeAllPrisoners)==0x12f);
void CHeroBoastsScript::BoastHobbeContestKillMoreHobbes(CCharString quest){
 while(!IsActiveThreadTerminating()){
  if(MasterScriptData->HobbeContestKillMoreHobbes){if(!IsActiveThreadTerminating())Success(28,quest);return;}
  if(Failed(quest)||Completed(quest)){if(!IsActiveThreadTerminating())Failure(28,quest);return;}
  Frame();
 }
}
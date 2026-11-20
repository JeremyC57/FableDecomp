// CTCCreatureModeManager::CModeBase::GetNoFramesBeforePlayingIdleAnims(NCreatureMode::EMode mode) -- Select the idle delay and apply active bullet-time scaling.
// Leaf: ?GetNoFramesBeforePlayingIdleAnims@CModeBase@CTCCreatureModeManager@@IAEJW4EMode@NCreatureMode@@@Z
// Naming wave 18: the global stand-in g_pThingManager (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
// NF167 (2026-11-20, q4152): the default delay 0x013BAFE8 respelt from the invented `?g_DefaultFrames_013bafe8@@3JA` to the private static
//   ?NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS@CModeBase@CTCCreatureModeManager@@0JB (Ego_d.pdb PUB32 + Ego_r / FableWin dynamic initializer
//   names = retail 0x0121F7E0), declared in place on the row's own nested CModeBase model as the landed twin 0x0073A6F0 does.
// Identity: original PDB protected method and own stand/sit callers; default013bafe8, Berserk0078c000, IsAffected004c9920, multiplier004c5fa0.
#define CBulletTimeManager CBulletTimeManager_HeaderLayout
#include "engine/CBulletTimeManager.h"
#undef CBulletTimeManager
#include "engine/CWorld.h"
#ifndef _MSC_VER
#define __fastcall
#endif
class CBulletTimeManager:public CBulletTimeManager_HeaderLayout {public:
 bool IsBulletTimeActive()const{return Override || ((const unsigned int*)BulletTimeInfoMap)[1]!=0;}
 float GetBulletTimeSlowdownMultiplier()const;
};
// Layout: no active CThingManager header; own global getter chains prove World+0x24. No other manager field is accessed.
class CThingManager {public:unsigned char Prefix[0x24];CWorld* World;};
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
// Layout: CTCBerserk supplies only a static method name; no object instance is accessed.
class CTCBerserk {public:static long __fastcall GetNumFramesBeforeCrazyIdle();};
namespace NCreatureMode {enum EMode {DescriptiveModeZero=0};}
// Layout: original nested CModeBase ParentThing+4, as PDB and retail agree; no active nested base header exists. Other members are not accessed.
class CTCCreatureModeManager {public:class CModeBase {public:void* TypeTable;CThing* ParentThing;
CThing& GetParentThing()const{return *ParentThing;}
protected:long GetNoFramesBeforePlayingIdleAnims(NCreatureMode::EMode);friend struct TestAccess;
private:static const long NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS;   // 0x013BAFE8 ?NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS@CModeBase@CTCCreatureModeManager@@0JB
};};
long CTCCreatureModeManager::CModeBase::GetNoFramesBeforePlayingIdleAnims(NCreatureMode::EMode mode){
 long frames=NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS;
 if(mode==(NCreatureMode::EMode)0x24)frames=CTCBerserk::GetNumFramesBeforeCrazyIdle();
 if(CThing::GetPCurrentThingManager()->World->PBulletTimeManager_ptr->IsBulletTimeActive() && GetParentThing().IsAffectedByBulletTime())
  return (long)(CThing::GetPCurrentThingManager()->World->PBulletTimeManager_ptr->GetBulletTimeSlowdownMultiplier()*frames);
 return frames;
}
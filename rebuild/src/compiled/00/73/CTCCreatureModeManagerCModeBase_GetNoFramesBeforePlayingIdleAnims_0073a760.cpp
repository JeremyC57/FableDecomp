// CTCCreatureModeManager::CModeBase::GetNoFramesBeforePlayingIdleAnims(NCreatureMode::EMode mode) -- Select the idle delay and apply active bullet-time scaling.
// Leaf: ?GetNoFramesBeforePlayingIdleAnims@CModeBase@CTCCreatureModeManager@@IAEJW4EMode@NCreatureMode@@@Z
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
extern CThingManager* g_pThingManager;
class CThing {public:bool IsAffectedByBulletTime()const;};
// Layout: CTCBerserk supplies only a static method name; no object instance is accessed.
class CTCBerserk {public:static long __fastcall GetNumFramesBeforeCrazyIdle();};
namespace NCreatureMode {enum EMode {DescriptiveModeZero=0};}
// Layout: original nested CModeBase ParentThing+4, as PDB and retail agree; no active nested base header exists. Other members are not accessed.
class CTCCreatureModeManager {public:class CModeBase {public:void* TypeTable;CThing* ParentThing;
CThing& GetParentThing()const{return *ParentThing;}
protected:long GetNoFramesBeforePlayingIdleAnims(NCreatureMode::EMode);friend struct TestAccess;
};};
// Label: default timing global name remains descriptive; retail address 013bafe8.
extern long g_DefaultFrames_013bafe8;
long CTCCreatureModeManager::CModeBase::GetNoFramesBeforePlayingIdleAnims(NCreatureMode::EMode mode){
 long frames=g_DefaultFrames_013bafe8;
 if(mode==(NCreatureMode::EMode)0x24)frames=CTCBerserk::GetNumFramesBeforeCrazyIdle();
 if(g_pThingManager->World->PBulletTimeManager_ptr->IsBulletTimeActive() && GetParentThing().IsAffectedByBulletTime())
  return (long)(g_pThingManager->World->PBulletTimeManager_ptr->GetBulletTimeSlowdownMultiplier()*frames);
 return frames;
}
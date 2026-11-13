// Leaf: ?SummonPillars@CAIStateGroup_JackOfBladesBase@@IAEXXZ
// CAIStateGroup_JackOfBladesBase::SummonPillars() -- raise Jack's arena pillars one at a time: while pillars remain and the
//   action loop is not told to terminate, every TimeBetweenPillars (battle sub-def, frames * ConstantFPS compared with it) take
//   the next live pillar, enable its CTCJackPillar component, and (while the arena centre is alive) turn it to face the centre and
//   drop it onto the ground height below its position; every pass plays a "SUMMON_LOOP" combat animation on Jack.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00973DC0, 833 bytes, __fastcall plain `ret`, no EH frame (Jack TU), /O2.
// Callees: CDefClassBase::GetSubDef<CJackOfBladesBattleDef> 0x008C33E0, CAIStateGroupBase::GetWorld 0x008FCFC0,
//   CWorld::GetFrame 0x0049D870, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<..>::LowerBound 0x0040F020 (expanded
//   GetTC<CTCJackPillar>), CTCJackPillar::Enable 0x007317D0, CThing::GetPos 0x004C73D0, _CIfmod 0x00BFEB28 (GetAngleXY wrap),
//   CWorld::PeekWorldMap (vslot 13), CWorldMap::GetHeightZBelowPos 0x0063E480, CTCPhysicsBase::SetFacingAngleXY (vslot 66)
//   / ResetPosition (vslot 31, +0x7c), CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, CCreatureAction_PlayCombatAnimation ctor
//   0x008B7FF0, CAIStateGroupBase::AIAction_PerformAction 0x0097D8B0, ~CCombatActionBase 0x0062E900; the def's slot-1 Delete.
//   Data: CMainGameComponent::ConstantFPS 0x01375550, GFastOneOverSqrtTable 0x013CE558.
// Shape: the GetSubDef<T> body is visible out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE), which gives the in-place
//   `dec [ecx+4]` release and drops the null test on the loop exit; the fast Normalise sums its squares unparenthesised
//   (FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN); Pillars[index] is re-indexed at every use; the pillar is placed with
//   ResetPosition (+0x7c; SetPosition is +0x78) and its height lands in pos.Y.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_JACKOFBLADESBASE_SUMMONPILLARS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_JackOfBladesBase.h"
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CTCJackPillar {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_JACK_PILLAR; }
    void Enable(bool enable);                                     // 0x007317D0
};
void CAIStateGroup_JackOfBladesBase::SummonPillars()
{
 CDefPointer<const CJackOfBladesBattleDef> def;
 GetCreature().PeekBaseDef().GetSubDef(&def);
 long count=Pillars.size();
 long index=0;
 if(count==0)return;
 float interval=def->TimeBetweenPillars;
 long last_frame=0;
 EAIActionReturn result;
 do{
  long frame=GetWorld().GetFrame();
  if((frame-last_frame)*CMainGameComponent::GetConstantFPS()>interval){
   last_frame=frame;
   if(IS_THING_ALIVE(Pillars[index])){
    CTCJackPillar* pillar;
    if(Pillars[index]->GetTC(&pillar))pillar->Enable(true);
    if(IS_THING_ALIVE(PArenaCentre)){
     C3DVector dir=PArenaCentre->GetPos()-Pillars[index]->GetPos();
     dir.Normalise();
     Pillars[index]->GetPhysicsTC()->SetFacingAngleXY(dir.GetAngleXY());
     C3DVector pos=Pillars[index]->GetPos();
     pos.Y=GetWorld().PeekWorldMap().GetHeightZBelowPos(pos)+0.1f;
     Pillars[index]->GetPhysicsTC()->ResetPosition(pos);
    }
    ++index;
   }
  }
  result=AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(),"SUMMON_LOOP",true));
 }while(index<count&&result!=AI_ACTION_TERMINATE_PROCESS);
}
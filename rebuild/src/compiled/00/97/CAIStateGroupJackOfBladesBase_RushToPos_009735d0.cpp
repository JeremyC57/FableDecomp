// Leaf: ?RushToPos@CAIStateGroup_JackOfBladesBase@@IAE?AW4EAIActionReturn@@ABVC3DVector@@@Z
// CAIStateGroup_JackOfBladesBase::RushToPos(const C3DVector&) -- rush Jack to a position: take the offset from the creature to the
//   position, its fast-normalised direction and its length; make the creature non-solid to the target thing (adding a
//   CTCSolidToThing component when it has none); perform-and-wait a CCreatureAction_CastAssassinRush along that direction with a
//   "RUSH_OUTOF" CCreatureAction_PlayCombatAnimation as its final action; then, if the target is still alive, make the creature
//   solid to it again. Returns the action result.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009735D0, 690 bytes, __fastcall `ret 4`, no EH frame (Jack TU), /O2.
// Callees: CThing::GetPos 0x004C73D0, CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020, CCharString ctor 0x0099EBF0 /
//   dtor 0x0099EAE0, CThing::AddTC 0x004C9D60 ("CTCSolidToThing"), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CTCSolidToThing::AddThingNotSolidTo 0x00770FA0 / RemoveThingNotSolidTo 0x00770F20, operator new 0x00BFEA1A / delete 0x00BFE9BC,
//   CCreatureAction_PlayCombatAnimation ctor 0x008B7FF0, CCountedPointer<T>::DeleteData 0x00668610 (stored in the count block),
//   CCreatureAction_CastAssassinRush ctor 0x008CE1B0, ~CCombatActionBase 0x0062E900 (end of the inline temp dtor),
//   CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70. Data: GFastOneOverSqrtTable 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Shape: the expanded C3DVector::Normalise sums its squares parenthesised one by one, Z, Y, X (overlay opt-in
//   FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX): the three squares are spilled first and summed again for GetMagnitude's
//   fsqrt (the default body shares the whole sum). Both string arguments are implicit const char* -> CCharString temporaries
//   (an explicit CCharString(...) temp loads the creature before constructing the string). GetCreature() is re-read at every use.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_JACKOFBLADESBASE_RUSHTOPOS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCSOLIDTOTHING_REMOVETHINGNOTSOLIDTO
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_JackOfBladesBase.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CCreatureAction_CastAssassinRush.h"
#include "engine/ThingAlive.h"
EAIActionReturn CAIStateGroup_JackOfBladesBase::RushToPos(const C3DVector& pos)
{
 CTCSolidToThing* solid=0;
 C3DVector delta=pos-GetCreature().GetPos();
 C3DVector dir=delta;
 dir.Normalise();
 float distance=delta.GetMagnitude();
 if(!GetCreature().GetTC(TCI_SOLID_TO_THING,&solid))
  solid=(CTCSolidToThing*)GetCreature().AddTC("CTCSolidToThing",false,0);
 if(solid)solid->AddThingNotSolidTo(PTargetThing.Get());
 CCountedPointer<CCreatureActionBase> final_action(new CCreatureAction_PlayCombatAnimation(GetCreature(),"RUSH_OUTOF",true));
 EAIActionReturn result=AIAction_PerformActionAndWait(CCreatureAction_CastAssassinRush(GetCreature(),dir,distance,false,0,final_action),false);
 if(IS_THING_ALIVE(PTargetThing)&&solid)solid->RemoveThingNotSolidTo(PTargetThing.Get());
 return result;
}
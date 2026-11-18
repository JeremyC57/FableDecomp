// CCreatureAction_CastStrengthMultiStrike::FrameUpdate() -- run the base CCombatActionBase::FrameUpdate, then finish the action once it is
//   ready for interruption (virtual IsActionReadyForInterruption, slot 27; virtual FinishAction, slot 4, tail jmp)
// Leaf: ?FrameUpdate@CCreatureAction_CastStrengthMultiStrike@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E6390, 29 bytes, virtual slot 56 of vtable 0x0128764C.
// NF75 (round 7, land1117fu q10916): was a CCreatureAction_CastStrengthMultiStrike_Methods stand-in with 28 placeholder virtuals
//   (v0..v26, DoStrike, ShouldStrike) on the flat header, calling the stand-in
//   ?CCreatureAction_Base_FrameUpdate@@YIXPAVCCreatureAction_CastStrengthMultiStrike@@@Z. Now the retail class of
//   engine/CCreatureAction_CastStrengthMultiStrike.h (overlay opt-in FABLE_RETAIL_CCREATUREACTION_CASTSTRENGTHMULTISTRIKE_CLASS) on
//   engine/CCombatActionBase.h (overlay FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE): the base call binds Ego_r.pdb
//   ?FrameUpdate@CCombatActionBase@@UAEXXZ (0x00859D80); slot 27 is CCreatureActionBase::IsActionReadyForInterruption and slot 4
//   FinishAction (engine/CCreatureActionBase.h retail virtual order, CBaseObject's Validate in slot 1).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCREATUREACTION_CASTSTRENGTHMULTISTRIKE_CLASS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NCreatureActionEvents.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_CastStrengthMultiStrike.h"

void CCreatureAction_CastStrengthMultiStrike::FrameUpdate()
{
    CCombatActionBase::FrameUpdate();
    if (IsActionReadyForInterruption())
        FinishAction();
}

// CCreatureAction_PerformExpressionExtended::CCreatureAction_PerformExpressionExtended(CThingCreatureBase& creature, CThing* pperform_to, const CDefPointer<const CExpressionDef>& pdef, long expression_item_def_index, bool emote_icon_added, unsigned long sound_id, long frames_running, long interruption_group_id) -- start an extended expression: the base action on the def's Animation, the members from the parameters, then the to-other loop animation when performing to a thing (else the plain loop animation) when the def names one
// Leaf: ??0CCreatureAction_PerformExpressionExtended@@QAE@AAVCThingCreatureBase@@PAVCThing@@ABV?$CDefPointer@$$CBVCExpressionDef@@@@J_NKJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00843310, 347 bytes, `ret 0x20`. Vtable 0x012744FC.
// Identity: Ego_r.pdb 0x0051D970 / FableWin.pdb 0x01DD04D0 (the leaf above); Ego_d expression actions TU: parameter names creature,
//   pperform_to, pdef, expression_item_def_index, emote_icon_added, sound_id, frames_running, interruption_group_id and NO named
//   locals -- so no animation-name local: the two SetAnimation calls of the if / else-if are cross-jumped by VC7.1 into one tail
//   (0x00843425..0x00843457), only the def-string load (+0x54 AnimationToOtherLoop / +0x44 AnimationLoop) differs per arm.
// Callees: CDefStringTable::GetString 0x009D49B0 (CDefString -> CCharString, inline), CCreatureActionBase::GetRandomAnimFromName
//   0x006924B0 (protected static __fastcall), CCreatureActionBase ctor 0x00693B30, ~CCharString 0x0099EAE0, CBaseIntelligentPointer
//   ctor / SetPItem / GetPItem 0x00A01B10 / 0x00A01B90 / 0x00A01B50, CDefString::operator!= 0x005FA740, SetAnimation 0x00693030.
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETANIMATION
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_compiler.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_PerformExpressionExtended.h"
#include "engine/CExpressionDef.h"
#include "engine/CSoundPair.h"

CCreatureAction_PerformExpressionExtended::CCreatureAction_PerformExpressionExtended(CThingCreatureBase& creature, CThing* pperform_to,
        const CDefPointer<const CExpressionDef>& pdef, long expression_item_def_index, bool emote_icon_added, unsigned long sound_id,
        long frames_running, long interruption_group_id)
    : CCreatureActionBase(creature, -1.0f, 0.0f, GetRandomAnimFromName(creature, pdef->Animation), CSoundPair(), interruption_group_id, 0),
      PExpressionDef(pdef), PPerformToThing(pperform_to), ExpressionItemDefIndex(expression_item_def_index),
      EmoteIconAdded(emote_icon_added), FrameCount(frames_running), ReplacedAction(false), SoundEffectID(sound_id)
{
    if (PPerformToThing.GetPItem() && PExpressionDef->AnimationToOtherLoop != "")
        SetAnimation(GetRandomAnimFromName(*PCreature, PExpressionDef->AnimationToOtherLoop), -1.0f, 0.0f);
    else if (PExpressionDef->AnimationLoop != "")
        SetAnimation(GetRandomAnimFromName(*PCreature, PExpressionDef->AnimationLoop), -1.0f, 0.0f);
}

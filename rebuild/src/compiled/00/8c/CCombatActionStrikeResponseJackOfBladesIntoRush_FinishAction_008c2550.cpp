// CCombatAction_StrikeResponseJackOfBladesIntoRush::FinishAction() -- bump Jack's boss GenericCounter (CTCBoss, TCI_BOSS 0xAD; wraps to 1
//   past the battle def's NumGetHitsBeforeRush), then, when CauseRush is set and a valid rush position exists, take the 2D distance
//   and the normalised direction to it, run the base strike-response FinishAction and make Jack cast an assassin rush along
//   that direction with a "RUSH_OUTOF" combat animation as its final action; otherwise just the base FinishAction.
// Leaf: ?FinishAction@CCombatAction_StrikeResponseJackOfBladesIntoRush@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C2550, 607 bytes (+ int3 padding), virtual, no EH frame.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8), CVectorMap::LowerBound 0x0040F020 (GetTC expanded, same-TU
//   noinline header body), CDefClassBase::GetSubDef<CJackOfBladesBattleDef> 0x008C33E0 (same-TU noinline header body,
//   engine/CDefClassBase_GetSubDef.h), GetValidRushPosition(C3DVector&) 0x008C1600 (same-TU, visible noinline below), CThing::GetPos
//   0x004C73D0, CCombatAction_GenericStrikeResponseBase::FinishAction 0x008663D0, operator new 0x00BFEA1A, CCharString 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CCreatureAction_PlayCombatAnimation(CThingCreatureBase&, const CCharString&, bool) 0x008B7FF0,
//   CCountedPointer<CCreatureActionBase>(CCreatureActionBase*) 0x00695880 (visible noinline, FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR),
//   CCreatureAction_CastAssassinRush(CThingCreatureBase&, const C3DVector&, float, bool, long, CCountedPointer<CCreatureActionBase>)
//   0x008CE1B0 / ~ 0x0072D990, CThingCreatureBase::SetCurrentAction 0x006644F0, operator delete 0x00BFE9BC, def release through
//   vtable slot 1. Fast inverse-sqrt table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Port (NF74, round 7; nf41:e2): every class from the shared headers, the 17 local models and the stand-in bodies gone -- the
//   Initialise sibling 0x008C12D0's block (CCreatureActionBase PCreature +0x08, CCombatActionBase, CCombatAction_GenericStrikeResponseBase
//   TYPED, CCombatAction_StrikeResponseJackOfBladesIntoRush CauseRush +0x148, CTCBoss GetGenericCounter / SetGenericCounter, CDefPointer
//   <const CJackOfBladesBattleDef> NumGetHitsBeforeRush +0x40), RushToPos 0x009735D0's CCreatureAction_PlayCombatAnimation /
//   CCreatureAction_CastAssassinRush, and the NF74 overlay opt-ins CCombatAction_GenericStrikeResponseBase.h FINISHACTION,
//   CCombatAction_StrikeResponseJackOfBladesIntoRush.h FINISHACTION (FinishAction + the protected Ego_r GetValidRushPosition(C3DVector&))
//   and CTCBoss.h GETPHASESTARTPOS (FableWin inline). Locals carry the Ego_d.pdb names.
// Shape: GetValidRushPosition's body is visible but out of line, as in the retail TU, so VC7.1 sees that rush_to_pos's address is not
//   kept: retail reuses its dead words for the rush vector (dz spilled to rush_to_pos.Z; frame 0x16C; with the declaration only the
//   frame is 0x178, NF81 grade 1). NF81 (round 7, q10988): the row no longer carries its own older copy of that body (the rejected
//   CVectorFilter_FurtherFromPointThan phase filter); it includes the one text, the landed 0x008C1600 body, from the NF81 overlay header
//   engine/CCombatAction_StrikeResponseJackOfBladesIntoRush_GetValidRushPosition.h as an `inline __declspec(noinline)` COMDAT (never
//   expanded; the row 0x008C1600 defines it), as engine/CMap.h does for GetStrongestEngineThemeAt. The call binds 0x008C1600. Normalise is the
//   PAREN_SQUARES_ZYX body and GetMagnitude2D the one-expression FableWin body: the shared X*X / Y*Y products land in retail's slots.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCBOSS_GETPHASESTARTPOS
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED   // CTCBoss::PhaseStartPos at +0x22 (alignment 1, engine/CTCBoss.h; as 0x008C1600 / 0x009547D0), so the visible GetValidRushPosition COMDAT is compiled as at its own row
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#include "fable_compiler.h"
#include <math.h>
#include "engine/C3DVector.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCPhysicsBase.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBOSS_GETGENERICCOUNTER
#include "engine/CTCBoss.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "engine/CThingCreatureBase.h"
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_RECOVER
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_TYPED
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_FINISHACTION
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH
#include "engine/CCombatAction_StrikeResponseJackOfBladesIntoRush.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CCreatureAction_CastAssassinRush.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorFilter_FurtherFromPointThan.h"
#include "engine/NCreatureHiding.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/GFRandom.h"
#include "engine/CCombatAction_StrikeResponseJackOfBladesIntoRush_GetValidRushPosition.h"   // NF81 overlay: the 0x008C1600 body, visible noinline


void CCombatAction_StrikeResponseJackOfBladesIntoRush::FinishAction()
{
    CTCBoss* ptc_boss;
    reinterpret_cast<CThing*>(PCreature.GetPItem())->GetTC(TCI_BOSS, &ptc_boss);
    CDefPointer<const CJackOfBladesBattleDef> p_jack_def;
    reinterpret_cast<CThing*>(PCreature.GetPItem())->PeekBaseDef().GetSubDef<CJackOfBladesBattleDef>(&p_jack_def);
    long number_of_hits = ptc_boss->GetGenericCounter() + 1;
    if (number_of_hits > p_jack_def->NumGetHitsBeforeRush)
        ptc_boss->SetGenericCounter(1);
    else
        ptc_boss->SetGenericCounter(number_of_hits);
    C3DVector rush_to_pos;
    if (CauseRush && GetValidRushPosition(rush_to_pos)) {
        C3DVector rush_vector = rush_to_pos - reinterpret_cast<CThing*>(PCreature.GetPItem())->GetPos();
        C3DVector rush_direction = rush_vector;
        rush_direction.Normalise();
        float rush_distance = rush_vector.GetMagnitude2D();
        CThingCreatureBase* pthing_creature = reinterpret_cast<CThingCreatureBase*>(PCreature.GetPItem());
        CCombatAction_GenericStrikeResponseBase::FinishAction();
        CCountedPointer<CCreatureActionBase> paction(new CCreatureAction_PlayCombatAnimation(*pthing_creature, "RUSH_OUTOF", true));
        reinterpret_cast<CThingCreatureBase*>(PCreature.GetPItem())->SetCurrentAction(
            CCreatureAction_CastAssassinRush(*pthing_creature, rush_direction, rush_distance, false, 0, paction));
    } else
        CCombatAction_GenericStrikeResponseBase::FinishAction();
}

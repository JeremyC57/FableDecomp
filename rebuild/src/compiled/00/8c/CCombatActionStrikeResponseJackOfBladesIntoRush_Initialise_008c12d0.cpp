// CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise() -- Jack of Blades rushes only on the hit that brings his boss
//   GenericCounter (CTCBoss, interface TCI_BOSS = 0xAD) up to the battle def's NumGetHitsBeforeRush (CauseRush); otherwise it is a plain
//   strike response (CCombatAction_GenericStrikeResponseBase::Initialise). On a rush: take the direction from Jack to the point of
//   hit (or to the living hitter), normalise it, and when it lies more than 100 degrees from his facing play the back rush and
//   face away from the hit, else the front rush (facing angle from a zero vector); then the base CCombatActionBase::Initialise.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C12D0, 808 bytes, virtual (vtable 0x012808EC slot 12), no EH frame, /O2 default flags.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (same-TU noinline, engine/CVectorMap.h),
//   CDefClassBase::GetSubDef<CJackOfBladesBattleDef> 0x008C33E0 (same-TU noinline real body, engine/CDefClassBase_GetSubDef.h),
//   CCombatAction_GenericStrikeResponseBase::Initialise 0x00865F40, CTCPhysicsBase::GetFacingVector (vslot +0x118),
//   CThing::GetPos 0x004C73D0, CCharString::CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CCombatActionBase::SetCombatAnimation 0x008574D0, __CIfmod 0x00BFEB28 (fmod intrinsic), CTCPhysicsBase::SetFacingAngleXY
//   (vslot +0x108), CCombatActionBase::Initialise 0x00857520, def release through CDefPointeeBase::Delete (vslot +4).
//   Data: GFastOneOverSqrtTable 0x013CE558 (inline C3DVector::Normalise), 3.0f / 0.5f, the double 1.745329383786327 at 0x01277128
//   (100 degrees in radians; retail takes its cosine at run time, `fld qword [0x1277128] ; fcos` at 0x008C14DF).
// Shape: CauseRush is set by two tests, as retail does at 0x008C1343 / 0x008C1345 (`cmp eax,ecx ; jg 0x8C1350 ; jne 0x8C1350`, both
//   to the one `mov [esi+148h],bl` false store, the fall-through storing 1): `hits > N` false, else `hits == N` true, else false.
//   The rush body is the `else` of `if (!CauseRush)` -- written as an early return VC7.1 gives the def a whole-function
//   callee-saved register (ebx) and frame 0x38; as if/else it keeps the def in edi only until the branch, spills it over the rush
//   body and reloads it for the release (`mov edi,[esp+14h]`), and the free ebx carries the 0 (def init, CauseRush=false, cmp).
//   The real CDefPointer (engine/CDefPointer.h) with the visible noinline GetSubDef body lets VC7.1 drop the Release null test.
//   The alive test is IS_THING_ALIVE<CIntelligentPointer<const CThing>> (engine/ThingAlive.h, `!thing->IsDead()`:
//   `test byte [eax+91h],1`; `IsAlive()` = `!Dead` gives `not cl ; test cl,1`).
// Layout: shared headers with per-TU opt-ins -- CCreatureActionBase (FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE, PCreature +0x08),
//   CCombatActionBase (FABLE_RETAIL_CCOMBATACTIONBASE_RECOVER: Initialise / SetCombatAnimation), overlay
//   CCombatAction_GenericStrikeResponseBase.h (FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_TYPED: PThingThatHit +0x114,
//   PointOfHit +0x11C, Initialise), overlay CCombatAction_StrikeResponseJackOfBladesIntoRush.h (FABLE_RETAIL_CCOMBATACTION_
//   STRIKERESPONSEJACKOFBLADESINTORUSH: the derived class, CauseRush +0x148), overlay CTCBoss.h (FABLE_RETAIL_CTCBOSS_GETGENERICCOUNTER:
//   FableWin.pdb ?GetGenericCounter@CTCBoss@@QBEJXZ inline, GenericCounter +0x10).
// Leaf: ?Initialise@CCombatAction_StrikeResponseJackOfBladesIntoRush
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(cos, atan2, fmod)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
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
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/ThingAlive.h"
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_RECOVER
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_TYPED
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH
#include "engine/CCombatAction_StrikeResponseJackOfBladesIntoRush.h"

void CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise()
{
    CTCBoss* boss;
    reinterpret_cast<CThing*>(PCreature.GetPItem())->GetTC(TCI_BOSS, &boss);
    CDefPointer<const CJackOfBladesBattleDef> def;
    reinterpret_cast<CThing*>(PCreature.GetPItem())->PeekBaseDef().GetSubDef<CJackOfBladesBattleDef>(&def);
    long hits = boss->GetGenericCounter() + 1;
    if (hits > def->NumGetHitsBeforeRush)
        CauseRush = false;
    else if (hits == def->NumGetHitsBeforeRush)
        CauseRush = true;
    else
        CauseRush = false;
    if (!CauseRush) {
        CCombatAction_GenericStrikeResponseBase::Initialise();
    } else {
        C3DVector facing = reinterpret_cast<CThing*>(PCreature.GetPItem())->GetPhysicsTC()->GetFacingVector();
        C3DVector direction = PointOfHit - reinterpret_cast<CThing*>(PCreature.GetPItem())->GetPos();
        if (IS_THING_ALIVE(PThingThatHit)) {
            const CThing* hitter = PThingThatHit.Get();
            direction = hitter->GetPos() - reinterpret_cast<CThing*>(PCreature.GetPItem())->GetPos();
        }
        C3DVector away(0.0f, 0.0f, 0.0f);
        direction.Normalise();
        if (facing.Dot(direction) < (float)cos(1.745329383786327)) {
            away = -direction;
            CCharString animation("RESPONSE_GENERIC_HIT_BACK_RUSH");
            SetCombatAnimation(animation);
        } else {
            CCharString animation("RESPONSE_GENERIC_HIT_FRONT_RUSH");
            SetCombatAnimation(animation);
        }
        reinterpret_cast<CThing*>(PCreature.GetPItem())->GetPhysicsTC()->SetFacingAngleXY(away.GetAngleXY());
        CCombatActionBase::Initialise();
    }
}
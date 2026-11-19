// CCreatureAction_KrakenHighAttackLoop::FinishAction() -- creature-action FinishAction; while the target is alive and loops remain, run the next high attack loop (an early finish jumps to the last two loops and marks it interrupted; entering them ends the beam and stops head tracking), otherwise the high attack "out of" action
// Leaf: ?FinishAction@CCreatureAction_KrakenHighAttackLoop@?A0xe2035276@@UAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): IsTargetAlive's dead test through CThing::IsDead() const (Ego_r.pdb
//   ?IsDead@CThing@@QBE_NXZ, the bool keeps retail's mov cl,1 ; test cl,dl) plus a same-TU noinline
//   CInterfaceMap::LowerBound stand-in (GOTCHAS chunk 240) that lets the kraken share the 0xE2 key word (sub esp,120h).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924930, 344 bytes (+ int3 padding), vtable slot 4 of 0x0129107C (RTTI
//   `?A0x29922355::CCreatureAction_KrakenHighAttackLoop`; FableWin.pdb
//   ?FinishAction@CCreatureAction_KrakenHighAttackLoop@?A0xe2035276@@UAEXXZ). Catalog "vfunc_4".
// Callees: CCreatureActionBase::FinishAction @ 0x00694430 (first: the combat base's override is skipped), GetPItem
//   @ 0x00A01B50, LowerBound @ 0x0040F020 (the inline unchecked GetKraken(), interface 0xe2; the kraken is homed in the key
//   word), CTCKraken::EndAttackBeam @ 0x007FC800, CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase*, CThing*, long,
//   bool) @ 0x00925780, CCreatureAction_KrakenHighAttackOutOf(CThingCreatureBase*) @ 0x00925820 (landed),
//   CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Layout: Ego_r.pdb CCreatureActionBase TotalTime +0x10 / FramesRunning +0x1C; the landed FrameUpdate 0x009242D0
//   (Interrupt +0x114, LoopNum +0x118); CKrakenDef HighStrikeAttackLoops +0x44; CTCKraken HeadTrackTarget +0x51 (PDB +0x61).
// Shape: the loop limit is read into a local before the alive test; the target is fetched into a local before the
//   SetCurrentAction call (retail's fetch order), as the twin 0x00924720.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
// Port (NF98, round 8, q11261 / q11262): the local CThing / CThingCreatureBase / CCreatureActionBase / CCombatActionBase / CTCKraken /
//   CKrakenDef / CInterfaceMap models replaced by the shared headers on the sibling 0x00924B90 (KrakenRoarLoop::FinishAction) recipe, with
//   the MELEE CCombatActionBase (PTarget +0xA8): SetCurrentAction 0x006644F0 and the temporary's destructor 0x0062E900 (~CCombatActionBase)
//   now bind the PDB spellings. The two action classes are retail anonymous-namespace classes (RTTI ?A0x29922355::, Ego_r.pdb / FableWin.pdb
//   ?A0xe2035276) with no shared header: row-local in `namespace { }`, ctors from Ego_r.pdb (0x00948BA0 Loop(CThingCreatureBase&, CThing&,
//   long, bool); FableWin.pdb 0x02C5E620 OutOf(CThingCreatureBase&)); own leaf respelled to the PDB ?A0xe2035276 spelling, as 0x00924B90.
//   HeadTrackTarget is set through FableWin's inline SetHeadTracking (NF98 opt-in FABLE_RETAIL_CTCKRAKEN_SETHEADTRACKING_INLINE).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CTCKRAKEN_SETHEADTRACKING_INLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCKraken.h"
#include "engine/CDefPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/ThingAlive.h"
namespace {
class CCreatureAction_KrakenHighAttackOutOf : public CCombatActionBase {   // vtable 0x01291124
public:
    CCreatureAction_KrakenHighAttackOutOf(CThingCreatureBase& creature);   // 0x00925820
};
class CCreatureAction_KrakenHighAttackLoop : public CCombatActionBase {   // vtable 0x0129107C
public:
    CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase& creature, CThing& target, long loop_num, bool interrupt);   // 0x00925780
    virtual void FinishAction();
private:
    bool Interrupt;                             // +0x114
    long LoopNum;                               // +0x118
};
}

void CCreatureAction_KrakenHighAttackLoop::FinishAction()
{
    CCreatureActionBase::FinishAction();
    CTCKraken* pTCKraken;
    PCreature->GetTC(&pTCKraken);
    long loops = pTCKraken->GetKrakenDef().HighStrikeAttackLoops;
    if (IS_THING_ALIVE(PTarget) && ++LoopNum < loops) {
        if (FramesRunning < TotalTime) {
            LoopNum = loops - 2;
            Interrupt = true;
        }
        if (LoopNum == loops - 2) {
            pTCKraken->EndAttackBeam();
            pTCKraken->SetHeadTracking(false);
        }
        CThing* target = PTarget.Get();
        PCreature->SetCurrentAction(CCreatureAction_KrakenHighAttackLoop(*PCreature, *target, LoopNum, Interrupt));
    } else
        PCreature->SetCurrentAction(CCreatureAction_KrakenHighAttackOutOf(*PCreature));
}

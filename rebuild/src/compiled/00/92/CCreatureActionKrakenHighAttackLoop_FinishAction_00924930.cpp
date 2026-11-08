// CCreatureAction_KrakenHighAttackLoop::FinishAction() -- creature-action FinishAction; while the target is alive and loops remain, run the next high attack loop (an early finish jumps to the last two loops and marks it interrupted; entering them ends the beam and stops head tracking), otherwise the high attack "out of" action
// Leaf: ?FinishAction@CCreatureAction_KrakenHighAttackLoop
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
#include "fable_compiler.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase;
class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CKrakenDef { public:
    unsigned char _pad00[0x44];
    long          HighStrikeAttackLoops;        // +0x44 (Ego_r.pdb)
};
class CTCKraken { public:                              // interface type 0xe2 component
    unsigned char     _pad00[0x0c];
    const CKrakenDef* Def;                      // +0x0C
    unsigned char     _pad10[0x51 - 0x10];
    bool              HeadTrackTarget;          // +0x51 (PDB +0x61)
    void EndAttackBeam();                       // 0x007FC800
};

class CCreatureActionBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void FinishAction();                // 0x00694430 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    long                    TotalTime;          // +0x10
    long                    ActionTime;         // +0x14
    long                    AnimDelayTime;      // +0x18
    long                    FramesRunning;      // +0x1C
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    virtual void FinishAction();                // 0x00859A20
    unsigned char           _pad20[0xa8 - 0x20];
    CBaseIntelligentPointer PTarget;            // +0xa8 CIntelligentPointer<CThing>
    unsigned char           _padb0[0x114 - 0xb0];
    ~CCombatActionBase();                       // 0x0062E900
    FABLE_FORCEINLINE CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.GetPItem()); }
    FABLE_FORCEINLINE bool IsTargetAlive() const    // two fetches: present, then not dead
    {
        return PTarget.GetPItem() && !PTarget.GetPItem()->IsDead();
    }
    FABLE_FORCEINLINE CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetTC(TCI_KRAKEN, &kraken);
        return kraken;
    }
};
struct CCreatureAction_KrakenHighAttackOutOf : CCombatActionBase {   // vtable 0x01291124
    CCreatureAction_KrakenHighAttackOutOf(CThingCreatureBase* owner);   // 0x00925820
};
struct CCreatureAction_KrakenHighAttackLoop : CCombatActionBase {   // vtable 0x0129107C
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase* owner, CThing* thing, long loopNum, bool interrupt);   // 0x00925780
    virtual void FinishAction();
};

void CCreatureAction_KrakenHighAttackLoop::FinishAction()
{
    CCreatureActionBase::FinishAction();
    CTCKraken* kraken = GetKraken();
    long loops = kraken->Def->HighStrikeAttackLoops;
    if (IsTargetAlive() && ++LoopNum < loops) {
        if (FramesRunning < TotalTime) {
            LoopNum = loops - 2;
            Interrupt = true;
        }
        if (LoopNum == loops - 2) {
            kraken->EndAttackBeam();
            kraken->HeadTrackTarget = false;
        }
        CThing* target = PTarget.GetPItem();
        Creature().SetCurrentAction(CCreatureAction_KrakenHighAttackLoop(&Creature(), target, LoopNum, Interrupt));
    } else
        Creature().SetCurrentAction(CCreatureAction_KrakenHighAttackOutOf(&Creature()));
}
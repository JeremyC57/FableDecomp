// CCreatureAction_KrakenHighAttackLoop::FrameUpdate() -- unless already interrupted, finish the action (virtual FinishAction) once the kraken reports its target hit; then the base FrameUpdate
// Leaf: ?FrameUpdate@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00009242D0, length 117 bytes (+ int3 padding), vtable slot 56 of 0x0129107C
// (RTTI `?A0x29922355::CCreatureAction_KrakenHighAttackLoop`; FableWin.pdb ?FrameUpdate@CCreatureAction_KrakenHighAttackLoop@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0xe2 = CTCKraken, bit 2 of the bitset byte at +0x3c), CTCKraken::HasTargetBeenHitByKraken @ 0x007FB500, the
// action's virtual FinishAction (slot 4, +0x10), CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call, last).
// Shape: Interrupt is tested before the lookup and re-read after the bool call (the nested `if (!Interrupt)` guard around
// FinishAction); the UNCHECKED lookup sits in an inline accessor (absent arm reads the key word).
// Layout: Ego_r.pdb Interrupt / LoopNum +0x12c / +0x130, retail +0x114 / +0x118 (landed ctor 0x00925780).
#include "fable_compiler.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCKraken { public:                              // interface type 0xe2 component
    bool HasTargetBeenHitByKraken();            // 0x007FB500
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void FinishAction();                // slot 4 (+0x10)
    virtual void FrameUpdate();                 // 0x00859D80 (slot 56)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    FABLE_FORCEINLINE CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.Get()->GetTC(TCI_KRAKEN, &kraken);
        return kraken;
    }
};
struct CCreatureAction_KrakenHighAttackLoop : CCombatActionBase {   // vtable 0x0129107C
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    virtual void FrameUpdate();
};

void CCreatureAction_KrakenHighAttackLoop::FrameUpdate()
{
    if (!Interrupt && GetKraken()->HasTargetBeenHitByKraken()) {
        if (!Interrupt)
            FinishAction();
    }
    CCombatActionBase::FrameUpdate();
}
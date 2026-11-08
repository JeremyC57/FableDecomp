// CCreatureAction_KrakenLowAttackLoop::Initialise() -- on the first loop (LoopNum 0) switch off the kraken's head tracking; then the base Initialise
// Leaf: ?Initialise@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923FD0, length 105 bytes (+ int3 padding), vtable slot 12 of 0x01290C64
// (RTTI `?A0x29922355::CCreatureAction_KrakenLowAttackLoop`; FableWin.pdb ?Initialise@CCreatureAction_KrakenLowAttackLoop@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0xe2 = CTCKraken, bit 2 of the bitset byte at +0x3c), CCombatActionBase::Initialise @ 0x00857520 (last).
// Shape: the UNCHECKED out-parameter lookup in an inline accessor (absent arm reads the key word and falls into the shared
// base call; the present arm duplicates the tail).
// Layout: Ego_r.pdb Interrupt / LoopNum +0x12c / +0x130, retail +0x114 / +0x118 (landed ctor 0x009255E0); CTCKraken
// HeadTrackTarget +0x51 (PDB +0x61).
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
    unsigned char _pad00[0x51];
    bool          HeadTrackTarget;              // +0x51 (PDB +0x61)
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520 (slot 12)
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
struct CCreatureAction_KrakenLowAttackLoop : CCombatActionBase {   // vtable 0x01290C64
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    virtual void Initialise();
};

void CCreatureAction_KrakenLowAttackLoop::Initialise()
{
    if (LoopNum == 0)
        GetKraken()->HeadTrackTarget = false;
    CCombatActionBase::Initialise();
}
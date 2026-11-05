// CCreatureAction_KrakenHighAttackOutOf::Initialise() -- end the kraken's attack beam, then the base Initialise
// Leaf: ?Initialise@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00009243B0, length 101 bytes (+ int3 padding), vtable slot 12 of 0x012911DC
// (RTTI `?A0x29922355::CCreatureAction_KrakenHighAttackOutOf`; FableWin.pdb ?Initialise@CCreatureAction_KrakenHighAttackOutOf@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0xe2 = CTCKraken, bit 2 of the bitset byte at +0x3c), CTCKraken::EndAttackBeam @ 0x007FC800 (landed),
// CCombatActionBase::Initialise @ 0x00857520 (direct base call, last).
// Shape: the UNCHECKED out-parameter lookup in an inline accessor (absent arm reads the key word); VC duplicates the two-call
// tail into both arms. The class adds no members (sizeof 0x114, ctor 0x00925820).
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
    void EndAttackBeam();                       // 0x007FC800
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520 (slot 12)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.Get()->GetTC(TCI_KRAKEN, &kraken);
        return kraken;
    }
};
struct CCreatureAction_KrakenHighAttackOutOf : CCombatActionBase {   // vtable 0x012911DC
    virtual void Initialise();
};

void CCreatureAction_KrakenHighAttackOutOf::Initialise()
{
    GetKraken()->EndAttackBeam();
    CCombatActionBase::Initialise();
}
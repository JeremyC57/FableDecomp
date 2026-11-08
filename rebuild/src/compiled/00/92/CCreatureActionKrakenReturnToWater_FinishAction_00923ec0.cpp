// CCreatureAction_KrakenReturnToWater::FinishAction() -- base FinishAction, advance the owner's boss phase, then clear the kraken's ReturnHeadToWater flag
// Leaf: ?FinishAction@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923EC0, length 160 bytes, vtable slot 4 of 0x012906F4
// (RTTI `?A0x29922355::CCreatureAction_KrakenReturnToWater`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenReturnToWater@?A0xe2035276@@UAEXXZ).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// address kept in edi), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xad = CTCBoss, bit 13 of the bitset dword at
// +0x34, `test ch,20h`; then 0xe2 = CTCKraken, bit 2 of the byte at +0x3c), CTCBoss::IncrementBossPhase @ 0x007AED20.
// Shape: both lookups are UNCHECKED accessors (absent arms read the key word); the final store is duplicated into both arms.
// Layout: CTCKraken ReturnHeadToWater +0x5c (Ego_r.pdb +0x6c).
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

class CTCBoss { public:                                // interface type 0xad component
    void IncrementBossPhase();                  // 0x007AED20
};
class CTCKraken { public:                              // interface type 0xe2 component
    unsigned char _pad00[0x5c];
    bool          ReturnHeadToWater;            // +0x5c (PDB +0x6c)
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    protected: virtual void FinishAction(); public:                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    FABLE_FORCEINLINE CTCBoss* GetBoss() const
    {
        CTCBoss* boss;
        PCreature.Get()->GetTC(TCI_BOSS, &boss);
        return boss;
    }
    FABLE_FORCEINLINE CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.Get()->GetTC(TCI_KRAKEN, &kraken);
        return kraken;
    }
};
struct CCreatureAction_KrakenReturnToWater : CCombatActionBase {   // vtable 0x012906F4, sizeof 0x114
    virtual void FinishAction();
};

void CCreatureAction_KrakenReturnToWater::FinishAction()
{
    CCombatActionBase::FinishAction();
    GetBoss()->IncrementBossPhase();
    GetKraken()->ReturnHeadToWater = false;
}
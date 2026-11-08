// CCreatureAction_KrakenRiseUp::Initialise() -- base Initialise, remove creature mode 0x54 through the owner's mode manager, then start the kraken's head effects
// Leaf: ?Initialise@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923CF0, length 166 bytes (+ int3 padding), vtable slot 12 of 0x01290594
// (RTTI `?A0x29922355::CCreatureAction_KrakenRiseUp`; FableWin.pdb ?Initialise@CCreatureAction_KrakenRiseUp@?A0xe2035276@@UAEXXZ).
// Callees: CCombatActionBase::Initialise @ 0x00857520 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// address kept in edi), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 = CTCCreatureModeManager, bit 17 of the
// bitset dword at +0x24; then 0xe2 = CTCKraken, bit 2 of the byte at +0x3c), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0,
// CTCKraken::StartHeadEffects @ 0x007FBC30 (landed).
// Shape: BOTH lookups are UNCHECKED out-parameter accessors (each absent arm reads the shared key word and still calls).
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

class CTCCreatureModeManager { public:                 // interface type 0x31 component
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
class CTCKraken { public:                              // interface type 0xe2 component
    void StartHeadEffects();                    // 0x007FBC30
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520 (slot 12)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    FABLE_FORCEINLINE CTCCreatureModeManager* GetModeManager() const
    {
        CTCCreatureModeManager* modeManager;
        PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
        return modeManager;
    }
    FABLE_FORCEINLINE CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.Get()->GetTC(TCI_KRAKEN, &kraken);
        return kraken;
    }
};
struct CCreatureAction_KrakenRiseUp : CCombatActionBase {   // vtable 0x01290594, sizeof 0x114
    virtual void Initialise();
};

void CCreatureAction_KrakenRiseUp::Initialise()
{
    CCombatActionBase::Initialise();
    GetModeManager()->RemoveMode(0x54);
    GetKraken()->StartHeadEffects();
}
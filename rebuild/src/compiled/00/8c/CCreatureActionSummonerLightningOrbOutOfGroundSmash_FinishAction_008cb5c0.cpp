// CCreatureAction_SummonerLightningOrbOutOfGroundSmash::FinishAction() -- base FinishAction, then if the creature has a mode manager (interface type 0x31) that holds mode 90, remove that mode
// Leaf: ?FinishAction@CCreatureAction_SummonerLightningOrbOutOfGroundSmash@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB5C0, length 96 bytes (byte-identical to the OrbOutOf body @ 0x008CB4A0 up to the relocations). FinishAction is vtable slot 4 (protected `MAE`); the base is called directly here.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::IsModeActive(unsigned long) @ 0x0073A8A0 (manifest "CPlayer::HasMode" is propagated; landed naming of 0x00977A30),
// CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0.
// Shape: one `push ecx` frame for the lookup key; the mode manager rides in esi for both calls (the landed 0x008B7640 lookup with the
// HasMode guard of 0x00977A30). No engine header matches the retail layout (CCreatureActionBase is 0xa8 bytes in retail), so `this` is modelled locally.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_SummonerLightningOrbOutOfGroundSmash : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_SummonerLightningOrbOutOfGroundSmash::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCCreatureModeManager* pModes;
    if (PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes)) {
        if (pModes->IsModeActive(90))
            pModes->RemoveMode(90);
    }
}
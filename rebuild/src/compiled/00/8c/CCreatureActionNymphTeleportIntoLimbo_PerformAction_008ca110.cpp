// CCreatureAction_NymphTeleportIntoLimbo::PerformAction() -- if the creature has the nymph component (interface type 0xf5), start it flying around for the remaining time (the words at +0x10 and +0x1c), then do the limbo effect
// Leaf: ?PerformAction@CCreatureAction_NymphTeleportIntoLimbo@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CA110, length 92 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0xf5 =
// bit 21 of the bitset dword at +0x3c, landed shape of 0x00849320), CTCNymph::StartFlyingAround(long) @ 0x0080F5E0, CCreatureAction_NymphTeleportLimboBase::DoEffect
// @ 0x008C9FE0 (non-virtual). Shape: one `push ecx` frame for the lookup key; the action rides in edi, the map in esi.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCNymph { public:
    void StartFlyingAround(long duration);  // 0x0080F5E0
};

class CCreatureAction_NymphTeleportLimboBase { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08
    long m_duration;                        // +0x10
    unsigned char _pad14[8];
    long m_elapsed;                         // +0x1c
    void DoEffect();                        // 0x008C9FE0
};
class CCreatureAction_NymphTeleportIntoLimbo : public CCreatureAction_NymphTeleportLimboBase { public:
    virtual void PerformAction();
};

void CCreatureAction_NymphTeleportIntoLimbo::PerformAction()
{
    CTCNymph* pNymph;
    if (PCreature.Get()->GetTC(TCI_NYMPH, &pNymph))
        pNymph->StartFlyingAround(m_duration - m_elapsed);
    DoEffect();
}
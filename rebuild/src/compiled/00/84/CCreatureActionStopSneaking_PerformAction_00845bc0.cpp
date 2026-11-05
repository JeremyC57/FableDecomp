// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// Leaf: ?PerformAction@CCreatureAction_StopSneaking@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// CCreatureAction_StopSneaking::PerformAction() -- the creature leaves creature mode 7 (sneaking) through its mode manager
//   (interface 0x31) and clears the active flag (+0x0f) of its interface-0x36 component
// Retail: Fable.exe (TLC, VC7.1) @ 0x00845BC0, 160 bytes, plain `ret`; slot 3 of vtable 0x01258F24 =
//   CCreatureAction_StopSneaking (rtti-vtables.tsv).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020,
//   CTCCreatureModeManager::RemoveMode(ulong) @ 0x0073ABA0 (manifest).
// Shape: two out-parameter GetInterface calls (void template, out pointers uninitialised so the absent path reads the
//   shared dead frame word); the RemoveMode call is tail-duplicated after the second lookup.
// Layout: CCreatureActionBase (retail 0xa8 bytes) with PCreature at +8; nothing else of the class is touched. The
//   interface-0x36 component is modelled only by the byte it clears (CTCSneaking is a descriptive name).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCCreatureModeManager { public:             // interface type 0x31 component
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];
};

struct CTCSneaking { unsigned char _pad[0xf]; unsigned char Active; };

class CCreatureAction_StopSneaking : public CCreatureActionBase { public:
    virtual void PerformAction();
};

void CCreatureAction_StopSneaking::PerformAction()
{
    CTCCreatureModeManager* mode;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &mode);
    CTCSneaking* sneak;
    PCreature.Get()->GetTC(TCI_STEALTH, &sneak);
    mode->RemoveMode(7);
    sneak->Active = 0;
}
// CCreatureAction_GetUpFromSitting::PerformAction() -- when the sitting mode is to be removed on PerformAction, take the creature out of it
// Leaf: ?PerformAction@CCreatureAction_GetUpFromSitting@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B79A0, length 121 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CVectorMap::LowerBound @ 0x0040F020, CTCCreatureModeManager::RemoveMode(ulong) @ 0x0073ABA0.
// Shape: SittingMode (+0x114) non-zero and the RemoveOnPerformAction byte (+0x118) tested through eax/al; the
//   out-parameter GetInterface template for the mode manager (bit 0x31 = dword +0x24 test, out pointer
//   uninitialised so the absent path reads the dead frame word) and RemoveMode(SittingMode), tail-duplicated; the
//   map pointer's esi is pushed only inside the lookup branch.
// Layout: Ego_r.pdb puts SittingMode/RemoveOnPerformAction at +0x12c/+0x130 (PDB CCombatActionBase 0x12c bytes,
//   retail 0x114): +0x114/+0x118 here; no engine header exists for this class.
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

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01();
    virtual void Initialise();              // 0x00857520
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
};

class CCreatureAction_GetUpFromSitting : public CCombatActionBase { public:
    long SittingMode;                       // +0x114 (PDB +0x12c)
    bool RemoveOnPerformAction;             // +0x118 (PDB +0x130)
    virtual void PerformAction();
};

void CCreatureAction_GetUpFromSitting::PerformAction()
{
    if (SittingMode != 0 && RemoveOnPerformAction) {
        CTCCreatureModeManager* modes;
        PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        modes->RemoveMode(SittingMode);
    }
}
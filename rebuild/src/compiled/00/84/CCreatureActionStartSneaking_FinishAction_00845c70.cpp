// CCreatureAction_StartSneaking::FinishAction() -- put the creature into mode 7 (sneaking) through its mode manager (interface type 0x31), then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_StartSneaking@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845C70, length 105 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup
// for type 0x31 = bit 17 of the bitset dword at +0x24), CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10 (bool result dropped),
// CCreatureActionBase::FinishAction @ 0x00694430 (direct base call).
// Shape: the mode manager is an UNINITIALISED out-parameter used unconditionally (the landed 0x00930DC0 shape): the AddMode + base call tail is
// duplicated into the miss path, which reads the never-written frame word; esi is pushed only around the lookup.
// The PDB header (CCreatureAction_StartSneaking.h) flattens the base; modelled locally (retail CCreatureActionBase is 0xa8 bytes).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCCreatureModeManager { public:             // interface type 0x31 component
    bool AddMode(unsigned long mode);       // 0x0073AE10
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4, 0x00694430
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_StartSneaking : public CCreatureActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_StartSneaking::FinishAction()
{
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);
    pModes->AddMode(7);
    CCreatureActionBase::FinishAction();
}
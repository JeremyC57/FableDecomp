// CCreatureAction_ScorpionTailWithdraw::FinishAction() -- resume the creature's suspended animation, drop creature mode 47 through the mode manager (interface type 0x31), then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_ScorpionTailWithdraw@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4780, length 139 bytes (+ int3 padding). FinishAction is vtable slot 4.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CThingAICreature::SuspendAI @ 0x00833A10
// (two zero arguments; called only while the thing's suspend byte at +0x10 equals 1), CVectorMap::LowerBound @ 0x0040F020 (the inlined
// GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0,
// CCombatActionBase::FinishAction @ 0x00859A20 (direct base call).
// Shape: the mode manager is an UNINITIALISED out-parameter of the inlined GetInterface, used unconditionally: on the miss path retail
// reads the never-written frame word (`mov eax,[esp+8]`, the slot shared with the lookup key) and the RemoveMode + base call tail is
// duplicated into both arms. The creature for SuspendAI is fetched BEFORE the two pushes, so it is a named local
// (a chained `GetPItem()->Set(0, 0)` pushes first). `this` rides in edi, &PCreature in esi until the lookup re-uses esi for the map.
// The PDB header (CCreatureAction_ScorpionTailWithdraw.h, 0x138 bytes) sits 0x24 bytes high against retail (CCombatActionBase is 0x114
// here), so `this` is modelled locally like the landed CCombatActionBase relatives (0x008CB4A0).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};

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
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_ScorpionTailWithdraw : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_ScorpionTailWithdraw::FinishAction()
{
    if (PCreature.GetPItem()->GetType() == 1) {
        CThing* creature = PCreature.GetPItem();   // fetched before the argument pushes: a named local
        ((CThingAICreature*)creature)->SuspendAI(false, (void*)false);
    }
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);
    pModes->RemoveMode(47);
    CCombatActionBase::FinishAction();
}
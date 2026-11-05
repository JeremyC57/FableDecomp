// CCreatureAction_UndeadRiseUp::FinishAction() -- if still rising: flag the creature's type-9 component (+0x10 = 1), drop the underground mode and clear the rising flag; then un-suspend the creature's animation and run the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_UndeadRiseUp@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC470, length 119 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice), CVectorMap::LowerBound @ 0x0040F020 (the
// inlined GetInterface lookup for type 9 = bit 9 of the bitset dword at +0x20, `test ch,2`), CCreatureAction_RiseUp::
// RemoveUndergroundMode @ 0x008B7640 (landed), CThingAICreature::SuspendAI(bool, bool) @ 0x00833A10 (two zero
// pushes), CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, kept a call by the push-ecx frame).
// Shape: the FrameUpdate twin (0x008EC3F0) without the progress test; the component pointer comes from the landed
// out-parameter GetInterface template. The rising flag at +0x118 follows the retail CCreatureAction_RiseUp base
// (0x118 bytes); modelled locally (the generated headers flatten the bases).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};

struct CTCType9 {                           // the creature's interface-type-9 component
    unsigned char _pad[0x10];
    unsigned char m_flag;                   // +0x10
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

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void FinishAction();                    // 0x00859A20
};

class CCreatureAction_RiseUp : public CCombatActionBase { public:   // retail sizeof 0x118
    unsigned char _pad114[4];               // +0x114
    void RemoveUndergroundMode();           // 0x008B7640
};

class CCreatureAction_UndeadRiseUp : public CCreatureAction_RiseUp { public:
    unsigned char m_rising;                 // +0x118
    void FinishAction();
};

void CCreatureAction_UndeadRiseUp::FinishAction()
{
    if (m_rising) {
        CTCType9* component;
        if (PCreature.GetPItem()->GetTC(TCI_TARGETED, &component))
            component->m_flag = 1;
        RemoveUndergroundMode();
        m_rising = 0;
    }
    CThing* creature = PCreature.GetPItem();      // a named local: retail fetches the creature before the argument pushes
    ((CThingAICreature*)creature)->SuspendAI(false, (void*)false);
    CCombatActionBase::FinishAction();
}
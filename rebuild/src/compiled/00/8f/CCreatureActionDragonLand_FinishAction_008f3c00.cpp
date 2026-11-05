// CCreatureAction_DragonLand::FinishAction() -- flag the dragon's interface-0x3a component (bit 0 of +0x50), release this action's animation suspension on a creature, then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_DragonLand@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F3C00, length 111 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap::LowerBound @ 0x0040F020,
//          CThingAICreature::SuspendAI(bool, requester) @ 0x00833A10 (landed UndeadRiseUp shape),
//          CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: `this` lives in ebp (it is pushed as the requester), PCreature's address in edi; the bool out-parameter
//   GetInterface for bit 0x3a (`test dword ptr [eax+24h],4000000h`) with the component flag set by `or byte ptr`;
//   the thing-type byte +0x10 compared with 1; the creature for SuspendAI is a named local fetched before
//   the pushes; the base FinishAction is a direct call.
// Layout: CCombatActionBase (retail 0x114 bytes) with PCreature at +8; nothing else of the class is touched. The
//   0x3a component is not identified (only its flag byte at +0x50 is modelled).
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

struct CTCInterface3A {                     // interface type 0x3a component
    unsigned char _pad00[0x50];
    unsigned char m_flags50;                // +0x50 bit 0 set when the dragon lands
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
    protected: virtual void FinishAction(); public:            // 0x00859A20
};

class CCreatureAction_DragonLand : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_DragonLand::FinishAction()
{
    CTCInterface3A* tc;
    if (PCreature.GetPItem()->GetTC(TCI_MAPWHO, &tc))
        tc->m_flags50 |= 1;
    if (PCreature.GetPItem()->GetType() == 1) {
        CThing* creature = PCreature.GetPItem();
        ((CThingAICreature*)creature)->SuspendAI(false, this);
    }
    CCombatActionBase::FinishAction();
}
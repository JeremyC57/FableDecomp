// CCreatureAction_BeetleTakeOff::FinishAction() -- add creature mode 0x1A on the owner (through its creature-mode manager component), then the base combat-action finish
// Leaf: ?FinishAction@CCreatureAction_BeetleTakeOff@@MAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00982B50, 82 bytes, thiscall, plain `ret`.
// Identity (proven): CCreatureAction_BeetleTakeOff vtable 0x012995EC slot 4 (FinishAction); Ego_r.pdb ?FinishAction@CCreatureAction_BeetleTakeOff@@MAEXXZ (donor 0x009A3140).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020,
//   CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10, CCombatActionBase::FinishAction() 0x00859A20.
// Shape (family K040, representative 0x008C19B0): `if (GetCreature()->GetInterface(&modes, 0x31)) modes->AddMode(0x1A);` (the
//   landed CCreatureAction_FinishFireballSpell::FinishAction 0x008BAC40 GetInterface model: the bitset word test, the lower
//   bound and its end / greater check), then `CCombatActionBase::FinishAction();`.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCreatureModeManager { public:                // interface 0x31
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
    bool AddMode(unsigned long mode);                 // 0x0073AE10
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCreatureActionBase { public:                   // retail sizeof 0xa8
    virtual ~CCreatureActionBase();
    virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                      // slot 4
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;            // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CThing* GetCreature() const { return PCreature.Get(); }
};
class CCombatActionBase : public CCreatureActionBase { public:      // retail sizeof 0x114
    unsigned char _pad0a8[0x114 - 0xa8];
    protected: virtual void FinishAction(); public:                      // 0x00859A20
};
class CCreatureAction_BeetleTakeOff : public CCombatActionBase {
protected:
    virtual void FinishAction();
};
void CCreatureAction_BeetleTakeOff::FinishAction()
{
    CTCCreatureModeManager* modes;
    if (GetCreature()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes))
        modes->AddMode(0x1A);
    CCombatActionBase::FinishAction();
}
// CCombatAction_ControlledStrafeJump::FinishAction() -- base FinishAction, then if the jump added the roll mode, remove mode 70 from the creature's mode manager and clear the flag
// Leaf: ?FinishAction@CCombatAction_ControlledStrafeJump@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085C120, 112-byte slot (97 bytes of code). FinishAction is vtable slot 4 (protected).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
//          (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x31 = bit 17 of the
//          bitset dword at +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0.
// Shape: the landed GetInterface out-parameter template (0x008CB4A0 family): one `push ecx` frame word for
// the lookup key, the interface map addressed once through esi. AddedRollMode (a bool at retail +0x12c,
// PDB +0x144) is cleared whether or not the creature still has a mode manager.
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
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCombatAction_ControlledStrafeJump : public CCombatActionBase { public:   // retail sizeof 0x130
    float RequiredFacing_X, RequiredFacing_Y, RequiredFacing_Z;    // +0x114
    float OriginalFacing_X, OriginalFacing_Y, OriginalFacing_Z;    // +0x120
    bool AddedRollMode;                     // +0x12c
    virtual void FinishAction();
};

void CCombatAction_ControlledStrafeJump::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (AddedRollMode) {
        CTCCreatureModeManager* pModes;
        if (PCreature.GetPItem()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes))
            pModes->RemoveMode(70);
        AddedRollMode = false;
    }
}
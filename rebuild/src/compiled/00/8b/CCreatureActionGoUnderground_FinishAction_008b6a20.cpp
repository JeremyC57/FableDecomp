// CCreatureAction_GoUnderground::FinishAction() -- perform the action if it never ran, resume an AI creature's AI, then the combat action's FinishAction
// Leaf: ?FinishAction@CCreatureAction_GoUnderground@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B6A20, 63 bytes (one int3 at 0x008B6A5F, then the next function 0x008B6A60
//   CCreatureAction_RiseUp::Initialise), thiscall, tail `jmp CCombatActionBase::FinishAction` 0x00859A20.
// Identity (proven, two sources): (1) the only reference is slot 4 (the FinishAction slot: CCreatureActionBase 0x01259D24 slot 4
//   = ?FinishAction@CCreatureActionBase@@UAEXXZ 0x00694430) of vtable 0x0127A894, RTTI .?AVCCreatureAction_GoUnderground@@ :
//   CCombatActionBase : CCreatureActionBase : CBaseObject : CBaseClass; (2) Ego_r.pdb / FableWin.pdb
//   ?FinishAction@CCreatureAction_GoUnderground@@UAEXXZ (public virtual), Ego_r donor order Initialise / FinishAction =
//   retail 0x008B69F0 / 0x008B6A20 (Initialise is this body's mirror: `SuspendAI(true, 0)` after the base Initialise).
//   Byte twin: 0x008B6A90 is CCreatureAction_RiseUp::FinishAction (vtable of its own, chunk 707): two symbols.
// Callees: CCreatureActionBase::HasPerformedAction() const 0x006925A0 (`mov al,[ecx+62h]`, ?HasPerformedAction@CCreatureActionBase@@QBE_NXZ,
//   both PDBs; Ego_r PerformedAction +98), this's virtual slot 3 PerformAction (+0x0C; _purecall in CCreatureActionBase),
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ),
//   CThingAICreature::SuspendAI(bool, void*) 0x00833A10 (?SuspendAI@CThingAICreature@@QAEX_NPAX@Z, both PDBs),
//   CCombatActionBase::FinishAction() 0x00859A20 (?FinishAction@CCombatActionBase@@MAEXXZ).
// Shape: `cmp byte [eax+10h],1` is CClass::Type (+0x10, Ego_r uchar) through the inline FableWin ?GetType@CClass@@QBEKXZ; type 1
//   is the AI-creature class (the creature is then used as a CThingAICreature for SuspendAI); the enumerator's name is in no name set.
// Layout: Ego_r CCreatureActionBase {PCreature +8, PerformedAction +0x62} (retail 0xA8 bytes), CCombatActionBase retail 0x114;
//   engine/CCreatureActionBase.h and engine/CCreatureAction_GoUnderground.h are flattened (vptr as data), so the action classes
//   are restated over engine/CBaseObject.h (FABLE_RETAIL_BASE_OBJECT); CThing : CClass : CBaseObject as the landed
//   CCreatureAction_LeapLong::FinishAction 0x008B74A0 (CClass through the layout-rename include).
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                         // CBaseObject : CBaseClass (IntelligentPointer +4)
#define CClass CClassFields                             // engine/CClass.h is flattened (vptr as data): layout-rename include
#include "engine/CClass.h"
#undef CClass

class CClass : public CBaseObject { public:            // RTTI CClass : CBaseObject; fields as engine/CClass.h
    unsigned char ClassListPos[0x4];                    // +0x08 list<CClass*>::iterator
    unsigned char TypeListPos[0x4];                     // +0x0c list<CClass*>::iterator
    unsigned char Type;                                 // +0x10
    unsigned long GetType() const { return Type; }      // FableWin.pdb ?GetType@CClass@@QBEKXZ (inline)
};

class CThing : public CClass { };                       // RTTI CThing : CClass (members not touched here)
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase {
public:
    void SuspendAI(bool suspend, void* suspender);      // 0x00833A10
};

#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // vtable 0x0129C95C (vptr + PData, 8 bytes; 1-aligned in retail)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    CBaseObjectPointer* PData;                          // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {    // FableWin member set: Get / Set / -> / *
public:
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
};
#pragma pack(pop)

class CCreatureActionBase : public CBaseObject {        // vtable 0x01259D24, retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();                     // 0x00693EF0
    virtual void EditUpdate(float delta);               // slot 2: 0x00692DB0
    virtual void PerformAction() = 0;                   // slot 3: _purecall
    virtual void FinishAction();                        // slot 4: 0x00694430
    bool HasPerformedAction() const;                    // 0x006925A0
    CIntelligentPointer<CThingCreatureBase> PCreature;  // +0x08
    unsigned char _pad10[0x62 - 0x10];
    bool PerformedAction;                               // +0x62
    unsigned char _pad63[0xa8 - 0x63];
};

class CCombatActionBase : public CCreatureActionBase {  // RTTI .?AVCCombatActionBase@@, retail 0x114 bytes
protected:
    virtual void FinishAction();                        // 0x00859A20
public:
    unsigned char _padA8[0x114 - 0xa8];
};

class CCreatureAction_GoUnderground : public CCombatActionBase {    // vtable 0x0127A894
public:
    virtual void PerformAction();                       // slot 3: 0x008B75E0
    virtual void FinishAction();
};

void CCreatureAction_GoUnderground::FinishAction()
{
    if (!HasPerformedAction())
        PerformAction();
    if (PCreature->GetType() == 1) {
        CThingAICreature* creature = static_cast<CThingAICreature*>(PCreature.Get());
        creature->SuspendAI(false, 0);
    }
    CCombatActionBase::FinishAction();
}
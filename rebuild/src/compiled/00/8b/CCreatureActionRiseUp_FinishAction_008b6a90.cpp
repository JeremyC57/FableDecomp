// CCreatureAction_RiseUp::FinishAction() -- make sure the rise-up action was performed; an AI creature gets its AI back (undoing Initialise's SuspendAI); then the combat-action FinishAction
// Leaf: ?FinishAction@CCreatureAction_RiseUp@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B6A90, 63 bytes (int3 padding to 0x008B6AD0), virtual, tail `jmp` to the base FinishAction.
// Identity: the only reference is slot 4 (FinishAction) of vtable 0x01268C9C (RTTI .?AVCCreatureAction_RiseUp@@ : CCombatActionBase :
//   CCreatureActionBase : CBaseObject : CBaseClass; slot 3 = RiseUp::PerformAction 0x008B76A0). Ego_r.pdb + FableWin.pdb
//   ?FinishAction@CCreatureAction_RiseUp@@UAEXXZ; Ego_r donor order Initialise / FinishAction = retail 0x008B6A60 (base Initialise, then
//   SuspendAI(true, 0) on an AI creature) / 0x008B6A90. Byte twin of 0x008B6A20, which is slot 4 of CCreatureAction_GoUnderground's vtable
//   0x0127A894: two classes' FinishAction, two symbols (no ICF in retail), no binding conflict.
// Callees: CCreatureActionBase::HasPerformedAction() const 0x006925A0 (both PDBs ?HasPerformedAction@CCreatureActionBase@@QBE_NXZ; its body
//   `mov al,[ecx+62h]` reads Ego_r PerformedAction +0x62), the virtual PerformAction (slot 3, +0xC), CBaseIntelligentPointer::GetPItem
//   0x00A01B50 (PCreature +8, twice), CThingAICreature::SuspendAI(bool, void*) 0x00833A10 (both PDBs), CCombatActionBase::FinishAction
//   0x00859A20 (?FinishAction@CCombatActionBase@@MAEXXZ, protected; tail jump).
// Shape: CClass::Type 1 = AI creature (`cmp byte [eax+10h],1`, the landed CModeDragonFireBeam::OnDeactivate 0x008DBB00 test); the creature
//   pointer is a named local inside the if block, so its GetPItem call comes before the two argument pushes (an inline
//   `static_cast<..>(PCreature.GetPItem())->SuspendAI(false, 0)` pushes the arguments first: 63v63 with three moved instructions).
// Model: CThing : CClass : CBaseObject from engine/CBaseObject.h (retail opt-in) and engine/CClass.h under the layout-rename include, as
//   the landed sibling FinishAction rows (0x008C0B10, 0x008B74A0). No engine header for CCreatureAction_RiseUp / CCombatActionBase /
//   CThing; engine/CCreatureActionBase.h and engine/CThingAICreature.h / CThingCreatureBase.h are flattened generated layouts.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                         // CBaseObject : CBaseClass (Validate slot 1, IntelligentPointer +4)
#define CClass CClassFields                             // engine/CClass.h is flattened (vptr as data): layout-rename include
#include "engine/CClass.h"
#undef CClass

class CClass : public CBaseObject { public:            // RTTI CClass : CBaseObject; fields as engine/CClass.h
    unsigned char ClassListPos[0x4];                    // +0x08 list<CClass*>::iterator
    unsigned char TypeListPos[0x4];                     // +0x0c list<CClass*>::iterator
    unsigned char Type;                                 // +0x10
    unsigned long GetType() const { return Type; }      // FableWin.pdb ?GetType@CClass@@QBEKXZ (inline)
};

class CThing : public CClass { };                       // RTTI CThing : CClass
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase {   // the CThingCreatureBase .. CThing chain at offset 0
public:
    void SuspendAI(bool suspend, void* suspender);      // 0x00833A10 ?SuspendAI@CThingAICreature@@QAEX_NPAX@Z
};

class CBaseIntelligentPointer {                         // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CCreatureActionBase : public CBaseObject {         // RTTI ... : CCreatureActionBase : CBaseObject : CBaseClass
public:
    virtual void EditUpdate(float deltaTime);            // slot 2 (Ego_r ?EditUpdate@CCreatureActionBase@@UAEXM@Z)
    virtual void PerformAction();                        // slot 3
    virtual void FinishAction();                         // slot 4 (Ego_r ?FinishAction@CCreatureActionBase@@UAEXXZ)
    bool HasPerformedAction() const;                     // 0x006925A0 (PerformedAction +0x62)
};
class CCombatActionBase : public CCreatureActionBase {   // retail sizeof 0x114 (Ego_r 0x12C less the Dinkumware container bytes)
public:
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0x114 - 0x10];                 // +0x10
protected:
    virtual void FinishAction();                         // 0x00859A20 ?FinishAction@CCombatActionBase@@MAEXXZ
};

class CCreatureAction_RiseUp : public CCombatActionBase {    // vtable 0x01268C9C
public:
    virtual void PerformAction();                        // slot 3 0x008B76A0
    virtual void FinishAction();
};

void CCreatureAction_RiseUp::FinishAction()
{
    if (!HasPerformedAction())
        PerformAction();
    if (PCreature.GetPItem()->GetType() == 1) {
        CThingAICreature* creature = static_cast<CThingAICreature*>(PCreature.GetPItem());
        creature->SuspendAI(false, 0);
    }
    CCombatActionBase::FinishAction();
}
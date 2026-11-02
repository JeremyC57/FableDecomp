// CCreatureAction_BalverineLungeAttack::FinishAction() -- the lunge is over: make the balverine pushable by the hero again and drop the hit-on-collision component the lunge added; then the combat-action FinishAction
// Leaf: ?FinishAction@CCreatureAction_BalverineLungeAttack@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CC8A0, 55 bytes (int3 padding to 0x008CC8E0), virtual, tail `jmp` to the base FinishAction.
// Identity: the only reference is slot 4 (FinishAction in every creature-action vtable) of vtable 0x01283A54 (RTTI
//   .?AVCCreatureAction_BalverineLungeAttack@@ : CCombatActionBase : CCreatureActionBase : CBaseObject : CBaseClass). Ego_r.pdb + FableWin.pdb
//   ?FinishAction@CCreatureAction_BalverineLungeAttack@@UAEXXZ; Ego_r donor order FinishAction / Initialise = retail 0x008CC8A0 / the landed
//   Initialise 0x008CC8E0. The catalog label is right.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, twice), the physics component's slot 84 (+0x150)
//   CTCPhysicsBase::SetAsPushableByHero(bool) (FableWin ?SetAsPushableByHero@CTCPhysicsBase@@UAEX_N@Z; retail slot 84 is `ret 4` in
//   CTCPhysicsBase 0x0125D2C4 and in CTCPhysicsStandard 0x00724D00 sets bit 5 of +0x85 = Ego_r PushableByHero; slot 83 reads that bit =
//   IsPushableByHero, slot 82 `ret 8` = ApplyControlledAcceleration -- the FableWin order), CThing::RemoveTC(ETCInterfaceType) 0x004C9840 (both
//   PDBs), CCombatActionBase::FinishAction 0x00859A20 (?FinishAction@CCombatActionBase@@MAEXXZ, protected; tail jump).
// Constants: interface 200 (0xC8) = CTCHitOnCollision (the only CTC vtable whose GetInterfaceType, slot 21, is `mov eax,0C8h; ret`:
//   0x0126E7A4 -> 0x007C8510); the landed Initialise 0x008CC8E0 adds that component and clears the pushable flag through the same slot 84.
// Shape: two GetPItem calls (the accessor is not CSE'd: out of line); the physics TC through FableWin's inline CThing::GetPhysicsTC().
// Model: CThing : CClass : CBaseObject (RTTI) from engine/CBaseObject.h (retail opt-in) and engine/CClass.h under the layout-rename
//   include, PhysicsTC at +0x60 (Ego_r +0x70 less the CVectorMap / list / bitset size differences, as the landed 0x008CCD70).
// Layout: engine/CCreatureAction_BalverineLungeAttack.h is a flattened generated layout (vptr as data, 0x114 = the CCombatActionBase size:
//   no own members), so the polymorphic class chain is modelled here; no engine header for CCombatActionBase / CThing, and
//   engine/CTCPhysicsBase.h is a flattened generated layout (vptr as data, no member functions), so slot 84 is declared locally.
enum ETCInterfaceType { };

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

#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
class CTCPhysicsBase {                                  // vtable 0x0125D2C4
public:
    // slots 0..83 (not called here)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF10(7) VF(80) VF(81) VF(82) VF(83)
    virtual void SetAsPushableByHero(bool pushable);    // slot 84 (+0x150)
};
#undef VF10
#undef VF

class CThing : public CClass { public:                  // RTTI CThing : CClass
    unsigned char   _pad14[0x60 - 0x14];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }   // FableWin ?GetPhysicsTC@CThing@@QAEPAVCTCPhysicsBase@@XZ (inline)
    void RemoveTC(ETCInterfaceType type);               // 0x004C9840 ?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z
};
class CThingCreatureBase : public CThing { };           // (engine/CThingCreatureBase.h is flattened)

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
};
class CCombatActionBase : public CCreatureActionBase {   // retail sizeof 0x114 (Ego_r 0x12C less the Dinkumware container bytes)
public:
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0x114 - 0x10];                 // +0x10
protected:
    virtual void FinishAction();                         // 0x00859A20 ?FinishAction@CCombatActionBase@@MAEXXZ
};

class CCreatureAction_BalverineLungeAttack : public CCombatActionBase {   // vtable 0x01283A54
public:
    virtual void FinishAction();
};

void CCreatureAction_BalverineLungeAttack::FinishAction()
{
    PCreature.GetPItem()->GetPhysicsTC()->SetAsPushableByHero(true);
    PCreature.GetPItem()->RemoveTC((ETCInterfaceType)200);
    CCombatActionBase::FinishAction();
}
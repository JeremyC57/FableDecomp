// NCreatureMode::CModeCreatureIsProjectile::OnActivate() -- remember where the throw starts, suspend the thrown creature's AI and its physics navigator, then the stand-mode OnActivate
// Leaf: ?OnActivate@CModeCreatureIsProjectile@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6690, 70 bytes, thiscall, tail `jmp CModeStandBase::OnActivate`.
// Identity (proven): the only reference is slot 5 of vtable 0x01285728 (RTTI .?AVCModeCreatureIsProjectile@NCreatureMode@@ :
//   CModeStandBase : CTCCreatureModeManager::CModeBase); slot 6 is OnDeactivate 0x008D90B0, slot 3 FrameUpdate 0x008E1670, slot 1
//   GetType 0x008E3640 (`mov eax,39h`, the mode type the landed Recover 0x008E08C0 removes), slot 4 GetAnimFilter 0x008E3650
//   (&Filter, +0x74). Ego_r (0x008179F0) and FableWin (0x02844380) both spell ?OnActivate@CModeCreatureIsProjectile@NCreatureMode@@UAEXXZ.
// Callees: CThing::GetPos() const 0x004C73D0 (?GetPos@CThing@@QBEABVC3DVector@@XZ, 42 bindings), CThingAICreature::SuspendAI(bool,
//   void*) 0x00833A10 (Ego_r / FableWin ?SuspendAI@CThingAICreature@@QAEX_NPAX@Z; landed row of that name), the physics component's
//   EnableNavigator(bool) (CTCPhysicsBase slot 40, +0xA0: the base body 0x006B10E0 is `ret 4`; slots 32-44 line up one-to-one with
//   FableWin's CTCPhysicsBase definition order ResetRHSet .. ReturnToDefaultInteractionFlags by `ret`/`ret 4`/`ret 8`/`mov al,1`),
//   CModeStandBase::OnActivate 0x008621A0 (Ego_r ?OnActivate@CModeStandBase@@UAEXXZ).
// Layout (Ego_r CModeCreatureIsProjectile 0x88 = retail, as the landed Recover 0x008E08C0): CModeStandBase 0x68, StartThrow +0x68
//   (C3DVector), Filter +0x74 (CAnimFilterOfGroup), OldTargetFlags +0x80, PCreature +0x84 (CThingCreatureBase*); CModeBase ParentThing
//   +0x04; CThing PhysicsTC +0x60 (Ego_r +0x70, read through FableWin's inline GetPhysicsTC()).
// Model: CThing : CClass : CBaseObject on engine/CBaseObject.h (retail opt-in) + engine/CClass.h (layout-rename include: flattened);
//   CTCPhysicsBase : CTCBase on engine/CTCBase.h (FABLE_RETAIL_CTCBASE) with its own slots 28-40 in FableWin order;
//   engine/CModeStandBase.h flattens the vptr (header debt), so CModeBase / CModeStandBase are declared here (slots 0-7).
// Shape: the creature pointer is a named local read after the GetPos call (it lands in edi across both calls); SuspendAI's second
//   argument is the PDB default 0.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                                 // CBaseObject : CBaseClass (IntelligentPointer +4)
#define CClass CClassFields                                     // engine/CClass.h is flattened (vptr as data): layout-rename include
#include "engine/CClass.h"
#undef CClass
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                     // CTCBase : CBaseClassNonCopyable, slots 0-27

#pragma pack(push, 1)
class C3DVector {                                               // 1-aligned in retail
public:
    float X, Y, Z;
};
#pragma pack(pop)
class C2DVector;
class CRightHandedSet;
class CAnimateThing;
class CAnimFilterBase;

class CClass : public CBaseObject {                             // RTTI CClass : CBaseObject; fields as engine/CClass.h
public:
    unsigned char ClassListPos[0x4];                            // +0x08 list<CClass*>::iterator
    unsigned char TypeListPos[0x4];                             // +0x0c list<CClass*>::iterator
    unsigned char Type;                                         // +0x10
};

class CTCPhysicsBase : public CTCBase {                         // vtable 0x0125D2C4
public:
protected:
    virtual C2DVector GetOffsetVector() const;                  // 28 0x006B13D0
    virtual C3DVector GetOffsetVector3D() const;                // 29 0x006B13F0
public:
    virtual void SetPosition(const C3DVector&);                 // 30 0x006B0C10
    virtual void ResetPosition(const C3DVector&);               // 31 0x006B01D0
    virtual void ResetRHSet(const CRightHandedSet&);            // 32 0x006B1070
    virtual void SetPositionToNavigateTo(const C3DVector&);     // 33 0x006B1080
    virtual void SetForcedToMovePos(const C3DVector&, bool);    // 34 0x006B1090
    virtual void ResetMovementState();                          // 35 0x006B10A0
    virtual void EnablePhysics(bool);                           // 36 0x006B10B0
    virtual bool IsPhysicsEnabled() const;                      // 37 0x006B10C0
    virtual void EnableGravity(bool);                           // 38 0x006B10D0
    virtual bool IsGravityEnabled() const;                      // 39 0x006B14B0
    virtual void EnableNavigator(bool);                         // 40 0x006B10E0
};

class CThing : public CClass {                                  // RTTI CThing : CClass
public:
    unsigned char _pad14[0x60 - 0x14];
    CTCPhysicsBase* PhysicsTC;                                  // +0x60
    const C3DVector& GetPos() const;                            // 0x004C73D0
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }        // FableWin ?GetPhysicsTC@CThing@@QAEPAVCTCPhysicsBase@@XZ (inline)
};
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase {
public:
    void SuspendAI(bool suspend, void* data = 0);               // 0x00833A10
};

class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        virtual ~CModeBase();                                   // slot 0
        virtual unsigned long GetType() const = 0;              // slot 1
        virtual void AnimUpdate(CAnimateThing&);                // slot 2
        virtual void FrameUpdate();                             // slot 3
        virtual const CAnimFilterBase& GetAnimFilter() const;   // slot 4
        virtual void OnActivate();                              // slot 5
        virtual void OnDeactivate();                            // slot 6
        CThing* ParentThing;                                    // +0x04
    };
};

class CModeStandBase : public CTCCreatureModeManager::CModeBase {
public:
    virtual void OnActivate();                                  // slot 5 0x008621A0
    unsigned char _pad08[0x68 - 0x08];
};

namespace NCreatureMode {

class CModeCreatureIsProjectile : public CModeStandBase {       // vtable 0x01285728
public:
    virtual void OnActivate();
    C3DVector          StartThrow;                              // +0x68
    unsigned char      Filter[0xc];                             // +0x74 CAnimFilterOfGroup
    unsigned long      OldTargetFlags;                          // +0x80
    CThingCreatureBase* PCreature;                              // +0x84
};

void CModeCreatureIsProjectile::OnActivate()
{
    StartThrow = ParentThing->GetPos();
    CThingAICreature* creature = static_cast<CThingAICreature*>(PCreature);
    creature->SuspendAI(true);
    creature->GetPhysicsTC()->EnableNavigator(false);
    CModeStandBase::OnActivate();
}

}   // namespace NCreatureMode
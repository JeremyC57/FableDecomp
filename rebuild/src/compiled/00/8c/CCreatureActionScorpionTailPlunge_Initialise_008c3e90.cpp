// CCreatureAction_ScorpionTailPlunge::Initialise() -- run the combat-action Initialise, set the total time to the creature's animation duration in frames, then perform the action
// Leaf: ?Initialise@CCreatureAction_ScorpionTailPlunge@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C3E90, 68 bytes, thiscall, tail `jmp [vtable+0Ch]` (int3 padding before and after).
// Identity (proven): the only reference is slot 12 of vtable 0x01286494 (RTTI .?AVCCreatureAction_ScorpionTailPlunge@@ :
//   CCombatActionBase : CCreatureActionBase : CBaseObject : CBaseClass); slot 12 of CCreatureActionBase 0x01259D24 is
//   ?Initialise@CCreatureActionBase@@UAEXXZ and of CCombatActionBase 0x01259E44 the callee 0x00857520. Ego_r (0x008FB970) and
//   FableWin (0x02A24250) both spell ?Initialise@CCreatureAction_ScorpionTailPlunge@@UAEXXZ, donor order Initialise /
//   PerformAction / FinishAction (retail slots 12 / 3 / 4: 0x008C3E90 / 0x008C4810 / 0x008C48A0).
// Callees: CCombatActionBase::Initialise 0x00857520 (Ego_r ?Initialise@CCombatActionBase@@UAEXXZ, the qualified base call),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (PDB ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ; PCreature +8),
//   CThingCreatureBase::GetAnimDuration(long) const 0x006627F0 (both PDBs ?GetAnimDuration@CThingCreatureBase@@QBEMJ@Z; `this`
//   is the creature), __ftol2 0x00BFEA70; virtual GetAnimationIndex (slot 7, +0x1C) and PerformAction (slot 3, +0x0C, tail).
// Globals: CMainGameComponent::ConstantFPS 0x01375550 (protected static, read through the PDB inline accessor GetConstantFPS;
//   copied into the `push ecx` frame word for the `fimul`).
// Layout: CThingCreatureBase opaque at its retail size from engine/CThingCreatureBase.h (layout-rename include). CCreatureActionBase retail 0xA8 (engine/CCreatureActionBase.h member list: PCreature +0x08, TotalTime +0x10),
//   CCombatActionBase 0x114. engine/CCreatureActionBase.h flattens the vptr (header debt), so the polymorphic base is
//   declared here with the retail vtable slots 2-12 (names from the PDB virtual list, slot order from vtable 0x01259D24).
// Shape: `TotalTime = (long)(creature->GetAnimDuration(GetAnimationIndex()) * fps)`: the creature is fetched before the
//   virtual call, the frames-per-second long is copied to the frame before the call, the product is fimul'd.
class CMainGameComponent {
protected:
    static long ConstantFPS;                                    // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                                 // CBaseObject : CBaseClass (IntelligentPointer +4)

#define CThingCreatureBase CThingCreatureBaseFields             // engine/CThingCreatureBase.h is flattened (vptr as data): layout-rename include
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase

class CThingCreatureBase {                                      // members not read here: opaque at the retail size (0x160)
public:
    float GetAnimDuration(long animation) const;                // 0x006627F0
private:
    unsigned char Layout[sizeof(CThingCreatureBaseFields)];
};

class CBaseIntelligentPointer {                                 // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                              // 0x00A01B50
private:
    CBaseObjectPointer* PData;                                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return GetPItem(); }
};

class CCreatureActionBase : public CBaseObject {                // vtable 0x01259D24
public:
    virtual void EditUpdate(float);                             // slot 2  0x00692DB0
    virtual void PerformAction() = 0;                           // slot 3
    virtual void FinishAction();                                // slot 4  0x00694430
    virtual bool IsPlayingAnimation() const;                    // slot 5  0x00692850
    virtual long GetAnimDelayTime() const;                      // slot 6
    virtual long GetAnimationIndex() const;                     // slot 7
    virtual long GetAnimationTransitionInTime() const;          // slot 8
    virtual long GetAnimationTransitionOutTime() const;         // slot 9  0x006924C0
    virtual unsigned long GetAnimExtraFlags() const;            // slot 10
    virtual CCreatureActionBase* Clone() const = 0;             // slot 11
    virtual void Initialise();                                  // slot 12 0x00694D10
    CIntelligentPointer<CThingCreatureBase> PCreature;          // +0x08
    long TotalTime;                                             // +0x10
    unsigned char _pad14[0xa8 - 0x14];                          // +0x14 .. (ActionTime .. DependentThings)
};

class CCombatActionBase : public CCreatureActionBase {          // vtable 0x01259E44, retail 0x114 bytes
public:
    virtual void Initialise();                                  // slot 12 0x00857520
    unsigned char _padA8[0x114 - 0xa8];
};

class CCreatureAction_ScorpionTailPlunge : public CCombatActionBase {   // vtable 0x01286494
public:
    virtual void Initialise();
};

void CCreatureAction_ScorpionTailPlunge::Initialise()
{
    CCombatActionBase::Initialise();
    TotalTime = (long)(PCreature->GetAnimDuration(GetAnimationIndex()) * CMainGameComponent::GetConstantFPS());
    PerformAction();
}
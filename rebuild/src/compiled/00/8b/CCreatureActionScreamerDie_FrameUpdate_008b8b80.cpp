// CCreatureAction_ScreamerDie::FrameUpdate() -- fade the dying screamer out: its graphic appearance alpha is (1 - the action's completed fraction) * 255; then the combat-action FrameUpdate
// Leaf: ?FrameUpdate@CCreatureAction_ScreamerDie@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B8B80, 56 bytes (int3 padding to 0x008B8BC0), virtual, tail `jmp` to the base FrameUpdate.
// Identity: the only reference is slot 56 (FrameUpdate in the creature-action vtables, as the landed CCreatureAction_PlayAnimation::
//   FrameUpdate 0x008426E0) of vtable 0x0127B374 (RTTI .?AVCCreatureAction_ScreamerDie@@ : CCombatActionBase : CCreatureActionBase :
//   CBaseObject : CBaseClass). Ego_r.pdb + FableWin.pdb ?FrameUpdate@CCreatureAction_ScreamerDie@@UAEXXZ (public, over the protected
//   ?FrameUpdate@CCreatureActionBase@@MAEXXZ; Ego_r donor order FrameUpdate / Initialise / FinishAction / Clone). The catalog label is right.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8), CCreatureActionBase::GetPercentageComplete() const 0x006925E0 (both
//   PDBs ?GetPercentageComplete@CCreatureActionBase@@QBEMXZ; its body divides the frames run by the total, `fild ; fidiv`), __ftol2
//   0x00BFEA70, NGraphicAppearance::CTCGraphicAppearance::SetAlpha(unsigned char) 0x004BFAC0 (both PDBs), CCombatActionBase::FrameUpdate
//   0x00859D80 (Ego_r ?FrameUpdate@CCombatActionBase@@UAEXXZ; tail jump).
// Constants: 1.0f (0x0122DED8, `fsubr`) and 255.0f (0x01230014, `fmul`), the shared __real@3f800000 / __real@437f0000 pool entries.
// Shape: the appearance pointer (CThing +0x64, FableWin's inline GetTCGraphicAppearance()) is loaded before the percentage call, kept in
//   edi; `(1.0f - x) * 255.0f` gives retail's fsubr / fmul; the (unsigned char) conversion is the __ftol2 call.
// Model: CThing : CClass : CBaseObject from engine/CBaseObject.h (retail opt-in) and engine/CClass.h under the layout-rename include;
//   PTCGraphicAppearance +0x64 (Ego_r +0x74 less 0x10, as the landed 0x008CCD70). No engine header for CCombatActionBase / CThing /
//   CTCGraphicAppearance; engine/CCreatureActionBase.h is a flattened generated layout.
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

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    void SetAlpha(unsigned char alpha);                 // 0x004BFAC0 ?SetAlpha@CTCGraphicAppearance@NGraphicAppearance@@QAEXE@Z
};
}

class CThing : public CClass { public:                  // RTTI CThing : CClass
    unsigned char _pad14[0x64 - 0x14];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance()  // FableWin ?GetTCGraphicAppearance@CThing@@QAEPAVCTCGraphicAppearance@NGraphicAppearance@@XZ (inline)
    {
        return PTCGraphicAppearance;
    }
};
class CThingCreatureBase : public CThing { };

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
    float GetPercentageComplete() const;                 // 0x006925E0
};
class CCombatActionBase : public CCreatureActionBase {   // retail sizeof 0x114 (Ego_r 0x12C less the Dinkumware container bytes)
public:
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0x114 - 0x10];                 // +0x10
    virtual void FrameUpdate();                          // 0x00859D80 ?FrameUpdate@CCombatActionBase@@UAEXXZ (slot 56)
};

class CCreatureAction_ScreamerDie : public CCombatActionBase {   // vtable 0x0127B374
public:
    virtual void FrameUpdate();
};

void CCreatureAction_ScreamerDie::FrameUpdate()
{
    PCreature.GetPItem()->GetTCGraphicAppearance()->SetAlpha((unsigned char)((1.0f - GetPercentageComplete()) * 255.0f));
    CCombatActionBase::FrameUpdate();
}
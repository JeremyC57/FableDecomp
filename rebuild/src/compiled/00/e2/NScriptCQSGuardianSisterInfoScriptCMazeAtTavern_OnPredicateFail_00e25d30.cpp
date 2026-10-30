// NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::OnPredicateFail() -- when the guardian already spoke to the hero, deactivate the active quest
// Leaf: ?OnPredicateFail@CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E25D30, 60 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012E1E28, which the landed
//   NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Alloc 0x00E25C70 stores (site 0x00E25CA6); Ego_r.pdb has ?OnPredicateFail@CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CMazeAtTavern (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 281 DeactivateQuestLater and 655 GetActiveQuestName (CCharString by hidden pointer), ~CCharString 0x0099EAE0.
// Shape: the outer call's vtable is loaded (edi) before the inner GetActiveQuestName call.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_GuardianSisterInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool GuardianSpokeToHero;                           // +0x48
    class CMazeAtTavern;
};
class CQS_GuardianSisterInfoScript::CMazeAtTavern : public CEntityScriptBase {
public:
    CQS_GuardianSisterInfoScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long TalkCounter;                                   // +0x1c
    bool WavedOver;                                     // +0x20
    long HitCount;                                      // +0x24
private:
    virtual void OnPredicateFail();
};

void CQS_GuardianSisterInfoScript::CMazeAtTavern::OnPredicateFail()
{
    if (ParentClass->GuardianSpokeToHero)
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}
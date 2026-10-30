// NScript::CQ_WaspBossScript::CGratefulVillagerSpawn::Main() -- wait (one script frame at a time) for the panicked-villagers scene, then switch on the "Post waspboss killed" category
// Leaf: ?Main@CGratefulVillagerSpawn@CQ_WaspBossScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E10A90, 102 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1).
// Identity (proven): slot 1 (Main) of the RTTI-less entity vtable 0x012E03B8, which the landed
//   NScript::CQ_WaspBossScript::CGratefulVillagerSpawn::Alloc 0x00E109D0 stores (site 0x00E10A06); Ego_r.pdb has ?Main@CGratefulVillagerSpawn@CQ_WaspBossScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CGratefulVillagerSpawn (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("Post waspboss killed" 0x012E03D4), CEntityScriptBase::IsActiveThreadTerminating
//   0x00F35B30; interface slots 7 NewScriptFrame, 720 SetCategoryActivity.
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
class CQ_WaspBossScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool DronesCreated;                                 // +0x4a
    bool QueenHornetAttacks;                            // +0x4b
    bool CutsceneFinished;                              // +0x4c
    bool SaidLineOnce;                                  // +0x4d
    bool PanickedVillagersScene;                        // +0x4e
    class CGratefulVillagerSpawn;
};
class CQ_WaspBossScript::CGratefulVillagerSpawn : public CEntityScriptBase {
public:
    CQ_WaspBossScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18

private:
    virtual void Main();
};

void CQ_WaspBossScript::CGratefulVillagerSpawn::Main()
{
    while (!ParentClass->PanickedVillagersScene) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetCategoryActivity("Post waspboss killed", true);
}
}
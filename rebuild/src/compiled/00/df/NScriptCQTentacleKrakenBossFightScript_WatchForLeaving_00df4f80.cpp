// NScript::CQ_TentacleKrakenBossFightScript::WatchForLeaving() -- script thread: while the hero lives and the mission is still open run frames; then (unless terminated or already succeeded) fail the mission
// Leaf: ?WatchForLeaving@CQ_TentacleKrakenBossFightScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00df4f80, 114 bytes, thiscall, plain `ret`.
// Identity (proven): the landed CQ_TentacleKrakenBossFightScript::DoMission 0x00DF4E00 (bound ?WatchForLeaving@CQ_TentacleKrakenBossFightScript@NScript@@QAEXXZ; Ego_r.pdb has the same name) spawns it as the thread "WatchForLeaving" (functor store at 0x00DF4EFE); the flags are Ego_r.pdb CQ_TentacleKrakenBossFightScript::MissionSucceeded / MissionFailed (PDB +0x5C / +0x5D, retail +0x48 / +0x49).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the game script interface (+0x40): GetHero (slot 70,
//   +0x118, const CScriptThing&) and NewScriptFrame (slot 7, +0x1c); CScriptThing::IsAlive (slot 75, +0x12c).
// Shape (family L143): the `while` is rotated (the hero test before the loop and at its bottom, the flag tests at its
//   head); the closing `if` re-tests the thread and MissionSucceeded.
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_TentacleKrakenBossFightScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;             // +0x40
    void*                 m_pOwner;                 // +0x44
    bool                  MissionSucceeded;         // +0x48
    bool                  MissionFailed;            // +0x49
    void WatchForLeaving();
};
void CQ_TentacleKrakenBossFightScript::WatchForLeaving()
{
    while (m_pInterface->GetHero().IsAlive() && !MissionFailed && !MissionSucceeded) {
        m_pInterface->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating() && !MissionSucceeded)
        MissionFailed = true;
}
}
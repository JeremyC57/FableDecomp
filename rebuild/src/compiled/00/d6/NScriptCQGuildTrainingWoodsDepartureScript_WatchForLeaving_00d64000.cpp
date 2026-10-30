// NScript::CQ_GuildTrainingWoodsDepartureScript::WatchForLeaving() -- script thread: while the hero lives and the mission is still open run frames; then (unless terminated or already succeeded) fail the mission
// Leaf: ?WatchForLeaving@CQ_GuildTrainingWoodsDepartureScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d64000, 114 bytes, thiscall, plain `ret`.
// Identity (proven): the landed CQ_GuildTrainingWoodsDepartureScript::DoMission 0x00D63DC0 spawns it as the thread "WatchForLeaving" (functor store at 0x00D63EC8); the flags are Ego_r.pdb CQ_GuildTrainingWoodsDepartureScript::MissionSucceeded / MissionFailed (PDB +0x5C / +0x5D, retail +0x48 / +0x49).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the game script interface (+0x40): GetHero (slot 70,
//   +0x118, const CScriptThing&) and NewScriptFrame (slot 7, +0x1c); CScriptThing::IsAlive (slot 75, +0x12c).
// Shape (family L143): the `while` is rotated (the hero test before the loop and at its bottom, the flag tests at its
//   head); the closing `if` re-tests the thread and MissionSucceeded.
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
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
class CQ_GuildTrainingWoodsDepartureScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;             // +0x40
    void*                 m_pOwner;                 // +0x44
    bool                  MissionSucceeded;         // +0x48
    bool                  MissionFailed;            // +0x49
    void WatchForLeaving();
};
void CQ_GuildTrainingWoodsDepartureScript::WatchForLeaving()
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
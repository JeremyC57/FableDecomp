// NScript::CV_KnotholeGladeGatesScript::WatchForTermination() -- wait (one script frame at a time) until the gates mission fails or succeeds, then deactivate the active quest
// Leaf: ?WatchForTermination@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9FEF0, 106 bytes, thiscall, plain `ret`.
// Identity (proven): the member function the landed NScript::CV_KnotholeGladeGatesScript::Main 0x00E9FCD0 spawns as
//   "WatchForTermination" (CSpawnedFunc, the pointer stored at 0x00E9FE0C); Ego_r.pdb has
//   ?WatchForTermination@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ.
// Layout: Ego_r.pdb CV_KnotholeGladeGatesScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CCharString 0x0099EAE0; interface slots 7 NewScriptFrame,
//   281 DeactivateQuestLater, 655 GetActiveQuestName. Same shape as the landed CQ_GuildTrainingWoodsMeleeScript::WatchForTermination.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_KnotholeGladeGatesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    void WatchForTermination();
};

void CV_KnotholeGladeGatesScript::WatchForTermination()
{
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}
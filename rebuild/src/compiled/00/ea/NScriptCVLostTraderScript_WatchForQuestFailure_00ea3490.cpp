// NScript::CV_LostTraderScript::WatchForQuestFailure() -- wait (one script frame at a time) until the lost trader or his brother has
//   been killed; unless the thread is terminating, mark the quest failed (g_bQuestFailed)
// Leaf: ?WatchForQuestFailure@CV_LostTraderScript@NScript@@QAEXXZ
// Retail: Fable.exe @ 0x00EA3490, 66 bytes (+ int3 padding). Referenced once as a thread entry (0x00EA3389).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (twice), CGameScriptInterface::NewScriptFrame
// (slot 7, +0x1c) through Game at +0x40.
// Layout: engine/CV_LostTraderScript.h (Ego_r.pdb - 0x14): g_bLostTraderKilled +0x48, g_bTraderBrotherKilled +0x49, g_bQuestFailed +0x4a.
// Shape: the WAIT macro `while (!(a || b)) { NewScriptFrame(); if (terminating) return; }`, the trailing terminate test, then the
// unconditional flag store.
// Identity (Naming NF194, 2026-11-20, q6470): Ego_r.pdb + FableWin.pdb ?WatchForQuestFailure@CV_LostTraderScript@NScript@@QAEXXZ;
//   CV_LostTraderScript::Main 0x00EA2E80 spawns it as "WatchForQuestFailure" (&CV_LostTraderScript::WatchForQuestFailure) and binds
//   this address under that spelling. Was the manifest label CV_BeardyBaldyScript::WatchForQuestFinished, propagated across a
//   byte-identical body (the real one is 0x00E50100), on a local CQuestScriptBase model.
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CV_LostTraderScript.h"
#include "engine/CGameScriptInterfaceBase.h"

void NScript::CV_LostTraderScript::WatchForQuestFailure()
{
    while (!(g_bLostTraderKilled || g_bTraderBrotherKilled)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    g_bQuestFailed = true;
}

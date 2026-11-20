// NScript::CV_BeardyBaldyScript::WatchForQuestFinished() -- wait (one script frame at a time) until BeardyBaldyKilled (+0x68),
// BarberLadyKilled (+0x69) or BeardyBaldyLeft (+0x6a) is set; unless the thread is terminating, mark the quest finished (+0x48)
// Leaf: ?WatchForQuestFinished@CV_BeardyBaldyScript@NScript@@QAEXXZ
// Retail: Fable.exe @ 0x00E50100, 73 bytes. Referenced once as a thread entry (0x00E4FB7F).
// Identity: Ego_r.pdb + FableWin.pdb ?WatchForQuestFinished@CV_BeardyBaldyScript@NScript@@QAEXXZ; referenced exactly once, by
//   CV_BeardyBaldyScript::Main 0x00E4FA20 (store at 0x00E4FB7C under the spawn name "WatchForQuestFinished" 0x012E27B4); it lies in
//   the BeardyBaldy TU range 0x00E4F5B0 (ctor) .. 0x00E50FD0 (CBB_BeardyBaldyMan::Main); the flags it reads are the
//   engine/CV_BeardyBaldyScript.h members (Ego_r layout). Was: ?WatchForQuestFinished@CV_AmbushScamScript@@QAEXXZ (local struct,
//   a label propagated across byte-identical bodies).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (twice), CGameScriptInterface::NewScriptFrame (slot 7, +0x1c)
//   through Game at +0x40.
// Shape: the WAIT macro `while (!(a || b || c)) { NewScriptFrame(); if (terminating) return; }` (rotated: `a` tested
//   at the entry and at the bottom, `b`/`c` at the aligned loop head), the trailing terminate test, the flag store.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CV_BEARDYBALDYSCRIPT_WATCHFORQUESTFINISHED
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CV_BeardyBaldyScript.h"
#include "engine/CGameScriptInterface.h"

void NScript::CV_BeardyBaldyScript::WatchForQuestFinished()
{
    while (!(BeardyBaldyKilled || BarberLadyKilled || BeardyBaldyLeft)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    QuestFinished = true;
}

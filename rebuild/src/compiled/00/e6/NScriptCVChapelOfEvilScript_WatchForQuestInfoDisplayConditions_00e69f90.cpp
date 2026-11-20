// NScript::CV_ChapelOfEvilScript::WatchForQuestInfoDisplayConditions() -- every script frame, check whether the quest-info display state disagrees with "inside the chapel and not attacked" (the retail branch body only re-checks thread termination)
// Leaf: ?WatchForQuestInfoDisplayConditions@CV_ChapelOfEvilScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E69F90, length 81 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CGameScriptInterface slot 7
// NewScriptFrame (+0x1c) through the interface pointer at +0x40.
// Identity: the manifest label CQ_BountyHuntScript::WatchForQuestSuccess is propagated (BSim); the only
// reference is CV_ChapelOfEvilScript::Main @ 0x00E693F0 (spawn at 0x00E697DE), and the flags fit Ego_r.pdb
// CV_ChapelOfEvilScript - 0x14: DisplayQuestInfo +0x55, InsideChapel +0x59, AttackedByHero +0x5a.
// Shape: the condition is the ternary `DisplayQuestInfo ? (AttackedByHero || !InsideChapel) : (InsideChapel &&
// !AttackedByHero)` (both arms share the final `test al,al ; jne`); the taken body is only `if (terminating)
// return;` (no store survives in retail).
// Naming (NF162, 2026-11-20, q6268): was the descriptive WatchForQuestInfoDisplay on a row-local NScript::CV_ChapelOfEvilScript over a
//   local CQuestScriptBase; now ?WatchForQuestInfoDisplayConditions@CV_ChapelOfEvilScript@NScript@@QAEXXZ on the shared
//   engine/CV_ChapelOfEvilScript.h. Two sources: Ego_r.pdb names WatchForQuestInfoDisplayConditions (and no WatchForQuestInfoDisplay),
//   and Main 0x00E693F0 spawns this address under the thread-name literal "WatchForQuestInfoDisplayConditions" 0x012CA7A4 (the landed
//   Main binds this spelling; its only referencer).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_ChapelOfEvilScript.h"

void NScript::CV_ChapelOfEvilScript::WatchForQuestInfoDisplayConditions()
{
    while (!IsActiveThreadTerminating()) {
        if (DisplayQuestInfo ? (AttackedByHero || !InsideChapel) : (InsideChapel && !AttackedByHero)) {
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}

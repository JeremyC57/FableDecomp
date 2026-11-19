// NScript::CQ_HerosOldHouseScript::WatchForBooty() -- watch the hidden booty's digging spot: every script frame, while the hero is away from Oak Bay wait for the region to load again; once the spot is no longer enabled (dug up) point the active quest's card at the second objective (Oak Bay) and stop
// Leaf: ?WatchForBooty@CQ_HerosOldHouseScript@NScript@@QAEXXZ
// Flags: /d2inlT57
// Inline-budget: /d2inlT57 (owner rule 2026-11-08, preference 2; source untouched) -- the ~CScriptThing (HiddenBooty) return sites: retail
//   expands it at the loop-top/bottom terminate test (0x00D8A387) and the `!loaded` terminate test (0x00D8A3B0) and calls 0x004AA840
//   at the inner-wait, after-wait and dig terminate tests and the final return. c2 trace (hand883 inl.py at 0x1078D1AA): callee
//   cost c1 = 60, site scores 44 (f 5.0), 56 (f 1.25), 65 (f 0.625), 78 (f 0.3125), 63 / 63 (f 0.703), end (f 0): the default
//   threshold 74 expands five sites (662 bytes); retail's split needs 56 < T <= 63. Measured: /d2inlT56 DIFFER(561v610),
//   /d2inlT57 .. /d2inlT63 RELOCATION_MATCH, /d2inlT64 DIFFER(655v610); smallest N = 57 (audit sweep T0..T70 on this
//   source: only 57..63 match).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D8A240, length 610 bytes, plain `ret`.
// Shape: FableWin.pdb ?WatchForBooty@CQ_HerosOldHouseScript@NScript@@QAEXXZ (FableWin 0x037BA020): HiddenBooty fetched by script name, then
//   one loop with the new frame at its step and the terminating test first in the body; inside it the Oak Bay wait (test, terminating
//   test, wait loop with the per-frame terminating test, terminating test after it), then the digging-spot test with the objective
//   update and the return. Every terminate return is `return _Fail_Val.Return();` (FableWin calls TRetVoid::Return before each
//   ~CScriptThing; it emits no code in retail). Locals = Ego_d.pdb NScript::CQ_HerosOldHouseScript::WatchForBooty (debug length 833):
//   _Fail_Val (TRetVoid, bprel -13, declared first) and HiddenBooty (CScriptThing, bprel -28).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940,
//   ~CScriptThing 0x004AA840 (out of line at the deeper returns; expanded at the others: CCountedPointer release, operator delete
//   0x00BFE9BC, ~CBaseClass 0x0099A2E0); through Game (+0x40): NewScriptFrame (7), IsRegionLoaded (12), GetThingWithScriptName (72),
//   SetQuestCardObjective (296), IsDiggingSpotEnabled (576), GetActiveQuestName (655).
// Strings: "HiddenBooty" 0x012D50B4, "OakBay" 0x012D31E4, "" 0x0122D70E, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_02" 0x012D508C.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_HEROSOLDHOUSESCRIPT_WATCHFORBOOTY
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_HerosOldHouseScript.h"
#include "engine/TRetVoid.h"
// Retail expands the destructor at the outer returns and calls 0x004AA840 at the inner ones: the inline body, VC7.1 decides per site.
inline CScriptThing::~CScriptThing() { }

void NScript::CQ_HerosOldHouseScript::WatchForBooty()
{
    TRetVoid _Fail_Val;
    CScriptThing HiddenBooty = Game->GetThingWithScriptName("HiddenBooty");
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!Game->IsRegionLoaded("OakBay")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!Game->IsRegionLoaded("OakBay")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (!Game->IsDiggingSpotEnabled(HiddenBooty)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_02", "", "OakBay");
            return;
        }
    }
}
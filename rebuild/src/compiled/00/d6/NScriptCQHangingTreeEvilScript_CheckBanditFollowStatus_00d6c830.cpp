// NScript::CQ_HangingTreeEvilScript::CheckBanditFollowStatus() -- watch the bandits' exit point: once the hero comes within 20 units the bandits follow him until the region switch is over, then the exit point is fetched again in the new region
// Leaf: ?CheckBanditFollowStatus@CQ_HangingTreeEvilScript@NScript@@QAEXXZ
// Flags: /d2inlT62
// Inline-budget: /d2inlT62 (owner rule 2026-11-08, preference 2; source untouched; the landed WatchForBooty 0x00D8A240 precedent) --
//   the ~CScriptThing (CorrectExit) return sites: retail expands it at the eight inner terminate returns and the function end (three
//   blocks after cross-jumping, 0x00D6C9F8 / 0x00D6CA1B / 0x00D6CA3E, the eax / edx / ecx copies) and calls 0x004AA840 at the
//   after-wait return of the outer loop (0x00D6C9DB -> 0x00D6CA7F). Default (T74): that site expanded too, DIFFER(587v607).
//   Measured (no-ATTEMPTS probes, tools/pp.py): T40 / T50 DIFFER(620v607), T57 / T60 / T61 DIFFER(607v607) (a second site called),
//   T62..T71 RELOCATION_MATCH, T72..T74 DIFFER(587v607); smallest N = 62. Plain loop forms tried first: `while (!terminating)`
//   (587v607), `if (!terminating) do .. while (!terminating)` (607v607, return blocks in another order), `for (;;)` with the frame
//   at the end of the body (587v607).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6C830, length 607 bytes, plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?CheckBanditFollowStatus@CQ_HangingTreeEvilScript@NScript@@QAEXXZ. Locals = Ego_d.pdb
//   NScript::CQ_HangingTreeEvilScript::CheckBanditFollowStatus (debug length 880): _Fail_Val (TRetVoid), CorrectExit (CScriptThing).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::operator= 0x008AB980,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CScriptThing 0x004AA840
//   (called at one return, expanded at the others: CCountedPointer release, operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0);
//   through Game (+0x40): NewScriptFrame (7), GetHero (70), GetThingWithScriptName (72).
// Strings: "HTE_BanditExitPoint" 0x012D2228.
// Ego_d line table (C13 lines, carved; debug length 880): 16 lines in the text's order -- the loop line with its terminate test (219),
//   the wait / distance / flag lines (221..230: ShouldFollowHero = true 225 and = false 227 are 7 B each, each wait line carries its
//   trailing terminate test), the inner loop's close (233), the outer loop's terminate test (234) and close (235). Unreproduced: line
//   236 (40 B) follows the endless loop (unreachable; no retail code), as line 96 of the landed WatchForBooty.
// IsDistanceBetweenThingsUnder from the shared engine/fable_script_distance.h (audit: was a local re-declaration; byte-neutral).
// Shape: the generated-script form of the landed TU-sibling WatchForBooty 0x00D8A240 (every terminate return is
//   `return _Fail_Val.Return();`): one loop with the new frame at its step and the terminating test first in its body.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_HANGINGTREEEVIL_CHECKBANDITFOLLOWSTATUS
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_HangingTreeEvilScript.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_distance.h"
inline CScriptThing::~CScriptThing() { }

void NScript::CQ_HangingTreeEvilScript::CheckBanditFollowStatus()
{
    TRetVoid _Fail_Val;
    CScriptThing CorrectExit = Game->GetThingWithScriptName("HTE_BanditExitPoint");
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!ShouldFollowHero) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), CorrectExit, 20.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ShouldFollowHero = true;
                while (!SwitchingRegions) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ShouldFollowHero = false;
                while (SwitchingRegions) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CorrectExit = Game->GetThingWithScriptName("HTE_BanditExitPoint");
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}
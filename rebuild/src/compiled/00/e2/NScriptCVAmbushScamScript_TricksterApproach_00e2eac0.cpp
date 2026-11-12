// NScript::CV_AmbushScamScript::TricksterApproach() -- wait, one script frame at a time, until the trickster is dead / out of health (TricksterKilledBeforeAmbush) or within TricksterApproachProximity of the hero (DoTricksterIntro)
// Inline-split: every terminate check is the statement macro `do { if (IsActiveThreadTerminating()) return; } while (0);` (as Main 0x00E2B090); the dead arm's check sits one `do { } while (0)` deeper and one folded `?:` statement leaves the flag word -- retail's wording of those two constructs is unproven (see Shape)
// Leaf: ?TricksterApproach@CV_AmbushScamScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2EAC0, 363 bytes, thiscall, plain `ret` (three epilogues), no frame pointer, default flags.
// Callees: GetTrickster 0x00E2EC30 (CScriptThing by value, same TU, defined later), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), ~CScriptThing 0x004AA840 (out of line at the near-arm
//   exit only) and its inline expansion (operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0), inline CScriptThing::IsAlive
//   (implementation slot 75, +0x12C); interface slots 7 NewScriptFrame (+0x1C), 70 GetHero (+0x118), 264 GetHealth (+0x420);
//   float pool 0x0129BA3C = 0.0001f (as the same-TU WatchForHeroAttack 0x00E2E520).
// Shape: generated level-script code in the form of Main 0x00E2B090: `for (;;)` with the terminate check at the loop head
//   (VC7.1 inverts the loop: one copy of the check before it at 0x00E2EAD9, one after NewScriptFrame at 0x00E2EB47), a check
//   at the start of each arm and one after the loop. ~CScriptThing is plain inline; VC7.1 decides each exit from c2's
//   static block frequency (docs/pipeline/INLINE_DEPTH_REVISIT.md). Retail expands it at the head, dead-arm, post-loop and
//   scope-end exits and calls 0x004AA840 at the near-arm exit only. Inferred: two things in retail come from code that emitted no
//   instruction of its own, so their bytes prove the mechanism but not the script text:
//   (1) the dead arm's exit is hotter than a loop exit of this form (this source without the extra wrapper matches retail
//       byte for byte under the diagnostic /d2inlT86../d2inlT100, and is 293v363 at the default threshold). An exit one
//       `do { } while (0)` deeper reproduces it at default flags: the dead-arm, post-loop and scope-end sites turn hot together
//       and the near arm stays a call. SpringAmbush 0x00E2F170 opens with the same loop and shows the same split in its dead arm.
//   (2) `mov [esp+0Ch],edi` at 0x00E2EACE (frame 10h for a 12-byte CScriptThing; edi is the zero register of the whole body)
//       is a conditional-destruction flag word that is never read: VC7.1 leaves it when a `?:` with a class temporary in an
//       arm is folded away (a compiled-out debug statement is the usual source). The first statement reproduces the
//       flag and nothing else; its operands are placeholders from this function's own vocabulary.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#include "stlport/fable_containers.h"
#include "engine/CV_AmbushScamScript.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/fable_script_distance.h"
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }

void NScript::CV_AmbushScamScript::TricksterApproach()
{
    true ? (void)0 : (void)GetTrickster();   // (2) reconstructed dead code: leaves the unused flag word
    CScriptThing trickster = GetTrickster();
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (!trickster.IsAlive() || Game->GetHealth(trickster) <= 0.0001f) {
            do {   // (1) one wrapper deeper than the other checks
                do { if (IsActiveThreadTerminating()) return; } while (0);
            } while (0);
            TricksterKilledBeforeAmbush = true;
            break;
        }
        if (IsDistanceBetweenThingsUnder(trickster, Game->GetHero(), TricksterApproachProximity)) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            DoTricksterIntro = true;
            break;
        }
        Game->NewScriptFrame();
    }
    do { if (IsActiveThreadTerminating()) return; } while (0);
}
# Chunk 1010b notes (CGameScriptInterface, the 11 rows chunk 1010 left)

**Every grade and probe ran with `FABLE_HDR_OVERLAY=$A/hdr` exported** (hdr/README.md lists every opt-in; default paths
unchanged -- landed 0x00897FA0 / 0x008930F0 still RELOCATION_MATCH through the overlay). Probes: `tools/probe.sh` (c2prio
`--obj` + `tools/pd.py` masked diff, free); grades: `gall.sh`.

## Results
| Row | Grade | Grades | Lever / residue |
|---|---|---|---|
| 0x0089B5B0 StartScriptingEntity | RELOCATION_MATCH | 1 | shared-header rebase (retail CScriptGameResourceObjectScriptedThing, CTCScriptedControl::IsLocked, CThingAICreature::GetCurrentStateGroupScriptPriority); the creature passed as `*static_cast<CThing*>(pthing.GetPItem())` (Get() calls GetPItem before the script-ID load); the 2-line Info-reload residue was c2's expression-temporary rotation (`c2prio --rotation`: pointer at ecx on the null-handle line) -- reading the ID through FableWin's inline `GetCurrentScriptID()` instead of the member leaves the pointer at eax |
| 0x00896390 OpenChest | RELOCATION_MATCH | 1 | natural nested form: `if (chest.IsAlive()) { ... if (GetTC(container)) { if (useHero) { ... if (SetCurrentAction(...)) { deed; return true; } } else { chestTC->Open(); return true; } } } return false;` |
| 0x008A8C60 EntitySetAsSittingOnFloor | RELOCATION_MATCH | 2 | rebase on new shared headers (CCreatureAction_GetUpFromSitting.h, CAnimFilterAnyGroup.h, NCreatureMode_CModeSitFloor.h, SitDown opt-in); mode-manager pointer uninitialised (retail reads the stale slot); group name by implicit conversion (forwarder forced inline, GetString 0x00415DD0 called); visible noinline CCountedPointer<CModeSitBase> ctor+dtor fix the frame packing and the Info copy |
| 0x00896B60 KickOffQuestStartScreen | DIFFER(437v445) | 1 | instructions identical; exit block placed at the function end (retail: after the first arm, 0x00896BE6, later exits jump back with near jumps) |
| 0x008A93A0 EntityWillTeleportToArea | DIFFER(618v620) | 1 | this/thing tie for ebx: c2prio thing -11 v this -20; thing gets +24 from its two references in the found block (ctor arg + SetCurrentAction this) and +24 from GetPos; re-fetching the creature (diagnostic H) flips it, so retail's block must reference thing once |
| 0x008A9880 ScreenFilterFadeTo | DIFFER(347v363) | 1 | const-0 piece #6 copy-prefers the strength-reduced offset temp #22 (edi); retail initialises the offset separately (`xor edi,edi; jmp; lea esp,[esp]` loop-head block) and keeps 0 in ebx |
| 0x0089C910, 0x0089CE70, 0x008A7A70, 0x008A8260, 0x0088EDC0 | not worked | 0 | local-model priors unchanged (prior/); session time went to the six rows above |

The grade budget was not exhausted on the three parked rows: each has a c2 / layout diagnosis (above) and 10-25 free probes
instead; repeating identical grades adds nothing.

## Identities and callee names (two sources each; LABELS.tsv / NAME_FIXES.tsv)
- 0x004B2FE0 is `CQuestManager::OnQuestStartScreenDisplayed(const CCharString&)` (bool) -- the landed row calls it
  AddActivatedQuestName, a name in neither PDB.
- 0x008AB380 / 0x008AB3D0 / 0x008AA810 are CCountedPointer<NCreatureMode::CModeSitBase> ctor / dtor / DeleteData (landed
  0x008AB380 is named CCountedPointer_008aa810).
- 0x0073A6E0 `CTCCreatureModeManager::GetCurrentMode() const` returns `CModeBase&`; 0x009039D0 is the 4-argument
  CScriptGameResourceObjectScriptedThing ctor; vtable 0x0127261C is CAnimFilterAnyGroup (not CAnimFilterSit).
- 0x008A8C60's mode test is `GetCurrentMode().GetType() == NCreatureMode::MODE_SIT_FLOOR` (10); the sit mode is
  `new NCreatureMode::CModeSitFloor(*creature, anim.GroupName)` (0x38 bytes, the animation's group string -> its CAnimFilterOfGroup).
- 0x00896B60: +0x0C / +0x1C / +0xC0 of CTCQuestCompletionUI are Active / Quitting / QuittingFadeOut (Ego_r order, STLport sizes);
  the 0x1C tutorial category has no known enumerator name (LUAGameflow knows 0x1B QUEST and 0x1D RENOWN).
- Globals used with established spellings: g_pScriptRunner 0x013D2838 (CSuspendableProcessMicrothread*, StopRunning +5),
  g_bQuestFinishScreensEnabled 0x01375741.

## Header debt
- New: CCreatureAction_GetUpFromSitting.h, CAnimFilterAnyGroup.h, NCreatureMode_CModeSitFloor.h, CTCQuestCompletionUI.h.
- CScriptGameResourceObjectScriptedThing.h / CCreatureAction_SitDown.h still default to their flat generated layouts; the
  retail classes are opt-ins.
- NCreatureMode::CModeSitFloor's CAnimFilterOfGroup Filter is opaque (no CAnimFilterOfGroup header).
- CTCQuestCompletionUI's container / string members are opaque byte runs (types in comments).

## Next per unmatched row
- **0x00896B60**: the exit-block placement; ideas not yet tried: a helper-shaped tail (the tutorial as the qualified,
  same-TU GiveHeroTutorial with `__declspec(noinline)` absent but `/d2inlS` raised), or the first arm written as a
  separate inline member; study c2's exit-block layout pass (which predecessor falls into the return block).
- **0x008A93A0**: needs a spelling where the found block references the thing once (retail loads it once into esi:
  `mov esi,[esp+20h] ; push esi ... mov ecx,esi`) -- e.g. a creature value that is not copy-forwarded into thing; or +10 of
  priority on `this`.
- **0x008A9880**: break the copy preference between the constant 0 and the loop offset (retail's offset init is its own
  block with the alignment jump).
- **0x0089C910 / 0x0089CE70 / 0x008A7A70 / 0x008A8260 / 0x0088EDC0**: rebase onto shared headers first (chunk 1010's lesson
  held again here: two of three rebased rows matched within one or two grades).

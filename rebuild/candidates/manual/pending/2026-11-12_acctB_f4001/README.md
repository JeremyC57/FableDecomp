# acctB hand-back, longest-row lane 4001 (1 row matched: 32,057 bytes)

Authored by a Fable 5.2 agent (the owner's "longest reserved rows" lane), re-verified by the acctB local coordinator.
Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00CE7670 `?Main@CGameflowScript@NScript@@UAEXXZ` | RELOCATION_MATCH 32057v32057, test PASS (g++ and VC7.1) | `// Flags: /d2inlT89690` (see review item 1); 14 full-function grades (pass 1: 10, pass 2: 4) + 3 prefix checks, all in `NOTES.md` |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`, flags from the row's `// Flags:` line):
`coordverify/` (grade, both test runs, read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE). Purity
and padded-struct greps are clean: no asm, no `volatile`, no pragma, no forced inline in the row. Grade it WITH the overlay:
without `hdr/engine/CGameflowScript.h` cl.exe was seen spinning under Wine instead of failing fast.

All 7,605 instructions and the four jump tables are equal; the agent paired all 26 call targets with retail one to one
(25 already in `reloc_bindings.tsv` under the same spelling; the 26th, 0x006E7A60, gets its first binding here as
`??0CScriptGameResourceObjectBase@@QAE@XZ`, the landed row).

## Review items for landing

1. **`/d2inlT89690` (inline budget), with an `// Inline-budget:` line in the row.** The credits cut-scene block at
   0x00CEE906 has static frequency 6.5e-5; retail expands four callees there (c2 trace scores 63439, 67814, 72189, 89689)
   and calls the others (next score 164066). T89689 still calls `operator=` (32037v32057, first difference 0x00CEE922);
   T89690 matches and flips no other inline site in the function. A control-flow spelling was tried first (g10) and
   cannot raise the block's frequency by the factor of about 1000 needed; forced header opt-ins (g08, g11) cannot expand an
   implicit destructor. Why retail's c2 saw this block as hotter is not known. The value is large; whether a value of
   that size is acceptable under ASM_RULES' inline-budget wording is your call. The flag has to reach the catalog
   CompilerFlags.
2. **Headers** (`hdr/README.md`): new `engine/CGameflowScript.h` (class model at retail offsets, CoreQuestWaiting +0x48,
   SavedScriptNames +0x4C, SavedCardDefNames +0x58, sizeof 0x64; Ego_r layout + retail bytes of Main and the landed
   OnPersist / CoreQuestReminder) and one new opt-in in `engine/CScriptGameResourceObjectBase.h`,
   `FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTBASE_OUTOFLINE_CTOR` (declaration only, default text unchanged; retail calls the
   COMDAT 0x006E7A60 at 0x00CEE967, 0x00D315EC and 0x00D3173E).
3. The landed siblings 0x00CEF3B0 and 0x00CEF8E0 still carry local class models and can be retyped with the new header.
4. The row declares `EScriptAIPriority`, `ETutorialCategory` and the three `AddLogbook*` functions locally, as the landed
   script Mains do.
5. Manifest oddities seen (not edited): row 0x00CEF550 lacks the `NScript::` module prefix; row 0x006E7A60 has a
   propagated parameter type `C3DAnimationUtil *`.

## Behaviour test

`rows/00ce7670.test.cpp` is generated (agent-side `tools/mktest.py`): the expected order (729 entries: calls with
arguments, frames, terminate checks, member stores) comes from the retail disassembly in address order, not from the
source. Two full runs (one per side of every branch), a start at each of the 36 handled stage values, the four
enumerators without a case, and a terminate request at every check of both runs.

## Retail v Aeon's Lua port (for the script-recovery side)

16 of 35 stages have identical calls. Real disagreements, per section in `NOTES.md`: the Lua has no case for stage 865;
retail runs the frame FIRST in the two guild-master message loops; retail tests only `IsAlive()` in Training and its
teleports take a third argument `false`; the credits sequence differs (`StartScriptingEntity` / `StartMovieSequence` /
`PauseAllNonScriptedEntities` / `RunCutsceneMacro`); a game loaded at 2800 only runs the frame loop.

## How to land

`rows/00ce7670.cpp` + `rows/00ce7670.test.cpp` with the two `hdr/engine/` files, the flag in CompilerFlags, then
`payload.json` through your usual vland. `LESSONS.md` has the method for very long generated functions (machine-made
draft + patch file, whole-function grades only) and the switch / loop / `_Fail_Val.Return()` spellings.

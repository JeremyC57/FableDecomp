# ESCALATE 0x00CF4EC0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main (6164 B), AUTH12 2026-11-22

## Best source
`00cf4ec0.cpp` (= p6d): stock VC7.1, no flags, overlay `hdr/` (FABLE_HDR_OVERLAY=<this dir>/hdr).
Grade: **DIFFER(6180v6164)**, mdiff.py 102 masked instructions (start: c0 DIFFER(6244v6164), 142 by AUTH10's count).
Retail's frame size (sub esp,0xe4) and exit-pad grouping now match.
**Oracle (forced readout, NOT a grade, not in STATUS):** `fv.sh` on `00cf4ec0.cpp` (stock flags, own overlay, ROW/LEAF set) forcing
only `??1CScriptGameResourceObjectMovieBase` occurrences 8..14 to expand (`inl/fm.json`, 7 forced) gives 6164v6164, and gobj.py
grades that object RELOCATION_MATCH (`inl/fv_best.log`; the 13 mdiff hunks left are masking artefacts and jump-table data). So the text is
proven up to those seven movie-dtor decisions: no other residue.

## Step 1: the port (grade after the port)
p1 = c0 with:
- `engine/ETextGroupSelectionMethod.h` and `engine/fable_script_distance.h` in place of the local enum and declaration;
- the three bare `__forceinline` definitions made plain `inline`.

It grades DIFFER(6176v6164). Plain inline leaves 21 `??4CTimer` calls and a called ~CIsThingAlive.

Devices then added, all from family precedent:
- p2: + /d2inlS15, the fan siblings' flag: 6064.
- p3: + overlay opt-in `FABLE_RETAIL_CTIMER_FORCEINLINE_SET_GET` (hdr/engine/CTimer.h, FABLE_FORCEINLINE operator= / GetTime, per ruling
  2026-11-21 (a)): 6096, masked 321.
- p4: + the 0x00DE6C20 predicate device (`FABLE_RETAIL_CPREDICATEBASE_NOVTABLE`, `inline_depth(0)` region round the CIsThingAlive
  ctor/dtor): 6096, masked 294.
  - This fixes AUTH10's top hunk (the SetPredicate(CIsThingAlive(Me)) temporary): copy 0x004ABE90 and ~CScriptThing 0x004AA840 are now
    called inside the expanded predicate, with no base vtable store, as in retail.

## Step 2: Ego_d corrections (eglocals / eglines against odall10 VC10 /Od, fablelib preset)
Calibration: the 118-B Ego_d lines are `if (T) return _Fail_Val.Return();` (18+100). Line 712 (140 B) is
SetPredicate + NewScriptFrame + `if (T) return` (82+27+18+13).
- p5: Ego_d locals.
  - `TRetVoid _Fail_Val` (BP -69), so every return is `return _Fail_Val.Return();`.
  - Names: OakValeFan, seh_OakValeFan, seh_BowerstoneFan, seh_HookCoastFan, ChatLevel.
  - One `long Conversation` per switch block (blocks of len 2811 and 1611). HLStyleConversationIndex only in the loop-1 hit else-arm (block of len 530).
  - Grade: no byte change (6096).
- p6: two Ego_d size misfits fixed.
  - The ChatLevel lines are 9 B in Ego_d (mov/add/mov), so `ChatLevel++` at 20 sites (`= 12` kept in loop-1 case 13, which is 7 B). Retail
    keeps `mov ebp,N`.
  - After loop 2, Ego_d has 962:118 and 963:278, so the generated ending is
    `if (T) return; while (!false) { NewScriptFrame(); if (T) return; } if (T) return;`. VC10 gives 118 / 278 exactly, as in landed
    0x00D706E0 / 0x00D2D930. This fixes the final-loop rotation (retail: jmp to the test, no duplicated test).
  - VC10 total 11912 v Ego_d 11940.
  - Grade with S15: DIFFER(6088v6164), masked 270.
- p6d: p6 without /d2inlS15: DIFFER(6180v6164), **masked 102**. See below for why.
- **Open Ego_d misfit (an Ego_d contradiction, so the text is NOT yet fully Ego_d-consistent):** each fan hit block is 5 B larger in Ego_d
  (541 v 536 in loop 1, 561 v 556 in loop 2; one macro line each). These are the blocks that own the seven unresolved movie sites.
  One odall10 round, six spellings in the six slots (`od/v5.cpp`, `od/v5.sizes`), all negative: `> 0` 536, `seh.IsPerformingScriptTask()
  == true` 537, `!(... <= 0.0f)` 536, `> 0.0` 556, `0.0f < ...` 556, `!= false` 556. 5 B at /Od is a near jmp, an argument-less call or
  a push imm32; still open.

## Inline readout (AUTH10 inl_force.py with an empty force list: READOUT ONLY, never a grade)
`inl/p6.json` (S15), `inl/p6s14.json` (S14), `inl/p6s9.json` (S9). In every row of the decision table, a callee is always-inlined (no
score) exactly when **c1 - 4 <= S**. Examples:

| callee | c1 | always-inlined at |
|---|---|---|
| ~CTimer | 14 | S >= 10 |
| CTimer ctor | 17 | S >= 13 |
| implicit movie dtor | 19 / 22 | S >= 15 |
| CIsThingAlive ctor | 20 | never (scored even at S15) |
| ScriptedThingBase ctor | 20 | never (scored even at S15) |
| ~CWideScreenMagicPauseEntities | 7 | always |
| ~CScriptGameResourceObjectBase | 5 / 8 | always |

- **~CTimer at default S9 is scored, and its 25 sites give retail's split exactly.**
  - Expanded: k0..k10 (scores 48/19/22/26), which are the loop-1 returns.
  - Called (0x00CD4470): k11..k13 (83/588/588), the post-loop-1 and loop-2 head returns.
  - Expanded: k14 (73, just under T74), the loop-2 Oakvale wait. Retail 0x1076 goes to its expanded pad.
  - Called: k15 onward, as at retail 0x1092 / 0x1121 / 0x11C6 / 0x113D / 0x11E2 / 0x12DE and the final exit 0x1773.
- **The movie dtor at S9:** k0..k7 expanded, k8..k14 called (99, 798, 131, 798, 177, 798, 74): the loop-2 hit-scene sites. Retail
  expands every movie site (census: no 0x006E7B80 call; 0x006E7AB0 x4, 0x0099A430 x4).
- **No (S, T) gives both.**
  - S >= 15 always-inlines the movie dtor (good) but also ~CTimer (bad). That is p6, which never calls 0x00CD4470 and has no ChatTimer slot,
    so its frame is 0xe0.
  - S <= 9 scores both. ~CTimer is then right and the movie dtor is wrong.
  - Scores are proportional to c1 at a shared site, and the movie dtor costs more than ~CTimer. So no T can expand the movie dtor (798) while
    calling ~CTimer (83..588) at the same returns.
  - Sweeps: T30..150 with S15 is never better; S10..30 on p4 gives only the two shapes.
- **What the readout implies about retail:** default S9 (no flag), and a movie dtor whose cost is c1 <= 13 (ours 19/22), so that it is
  always-inlined while ~CTimer (c1 14) is scored. Our implicit dtor body is the PImp `CCountedPointer<MovieBase>` dtor (declared-only
  specialization, called 0x006E7AB0) plus the GROBase dtor (inline: vtable store + `call 0x0099A430`, c1 5/8). Retail's expansion has the
  same instructions, so the cost difference has to be in how the helper bodies are declared. That fits the advisor's own warning that
  inline scores depend on helper body sizes.

## Remaining diff in p6d (udiff, trimmed)
- Loop-2 pads.
  - Retail 0x1702 / 0x174B: `call 0x006E7AB0; mov [esp+0x38],0x0126008C; call 0x0099A430`.
  - Ours 0x1727 / 0x174F / 0x1768 / 0x12BA: `call ??1CScriptGameResourceObjectMovieBase` (0x006E7B80).
- Ours has one extra expanded pad at 0x0EF3..0x0F04 (pause + movie + DeregisterTimer).
- Frame slots of the movie and ChatTimer are assigned differently (ours ChatTimer [esp+0x3c], retail [esp+0x4c]). This follows from which
  sites take the movie's address.
- 0x0833 / 0x12ED: the jump-table operands differ only by relocation.

## Other negatives
- p7: the 0x00D17A80 budget-inliner device (`/Og- /d2vol10` + `#pragma optimize("g", on)`): DIFFER(6340v6164), mdiff 1462.

## Finding on the family flag
The three fan Mains (0x00CF2B70 BowerstoneFan, 0x00CF3D20 HookCoastFan, this row) are members of one CQ_ArenaHoldingScriptScript TU
(contiguous 0x00CF0780..0x00CF68C0), so retail compiled them with one S. This row calls ~CTimer 0x00CD4470, which S >= 10 cannot give
(~CTimer c1 14 is always-inlined there); so retail's S was the default 9, and the siblings' landed `/d2inlS15` is a fit for the same gap
seen here: the implicit movie dtor expanded at cold sites under stock S. All three rows show it; (a) below is the lead, (c) is weak.

## Question for the advisor
Retail matches "S9 for ~CTimer, S15 for the implicit movie dtor" in one function. Which device is admissible?
- (a) A shared-header opt-in that lowers the implicit movie dtor's c2 cost to c1 <= 13 without changing its bytes, e.g. a different
  declaration of the PImp `CCountedPointer<MovieBase>` dtor or of the GROBase dtor. It would be found by readout, then graded plainly
  under stock S9. Acceptance: the family 0x00CF2B70 / 0x00CF3D20 also match WITHOUT their /d2inlS15 (that is the check that the
  cost is the original one).
- (b) Keep S15 and add a scoped `inline_depth(0)` region round the 14 retail-called ~CTimer return statements, as landed 0x00D29880 has
  regions inside a body.
  - Doubts: the region would also call the movie dtor at the loop-2 hit returns, which retail expands.
  - And the "one statement per region" ruling (0x00E5E350).
- (c) Owner packet: a proven per-callee threshold tie. No plain text on the current headers can give it.

Which of these, and should (a)'s readout hunt be pre-registered with the two fan siblings (without /d2inlS15) as its acceptance set?
Also: CTimer operator= / GetTime forcing. `engine/CTimer.h`'s own wave-8 contract says rows define the inline bodies after the include
(98 landed rows do it row-locally with FABLE_FORCEINLINE); ruling 2026-11-21 (a) says such bodies go in the shared header as add-only
opt-ins. I used the overlay opt-in `FABLE_RETAIL_CTIMER_FORCEINLINE_SET_GET` (hdr/, needs header claim). Which wins?
Housekeeping: the repo working tree has uncommitted changes (HANDOFF.md, CCountedPointer.h, NUISystem rows, ...) that are not AUTH12's;
AUTH12 wrote nothing into the repo.

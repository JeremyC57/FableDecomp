# wfE3 NOTES (acctC lane; rows 0x00E50FD0, 0x00E3BB70, 0x00E4B5B0, 0x00F1EED0)

## Setup facts
- hdr/ = wfD2 overlay + engine/CV_BeardyBaldyScript.h (new header, byte-copy of the H40i pending overlay; repo has none) +
  engine/CV_BanditTollScript.h (repo header + the H40i `FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT` blocks: CheckForBanditTollPaid,
  SetUpBanditToll, Should_I_Flee decls; diff vs repo = added lines only).
- tools/: g.sh <addr> <src> (grade), sw.sh (flag sweep), inl.sh (c2 inline trace; FN= function), it.py (trace table), vs.sh (virtual-slot
  call sequence retail v ours), ct.py (call targets), cmp.py / an.py / sk.py (listings).
- Anniversary PDB S_REGREL32 locals of the four rows: tmp/anniv_rows.txt.

## 0x00F1EED0 CQ_ArenaScript::PlayWave (open)
- Identity: Ego_r ?PlayWave@CQ_ArenaScript@NScript@@QAEXXZ (0x00CF07F0) + FableWin (0x0364F420); the matched Arena Main (0x00F0FB70,
  RELOCATION_MATCH) calls 0x00F1EED0 at 0x00F13609 as PlayWave(). Extent 9262 bytes confirmed (jmp 0xF2119D at 0xF212F9, int3 pad, next
  function 0x00F21300 = landed GivePrizeFund).
- Locals (Anniversary PDB): SmallEnemySpawnPoint, BigEnemySpawnPoint, DoorTimer, AllBigSpawnMarkers, AllSpawnMarkers, FurthestArenaSpawn,
  CurrentEnemies, MultipleBigEnemySpawnPoints (long), CreatureCounter / MaxCreatures (long[3]), NextCreature (x3), CreatedObject,
  random_vector, spawn_pos, speech_movie, PauseMyEntitiesPlease, seh_Hero. DESCRIPTIVE (register-only, no PDB name): NumSpawnMarkers,
  NextSpawnUp, NextSpawnDown, SpawnUp, CreaturesLeft, CrowdLevel, reward, answer, conversation, i/j/k.
- Draft differ/00f1eed0.cpp (first hand draft): DIFFER(11525v9262). Inline decisions: ~vector<CScriptThing> (0x008AC970) expanded at the
  i/j-loop T exits (ours f 0.515 / 1.288, score 72 / 59) where retail calls; ~CTimer expanded at f~0.08 exits retail calls (212F0 pad);
  /d2inlT sweep: T54 9365, T60 9474, default = T74. Block layout: our counter loop (AddQuestInfoCounterList) is laid right after the
  first-wave branch; retail lays it after the creature loops (first-wave branch ends `jmp 0x00F1FF46`). Swapping the if/else, while-loops
  for i/j, moving the CreatureCounter/MaxCreatures declarations: byte-identical.
- Opt-ins used: FABLE_RETAIL_C3DVECTOR (RotateXY decl), movie OUTOFLINE_CTOR/DTOR and scripted-thing OUTOFLINE_CTOR (single sites, retail
  calls 0x006E7B60 / 0x006E7B80 / 0x007E72A0), FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION (retail calls 0x00415D70). Not
  IMPLICIT_ASSIGNMENT (retail calls operator= 0x008AB980; Main expands it).

## 0x00E50FD0 CBB_BeardyBaldyMan::Main (open, 22 hunks, DIFFER(9869v9885))
- Identity: catalog + earlier lanes (Ego_r/FableWin ?Main@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@EAEXXZ); not re-proved this session.
- What moved it (99 masked lines -> 22 hunks): main loop `for (; !false; Game->NewScriptFrame()) { BB_T; ... }` (the landed
  CV_SwordInTheStone crowd Main 0x00ED5F50 shape) instead of the old fitted `for (; !T && true; NSF)`. With it EVERY ctor/dtor
  call-or-expand decision is retail's with no flag and no region (phase-0 movie inline_depth region no longer needed; the
  attack-scene exits: movie expanded at the START-loop exits, called at the speech exits; seh_me expanded at both loop exits).
  Also fixed: bare `__forceinline` -> FABLE_FORCEINLINE (CTimer::GetTime, CSpawnedFunc1 specialization). Dropping the
  CSpawnedFunc1 specialization (header ctor) = 89 / 38 hunks: keep it.
- Left (root cause): retail's SECOND entry zero store `mov [esp+34h],ebp` (0x00E50FE8; the first, 0x00E50FDF, stores the
  conditional-temp flags home ebx to the same final frame word +0x34). Ours stores only the flags home, at +0x48; so the
  coalesced temp slot (flags home / CCharString("") temp 0x00E510A9 / "M_BB_ExitPoint" temp 0x00E533B9) sits behind speech /
  phase movies instead of in front, and the 4-byte shift moves three alignment fillers (0x00E51109, 0x00E511CA, 0x00E52C0E).
- Same residue family as 0x00E3BB70 (below): both rows have a function-scope default-constructed CCharString passed to a Msg*
  out-parameter (unusedText / item, ctor 0x0099E4B0); TollBandit (no such local) has a single entry store. Not tried yet: a
  source shape where that local is NOT default-constructed at function scope.

## 0x00E3BB70 CMadame::Main (open, DIFFER(10133v10149) /d2inlT69, 5 masked hunks)
- Changes: landed inline `CTimer::~CTimer()` defined (retail expands DeregisterTimer at two exits, calls 0x00CD4470 once:
  draft had it declared only -> 10053, now length-exact without the flag); scoped inline_depth(0) round the chit-chat movie
  declaration kept (retail calls the ctor 0x006E7B60 at 0x00E3C8BE); the draft's region round the arm's closing `}` DROPPED
  (it also outlined the pause dtor, retail expands it at 0x00E3CA98); `/d2inlT69` (window T69..T70, T71 = default 10181)
  calls the chit-chat ~movie (tail-merged with the work-scene end at 0x00E3CAAB) and keeps the deeds / beer / attacked ends
  expanded. Second loop as `for (; !false; NSF) { T; ...}` = byte-identical to `while (!T) {...; NSF;}`.
- Left: retail's entry store `mov [esp+9Ch],edi` (0x00E3BB81) into a frame word that NO other instruction references (ours
  reserves the same word, frame 0x180 both); the 4-byte shift moves four alignment fillers. Probes (all at T69, no store):
  function-scope `long answer = 0` (two placements), `long answer;` + assignments, `long chat=1` / `CCharString item` / timer
  declaration moved to the top.

## 0x00E4B5B0 CTollBandit::Main (open, DIFFER(10610v10594))
- Header: re-applied FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT (hdr/engine/CV_BanditTollScript.h). Call targets identical to
  retail. Residue = frame-slot order (the whole frame below +0x40 permuted), eax/edx picks in expanded pause dtors, the timer
  index held in ebp at 0x00E4C5B5 / 0x00E4D815 (ours reloads), alignment fillers. ~CTimer stays declared-only (retail calls
  0x00CD4450 / 0x00CD4470). md.py (difflib) misaligns this row; use tools/md2.sh (GNU diff -d, MASKF=1 masks frame offsets).
- PlayWave update (later): scoped inline_depth(0) round the two small-spawn `CScriptThing NextCreature;` declarations,
  `CScriptThing CreatedObject;` and `CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);` fixes every ctor site (retail
  calls 0x006E7B40 at 0x00F1F7D8 / 0x00F1FB72 / 0x00F20D1A and 0x00CBE09A at 0x00F20F5C, expands the big-spawn NextCreature
  ctor 0x00F1F4DF); with /d2inlT53..54 the row is 9253v9262 (default 11534). Left: (1) block layout: the quest-counter loop
  (AddQuestInfoCounterList) is placed right after the first-wave branch in ours, after the creature loops in retail (first-wave
  branch ends `jmp 0x00F1FF46`); its exit pads follow; (2) the FadeScreenOut CRGBColour ctor is CALLED at T54 (retail expands
  it, 0x00F21107; Main expands it under default T); (3) the countdown's first exit 0x00F1F059 (retail expands ~CTimer).
  The CScriptThing operator= is called (0x008AB980) here although Main expands it (Main defines IMPLICIT_ASSIGNMENT): the TU
  difference is an inline decision; the row declares it (default header).
- TollBandit update: wrapping `CTimer call_timer;` and everything after it in one `{ }` block makes the row length-exact
  (10594v10594, 162 hunks; FITTED) -- the state-1 temporaries can then share retail's low slots. Wrapping the state-1 loop in its
  own block instead = byte-identical to the draft. Still open: the frame permutation from the first statement (flags home
  0x30 v ours 0x24; AddSpawnedFunction temporaries) -- needs a frame-packer replay, not a spelling.

## Next steps for a successor (priority)
1. 0x00E3BB70 / 0x00E50FD0: find the construct behind the extra entry zero store (both rows otherwise retail-exact at the
   decision level). Ideas not yet tried: the default-constructed CCharString local (item / unusedText) spelled differently
   (e.g. declared later / in the scope of its Msg* use: retail constructs it at entry, so the declaration position is evidenced);
   compare with any LANDED row whose retail entry has two zero stores (scan exe prologues for `mov [esp+X],reg` x2).
2. 0x00F1EED0: block layout of the quest-counter loop; then a regions-only (no /d2inlT) variant so the TU flags equal Main's.
3. 0x00E4B5B0: frame-packer replay against retail's slots.

## Successor session (wfE3 #2) -- no row closed; measurements that narrow the open residues
### 0x00E3BB70 / 0x00E50FD0 entry-store residue ("V") -- what it is NOT (all measured)
- Ours /FAs (tools/fa.py, tmp/fa_m0.asm): retail's extra word [esp+9Ch] at 0x00E3BB81 = frame -244 = the slot ours gives to the
  ATTACKED / ATTACKED_PIMP CCharString temporaries ($T8301 / $T8392, lines 198 / 298). So V is a 4-byte node packed with late temps,
  stored 0 at entry and never read: a dead store c2 keeps. In mini TUs (probe/t/t1..t13, tools/fat.py) c2 keeps exactly such
  stores only for the FE conditional-temporary flags word (`$T = 0` at entry, even when the flags live in ebx and the home is unread).
- One flags word per function: two ||-expressions, a new-expression + ||, a forceinline/inline callee with its own conditional
  temps (bits merge into the caller's word), a conditional temp of a class with an empty inline dtor -- always ONE word, one store.
- Landed CBordelloLady::Main 0x00E3EB10 (same TU, same attacked `||` with CCharString temps, MATCH) has ONE entry store -> V is
  not from the attacked conditions. TollBandit (single store) and Lady lack the function-scope default CCharString out-param;
  Madame (PresentedItem) and BB (regionName, Anniversary PDB names) have it -- the correlation stands, cause still unknown.
- Probes on the real source, all byte-identical to the baseline (no store): `CCharString item = CCharString();`; dropping
  FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (diag, 10117, still no store). CDEFSTRING_OUTOFLINE_CONVERSION is unused by Madame.
- BB local name: Anniversary PDB calls `unusedText` `regionName` (first local), `seh_me` `sehMe`; Madame's `item` is `PresentedItem`,
  `timer` is `TurnAndFaceTimer` (weak third source; rename when the row closes).
- Next idea not tried: c2 frame-packer trace (MATCHING_PLAYBOOK "Frame packer"; tools in
  rebuild/candidates/manual/pending/2026-11-08_linux_hand886_wip/tools8/fr phook.py + pack.py) of ours, to see whether ours has a
  node that is created but loses its store (then V is ours' node with the store eliminated, not a missing node).
### 0x00F1EED0 PlayWave
- Call-target diff (tools/ct.sh): ours still expands ~vector<CScriptThing> (free / 0x99A2E0 / 0xBFE9BC sequences) at exits where
  retail calls 0x008AC970 (15 calls in retail, 0 in ours at T54) -- the residue is NOT only block layout; ours calls 0 x 0x00CD4470
  where retail calls it once. Countdown: retail keeps the FIRST AR_COUNT post-loop exit copy (0x00F1F059, ~CTimer expanded) and the
  line-84 `if (T) return;` jumps forward to it; ours keeps the line-84 copy and the post-loop exit jumps back (cross-jump direction).
### 0x00E4B5B0 TollBandit -- not re-opened beyond the baseline grade (162 hunks): pure frame-slot permutation; per MATCHING_PLAYBOOK
  the packer orders nodes by size then IL weight and sorts slots by density, so the lever is the node set / weights (temporaries
  added or dropped by spelling), measurable only with the phook trace.

## Third session (wfE3 #3)
### V (0x00E3BB70 / 0x00E50FD0 second entry store): first construct that reproduces it (mini TUs, probe/u/)
- VC7.1 opens a SECOND conditional-temporary flags word once 32 bits are used (probe/u/c16: 16 x `f("A")||g("B")||g("C")` = 48
  bits -> two entry `$T = 0` stores). Bits are numbered per function and never reused.
- Conditional temporaries of a class with an EMPTY inline user dtor (`~E() {}`) still take bits; c2 then deletes every bit op,
  leaving only the entry `$T = 0` store of their word. probe/u/v32.cpp (32 x `if (z() && e("E"))` then the S `||`): two entry
  stores, the S bits are 1/3/7 in word B = exactly retail's picture (word A all-vanished -> the dead store V; retail Madame's
  attacked bits are 1,2,4 / 8,16,32, BB's new-expression bits 1,2,4 in ebx). probe/u/w3 (16 x two-temp `&&`) likewise.
- So retail very likely has >= ~29..32 conditional temporaries of an empty-dtor (or trivial-in-ours) class that ours lacks
  (ours: 6 bits in Madame, 3 in BB, all CCharString). Lady (one store) / TollBandit (one store) must stay under 32.
  Counts per row (preprocessed): GetHealth (MD_SAY) Madame 22, BB 24, Lady 11, TollBandit 13 -- the only construct count that
  separates the two-store rows from the one-store rows (2 bits per SAY would fit all four). Unproven: which temp it is.
### 0x00E3BB70 CMadame::Main -- RELOCATION_MATCH (src/00e3bb70.cpp, /d2inlT69, test PASS, vland PASS, addmatch done)
- Closed by an Inferred statement after `TurnAndFaceTimer=2;`: `bool inferredChecks = false; if (inferredChecks && (<32 x
  InferredDeadTest(InferredDeadTemp())>)) {}` (row-local descriptive helper, empty inline ctor/dtor). Proof that the count is
  exact: 26/27/31/33/40 DIFFER, 32 RELOCATION_MATCH; position free inside region 1 (start / after timer=2 / intro / after chat=1
  all match); placing bits between the two attacked tests breaks the second test's bit numbers (8,16,32 in retail).
- Locals renamed to the Anniversary PDB names (PresentedItem, TurnAndFaceTimer, full_text_name, speech_movie): bytes unchanged.
### 0x00E50FD0 CBB_BeardyBaldyMan::Main -- RELOCATION_MATCH (src/00e50fd0.cpp, no flag, test PASS, vland PASS, addmatch done)
- Same Inferred statement right after the AddSpawnedFunction statement (>= 30 bits needed there: 29 DIFFER, 30..35 match; placed
  before it, the two entry stores swap registers, i.e. retail's second word is numbered after the new-expression's ebx word).
- Locals renamed to Anniversary names (regionName, sehMe, randomSpeech, speech_movie, Exit): bytes unchanged.
- vland RELOC_NAME: 0x00E53670 is labelled IsHeroWearingBeard; Ego_r holds only IsHeroWearingAnyBeard for this class -> NAME_FIXES.tsv.
### Coordinator decision for both: the wording of the Inferred statement is FITTED (the real construct emits no bytes: a run-time-false
  test with 32 class temporaries whose cleanup c2 deletes). Natural candidates checked and ruled out by counts: per-T / per-SAY /
  per-while bits (Lady 0x00E3EB10 and Magicman 0x00E40E80, same TU, have more of each and one flags word); literal `if (0)` / `0 &&`
  release-assert arms (fable_assert.h form) number no bits. The row-local helper names are descriptive.
### 0x00E4B5B0 TollBandit (wfE3 #3)
- Vanishing-bit weight sweep (0/1/2/4/8/16/26 dead conditional temps at the start): 162 -> 162/166/188 hunks; not the lever.
- tools/fmap.py (ours /FAs symbol -> retail frame offset over aligned instructions; tmp/tbmap.txt): 44 of 72 frame symbols move.
  Slot MEMBERSHIP differs, not only order: ours -180 = {$T11354,$T11383 -> retail -184, $T11431 -> retail -104}; ours -160 =
  {$T11355,$T11359 -> -168, $T11432 -> -184}; ours -156 {$T11382 -> -244 (joins $T11350/11351/11459), $T11426 -> -164}. So the
  packer's node list order (size, then IL weight) differs -> needs the phook trace (tools8 phook.py + pack.py), not attempted.
- The first code hunk (0x00E4BB0B, ours `jmp ; lea ecx,[ecx]` loop-alignment pad) is a consequence of disp8/disp32 shifts.
- PACKER REPLAY WORKS (wfE3 #3): tools/ph/ph_host.py <src> <out.json> 'Main@CTollBandit' (15 s; copy of hand886 phook.py) +
  tools/ph/sim.py <json> [x:A,B drop conflict] [a:A,B add conflict] [A,B swap order] replays the slot joining and reproduces
  ours' partition exactly (node names: named locals + sid, temps T<symbol id>, sid = u32 at symbol+40).
  Dropping ONE conflict (pause _PauseMyEntitiesPlease10482 of the line-203 state-2 BC_MOVIE v _warn_timer10617) reproduces
  retail's whole 4-byte partition (retail -244 {350,351,382,459}, {354,383,432}, {355,359,425}, {356,358,426}, 431 alone;
  target in tmp/tb_target.txt). In c2 a named object conflicts with everything inside its enclosing block from the block start.
- differ/00e4b5b0_tb6.cpp = differ + an inner `{ }` from `CTimer warn_timer;` to the end: the 4-byte partition becomes retail's,
  132 hunks, but 10610 bytes (frame 0x10C v 0x118) because nearest10662 (line 275) no longer conflicts with followers10471
  (line 201 BC_SET_FEE) and $T11365 (line 210): retail needs nearest live over state 2 but warn_timer NOT live at line 203.
  A pure block-start model cannot give both while nearest (275) follows warn_timer (238) and warn_timer is used in states 4/5:
  next lead = the real state-machine shape (e.g. states inside one outer loop / different declaration places), measured with
  ph_host + sim against tmp/tb_target.txt (12-byte target: {escape_pos, followers10471, followers10698, trader}, nearest alone,
  $T11365 alone).
### 0x00F1EED0 PlayWave (wfE3 #3)
- Call counts (ours obj at T54 v retail): ~vector<CScriptThing> 9 v 15, _free 27 v 22, operator delete 19 v 17, ~CBaseClass 16 v 17,
  ~CScriptThing 25 v 26. Retail calls ~vector at 5 pads x3: 0x00F201B1 (<- 0x00F1FEE6, the T check after the creature loops),
  0x00F2029D (<- 0x00F1FF5D), 0x00F20CA1 (shared pad of ~10 exits incl. the counter / CreaturesLeft loops and line `if (CrowdLevel>5)`),
  0x00F21166 (normal end), 0x00F212CC (movie exit). Ours (listing tmp/p3main.asm) expands it at the CreaturesLeft-loop exits
  (source lines 257..288, one pad each) and calls it only at 309 / 365 / 371: retail cross-jumps those loop exits into 0x00F20CA1.
- c2 inline trace (tools/inl.sh, tmp/inl_pw0.txt): expanded ~vector sites score 43..48 at f 3.4..21 (hot loops), called sites
  >= 58. /d2inlT49 11565, T47 10509, T46 11309, T45 10973, T44 11021 (non-monotonic): no uniform T; the residue is the
  block layout / cross-jump structure of the counter loops, not a threshold.
- sim on tb6 + added conflicts: nearest10662 must conflict with followers10471 AND the earlier 12-byte temps ($T11361/365/369/
  373/377, state 1-2 code) to give retail's 12-byte slots, i.e. in retail nearest is live (block-start sense) over states 1-2,
  while warn_timer is not live at line 203. Not yet found a C++ block shape giving both (code order: warn_timer ctor
  0x00E4C73B before nearest; both timers destroyed only at the common exit 0x00E4DD1A/23).

## wfE3 fix-up (2026-11-15): Inferred dead-guard device of 0x00E50FD0 / 0x00E3BB70 replaced by device d
- What the old device moved: it numbered 32 VC7.1 conditional-temporary flag bits (row-local empty-ctor/dtor temps behind a run-time-false
  local) so the FE opens a SECOND flags word whose bits c2 deletes, leaving only its entry `$T = 0` = retail's unread entry stores 0x00E50FE8
  (BB, `mov [esp+34h],ebp`) / 0x00E3BB81 (Madame, `mov [esp+9Ch],edi`) and the slot / filler shifts behind them.
- The earlier note "literal if(0) / 0&& arms number no bits" was WRONG for conditional temps: probe/u/a3 held only 16 bits (< 32, no word to
  open). On the real rows 31 x FABLE_RETAIL_ASSERT(seh_me.IsValid() && PresentedItem == CCharString("...")) -> DIFFER 10155v10149, 32 ->
  RELOCATION_MATCH, 33 -> DIFFER 10149v10149; 40 plain asserts without a temp (FABLE_RETAIL_ASSERT(seh_me.IsValid())) -> DIFFER 10133v10149
  (= no device). So retail's fact = dead release asserts whose conditions create class temporaries right of && / ||.
- Final form (both RELOCATION_MATCH, vt PASS, addmatch redone, strict-header vland RELOCATION_MATCH PASS, no HEADER_POLICY /
  SHARED_HEADER_AVAILABLE / RELOC_IDENTITY): `#define FABLE_RETAIL_ASSERT_DEAD_ARM` + `#include "fable_assert.h"` (overlay copy of agentC6's
  header, hdr/README.md) and 8 asserts of 4 conditional temporaries each: Madame (after `TurnAndFaceTimer=2;`)
  `FABLE_RETAIL_ASSERT(Me.IsNull() || (!Game->GetThingWithScriptName("BordelloHouse").IsNull() && Game->GetThingWithScriptName("BordelloHouse") != Me));`,
  BB (after the AddSpawnedFunction statement) `FABLE_RETAIL_ASSERT(!sehMe.IsValid() || (sehMe.GetScriptThing() == Me && ... != Game->GetHero() && !...IsNull() && ...IsAlive()));`.
  Count / region EVIDENCED, conditions INFERRED/FITTED (marked in each header). Row-local InferredDeadTemp / InferredDeadTest / INFERRED_DEAD_TEST4 removed.
- BB Inline-budget: scoped inline_depth(0) round the CSpawnedFunc1<CV_BeardyBaldyScript,CScriptThing> ctor specialization; without region or
  without specialization DIFFER 9854v9885; retail copy ctor 0x004ABE90 @0x00E51090 + ~CScriptThing call 0x004AA840 @0x00E51099; landed
  CUndeadFellow::Main 0x00EF87D0 same idiom. Header "no flag or region" corrected.
- Madame: its CIsThingAlive ctor inline_depth(0) region was unmarked: now on the Inline-budget line (without it DIFFER 10165v10149; same-TU
  landed CBordelloLady::Main 0x00E3EB10 carries it). Backups: tmp/<addr>.before_fixup.cpp, tmp/payload.before_fixup.json, tmp/oracle.before_fixup.tsv.

## wfE3 #4 (2026-11-15) -- 0x00F1EED0 PlayWave: the /d2inlT54 flag is a fit; retail is default T74 with different static frequencies
- New tools: tools/tx.py (per T-check exit kind, retail v ours: V call ~vector, F expanded vector free, S call ~CScriptThing,
  C call ~CTimer 0x00CD4470, D expanded DeregisterTimer), tools/side.sh <src> [flags] (FAsc listing + line-keyed compare against
  tmp/rsites.tsv = retail exit kind per source line, from the T54 alignment), tools/side2.py, tools/pv.sh <src> [flags] (grade + line
  diff, one line), tools/ex.py / ex2.sh (per-exit f / scores from an inline trace; ex2 maps late exits to lines for g1-shape sources).
- Retail per-line exit kinds are consistent with the DEFAULT threshold 74 and NOT with any uniform /d2inlT on our frequencies:
  countdown (84/86/86 D, 88+ C) fits T74 exactly with our f; the creature loops / counter loops / post-loop code are colder in retail.
  Thresholds measured from our trace scores (pen 0): ~vector called iff f < ~0.47, ~CScriptThing called iff f < ~0.38, ~CTimer
  called iff f < ~0.033 (pen 10-13 later: ~0.07).
- pw/g1.cpp = differ + `if (N>0)` guards in front of the creature i and j `for` loops (FITTED, device a "redundant guard"):
  every creature-loop site (114..236) now has retail's kind at DEFAULT T (lines differing 22 -> 13; 10677 bytes). Left:
  241 (retail VVVD = f in (.38,.47); ours .846), 275..299 (retail VSVVD; ours f 3..13), 313/331/333/340 (retail ~CTimer called).
  Constraint found: retail f(241) < .47 < f(253) (first counter loop: body top colder than after-loop), impossible for a `for`
  (after = top) or a `while (i<3){..;++i;}` (top .5E, after .25E) or a guarded do-while (top 5E, after .33 top).
- c2 loop rules measured here: for = body 5x, after = top; `while (i<3) {...; ++i;}` = top x(2/3..1/2) E, after = E x normal/(all exits)
  (inner loop with 2 return exits: after = E/3); AR_COUNT-style condition while = body x1, after x.5; a `while` containing a `for`
  behaves like a for. Inner while loop at 265 (w00100 / o_while_00) fixes 266..282.
- Score formula fitted on our traces (matches every logged sc): f >= 1: c*(1/f+2)/3, f < 1: c*(1/f+6)/7, expanded iff < 74; effective
  c: ~CTimer 14, ~CScriptThing 60, ~vector<CScriptThing> ~63.5 (NOT the logged c1 44), GetPos 15; + pen = floor(sz*(a+g)/18000)
  once a+g > 9000 (sz: timer 20, ST 66, vec 50). Each fully expanded exit adds ~518 to g.
- pw/k1.cpp = g1 + `if (3>0)` in front of the first counter loop (FITTED): 241 VVVD and 253 FFFD both right (a guard gives
  after = 5pE + (1-p)E > top). pw/s1/while_for_for_gdo.cpp (= pw/b2.cpp): k1 + inner counter loop at line 265 as
  `{ long i=0; while (i<3) {...; ++i;} }` + outer `if (CreaturesLeft>0) do {...} while (CreaturesLeft>0);` -> only 291/293/299
  differ in exit kind (DEFAULT T, 9701 bytes, 264 hunks). Retail 293/299 = FSFF (vectors expanded, Furthest ~CScriptThing CALLED):
  needs pen>0 there (retail a+g crosses 9000 by line 291; ours at 299). Dead release asserts DO add to g (the dead arm is IL at
  inline time): N x FABLE_RETAIL_ASSERT(ArenaRound>=0) moves the pen onset (n=54: 291 right, 293/299 then VSVV). FableWin /Od
  PlayWave is +287i / +861b over ours = ~3 int asserts (wfE7 lesson: +81i/+314b each) -> retail likely has a few asserts.
- Other helper splits still open in b2: GetPos expanded in retail at 0x00F1F52E (site line 124; ours f .0201 < .035 -> called);
  CRGBColour ctor (FadeScreenOut) called in ours; 0x007E74D0 extra calls near the end.
- BEST NOW (saved differ/00f1eed0.cpp, old T54 draft kept in tmp/00f1eed0.differ_T54.cpp): DIFFER(8976v9262) 30 hunks at DEFAULT T (no flag).
  Steps that each cut hunks: GetPos FABLE_FORCEINLINE (179 -> 62; retail expands the only direct site 0x00F1F52E), GFCharStringToInt
  __fastcall (PDB YI; 62 -> 53), i/j loop guards spelled `long i=0; if (i<N) for (; i<N; ++i)` (init before the guard; 53 -> 49),
  named `float angle` for RotateXY (49 -> 30). Remaining: 293/299 FSFF (see above; pen/frequency model says f(293)=f(299)~2-2.6 with
  pen but then 313 would expand ~CTimer -- the c2 model has a gap here), CRGBColour ctor (retail expands at f where ours scores 888),
  outer loop guard codegen, SIB 0x00F1FFB8, fillers. Note the dead-assert count (54) was fitted on the pen onset; 53/54 equal.
- +CRGBColour: new overlay opt-in FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_CTOR4 (hdr/README.md; no out-of-line ??0CRGBColour@@QAE@EEEE@Z
  bound anywhere in retail; expanded at 0x00F21108 where the plain score is 888) -> 27 hunks, DIFFER(8974v9262), differ/00f1eed0.cpp.
  Regraded 0x00E3BB70 with the changed overlay: RELOCATION_MATCH (0x00E50FD0 does not include CRGBColour.h).
- Branch spellings carry different c2 static probabilities: `x>=1` v `x>0` changed the inline split (8958 v 8974) where `x!=0` / `x`
  did not; `if (!call())` gave the then-path 0.375. (>=1 is wrong codegen for the outer guard: retail `test ebx,ebx ; jle`.)
- Open model gap (for a successor): retail 293/299 FSFF needs f(293)=f(299) in (1.97, 2.58] with pen (k~0.5); then 313
  (3Z/64) and 331 would expand ~CTimer, but retail calls it there. Either the branch probabilities of 304..331 differ from
  `if (x)`=0.5 (try other comparison spellings with retail's codegen), or the decision order differs. The final loop body
  should be ~2x hotter than ours (291 ~0.86 with pen keeps VSVVD).

### 0x00E4B5B0 TollBandit (wfE3 #4) -- not re-graded; analysis only
- Retail timer slots: call_timer [esp+10h] (state 2), warn_timer [esp+14h] used by states 3, 4 (0x00E4D815) and 5; ~CTimer called only
  at the common exit 0x00E4DD1A / 0x00E4DD23 (state-2 exits cross-jump into the second call). nearest is RVO-initialised directly
  (call [ebx+134h] 0x00E4CB9B into its slot), so it is a declaration-with-init after warn_timer's ctor 0x00E4C73B.
- With c2's "named object conflicts with its whole enclosing block from the block start" rule, retail's partition (warn_timer shares
  with the state-2 pause object, nearest does NOT share with the state-2 followers / temps) has no C++ block shape: nearest would need
  an earlier block start than warn_timer although it is declared after it and both are used in state 4. Either the rule is incomplete
  (the 12-byte temps $T11361.. are header-inlined temps, line 432 of a header, not state 1-2 code: re-locate them before trusting the
  "nearest must be live over states 1-2" inference) or the conflict comes from another node. Next: map those temps with tools/fmap.py.

### 0x00E4B5B0 CTollBandit::Main -- RELOCATION_MATCH (wfE3 #5; src/00e4b5b0.cpp, no flag, test PASS, strict vland PASS, addmatch done)
- Closer: define the landed inline texts `inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }` and
  `inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }` (the spelling of 106 / 88 landed NScript rows) above the row.
  Retail still CALLS both (0x00CD4450 / 0x00CD4470: the inliner does not expand them here), but with the bodies visible c2 sees that
  `this` is not captured, so (a) TimerIndex stays in ebp across calls (retail 0x00E4C5B5 / 0x00E4D815; ours reloaded) and (b) the
  timers stop interfering by whole-block scope: warn_timer shares +0x14 with the state-2 PauseMyEntitiesPlease and T352, call_timer
  shares +0x10 with T353, exactly retail's partition. Ctor only / dtor only: both DIFFER. The earlier FITTED `{ }` block from
  `CTimer call_timer;` to the end is NOT needed (removed; no-block source = RELOCATION_MATCH).
- How it was found: packer replay showed d0-minus-the-pause1-node = retail's whole partition and that no lexical block shape can give
  it (warn_timer must not see P's pause while nearest must see P's temps); the TimerIndex-in-ebp hunks then pointed at c2 alias /
  capture analysis (LESSONS_wfB5: visible callee bodies change capture). Tools: tools/ph/chk.py (interference facts + normalized
  partition diff), tools/ph/lines.py (nodes by source line), tools/ph/inter.py.
- vland: RELOC_NAME 0x00E4E0E0 labelled CTollBanditScript::HandleBanditToll; PDB name is SetUpBanditToll -> NAME_FIXES.tsv. RELOC_ALIAS
  _rand -> import thunk (expected).
## wfE3 #5 (2026-11-15) -- 0x00F1EED0 PlayWave: 27 -> 21 hunks (DIFFER(8958v9262)), differ/00f1eed0.cpp (old #4 source: tmp/00f1eed0.differ_wfE3_4.cpp)
- Closer: `CreaturesLeft=0;` moved before the do-loop's first `if (T) return;` (`NSF; CreaturesLeft=0; if (T) return;`). Ours had kept a zero
  register live across the loop top (`xor edi,edi ; cmp ebx,edi` preheader + `xor edi,edi ; jmp top` back edge) because the line-267 exit pad
  pushes edi as 0; retail zeroes edi after the T call (0x00F2056F) so the pad sees edi=0 from CreaturesLeft. Now the guard `test ebx,ebx ; jle`,
  the jmp-over-filler, the `jg 0x00F20560` back edge and the Remove loop laid inline after the loop all match (whole hunk block 0x00F206E4.. gone).
- `while (C>0) {}` == `if (C>0) do {} while (C>0)` byte for byte. Remove-loop condition spellings (<=2, 3>i, !=3, unsigned, int) identical;
  do-while / while(++i) Remove loops worse (8904). Guards `if (3>0)` before the Remove loop: no change.
- Left (structural): Remove-loop top exit 0x00F20702 and the after-loop exit 0x00F20743 are FSFF in retail (vectors expanded, ~CScriptThing
  called), ours VSVV. Inline trace (tools/inl.sh, tmp/inl_z1.txt) + score fit: score = (c + pen) * g(f), g = (1/f+2)/3 (f>=1) or (1/f+6)/7,
  expand iff < 74; c: ~vector<CScriptThing> 63.5, ~CScriptThing 59, ~CTimer 13.5; pen = floor(sz*k), sz 50 / 66 / 20, k = 0.5 here.
  FSFF at k=0.5 needs f in (1.96, 2.42]; ours f(297)=f(303)=1.072 (= 5 x half of f(295)=0.429). PROBE: deleting the 0x00F206EA T-check
  (line 295) gives FSFF at both (pw n1: DIFFER(9246v9262) 31 hunks) but then ~CTimer expands (D) at 317 / 335 where retail calls it (C):
  retail is ~2x hotter at 297/303 yet as cold as ours from 317 on (f(317) < 0.065, f(327) > 0.065, f(335) < 0.065). No spelling found
  that gives both: early `if (Wave!=NumWaves) return;` (no change at 317/335, layout worse), T spellings at 295 (!T else, ==true, !=false,
  braces: identical), dead-assert count sweep 0..50 on z1 (295 flips to VVVD before 297 reaches FSFF), `for(;;){ if (C<=0) {T; Remove; T;
  break;} body }` (CFG-identical to retail, exit block 3.17 = far too hot; while(true) differs from for(;;)), while(true)+break at bottom
  (loop inversion duplicates NSF+T: not retail).
- More probes (all on z1, none adopted): `do { if (T) return; } while (0);` at 295 = identical; `for (;;) { if (T) return; break; }` /
  `while (true) {...; break;}` at 295 make everything after it COLDER (297.. VSVVC, 8904). Remove-loop guard / shape variants: no change.
- Retail layout evidence for the open gap: the 303 return path (FSFF) is laid as the FALLTHROUGH of `je 0x00F20C1A` with the continuation
  after the pads, i.e. c2 rated the 303 return at least as likely as the continuation (p(cont) <= 0.5), while 295 behaves as if it did not
  halve. A successor should look for a source construct that changes these two branch probabilities together (both are T-checks right
  after a loop: the do-loop at 295, the Remove loop at 303); measuring tools: tools/ek.sh <src> [lo] [hi] (grade + per-line exit kinds,
  5 letters), tools/inl.sh + tools/ex.py (per-exit f), probe pw-style edits in tmp/00f1eed0_<tag>.cpp.
- Residue list of differ/00f1eed0.cpp (21 hunks): SIB 0x00F1FFB8 (device c candidate only once the structure is right), xor edi,edi one
  instruction earlier (0x00F2056A v 0x00F2056F: retail zeroes between the T call and its test), 297/303 FSFF (-~300 bytes and the pads
  0x00F20B81..0x00F20C15 / inline 0x00F20750..0x00F207E5), alignment fillers behind them.
- wfE3 #6 (2026-11-15 13:2x, successor): 0x00E4B5B0 re-verified against today's headers + overlay (no overlay change since the match): q.py RELOCATION_MATCH, vt.py PASS, payload.json/oracle.tsv entries present; inline CTimer ctor/dtor texts = the 106/88-row landed spellings, operator=/GetTime = landed FABLE_FORCEINLINE texts (no reloc binding, expanded). Nothing re-authored.

## wfE3 #7 (2026-11-15, successor) -- 0x00F1EED0 PlayWave: no change to differ/00f1eed0.cpp (still DIFFER(8958v9262), 21 hunks); parked
- New measuring tools: tools/pk.sh <src> [flags] (grade + exit-kind compare by site index against retail, one line), tools/fq.sh +
  probe/fq/p1..p7.cpp (c2 static-frequency probes, 12 s per TU; wfB2 method), tools/gen_sw1.py + tools/sweep.sh (loop-shape sweeps).
- Exact constraint, measured: probe a2 (tmp/00f1eed0_a2.cpp = 295 T replaced by a bare call AND `&& ArenaRoundWave!=ExtraCreatures` on the
  Wave test) gets EVERY exit kind right except the deleted site (trace: f(297)=f(303)=2.144, f(308..)=ours). So retail is 2x ours at
  297/303 but equal to ours from 308 on; 313 (ours .067, threshold .0625) leaves no slack, so the extra halving must sit on the path
  303 -> 308. With forward-only c2 frequencies that is TWO facts (one doubling before 297, one no-code halving 303->308), or a loop-like
  region around [Remove loop; 303 T] with body x2 and a halving exit (do{}while(0) gives exactly that ratio but x10; for(k<1) x5; guards
  x.5; products of {5,10,.5} never give 2).
- Swept (all no better): 96 loop-shape combos (first counter loop guard/for/while x sum loop for/while x inner counter while/for x
  TotalCreatures for/while x outer guarded-do/while x Remove for/while; tmp/sw1.txt: best = current source); Remove-loop spellings
  (<=, !=, unsigned, while, do, for(;;)+break, continue, goto: for-type all equal, while/do-type body x10 but after-loop /30);
  /d2inlT88, /d2inlT90 (~15 other sites flip); `if (!T) { rest }` == `if (T) return;` byte and frequency identical; outer loop as
  for(;;)/while(true) with the exit block `if (C<=0) { if (T) return; break; }` (y1..y3: far too hot); bool/!=/++ spellings of the
  Wave test (same 0.5).
- TRAP found: moving the 295 check into the do-body as `if (CreaturesLeft<=0) { if (T) return; }` (t1, u1..u6) grades the SAME
  "DIFFER(8958v9262) 21 hunks" and raises f(297) to 2.005 (1.87x; vec sc 74, still called), but the guard's `jle` then targets the
  Remove loop instead of the T call (retail jle 0x00F206EA): fg/q mask jump targets, so a clean hunk count there is not proof (checked
  in the FAsc listing). Not adopted.
- Layout note corrected: retail's fall-through return path at 0x00F2074A is a CONSEQUENCE of the expanded FSFF pad (a2 lays it the same
  way), not evidence of a branch probability.
- Open for the coordinator: the only closers measured are fitted no-code frequency devices (a halving on 303->308 such as a constant
  guard around the Wave block, plus a doubling before 297 that no byte-identical spelling gave). Not handed back.

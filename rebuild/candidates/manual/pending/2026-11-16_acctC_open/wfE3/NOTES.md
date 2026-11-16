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

## wfE3 #8 (2026-11-15, successor) -- 0x00F1EED0 PlayWave: every exit kind right, DIFFER(9262v9262) 24 hunks (tmp/v/d.cpp)
- Tolerance table (tools/tol.py <inl.txt>: per exit, the factor range on f that keeps every call/expand decision): every site from
  line 257 to 313 tolerates x2 (297/303 NEED x(1.886,2.268]); 245 and 317+ must stay x1 (317 <= 1.26, 327 >= 0.85). So the
  two facts are: a doubling anywhere after 245 (not only before 297) and a halving in (303, 317].
- c2 for-loop facts (probe/fq/p14..p16): plain `for (i<3)` body = after = header H; `i<3 && 1` body H/2, after 1.5H;
  `i<3 || 0` body 1.5H, after H/2; constant `if (3>0)` guard halves; a do-while condition `C>0 || 0` gives body x1.13 after x0.87.
- Closer d = differ with (1) first counter loop `for (long i=0; i<3 && 1; ++i)` (no `if (3>0)` guard; body unchanged, after x2.5),
  (2) outer `} while (CreaturesLeft>0 || 0);` (after x0.87 -> 297/303 x2.17), (3) `if (ArenaRoundWave==Rounds[ArenaRound].NumWaves && 1)`
  (halving for 317+). All 65 exit kinds = retail, length 9262 = retail. Left: 6 SIB base/index swaps in the creature loops
  (0x00F1F576.., flipped by change (1): the guard removal, not frequencies), xor edi,edi 1 instr early (0x00F2056A), pad 0x00F207EA
  uses push 0 / test esi,esi where retail uses edi as zero (+ fillers). k unused `long uN=0;` locals k=1..7: no change.
- d saved as differ/00f1eed0.cpp (old #7 source: tmp/00f1eed0.differ_wfE3_7.cpp). Sweeps on d (all 24 hunks, same residue): token spellings
  of the three facts (1/true/3>0/!0, 0/false, 36 combos), dead-assert count 53..58 (needs 53..58; position anywhere in the body is
  equal, z1..z3), assert expression, k unused `long`/`float` locals k=1..8 at entry (no effect; k=80 flips line 250 the wrong way),
  DiagRuler at top/end, `int`/`unsigned` counter index, CreaturesLeft=0 placements (q1..q4: equal; after the T check = preheader zero, m1).
- SIB orientation map (tools/sibo.sh: Creatures[i].CreatureType x6 / HUDType line 250): retail begin-first x6 / idx-first; old
  differ begin/begin; d idx/idx; every variant where 297/303 expand the vectors (a, c, apoc, appn, d) has the 6 CreatureType sites
  idx-first; variants with 297/303 called (base, b, kpon) begin-first; w1 (creature i-loop unguarded) begin-first but kinds wrong.
  So the orientation follows a global c2 state that the extra FSFF expansions move; retail has the same expansions yet begin-first.
  c2prio (tools/c2prio.py, tmp/prio_b.txt v prio_d.txt): same colouring, ids shifted +78 (idx split ebx #218 -> #296).
- Other closers tried for 297/303 x2 with the guard kept (sw2 / sw3 sweeps: 117 combos of first-loop shape x outer guard x outer
  condition x halving place): only the no-guard `&& 1` first loop reaches the window.
- SIB = c2 symbol-record THRESHOLD (measured, diagnostic only): DiagRuler3 x k at the END of PlayWave (tools/rul.py, no budget
  change up to k=38): line 250 (0x00F1FFB8) flips to begin-first at k=27, the six CreatureType sites flip to begin-first (= retail)
  at k=28 (19 hunks, tmp/v/r28.cpp, NOT a hand-back). Retail = creature begin-first AND line 250 idx-first, impossible with an
  end-placed count (250 flips first), so retail's extra records sit between the creature loops and line 250's inline expansion
  in inline order (rulers there move the inline budget: rul2.py k>=12). Parse-time records (unused locals) do nothing up to
  k=40. Lead for a successor: an expanded helper between lines 117..250 whose real text has more named locals/records than our
  header text (lessons C-series: "id-order tie ... look for an expanded inline helper whose FableWin /Od size disagrees") --
  run fwsize.py on the creature-loop helpers (GetPos, CScriptThing ctor/assign, vector<CScriptThing> operator[], CCountedPointer).
- Pads 0x00F207EA / 0x00F20A2B (retail `push edi` / `cmp esi,edi` / retail xor edi,edi at 0x00F20A2B) and the early xor at
  0x00F2056A: the old differ matched both pads; d's doubling makes the outer-loop body x2.8 hotter (allowed for kinds) and the
  const-0 register candidate loses those pieces. Retail likely keeps the outer-loop body at the old frequency and doubles only
  after it; the shapes tried for that (first loop inner `if (3>0)` + outer guard `&& 1` + cond `|| 0` / `(..||0)||0`, u05..u08)
  break 277/295 in the real function although the probe factors fit. Not closed.
- Identical-text "replace" hunks (stores of 0 at 0x00F2065A / 0x00F206C3 / 0x00F20C8A / 0x00F20D27 / 0x00F211FB) are present in
  every variant incl. the old differ: fg display artefact around jump targets; check with q.py / vland once the rest is clean.
- fwsize.py on differ/00f1eed0.cpp (hints only): creature-region helpers that disagree with FableWin /Od: ??0CScriptThing@@QAE@XZ ours
  15i/42b v FableWin 24i/87b (+9i: real default-ctor text likely has more members/temps = more inline records where it is expanded
  in the creature loops -> the positional records the SIB threshold asks for), C3DVector operator+ +6i (line 322, downstream),
  CCountedPointer<CScriptThing> dtor -29i, vector operator[] +30i (FableWin STLport range check, ignore). NEXT STEP: try the
  FableWin-sized CScriptThing default ctor text (from the PDB member list / landed CScriptThing rows) as a new overlay opt-in and
  re-measure the SIB orientation with tools/pks.sh, then the const-0 pads.

## wfE3 #9 (2026-11-15, successor) -- 0x00F1EED0 PlayWave: real ~CCountedPointer text fixes the 6 creature SIBs
- fwsize lead: ??1?$CCountedPointer@VCScriptThing@@@@QAE@XZ ours 38i v FableWin 9i (= `{ ReleaseData(); }`, FableWin's 420
  ReleaseData instantiations); retail ~CScriptThing 0x004AA840 zeroes Data before Info = the existing opt-in
  FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (137 landed rows). Probes: dtor layer alone (tmp/hA) = the whole effect; ShareData's
  Info reload alone (tmp/hB) = no change. With it the 6 CreatureType SIBs are retail; inline budget re-fit: dead asserts window
  12..18 (was 54), 15 chosen -> DIFFER(9262v9262) 19 hunks (tmp/n9/best.cpp): line 250 SIB 0x00F1FFB8 now flipped, const-0
  piece (0x00F2056A xor early, pads 0x00F207EA / 0x00F20A2B push 0) unchanged, + 5 identical-text artefacts.
- Saved as differ/00f1eed0.cpp (old #8 source: tmp/00f1eed0.differ_wfE3_8.cpp). Token re-sweep with the real dtor (tools/tok3.py,
  body-only; NOTE: the older tok.py/tok2.py replaced text inside the header COMMENT first -- their "|| 0" results are void):
  48 combos of first-loop / sum-loop / outer-condition / Remove-loop / Wave tokens at 15 asserts: only the #8 set (first `i<3 && 1`,
  outer `|| 0`, Wave `&& 1`) gives all exit kinds; `[sum-loop && 1 + outer || 0 + Wave && 1]` misses only 246/278.
- Line 250 SIB (0x00F1FFB8): k dead `long fuN=ArenaRound;` locals at the top of the first counter loop body: k=30 and k=32 flip it to
  retail with the creature sites kept (18 hunks, tmp/n9/k30.cpp, NOT handed back: a large device-c count and the row still differs);
  k=1..16 no change / 18..28 erratic (chunk-of-32 ids); constant-initialised `=0` locals and moved dead asserts do nothing; entry
  placement flips the creature sites first (k>=12). Statement spellings of line 247..253 (Max v Total tests, order): no change.
- const-0 piece (c2prio, tools/c2prio.py --trace): const 0 (#4) is split into 17 pieces (4 edi, 3 ebx, esi, ebp, 8 immediate), the
  same shape as the #7 source whose pads matched; the 269/278 pads' first vector-dtor piece is a "spill <= 0: not worth a register"
  piece (spill -2/-3). Pad 271/274 (inner loop, loop depth 2) are fine. Probe z1 (tmp/n9/z1.cpp, NOT behaviour-preserving:
  `TotalCreatures[i]=CreaturesLeft=0;` in the PlayerLeaving loop) makes both pads and the xor position retail (14 hunks) but brings
  back the zero register live across the loop top (0x00F20553 xor/cmp) and a jne/jmp bottom: so the pads follow the const-0 web of
  the outer loop (one more CreaturesLeft=const-0 copy at loop depth 2 merges it). Behaviour-preserving copies tried (redundant
  CreaturesLeft=0 after the T check, self-assignments, bool t=T split, int type, Game->NewScriptFrame() spelled out, inner loop as
  do-while / i++): identical or worse. Not closed.
- Also tried (no effect / worse): empty inline ValidatePImp() in GetPos / GetDataString; named references for the line-250 operand
  (creature / wave / round def); outer loop as for(;;)+break, while(C>0||0), `if (C>0||0) do`; C3DVector operator+ named-result /
  += / member-assign texts (FableWin x87 sizes are unreliable; named result breaks the budget: 9278).
- test/00f1eed0.cpp written (model test of the row's decisions, lane style), vt.py PASS. Row is NOT matched: no addmatch.
- Next lead for a successor: extend tools/c2prio.py to print the source lines of split PIECES (the "piece #N of #M" events) to see
  which const-0 piece covers the 0x00F207EA / 0x00F20A23 pads in ours and why its spill is <= 0; then look for the source fact that
  gives retail's piece a positive spill (the #7 source with 297/303 called had the pads right with the same decision sequence).

## wfE3 #10 (2026-11-15, successor) -- 0x00F1EED0 PlayWave: CL=0 after the T check + a preheader that is not a const-0 ref: 19 -> 11 hunks
- Probes in tmp/n10/. GetFanfareMusic's landed text defined above the row (same-TU callee, retail 0x00F14270 sits above PlayWave): no
  change (b1). Function-scope `long i;` (c1/c3): no change; i,j,k function-scope (c2): 112 hunks. do{..}while(0) round the pad-1 / pad-4
  T checks (d1..d4): transparent.
- c2 piece map (new tools/pc0.py <trace.json> 4 81 <pass> <lo-hi>: time-aware ids, per block of one priority pass, which const-0 piece
  is referenced / live): in base the loop-region piece #459 = {preheader `CL>0` (L265), loop-top block, loop bottom `CL>0||0`,
  pad-1's FIRST vector dtor} has spill -3 (immediates) and the rest of pad 1 is a separate edi piece; pad 4 the same (#18 low / #19 edi).
  Retail's `call T; xor edi,edi; test al,al; jne pad1` is exactly what c2 emits when the piece holding pad 1 is live-out but not
  live-in of the loop-top block (load at the block end, before the flag-setting test) and CL's in-loop zero is a copy of it.
- a1 = `NSF; if (T) return; CreaturesLeft=0;` (natural order): pads use edi (retail) but the edi piece also covers the preheader
  (`xor edi,edi; cmp ebx,edi`) and the back edge (`xor edi,edi; jmp top`): 9278, 21 hunks.
- e5 = a1 + preheader `if (CreaturesLeft>=1) do {` (f4 `!(CreaturesLeft<1)` identical): DIFFER(9262v9262) 11 hunks: both pads and
  every other const-0 piece retail; left: line-250 SIB, preheader `cmp ebx,1; jl` (retail `test ebx,ebx; jle`), xor edi,edi at the
  loop-top block start (before NSF, 0x00F20561) instead of after the T call (0x00F2056F), loop-head alignment (retail aligns the outer
  head 0x00F20560, ours the inner head) + the 5 identical-text artefacts. Tokens (`&& 1` first loop, `|| 0` outer, `&& 1` Wave) still
  all needed on e5 (f4_xxx sweep).

## OWNER GUIDANCE 2026-11-15 (coordinator note, for every agent in this dir, read now)
Sometimes hand-authoring is more likely to get a match than automatic tools. Read the retail disassembly and the matched same-class / same-TU siblings, reason about the statement structure a programmer would have written, and write it by hand. Use sweeps and generators only to answer one specific measured question; stop sweeping when two sweeps in a row move nothing. Full text: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfcommon/WF_RULES.md (last section).
- e5 saved as differ/00f1eed0.cpp (old #9 source: tmp/00f1eed0.differ_wfE3_9.cpp); vt.py PASS (model test unchanged: both edits keep
  behaviour, CL is local and `>=1` == `>0` on long). Header comment marks `>=1` FITTED (retail bytes say `>0`: NOT a final spelling).
- Reading of retail's loop top (hypothesis, consistent with every probe): pad 1 is its OWN const-0 piece starting with `xor edi,edi`
  (like retail pad 4 at 0x00F20A2B and the inner pads), CL lives in edi and `CreaturesLeft=0` is the first statement after the T check;
  c2 then hoists the identical `xor edi,edi` heading both successors of the T branch into the predecessor (before the flag test) =
  retail's `call T; xor edi,edi; test al,al; jne 0x00F207EA`. Ours: a1 merges pad 1 with the loop-region piece (preheader `CL>0`, back
  edge) in edi; e5 merges it with the loop-top block (load at the block start). What keeps pad 1 separate in retail is not found.
- Tried on a1/e5 (no better): preheader spellings `0<CL`, `CL>0L`, `(long)CL>0`, `CL>(char)0`, `!(CL<=0)`, `CL<=0 ; else`, extra braces,
  T check on the same line, `&& 1` / `|| 0` on the preheader (9478 / 8974); CL as int / unsigned long / short; bottom `>=1` (e3/e4),
  `0<CL||0`, `!(CL<=0)||0`, `||0||0`; outer as while / for(;;cond;) with each token combo (w_*, z1..z4: 9752..10237, kinds wrong
  296..318 -- while/for are colder after the loop than if-do); `if (CL>0) for(;;){..; if (CL<=0) break;}` 9361; inner loop as for
  (10389) / do-while (= e5); sum loop as while/do (8190); CL=0 inside the inner block (= e5); device c k=1..7,28,30,32 dead locals at the
  first counter loop on e5: line 250 unchanged (k=28: 12 hunks).
- c2 tools added: tools/pc0.py (const-0 piece map per block per priority pass), tools/blkdump.py (all candidates of chosen blocks);
  traces tmp/n10/pbase.json (base), pf4.json (f4 = e5 shape), pa1.json (a1), pold7.json (#7 source).

## wfE3 #11 (2026-11-15, successor) -- 0x00F1EED0 PlayWave: no hunk progress (still e5, 11 hunks); ~55 grades + 3 inline traces + 2 c2prio runs
Probes in tmp/n11/ (base.cpp = e5 = differ/00f1eed0.cpp; variants in tmp/n11/sw/). New tools: tools/cs.sh <src> (q.py grade + call
sequence retail v ours by callee class T/V/F/S/C/i(indirect)/x(other), difflib blocks; reliable, unlike side.sh whose FA compile is
offset ~0x30 from the graded object), tools/ek2.py, tools/mk.py (variant generator: asserts=N, tok1=0, tok3=0, repl=old@@new).
- NATURAL OUTER `while (CreaturesLeft>0) { NSF; if (T) return; CreaturesLeft=0; ... }` (the idiom of the matched same-class Arena Main
  0x00F0FB70: `while (!PlayerWon) { NSF; if (T) return; ...}`) = tmp/n11/w1.cpp: the loop top is EXACTLY retail -- `call T; xor edi,edi;
  test al,al; jne pad1`, outer head aligned, inner head not aligned (residues (c) and (d) of #10 gone); preheader `xor ebp,ebp; cmp ebx,ebp`
  (own ebp piece, retail immediate). BUT DIFFER(10013v9262), 142 hunks: inline-time frequencies of the loop body AND everything after it
  are x8.5 those of the if-do (inline trace tmp/inl_nw1.txt v inl_nb.txt, tools/ex.py: first in-loop exit f 7.933 v 0.933, first post-loop
  exit 2.479 v 0.291, constant ratio 8.50), so in-loop / post-loop vector dtors expand where retail calls (cs.sh: +35 calls) and the pen
  onset moves the countdown ~CTimer (0x00F1F062) too.
- c2prio on base v w1: the in-loop CreaturesLeft candidate has identical prio -28 / key 2296 / spill 24 in both -> ALLOCATION-time loop
  weights are the same; the while loop top comes from the while lowering (block order / rotated test copy), not from weights. Inline-time
  and alloc-time frequencies differ for while: retail = while-shaped lowering with if-do-like INLINE-time frequencies.
- Tried to cool the while (none closer than 9413): dead asserts 0..300 (10205..8341, erratic: 220 -> 9413/144 hunks); conditions
  `&&1`, `&&1&&1`, `!(<=0)`, `||0`, `||0||0` (9761), `1&&`, `0||` (9933), `0<CL`, `>=1`, `>0L`, `&& !PlayerLeaving`; tokens removed
  (t1 9901); `if (CL>0) while (CL>0)` (9949; inline-time factor 0.6 only), `if (CL>=1) while` (9965/103 hunks), `if (CL>0) while (..||0)`;
  `for(;;){ if (CL<=0) break; ...}` / `while(true)` (10161/10325); goto forms (`goto test; top:{..} test: if (..) goto top;` 8993 / 9089;
  `if (CL<=0) goto end; top:{..} if (CL>0||0) goto top; end:` 9294, 27 hunks = layout move only); ALL-natural source (while outer, plain
  for loops, no tokens, no guards) 11221 / 10997 / 10885 at 0 / 15 / 54 asserts.
- Line-250 SIB on e5: k = 8, 12, 16, 20, 24 dead `long fuN=ArenaRound;` at the first counter loop top: SIB unchanged (11..13 hunks);
  k >= 34 breaks the budget (9374). Retail's three SIBs of that expression: Rounds `[off+Data]`, Waves `[Data+off]`, Creatures retail
  `[IV+Data]` (IV = i*0x38 in edi, frame home 0x64, LFTR'd loop test `cmp edi,0xa8`) v ours `[Data+IV]`.
- Reading: the real source is a natural `while` (sibling idiom + exact loop-top bytes) inside a function whose c2 static frequencies are
  much colder than our spelling anywhere after the countdown; the fitted tokens / guards / inner while / if-do of earlier sessions emulate
  that coldness. The source fact behind it is still unknown (the T-check spelling is the matched siblings' `if (T) return;`). Next lead:
  find what in retail makes the code after the countdown colder at inline time (compare the inline-time f of the creature-loop exits of
  the all-natural source with the frequencies retail's kinds require, NOTES #4) -- then the outer loop can be the natural while.
- LEAD (later in #11): natural outer while + the PlayerLeaving zeroing loop (line ~289) as `{ long i=0; while (i<3) {..; ++i;} }`
  (= tmp/n11/m4.cpp; subset sweep of the 4 counter loops as while, tools/f2w.py): DIFFER(9118v9262) 16 hunks; the WHOLE loop region
  0x00F20553..0x00F206FB is retail (preheader `test ebx,ebx; jle`, xor after T, alignment: #10 residues (b)(c)(d) all gone), and the
  Wave test's `&& 1` token is no longer needed (tok3=0 identical). + post-loop counter loop `i<3 && 1` = tmp/n11/q2.cpp: 12 hunks, 9118.
  Left on q2: line-250 SIB, the 2 identical-text artefacts, and ONE frequency decision after the outer loop: the post-loop T check at
  0x00F20741 -- retail lays its return pad inline (`je 0xf20c1a`, pad 0x00F20750 with expanded vector dtors) and the continuation
  (DisplayQuestInfo(false) / ++ArenaRoundWave) after the pads; ours lays the continuation inline and calls ~vector in a remote pad
  (-144 bytes) + an eax/edx swap at 0x00F2072C. Knobs measured on q2: dead asserts 11..13 9182, 14 9134, 15..18 9118 (16 hunks),
  0 9310, 54 8910; 289 loop as for / guarded for / `||0` for: 10205; do-while = while; `while (i<3 && 1)` 8974; post-loop guarded for
  / `&&1&&1` / `||0` / ArenaRoundWave++: 9118; outer `while (CL>0 || 0)` (tmp/n11/u1.cpp): 9242, 33 hunks, ALL exit kinds equal
  (call sequence = retail but for one pad-group layout move) but the preheader const-0 piece is back (`xor ebp,ebp; cmp ebx,ebp`);
  outer `&& 1` 9038. The frequency knob between "289 while" (too cold after the loop) and "289 for" (much too hot) is not found.
  differ/00f1eed0.cpp stays e5 (equal length, 11 hunks); q2 is the structurally more natural candidate (outer loop = sibling idiom).

## wfE3 #12 (2026-11-16, successor) -- 0x00F1EED0 PlayWave: CORRECTION of #11 -- q2 is NOT "loop region fully retail"
- fg/q MASK JUMP TARGETS. Target-resolved listing of q2 (tmp/q2.q built side): the expanded pad 0x00F20B81 is reached from the
  ZEROING-loop T (0x00F206BD jne) in q2 but from the post-loop COUNTER-loop top T (0x00F20709) in retail; q2's counter-loop-top T and
  post-counter T (0x00F2074A) go to the called pad 0x00F210D6 where retail expands. So q2 has 3 wrong exit kinds (zeroing exit
  expanded / counter-top called / post-counter called); e5 (differ) stays the best source.
- Site -> inline-trace group map (first reliable one): tools/prb.py <src> <dst> inserts inline marker calls PrbA::A..H at fixed lines
  (H after the outer-loop-top T, G after the zeroing-loop T, C/D around the post-outer T, E after the counter-loop top T, F after the
  counter-loop if T, A/B around the post-counter T; line numbers of q2/e5) and tools/exmap.py <inl.txt> prints probe f's and exit
  groups in trace order (an exit's dtor group comes right after its T check, before the continuation). Block f's (probe = block f,
  exit f = half):  q2: H 30.7, G 36.5, C .529, D .264, E .331, F .083, A 1.98, B .99.   e5: H 7.93, G .62, C 1.87, D .93, E 2.33,
  F .58, A 4.66, B 2.33 (e5's post-counter exit 2.33 = expanded like retail; the plain post-loop `for` makes the after-loop block =
  loop body f).
- Device-b precondition test (advisor-suggested, wfE3 #12): ALL-natural source (tmp/n12/nat.cpp: natural outer while, plain for loops,
  no tokens / guards / asserts, real helper texts kept) under /d2inlT30..80 (tools/tsw.sh = grade + tx.py exit-kind mismatches +
  target-aware hunks via tools/tgt.py): erratic, best T55 9121 20 mismatched exits (T53/54 9025 24, T57 9093 23, T74 11221 38).
  Mismatches are mostly ~CTimer expanded where retail calls in the creature-spawn loops (retail is colder there) -> natural +
  one threshold does NOT close; device b dropped. e5 target-aware residue (tools/tgt.py tmp/e5.q): 12 hunks = SIB 0x00F1FFB8 +
  the preheader / xor / alignment shifts of the loop region (every jump target otherwise equal); tx.py: all 65 exit kinds equal.
- c2 INLINE-TIME FREQUENCY MODEL of the outer loop, measured with the probes (z1 = q2 with the zeroing loop as plain `for`, i.e. e5's
  body inside a natural while; H = outer body after the top T, C = block after the loop):
    while (CL>0): H 7.93 C 15.87 | `||0` H 11.9 C 7.93 | `||0||0` H 13.9 C 3.97 | `||0||0||0` H 14.9 C 1.98  => H = 15.87p,
    C = 31.7(1-p) with p = 1-2^-(n+1) for n `||0` terms; `&&1` H 3.97 C 23.8; `(..||0)&&1` H 5.95 C 19.8; `if (CL>0) while` H 3.97
    C 9.52; `for(;CL>0;)` = while. `if (CL>0) do..while (CL>0)` H 7.93 C 2.14; with `||0` bottom (e5) H 7.93 C 1.87.
    `if (T || 0) return;` halves the continuation (return arm 3/4), `if (T && 1) return;` x1.5. In-loop H of z1 == e5 exactly; only C
    (and everything after the loop) is x8.5 hotter under the natural while. The zeroing loop as `for` vs `while(i<3)`/for-without-
    increment/do-while flips the after-flow (for-increment leak) and also moves H (q2: H 30.7, C .53).
  Retail's post-outer exit is CALLED, which needs C ~ 2 (C 15.87 gives exit f 7.9 -> expanded at any plausible pen), so a natural
  `while (CL>0)` with this body can NOT give retail's decisions: retail's loop has if-do-like exit frequency with a while lowering.
- Token fits on the natural while (all measured, none better than e5): c2 = `while (CL>0||0||0||0)` + loop-top `if (T||0) return;` +
  plain post counter loop (tmp/n12/c2.cpp): probe frequencies within 6% of e5 everywhere (H 7.44 C 1.98 D .99 E 2.48 A 4.96), tx 3
  ~CTimer mismatches, BUT 9409 bytes with structural hunks from the countdown (0x00F1EF96 layout) through the creature region and the
  loop top (the `||0` terms bring back the preheader const-0 piece, as u1): allocation / layout weights differ from e5 even with equal
  inline-time weights. c4 (`||0||0||0` alone) 9361 / 3 mismatches; `|| false` == `|| 0`; assert re-fit on c2 (0..30): 9409..9601.
  Zeroing-loop forms on q2 (for / for&&1 / for without increment / for(;;++i)+break / while(true)+break / do-while): 9118 or 10205
  (8974 for while(true)+break), no closer. e5's line-250 SIB with device c (k = 1,2,3,4,6,8,12 dead `long fuN=ArenaRound;` at the
  function top / inside the line-250 block / before `long CreatureCounter[3];`): unchanged (k=12: 14 hunks).
- fwsize on e5: only STLport-debug / assert-inflated FableWin sizes differ (vector::operator[] 12i v 42i = _STLP_DEBUG range check;
  CScriptThing() { } is the landed text) -- no new helper-text lead.
- State: row parked again; differ/00f1eed0.cpp = e5 unchanged (DIFFER(9262v9262), 12 target-aware hunks, all 65 exit kinds equal).
  Next lead for a successor: find a NO-BYTE construct that gives a while-lowered loop an if-do-like exit frequency (C ~ 0.24 H) without
  adding const-0 references to the loop condition (e.g. study how c1xx lowers `while` vs `if..do` in the IL dump: tools/inl trace
  of a 20-line mini TU with the three shapes, compare the pre-c2 block graph), then rebuild e5's downstream on it.
- Condition spellings with identical bytes (`0<CL`, `!(CL<=0)`, `(CL>0)!=false`, `CL>0L`, `(bool)(CL>0)`) all give p = 0.5 (H 7.93
  C 15.87): c2's loop-test probability changes only with compound (&&/||) conditions, not with the comparison spelling.
- Alignment evidence for the outer loop being top-tested in the source: retail aligns the OUTER head (0x00F20560) and not the inner
  sum loop; e5 (if-do) aligns the inner head, q2 / z1 (while) the outer -- so a goto / if-do outer is unlikely to be retail's.
- wfE3 #12 tools: tools/tsw.sh (grade + tx mismatches + target-aware hunks), tools/tgt.py (target-aware diff of a q.py output),
  tools/prb.py + tools/exmap.py (frequency probes), tmp/n12/pv.sh (batch probe runner; absolute paths only).
- Re-scored loop-shape subset sweep (advisor-requested, replaces #11's masked-hunk ranking): natural base (tmp/n12/nat15.cpp = natural
  outer while, no tokens / guards, 15 asserts) with each of the 5 counter loops (first counter 244, sum 260, inner sum 269, zeroing 288,
  post counter 296) as `for` or `{ long i=0; while (i<3) {..; ++i;} }` (32 combos, tmp/n12/ss/sXXXXX.cpp, results tmp/n12/ss.out,
  tools/tsw.sh): best 26 wrong exits (s00101 9653, s00011 10101); all-while 76..80. Stopping rule met (>= 3 mismatches): the
  frequency gap is not in the counter-loop shapes; no further token / asserts x shape work. Row PARKED (not a near-miss: the residue is
  the outer loop's lowering + a SIB no device-c placement moves; e5 relies on the non-retail `>=1`).

## wfE3 #13 (2026-11-16, successor) -- 0x00F1EED0 PlayWave: no progress; differ/00f1eed0.cpp stays e5; ~17 grades (tools/tsw.sh, ranked by target-aware hunks + tx mismatches)
Probes in tmp/n13/ (e5.cpp = differ, q2.cpp = #11 lead). Calibration: e5 9262 txmis=0 12 target hunks; q2 9118 txmis=6 69 target hunks.
- Device b on q2 (p1.cpp: `#pragma inline_depth(0)` / `()` round the zeroing-loop `if (T) return;`): that exit becomes called but the
  pad turns VSVVCSSR (~CTimer ALSO called; retail VSVVDSSR), the two called post-loop exits (retail expanded) do not flip; 8974, txmis 6.
  inline_depth(0) at a return blocks EVERY dtor of that pad, so it cannot split ~vector called / ~CTimer expanded. Dropped.
- Sibling precedent: the landed CTheRealGuildmaster::Main 0x00D5E0C0 uses a guarded do-while (`if (answer < 0) { do {..} while (answer < 0); }`)
  exactly as a static-frequency device (lower exit frequency -> its pad calls ~CTimer); HangingTreeEvil DoMission 0x00D6BFA0 uses
  `do {NSF; T..} while (HTE_State != 6);` after a known-true store. Searched rebuild/src/compiled/00/{d5,d6,d8,e6} (first, truncated pass) and then 00/f0, 00/f1 (the Arena TU) + wfcommon/style_*/src
  (tmp/n13/sib.txt): no matched row has a guarded loop with while-style rotation + cold exit. The Arena-TU main-loop idiom is
  `for (; !false; Game->NewScriptFrame())` (CArenaCellDoorGuard2 0x00F19BB0, CArenaCellExitGuard, CCellWhisper, CFlick, CNeedle, CShadow);
  NSF in the increment cannot be this loop (retail calls NSF before the first body). Applied to e5: h1 `if (CL>0) for (; !false; ) {..;
  if (CL<=0) break; }` 9997 txmis 10; h2 `for (; !false; ) { if (CL<=0) break; NSF; ..}` 10325 txmis 10. That idiom on e5 (`if (CL>0) { do {..} while (CL>0); }` g1 9345 txmis 1; with `||0` bottom g2 9278 txmis 0 125
  hunks = #10's a1; `>=1` guard braces g3 9345 txmis 1): the `||0` bottom and the `>=1` guard are both still needed.
- DERIVED from #12's probe numbers (not probed this session) c2 inline-time model: loop multiplier 8 (if-do: guard-fail E/2 = 1.98 gives almost all of C 2.14;
  while: header F = 8E = 31.7, exit C = F(1-p) = 15.87). A header-tested exit is therefore always hot; HYPOTHESIS: retail's cold post-loop exit
  needs the exit at the bottom of the body at INLINE time (after the body's T-check halvings) = guarded do-while (e5 shape); this
  CONFLICTS with #12's alignment evidence (retail aligns the outer head as the top-tested while does) -- a successor should weigh both. The
  rotation-dependent bytes (preheader `test ebx,ebx`, xor after T, outer-head alignment) come with c2-rotated while loops. Unresolved.
- Bottom break in a while (v1/v2: `while (CL>0) {..; if (CL<=0 [||0]) break; }`): 10029 txmis 11 (header exit still hot); guarded
  `for(;;)` + `if (!(CL>0||0)) break;` (v3): 9294 txmis 1 123 hunks.
- Compound while conditions of duplicated real compares (d1..d3: `while (CL>0 || CL>0 [|| CL>0 [|| CL>0]])`): c2 removes the
  duplicates (no extra compares) but does NOT rotate a compound-condition while (test stays at the loop head, back edge `je head`, const-0
  piece `xor ebp,ebp; cmp edi,ebp`): d3 9361 txmis 0 (exit kinds all right!) 244 hunks; d2 9761 txmis 5; d1 10049 txmis 9.
- Post-loop redundant-but-real guard on z1 (natural while + e5 body; y1..y3 wrap everything after the loop in `if (CL<=0 [&& ..])`):
  z1 10205 txmis 13; y1 9997 / 18; y2 9622 / 6; y3 9462 / 11 -- erratic, no closer. Two sweeps in a row moved nothing: stopped.
- State: PARKED (not a near miss): e5 relies on the FITTED non-retail `>=1` guard and the `||0` bottom; SIB 0x00F1FFB8 untouched
  (device c is meaningless until the loop region is retail).
- Not tested on this row: the wfD2 lesson's explicit-`true` loop condition (halves static frequency in and after the loop, no bytes) as
  a separate construct beyond h1/h2; the stop rule was met before it.

## wfE3 #14 (2026-11-16, successor) -- 0x00F1EED0 PlayWave: Anniversary 360 line table (THIRD SOURCE) read; e5's outer if-do is a fit
- Anniversary Xbox 360 Test build has PlayWave: public `?PlayWave@CQ_ArenaScript@NScript@@QAAXXZ` 0005:10214232 (0x9BDB58), module 142
  (FableScripting Unity_spawned_functionEtAl), source q_arena.hpp lines 1228..1606, PPC 9720 bytes. Tools/files (probe/anniv/):
  m142_lines.txt (`llvm-pdbutil dump -modi=142 -l`), m142_syms.txt, pub5.txt (section-5 publics), ann.py (annotated PPC listing from
  wfB5's test.img), pw_ann.txt (annotated listing, bl targets named), pw_sum.txt (one line per line-table entry: calls V<vtoff>/C:<name>,
  compares, T = inlined IsActiveThreadTerminating check). Post-TLC additions in the 360 file: lines 1561..1577 (answer-NO branch has more
  questions) and 1595 (AchievementInstantAction); ignore those. PDB locals (frame only): CurrentEnemies, CreatureCounter, MaxCreatures,
  MultipleBigEnemySpawnPoints, SmallEnemySpawnPoint, BigEnemySpawnPoint, AllBigSpawnMarkers, AllSpawnMarkers, DoorTimer, FurthestArenaSpawn,
  NextCreature x3, seh_Hero, speech_movie, PauseMyEntitiesPlease, CreatedObject, random_vector, spawn_pos (= the names e5 already uses).
- Line map (order matches e5 statement for statement, apart from the post-TLC additions): 1230/1231 spawn points, 1232 CTimer, 1237 the
  ShortWave/first-wave if + T, 1239/1240 keeper conversation, 1241 DoorTimer=2, 1242 = the WHOLE wait loop + post-loop T on ONE line
  (same 1246, 1250), 1257..1266 markers / NextSpawnUp / NextSpawnDown / SpawnUp / if(-1), 1272/1273 CurrentEnemies + first-wave if,
  1279 creature i `for` (guard + bottom test both under 1279), 1282 j loop guard (bottom test under 1414 with ++TotalCreatures[i]: NOT
  settled whether j is a for), 1295/1314/1362 k `for` loops (bottom under the header line), 1425 counter `for`, 1440/1441 T +
  DisplayQuestInfo(true), 1444/1446 sum loop, 1451 OUTER LOOP: the preheader `cmpwi r7,0; ble` (0x9BF354), NSF and the loop-top T check
  and the rotated bottom test `cmpwi r26,0; bgt` (0x9BF59C) ALL under line 1451 => `while (CreaturesLeft > 0) { NSF; if (T) return;`
  on one source line (script-compiler while idiom); 1453 CreaturesLeft=0; 1454 inner `for` (increment + test under 1454 = a for, not
  e5's `while (i<3) {..; ++i;}`); 1457/1459 sum + update; 1463 T; 1464..1469 ExtraCreatures block; 1473 IsLevelLoaded if + T; 1476/1478
  PlayerLeaving + zeroing loop; 1480 T (inside the if); 1484 T; 1486..1490 post counter `for`; 1494/1495 T + DisplayQuestInfo(false);
  1499 ++ArenaRoundWave / PauseCrowdChecker / NumWaves test.
- CONSEQUENCE: e5's `if (CreaturesLeft>=1) do {..} while (CreaturesLeft>0||0)` and `while (i<3)` inner loop are fits, not source;
  the real outer loop is the natural while (w1 / z1 / q2 family). The c2 frequency problem of #11/#12 (natural while = post-loop x8.5
  hotter) therefore has to be solved elsewhere (helper texts / visibility / inline control), not by the outer loop's spelling.
- za1..za8 (z1 + 1/2/3/5/8 dead FABLE_RETAIL_ASSERT right after the while): no frequency effect (10205 txmis 13 -> 10125..10317 txmis 13..18).
- Natural-base measurements this session (tools/tsw.sh, files tmp/n14/): nat.cpp (all natural) 11221 txmis 38 -- wrong exits are MIXED
  (ours-only and retail-only pads in the countdown region 0x552..0x8f4, in the creature region 0x0fab..0x10aa and after the outer loop
  0x19c2..0x23c9; callees vector / ~CTimer both, both directions) => no single callee text / visibility lead. Creature-region loops as
  `{ long V=0; while (..) {..; ++V;} }` (tools/f2w.py) on nat: all five (i, j, three k) wa 10213 txmis 47; k loops only wb 11013 / 44;
  i/j only wc (2 loops converted, checked by diff): same grade / txmis / hunks as nat (11221 / 38 / 369) -- no lead.
- State: PARKED again (not a near miss: e5's equal length exists only through the non-retail `>=1` guard + `||0` bottom, which the 360
  line table now shows are not source). differ/00f1eed0.cpp stays e5. No natural-outer source reaches retail's exit kinds: nat 38 wrong
  exits, z1 13, q2 6 only with fitted inner / zeroing / counter loops (q2 keeps e5's inner `while (i<3)`, which line 1454 contradicts;
  q2f = q2 with that inner loop as the 360-consistent plain `for`: 10797, txmis 18, 309 hunks; device b on q2 cannot split ~vector
  called / ~CTimer expanded in one pad, NOTES #13). Line table settles: outer `while (CreaturesLeft > 0)` and the inner `for` (1454);
  it does NOT settle the j loop, the sum loop or the zeroing loop (increments interleaved with body lines).
  Decision for the coordinator: does the third-source evidence rule out handing back e5-family sources (if-do, `>=1`, `||0`, inner while)?

## wfE3 #15 (2026-11-16, successor) -- 0x00F1EED0 PlayWave: mini-TU loop-shape experiment (the #12 "next lead"); re-parked, differ unchanged (e5)
- Probe files probe/m15/ (m.cpp, m2.cpp, m3.cpp + .json inline traces, .obj): one function per outer-loop shape around an identical body
  (dtor-bearing local, inner 3-count loop, several `if (T) return;`), inline probe sites PH (body after the top T) / PE (body end) /
  PC (after the loop); tools/inl/inl_host.py with OBJ_OUT, tools/inl/an.py.
- Controls reproduce the full-row model: while PH 6.25 PC(before post-T) 12.5 = C/H 2 (row: 7.93 / 15.87); if-do C/H 0.78 here (fewer
  T halvings in the mini body; the row's 0.25), i.e. while = header exit hot, if-do = bottom exit cold. Valid proxy.
- Results: (1) `while (CL>0)`, `if (CL>0) do .. while (CL>0)` and `if (CL<=0) goto out; top: .. if (CL>0) goto top; out:` compile to
  BYTE-IDENTICAL code in both minis (m: 105 instrs; m3: real-loop-sized body with U/R/A/IsLevelLoaded virtuals, 158 instrs; same
  preheader `test esi,esi; jle`, same rotation) -- the shape changes ONLY the inline-time frequencies (if-do == goto == guarded
  `for(;;)`+bottom break: cold exit; while: hot exit). (2) An explicit `continue` at the body end (or `if (L()) continue;` paths,
  `{continue;}`, `if (CL>0) continue; break;`) makes c2 rate the body end 0 and the whole loop x0.1 (PH 0.625): no use (H must stay).
  (3) No shape gives a top-tested single-compare loop a cold exit at inline time.
- ALIGNMENT is not shape evidence: neither mini aligns any loop head; in the full row retail's outer head pad is position-driven: retail
  preheader `test ebx,ebx`(2)+`jle`(6) ends at 0x00F2055B, padded with `jmp +3; lea ecx,[ecx]` to 0x00F20560; e5's `cmp ebx,1`(3)+`jl`
  ends at 0x00F2055C (one byte later) and gets no pad there (tmp/e5.q line 4228). So #12's "retail aligns the outer head => top-tested
  source" is weakened: e5's missing outer pad follows from the 1-byte longer FITTED `>=1` preheader, not from the if-do.
- Conclusion (kill rule met, ~3 compiles, 0 full grades): under VC7.1's static model no top-tested single-compare loop has retail's cold
  post-loop exit, and the loop shapes are byte-identical given equal inline decisions. So either the PC TLC source differs from the 360
  source at q_arena.hpp line 1451 (a bottom-exit form: if-do / goto / guarded for(;;)+break), or an unknown function-wide factor makes
  retail's frequencies colder (natural source misdecides in BOTH directions also before the loop, NOTES #14). Retail bytes win over the
  third source. State: PARKED; differ/00f1eed0.cpp = e5 (DIFFER(9262v9262), 12 target-aware hunks, 65/65 exit kinds equal), not re-graded.

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

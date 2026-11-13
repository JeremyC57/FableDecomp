# Hand chunk 2046 (CQ_HobbeCaveScript::CHobbeTorrent .. CQ_HangingTreeGoodScript): 2 of 2 byte-exact

## What matched (2)
### 0x00D92380 NScript::CQ_HobbeCaveScript::CHobbeTorrent::Main -- RELOCATION_MATCH 1468v1468 (grade 2 of pass 1)
- What it does: waits until the parent's NymphAngry or ReleaseBrat; then, while NumberToGenerate > 0, refreshes the "HobbeRoaming"
  list, waits for one of them to die while there are more than pGlobalScriptDef->HC_HobbeMaxNumberInRegion, and when the spawn
  timer is 0, the hero is more than 8 away and its own position is off-screen, spawns a random hobbe (rand() % 12: 1/12
  SPELLCASTER, 1/12 LIEUTENANT, 3/12 GRUNT_SPIKE, 2/12 GRUNT_HAMMER, 5/12 GRUNT) 5 units from itself as "HobbeRoaming",
  decrements NumberToGenerate and sets the timer to 2; finally removes itself.
- Extent: 1468 bytes is right (SPLITS.tsv): `ret` 0x00D92919, `mov edi,edi`, jump table 0x00D9291C (5), index table 0x00D92930 (12)
  -> 0x00D9293C; int3 x4; then 0x00D92940 = uncatalogued CQ_HobbeCaveScript::CGranny::Main (vtable 0x012D59E4 slot 1, stored by the
  landed CGranny::Alloc 0x00D8FC70 at 0x00D8FCA6; Ego_r/FableWin ?Main@CGranny@CQ_HobbeCaveScript@NScript@@EAEXXZ). The 19,950-byte
  reservation ran to the next CATALOGUED start.
- Name: Ego_r.pdb + FableWin.pdb ?Main@CHobbeTorrent@CQ_HobbeCaveScript@NScript@@EAEXXZ; retail vtable 0x012D5960 (stored by the
  landed Alloc 0x00D8F3F0 at 0x00D8F426) slot 1 = 0x00D92380 (.rdata ref 0x012D5964).
- Members: ParentClass +0x14 (CQ_HobbeCaveScript*, Ego_r pointer), NumberToGenerate +0x1C; parent NymphAngry +0x6C, ReleaseBrat
  +0x4F (engine/CQ_HobbeCaveScript.h = Ego_r - 0x14); pGlobalScriptDef->HC_HobbeMaxNumberInRegion +0xD08 (engine/CScriptDef.h).
- Shape lesson: the generator pattern -- `while (c) { Game->NewScriptFrame(); if (T) return; ... }`, `if (T) return;` after every
  loop, at the top of every for body and of every if body -- gave the whole block layout and register allocation at once; inline
  CTimer bodies over pDSTGame timer slots 87..90 copied from the landed CPreMeleeWhisper::Main 0x00D524A0; `switch (rand() % 12)`
  with the case labels grouped exactly as retail's index table (5 blocks, literal per block checked by hand: SPELLCASTER,
  LIEUTENANT, GRUNT_SPIKE, GRUNT_HAMMER, GRUNT; a CCharString initialised to GRUNT and reassigned).
- The one device: retail CALLS vector<CScriptThing>::erase 0x008AC9D0 for `hobbes.clear()`; with the inline body VC7.1 expands it
  (c2 inline trace t01: c1 67, f 5.21, score 66 < 74). Shared opt-in FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE (as the landed
  CDFSummonsCreatures::Main 0x00E74D70). Free probes (inl trace, no grade): do-while 65, while(true)+break 64, if+do-while 68,
  `&& true` 64, `|| false` 72 -- no plausible loop spelling lowers the site's frequency enough (needs f <= ~2.1).
- The one out-of-line ~vector call retail makes (0x00D928A6, the spawn block's terminate exit) came out by itself (score 125).
- Grades: g01 DIFFER(1532v1468) (erase expanded), g02 RELOCATION_MATCH. Commands: `ROW=00d92380 python3 tools/cc.py src/00d92380.cpp
  gNN; python3 tools/pd.py gNN --cf` (skeleton diff), `tools/rel.py gNN --all` (10 call symbols, 10 targets, no inconsistency),
  `tools/drel.py gNN` (every data / string / jump-table relocation paired with the retail dword at the same offset).
- Test test/00d92380.cpp: recording model; expected event lists read off the retail disassembly (scenario with a wait frame, a
  two-pass dead-hobbe wait, spawns with rand 6 and 0, a busy timer, a near hero and an on-screen spot), all 30 rand values against
  the retail index table, a terminate request at every T check (cleanup only, vector things destroyed, timer dropped once),
  NumberToGenerate 0. PASS under g++ and VC7.1.

### 0x00D77690 NScript::CQ_HangingTreeGoodScript::DoCutscene(long) -- RELOCATION_MATCH 3064v3064 (grade 2 of pass 1)
- What it does: case 0 intro (hero handle, cast HERO, CS_HANGINGTREE_GOOD, then once KickOffQuestStartScreen("Q_HangingTreeGood") and
  QuestStartScreenShown = true), case 1 execution (executioner / prisoner / hero handles, cast HERO/PRIS/EXEC,
  CS_HANGINGTREE_GOOD_EXECUTION), case 2 escape (movie + pause first; prisoner and the first two CurrentGuardVector entries; camera
  looks at the prisoner from 2 above the hero; guard 1 walks to the nearest REGION_EXIT_POINT, the prisoner follows guard 1, guard 2
  (if alive) follows the prisoner, until guard 1 is within 2 of the exit; then CameraDefault).
- Name: Ego_r.pdb + FableWin.pdb ?DoCutscene@CQ_HangingTreeGoodScript@NScript@@QAEXJ@Z; called by the landed DoMission 0x00D74430
  (0x00D74650, 0x00D74CC7); engine/CQ_HangingTreeGoodScript.h already declares it at 0x00D77690.
- Shape: cut-scene blocks spelled like the landed CV_HeroDuelScript::FirstFleeCutscene 0x00E9A970 / f4001 Gameflow Main; switch in
  source order 0, 1, 2 (dispatch dec/je, case 2 laid out first as the fall-through). Inline CScriptThing copy / dtor / IsAlive /
  GetPos bodies as the 300+ landed NScript rows write them.
- Devices: FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (shared opt-in): retail expands the pause ctor at all three
  sites; plain inline calls the case-1 site (trace i01: c1 17, f 0.0039, score 636) while the MovieBase ctor (score 748) and the
  hero handle ctor (200) at that frequency and the case-0 dtors (600) are called in retail too -- no /d2inlT separates them.
  `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` declared (retail calls 0x006E7AB0 inside the
  expanded movie dtors 0x00D77A69 / 0x00D77C43), as the landed f4001 and CPreMeleeWhisper rows do.
- Everything else (handle ctors expanded except case 1's hero handle 0x007E72A0, movie ctor called in case 1 0x006E7B60, all dtors
  called at the natural exits and at case 0 / 1 returns, fully expanded at case 2's inner wait-loop exit pad 0x00D77A8C, partly at its
  outer-loop pad 0x00D77A1A) is VC7.1's own plain-inline choice.
- Grades: h01 DIFFER(3102v3064) (pause ctor called in case 1, CCountedPointer<MovieBase> release expanded), h02 RELOCATION_MATCH
  (855/855 instructions exact). rel.py: 25 call symbols, 25 targets, no inconsistency; drel.py: strings / vtables / NullCoord /
  __real@40000000 (2.0f at 0x0122DCDC) one-to-one.
- Test test/00d77690.cpp: recording model; expected lists read off retail for case 0 (twice: start screen once), case 1, case 2 with
  guard 2 alive and dead, values -2..6 other than 0..2 do nothing, a terminate request at every T check of each case (no event but
  PauseAll(false) after it, every local destroyed, pause balanced). PASS under g++ and VC7.1.

## Parked for Fable 5.1 (0)

## Skipped
- none.

## Names
- Proven (two sources): both rows (PDB name sets + vtable slot / landed caller); CGranny::Main 0x00D92940 (uncatalogued);
  IsDistanceFromThingToPositionOver 0x00CBE45C. All callees use shared-header spellings that pair one-to-one with retail.
- Descriptive (no PDB locals exist): locals spawn_timer, hobbes, one_dead, creature (0x00D92380); seh_hero, seh_executioner,
  seh_prisoner, seh_guard1, seh_guard2, cast, movie, pause, executioner, prisoner, guard1, guard2, exit_point, exit_pos, parameter
  `cutscene` (0x00D77690; the PDB name set has no parameter names).
- NAME_FIXES.tsv: 0x00F35B30 (NScript-namespaced spelling), 0x008AC970 (eight invented spellings), 0x008AC9D0 (four).

## Header debt
- engine/CQ_HobbeCaveScript.h: the nested entity classes are forward-declared only; the CHobbeTorrent opt-in (hdr/README.md) should
  be folded in at landing (the landed Alloc / OnPersist rows of CHobbeTorrent still use their own local class text).

## Budget
- Row 1: 2 grades (pass 1). Row 2: 2 grades (pass 1). Probe compiles on top: 0x00D92380 inline traces t01, t01b, v1, v2, v3, v6, v7,
  v8 (8, no grade); 0x00D77690 inline trace i01 (1). gall.sh re-verification after comment-only edits (not counted as grades).

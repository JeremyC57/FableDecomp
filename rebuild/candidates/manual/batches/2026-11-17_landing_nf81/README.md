# 2026-11-17 landing nf81: name-fix round 7, NF81 (25 rows: CMap's theme accessors re-authored, the nf78 follow-ups)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", "add another agent to speed
up naming queue completion", then "Decrease to 2 agents at a time once next agent finishes".

NF81 took q10951 (CMap theme accessors) and the 14 entries queued at the nf78 landing (q10975..q10988), with NF78's held 0x0063A970.
Brief, list and records: `naming/2026-11-17_agentNF81/`.

## Rows (STATUS.tsv, 27): 25 ported, 2 not-a-binder (0x004E7720, 0x0059899A)
- **CMap theme group.** 0x0081EE60 SetEngineThemeAt was a legacy wave-3 row accepted on its test alone (DIFFER 174 v 207 from the repo),
  and its siblings 0x0081EFC0 / 0x0081F090 / 0x0081F170 were DIFFER too. NF81 re-authored all four on `engine/CMap.h` on the protected
  spellings both PDBs give; each matched on its first grade.
- **CWorldMap callers.** 0x0063A970 (NF78's held row, ported again), 0x0063A9E0, 0x0063AA50 and 0x0063AAB0 are on `engine/CWorldMap.h`,
  their own names now taking C2DCoordI as a class like both PDBs (q10976). 0x0063EB20 DestroyThemeAt, a SetEngineBlendAt caller the
  worklist missed, binds the right spelling.
- **q10988.** 0x008C2550 now calls 0x008C1600. It still compiles a never-expanded `inline __declspec(noinline)` copy of
  GetValidRushPosition from the new shared header
  `engine/CCombatAction_StrikeResponseJackOfBladesIntoRush_GetValidRushPosition.h`. That copy is byte-identical to 0x008C1600; without it the
  frame is 0x178 against retail's 0x16C (retail compiled the body in the same object). The coordinator accepted it as the same shape as
  CMap::GetStrongestEngineThemeAt; it joins the open whole-image link ruling (below).
- **Tree and pointer rows.** The CComboMultiplierData and EDamageAttribute tree `_M_erase` / `clear` / `~_Rb_tree` rows, 0x006DC350 /
  0x006DB060 (CPersistTraits<CComboMultiplierData>::TransferBinaryOut), CCountedPointer<CConsoleInputBase> DeleteData / Reset, and
  CFrontEndManager::GotoNewProfileScreen. 0x004E81E4 and 0x006DC020 are named as tree destructors; the bytes can not show the map
  destructor from the tree one (NOTES).

## Entries (APPLIED.tsv): 14 done, 1 blocked
q10981 (0x0059A238's own name) needs the row on engine/CFrontEndManager.h with its CObserver base; one form shifted every member by 4
bytes (queued with the list of local classes that must move first).

## Rows and checks
- **Section 1: 18 self renames (16 file moves).** STL modules match their siblings' catalog spellings.
- **Headers (opt-ins in 6 files, one new header):** engine/CMap.h (`FABLE_RETAIL_CMAP_ENGINETHEMEAT`), engine/CGameMap.h
  (`_CGAMEMAPCELL_ATMOSGETTERS`, 2 asserts pinned at 49 / 50), engine/CWorldMap.h (`_CWORLDMAP_THEMEATBLOCK`), candidates/world_map_theme.h
  (`_WORLDMAPTHEME_C2DCOORDI_SHARED`, tests only), engine/CHitParameters.h (`_CHITPARAMETERS_CLEARALLDAMAGE`), engine/CPersistTraits.h
  (`_CPERSISTTRAITS_COMBOMULTIPLIERDATA_BINARYOUT`). NF81's base copies were identical to the repo; `lane_merge/assert_names_check.py`: 0
  mismatches against 49b943df65.
- **Made-up names (descriptive):** the EThemePaintType enumerators `THEME_PAINT_ENGINE` / `THEME_PAINT_ATMOS` in the CMap.h opt-in; no source
  names them, and the repo has two other made-up spellings (queued to unify; enumerator names do not change bytes).
- **Tests:** the eight CMap / CWorldMap tests link the row objects and were rewritten minimally (each PassPattern kept).
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 25 rows graded from the repo after landing: 22 RELOCATION_MATCH and 3 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 38 binding lines and added 32; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 7)
NF81's NAME_FIXES.tsv (4: the pair scalar deleting destructor 0x004E3625, 0x0045128D's callers 0x00451AC6 / 0x0046190A, the `_M_erase`
stand-ins in 0x006DC970 and 0x00869900), plus 0x0059A238's port, the EThemePaintType names, and the GetValidRushPosition one-definition
question.

## Rule slips
- Two Bash calls exited non-zero (a Python TypeError in a test writer; a helper run outside the repo directory); both were rerun.
- q10981 was blocked after one graded form.

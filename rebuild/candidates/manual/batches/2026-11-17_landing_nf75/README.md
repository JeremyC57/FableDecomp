# 2026-11-17 landing nf75: name-fix round 7, NF75 (41 rows: follow-ups queued at nf70-nf73 and the assert audit)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF75 took the 35 follow-up entries q10910..q10944: 73 rows, plus 13 it claimed. Brief, entries, worklist and records:
`naming/2026-11-17_agentNF75/`.

## Rows (STATUS.tsv, 87)

| Status | Rows |
|---|---|
| ported and landed | 41 (40 row files + 0x008F70F0 through the header alone) |
| not-a-binder | 28 (incl. 9 CMap THEMEPALETTE users regraded) |
| blocked | 17 (0 grades each; reasons in STATUS.tsv) |
| deferred | 1: 0x006C9B70 (NF74 held it; NF74 has since landed it) |

**What changed:**
- **One name, one address for the `CCountedPointer` helpers (q10944):** 0x0059A994 / 0x0059AA86 are
  `CCountedPointer<NUISystem::CScrollableList>::ReleaseData` / `ShareData` (FableWin.pdb has both; Ego_d.pdb's ui_frontend_manager.obj
  contributes exactly this pair). The byte-identical 0x00429288 / 0x00429486 are the `CList` instance; 0x00429288 is `ReleaseData`, not
  the destructor the catalog said. Both pairs had been catalogued under one name; each now has its own.
- **NF73's held rows land:** 0x00596CCE / 0x00597006 `NUISystem::CFrontEndManager::RefreshAvailableProfiles(ForDelete)`, protected
  (`IAEJXZ`) as both PDBs give them, now bind the CScrollableList helpers; their caller 0x00597B20 follows.
- **0x008F70F0** `CMap::AddThemeDefIndexToPalette` is now protected (`IAEEK`, both PDBs) in CMap.h's `FABLE_RETAIL_CMAP_THEMEPALETTE`
  block. Every row that uses the block was rechecked (below).
- 0x006C4430 (held at nf72) now reaches FadingThings through a `friend` opt-in (`FABLE_RETAIL_CTCFADEOUTANDIN_SDD_FRIEND`) instead of an
  offset cast.
- 0x004F9784 `std::pair<long const, CComboMultiplierData>::pair` needs `// Flags: /d2inlS22` (`// Inline-budget:` line: S=10/20/21
  differ, 22 and up match). 0x00892E80 `CGameScriptInterface::ActivateQuest`, the STLport rows 0x004F9660 / 0x006C80E0 and
  `CPersistContext::TransferHelper` are descriptive or library names (NOTES).
- Others: CCreatureAction FrameUpdate rows (StartBlocking, HeroJump, CastStrengthMultiStrike), CBankFile::PostInitEntries,
  CInputManager SetEventsFromEventPackage / LoadEventPackage, CPhysicsMeshInfo::GetMesh, CBankFileAsyncEntry's constructor,
  CAIStateGroup_CreatureFindCover::IsStillRunnable, CAIStateGroup_BanditKingLunge::IsRunnable.

## Entries (APPLIED.tsv, 35): 26 done, 1 done-already, 1 rejected, 7 blocked
- Rejected: q10915 (no landed row exists at 0x00A90D60).
- Blocked: q10920 (0x00835C80, a whole-row port), q10921 / q10922 (0x006C9B70, now landed by NF74), q10923 and q10939 (the root
  fable_string.h), q10931 (0x009C8060), q10943 (0x005907AB's local CAction stand-in is CDeletion).

## Rows and checks
- **Section 1: 21 self renames (18 file moves). Section 2: 42 respelled bindings.**
- **Headers (4 files):** CCombatActionBase.h (`FRAMEUPDATE`), CCreatureAction_CastStrengthMultiStrike.h (`CLASS`; its three asserts sit
  in the `#else` arm, pinned `FABLE_STATIC_ASSERT_AT(80..82)`), CMap.h (one `public:` made `protected:` inside the THEMEPALETTE block;
  the default path is unchanged), CTCFadeOutAndIn.h (`SDD_FRIEND`). `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck_part0..2.log`):** 50 rows graded from the repo after landing (the 40 changed rows, 0x008F70F0 and the 10 rows
  that use the THEMEPALETTE block): 49 RELOCATION_MATCH and 1 MATCH, every test PASS.
- **Checks:** reloc_bindings.tsv: 119 lines removed, 89 added; bindings_multi_check is at its baseline (`NullCoord`). One stale binding
  remains: CTCInventoryExperience::AddAbilityToList 0x00570332 still binds 0x00429288 as `??1?$CCountedPointer@VCList...` (queued).

## Name fixes queued (`land1117fu`, 8)
NF75's NAME_FIXES.tsv (7) plus the 0x00570332 binding above.

## Rule slips (NF75)
- All 17 blocked rows have 0 grades. Two Bash calls exited 2. Backticks in a double-quoted note ran as a command substitution
  (rewritten). One scratch write to /tmp/x1 (removed).

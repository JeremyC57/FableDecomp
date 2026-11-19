# 2026-11-19 landing nf94: naming round 8, NF94 (81 rows; its whole slice of q11225..q11442)

Round 8: see `batches/2026-11-19_landing_nf93a/README.md`. NF94 finished its slice in one run. Records: `naming/2026-11-19_agentNF94/`
(NOTES.md "Read first (coordinator)" lists the decisions it hands back).

## Entries (APPLIED.tsv, 74): 36 done, 14 done-already, 24 blocked, 0 open
- q11424 is applied as the PDB spelling: `NThingMap::InitLayerInfo` is `YI` (__fastcall) in both PDBs, so the queued `YAXXZ` (cdecl) is
  rejected in REJECTED.tsv.
- 0x0059C747 `CTCInventoryTrade::RemoveItemFromInventory` (q11226) moved onto the shared headers. This also clears the pre-existing
  DIFFER(35v29) the nf93a includer recheck found (FINDINGS_LOG).
- **Blocked families (rows bind each other by local spellings; they need one move together):** the anim-component sort TU (q11369; a graded
  draft for 0x005E1C30 is in NF94's work/), the particle heap (q11427), CSalSwFilter (q11410..q11412; CODAApi.lib supplies the member names),
  the GetCardDef callers (q11339). **Header item:** q11441, the default slot 1 of CGameNavigatorManagerExternalInterface.h is `Initialise`
  (a default-branch rename needs a header claim). **One definition:** q11230 waits on the q11204 decision.

## Rows (81): 32 self renames (RENAMES_applied.tsv, 36 file moves), 63 respelled, 18 moved onto shared headers
Recheck from the repo after landing (`logs/recheck.log`): 81 MATCH / RELOCATION_MATCH, every test PASS.

## Headers (7 opt-ins in 5 headers; token-identical without them, 0 asserts moved)
NHeroInformationScreens_CManager.h `_INITIALISESCROLLBARINDICES`; CTCTavernGameCardBlackjack.h `_GETHANDVALUE_INLINE`; CMainGameComponent.h
`_PPLAYERINTERFACE`; CThingMapManager.h `_GETALLACTIVETHINGMAPSINAREA`; CGamePlayerInterface.h `_QUICKACCESSITEMBUTTONS_TYPED` and
`_CONTEXTSENSITIVEBUTTONS`. Its CCategory.h `_REMOVEITEM` block was already landed verbatim with nf93a (checked; not merged again).
`lane_merge/assert_names_check.py`: 0 mismatches. Direct-includer recheck (468 rows + the 14 catalog-only renames whose files moved): 481 of 482 good; 0x007E2A60 is the known on-hold row. `logs/includer_recheck.txt`.

## Checks
`naming/rebind.py`: removed 114 binding lines, added 89; `bindings_multi_check.py` at baseline (`NullCoord`); funclets 1,355 checked, 0 broken.

## Queued (`land1119nf94`, 10): NF94's NAME_FIXES.tsv.

## Rule slips (NF94 NOTES)
Two Bash calls waited on stdin (stopped); one exited 2 on an apostrophe; one grade without env.sh (NO_RESULT); a `// Flags: /Gr` line added to
0x005E1B60 was removed again (it graded DIFFER under --recheck-landed); a rows.txt lock removed by mistake and restored; one late-found binder
(0x006C3CA0) fixed and included in the final recheck.

# 2026-11-17 landing nf64: name-fix round 7, NF64 (82 rows: follow-up entries queued at nf59-nf61)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF64 took the 87 follow-up entries queued at the nf59, nf60 and nf61 landings (NAME_FIXES_QUEUE.tsv q10637..q10723): 208 rows (the
entries' landed self rows, the binders of their addresses in reloc_bindings.tsv as of 670230e4f3, and callers the evidence names).
Brief, worklist and records: `naming/2026-11-17_agentNF64/`.

## Rows (STATUS.tsv: the 208 rows + 3 held by NF62)

| Status | Rows |
|---|---|
| ported | 82 |
| not-a-binder | 82 |
| blocked | 44 |
| deferred: held by NF62 | 3 (0x006D41F0, 0x00691AE0, 0x0057770A) |

**Ported, among them:**
- 21 STLport tree `operator=` rows (were `CRetailTree_NNNN::operator=` stand-ins), now explicit instantiations of fable_map.h's
  `_Rb_tree::operator=` (opt-in `FABLE_RETAIL_STLP_TREE_ASSIGN_BODY`), plus the `map<long, long>` ctor 0x004DF86C, `~_Rb_tree`
  0x004DD2B1 (was `std::list<T_004dd2b1>::~list`) and the SExpSpendUndo map / pair rows;
- five `std::pair` constructors (four were `_unattributed::SPairLongCSecond_*` stand-ins);
- CTCQuickAccessMenu::Deactivate 0x00625920 (was catalogued `CTCCreditsUI::Deactivate`), CTCInventoryAbilities::SetAbilityLevelTo
  0x005EE01A (was `CEngineLandscapeRenderer::OnRegistedZSortingPatchRelocation`), CInputProcessHeroInformationScreens::ProcessInput,
  CNavQuadTree::ClearNodes, CTCPerceiveThing::UpdatePerception, `CPersistTraits<bool>::TransferBinaryIn` 0x00403EB0 and
  CTexture::CalcAlphaHistogram.

**Blocked (44):**
- **21 need porting first.** The row's own local STL copies clash with the shared STLport headers, or the right name depends on how
  retail merged identical functions (ICF), which needs a coordinator ruling (below).
- **9 need re-authoring**, two of them (0x005BEAD5, 0x005BF30D) with no grade.
- **13 are finished but held** (NF64 `work/ready/`, kept by the coordinator): landing them alone would split the binding of a caller
  outside the slice. Each set lands with the caller fixes queued here: six tree `operator=` rows; 0x00868CA0 with 0x007E2600 (from
  NF61); 0x00A90F00; 0x00694020 with 0x008D5000; 0x00467831; 0x004E9021.
- **0x0058148D** grades RELOCATION_MATCH but its test no longer links: the row's object now carries the STLport `erase` bodies, so
  the test's stub of `erase(const key&)` is defined twice. It is held in `work/ready/` too.

## Entries (APPLIED.tsv, 87)
- **38 done, 4 done-already, 44 blocked.**
- **1 rejected (REJECTED.tsv):** q10720 claims `?FrameUpdate@CCreatureActionBase@@QAEXXZ`. Ego_r.pdb 0x0051A410 and FableWin.pdb
  0x01DB65E0 both give `MAEXXZ` (protected virtual); that spelling is prepared in `work/ready/`.

## Rows and checks
- **Section 1: 38 self renames (64 file moves). Section 2: 115 respelled bindings.**
- **Coordinator corrections:** four container modules were respelled to the catalog's existing spelling of the same instance
  (0x00579114, 0x0057914C, 0x004DF86C), or kept the old catalog name on a leaf-only change (0x00464ACE).
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** all 82 changed rows graded from the repo after landing: 78 RELOCATION_MATCH and 4 MATCH,
  every test PASS. NF64's own final run agrees.
- **Checks:** `naming/rebind.py` removed 194 binding lines and added 99; bindings_multi_check is at its baseline (`NullCoord`).

## Headers (3 opt-ins in 3 files; default path unchanged)
`stlport/fable_map.h` (`FABLE_RETAIL_STLP_TREE_ASSIGN_BODY`), `engine/CInputProcessGameBase.h` (`_ISEVENTBUTTON`: the protected const
IsEventButtonPressed / Held / Released) and `engine/CInputProcessHeroInformationScreens.h` (`_PROCESSINPUT`). The repo's fable_map.h
and CInputProcessGameBase.h changed during the run; NF64 rebased both overlays (three-way merge, no conflicts) and reran its recheck.

## Coordinator decisions requested
- **ICF:** 0x004E37BF calls the address named `~vector<CTargetPoint>`, though CGlobalDef's members are `vector<CCharString>` /
  `vector<CDialogueLayerDef>`. 0x00CD3A16, an `operator[]` of `map<long, CCharString>`, calls the insert named `map<int, CCharString>`.
  Both look like identical functions merged at link time; which instance names the shared address needs a ruling.
- **Not yet checked:** 0x008D5000 was an extern "C" stand-in and now defines `?FrameUpdate@CCreatureAction_FireMissileWeapon@@UAEXXZ`
  (held in `work/ready/`); no caller or vtable check was run. 0x00771DE0's `clear()` now binds the vector clear at 0x00485D1A, which
  is landed under a placeholder name (one source only, not queued).
- **Weak test coverage:** the landed tests of 0x008C7480 and 0x0078B640 carry their own copy of the function, so their PASS does not
  exercise the re-authored rows (graded by bytes and bindings only).

## Name fixes queued (`land1117fu`, 50)
From NF64's NAME_FIXES.tsv: the callers that the 13 held rows must land with, and others outside its rows. For q10653, q10661, q10663,
q10664 and q10674 the caller lines come from the queued evidence and were not re-probed.

## Rule slips (NF64 `slips.txt`)
- One read-only script exited 1. Two helper runs got a bad path and wrote a bogus "recent" line into grades.log (removed; no compile
  ran). One payload got a relative output path and failed; nothing was graded and nothing was written under the repo. One extra recheck
  ran with a stale payload.
- 0x00694020 was first marked not-a-binder on the wrong q10720 claim; corrected to blocked (ready).
- One `git status` call was attempted against the brief; the permission system denied it.

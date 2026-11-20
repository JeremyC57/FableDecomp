# NF195 notes (2026-11-20, round 10: next 50 of the remaining mixed queue)

Counts (from APPLIED.tsv / STATUS.tsv): 50 entries: 12 done, 13 done-already, 1 rejected, 18 blocked, 6 deferred, 0 open.
Rows: 17 changed and finished (rows/: 2 MATCH + 15 RELOCATION_MATCH, every test PASS; 4 tests rewritten), 13 not-a-binder,
11 blocked. Final recheck: recheck_final.log (one vland run over the 17 rows).

## Done (rows changed)
- q4389 (partial, blocked): 0x006E9270 / 0x006E92C0 (NF185 drafts; tests rewritten on stlport/fable_map.h with a real map, the
  _M_find body emitted by the test, and a test-only `_Rb_global<bool>::_Rebalance` body), 0x006E9E40 / 0x006EDC50 (NF185 transformer
  with MFIND_NOINLINE: the _M_find COMDAT copy is byte-identical to owner 0x006F0250). 0x006E93C0 / 0x006E9700 blocked after 3 forms
  (frame +8: the by-value const_iterator results). 16 rows left (APPLIED).
- q4567: the six CEngineWaterRenderer rows (0x00B75CA0 0x00B77E20 0x00B7EEC0 0x00B7F0F0 0x00B7F3B0 0x00B7F600) moved off their row-local
  `template <class T> class CArray` onto engine/CArray.h (tools/qcarr.py). resize(n, value) is declared only through a new add-only
  opt-in (FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY): the existing noinline form (RESIZE_OUTOFLINE) also matched 0x00B7F600 but its
  94-byte resize COMDAT differs from owner 0x00B7EDF0 (102), so the pick-any rule rejects it.
- q4774: 0x00579FAA re-authored on stlport/fable_map.h as the map tree destructor (0x005BB04F recipe); self rename
  ??1list_T_00579faa@std@@ -> ??1?$_Rb_tree<EHeroTrainableStatType,pair<..SExpSpendUndo>..>; its only referencer 0x0057846F already binds it.
- q6206 / q6207: 0x00AAD5D0 CWaveFile::ConvertToMono on engine/CWaveFile.h + CArray.h (SIZECTOR_VISIBLE_NOINLINE + STLP_VECTOR_C);
  vector<short>(n) / operator= COMDAT copies byte-identical to owners 0x00AADD80 / 0x00AADE80. Test rewritten (links the row).
- q6240: 0x00442870 self rename to ~vector<CSubtitleRenderer::CEmbeddedHighlight> (FableWin name + Ego_r layout / caller); new add-only
  opt-in FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDEDHIGHLIGHT_NATURAL_PACK. Test rewritten on the shared headers.
- Self renames, all referencers already on the new spelling: 0x00798D30 CTCShopKeeper::GetHint (q6327/q6335), 0x006588CF
  CTCVillage::GetCostToBribe (q6328/q6336; exe scan: 4 call sites, no data / vtable refs), 0x00485D25 vector<NUISystem::CComponent*>::clear
  (q6331/q6339), 0x00F0CA30 vector<CQ_AmbushTradersScript::TeleportData>::swap (q6415, both PDBs).

## Descriptive names
- No made-up names added. STL instances carry the STLport header spelling (descriptive-by-library, EXTRA_RULES 2026-11-20):
  0x00579FAA (~_Rb_tree), 0x00485D25 (vector::clear), 0x00F0CA30 (vector::swap, also in both PDBs), 0x00442870 (~vector, FableWin name).

## Header opt-ins (hdr/README.md)
- stlport/fable_containers.h: FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY (add-only; 6 rows).
- engine/CSubtitleRenderer.h: FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDEDHIGHLIGHT_NATURAL_PACK (add-only; 1 row). Coordinator: the header's
  pack(1) makes CEmbeddedHighlight 0x11 bytes where Ego_r has 0x14 -- a header defect; making this the default is a header claim.

## Pick-any checks
- Final, unfiltered (tools/owners2.py, work/pickany_final.log): every row's object was rebuilt from the hand-back source (pS) and every
  NEW external symbol (pS minus HEAD object, 126 + 9) was matched EXACTLY against every landed row's Leaf (fresh index from
  build_candidates sources) and every reloc_bindings target, the row itself excluded. 9 owned copies, all byte-identical to the owner's
  retail bytes (relocations masked): ??9_Rb_tree_base_iterator 0x0041C830 (006e9270, 006e92c0, 006e9e40), less<unsigned __int64>::operator()
  0x00526120 (006e9e40, 006edc50), _M_find<unsigned __int64> 0x006F0250 (006e9e40, 006edc50), vector<short>(n) 0x00AADD80 and
  vector<short>::operator= 0x00AADE80 (00aad5d0). No DIFF, no NOBODY. The other 126 new symbols have no exact owner (FREE).
- 0x00442870's 12 new CEmbeddedHighlight COMDATs are all FREE. Retail binds a different spelling, ??$__destroy_aux@...@YIX...ABU__false_type@0@@Z
  (fastcall, by reference), at 0x00440BA0 (witness 0x004940AC). The row's retail bytes inline the destroy loop (they bind only
  ~CWideString and _free), so the row binds nothing at 0x00440BA0. Debt seen (not created): 0x00440BA0 is also bound as
  ?AutoTinyFinishRead_00440ba0@@ by 0x004940E3.
- 0x006588CF, 0x00485D25, 0x00F0CA30, 0x00798D30, 0x00579FAA and the six B7 rows define no owned new symbol.
- Full-set pass (tools/owners3.py, work/pickany_full.log) checks every defined symbol of both the HEAD and the hand-back object, so
  same-named COMDATs whose body now comes from a shared header are covered. AttachTextureToStage 0x009A0CF0 (00b75ca0, 00b77e20) is OK on
  both sides; the rest is as above.
- One DIFF line: hand-back 00aad5d0 still emits ??4?$CArray@F@@ (19 bytes; HEAD 89) against 0x00AADE80 (236). HEAD was DIFF as well, and
  the only reloc_bindings witness for that spelling at 0x00AADE80 is HEAD 0x00AAD5D0 itself. The hand-back binds
  ??4?$vector@F...@std@@ there instead (OK, 236 = 236; RENAMES section 2). Once landed, nothing binds the CArray spelling at 0x00AADE80
  and the COMDAT is an unbound wrapper. Not a new pick-any violation.
- Debt seen (not created): 0x00AAD650 CWaveLoader::LoadWaveFile defines `template<> vector<short>::vector(size_type)` as a strong
  specialization while owner 0x00AADD80 defines it by explicit instantiation; 0x00AAD5D0 now adds a third, byte-identical COMDAT copy.

## Coordinator decisions needed
- q6310 0x00A58720: the self rename to the anonymous-namespace CTransformWorldCoordsToPlaneCoords::operator() adds a class token the row
  lacked (sibling shape exists in 0x00A593F0 / 0x00A58B50, no header, no base) and needs scaffolding to emit an internal operator() out of
  line; the ?A0x hash is fixed only at landing. Not applied.
- q6373 / q6384 0x00E81A60: the row defines ?CheckForFish@CFishCompOrganiser@@QAE_NXZ, the same symbol the real CheckForFish row
  0x00E81AD0 defines (two strong definitions in landed rows). The fix needs the nested NScript::CV_FishingCompetitionScript::CFishCompOrganiser
  in engine/NScript_CV_FishingCompetitionScript.h (header item).
- q5215 0x009EA3A0: needs a CConsoleCommandBase.h header claim (+4 CCharString member, protected virtual GetFormattedName).
- q6245 0x00A98600: the port onto CCountedPointer.h depends on why retail runs no ~CCountedPointer / EH frame for the zeroed temporary.

## Follow-ups seen (not in this slice)
- 0x00579ECC / 0x00579FC1 / 0x005796AB / 0x00579616: the EHeroAbility / UpgradableStats twins of q4774 (OnKill / list_T_ stand-ins).
- 0x00485AA7 vector<NUISystem::CComponent*>::erase (NAME_FIXES line). Full census (reloc_bindings):
  - The STLport range-erase spelling is bound only there: witness 0x00587847, now also 0x00485D25.
  - The single-element erase (0x00485468) is a different symbol.
  - Stand-ins: ?erase@CComponentVector@@ (0x0047DBB8, 0x0059EC48), CRetailVector (0x00587A41), CPODVector (0x0047E07F).
  - HEAD 0x00485D25 bound ?Apply@CClass_00485d25@@ there; this hand-back removes it.
  - The self row has no Leaf and models CGuiControlListBox::SelectEntryFromData.
- 0x00B7EDF0 self row Leaf is cut (`?resize@?$vector@UCPreTransposedBoneMatrix@@`): should be written in full at its next touch.
- q6387: 21 named stand-ins for the 0.0f literal 0x0122DEDC (work/q6387_standins.tsv).

## Funclets
- No renamed self row (0x00442870, 0x00579FAA, 0x00798D30, 0x006588CF, 0x00485D25, 0x00F0CA30) is included by any funclet or other source
  (grep of each old file name over rebuild/src and rebuild/tests).

## Flags lines
- None added.

## Rechecks
- q6320 / q6374 / q6378 (done-already): probe.py compiled all 45 REFS rows (work/probe_q6320.tsv). The 36 rows for 0x009A08B0 and the 9 rows
  for 0x009AC2D0 all bind the claim spelling, so the status rests on compiled objects, not only the table.
- RENAMES section 1 old_leaf now holds the bare symbol from each HEAD object; remarks moved to evidence.

## Rule slips
- One Bash call exited non-zero (a `cat` of two lock files that did not exist yet, q4774).
- Entries q6381-q6383 (done-already) and q6320 were recorded before q6373 / q6374 / q6377 / q6387 in time; each entry was still taken to
  its terminal state on its own.
- First pick-any pass on 0x00442870 was piped through `grep -v DIFF` and checked only the self Leaf. Redone unfiltered (above).
- q6320 / q6374 / q6378 were first graded done-already from reloc_bindings alone. Since confirmed by probe (above).
- 0x00AAD5D0 took 2 failed front-end compiles (FABLE_STATIC_ASSERT needs rebuild_abi.h) before its forms; counted in grades_used.

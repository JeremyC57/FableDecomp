# NF75 notes (round 7, land1117fu q10910..q10944)

The counts come from STATUS.tsv and APPLIED.tsv.

**Rows: 41 ported, 28 not-a-binder, 17 blocked, 1 deferred (87 in all).**
- 73 rows come from rows.txt: 37 ported, 19 not-a-binder, 17 blocked.
- 14 rows outside rows.txt:
  - 13 claimed with claim.sh (nflocks):
    - the 9 landed CMap THEMEPALETTE users: regraded on the CMap.h access edit, all not-a-binder;
    - 0x0059A994, 0x0059AA86, 0x00429288 and 0x00429486: ported for q10944.
  - 0x006C9B70: never mine. NF74 holds it; recorded `deferred: held by NF74`.
- 0x008F70F0 is ported by header only: the CMap.h overlay changes its object symbol to IAEEK, and the row text is unchanged, so it has no rows/ file. It has a RENAMES section-1 line. This is why 41 rows are ported but rows/ has 40 .cpp files.
- Landed callers of AddThemeDefIndexToPalette:
  - 0x0081EF30, 0x008203A0 and 0x008F71A0 already declare it IAE on local models, and none of them include engine/CMap.h.
  - 0x0081EE60 is in NAME_FIXES.tsv.
  - 0x0081EA50 also mentions THEMEPALETTE, but only in unlanded pending/differ drafts.

**Entries: 26 done, 1 done-already, 1 rejected, 7 blocked (35 in all).**
- done-already: q10934. The self row and the header already spell EndMission correctly.
- rejected: q10915. No landed row exists at 0x00A90D60 (REJECTED.tsv).
- blocked: q10920, q10921, q10922, q10923, q10931, q10939 and q10943.
  - q10922's only row left is 0x006C9B70 (deferred: held by NF74).

recheck_final.log covers every changed row plus the 9 CMap users: 50 rows, 49 RELOCATION_MATCH and 1 MATCH, all PASS.

## Descriptive names
These names are descriptive, either by library or made up.
- **0x004F9784, ??0?$pair@$$CBJVCComboMultiplierData@@@std@@QAE@ABU01@@Z:** descriptive by library. It is the STLport pair copy ctor, which Ego_d carves into the CComboMultiplierData TU. The row is the 0x004F9763 TU under `#pragma optimize("s", on)` with `// Flags: /d2inlS22` and an `// Inline-budget:` line. Grades: S=10/20/21 give DIFFER (29v47 bytes, copy ctor not inlined); S=22/23/25/30/40 match.
- **0x004F9660:** STLport `_Rb_tree<long, pair<long const, CComboMultiplierData>>::operator=`, descriptive by library (TREE_ASSIGN_BODY).
- **0x006C80E0:** STLport `vector<pair<long, COpinionDeed*>>::_M_insert_overflow`, descriptive by library.
- **CPersistContext::TransferHelper(CWeaponTrailGraphicSet*) 0x004E8FCD and TransferHelper(CObjectAugmentationParticleSet*) 0x004E9550:** the queue spelling. No PDB names these out-of-line helpers. They are used consistently, one name per address.
- **CAddFreeSeatToList (0x00938BF0):** the FableWin.pdb functor name, not made up.
- **0x00892E80 `CGameScriptInterface_ActivateQuest` (free __stdcall stand-in, RENAMES section 1):** descriptive, made up. The retail function is the virtual ?ActivateQuest@CGameScriptInterface@@UBEXABVCCharString@@@Z. The row keeps the landed free-function spelling until CGameScriptInterface is ported.

## Header opt-ins
See hdr/README.md:
- CCOMBATACTIONBASE_FRAMEUPDATE
- CCREATUREACTION_CASTSTRENGTHMULTISTRIKE_CLASS
- the CMap.h THEMEPALETTE access edit
- CTCFADEOUTANDIN_SDD_FRIEND

The rows also use existing repo opt-ins:
- MAP_COPY_OUTOFLINE
- TREE_ASSIGN_BODY
- FIND_RANDOM_DISPATCH and FIND_NOINLINE_BODY
- VECTOR_ERASE_RANGE_OUTOFLINE
- ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
- CCOUNTEDPOINTER_RELEASEDATA and RELEASEDATA_DECL_ONLY
- CWORLDMAP_STRONGESTTHEME
- CQUESTMANAGER ACTIVATEQUEST

## q10944 (CScrollableList / CList counted pointers)
- **CScrollableList pair:** 0x0059A994 and 0x0059AA86 are CCountedPointer<NUISystem::CScrollableList>::ReleaseData and ShareData. Evidence:
  - Ego_d's ui_frontend_manager.obj contributes exactly this pair.
  - FableWin.pdb has both symbols.
- **CList pair:** the byte-identical pair is 0x00429288 (ReleaseData, not the destructor) and 0x00429486 (ShareData). Retail has no ICF, so these are different names at different addresses.
- **Drafts:** rows/00596cce.cpp and rows/00597006.cpp are the NF73 held drafts (batches/2026-11-17_landing_nf73/held/), respelled to CCountedPointer<CScrollableList> with NUISystem_CScrollableList.h.

## Blocked plans
- **q10923, 0x00429950 operator< (13 rows: the self row plus 12 binders).**
  - The self row needs a single definition: the q10763 change, held in batches/2026-11-17_landing_nf70/held/q10763_change.txt, makes the body an opt-in in fable_string.h. That is a root fable_string.h overlay, which this brief forbids.
  - The 12 binders each model CBasicString / CCharString locally, so each needs a port onto the shared string header first. They were not attempted (0 grades), and the NF70 verdict is kept.
- **q10920, 0x00835C80:** the whole row lives in namespace ExternalRetry_00835c80 and binds 10 callees under namespaced spellings. It needs a full port. 0x0082D920 is ported.
- **q10921, 0x006C80E0:** the self row is ported. The only binder still on the stand-in spelling is 0x006C9B70, which NF74 holds.
- **q10931, 0x009C8060:** needs three Sizeof opt-ins and ports of the local CResource / vector / CCountedPointer / NGameText::CGroup models. Not attempted.
- **q10939, 0x0099B2C0:** needs an inline GetLength / text accessor in the root fable_string.h, which is forbidden here. 0x009CBE10 already binds ToCharString; its reloc_bindings line is stale.
- **q10943, 0x005907AB:** the row's local NUISystem::CAction stand-in (8 bytes: type + param list) is really CDeletion (Method + AssociatedParents). It needs a NUISystem::CComponent / CDeletion port first. 0x00597006 is ported.

## Wrong spellings seen elsewhere (not fixed here)
- **0x008FCF90:** carries several different spellings across landed callers.
- **0x004E8CE9:** still has the local CWeaponTrailMap / CArrowTrailMap stand-ins. Only the TransferHelper calls were respelled.
- **0x00892E80:** the row's own function is a free stand-in, `CGameScriptInterface_ActivateQuest`. The retail virtual is ?ActivateQuest@CGameScriptInterface@@UBEXABVCCharString@@@Z.
- **0x00892EC0:** sits on a local CGameScriptInterface_Methods model.
- **0x00541120:** the catalog name is vfunc_64.
- **0x00597B20:** defines GotoProfileMenu as public QAE, but both PDBs say protected IAE (NAME_FIXES.tsv).
- **Further fixes:** NAME_FIXES.tsv also lists 0x004F97B3, 0x004E7720, 0x004F9660, 0x0081EE60, 0x009F5170 and 0x00429288.

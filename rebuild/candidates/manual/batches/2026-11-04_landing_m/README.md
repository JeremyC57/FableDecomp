# Landing 2026-11-04m: chunk 749 (NUISystem CTextSlider / CText / CMesh), 7 landed, 2 parked

## Result
- **Landed 7 rows** (`logs/land.log`). Before landing, all 7 were graded read-only on the merged tree with no overlay
  (`logs/new2.log`): RELOCATION_MATCH + PASS.
  - 0x00549440 `NUISystem::CTextSlider::ProcessEvent` (slot 1 of the CObserver vtable 0x01248A68).
  - Six `vector deleting destructor` adjustor thunks, `adjustor{4}` and `adjustor{24}`, for CTextSlider, CText and CMesh. Their catalog
    name was "vfunc_0". The thunks reference `??_E`, the alias VC7.1 gives `??_G`, so their RELOC_NAME warnings are harmless.
  - No row source or test contains asm, a pragma or `volatile`.
- **Parked 2** (`pending/2026-11-04_linux_hand749/`, each with a `// Grade:` line and a passing test):
  - 0x0054EF00 `NUISystem::CText::Draw`, DIFFER(1127v1127): the slots of five small locals plus one ecx/edx swap.
  - 0x005502E0 `NUISystem::CMesh::Draw`, DIFFER(1321v1288): zoom spill and x87-vs-integer copies of the box values.
  - Both drafts use the shared `GFFloatToLongNear` helper for the bare `fistp dword`. The C++ attempts at that site come first and are
    in NOTES.md.

## Headers (11 changed, 7 new; every change sits behind a retail macro)
- The overlay base is 7d2859ef7. None of these headers changed in the repo since then.
- hdrcheck2 before the merge: `default=same` for every header (`logs/hdrcheck2_pre_merge.txt`).
- Changes to existing retail branches:
  - FABLE_RETAIL_CTEXT: slot 142 is GetValue (it was named GetAlignment), and slots 146-150 are added.
  - FABLE_RETAIL_CSYSTEMMANAGER adds GetFontManager.
  - FABLE_RETAIL_STLP_LIST adds erase / pop_front.
  - FABLE_RETAIL_CMANAGER adds GetMeshBank.
- New macros:
  - FABLE_RETAIL_CTEXT_LAYOUT and FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE.
  - Used by the parked CMesh draft only: FABLE_RETAIL_C3DANIMATIONSTATE, _CBANKFILE_ENTRYQUERY, _CMESHDATABANK_BASES, _CMESHDATABANKENTRY,
    _C2DBOXF_CORNERS.
- New headers: NUISystem_CTextSlider.h, NUISystem_CMesh.h, CEnginePrimitive2DText.h, C3DOrientationInterpolator.h,
  C3DAnimationBlendState.h, CEnginePrimitive2DStaticMesh.h, CEnginePrimitive2DAnimatingMesh.h.
- **Siblings:** `includers.py` found 3,609 files; `ppcmp.py` (`logs/ppcmp.txt`) shows 3,597 same and 12 different.
  - The 12 were rechecked on the merged tree (`logs/recheck.log`): 12 / 12 RELOCATION_MATCH + PASS.
  - Four of the 12 are the chunk-750 rows landed just before; the chunk mirror predates their header change.
  - The agent's own check of 16 includers is in `pending/.../SIBLINGS.tsv`.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04m.tsv` (20 labels, from the PDB symbols in the chunk's LABELS.tsv).
  `apply_labels.py` made 17 catalog and 19 manifest renames: the 7 rows, CText / CMesh `Draw` (slot 2), CText slots 142-150 and
  CMesh slots 142-148.
- **Queued PRIORITY** (tag `land0411m` in `NAME_FIXES_QUEUE.tsv`): 12 entries, for example:
  - CText GetValue vs GetAlignment;
  - the CEnginePrimitive2DText ctor takes `const CFontBank&`;
  - 0x0042B707 is list::size;
  - 0x013B8A98 is `CList::GPreviousSelected`;
  - 0x00A24270 PeekMesh (the bound name GetMeshInfo has no PDB symbol).
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage (`write_decomp_dashboard.py`, `update_readme_progress.py`)
46,607 genuine of 49,568 (94.03%); 5,800,127 function bytes (56.72%).

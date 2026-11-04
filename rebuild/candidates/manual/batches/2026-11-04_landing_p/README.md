# Landing 2026-11-04p: chunk 752 (NUISystem CSlider / CMorphingSprite / CScrollBar), 6 landed, 3 parked

## Result
- **Landed 6 rows** (`logs/land.log`). Before landing, the 6 were graded read-only on the merged tree with no overlay
  (`logs/new2.log`): RELOCATION_MATCH + PASS.
  - The 6 are the destructor adjustor thunks (`adjustor{4}` / `adjustor{24}`) of CSlider, CMorphingSprite and CScrollBar, slot 0 of
    each class's +4 and +0x18 vtables. They match from a TU that defines the virtual destructor out of line.
  - No row source or test has asm, a pragma or `volatile`.
- **Parked 3** (`pending/2026-11-04_linux_hand752/`, each with a `// Grade:` line and a passing test):
  - 0x0054AC90 `NUISystem::CSlider::ProcessEvent`, DIFFER(1454v1454), 26 lines after 36 grades:
    - `lea ebx,[edi*8]` is scheduled early in one of the two identical action loops.
  - 0x00554740 `NUISystem::CMorphingSprite::Draw`, DIFFER(1524v1572):
    - VC7.1 calls the handle `Clear` / ctor out of line at two cold exits, where retail expands them.
    - `do{}while(0)` or `__forceinline` closes that gap. Neither is a proven retail construct, so neither is kept.
    - What is left after that is register colouring.
    - The two bare `fistp dword` sites use GFFloatToLongNear, with the per-site C++ attempts table in its NOTES.md.
  - 0x00559A90 `NUISystem::CScrollBar::ProcessEvent`, DIFFER(2144v2163): frame-slot sharing only (retail `sub esp,5Ch`, ours `30h`).

## Headers
- Two existing headers changed, behind their retail macros. The agent rebased its overlay on the current repo include, and both overlay
  copies are pure additions to the repo copies.
  - `C2DVector.h`, under FABLE_RETAIL_C2DVECTOR: `extern DELTA_NOTIONAL_ZERO` and a `__forceinline operator==`. The evidence is Ego_r
    `??8C2DVector` and five uncalled out-of-line retail copies.
  - `NUISystem_CManager.h`, under FABLE_RETAIL_CMANAGER: GetMouseMovement and GetGraphicsBank.
- New headers: NUISystem_CSlider.h, NUISystem_CMorphingSprite.h, NUISystem_CScrollBar.h, NUISystem_CActionParamFloat.h.
- hdrcheck2 `default=same` (`logs/hdrcheck2_pre_merge.txt`).
- **Siblings:** `includers.py` found 121; `ppcmp.py` reports 103 same and 18 different. All 18 were rechecked on the merged tree
  (`logs/recheck.log`): RELOCATION_MATCH + PASS.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04p.tsv`, 24 labels demangled from the chunk's PDB symbols. `apply_labels.py` made
  19 catalog and 22 manifest renames:
  - the 9 rows;
  - the three classes' slot-0 `vector deleting destructor` (Ego_r `??_E`);
  - CMorphingSprite slots 145 / 149-151 (PDB spelling `GetGRaphicIndices`) and CScrollBar slots 154-159;
  - the map::operator[] and list::push_back instances.
- **Queued PRIORITY** (tag `land0411p`): 12 NAME_FIXES, e.g. the landed slot-0 stand-ins and `??1CSlider`, and the
  CGraphicInfoFrame / CResource / CManager::GetUIScale spellings.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

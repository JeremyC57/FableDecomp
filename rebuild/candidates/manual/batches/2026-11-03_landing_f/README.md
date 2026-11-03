# Landing 2026-11-03f: chunk 735 (front end + inventory experience): 4 rows, 0x00B3EFA0 respelled, chunk 735 names

## Result
- **Landed 4 rows** (`logs/land4.log`), all RELOCATION_MATCH + PASS on the merged tree (`logs/new4_regrade.log` before landing):
  - 0x0047F58B `CTCInGameMenu::AddRegionAndTimeInfo` (`/GS`)
  - 0x00597228 `NUISystem::CFrontEndManager::AddRegionAndTimeInfo` (`/GS`, byte twin)
  - 0x005729D8 `CTCInventoryExperience::AddStatToList`
  - 0x00573792 `CTCInventoryExperience::AddStatToListPC`
  - The oracle rows equal the retail bytes; each is followed by a `push ebp`.
- **Re-landed 0x00B3EFA0 `CEngineMap::ReadMapInfoBlock`** with `sib/00b3efa0_revised.cpp` (the `ReadSLONG()` / `ReadULONG()` spelling).
  - On the merged tree the landed source grades DIFFER(1597v1597) and the revised one RELOCATION_MATCH (`logs/b3_grades.log`).
  - `repair_remove.py 00b3efa0`, then `vland --land`: RELOCATION_MATCH + PASS (`logs/repair_remove.log`, `logs/b3_land.log`). The file name and test are unchanged.
- **Not landed:** the three parked chunk 735 rows stay in `pending/2026-11-03_linux_hand735/differ/`:
  - 0x00570332 AddAbilityToList: DIFFER(3918v3918);
  - 0x00571280 AddAbilityToListPC: DIFFER(3962v3962);
  - 0x00577AC0 OpenInventoryInternalPC: DIFFER(2157v2157).
- **Coverage** (`write_decomp_dashboard.py`, `update_readme_progress.py`):
  - 46,548 genuine, 93.91% of the 49,564-row catalog (was 46,544);
  - 54,081 / 57,609 known, 93.88%;
  - 5,719,711 bytes, 55.92% of function bytes (was 5,708,049); 38.495% of `.text`;
  - landed rows 54,090 -> 54,094.

## Part 1: header overlay merge (21 changed, 7 new)
- I diffed each overlay header against the current tree. Every overlay file holds every line of its repo copy, except CDataInputStream.h, where the
  replaced lines are chunk 735's own `Read` change. No round e branch is reverted.
- **Chunk 733 lines found in 735's overlay.** The chunk 735 mirror included part of chunk 733's staged overlay.
  - Kept, because 735's rows define these macros:
    - the `FABLE_RETAIL_CFILEDATAINPUTSTREAM` branch;
    - the `FABLE_RETAIL_CTCINGAMEMENU` skeleton (InGameMenuOpen +0x0C, the inline StatGetInterfaceType / IsOpen).
  - Dropped, because no 735 row uses them:
    - `CDataInputStream::ReadString`;
    - `CPersistContext(CCharString&)`;
    - `CTCInGameMenu::VideoOptionsChanged` / `VideoOptionsNotAccepted`.
  - `NUISystem_CUIScreenUnitConverter.h` is chunk 735's getters-only version, not chunk 733's public statics.
- New headers: CSliderAnimator.h, NHeroInformationScreens_CManager.h, NUISystem_CYesNo.h, NUISystem_CUIScreenUnitConverter.h, plus
  CPersistContextReadSection.h, NUISystem_CActionParamSLONG.h and NUISystem_CComponentContainer.h (chunk 735's earlier additions).
- `hdrcheck2` (HEAD copies against the merged headers) prints `default=same` for 21 of 21, with one FABLE_RETAIL_<CLASS> branch per class
  (`logs/hdrcheck2_overlay_merge.txt`, `logs/hdrcheck2_final.txt`).
  - The `(cpp-err)` on the CTCHeroExperience branch is the CDefPointer.h `#error` gate. It is the same on both sides.
- **Checked branch 1: `__forceinline GetRGBColour`** (FABLE_RETAIL_CRGBFLOATCOLOUR). The evidence holds:
  - retail 0x00570332 calls `__ftol2` 0x00BFEA70 four times (0x00570785..0x005707AB) and never calls the COMDAT 0x00429880 (FableWin
    `?GetRGBColour@CRGBFloatColour@@QBE?AVCRGBColour@@XZ`, landed).
  - I added that evidence, and "plain inline is called out of line under /Os", to the branch comment. Only the parked ability rows use it.
- **Checked branch 2: `FABLE_RETAIL_STLP_VECTOR_ASSIGN`.**
  - STLport 4.6.1 `_vector.h:260` declares `vector<_Tp,_Alloc>& operator=(const vector<_Tp,_Alloc>&)`, and `_vector.c:90` defines it out of class.
  - The branch declares the same signature and no inline body. Only the parked 0x00577AC0 uses it.

## Siblings (`sibling_grades.tsv`, `logs/final_recheck0*.log`)
- 3,185 landed sources and tests reach a changed header (landing d `includers.py`). `ppcmp.py` against the HEAD headers:
  3,166 token-identical, 19 differ (`logs/ppcheck.txt`).
  - The 19 are the 16 SIBLINGS.tsv rows (0x00B3EFA0 now in its revised form) plus 0x004A5A40 CWorld::UpdateFrame, 0x005EB9E0
    CTCTavernGame::FrameUpdate and 0x008EDFF0 CWillButtonHelperTimer.
  - All 19 were regraded, so no declaration-only split was needed.
- **Final `--recheck-landed` on the final tree:** 70 rows, 67 RELOCATION_MATCH + 3 MATCH, all PASS. The 70 are:
  - the 19;
  - 37 rows sampled from the 3,166 identical ones (every 80th);
  - the 4 new rows;
  - the 10 respelled rows.
- No row that matched before regressed, so no header branch needed fixing.

## Names (`NAME_DECISIONS.tsv`, `RESPELL_QUEUE.tsv`, `ghidra_out/labels_landing_2026-11-03f.tsv`)
Every decision cites two sources:
- the PDB name sets (Ego_r / FableWin) or the Ego_r layout;
- the retail bytes, the call sites, or vland's relocation identity in the matched rows.

**Applied as written:** 22 of the 25 NAME_FIXES, plus most LABELS.

**Applied with a corrected spelling:** 0x004861DF is the one-argument `?resize@?$vector@EV?$allocator@E@std@@@std@@QAEXI@Z` (Ego_r).
- Its body passes a zeroed byte temporary to resize(n, const T&) 0x00486189.
- The agent's `QAEXIABE` is that callee's form.

**Rejected:**
- 0x0066E799 SelectScreenOfType: one source only, as instructed.
- 0x00579EA3 / 0x00579ECC `map::clear`: whether the COMDAT is map::clear or `_Rb_tree::clear` is unproven. Both are in-class inline and give
  the same bytes.
- 0x00579114 / 0x0057914C `map::_M_find`: owner D1 makes `_M_find` an `_Rb_tree` member template.
- 0x00579958 insert_unique is deferred. Its definition row's Leaf would re-sync the catalog, so it is queued instead.

**Lander-found and applied:**
- 0x006D64A0 `CTCHeroExperience::GetExperienceSpentOnStat`:
  - Ego_r + FableWin name;
  - the body reads +0x44, which is ExperienceSpentOnStats in the retail layout;
  - the Ego_r donor order matches.
  - 0x005724CD bound `GetTrainableStatLevel` (a `_Calls` stand-in) to 0x006D64A0; the real GetTrainableStatLevel is 0x006D6520.
- 0x0041CF47 `GetUIScale` is `const`, returning `class C2DVector` (both PDBs).
- 0x0056F69F `AddMagicCategoryToList` is private (both PDBs).

**Respelled in this landing.** Every row was graded before (`logs/nf_base.log`) and after (`logs/nf_respell.log`, `logs/nf_006d6850.log`):
all RELOCATION_MATCH + PASS, except 0x006D6850, which is MATCH both times. Their `reloc_bindings.tsv` rows were rebound (10 rows).

| Row | Respell |
|---|---|
| 0x0056F69F | AddSoundAction -> private AAE; the row's own AddMagicCategoryToList -> private; `// Leaf:` added |
| 0x0056F615 (definition) | `struct CGUIElement` -> `class NUISystem::CComponent`, private AAE, `AddUIActionParam(CActionParamString*)` -> `AddParam(CActionParam*)` (CActionParamString derives from CActionParam); Leaf added; test renamed to match |
| 0x0057229B | GetCategoryTrainableStatIsIn const (QBE) |
| 0x006D6850 (definition) | GetCategoryTrainableStatIsIn const + Leaf |
| 0x006D7CB0 | GetExperienceCostToUpgradeAbility const (QBE) |
| 0x0041CF47 | free `GetScreenWidth/Height` -> `NUISystem::CUIScreenUnitConverter::GetBackBufferWidth/Height` (new header); own symbol const + Leaf |
| 0x0054B4B0, 0x00555EB0 | `CActionParamLONG` / `CActionParamLong` -> `class NUISystem::CActionParamSLONG` (vtable 0x012366F4; 0x00555EB0 test renamed) |
| 0x00570161 | `GetStatExperience(long)` -> `GetStatExperience(EHeroExperienceStatCategory) const` |
| 0x009AD6E0 | `CPersistContext(CDataInputStream*, bool)` ctor stand-in -> `CPersistContext context; context.Init(stream, flag)` (0x009BAA30 has no `this` return) |

**Queued PRIORITY** (17 lines, tag `land0311f`, appended to `naming/NAME_FIXES_QUEUE.tsv`). These are invented stand-ins or placeholder
definition rows that need a re-model, not a rename:
- `_Calls` binders 0x005721FA / 0x005724CD / 0x0056E93D;
- 0x00437CD0 binders (5) and its definition row;
- 0x004861DF binder 0x009E95F0 and its definition row;
- 0x0042B646 binders (2) and its definition row;
- definition rows:
  - 0x006D6510 / 0x006D6520 / 0x006D64A0;
  - 0x0041CC14 / 0x0041CC2B (blocked on the statics' access);
  - 0x0056E545, 0x0057955A, 0x00578FCE, 0x00579958;
  - the CCountedPointer dtor / ShareData rows;
- the map-clear binder 0x0057770A;
- the GetUIScale binder 0x0052ECC0;
- the `~_List_base` binding from `stlport/fable_containers.h` for 0x0042ABCA. Neither PDB has any `~_List_base`; FableWin has `~list`.

**Labels:** 33 entries. `apply_labels.py` made 20 catalog and 26 manifest renames. Then `sync_catalog_names.py` (0 renames) and
`sync_manifest_from_catalog.py --labels` ran (`logs/`).

## Final checks
- `bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` prints 0.
- Catalog rows 54,094 == `Address = '` entries 54,094.
- `sync_manifest_from_catalog.py --labels --dry-run` reports 0 changes.
- `git diff | grep -c /tmp/claude` is 0, and the new files hold no `$S` path.
- hdrcheck2: 21 of 21 `default=same`.

## Procedure
1. Merge the overlay: strip the chunk 733 lines, edit the GetRGBColour comment, run hdrcheck2.
2. Find the includers, pp-compare them, regrade the 4 rows on the merged tree.
3. `fixmove.py`, `keypayload.py` (0 texts changed), asm/volatile grep (clean), `vland --land` for the 4 rows.
4. Grade 0x00B3EFA0 in both forms, `repair_remove.py`, `vland --land` the revised source, `git reset`.
5. Name verification, respells (baseline regrade, edits, regrade, rebind), queue, labels, sync.
6. Final sibling recheck (3 shards), dashboard, final checks, GOTCHAS (2 lines).

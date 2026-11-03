# Landing 2026-11-03g: chunk 736 (CNavQuadTree / GameCODA voice): 4 rows, nav header branches, 18 respells

## Result
- **Landed 4 rows** (`logs/land4.log`). All four grade the same and PASS on the merged tree (`logs/new4_regrade.log`, before landing):

  | Address | Name | Grade |
  |---|---|---|
  | 0x00C1EE30 | `CEventTriggerRef::GetDriverCount` | MATCH |
  | 0x00C5A1F0 | `CSoftwareVoice::Stop` | RELOCATION_MATCH |
  | 0x00C5A2A0 | `CSoftwareVoice::Reset` | RELOCATION_MATCH |
  | 0x00A78E20 | `CNavQuadTree::IsAreaClear` | RELOCATION_MATCH, grade `retail_asm` |

  - 0x00A78E20 has no asm of its own. It calls the shared `engine/GFFloatToLongFloor.h` helper and carries its own `// Retail-asm:` line.
  - It keeps two kinds of body out of line, both same-TU under `auto_inline(off)`: the growth specialisations and `IsAreaBlockedByLines`.
  - The catalog labels for these addresses were CMap::GetWorldMapNumber, CMap::Update (twice) and `CNavQuadTree_InitializeLeafNode`. The labels file corrects all four.
- **Not landed** (left in `pending/2026-11-03_linux_hand736/`):
  - 0x00A7A0A0 SetUpRegions is parked at DIFFER(1270v1222). On the merged tree it still grades DIFFER(1270v1222) PASS (`logs/parked_00a7a0a0_merged.log`).
  - 0x00A7AF60 CNavQuadTree::LoadFromFile and 0x00821A80 CMap::LoadFromFile were not attempted. Their labels are applied.
- **Class choice for 0x00C1EE30.** It is landed as `CEventTriggerRef`, the spelling bound to its own address (by 0x00C2E4D0).
  - The callers also bind the ctors 0x00C1EE60 / 0x00C1EEE0 as `??0CEventTriggerRef`.
  - The landed sources of that object say `CItemAction`.
  - libpk has no PDB or RTTI, so neither name has a second source. Queue row 590 stays open; a `land0311g` NOTE line was added.
- **Coverage** (`write_decomp_dashboard.py`, `update_readme_progress.py`):
  - 46,552 genuine, 93.92% of the 49,564-row catalog (was 46,548);
  - 54,085 / 57,609 known, 93.88%;
  - 5,720,846 bytes, 55.93% of function bytes (was 5,719,711); 38.502% of `.text`;
  - landed rows 54,094 -> 54,098; `retail_asm` 187 -> 188.

## Part 1: header overlay merge (4 changed, 3 new)
- Every overlay file holds every line of its repo copy: `diff` removes 0 lines from each. Nothing that landings e or f added is reverted.
- `engine/CANavQuadTreeNode.h`, branch `FABLE_RETAIL_CANAVQUADTREENODE`:
  - chunk 721 round 4's polymorphic class (slots 0..8, operator==, GetPosition, the Ego_r members);
  - plus chunk 736's slots 9..15 and the inline GetSize / GetAreaCovering;
  - one branch, which defines `FABLE_RETAIL_C2DBOXF` and includes fable_containers.h.
- `engine/CNavBlockedQuadTreeNode.h`, branch `FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE`: chunk 721 round 4's branch, unchanged. Only the parked SetUpRegions uses it.
- `engine/CNavQuadTree.h`, new branch `FABLE_RETAIL_CNAVQUADTREE` (0x88, MapPos +7). New headers: C2DLineF.h, CNavRegion.h, CNavigationPosition.h. None of the three existed in the repo.
- **Nothing else from chunk 721 round 4 comes in:** not CNavigatorManager.h, not C2DVector.h / C3DBoundingBox.h / CVectorMap.h / CThing.h / CTCVillage.h / CTCInventoryItem.h, and not the `FABLE_RETAIL_STLP_LIST_MEMBERS` opt-in. No chunk 736 row needs them.
- **`stlport/fable_containers.h`.** Chunk 736's additions go in under `FABLE_RETAIL_STLP_LIST`:
  - `_List_iterator_base::_M_decr`;
  - `_List_iterator::operator--`;
  - `list::back()` and `back() const`, in STLport 4.6's `--end()` form.

  The four landed `FABLE_RETAIL_STLP_LIST` users regrade unchanged. There is now one `back()` model.
- **Chunk 721 must not bring a second `back()`.** Round 4's `FABLE_RETAIL_STLP_LIST_MEMBERS` duplicates `size()`, `push_front()` and `back()`. When 721 lands:
  - cut that block down to what the list branch lacks, `const front()` and the `remove_if` declaration (`scripts/fc_721_rebase.diff`);
  - add `#define FABLE_RETAIL_STLP_LIST` to its row.
- **Both users compile on that model:**
  - 721's 0x00A1CEB0 grades DIFFER(992v992) PASS, its parked grade (`logs/parked_00a1ceb0_backcheck.log`), on a scratch mirror: the merged tree, plus 721's CNavigatorManager.h, plus that rebase;
  - 736's 0x00A7A0A0 grades DIFFER(1270v1222), as before.
- **hdrcheck2** prints `default=same` for every file in all three runs:
  - the overlay against the pre-merge tree (`logs/hdrcheck2_overlay_pre.txt`);
  - the overlay against the merged tree, all `same` (`logs/hdrcheck2_overlay_vs_tree.txt`);
  - the merged headers against HEAD's `rebuild/include` (`logs/hdrcheck2_final_vs_HEAD.txt`).

## Siblings (`sibling_grades.tsv`, `logs/final_recheck0*.log`)
- `includers.py` finds 3,005 landed sources and tests that reach a changed header. `ppcmp.py` against HEAD's headers (`logs/ppcheck.txt`):
  - 3,000 are token-identical;
  - 5 differ: the four `FABLE_RETAIL_STLP_LIST` users (0x004B05C0, 0x005385AE, 0x005392FC, 0x0053A0A1) and the new 0x00A78E20.
- Every includer of the nav headers (17 addresses) is in chunk 736's SIBLINGS.tsv, so all are rechecked.
- **Final `--recheck-landed` on the final tree:** 93 rows, 16 MATCH + 77 RELOCATION_MATCH, all PASS. The 93 are:
  - the 37 SIBLINGS rows;
  - 36 sampled pp-identical includers (every 80th);
  - the 4 new rows;
  - the 18 respelled rows (2 of them are also in the SIBLINGS list).
- No row that matched before regressed, and no MATCH became RELOCATION_MATCH. No header branch needed a fix.

## Names (`NAME_DECISIONS.tsv`, `BINDINGS.tsv`, `ghidra_out/labels_landing_2026-11-03g.tsv`)
Every decision cites two sources:
- the Ego_r / FableWin name sets, the Ego_r layouts or the STLport 4.6 source;
- the retail vtable slots, the call sites, or vland's relocation identity in the matched rows.

GameCODA and libpk have no PDB, so their names are the established descriptive family names: CCodaVoice / CSoftwareVoice / CAudioRingBuffer. `Flush` and `g_QuadTreeLevelSizes` are descriptive because no PDB name exists (the PDB name sets carry no data symbols).

**Applied.** Of chunk 736's 11 NAME_FIXES:
- 5 are applied in full: 0x00C59860, 0x00C5CEA0, 0x00C5CE40, 0x00A76F30, 0x0129DCAC.
- 5 are applied in part, with the binders that need a re-model queued: 0x00C5A1F0, 0x00C5A2A0, 0x00C59710, 0x00A770B0, 0x013D2D2C.
- 1 is applied as a label only, with its re-model queued: 0x00A80A20.

Every respelled row was graded before (`logs/nf_base.log`, `logs/nf_base_head_sources.log`) and after (`logs/nf_*.log`). All grade the same as before and PASS.

| Row | Respell |
|---|---|
| 0x00C59710 (definition) | `C_00c59710::M` (Leaf `Clear`) -> `?Stop@CCodaVoice@@UAEXXZ` (virtual slot 2 model); MATCH |
| 0x00C59860 (definition) | `?Close@CDataInputStream@@QAEXXZ` -> `?Reset@CCodaVoice@@UAEXXZ`. The two `__int64` stores reordered the dword stores, so the positions are modelled as word pairs (GOTCHAS); MATCH; test rewritten |
| 0x00C5CEA0 (definition) | `?Init_b@CV_MurderTwistScript@@QAEXXZ` -> `?Flush@CAudioRingBuffer@@QAEXXZ`; MATCH; test links the source |
| 0x00C5CE40 (definition) | `@CGameflowAssistanceScript_Init@4` -> `?Clear@CAudioRingBuffer@@QAEXXZ`; MATCH; test links the source |
| 0x00A76F30 (definition) | `CNavQuadTree_Methods` wrapper -> the private const member on `FABLE_RETAIL_CNAVQUADTREE` (full Leaf) |
| 0x00A14F70 | callee `GetNodeAt` -> Ego_r `QBEPBV...ABVC2DVector@@J`; own Leaf -> Ego_r `?GetNodeAt@CNavigatorManager@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@E@Z` |
| 0x00A77570 | callee `GetNodeAt`, `g_navNodeSizes` -> `g_QuadTreeLevelSizes`, `CNavLeaf` / `CNavigationPoint` -> `CANavQuadTreeNode` / `CNavigationPosition`; own Leaf -> Ego_r private const |
| 0x00A76060, 0x00A472F0 | `g_BlockedNavNode` / `g_BlockedNode` -> static `CNavQuadTree::BlockedNode` |
| 0x00A7CB50, 0x00A75CA0, 0x00A75CE0, 0x00A75D20, 0x00A75D60, 0x00A792E0 (lander-found) | the same `g_BlockedNavNode` respell (users of the global that never got a binding row) |
| 0x00A763B0, 0x00A75B30 | size table -> `g_QuadTreeLevelSizes`; Leaf added; 0x00A75B30 made const (Ego_r `QBEMXZ`), 0x00A763B0 C2DBoxF members -> TLX/TLY/BRX/BRY |
| 0x00A472F0 (lander-found) | free `GetSmallestNodeSize` / `SetBlockedNode(long, long, C2DVector)` -> Ego_r static `__fastcall` `CNavQuadTree::GetSmallestNodeSize` / `SetBlockedNode(float, float, long, long)`, as 0x00A15140 spells them |
| 0x00A81590 (definition, chunk 736 LABELS) | `vector<CUnprovenElement_00a81590*>` -> `vector<const list<C2DLineF>*>` (the proven element type) |

- **`reloc_bindings.tsv`** (`BINDINGS.tsv`): 10 rows retired, 1 added.
  - 0x013D2D2C: 2 rows; 0x0129DCAC: 3; 0x00A770B0: 2; 0x00A14F70: 1 stale `bool` spelling that no source uses; 0x00A76320 / 0x00A76340: 2.
  - Added: the Ego_r `GetNodeAt`.
  - The landing itself added 5 rows: CCodaVoice Stop / Reset, CAudioRingBuffer::Flush, IsAreaBlockedByLines and the C2DLineF-list `_M_insert_overflow`.
- **Applied as labels only (rows not re-modelled):**
  - 0x00C3F490 `CSoftwareVoice::~CSoftwareVoice`, 0x00C3F3D0 `CCodaVoice::~CCodaVoice`: a destructor is named by the vtable it stores first.
  - 0x00C5C290 `CSoftwareStreamVoice::Reset`: slot 17 of 0x012B5D70.
  - 0x00A80A20 `std::_List_base<CCountedPointer<CNavRegionConnection>, ...>::clear`.
  - 0x0122AAD0 / 0x012243D0, the BlockedNode atexit destructor and dynamic initializer (Ego_r + FableWin).
  - 0x00A76320 `CNavQuadTree::GetSmallestNodeSize`.
  - The unlanded 0x00A7A0A0 / 0x00A7AF60 / 0x00821A80 / 0x00A770B0.
- **Rejected:** none of the 11. Each was confirmed by two sources.
- **Queued PRIORITY** (9 lines tagged `land0311g` in `naming/NAME_FIXES_QUEUE.tsv`). Each needs a re-model, not a respell:
  - **0x00C3F490 and 0x00C3F3D0.** Rename the class model to CSoftwareVoice / CCodaVoice, including the vtable symbols and the 0x00C4CF40 base.
  - **0x00C5C290.** This is CSoftwareStreamVoice : CSoftwareVoice.
  - **The three rows on `candidates/nav_quad_tree_transition_points_compiled.h`.** These are 0x00A77480, 0x00A77500 and 0x00A7A650, and they use a non-const pointer GetNodeAt.
  - **0x0122AAD0 atexit destructor.** The row is a `Cons_val` stand-in.
  - **0x00A80A20.** The definition is the invented CRetailList_CountedPointer; binder 0x00A833B0 also needs the re-model.
  - **0x00A157C0 and 0x00500230.** They take the process by reference in Ego_r: lander-found.
  - **0x00A16050 and 0x012242B0.** These two still spell 0x00A76320 wrong: lander-found.
  - The 0x00C1EE30 NOTE.

## Final checks
- `bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` prints 0.
- Catalog rows 54,098 == `Address = '` entries 54,098.
- `apply_labels.py`: 16 catalog and 17 manifest renames. `sync_catalog_names.py`: 0 renames. `sync_manifest_from_catalog.py --labels` ran.
- `sync_manifest_from_catalog.py --labels --dry-run` reports 0 changes.
- `git diff | grep -c /tmp/claude` is 0, and no new file holds an `$S` path.
- hdrcheck2 is `default=same` on all three runs, including the final run against HEAD.
- GOTCHAS: 2 lines (binding rows record only the first binder; `__int64` zero stores reorder).

## Procedure
1. Ran hdrcheck2 on the overlay against the current tree, then diffed each file (0 removed lines) and copied it in. Compared fable_containers.h with chunk 721 round 4's version.
2. Graded the 4 rows on the merged tree, then `fixmove.py`, `keypayload.py` (0 texts changed), the asm/volatile grep (clean) and `vland --land`.
3. Found the includers (landing d `includers.py`) and pp-compared them against `git archive HEAD rebuild/include`.
4. Verified the names (PDB name sets, `nm.py` callers / vtable refs, the bindings, landed sources). Then the respells: baseline regrade, edits, regrade, `apply_bindings.py`, queue.
5. Checked the single `back()` model on both parked list users.
6. Labels, `apply_labels.py`, `sync_catalog_names.py`, `sync_manifest_from_catalog.py --labels`, the final sibling recheck (3 shards), the dashboard, the final checks and GOTCHAS.

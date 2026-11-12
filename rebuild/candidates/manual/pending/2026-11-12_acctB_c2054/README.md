# acctB hand-back, row 0x00A7AF60 CNavQuadTree::LoadFromFile: matched

| address | result | notes |
|---|---|---|
| 0x00A7AF60 `?LoadFromFile@CNavQuadTree@@QAE_NAAVCAFile@@JABVC2DBoxI@@AAVCASuspendableProcess@@@Z` | RELOCATION_MATCH 2503v2503 (770 of 770), test PASS (g++ and VC7.1) | `// Flags: /Gr` (measured need + landed same-TU siblings 0x00A85870, 0x00A762A0). 34 grades (`grades.log`) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 1 win, no RELOC_CONFLICT / RELOC_IDENTITY / NOT_GENUINE; two `WARN RELOC_NAME` lines for STLport helper
instances whose catalog labels are unmangled (0x00A85870 `__copy`, 0x00A83420 `__destroy_aux`).

## What closed it

1. **Region loop:** `{ unsigned long i = 0; if (no_regions != 0) { for (; i < no_regions; ++i) Regions.push_back(CNavRegion(MapIndex, i + 1)); } }`
   gives retail's single guard and keeps the CNavRegion copy constructor a call.
2. **Constructor text:** `CNavRegion(long, long) : MostRecentSearchID(0) { RegionID = ...; }` (store order at 0x00A7B07D).
3. **`FABLE_RETAIL_STLP_VECTOR_ASSIGN`** (existing repo opt-in): with it the TU's own `std::__copy<CNavigationLayer*>`
   equals retail 0x00A85870 and VC7.1 keeps it a call by itself. An earlier forced-noinline `__copy` was a wrong
   helper text (68 bytes against 97) and is gone; no inline decision in the row is forced.

## Review items

- **FITTED:** the region loop spelling (the bytes prove one guard, the copy constructor called and `resize` expanded;
  it is the only graded form that gives all three) and the `CNavRegion(long, long)` text.
- **PROVEN:** order and size of every Read, the version thresholds 5 / 6 / 7, member offsets, the shared BlockedNode
  selection, the five `GFProcessBreath` expansions (text of landed 0x00A762A0).
- **`NAME_FIXES.tsv`** (11 stale landed spellings, e.g. 0x00516180 is `??$ReadVar@H@CAFile@@QAEXAAHPAVCASuspendableProcess@@@Z`,
  0x00A80A20 is `_List_base<CCountedPointer<CNavRegionConnection>>::clear`); `LABELS.tsv` has 14 entries.
- **Headers:** new `engine/CCachedFile.h` (polymorphic, inline `~CCachedFile() { Close(); }`); opt-ins in
  `CANavQuadTreeNode.h` (virtual slots 16..22), `CNavQuadTreeNode.h`, `CNavNavigableLeafNode.h`,
  `CNavSwitchableLeafNode.h`. The STLport headers are the repo's, unmodified.

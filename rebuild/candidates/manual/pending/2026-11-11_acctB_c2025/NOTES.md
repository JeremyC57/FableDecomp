# Hand chunk 2025 (CTCInventoryMap .. CTCInventoryMap): 2 of 2 byte-exact

## What matched (2)
### CTCInventoryMap
- **0x00600B60 ConstructRegionDescriptionBuyableBuildings** (RELOCATION_MATCH, plain C++, 1 grade). With `compute` it walks every
  TCI_BUYABLE_HOUSE (213) building of the region's maps (GetAllLeafThings<CThingBuilding> 0x00617A90, the same instance as the
  landed twin Shops 0x006001E0) and counts owned shops / houses (GetOwnedByPlayer +0x11, TCI_SHOP decides shop v house) and, when
  not owned, IsForSale 0x006BF960 shops / houses; stores them in the CUIRegionInfo (ShopsOwned / HousesOwned / ShopsForSale /
  HousesForSale) or reads them back; then one coloured text line per non-zero count (houses owned, shops owned, houses for sale,
  shops for sale). Shape lessons: the landed Shops body carried over 1:1 (same opt-ins, same-TU GetDef / map::operator[] bodies under
  auto_inline(off)); the text bank is declared after the counting (retail reads PMainGameComponent there and keeps the reference in
  the dead `compute` home); the inner HasTC(TCI_SHOP) tests go through the iterator, `(*it)->HasTC(...)` (after the IsForSale call
  retail re-reads the building from the list node, `mov ecx,[ebp+8]`); counters declared in retail's zeroing order.
- **0x00605570 ConstructRegionDescription** (RELOCATION_MATCH with `// Flags: /d2inlT107`, Inline-budget device; 4 grades). Finds or
  appends the region's CUIRegionInfo (recompute when the region is the hero's current one), then builds the panel: description text
  (region def RegionDescription id, else display name), crime report (GetTC<CTCHeroStats> + GetCrimeReport), Quests, Shops +
  BuyableBuildings for a visited region (std::find over VisitedRegions), the map-keys line, MapKeys. Shape lessons:
  - Retail holds two complete copies of the region-info search loop (0x006055F7 / 0x00605660); one loop with a `recompute = true`
    if before it compiles to one loop: the source has the loop in both arms of `if (FromName != MapIsIn) {...} else { recompute = true;
    ... }` (the `!=` arm first gives retail's block order). (a fragment probe with `for` loops gave the same loop shape); the `while` form keeps the static frequency after the loops
    at the entry's, which the inline-budget device below relies on.
  - A same-TU body of STLport 4.6 `__find` (under auto_inline(off)) that never touches its tag: c2 then homes the
    random_access_iterator_tag temporary in the live `region` argument slot (0x00605ABE `lea eax,[esp+74h]`); declaration-only it
    took a frame byte and shifted every register after it (grade v3, 1891v1890).
  - The CText pointer is read into a named local before `container->AddChild(text_component)` (retail loads it before the call).
  - The def-pointer condition `!region.GetPRegionDef().IsNull() && region.GetPRegionDef()->RegionDescription` in the `if` itself
    gives retail's conditional-temporary flags (1 / 3) and the materialised result byte (the condition must outlive the temporaries).

### The inline-budget device on 0x00605570 (why plain C++ cannot match)
c2 inline trace (tools/inl.sh, hook at c2.dll 0x1078D1AA; regime 0, caller-size penalty 0 since g <= 2036 < 9000):
- GetDef<NUISystem::CUIMiscThingsDef>: c1 106 (every legitimate body / CDefPointer release form tried: 106; COUNTREF 114).
- CCountedPointer<CComponent>::Reset: c1 95 (default, RESET_CALLS_DTOR, RELEASEDATA all 95).
- score = c1*(1/f + 2)/3 for f >= 1 (fits every GetDef / Reset point measured), expanded iff score < 74.
- Retail expands GetDef at its one site and calls Reset at all eight sites. Every Reset site is one arm of an `if (pc)` (retail's
  per-arm CCharString construction and the per-arm `push edi; lea e?x` pairs prove complete if/else statements, tail-merged), so
  f(Reset) = F/2 where F is the frequency at the join where GetDef sits. GetDef needs F > 10.6; Reset needs F/2 <= 2.97: impossible.
  Measured: `for` search loops F 7.5 (GetDef 75 called, Reset 71 expanded); `while` loops F 1.0 (GetDef 106 called, Reset 108).
- No single /d2inlT works with the `for` loops (GetDef 75 > Reset 71). With the `while` loops the window is T 107..108; the smallest
  is 107. Under /d2inlT107 every other decision of the row equals retail (push_back, CUIRegionInfo(), _Construct, LowerBound in the
  expanded GetTC, find, the CCountedPointer dtors expanded; _M_insert_overflow and __find called), and with the __find body
  the row graded RELOCATION_MATCH (grade 3; grade 2 differed only by the tag slot).
- The same TU's Shops / BuyableBuildings call GetDef 0x0042B312 (f 1.0, score 106) and match at the default T, so T 107 is a per-row
  emulation, not retail's TU setting. Retail's c2 must have seen different callee costs in this function (the landed-sibling
  hand-back hand851 parked ConstructRegionExitMarkers 0x005FE840 on the same TU's inline budget). Review at landing: if the budget
  consumer / cost difference is ever found, drop the flag.

## Parked for Fable 5.1 (0)
None.

## Skipped
None. Both extents confirmed: 0x00605570 `sub esp,5Ch` after int3 padding .. `ret 0Ch` 0x00605CCF, int3 to 0x00605CE0
(UpdateVisitedRegions); 0x00600B60 after int3 .. `ret 14h` 0x00601308, int3 to 0x00601310. No SPLITS.tsv.

## Label corrections
See NAME_FIXES.tsv: 0x005FB7E0 (landed MapKeys row mangles CRegion* / public), 0x0050E540 (STLport `__find`, landed as Find_0050e540 /
Dinkumware _Find), 0x00618420 (vector<CUIRegionInfo>::_M_insert_overflow, landed under N00618420::vector<Element>).

## Names
All function names proven from two sources (LABELS.tsv). No descriptive function names. Local variable names are descriptive
(recompute, info, container, text_component, misc_def, hero_stats, crime_report, description / crimes / map_keys).
Callee names: every callee / global uses the spelling reloc_bindings.tsv binds at its address (rel.py: all `same`); the two wide
literals (L"" 0x0122D70C, L"\n" 0x01231728) are literal COMDATs (reloc_bindings has stand-in data names g_emptyWideString_0122d70c /
_s_newLine there from other rows).

## Header debt
- engine/CRegionDef.h is new (overlay): the repo had no CRegionDef header (fable_system.h's CRegionDef is an unrelated 0x5C stand-in).
- engine/CTCInventoryMap.h / CTCBuyableHouse.h: nested opt-ins only (hdr/README.md).

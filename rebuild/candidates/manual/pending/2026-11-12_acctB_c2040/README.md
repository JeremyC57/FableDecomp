# acctB hand-back, chunk 2040 (1 row matched)

Authored by an Opus 5.5 agent (from the outline chunk 2036 left), re-verified by the acctB local coordinator. Nothing is
landed.

| address | result | notes |
|---|---|---|
| 0x005F6E40 `?OnSerialise@CTCInventoryAbilities@@UAEXAAVCPersistContext@@_N@Z` | RELOCATION_MATCH 1746v1746, test PASS (g++ and VC7.1) | 8 grades, all in pass 1 (matched at grade 5; 6..8 are regrades after the helper-body rewrite and comment edits). True length 1746 (the 2113 reservation estimate includes separate accessor COMDATs) |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE, no name warnings). Purity and padded-struct
greps are clean.

## Review items for landing

1. **Inline-budget devices (two EXISTING shared-header opt-ins), with an `// Inline-budget:` line in the row:**
   `FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ` and `FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND`. Agent's c2 inline trace:
   this `optimize("s")` TU expands plain inlines only up to callee size 15 (largest expanded 15, smallest refused 22);
   `CCharString::operator==(const char*)` is 39 and `CVectorMap<long,EHeroAbility>::Find` is 37, both refused without a
   score, so no control-flow spelling or `/d2inlT` value reaches them. Retail expands both (0x005F7310..0x005F7337,
   0x005F6F19..0x005F6F2E).
2. **Same-TU body of the landed `GetMiscThingsDef` (0x005EE3B3)** under `#pragma auto_inline(off)` and an
   `_MSC_VER`-guarded `__declspec(noinline)`: with the body visible VC7.1 knows the hidden result is not captured and the
   misc def releases in place (`dec dword ptr [ecx+4] ; jne`, 0x005F72BF / 0x005F74F4). The body as written here itself
   grades RELOCATION_MATCH against 0x005EE3B3 (agent's two q.py compiles), so it is a pair-landing candidate with that
   landed row rather than a stand-in.
3. **Headers** (`hdr/README.md`; default text unchanged): `engine/CTCInventoryAbilities.h`
   `FABLE_RETAIL_CTCINVENTORYABILITIES_ONSERIALISE` (the override declaration; vtable 0x01256AD4 slot 1) and
   `engine/CWorld.h` `FABLE_RETAIL_CWORLD_ABOUTTOLOADHEROSTATE` (FableWin's inline
   `IsSerialisingAboutToLoadHeroState()`, reads +0x101; not in Ego_r, inline in release builds).
4. **Name fixes** (`NAME_FIXES.tsv`, 6): 0x005F7F4C `CVectorMap<long,EHeroAbility>::LowerBound` (pair outside `std` in
   the binding), 0x005F8F89 / 0x005F9BCD / 0x005F9DF8 `vector<EHeroAbility>::erase / push_back / resize` (bound under
   invented `CAbilityVector` / `CAbilityOrderVector`), 0x0043336A `vector<CCharString>::erase` (ten conflicting
   bindings), 0x009AD390 `CDefinitionManager::GetGlobalIndexFromClassIndex` (bound as GetDefGlobalIndex / GetDefIndex,
   which neither PDB has). Landed binders 0x005F694D, 0x005EEDE2, 0x005EE3B3 need respelling.
5. The `GetNumAssignableSpecialMoveButtons` chain is written out in the load path (retail has it in place there and
   calls 0x005EDCF0 in the save loop; with the member's body visible c2 refuses it at every site).

## How to land

`rows/005f6e40.cpp` + `rows/005f6e40.test.cpp` with the two `hdr/engine/` overlays merged, then `payload.json` through
your usual vland. `LESSONS.md`: a by-value CCharString call result pushed by reference with retail's `lea` is a named
local in its own block; a same-TU body for a hidden-result callee changes the release shape; the size-TU inline limit is
callee size 15; if / else assignments (not a ternary) give two stores and the parking spot for cold blocks; reading a
count into a named local before `resize` changes a scheduler tie.

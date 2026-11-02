# Shop identity and reconstruction evidence

Mandatory package instructions, the ordered document set, all row neighbours and both native spans were read before authorship. Native bytes were cut from the supplied executable through its PE section table, with the required SHA-1 checked by every grade. Manifest labels were not used as identity proof.

## Original identity and extent

`?OnSerialise@CTCShop@@UAEXAAVCPersistContext@@_N@Z`: Ego_r007832b0 and FableWin02622e10 name the same original virtual. Native CTCShop RTTI vtable0126a94c slot1 points to00775c60, and the first call qualifies protected CTCBase::OnSerialise with the original context/bool arguments. The manifest CPersistentShop helper label was wrong. The1162-byte span ends007760ea after `ret8`; it contains one function and no table or second entry.

## Full original types and headers

- `CTCShop` is the original `class` derived from `CTCBase`. Raw Ego type0x2cb84 and native constructor/method accesses establish complete size0x58, `ParentThing` inherited+4, `GoodsPlaces` vector+0x28, `Stock` map+0x34, `SpecialItem`+0x40, `DaysSinceSpecialItemPlaced`+0x44, `DayLastChecked`+0x48, `GoldDonated`+0x4c and `Alarm`+0x54. Native containers are12 bytes; Anniversary donor containers are16. `GoodsPlace` is the original nested **struct**, with `pos` and `rhset`, complete size36. The header preserves the original base and member types rather than declaring local replacement classes.
- `CShopItem` is an original class, seven words/28 bytes. PDB and the native seven-dword copy establish `Quantity`, `PriceMultiplier`, `MaxStock`, `RestockPeriod`, `RestockPeriodRandomness`, `SalesPerDay`, `SalesPerDayRandomness`, in that order; `RestockPeriod` is long and the others float.
- Original STLport4.6.2 `_map.h`, `_tree.h`, `_vector.h`, `_alloc.h`, `_uninitialized.h`, `_type_traits.h`, `_bitset.h`, `_range_errors.h` were used for the real templates, hierarchy, class keys and member access. `std::map` owns `_Rb_tree`, whose three-word layout is confirmed by native +0/+4/+8 accesses. No flat-vector or fake-map class is used. `_M_erase` is private; `_M_insert_overflow` is protected. Pointer/reference constness and empty-tag struct keys follow original source.
- `CGameDefinitionManager` is only a forward-declared class in both donor TPI streams; no original tail field names have been recovered. Native allocation at00418536 explicitly pushes0xe0 before constructor call00418547. Native constructor0044c6c2 calls the real `CDefinitionManager` constructor009b0470 and initializes four words at+d0,+d4,+d8,+dc; original RTTI confirms inheritance. Its real base and complete16-byte unnamed tail are retained. The target uses its original API and the inherited manager, never invented tail fields. This limitation is explicit, not an assertion that names for that tail were recovered.
- `CDefString` is the original class with `long TablePos`; `CDefStringTable` is the original full56-byte class, and `GDefStringTable` is the exact PDB global. Native GetString009d49b0 receives a long, not a made-up CDefString parameter. `CDefPointer` and all definition/resource base classes use their existing original shared headers.
- Existing shared defaults remain intact. New behavior is opt-in through `FABLE_RETAIL_SHOP` and related original retail branches. No new packing directives were introduced.

## Callees, globals and exact emitted spellings

The table below is extracted from the selected COFF object, not inferred from source spelling. Every direct-call target is checked in native instruction order. Literal bytes are compared independently. Existing bindings were searched before declarations; corrected stale spellings are listed separately in `NAME_FIXES.tsv`. No selected symbol is bound to another native address in the supplied table.

| Retail address | Emitted symbol | PDB name-set evidence |
|---|---|---|
| 0x00410be0 | `??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z` | fablewin_pdb_names.tsv:01895220; egor_pdb_names.tsv:0040d020 |
| 0x00433530 | `?_M_insert_overflow@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@IAEXPAVCCharString@@ABV3@ABU__false_type@2@I_N@Z` | Original library source / native body, as detailed above |
| 0x004408a0 | `?_M_insert_overflow@?$vector@MV?$allocator@M@std@@@std@@IAEXPAMABMABU__true_type@2@I_N@Z` | Original library source / native body, as detailed above |
| 0x0049b8d0 | `??$TransferVector@VCCharString@@@CPersistContext@@QAEXPBDAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z` | fablewin_pdb_names.tsv:018b81e0; egor_pdb_names.tsv:0043dadb |
| 0x00569fe0 | `??$Transfer@M@CPersistContext@@QAEXPBDAAV?$vector@MV?$allocator@M@std@@@std@@@Z` | Original library source / native body, as detailed above |
| 0x00686960 | `?OnSerialise@CTCBase@@MAEXAAVCPersistContext@@_N@Z` | fablewin_pdb_names.tsv:02190cb0; egor_pdb_names.tsv:006b1d30 |
| 0x006869d0 | `?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ` | fablewin_pdb_names.tsv:02190d80; egor_pdb_names.tsv:006b1dc0 |
| 0x00772250 | `?FindDefaultStockItem@CTCShop@@QBEPBVCShopItem@@J@Z` | fablewin_pdb_names.tsv:0261e5b0; egor_pdb_names.tsv:0077f200 |
| 0x00776aa0 | `?_M_erase@?$_Rb_tree@JU?$pair@$$CBJVCShopItem@@@std@@U?$_Select1st@U?$pair@$$CBJVCShopItem@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCShopItem@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJVCShopItem@@@std@@@2@@Z` | Original library source / native body, as detailed above |
| 0x00778280 | `??A?$map@JVCShopItem@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJVCShopItem@@@std@@@3@@std@@QAEAAVCShopItem@@ABJ@Z` | fablewin_pdb_names.tsv:02624070; egor_pdb_names.tsv:00783f70 |
| 0x0099eae0 | `??1CCharString@@QAE@XZ` | fablewin_pdb_names.tsv:02f60f80; egor_pdb_names.tsv:00afa3f0 |
| 0x0099ec30 | `??0CCharString@@QAE@ABV0@@Z` | fablewin_pdb_names.tsv:02f60fe0; egor_pdb_names.tsv:00afa5f0 |
| 0x009accc0 | `?GetDefNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z` | fablewin_pdb_names.tsv:03062780; egor_pdb_names.tsv:00b04b10 |
| 0x009ad410 | `?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z` | fablewin_pdb_names.tsv:03062820; egor_pdb_names.tsv:00b05360 |
| 0x009d49b0 | `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z` | fablewin_pdb_names.tsv:02fccf40; egor_pdb_names.tsv:00b28a90 |
| 0x00bfea14 | `_free` | Original library source / native body, as detailed above |
| 0x013ca828 | `?GDefStringTable@@3VCDefStringTable@@A` | Original library source / native body, as detailed above |

STLport instantiations absent from the newer donor name set are established by the original4.6.2 library spelling plus native algorithm/element layout and the original game element PDB type. The donor uses a newer tree/container implementation, so its different library internals were not substituted. CRT symbols are verified against the native import/thunk chain. Relocations for numerical and string literals are recorded with exact bytes in `RELOCATIONS.json`.

## Naming corrections that matter

- 005d99d0 is static fastcall `CTCInventoryItem::GetInventoryType(long)`, returning `ETCInterfaceType`, **not** GetInventoryCategory. Ego names GetInventoryType at0055a9a0; its native body calls CacheDef005d94b0 and reads the inventory-type field+0x50. The distinct category accessor reads+0x3c.
- 005bead5 is **const** `NInventory::CTCInventoryBase::GetBoughtPrice(long)`, with the original namespace and class. Ego donor00621385 and the native Categories+0x20 lookup distinguish it from the stale global-class nonconst label.
- 006d8300 takes `EHeroExperienceStatCategory`, not long, for its first parameter. The original enum gives HERO_STAT_SKILL=1; caller pushes1 and the original PDB emitted signature includes the enum.
- GetCurrentBasicPrice007727e0 and GetCurrentBuyPrice007742c0 are const. GetFreeSaleItemPlaces00774b40 is nonconst and returns `vector<CTCShop::GoodsPlace>` by value. AddSaleItem00772bb0 takes the original nested **struct** by const reference.
- CTCBase::GetDefinitionManager006869d0 is protected const, returning const CGameDefinitionManager&. Its vtable OnSerialise00686960 is protected virtual. CPersistContext00569fe0 is `Transfer<float>(...,vector<float>&)`, while0049b8d0 is `TransferVector<CCharString>`; these are distinct original names.
- The mutable LowerBound0040f020 and const LowerBound004365b0 keep separate symbols. The established nonconst GetMainPlayer00449970 and const GetPControlledCreature00487dc0 are used exactly.

These corrections are recorded only when their symbols occur in the selected row; the complete helper search is retained locally in the immutable experiment/evidence records. Names for ordinary reconstruction locals are not claimed as recovered unless listed below.

## Recovered local names

Both donor module streams prove `context`, `in_game`, `current_stock_item_defs_keys` (vector<CCharString>), `current_stock_item_defs_quantity` and `current_stock_item_defs_max_stock` (vector<float>), `it`, `def_index`, and `item` (CShopItem). FableWin additionally proves `end_it` is declared before `it`, and loop counter `n` is long. All these names and types are used. The native map iterator is the original STLport form, not the donor newer-Dinkumware iterator type.

## Independent retry result

Attempt18 achieved all1162 target bytes after restoring concrete original STLport4.6 map/tree insertion bodies before the target. Attempt19 regraded the final shared headers and source and remains exact. The helper declarations and algorithms are the original `_map.h`/`_tree.h`/`_tree.c` entities, using the already-proven `long` and `CShopItem` types. No substitute class or invented allocation boundary was introduced. The selected target is the only function claimed byte-identical; supporting helper bodies are compiler-visible original implementations.

The new `FABLE_RETAIL_SHOP_TREE_BODY` branch adds only original tree accessors, node construction, iterator decrement and original insertion declarations. Existing branches remain intact. Source retains the vendor notice. Exact source-shape history is in PROBES.md.

## Behavioral verification limits

The self-contained test now expands41 genuine headers, including the complete real CThing hierarchy instead of the earlier empty reference completion. It executes the original restored map insertion and red-black rebalancing bodies. Twelve cases test text/binary save modes, in_game gating, empty/populated data, exact seven-field transfer order, load independently of in_game, preserving old stock for empty keys, clearing/rebuilding for nonempty keys, unknown/missing-default entries, duplicate keys, item field preservation and string/resource lifetime counts. Serialization and definition lookup remain controlled test boundaries.

GNU64 C++03/no RTTI, native VC7.1 RTM /Od /GR- /W3 and GNU64 -fno-elide-constructors all pass. The native test enforces original class sizes and offsets; host tests account for LP64 long/pointers. Test-only constructor/access instrumentation is absent from production. Every one of52 COFF operands is independently checked at its exact native position, including37 direct calls; strings are checked as bytes. Emitted symbol spelling is captured with objdump -t.

## Selected hashes

- Source SHA-256: `039a11590a43b08c4cc0e2e3a2783a1e6d7091acab6c8e4e760d34c85e6f8423`.
- Test SHA-256: `7458482e80d228e3b32a4243ee8344bb770847d564039d7690f0bf40d45a38a0`.

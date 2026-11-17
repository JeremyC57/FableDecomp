# NF50 round 7 notes: STLport red-black tree / list internals (nf44 entries 39, 42, 53, 55, 85; nf41 e175, e117, e217, e185)

Run in two parts: the first run ported 75 entry-39 rows, then the container restarted. This resume finished the remaining 24 rows and
added two extra rows. Every number below is counted from STATUS.tsv, APPLIED.tsv, work/grades.log and recheck_final.log.

## Outcome
- **Worklist rows (99):** 96 ported, 3 not-a-binder, 0 blocked, 0 deferred.
- **Extra rows (2, not in the worklist):**
  - 0x00769980, the `_M_create_node` callee of 0x0076A110, ported in step so the pair class key agrees.
  - 0x00566FE0, the e185 callee, self-renamed to the symbol its caller already binds.
- **Rows changed:** 98, all in rows/. NF50 holds a lock on exactly these 98 rows. The 3 not-a-binder rows were claimed, left unchanged,
  and their locks released.
- **Entries (APPLIED.tsv):** all 9 queue lines are `done`.
  - e39: 79 rows (_Rebalance).
  - e42: 1.
  - e53: 3.
  - e55: 5.
  - e85: 2.
  - e175: 4.
  - e117: 4 (2 ported, 2 not-a-binder).
  - e217: 2.
  - e185: 1 not-a-binder; the callee rename is the extra row 0x00566FE0.
- **Final recheck (recheck_final.log):** one vland run over all 98 changed rows, through the hdr/ overlay. Result: 97 RELOCATION_MATCH,
  1 MATCH (0x00566FE0), 98 tests PASS. No test file changed; the row tests are self-contained copies.
- **Unrelated-includer regrade (work/recheck_unrelated.log):** 0x00AD9770 and 0x00AD9F10 are repo rows that include both overlaid
  headers. Both are RELOCATION_MATCH and PASS.
- **EH funclets:** no row under rebuild/src/compiled includes the file of any changed row (work/eh_includers.txt is empty).
- **Grades:** work/grades.log has 245 lines across both runs.
  - The most on one row is 5 (0x0059FAFE, including 1 wasted; see slips).
  - Nine of the lines are probe compiles of unchanged caller rows, used to read their bindings for RENAMES section 2
    (work/caller_bind.tsv).

## Not-a-binder (the confirming compile already binds the claim exactly)
- 0x004804DE and 0x004830A1: both bind the e117 `_Rb_tree<CWideString, pair<const CWideString,bool>>::clear` at 0x00485AEF.
- 0x00563920: binds the e185 `find` at 0x00566FE0.
  - The real e185 defect was the callee's own placeholder leaf (`CUnprovenKey_00566fe0`).
  - Fixed as the extra row 0x00566FE0. Its type is Ego_r.pdb CTCHeroOnlineScoreboard::DownloadedClanScoreboard.

## How each group was ported
- **e39 `_Rebalance`, 79 rows.**
  - **Step 1 (69 rows: 67 of the first run, plus 0x0076A110 and 0x00739670 now):** the call binds `std::_Rb_global<bool>::_Rebalance` of
    stlport/fable_map.h. Each stand-in node type derives from `std::_Rb_tree_node_base`, so the row's own leaf is unchanged.
  - **Step 2 (10 rows):** the explicit instantiation of the header `_M_insert` (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY).
    - Rows: 0x004ABC70, 0x0051AA00, 0x00529150, 0x005292F0, 0x005293D0, 0x0052E0E0, 0x00777790, 0x00990830, 0x004759AD and 0x00A61270.
    - Instance types come from Ego_r.pdb members and the landed callers.
  - **0x004759AD and 0x00A61270** had a local `namespace std` tree that cannot coexist with fable_map.h, so step 2 was the only option.
    - Their pre-existing local CCharString / CWideString models stay (not new). fable_string.h has no CWideString::operator< and is a
      root `#pragma once` header that is not overlaid.
    - 0x004759AD needs `std::less<CCharString>::operator()` force-inlined through an explicit specialisation, because the size TU
      otherwise calls it out of line.
- **e55 `_M_erase` of map<unsigned long, CCharString> (Ego_r.pdb CUIDef::ActionMap):**
  - 0x00630D90 is the body itself; renamed to the STLport `_M_erase`.
  - 0x00630FE0 is `_Rb_tree::clear`; renamed. Its caller 0x00632B90 already binds the new name.
  - 0x006301A0 (~CUIDef): ActionMap is now the header map.
  - 0x006311D0 and 0x00631420: the stand-in derives from the header map.
- **e117/e217 (map<CWideString,bool> clear):**
  - 0x005958F5 (CFrontEndManager::Init): local vector/map replaced by the headers (TREE_CLEAR_OUTOFLINE + VECTOR_DTOR_OUTOFLINE).
  - 0x00485E45: renamed to `~_Rb_tree` of that map.
    - STLport 4.6 map declares no destructor, so the out-of-line COMDAT that owners call is the tree's.
    - A compile of the implicit map destructor under this TU's optimize("s") emits exactly that call.
    - The ~CFrontEndManager caller already binds the new name.
- **e175 (`_List_base<CCountedPointer<CNavRegionConnection>>::clear`):**
  - 0x00A81470 and 0x00A82230: the stand-in derives from the header list (LIST_CLEAR_OUTOFLINE).
  - 0x00A833B0: member type replaced.
  - 0x00A84110: renamed `_List_base<CNavRegion>::clear`. Ego_r.pdb CNavQuadTree::Regions = list<CNavRegion>; LIST_CLEAR_OUTOFCLASS keeps
    the inner Connections clear out of line.
- **e53 (`_M_find` of map<CComponent*, CList*>):** 0x0059DB82, 0x0059E6BD and 0x0059FAFE.
  - The local pair / CListMap / node models were replaced by the header map, with the new overlay opt-in
    FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE (visible body, noinline). The declaration-only MFIND_DECL was 293 v 281 bytes.
  - The local class access changed public -> private, so each self leaf is now exactly the Ego_r.pdb symbol (AAE, map parameter). The
    landed callers already bind that symbol through engine/CTCInventoryTrade.h (probe of 0x005B0AAC).
- **e85 (const `find` of map<unsigned long,long>):** 0x00AD8C30 and 0x00AD8CA0 moved onto engine/CVertexGroupBlock.h
  (CVERTEXGROUPBLOCK_MEMBERS + TREE_CONSTFIND_NOINLINE, the 0x00AD9F10 precedent).
  - 0x00AD8C30 needs the new overlay opt-in FABLE_RETAIL_CVERTEXGROUPBLOCK_ISSUBSETOF, because both PDBs name IsSubsetOf.
- **e42 (hinted `insert_unique` of map<unsigned long, CUIState>):** 0x005331A0, local std block replaced by the headers.
  - The real STLport hinted insert_unique body (TREE_INSERT_UNIQUE_HINT_BODY) gives the same size with a store-order DIFFER.
  - Adding the visible `_M_insert` body (MAP_ORIGINAL_INSERT_BODY) gives RELOCATION_MATCH.
  - So the row's old fake same-TU read-only insert_unique specialisation is gone (see REJECTED.tsv for why it was not kept).

## Stale landed spellings left for the coordinator (RENAMES section 2 and follow-ups)
- **Section 2:** 6 callers still bind the old stand-in spelling of a renamed row. Each was read from a probe compile of the repo copy,
  work/caller_bind.tsv.
  - 0x00A7A7F0, 0x00A84640 and 0x00A84BE0 (-> 0x00A84110).
  - 0x0047E07F (-> 0x00485E45).
  - 0x004758E3 (-> 0x004759AD).
  - 0x00A61360 (-> 0x00A61270).
  - Each is a local-model caller. The respell is a one-line model change in each, except 0x004758E3 and 0x00A61360. Those two have
    their own local `namespace std` tree, so they need the same step-2 move.
- **0x0046ECE7** (`_M_create_node` of map<CCharString,long>): 0x004759AD now calls it as
  `?_M_create_node@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$_Select1st@..@V?$allocator@..@std@@IAEPAU?$_Rb_tree_node@..@Z`.
  - The landed row's own leaf is a local three-argument-plus-allocator spelling.
  - It needs a self rename onto the header's `_M_create_node`. Its other caller is _M_copy 0x0046EC57, landed as CRetailTree_0046ece7::Copy.
- **0x0076A110 / 0x00769980 (CSoundSetTree):**
  - The landed callers 0x0076A1E0 and 0x0076A700 already bind the real STLport `_M_insert` spelling.
  - The row still compiles as the stand-in class. Its mixed shape (left-arm `_M_create_node` inlined, right arm calling 0x00769980)
    and engine/CObjectAugmentationSoundSet.h (an out-of-line copy constructor, where retail inlines the copy) keep it off the header
    body for now.
- **0x00739670 (CCombatAbilityTree):**
  - The types are proven: Ego_r.pdb CTCCombat::CombatAbilities = multimap<ECombatAbilityDataType, pair<CCountedPointer<
    CCombatAbilityData>, CCombatAbility> >.
  - What is missing is a port of its caller insert_equal 0x00739B00, which binds the same stand-in.

## Self name still a stand-in (78 rows; step 1 / binding only, own leaf unchanged)
Instance types are not proven by these rows' own callers in this pass: the landed callers bind the same stand-in, or there is no caller
or data reference at all.
- To finish them, port the stand-in's caller (insert_unique / insert_equal / owner) onto the header first, then move the row.
- Listed with the row's current self symbol and its landed callers (work/e39_callers.tsv):

| row | self symbol (stand-in) | landed callers |
|---|---|---|
| 0x00A81470 | `??1list_T_00a81470@std@@QAE@XZ` | none landed |
| 0x00A82230 | `??1list_T_00a82230@std@@QAE@XZ` | none landed |
| 0x00A833B0 | `??_GCCountedPointerListOwner_00a833b0@@QAEPAXI@Z` | none landed |
| 0x00A84110 | `?clear@?$_List_base@VCNavRegion@@V?$allocator@VCNavRegion@@@std@@@std@@QAEXXZ` | none landed |
| 0x00485E45 | `??1?$_Rb_tree@VCWideString@@U?$pair@$$CBVCWideString@@_N@std@@U?$_Select1st@U?$pair@$$CBVC..` | none landed |
| 0x0042B98B | `?Insert@CRetailTree_ulongkey_buy0042b5a8@@QAE?AUiterator@1@PAUnode@1@0ABKJ@Z` | 0x0042BA13 0x0042C082 |
| 0x0042BA99 | `?Insert@CRetailTree_longkey_buy0042b5ca@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0042BB21 0x0042C1C1 |
| 0x0042BBA7 | `?Insert@CRetailTree_longkey_buy0042b5ec@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0042BC2F 0x0042C300 |
| 0x004313E9 | `?Insert@CRetailTree_ulongkey_buy00431471@@QAE?AUiterator@1@PAUnode@1@0ABKJ@Z` | 0x00431363 0x005537F0 |
| 0x0045CA8F | `?Insert@CRetailTree_ulongkey_buy0045c8a1@@QAE?AUiterator@1@PAUnode@1@0ABKJ@Z` | 0x0045CA09 0x005D87B6 |
| 0x0045E0FA | `?Insert@CRetailTree_longkey_buy0045de2f@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0045E074 |
| 0x0045E4B4 | `?Insert@CRetailTree_longkey_buy0045e2b1@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0045E42E |
| 0x0045EE0E | `?Insert@CRetailTree_longkey_buy0045e797@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0045ED88 |
| 0x0045F4FD | `?Insert@CRetailTree_longkey_buy0045f13d@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0045F477 |
| 0x0046180C | `?Insert@CRetailTree_longkey_buy0046149e@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x00461786 |
| 0x00461A40 | `?Insert@CRetailTree_longkey_buy004615c3@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004619BA |
| 0x00463EB4 | `?Insert@CRetailTree_longkey_buy004622ec@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x00463E2E |
| 0x004647B9 | `?Insert@CRetailTree_longkey_buy004630e7@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x00464733 |
| 0x00465DBC | `?Insert@CRetailTree_longkey_buy00461cf3@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x00465D36 |
| 0x004669BF | `?Insert@CRetailTree_longkey_buy00462411@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x00466939 |
| 0x00469639 | `?Insert@CRetailTree_longkey_buy0045c2a2@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004695B3 |
| 0x0046FBC9 | `?Insert@CRetailTree_longkey_buy0045c4d2@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0046FB43 |
| 0x00472A05 | `?Insert@CRetailTree_longkey_buy00466255@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0047297F |
| 0x004E51F6 | `?Insert@CRetailTree_longkey_buy004e4fef@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004E5170 |
| 0x004E5425 | `?Insert@CRetailTree_longkey_buy004e4eca@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004E539F 0x005B2A37 |
| 0x004E57AC | `?Insert@CRetailTree_longkey_buy004e55cf@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004E5726 0x005BB5EF |
| 0x004E5BA7 | `?Insert@CRetailTree_longkey_buy004e597b@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004E5B21 |
| 0x004E6466 | `?Insert@CRetailTree_longkey_buy004e6210@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004E63E0 0x00801EC0 |
| 0x004E699E | `?Insert@CRetailTree_longkey_buy004e6623@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004E6918 |
| 0x004E6C39 | `?Insert@CRetailTree_longkey_buy004e6748@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004E6BB3 |
| 0x004E7C3B | `?Insert@CRetailTree_longkey_buy004e4429@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004E7BB5 |
| 0x004E9299 | `?Insert@CRetailTree_longkey_buy004e4bee@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004E9213 |
| 0x004E94C8 | `?Insert@CRetailTree_longkey_buy004e4d19@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004E9442 |
| 0x004EC79D | `?Insert@CRetailTree_longkey_buy004ec31c@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004EC717 |
| 0x004ECA59 | `?Insert@CRetailTree_longkey_buy004ec441@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004EC9D3 |
| 0x004ECEE6 | `?Insert@CRetailTree_longkey_buy004e9ab1@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x004ECE60 |
| 0x00511430 | `?_M_insert@CRbTree@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSValue@@1@Z` | 0x00512DA0 |
| 0x00567AF0 | `?Insert_CRetailTree_00567af0@CRetailTree_00567af0@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x00567BB0 0x00568230 |
| 0x00579FD8 | `?Insert@CRetailTree_longkey_buy00579f3e@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0057A124 |
| 0x0057A060 | `?Insert@CRetailTree_longkey_buy00579f5f@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0057A1AA |
| 0x005808A0 | `?Insert@CRetailTree_ulongkey_buy00580428@@QAE?AUiterator@1@PAUnode@1@0ABKJ@Z` | 0x00580D21 |
| 0x0059B1BE | `?Insert@CRetailTree_longkey_buy0059afab@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x0059B246 0x0059B35C |
| 0x005B2337 | `?Insert@CRetailTree_ulongkey_buy005b200c@@QAE?AUiterator@1@PAUnode@1@0ABKJ@Z` | 0x005B2872 0x005B2EFE |
| 0x005B23BF | `?Insert@CRetailTree_ulongkey_buy005b2029@@QAE?AUiterator@1@PAUnode@1@0ABKJ@Z` | 0x005B2447 0x005B28F8 |
| 0x005B24CD | `?Insert@CRetailTree_ulongkey_buy005b204b@@QAE?AUiterator@1@PAUnode@1@0ABKJ@Z` | 0x005B2555 0x005B2B76 |
| 0x005B25DB | `?Insert@CRetailTree_ulongkey_buy005b206c@@QAE?AUiterator@1@PAUnode@1@0ABKJ@Z` | 0x005B2663 0x005B2CB5 |
| 0x005BB220 | `?Insert@CRetailTree_longkey_buy005baf06@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x005BB2A8 0x005BB85A |
| 0x005BBCBE | `?Insert@CRetailTree_longkey_buy005bbba5@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x005BBD46 0x005BBE32 |
| 0x005F926B | `?Insert@CRetailTree_longkey_buy005f8c41@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x005F92F3 0x005F9853 |
| 0x00632A30 | `?Insert_CRetailTree_00632a30@CRetailTree_00632a30@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x00632990 |
| 0x0067B320 | `?Insert_CRetailTree_0067b320@CRetailTree_0067b320@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x0067BA30 |
| 0x0067B3E0 | `?Insert_CRetailTree_0067b3e0@CRetailTree_0067b3e0@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x0067BAF0 |
| 0x0067B4A0 | `?_M_insert@CRbTree@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSValue@@1@Z` | 0x0067BBB0 |
| 0x0069D520 | `?_M_insert@CCameraModeMap@@QAE?AUiterator@1@PAUSRbNodeBase@@0ABUSCameraModeValue@@0@Z` | 0x0069D900 |
| 0x006AE900 | `?_M_insert@CMovementAccelerationMap@@QAE?AUiterator@1@PAUSRbNodeBase@@0ABU?$pair@$$CBW4EGa..` | 0x006AE9F0 |
| 0x006DAFA0 | `?Insert_CRetailTree_006dafa0@CRetailTree_006dafa0@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x006DB450 |
| 0x006DB390 | `?Insert_CRetailTree_006db390@CRetailTree_006db390@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x006DB610 |
| 0x006DCB20 | `?Insert@CUIBinaryTree@@QAE?AUiterator@1@PAUnode@1@0ABUSUITreePair@@J@Z` | 0x006DCDD0 |
| 0x006F1280 | `?Insert@CThingCompareMap@@QAE?AUiterator@1@PAUSRbNodeBase@@0ABUSThingCompareValue@@0@Z` | 0x006F1EF0 |
| 0x006F4D00 | `?Insert@CCombatantMap@@QAE?AUiterator@1@PAUSRbNodeBase@@0ABUSCombatantValue@@0@Z` | 0x006F4F30 |
| 0x0070AEA0 | `?_M_insert@CEngineTextureDecalMap@@QAE?AUiterator@1@PAUSRbNodeBase@@0ABUSDecalMapValue@@0@..` | 0x0070AFE0 |
| 0x00739670 | `?_M_insert@CCombatAbilityTree@@QAE?AU_Rb_tree_iterator@@PAU_Rb_tree_node_base@@0ABUSCombat..` | 0x00739B00 |
| 0x0076A110 | `?_M_insert@CSoundSetTree@@QAE?AUiterator@1@PAU_Rb_tree_node_base@std@@0ABU?$pair@$$CBW4EOb..` | 0x0076A1E0 0x0076A700 |
| 0x00783F00 | `?Insert@CThingMap64@@QAE?AUiterator@1@PAUSRbNodeBase@@0ABUSThingMapValue@@0@Z` | 0x00784040 |
| 0x00801730 | `?Insert_CRetailTree_00801730@CRetailTree_00801730@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x008017F0 0x00801CB0 |
| 0x00817D70 | `?Insert_CRetailTree_00817d70@CRetailTree_00817d70@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x00818000 |
| 0x0085DED0 | `?_M_insert@CRbTree@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSValue@@1@Z` | 0x0085E140 |
| 0x00869CA0 | `?_M_insert@CRbTree@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSValue@@1@Z` | 0x00868E50 0x00869020 |
| 0x008B29A0 | `?Insert_CRetailTree_008b29a0@CRetailTree_008b29a0@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x008B2DD0 |
| 0x008B2A60 | `?Insert_CRetailTree_008b2a60@CRetailTree_008b2a60@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x008B2E90 |
| 0x008B2B20 | `?Insert_CRetailTree_008b2b20@CRetailTree_008b2b20@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSVa..` | 0x008B2F50 |
| 0x009C2350 | `?_M_insert@CRbTree@@QAEPAUSIterator@@PAU2@PAUSNode@@1ABUSValue@@1@Z` | 0x009C2800 |
| 0x009EF750 | `?InsertNode@CConsoleInputMap@@QAE?AUIterator@@PAUNode@@0ABUValue@@J@Z` | 0x009EFB80 |
| 0x00CD300B | `?Insert@CRetailTree_longkey_buy00cd2c5b@@QAE?AUiterator@1@PAUnode@1@0ABJJ@Z` | 0x00CD3093 0x00CD34C5 |
| 0x00630FE0 | `?clear@?$_Rb_tree@KU?$pair@$$CBKVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBKVCCharStrin..` | none landed |
| 0x006311D0 | `??1CRetailTree_erase00630d90@@QAE@XZ` | none landed |
| 0x00631420 | `??1CRetailTree_erase00630d90@@QAE@XZ` | none landed |
| 0x00769980 | `?_M_create_node@CSoundSetTree@@QAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EObjectAugmentationType..` | none landed |

## Rule slips (work/slips.txt)
- Two Bash calls in this resume exited non-zero: a `grep -l` with no match (2) and a trailing `grep -c` with zero matches (1).
- One e53port.py assertion failed, and the grade chained after it with `;` still ran on a missing file. That wasted 1 grade on
  0x0059FAFE; later commands chain with `&&`.
- From the first run: one `diff` exited 1.

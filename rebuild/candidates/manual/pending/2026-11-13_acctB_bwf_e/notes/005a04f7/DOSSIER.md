# DOSSIER 0x005A04F7 CTCInventoryTrade::ConstructNormalShopList (9469 bytes) -- identity scout
(No earlier scout material existed: $W/prior/ and /opt/fable_lane/w22/agent4026/{src,test,probe,hdr} are empty, so this is a fresh scout.)
Scout aids written: $W/scout/c.txt (compact retail listing, every call/string annotated), $W/scout/callees.tsv (callee -> manifest name -> landed file), $W/scout/ann.py (the annotator).

## 1. Retail shape (xdis 005a04f7 0x250d; $W/dis_all.txt)
- `__thiscall`, two stack args, `ret 8` at 0x005A29F1. Returns `list` (esi = [ebp+1Ch]) in eax.
- Prologue `push ebp ; lea ebp,[esp-70h] ; sub esp,0D8h ; push ebx ; push esi ; push edi` -- the EBP-OFFSET frame. Epilogue `pop edi ; mov eax,esi ; pop esi ; pop ebx ; add ebp,70h ; leave ; ret 8`.
  => pragma: `#pragma optimize("s", on)` ONLY (as landed sibling 0x005A5B31 ConstructActualWantedInventoryPC, whose header says: `lea ebp,[esp-78h]` = s-only; adding optimize("y",off) gives `mov ebp,esp` -- which is what 0x005A29F4 / 0x005A30A1 / 0x0059DF49 show, so do NOT copy their "y off").
- No fs:[0] / no EH states / no /GS cookie: TU built without /EHsc (no `// Flags:` line in any CTCInventoryTrade row).
- Size idioms: `or esi,-1` held for every `CCharString(text,-1)`; vtable read before the stack CCharString for every FindChild (`mov edi,[x] ; push ecx ; mov ecx,esp ; ...`); `repe cmpsb` inline strcmp of literal v "" (0x0122D70E) = FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ.
- Param slots are reused as locals: [ebp+78h] = arg1 `buyItems`, [ebp+7Ch] = arg2 `sellItems` (later both hold iterators / temps).

## 2. Name / signature (two sources + callers)
- `?ConstructNormalShopList@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@ABV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@0@Z`
  - FableWin.pdb: ghidra_out/_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv (0x023085D5, debug size 23318; the PC form 0x0233AA14 is only 5206 -> ours is the console form).
  - Ego_r.pdb: ghidra_out/egor_signatures.tsv 0x00612B12.
  - FableWin source order: AddBuyItemToList 0x02307317 -> ConstructNormalShopList 0x023085D5 -> ConstructSpecialShopList 0x0230F8E3 -> OpenInventory 0x02310FF7 = retail 0x0059FEE3 -> 0x005A04F7 -> 0x005A29F4 -> 0x005A94BD (same TU, same order).
  - Only caller: 0x005A996F in CTCInventoryTrade::OpenInventory 0x005A94BD (console), pushes &[ebp-40h] (2nd) then &[ebp-30h] (1st).
  - Manifest label CTCInventoryTrade::ConstructNormalShopList is CORRECT.
- Public. Header already declares it: rebuild/include/engine/CTCInventoryTrade.h:145 under FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS:
  `NUISystem::CComponent* ConstructNormalShopList(const std::vector<std::pair<long,long> >& buyItems, const std::vector<std::pair<long,long> >& sellItems);`
  Body confirms the names: the SELL section iterates arg2 ([ebp+7Ch], AddSellItemToList), the BUY section arg1 ([ebp+78h], AddBuyItemToList).

## 3. Callees (landed file; landed name; notes)
| retail | used as | landed row | note |
|---|---|---|---|
| 0x005B1C18 x2 | ItemsToBuy.clear() / ItemsToSell.clear() (map<CComponent*,pair<long,long>>::clear) | 00/5b/CTCCarryable_OnKill_005b1c18.cpp ("RbTree_Clear_005b1c18") | landed name is descriptive/wrong file stem; C++ spelling `ItemsToBuy.clear()` as 0x005A29F4 |
| 0x005B1C41 x3 | SellListsByCategory / BuyListsByCategory / WantedListsByCategory .clear() | 00/5b/CTCCarryable_OnKill_005b1c41.cpp | same |
| 0x005B21A1 x3 | NumSellableThings / NumBuyableThings / NumWantedThings .clear() | 00/5b/CTCCarryable_OnKill_005b21a1.cpp | same |
| 0x0099EBF0 / 0x0099EAE0 | CCharString(const char*, long=-1) / ~CCharString | landed | |
| 0x0041E5F2 x3 | NUISystem::CManager::GetInstance | landed | called 3x: sellActions temp, `manager`, tail screen title |
| 0x0041DB1D x30 | CManager::CreateComponent(CCharString, bool) | landed | |
| 0x00429C15 x12 | CCountedPointer<CComponent>::CCountedPointer(T*) (out of line) | landed | FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR |
| 0x00429C6F x17 | CCountedPointer<CComponent>::Reset(T*) | landed | |
| 0x004291DE x13 | CCountedPointer<CComponent>::ReleaseData (= dtor) | landed | FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA |
| 0x00429460 x1 | CCountedPointer<CText>::SetStaticCast (COMDAT-folded; manifest "CCountedPointer<CAction>::ShareData") | landed | same call in landed 0x005A29F4 (0x005A301F) |
| 0x0042925C x1 | ~CCountedPointer<CText> (folded; landed file DestvalstdallocatorCCountedPointer..._0042925c) | landed | same call in 0x005A29F4 (0x005A305F) |
| 0x0044C6B0 x2 / 0x009AD390 / 0x0042B312 | CGameDefinitionManager::Get / GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF",1) / GetDef<CUIMiscThingsDef> | landed (9ad390 file is named COptimisedPrimitive_ActualToUniqueIndex) | copy 0x005A29F4's same-TU `GetDef` template under auto_inline(off) |
| 0x004DF81D x3 / 0x004DD1C4 x3 | std::map<CComponent*,long> ctor / dtor | landed (std_map_T_004df81d_map / CActiveFile_OnReadFinished_004dd1c4 = "StdList_Dtor") | out of line; the PC sibling 0x005A5B31 gets them with FABLE_RETAIL_STLP_MAP |
| 0x005B1AC5 x4 | map<CComponent*,long>::find (_M_find) | 00/5b/stdRbtree_Mfind_005b1ac5.cpp | |
| 0x0042D1B7 x4 | map<CComponent*,long>::operator[] | landed | |
| 0x005B1A43 x4 | map<CComponent*,CList*>::find | 00/5b/stdRbtree_Mfind_005b1a43.cpp | |
| 0x005B3076 x3 | map<CComponent*,CList*>::operator[] | landed | |
| 0x005B1A8D x3 | map<CComponent*,pair<CWideString,long>>::find | landed | |
| 0x005B3100 x6 | map<CComponent*,pair<CWideString,long>>::operator[] | landed | |
| 0x004292C0 x6 | _Rb_tree_base iterator ++ (_M_increment) | landed | map iterator ++ (out of line) |
| 0x0059B79A x8 | CTCInventoryTrade::GetMenuBranchFromDefIndex(long, CComponent* menu, EItem_Menu_Type&) | landed | header line 70 (always declared) |
| 0x0059ED42 x1 | AddSellItemToList(long,long,CList*,CViewport*,bool) | landed | header line 38 |
| 0x0059FEE3 x2 | AddBuyItemToList(long,long,CList*,CViewport*,bool) | NOT landed (row just before ours) | header SHOPLISTS macro |
| 0x0059CB90 x1 | AddWantedItemToList(long,CList*,CViewport*) | landed | header FABLE_RETAIL_CTCINVENTORYTRADE_WANTEDLIST |
| 0x0059DF49 x3 | ConstructSubtitles(CComponent*, map<CComponent*,pair<CWideString,long>>&) PRIVATE `AAE` | landed (but that row models its own local classes) | NOT declared in any shared header: add it in an hdr overlay (new macro), leaf ?ConstructSubtitles@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@PAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@...@Z |
| 0x005BC4C4 x1 | CTCInventoryBase::GetBuildingThing (landed as global_DrawGetEnvironment / "GetMember_dword0xb8") | landed | FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING, as 0x005A29F4/0x005A5B31 |
| 0x00410DE0 x2, 0x0040F020, 0x004365B0 | bitset<274>::test + CVectorMap::LowerBound (GetTC, non-const) / LowerBound (PeekTC, const) | landed | inline CThing::GetTC / PeekTC: FABLE_RETAIL_CTHING_GETTC_EXPANDED + FABLE_RETAIL_STLP_BITSET_TEST_DECL |
| 0x00774100 | CTCShop::GetWantedItemsList() const -> vector<long> by value (hidden ret = &[ebp-38h]) | landed | FABLE_RETAIL_CTCSHOP_WANTED |
| 0x00772610 x5 | CTCShop::GetName() const -> unsigned long | landed | FABLE_RETAIL_CTCSHOP_NAME |
| 0x00449960 / 0x00487DD0 | CPlayerManager::GetMainPlayer / CPlayer::PeekPControlledCreature | landed | |
| 0x009C95E0 x13 | NGameText::CDataBank::GetTextBySymbol(const CCharString&) const -> CWideString | landed | |
| 0x009CC190 / 0x009C7D00 / 0x009C9070 x2 / 0x009C7A40 | CBankFile::EntryExists / CDataBank::GetTextInfo / GetPTextEntry(ulong,bool) / CTextEntry::GetText | landed | identical sequence in 0x005A29F4 |
| 0x005CC772 x14 | CTCInventoryStats::GetXString() (static) | landed | |
| 0x00405F00 x25 | GFIntToWideString (__fastcall, edx=value, ecx=ret) | landed | declare as 0x005A29F4 |
| 0x0099BE70 x12 | operator+(const CWideString&, const CWideString&) (__fastcall) | landed | |
| 0x0099B720 / 0x0099AED0 / 0x0099B510 / 0x0099B7D0 / 0x0099B8D0 | CWideString copy ctor / default ctor / dtor / operator= / operator+= | landed | |
| 0x004115A0 x9 | CBasicString<char>::operator==(const char*) | landed | the non-null arm of the inline CCharString::operator==(const char*) |
| 0x00BFEA14 | free (vector<long> dtor, inline STLport deallocate) | | |
Virtual slots used: CComponent +0Ch FindChild(CCharString) slot 3; +24h FindFirstChildOfType(UI_TYPE_TEXT=6) slot 9; +D4h GetChildren() slot 53 (vector<CCountedPointer<CComponent>>&, 8-byte elements); +ECh AddChild(CCountedPointer&) slot 59; +F0h AddChildDynamically slot 60; +14Ch SetName(const CCharString&) slot 83; +150h GetName() slot 84 (CCharString by value); CText +238h GetValue() slot 142, +240h SetValue(const CWideString&,bool,bool) slot 144; CViewport +238h AddFakeChild(CComponent*); CList +244h AddNonScrollingChild(CCountedPointer&), +270h InitialiseOffsets(unsigned long) slot 156, +278h SetDefaultChild(long) slot 158 (all as spelled in landed 0x005A29F4 / 0x005A5B31).
Globals: 0x013B86A0 CMainGameComponent::PMainGameComponent (`CMainGameComponent::Get()`; +14h text bank, +1Ch player manager). Anonymous-namespace CCharStrings of the trade TU (labels_landing_2026-11-12dj.tsv, Ego_r + FableWin dynamic initializers): BUY_SUBTITLE 0x013B8B64, SELL_SUBTITLE 0x013B8B68, WANTED_SUBTITLE 0x013B8B6C, WEAPONS_SUBTITLE 0x013B8B70, CLOTHES_SUBTITLE 0x013B8B74, ITEMS_SUBTITLE 0x013B8B78, CATEGORY_CHILD_SELL_LIST_STRING 0x013B8B94 -- spell `namespace { extern CCharString X; }` as 0x005A29F4 / 0x005A5B31.

## 4. Members of `this` (all in rebuild/include/engine/CTCInventoryTrade.h base block under FABLE_RETAIL_CTCINVENTORYTRADE; names = Ego_r struct_layouts_egor.tsv)
+140h std::list<long> NotInStock; +144h ItemsToBuy, +150h ItemsToSell (map<CComponent*,pair<long,long>>); +180h SellListsByCategory, +18Ch BuyListsByCategory, +198h WantedListsByCategory (map<CComponent*,CList*>); +1B0h NumBuyableThings, +1BCh NumSellableThings, +1C8h NumWantedThings (map<CComponent*,pair<CWideString,long>>); +1D4h/1D8h/1DCh TotNumClothes/Weapons/ItemsToSell; +1E0h/1E4h/1E8h TotNumClothes/Weapons/ItemsToBuy; +1ECh/1F0h/1F4h TotNumClothes/Weapons/ItemsWanted. GetBuildingThing() (CTCInventoryBase +B8h, macro _BUILDINGTHING).
List node layout: [node] = next, [node+8] = value (`*n`). Map node: key +10h, value +14h (pair<CWideString,long>: .first +14h, .second +18h).
EItem_Menu_Type: 0 clothes, 1 weapons, 2 items (switch arms add to TotNumClothes.. / Weapons.. / Items..; enum in header).
No local struct models needed: every class has a shared header (CTCInventoryTrade, CTCShop, CTCHeroStats, CThing, CMainGameComponent, CPlayerManager, CPlayer, NGameText_CDataBank, NUISystem_CManager/CList/CViewport/CText, CGameDefinitionManager, NUISystem_CUIMiscThingsDef, CCountedPointer). Only `class CThingPlayerCreature : public CThing { };` (as both siblings).

## 5. Visibility / inline decisions
- Out of line in retail, keep them called: all three map::clear, map ctor/dtor, find, operator[], iterator ++, CCountedPointer ctor/Reset/ReleaseData, CText SetStaticCast/dtor, every CWideString/CCharString member, GetXString, GetTextBySymbol.
- Expanded in retail: CThing::GetTC / PeekTC (bitset test + LowerBound + end check), CTCHeroStats::GetMoney ([x+3Ch]), CCharString::operator==(const char*) (null test + `repe cmpsb` vs "" else 0x004115A0) -> FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ plus the same-TU `CBasicString<T>::Compare` / `operator==(const T*)` bodies under auto_inline(off) exactly as 0x005A5B31 lines 79-99; CDefPointer dtor (dec [ecx+4] / call [eax+4]) = FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE; vector<long> dtor (free); CCountedPointer default ctor (two zero stores); GetPTextEntry temp's CCountedPointer dtor (dec [eax+4] inline, x2) as 0x005A29F4; vector size `(end-begin)>>3` loops.
- Same-TU neighbours defined earlier: AddBuyItemToList 0x0059FEE3 (not landed, `bool` member -- keep a declaration only; retail calls it), AddSellItemToList 0x0059ED42, AddWantedItemToList 0x0059CB90, ConstructSubtitles 0x0059DF49, GetMenuBranchFromDefIndex 0x0059B79A: all called, none expanded.

## 6. Siblings to copy (same TU / family)
- 0x005A5B31 ConstructActualWantedInventoryPC (rebuild/src/compiled/00/5a/CTCInventoryTrade_ConstructActualWantedInventoryPC_005a5b31.cpp) -- BEST TEMPLATE: same pragma (`optimize("s",on)` only, same `lea ebp,[esp-78h]` frame), same branch-count map, sub-list loop, NumWantedThings loop with cached `end`, the three category-text blocks, the per-entry GetChildren loop with `entryName == "..."` chain, the three goods-list description loops, ConstructSubtitlesPC, gold. Macros: CCOUNTEDPOINTER_RELEASEDATA, CCHARSTRING, CCHARSTRING_FORCEINLINE_TEXT_EQ, CWIDESTRING, CTCBASE, CIVCOUNTEDPOINTER, CIVCOUNTEDPOINTER_DTOR_DIRECT, CENGINEPERSISTENTPRIMITIVEBASE, CTCINVENTORYBASE, CTCINVENTORYBASE_BUILDINGTHING, CTCINVENTORYTRADE, CTCINVENTORYTRADE_PCINVENTORY, CTHING, CTHING_GETTC_EXPANDED, STLP_BITSET_TEST_DECL, STLP_LIST, STLP_MAP, STLP_VECTOR_PUSHBACK_OUTOFLINE, CTCSHOP_SHOPTYPE, CTCSHOP_WANTED, CTCHEROSTATS, CTCHEROSTATS_BRIBE, CTCHEROSTATS_GETMONEY, CMAINGAMECOMPONENT(+_PLAYERMANAGER, _TEXTBANK), CPLAYERMANAGER, PLAYER.
- 0x005A29F4 ConstructSpecialShopList (..._ConstructSpecialShopList_005a29f4.cpp) -- the console twin: the top (clears, CManager, viewport, misc def via same-TU GetDef template), the `if (!items.empty() || !NotInStock.empty())` NotInStock loop, and the shop-name / text-bank tail (bool haveName = EntryExists && GetTextInfo && !GetPTextEntry(..,true).IsNull(); SetStaticCast; AddChildDynamically; SetDefaultChild(0L)). Its extra macros: DEF_CLASS_BASE, CDEFPOINTER_RELEASE_POINTEE, CTCINVENTORYTRADE_SHOPLISTS, STLP_LIST_SIZE_DISTANCE, CTCSHOP_NAME, CDEFSTRINGTABLE, CRESOURCELIST, CDEFINITIONMANAGER, CGAMEDEFINITIONMANAGER, CCOUNTEDPOINTER_CALLED_CTOR. (Its pragmas "y off"+"s on" do NOT apply here.)
- 0x005A30A1 InitialiseBuyableItems, 0x005A45F6 ConstructSellActionsList, 0x0059DF49 ConstructSubtitles: same TU, "y off"+"s on" frames.
- Union of macros to start from = 5a5b31 set + {DEF_CLASS_BASE, CDEFPOINTER_RELEASE_POINTEE, CTCINVENTORYTRADE_SHOPLISTS, CTCINVENTORYTRADE_WANTEDLIST, CTCSHOP_NAME, CDEFSTRINGTABLE, CRESOURCELIST, CDEFINITIONMANAGER, CGAMEDEFINITIONMANAGER, CCOUNTEDPOINTER_CALLED_CTOR} (drop _PCINVENTORY unless needed) + hdr overlay declaring the private ConstructSubtitles.

## 7. Facts that are easy to get wrong
- Section ORDER: SELL (arg2) -> BUY (arg1 + NotInStock) -> WANTED (shop->GetWantedItemsList()) -> gold -> screen title -> SetDefaultChild(0L).
- Two dead locals the frame needs: `CCountedPointer<CComponent>` at [ebp-68h] = CManager::GetInstance()->CreateComponent("UI_LIST_SELL_ACTIONS", false) (never used again), and a default-constructed CCountedPointer<CComponent> at [ebp-50h] (never used); plus the unused miscDef (as 0x005A29F4). Construction order (from the reverse dtor order at 0x005A2971..0x005A29E3): [-68h] sellActions, [-50h] unused, [-14h] miscDef, [+4Ch] description, [-60h] sellTitle, [+30h] xString, [+28h] title, [-2Ch] sellSubtitle, [-58h] buyTitle, [-24h] buySubtitle, [-38h] wantedItems vector, [-48h] wantedTitle, [-1Ch] wantedSubtitle.
- The text bank reference is taken FIRST (0x005A0543, [ebp+2Ch]) -- before the UI_LIST_SELL_ACTIONS CreateComponent; the tail re-reads CMainGameComponent::Get() each time (no local).
- `title` [ebp+28h] is CONSTRUCTED from `GetTextBySymbol("TEXT_GUI_MENU_SELL") + xString` (operator+ writes straight into it), then ASSIGNED (operator=) for BUY (`+ xString`) and WANTED (`+ CTCInventoryStats::GetXString()` temporary, not xString).
- Total sums, retail add order: sell 1DCh+1D4h+1D8h (Items+Clothes+Weapons), buy 1E4h+1E8h+1E0h (Weapons+Items+Clothes), wanted 1F0h+1F4h+1ECh (Weapons+Items+Clothes); 0x005A5B31 wrote `Items + Clothes + Weapons` for its matching 3-term sum.
- Zeroing order 1DCh,1D8h,1D4h = `TotNumClothesToSell = TotNumWeaponsToSell = TotNumItemsToSell = 0;` (same for buy / wanted).
- Only the SELL sub list gets `SetName(CATEGORY_CHILD_SELL_LIST_STRING)`; sell keeps `branch` in a local ([ebp+68h]) and keys SellListsByCategory[branch]; buy / wanted key `XListsByCategory[b->first]` (operator[] gets &node+10h).
- Sell/buy NumXThings update: `if (m.find(branch) == m.end()) m[branch].second = it->second; else m[branch].second += it->second;` (two operator[] calls, find compared with header). Wanted: `= 1` / `++`.
- `GetMenuBranchFromDefIndex(.., entry, type)` -- the menu argument is the SECTION ENTRY ([ebp+6Ch] = list->FindChild("UI_TRADING_MENU_<X>_ENTRY")), not the goods list. The goods list is looked up twice per section: once for AddNonScrollingChild(title), again (later) as the children/`menu` for the per-entry loop, the three *_LIST description loops and ConstructSubtitles(menu, NumXThings).
- ConstructSubtitles gets `&NumSellableThings` (+1BCh) for sell, `&NumBuyableThings` (+1B0h) for buy, `&NumWantedThings` (+1C8h) for wanted.

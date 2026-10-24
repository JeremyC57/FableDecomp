# Hand chunk 325 (CActiveFile .. CTCInventoryTrade): 8 of 40 byte-exact

All 8 grade RELOCATION_MATCH on RTM cl 13.10.3077; their tests pass under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (8)

- **STLport vector assignment** (manifest "CActiveFile::CopyAssign_12", wrong)
  - 00460eb5 std::vector<CMusicSetEntry>::operator= and 00462b4e std::vector<CBrainBehaviour>::operator=: 12-byte POD
    elements. The shape is a size-TU STLport `operator=` that reallocates when the source is bigger than the capacity,
    otherwise copies over the live elements and constructs or destroys the tail. Two out-of-line helpers per
    instantiation (0x460FC6/0x461009 and 0x462C5F/0x462CA2). Leaf `??4<Type>Vector`.
- **CTCInventory**
  - 005907ab CTCInventory::ConstructItemCategoryListPC (manifest "CTCInventoryAbilities::ConstructPCExpressionCategoryList";
    FableWin.pdb gives the real name). The PC category row: merges items per def into a map, then builds the sub-list,
    the actions list and the frame registrations. Two shapes made it match. `merged.insert(std::make_pair(key, value))`
    loads the value into a register before the key store, which a direct pair constructor does not. The usable /
    can't-use template is a ternary on a `usable` flag, and VC threads the flag so the default arm sits out of line.
- **CTCInventoryClothing**
  - 005b5cf8 GetItemDescription and 005b62fe GetItemDescriptionNoArmourRating: stat blocks built from an ArgPasser copy.
    They need ONE reused `long change` local for the per-stat differences. It reproduces retail's frame-slot homing:
    the first web sits in ebx and later webs sit in the dead parameter slot.
  - 005b70b8 ConstructClothingList (console): back / select / title headers on the actions list and the sub-list, then
    one branch entry per item with five actions. It matched on the first grade. The key shape is
    `CList* subList = 0; if (!items.empty()) { ... } return subList;`. That puts the sub-list in edi, where it doubles
    as the `false` argument, and keeps the sub-list prompts' destructors inside the block. The two CText pointers are
    default-constructed and assigned from the component through the ShareData template operator=.
- **CTCInventoryTrade** (no `NInventory` namespace: the PDB has `@CTCInventoryTrade@@`)
  - 0059ed42 AddSellItemToList and 0059f51e AddSellItemToListPC (manifest "AddSellItemToList"): the sell-menu entry with
    seven actions and the ItemsToSell record. Named locals fix the call and argument order:
    `desc = pointer.m_pObject; bank = GetGameSubsystem()->TextBank;`.

## Parked for Fable 5.1 (32)

- 0058a0c7 CTCInventory::ConstructItemEntry DIFFER(1626v1629): `ItemsMenuEntries[defIndex] = entry`. Retail loads
  defIndex for the next call before the store through operator[]'s reference. The by-value Set copies the key to
  [ebp-0xC] | tried: direct + named locals, Set(long,..) [best], direct key + Set for the icons, Set(const long&,..) |
  next: lane gap: operator[] store scheduling
- 0058a763 CTCInventory::OpenPCSkillsMenu DIFFER(-v1965): not graded | tried: decode only | next: author it
- 0058e208 CTCInventory::ConstructItemEntryPC DIFFER(-v1943): not graded. It is the PC twin of 0058a0c7 (PC frame
  lookup, expression-assignment icon, three extra actions) | tried: decode only | next: start from differ/0058a0c7.cpp;
  expect the same operator[] residue
- 0058f016 CTCInventory::ConstructItemCategoryList DIFFER(1588v1587): merge-loop registers. Retail has
  `lea edi,[edi+eax*8]` with start in edi and count in eax; ours has start in eax and count in ecx. The PC twin 005907ab
  matches | tried: base, `*(begin()+n)`, item declared outside the loop | next: copy the 005907ab loop text verbatim
- 0058f9bc CTCInventory::OpenPCPhotojournal DIFFER(-v2012): not graded | tried: decode only | next: author it
- 00590235 CTCInventory::GetItemsListUIComponent DIFFER(1382v1393): this/zero/index sit in edi/esi/ebx in retail and in
  esi/ebx/edi in ours, with permuted slots. Retail also hoists the tattoo pointer above the hair test and jump-threads
  both exits into its release | tried: base, `case 4:` split from `default:` (kept: it gives retail's dead `dec eax`),
  release through a `long&` count | next: hair/tattoo test as one `bool` local, or a nested if
- 005f077f CTCInventoryAbilities::ContructItemLists DIFFER(1239v1248): statement shapes match. Retail has the item
  offset in edi and the inventory in ebx, ours the reverse; retail loads the def before the quick-slot pointer
  (`cmp [ecx+8],eax`); retail shares one temp slot for the GetInterface key and the stored map values (0xAC vs 0xB0
  frame) | tried: base, CEngineGraphic user ctor (fixed the hidden-return pointer), named value locals, inline
  Set(key, value) [kept] | next: swap the `==` operands, and give the GetInterface key no named local
- 005f1526 CTCInventoryAbilities::ConstructPCItemLists DIFFER(-v1514): not graded. It is the PC sibling of 005f077f
  (PC frame registrations, expression-assignment icon sprites, two extra actions, a null-tested SetUserValue) |
  tried: decode only | next: start from differ/005f077f.cpp (CEngineGraphic ctor, Set helper)
- 005f3f7b CTCInventoryAbilities::ConstructExpressionCategoryList DIFFER(1438v1438): frame slots only. Retail homes the
  description temporary at [ebp-0xC] and the template component at [ebp-0x3C] | tried: base [best], the extra as a
  temporary + a named block-local text | next: lane gap: slot assignment
- 005f45b5 CTCInventoryAbilities::ConstructPCExpressionCategoryList DIFFER(1232v1233): retail puts -1 in ebx and
  items/owner/frame in esi/edi with items reloaded per block; ours keeps items in ebx | tried: base, named desc/bank
  locals, named title object, caption count in an inline helper | next: lane gap: register allocation
- 005f4c8f CTCInventoryAbilities::GetExpressionsListUIComponent DIFFER(1296v1296): retail keeps the loop index in memory
  and the def object in edi (`dec [edi+4] ; jne` release). Ours keeps the index in edi and re-reads the pointer slot
  for the release | tried: base, the dtor as one `&&` test, the dtor through a local copy | next: GetDef through a
  same-TU stand-in, or the def pointer as a function-scope local
- 005f6e40 CTCInventoryAbilities::OnSerialise DIFFER(-v1746): not graded | tried: decode only | next: author it
- 005b6a66 CTCInventoryClothing::ConstructClothingListPC DIFFER(1511v1507): frame layout. The retail frame is 0x7C with
  the sub-list sharing the early CCharString slot; ours is 0x78 | tried: base, the non-empty path as an else block,
  the console twin's shape (null sub-list + block, 1509v1507) | next: look for the extra retail local (a dead
  `CComponent*` copy?)
- 005b9641 CTCInventoryClothing::vfunc_11 DIFFER(-v1455): not graded | tried: decode only | next: author it
- 00577216 CTCInventoryExperience::vfunc_86 DIFFER(-v1268): not graded | tried: decode only | next: author it
- 005fe090 CTCInventoryMap::ConstructQuestsScreen DIFFER(-v1937): not graded | tried: decode only | next: author it
- 00600b60 CTCInventoryMap::ConstructRegionDescriptionBuyableBuildings DIFFER(-v1963): not graded | tried: decode only |
  next: author it
- 00605570 CTCInventoryMap::ConstructRegionDescription DIFFER(-v1890): not graded | tried: decode only | next: author it
- 00605ce0 CTCInventoryMap::UpdateVisitedRegions DIFFER(-v1226): not graded (ESP-frame speed TU, heavy inlining) |
  tried: decode only | next: author it
- 0061ad60 CTCInventoryQuests::DrawActiveTestQuests DIFFER(-v1631): not graded | tried: decode only | next: author it
- 00621f70 CTCInventoryQuests::TakeQuest DIFFER(-v1098): not graded (ESP frame) | tried: decode only | next: author it
- 006224c0 CTCInventoryQuests::OpenInventoryPC DIFFER(-v1338): not graded | tried: decode only | next: author it
- 0059bb1b CTCInventoryTrade::AttachToFrame DIFFER(-v1451): not graded | tried: decode only | next: author it
- 0059fee3 CTCInventoryTrade::AddBuyItemToList DIFFER(1553v1556): retail spills `this` and keeps the hero tab in ebx
  (sharing `xor ebx,ebx`); ours keeps `this` in ebx | tried: base, const found-form, inline hero arg, const long& ctor |
  next: lane gap: register allocation
- 005a29f4 CTCInventoryTrade::ConstructSpecialShopList DIFFER(-v1709): not graded | tried: decode only | next: author it
- 005a3734 CTCInventoryTrade::RefreshBuyPossibilityNumbers DIFFER(-v1811): not graded. Decoded: it clears the buy
  totals and the branch map, walks ItemsToBuy (affordable + allowed or special) into map<branch, pair<CWideString,long>>
  +0x1B0, then relabels the branches "name xN" and builds the subtitles | tried: decode only | next: author it
- 005a3e47 CTCInventoryTrade::ConstructBuyActionsList DIFFER(1967v1967): only the store through operator[]'s result is
  scheduled before the parent/vtable loads | tried: const-ref min, if-min, decl order (fixed the min and the early null
  store), a `target` local | next: lane gap: operator[] store scheduling
- 005a45f6 CTCInventoryTrade::ConstructSellActionsList DIFFER(1767v1767): same operator[] store scheduling | tried: base
  ternary, inline fn, if-chain+local [best], key type | next: lane gap: operator[] store scheduling
- 005a6769 CTCInventoryTrade::ConstructNormalShopListPC DIFFER(1336v1336): retail adds the wanted total as
  Weapons + Items + Clothes; ours always loads Items first | tried: W+I+C, I+W+C, C+W+I, `+= 1` counters | next: lane
  gap: operand order
- 005a6ca1 CTCInventoryTrade::AddBuyItemToListPC DIFFER(1681v1662): the same regalloc as 0059fee3 | tried: base, tab
  lookup in an inline helper | next: lane gap: register allocation
- 005a731f CTCInventoryTrade::ConstructSellActionsListPC DIFFER(1735v1735): the same operator[] store scheduling as
  005a45f6 | tried: as 005a45f6 | next: lane gap
- 005a79e6 CTCInventoryTrade::ConstructBuyActionsListPC DIFFER(1935v1935): the same operator[] store scheduling as
  005a3e47 | tried: the 005a3e47 best source with PC templates | next: lane gap

## Skipped

None skipped as two-function rows: no row holds a second body. The 17 rows marked "not graded" above were left out
by the time box, not skipped for content.

## Label corrections

- 00460eb5 / 00462b4e: "CActiveFile::CopyAssign_12" -> std::vector<CMusicSetEntry>::operator= /
  std::vector<CBrainBehaviour>::operator=.
- 005907ab: "CTCInventoryAbilities::ConstructPCExpressionCategoryList" -> CTCInventory::ConstructItemCategoryListPC
  (FableWin.pdb).
- 005f3f7b: "ConstructPCExpressionCategoryList" -> CTCInventoryAbilities::ConstructExpressionCategoryList (the
  console row).
- 0059f51e: "AddSellItemToList" -> CTCInventoryTrade::AddSellItemToListPC; 005a731f -> ConstructSellActionsListPC;
  005a79e6 -> ConstructBuyActionsListPC.
- Callees:
  - 0x007723D0 "_atexit" -> CTCShop::HasDefaultStockItem
  - 0x005BC4C4 "CWorld::DrawGetEnvironment" -> CTCInventoryBase::GetTradingThing
  - 0x005BF2ED / 0x005BF30D "CTCInventoryBase::GetItemDescription" -> the tab's IsItemAllowed / GetMaxBuyable
  - 0x005D9830 "IsAugmentation" -> CTCInventoryItem::IsExpression (CExpressionSubDef lookup 0x005DA240)
  - 0x005D9970 "IsAugmentation" -> CTCInventoryItem::IsBook (CCarriedReadableDef lookup 0x005DA3C0)
  - 0x005D96F0 "GetInventoryCategory" -> CTCInventoryItem::GetUIInventoryCategory (def +0x6C, 4 without a def; it
    follows GetDescription 0x5D95C0 / GetDetails 0x5D9630 / GetEngineGraphic 0x5D96A0 as in the FableWin order)
  - 0x0048524C "CCountedPointer<CAction>::ShareData" -> ShareData of CCountedPointer<NUISystem::CMenuEntry> (dtor
    0x004850F9)
  - 0x00593139 "MedianCut::GetNewTreeNode" -> vector<pair<long,long> >::erase (already noted by chunk 913)

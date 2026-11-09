# Hand chunk 891 (CTCInventoryTrade .. CTCInventoryTrade): 1 of 3 byte-exact

Extents (xdis): each row is one function ending in a plain `ret` with the next function's `push ebp` directly after it
(0x005A5B30 -> 0x005A5B31, 0x005A6768 -> 0x005A6769, 0x005ACED9 -> 0x005ACEDA). True lengths = catalog lengths (3668 / 3128 / 3532).

## What matched (1)
- CTCInventoryTrade::ConstructActualWantedInventoryPC 0x005A5B31 (RELOCATION_MATCH, 3128): builds the PC "wanted" trade screen
  (shop's wanted list -> one sub list per menu branch, wanted entries, per-branch / per-category counts on the labels, descriptions,
  subtitles, gold, refresh for ordinary shops). Shape lessons: (1) frame `push ebp ; lea ebp,[esp-78h]` = optimize("s",on) ONLY
  (the sibling's optimize("y",off) gives `mov ebp,esp`); (2) `CCharString == "literal"` must expand in place
  (FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ); (3) retail loads PStringData once per name test: give the TU the bodies of
  CBasicString<char>::Compare (0x00411570) and operator==(const char*) (0x004115A0) under `#pragma auto_inline(off)` (mod/ref: they
  only read), else every test reloads it after the out-of-line ==; (4) retail's discarded first FindChild of the menu entry is real.

## Parked for Fable 5.1 (2)
- 0x005A4CDD CTCInventoryTrade::ConstructActualSellInventoryPC DIFFER(3664v3668): 0x005A4E49 retail `mov esi,[ebp+4] ; cmp esi,[ebp+8] ; je 0x005A4F19` (head test duplicated, empty exit threaded past the ItemsToBuy insert loop) vs ours `mov esi,[ebp+4] ; jmp <bottom test>` | tried: for / while / do-while / for(;;)+break, fresh vs shared vs outer iterator, index placement, cached end, guard `if` around the filter loop only, inline and FABLE_FORCEINLINE filter helper, clears swapped, CLEAR_OUTOFLINE opt-in (18 grades + 30 compile-only probes); `if (!buyableItems.empty()) { filter; insert }` (probe/s7.cpp) reproduces the flow with one extra `mov esi,eax` (c2prio: begin() temporary prio 14 colours eax before `it` prio 10) | next: find one more real reference to `it` in the K=5 body blocks (+5 priority) under the wrap, or lane gap: loop-head duplication / jump threading
- 0x005AC10E CTCInventoryTrade::ConstructActualBuyInventoryPC DIFFER(3538v3532): 0x005AC189 retail `mov esi,[ebp+28h] ; jmp 0x005AC21D`, ours `mov eax,[ebp+28h] ; cmp eax,[ebp+2Ch] ; mov esi,eax ; je`; 0x005AC373 retail `mov eax,[ebp+2Ch] ; cmp [ebp+28h],eax`, ours `mov eax,[ebp+28h] ; cmp eax,[ebp+2Ch]` | tried: condition spellings (&&/||/negated, .Object, InternalGetObject, bool split), while/do/guarded do, shared/outer iterator, item reference, iterator copy, it++/+=1/=it+1, joins before the loop, Initialise in if/do-while(0), ERASE_OUTOFLINE off, CLEAR_OUTOFLINE on, /Oa /GS /Ow /Op /Gy /Ob1 /Ob2 /G5-7, QFE-4035 cl, empty-test spellings (size, pointer difference, end()!=begin(), &&-form) (8 grades + 58 compile-only probes) | next: c2prio shows the begin() temporary (2 refs, prio 20) beating `it` (13); truncation probes show our optimizer rotates the loop whenever a CALL follows it (store / if / loop after -> retail's form) while retail keeps the jmp form before ItemsToBuy.clear(); lane gap: loop rotation heuristic (the sibling 0x005A8C66 gets retail's `cmp [begin],reg` from the same `!v.empty()` spelling, so hunk 2 is context-driven too)

## Skipped
(none)

## Label corrections
- 0x0059D270 catalog `CTCInventoryTrade::AddWantedItemToList` is `AddWantedItemToListPC` (4 args, `ret 10h`; Ego_r/FableWin hold both
  the 3-arg AddWantedItemToList(long,CList*,CViewport*) and the 4-arg PC variant; the Wanted builder pushes frame, def, list, viewport).
- 0x004DD1C4 catalog `StdList_Dtor_004dd1c4` is the `std::_Rb_tree<NUISystem::CComponent*,std::pair<NUISystem::CComponent* const,long>,...>`
  destructor (`std::map<CComponent*,long>` local `branchCounts` in all three builders); 0x004DF81D `std::map<T_004df81d>::map` is that
  map's default constructor.
- 0x005B1C18 / 0x005B1C41 / 0x005B21A1 `RbTree_Clear_*` are `_Rb_tree::clear` of map<CComponent*,pair<long,long> > (ItemsToBuy /
  ItemsToSell), map<CComponent*,CList*> (Buy/Sell/WantedListsByCategory) and map<CComponent*,pair<CWideString,long> > (NumBuyable /
  NumSellable / NumWantedThings); 0x005BC4C4 `GetMember_dword0xb8_005bc4c4` is NInventory::CTCInventoryBase::GetBuildingThing (already
  in NInventory_CTCInventoryBase.h).
- The landed ConstructSubtitlesPC 0x0059E4D7 / FinalConstructSubtitlesPC 0x0059D65A model the map parameter as a local `CSubtitleMap`;
  the PDB type is std::map<NUISystem::CComponent*,std::pair<CWideString,long> > (see NAME_FIXES.tsv).
- Landed 0x00411570 `strcmp_signed` (global_global_00411570.cpp) is CBasicString<char>::Compare (NAME_FIXES.tsv).

## Compile-only probes
Besides the q.py grades in ATTEMPTS.log, the variants of the two parked rows were compiled with the same cl 13.10.3077 + overlay
(cc.py / ccf.sh, disassembly inspected) and run through c2prio (probe/prio_*.txt); they are summarised in ATTEMPTS.log.

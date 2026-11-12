# OUTLINE 0x005A04F7 CTCInventoryTrade::ConstructNormalShopList (9469 B, thiscall, ret 8)
Full annotated listing: $W/scout/c.txt (grep an address). Template rows: 0x005A5B31 (PC wanted, same frame form) and 0x005A29F4 (console twin).

## Frame facts
- `push ebp ; lea ebp,[esp-70h] ; sub esp,0D8h ; push ebx ; push esi ; push edi` (save order ebx, esi, edi). Locals [ebp-68h .. ebp+6Fh]; arg1 buyItems [ebp+78h], arg2 sellItems [ebp+7Ch] (both slots reused later).
- `#pragma optimize("s", on)` only. No EH, no /GS. ebx = this all function (until the tail reuses it), esi = -1 (CCharString len) most of the time, edi scratch / iterator.
- Epilogue 0x005A29E8: `pop edi ; mov eax,esi ; pop esi ; pop ebx ; add ebp,70h ; leave ; ret 8` (esi = list).

## Frame map (named locals; temporaries share the rest)
| slot | local | type | ctor / dtor |
|---|---|---|---|
| [ebp-68h] | sellActions (unused) | CCountedPointer<CComponent> | 0x00429C15 @0x005A0574 / ReleaseData @0x005A29E3 |
| [ebp-50h] | unused2 (default ctor, two zero stores @0x005A0581) | CCountedPointer<CComponent> | ReleaseData @0x005A29DB |
| [ebp-14h] | miscDef (unused) | CDefPointer<const CUIMiscThingsDef> | `and [ebp-14h],0` @0x005A05D2 / inline release @0x005A29C7 (slot first used as a vtable spill) |
| [ebp+5Ch] | manager | CManager* | |
| [ebp+1Ch] | list | CComponent* (UI_TRADING_MENU) | |
| [ebp+44h] | viewport | CViewport* | |
| [ebp+2Ch] | textBank | const CDataBank& (CMainGameComponent::Get()+14h), loaded first @0x005A0543 | |
| [ebp+6Ch] | entry (sell / buy / wanted entry, reassigned) | CComponent* | |
| [ebp+4Ch] | description | CCountedPointer<CComponent> | ctor @0x005A0652, then Reset everywhere / ReleaseData @0x005A29C2 |
| [ebp-60h] | sellTitle | CCountedPointer<CComponent> | @0x005A06B3 / @0x005A29BA |
| [ebp+4h] | branchCounts (sell; buy & wanted use [ebp+10h]) | std::map<CComponent*,long> | 0x004DF81D / 0x004DD1C4 (scoped to each `if`) |
| [ebp+30h] | xString | CWideString = GetXString() @0x005A0AFC | dtor @0x005A29AF |
| [ebp+28h] | title | CWideString (operator+ result @0x005A0B28) | dtor @0x005A29AA |
| [ebp-2Ch] | sellSubtitle | CCountedPointer<CComponent> | @0x005A0BB3 / @0x005A29A2 |
| [ebp-58h] | buyTitle | CCountedPointer<CComponent> | @0x005A1180 / @0x005A299A |
| [ebp-24h] | buySubtitle | CCountedPointer<CComponent> | @0x005A1704 / @0x005A2992 |
| [ebp+58h] | shop | CTCShop* = 0 @0x005A1C31 | |
| [ebp-38h] | wantedItems | std::vector<long> (by-value return of GetWantedItemsList) | free @0x005A2989 |
| [ebp-48h] | wantedTitle | CCountedPointer<CComponent> | @0x005A1D20 / @0x005A297C |
| [ebp-1Ch] | wantedSubtitle | CCountedPointer<CComponent> | @0x005A21E3 / @0x005A2974 |
| [ebp+14h] | screenTitle (scoped) | CCountedPointer<CComponent> | @0x005A27F1 / @0x005A295E |
| [ebp+8h] | screenTitleText (scoped) | CCountedPointer<CText> | zero stores @0x005A2807 / 0x0042925C @0x005A2956 |
| [ebp+7Ch] | titleText (tail, scoped) | CWideString | dtor @0x005A294E |
| [ebp+7Bh], [ebp-40h] | haveName bool / cond-dtor flag of the GetPTextEntry temp ([ebp+40h]) | | [ebp-40h]=0 at 0x005A050F |
Per-entry loop locals: index i [ebp+48h]; children vector& [ebp+60h]; subtitle CText* [ebp+40h] (sell) / [ebp+34h] (buy, wanted); subtitleText CWideString [ebp+6Ch] (sell) / [ebp+78h]; entryName CCharString [ebp+54h]. *_LIST loops: index [ebp+60h] (sell) / [ebp+78h].

## Body (retail order)
```cpp
CComponent* CTCInventoryTrade::ConstructNormalShopList(const std::vector<std::pair<long,long> >& buyItems,
                                                       const std::vector<std::pair<long,long> >& sellItems)
{
  // 0x005A0509
  ItemsToBuy.clear(); ItemsToSell.clear();                                     // 0x005B1C18 x2
  SellListsByCategory.clear(); BuyListsByCategory.clear(); WantedListsByCategory.clear();   // 0x005B1C41 x3
  const CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();         // 0x005A0543 -> [ebp+2Ch]
  CCountedPointer<CComponent> sellActions(CManager::GetInstance()->CreateComponent("UI_LIST_SELL_ACTIONS", false)); // 0x005A054B
  CCountedPointer<CComponent> unused2;                                           // 0x005A0581
  CManager* manager = CManager::GetInstance();                                   // 0x005A0587
  CComponent* list = manager->CreateComponent("UI_TRADING_MENU", false);         // 0x005A05A5
  CViewport* viewport = (CViewport*)list->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");   // 0x005A05B7
  CDefPointer<const CUIMiscThingsDef> miscDef;                                   // 0x005A05D2
  long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);  // 0x005A05D6
  CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);                     // 0x005A0604 (0x0042B312)
  // ---- SELL ----
  CComponent* entry = list->FindChild("UI_TRADING_MENU_SELL_ENTRY");             // 0x005A0617 -> [ebp+6Ch]
  CCountedPointer<CComponent> description(manager->CreateComponent("UI_TRADE_SELL_DESC", false));   // 0x005A062D
  entry->AddChild(description);                                                  // 0x005A065F  +ECh
  viewport->AddFakeChild(description.Get());                                     // 0x005A066D  +238h
  CList* goods = (CList*)entry->FindChild("UI_TRADING_GOODS_LIST");              // 0x005A067B
  CCountedPointer<CComponent> sellTitle(manager->CreateComponent("UI_TRADE_SELL_TITLE", false));   // 0x005A0691
  goods->AddNonScrollingChild(sellTitle);                                        // 0x005A06C0  +244h
  TotNumClothesToSell = TotNumWeaponsToSell = TotNumItemsToSell = 0;             // 0x005A06CE (stores 1DC,1D8,1D4)
  if (!sellItems.empty()) {                                                      // 0x005A06E5 cmp [edi],[edi+4]; je 0x005A0AF9
    std::map<CComponent*, long> branchCounts;                                    // 0x005A06F0
    for (it = sellItems.begin(); it != sellItems.end(); ++it) {                  // body 0x005A06FF, test 0x005A0744; it in [ebp+7Ch]
      EItem_Menu_Type type;                                                      // [ebp+40h]
      CComponent* branch = GetMenuBranchFromDefIndex(it->first, entry, type);    // 0x005A070D
      iterator found = branchCounts.find(branch);                                // 0x005B1AC5
      if (found == branchCounts.end()) branchCounts[branch] = 1;                 // 0x0042D1B7
      else ++found->second;                                                      // 0x005A073A inc [eax+14h]
    }
    // 0x005A0749: loop entered at its test (jmp 0x005A07E4); iterator kept in [ebp+7Ch]
    for (b = branchCounts.begin(); b != branchCounts.end(); ++b) {               // body 0x005A0756, ++ = 0x004292C0
      CCountedPointer<CComponent> subListPtr(manager->CreateComponent("UI_SUB_LIST", false));   // [ebp+3Ch]
      CList* subList = (CList*)subListPtr.Get();                                 // [ebp+34h]
      subList->SetName(CATEGORY_CHILD_SELL_LIST_STRING);                         // 0x005A0790 +14Ch (SELL ONLY)
      CComponent* branch = b->first;                                             // [ebp+68h]
      branch->AddChild(subListPtr);                                              // 0x005A07A5
      subList->InitialiseOffsets(b->second);                                     // 0x005A07B6 +270h
      SellListsByCategory[branch] = subList;                                     // 0x005A07C6 (0x005B3076, key = &local)
    }                                                                            // ReleaseData 0x005A07D3
    NumSellableThings.clear();                                                   // 0x005A07ED (0x005B21A1)
    for (it = sellItems.begin(); it != sellItems.end(); ++it) {                  // jmp test 0x005A08C0; body 0x005A0802; it in [ebp+7Ch]
      EItem_Menu_Type type;                                                      // [ebp+68h]
      CComponent* branch = GetMenuBranchFromDefIndex(it->first, entry, type);    // 0x005A0810 -> [ebp+54h]
      if (AddSellItemToList(it->first, it->second, SellListsByCategory.find(branch)->second, viewport, false)) {  // 0x005A0839
        if (NumSellableThings.find(branch) == NumSellableThings.end())           // 0x005A084C
          NumSellableThings[branch].second = it->second;                         // 0x005A0863
        else NumSellableThings[branch].second += it->second;                     // 0x005A0873
        switch (type) {                                                          // 0x005A0883 sub 0 / dec / dec
          case 0: TotNumClothesToSell += it->second; break;                      // 0x005A08AD
          case 1: TotNumWeaponsToSell += it->second; break;                      // 0x005A089F
          case 2: TotNumItemsToSell   += it->second; break;                      // 0x005A0891 (placed first)
        }
      }
    }
    iterator end = NumSellableThings.end();                                      // 0x005A08C9 cached in [ebp+24h]
    for (s = NumSellableThings.begin(); s != end; ++s) {                         // body 0x005A08DD, iterator in edi
      CText* text = (CText*)s->first->FindFirstChildOfType(UI_TYPE_TEXT);        // +24h(6) -> [ebp+20h]
      CWideString value(text->GetValue());                                       // [ebp+7Ch]
      s->second.first = value;                                                   // 0x005A0904
      if (s->second.second > 0) { value += CTCInventoryStats::GetXString(); value += GFIntToWideString(s->second.second); }
      text->SetValue(value, true, true);                                         // 0x005A0951 +240h
    }
    { CText* t = (CText*)entry->FindChild("UI_TRADING_CLOTHING_TEXT"); CWideString v(t->GetValue());   // 0x005A0971
      v += CTCInventoryStats::GetXString(); v += GFIntToWideString(TotNumClothesToSell); t->SetValue(v, true, true); }
    { ... "UI_TRADING_WEAPONS_TEXT" ... TotNumWeaponsToSell ... }                // 0x005A09F1
    { ... "UI_TRADING_ITEMS_TEXT"   ... TotNumItemsToSell   ... }                // 0x005A0A71
  }                                                                              // ~map 0x005A0AF1
  CWideString xString = CTCInventoryStats::GetXString();                         // 0x005A0AF9 [ebp+30h]
  CWideString title = textBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + xString;  // 0x005A0B01..0x005A0B28 [ebp+28h]
  title += GFIntToWideString(TotNumItemsToSell + TotNumClothesToSell + TotNumWeaponsToSell);  // 0x005A0B3D (1DC+1D4+1D8)
  ((CText*)entry->FindChild("UI_TRADING_SELL_TEXT"))->SetValue(title, true, true);   // 0x005A0B68
  CCountedPointer<CComponent> sellSubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));  // 0x005A0B93 [ebp-2Ch]
  CText* sellSubtitleText = (CText*)sellSubtitle.Get();                          // edi
  sellSubtitleText->SetName(SELL_SUBTITLE);                                      // 0x005A0BCC
  sellSubtitleText->SetValue(title, true, true);                                 // 0x005A0BDE
  entry->AddChild(sellSubtitle);                                                 // 0x005A0BEF
  CComponent* menu = entry->FindChild("UI_TRADING_GOODS_LIST");                  // 0x005A0BF5 -> [ebp+7Ch]
  std::vector<CCountedPointer<CComponent> >& entries = menu->GetChildren();      // 0x005A0C12 +D4h -> edi / [ebp+60h]
  for (long i = 0; i < (long)entries.size(); ++i) {                              // [ebp+48h]; pre-test 0x005A0C26 jle 0x005A0F14; body 0x005A0C31; latch 0x005A0F00
    description.Reset(manager->CreateComponent("UI_TRADE_SELL_DESC", false));    // 0x005A0C51
    entries[i]->AddChild(description);                                           // 0x005A0C6C
    viewport->AddFakeChild(description.Get());                                   // 0x005A0C7A
    sellSubtitle.Reset(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));  // 0x005A0CA0
    CText* subText = (CText*)sellSubtitle.Get();                                 // [ebp+40h]
    CWideString subtitleText;                                                    // 0x005A0CB6 [ebp+6Ch]
    CCharString entryName = entries[i]->GetName();                               // 0x005A0CC9 +150h [ebp+54h]
    if (entryName == "UI_TRADING_MENU_CLOTHING_ENTRY") {                         // null arm 0x005A0CD5 (repe cmpsb vs ""), else 0x005A0D1E (0x004115A0); arm 0x005A0D2F
      subtitleText = textBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + xString;
      subtitleText += GFIntToWideString(TotNumClothesToSell);
      sellSubtitle->SetName(CLOTHES_SUBTITLE);                                   // push 0x013B8B74; jmp 0x005A0EBB (shared SetName call)
    } else if (entryName == "UI_TRADING_MENU_WEAPONS_ENTRY") {                   // 0x005A0CE8 / 0x005A0DA8; arm 0x005A0DB9
      ... "TEXT_GUI_MENU_WEAPONS" ... TotNumWeaponsToSell ... WEAPONS_SUBTITLE (0x013B8B70)
    } else if (entryName == "UI_TRADING_MENU_ITEMS_ENTRY") {                     // 0x005A0CFF / 0x005A0E32; arm 0x005A0E47
      ... "TEXT_GUI_MENU_ITEMS" ... TotNumItemsToSell ... ITEMS_SUBTITLE (0x013B8B78)
    }                                                                            // no match -> 0x005A0EC6 / 0x005A0EC9
    subText->SetValue(subtitleText, true, true);                                 // 0x005A0EC9
    entries[i]->AddChild(sellSubtitle);                                          // 0x005A0EDC
  }                                                                              // ~entryName 0x005A0EF0, ~subtitleText 0x005A0EF8
  { std::vector<CCountedPointer<CComponent> >& items = menu->FindChild("UI_TRADING_CLOTHING_LIST")->GetChildren();  // 0x005A0F14
    for (long i = 0; i < (long)items.size(); ++i) { description.Reset(manager->CreateComponent("UI_TRADE_SELL_DESC", false));
      items[i]->AddChild(description); viewport->AddFakeChild(description.Get()); } }   // body 0x005A0F49, index [ebp+60h]
  { ... "UI_TRADING_WEAPONS_LIST" ... }                                          // 0x005A0FA8
  { ... "UI_TRADING_ITEMS_LIST" ... }                                            // 0x005A103C
  ConstructSubtitles(menu, NumSellableThings);                                   // 0x005A10DC
  // ---- BUY ----  (same shape; differences marked)
  entry = list->FindChild("UI_TRADING_MENU_BUY_ENTRY");                          // 0x005A10E1
  description.Reset(manager->CreateComponent("UI_TRADE_BUY_DESC", false));       // 0x005A111F (Reset, not ctor)
  entry->AddChild(description); viewport->AddFakeChild(description.Get());       // 0x005A112C / 0x005A113A
  goods = (CList*)entry->FindChild("UI_TRADING_GOODS_LIST");                     // 0x005A1148
  CCountedPointer<CComponent> buyTitle(manager->CreateComponent("UI_TRADE_BUY_TITLE", false));   // 0x005A1180 [ebp-58h]
  goods->AddNonScrollingChild(buyTitle);                                         // 0x005A1195
  TotNumClothesToBuy = TotNumWeaponsToBuy = TotNumItemsToBuy = 0;                // 0x005A119E (1E8,1E4,1E0)
  if (!buyItems.empty() || !NotInStock.empty()) {                                // 0x005A11B2 jne body / 0x005A11B9 je 0x005A1639
    std::map<CComponent*, long> branchCounts;                                    // [ebp+10h] 0x005A11C7
    for (it over buyItems) {...same count..., type [ebp+40h], branch [ebp+64h]}  // 0x005A11D6..0x005A121E, it in [ebp+7Ch]
    for (std::list<long>::iterator n = NotInStock.begin(); n != NotInStock.end(); ++n) {  // 0x005A1220..0x005A1270, n in edi
      EItem_Menu_Type type; CComponent* branch = GetMenuBranchFromDefIndex(*n, entry, type);   // both in [ebp+7Ch]
      ...same find / operator[]=1 / ++found->second... }
    for (b ...) {                                                                // 0x005A1272 je 0x005A12F6; body 0x005A127C; b in edi
      CCountedPointer<CComponent> subListPtr(manager->CreateComponent("UI_SUB_LIST", false));
      CList* subList = (CList*)subListPtr.Get();                                 // [ebp+64h]  (NO SetName)
      b->first->AddChild(subListPtr); subList->InitialiseOffsets(b->second);
      BuyListsByCategory[b->first] = subList; }                                  // key = &node+10h
    NumBuyableThings.clear();                                                    // 0x005A12F6
    for (it over buyItems) {                                                     // 0x005A130B..0x005A13B6, it in edi
      EItem_Menu_Type type; CComponent* branch = GetMenuBranchFromDefIndex(it->first, entry, type);   // type [ebp+40h], branch [ebp+7Ch]
      if (AddBuyItemToList(it->first, it->second, BuyListsByCategory.find(branch)->second, viewport, false)) {  // 0x005A133C
        NumBuyableThings find/[]= / += ... ; switch(type) TotNum*ToBuy += it->second; } }   // 1E0/1E4/1E8
    end = NumBuyableThings.end() ([ebp+0Ch]); label loop 0x005A13D0 (value [ebp+78h])
    three texts UI_TRADING_CLOTHING/WEAPONS/ITEMS_TEXT with TotNum*ToBuy        // 0x005A1464 / 0x005A14E4 / 0x005A1564
    for (n over NotInStock) {                                                    // 0x005A15E4..0x005A162F
      EItem_Menu_Type type; CComponent* branch = GetMenuBranchFromDefIndex(*n, entry, type);   // [ebp+78h]
      AddBuyItemToList(*n, 0, BuyListsByCategory.find(branch)->second, viewport, false); }    // 0x005A1622
  }                                                                              // ~map 0x005A1631
  title = textBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + xString;               // 0x005A1639 (operator=)
  title += GFIntToWideString(TotNumWeaponsToBuy + TotNumItemsToBuy + TotNumClothesToBuy);   // 0x005A1686 (1E4+1E8+1E0)
  ((CText*)entry->FindChild("UI_TRADING_BUY_TEXT"))->SetValue(title, true, true);    // 0x005A16B1
  CCountedPointer<CComponent> buySubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));   // 0x005A16E4 [ebp-24h]
  CText* buySubtitleText = (CText*)buySubtitle.Get();                            // [ebp+34h]
  buySubtitleText->SetName(BUY_SUBTITLE); buySubtitleText->SetValue(title, true, true); entry->AddChild(buySubtitle);   // 0x005A171E..0x005A173F
  menu = entry->FindChild("UI_TRADING_GOODS_LIST"); entries = menu->GetChildren(); // 0x005A1745 (menu [ebp+7Ch], entries [ebp+60h], i [ebp+48h])
  per-entry loop as SELL with "UI_TRADE_BUY_DESC", buySubtitle, TotNum*ToBuy     // 0x005A1781..0x005A1A5E (arms 0x005A187F / 0x005A1909 / 0x005A1997; SetName join 0x005A1A0B)
  three *_LIST description loops with "UI_TRADE_BUY_DESC" (index [ebp+78h])      // 0x005A1A64 / 0x005A1AF8 / 0x005A1B8C
  ConstructSubtitles(menu, NumBuyableThings);                                    // 0x005A1C2C
  // ---- WANTED ----
  CTCShop* shop = 0;                                                             // 0x005A1C31 [ebp+58h]
  GetBuildingThing()->GetTC(TCI_SHOP /*0x53*/, &shop);                           // 0x005A1C37 (inline: bitset test 0x00410DE0, LowerBound 0x0040F020)
  std::vector<long> wantedItems = shop->GetWantedItemsList();                    // 0x005A1C7C [ebp-38h]
  entry = list->FindChild("UI_TRADING_MENU_WANTED_ENTRY");                       // 0x005A1C81
  description.Reset(manager->CreateComponent("UI_TRADE_WANTED_DESC", false)); entry->AddChild(description); viewport->AddFakeChild(description.Get());
  goods = (CList*)entry->FindChild("UI_TRADING_GOODS_LIST");                     // 0x005A1CE8
  CCountedPointer<CComponent> wantedTitle(manager->CreateComponent("UI_TRADE_WANTED_TITLE", false));   // 0x005A1D20 [ebp-48h]
  goods->AddNonScrollingChild(wantedTitle);                                      // 0x005A1D35
  TotNumClothesWanted = TotNumWeaponsWanted = TotNumItemsWanted = 0;             // 0x005A1D3B (1F4,1F0,1EC)
  if (!wantedItems.empty()) {                                                    // 0x005A1D4F je 0x005A210B (begin != end)
    std::map<CComponent*, long> branchCounts;                                    // [ebp+10h] 0x005A1D5B
    for (std::vector<long>::iterator it = ...) {...count...}                     // 0x005A1D68..0x005A1DA9, it in edi, type/branch [ebp+78h]
    for (b ...) { UI_SUB_LIST; subList [ebp+7Ch]; b->first->AddChild; InitialiseOffsets; WantedListsByCategory[b->first] = subList; }  // 0x005A1DB5
    NumWantedThings.clear();                                                     // 0x005A1E2F (lea edi,[ebx+1C8h] kept in edi for the next loop)
    for (it ...) {                                                               // 0x005A1E47..0x005A1ED9, it in [ebp+78h]
      EItem_Menu_Type type; CComponent* branch = GetMenuBranchFromDefIndex(*it, entry, type);   // type [ebp+40h], branch [ebp+7Ch]
      AddWantedItemToList(*it, WantedListsByCategory.find(branch)->second, viewport);   // 0x005A1E79 (no if)
      if (NumWantedThings.find(branch) == NumWantedThings.end()) NumWantedThings[branch].second = 1;
      else ++NumWantedThings[branch].second;
      switch (type) { case 0: ++TotNumClothesWanted; break; case 1: ++TotNumWeaponsWanted; break; case 2: ++TotNumItemsWanted; break; } }
    end = NumWantedThings.end(); label loop                                      // 0x005A1EDF / 0x005A1EEF
    three texts with TotNum*Wanted                                               // 0x005A1F83 ..
  }                                                                              // ~map 0x005A2103
  title = textBank.GetTextBySymbol("TEXT_GUI_MENU_WANTED") + CTCInventoryStats::GetXString();   // 0x005A210B (temp, NOT xString)
  title += GFIntToWideString(TotNumWeaponsWanted + TotNumItemsWanted + TotNumClothesWanted);   // 0x005A2165 (1F0+1F4+1EC)
  ((CText*)entry->FindChild("UI_TRADING_WANTED_TEXT"))->SetValue(title, true, true);   // 0x005A2190
  CCountedPointer<CComponent> wantedSubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));   // 0x005A21C3 [ebp-1Ch]
  ((CText*)wantedSubtitle.Get())->SetName(WANTED_SUBTITLE); ...SetValue(title,true,true); entry->AddChild(wantedSubtitle);
  menu / entries / per-entry loop with "UI_TRADE_WANTED_DESC", wantedSubtitle, TotNum*Wanted, xString   // 0x005A2224..0x005A253D
  three *_LIST loops "UI_TRADE_WANTED_DESC"                                      // 0x005A2543 / 0x005A25D7 / 0x005A266B
  ConstructSubtitles(menu, NumWantedThings);                                     // 0x005A270B
  // ---- tail: identical spelling to 0x005A29F4's tail except the title text ----
  CComponent* gold = list->FindChild("UI_GOLD_AMOUNT_TRADE_SCREEN");             // 0x005A2710
  if (gold) { const CTCHeroStats* heroStats = 0;                                 // 0x005A2731
    CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_HERO_STATS /*4*/, &heroStats);  // LowerBound 0x004365B0
    ((CText*)gold)->SetValue(GFIntToWideString(heroStats->GetMoney()), true, true); }   // 0x005A279A
  CComponent* screen = list->FindChild("UI_TRADING_MENU_SCREEN");                // 0x005A27AD
  if (screen) {                                                                  // je 0x005A2963
    CCountedPointer<CComponent> screenTitle(CManager::GetInstance()->CreateComponent("UI_TITLE_NEW", false));   // 0x005A27CE
    CCountedPointer<CText> screenTitleText;                                      // 0x005A2807
    CWideString titleText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_TRADE");    // 0x005A27FE
    if (shop->GetName() > 0) {                                                   // 0x005A2834 jbe 0x005A291B
      bool haveName = CMainGameComponent::Get()->GetTextBank().EntryExists(shop->GetName())
                   && CMainGameComponent::Get()->GetTextBank().GetTextInfo(shop->GetName()) != 0
                   && !CMainGameComponent::Get()->GetTextBank().GetPTextEntry(shop->GetName(), true).IsNull();  // 0x005A2841..0x005A28C1 (flag [ebp-40h])
      if (haveName)                                                              // 0x005A28C3
        titleText = CMainGameComponent::Get()->GetTextBank().GetPTextEntry(shop->GetName(), true)->GetText();   // 0x005A28C9..0x005A2918
    }
    screenTitleText.SetStaticCast(screenTitle);                                  // 0x005A2924 (0x00429460)
    screenTitleText->SetValue(titleText, true, true);                            // 0x005A2936
    screen->AddChildDynamically(screenTitle);                                    // 0x005A2945 +F0h
  }
  static_cast<CList*>(list)->SetDefaultChild(0L);                                // 0x005A296B +278h
  return list;                                                                   // dtors 0x005A2971..0x005A29E3 in reverse construction order
}
```
No jump tables (each switch is `sub eax,0 ; je / dec ; je / dec ; jne`). No cold blocks after the epilogue.

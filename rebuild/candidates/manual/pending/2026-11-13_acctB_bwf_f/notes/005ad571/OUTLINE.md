# OUTLINE 0x005AD571 CTCInventoryTrade::ConstructTradeDescription(long itemDefIndex, unsigned long count, bool buying, bool special)

## Frame
`push ebp; mov ebp,esp; sub esp,74h; push esi; push edi` (ebx pushed later at 0x005AD5C3). No EH, no cookie. `ret 10h`. Return eax=[ebp-14h].
| slot | holds |
|---|---|
| [ebp-74h..-6Ch] | std::vector<CWideString> args (loop body only; zeroed 0x005AE40D, ~vector 0x00412130 at 0x005AE588) |
| [ebp-68h,-64h] | CCountedPointer<CComponent> textComponent {Data, Info} (zeroed 0x005AD5D6; ReleaseData 0x004291DE at exit) |
| [ebp-60h] | CDefPointer<const CHeroCombatDef> combatDef |
| [ebp-5Ch] | map key temp 0 (loop colour) |
| [ebp-58h..-4Ch] | CWideString temporaries pushed into args |
| [ebp-48h] | CTCInventoryExperience* invExperience |
| [ebp-44h..-30h] | CWideString / CCharString temporaries; [ebp-44h] also TCI key 0x53 and loop `fmt`; [ebp-34h] also maxStock |
| [ebp-2Ch] | CCharString temp (container template name) |
| [ebp-28h] | CDefPointer<const CDefClassBase> def (GetPDefFromGlobalIndex) |
| [ebp-24h] | CDefPointer<const CStockItemDef> stockDef (earlier: CCharString temps) |
| [ebp-20h] | CDefPointer<const CUIMiscThingsDef> miscDef (value cached in esi from 0x005AD778) ; later `const CTCInventoryBase* inventory` (!special path) |
| [ebp-1Ch] | CTCShop* shop ; later long days ; later CDefPointer<const CThingBaseDef> thingDef |
| [ebp-18h] | long price ; later CTCHeroExperience* experience |
| [ebp-14h] | CComponent* container (return) |
| [ebp-10h] | this |
| [ebp-0Ch] | const NGameText::CDataBank& bank |
| [ebp-8] | CManager* manager |
| [ebp-4] | CWideString text |
| [ebp+0Ch] | count -> long value (default price) -> temps / PeekTC key+inventory (special path) / byte [ebp+0Fh] isSpecialShop / CDefPointer<const CWeaponDef> weaponDef |
| [ebp+10h],[ebp+14h] | buying/special -> CCharString/CWideString temps, map iterator ([ebp+10h] in loop), line pointer ([ebp+14h]) |

## Body (retail order)
```cpp
// 0x005AD57E
if (CTCInventoryItem::IsWeapon(itemDefIndex))                       // 0x005D97F0
    return ConstructTradeDescriptionWeapon(itemDefIndex, count, buying, special);   // 0x005A9CBE, jmp 0x005AE6A9
// 0x005AD5A2
if (CTCInventoryItem::IsClothing(itemDefIndex))                     // 0x005D9C60
    return ConstructTradeDescriptionClothing(itemDefIndex, count, buying, special); // 0x005AAF28
// 0x005AD5C3
CManager* manager = CManager::GetInstance();                        // [ebp-8]
CCountedPointer<CComponent> textComponent;                          // [ebp-68h]
CComponent* container;                                              // [ebp-14h]
if (InventoryTradeOpen) {   // [this+218h]; `push -1` hoisted above je; arms cross-jumped at 0x005AD635 (identical tail)
    container = manager->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT", false);   // 0x012537FC
    textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));             // 0x012537D8
} else {                    // 0x005AD60D
    container = manager->CreateComponent("UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE", false);                 // 0x012537B0
    textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));             // 0x01247D70
}
// 0x005AD65A
CWideString text;                                                   // 0x0099AED0
CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;             // [ebp-20h]=0 (store interleaved)
long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);   // 0x0044C6B0, 0x009AD390
CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);          // 0x0042B312
CText* line = (CText*)textComponent.Get();                          // edi, loaded at 0x005AD6A4 BEFORE the shop lookup
CTCShop* shop = 0;                                                  // [ebp-1Ch]
GetBuildingThing()->GetTC(&shop);   // 0x005BC4C4 + expanded GetTC (test(0x53) 0x00410DE0, LowerBound 0x0040F020)
// 0x005AD6E8
long price;                                                         // [ebp-18h]
if (buying && !special) price = shop->GetCurrentSellPrice(itemDefIndex, count);   // 0x007743B0
else                    price = shop->GetCurrentBuyPrice(itemDefIndex, count);    // 0x005AD702 -> 0x007742C0
// 0x005AD710
CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(itemDefIndex);   // 0x006869D0, 0x009AD9E0 ([ebp-28h])
CDefPointer<const CStockItemDef> stockDef;                          // [ebp-24h]
long value;                                                         // [ebp+0Ch]
if (!def.IsNull() && def->GetSubDef(&stockDef)) value = stockDef->DefaultPrice;   // 0x0059350C, +0x28
else value = 1;                                                     // 0x005AD74B
// 0x005AD752
const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();      // [0x013B86A0]+0x14 (load interleaved with the test)
if (special) {                                                      // je 0x005ADA1E
    // 0x005AD766
    text = bank.GetTextBySymbol("TEXT_GUI_PRICE") + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);   // 0x012501E4; op+ x3, op= 0x0099B7D0
    text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(value) + miscDef->CloseBracket;                     // 0x005AD7F3; += 0x0099B8D0
    container->AddChild(textComponent);                             // slot 59 +0xEC (0x005AD858)
    line->SetValue(text, true, true);                               // slot 144 +0x240
    // 0x005AD870 colour: textComponent->GetStates()[0] (slot 82 +0x148 then map[] 0x0042D5B1, key temp in [ebp+10h])
    if (price < value) {            // jge 0x005AD8D1
        textComponent->GetStates()[0].Colour.B *= 0.5f;             // +0x18
        textComponent->GetStates()[0].Colour.G *= 0.5f;             // +0x19, tail cross-jumped to 0x005AD930
    } else if (price > value) {     // 0x005AD8D1, jle 0x005AD93D
        textComponent->GetStates()[0].Colour.B *= 0.5f;
        textComponent->GetStates()[0].Colour.R *= 0.5f;             // +0x1A
    }
    // 0x005AD93D
    if (InventoryTradeOpen) textComponent.Reset(manager->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else                    textComponent.Reset(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));  // cross-jumped at 0x005AD959, temp [ebp+10h]
    CText* descLine = (CText*)textComponent.Get();                  // [ebp+14h]
    text = CWideString::NullString;                                 // 0x013BCA24
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();  // +0x1C, 0x00449960, 0x00487DD0
    const NInventory::CTCInventoryBase* inventory = 0;              // [ebp+0Ch]
    hero->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);   // 0x005D99D0; expanded: test 0x00410DE0, LowerBound const 0x004365B0
    inventory->GetItemDescription(text, itemDefIndex, true);        // 0x005AD9EC slot 78 +0x138
    container->AddChild(textComponent);
    descLine->SetValue(text, true, true);                           // jmp 0x005AE654 (shared call with the end)
} else {
    // 0x005ADA1E
    text = bank.GetTextBySymbol(miscDef->TradeCostString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);  // +0x80, 0x0099B2C0
    text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(value) + miscDef->CloseBracket;
    container->AddChild(textComponent);
    line->SetValue(text, true, true);
    // 0x005ADB25  (X = red: B,G ; Y = green: B,R)
    if ((price > value && buying) || (price < value && !buying)) { /* 0x005ADB3F X */ B *= 0.5f; G *= 0.5f; }
    else if ((price < value && buying) || (price > value && !buying)) { /* 0x005ADB9F Y */ B *= 0.5f; R *= 0.5f; }
    //   branch skeleton: cmp p,v; jle B1; cmp buying; jne X | B1: cmp p,v; jge B2; cmp buying; jne Y; (fall X)
    //   | B2(0x005ADB98): jle end; cmp buying; jne end; (fall Y). X and Y share the fmul/__ftol2 tail at 0x005ADBF6.
    // 0x005ADC03
    const CThingPlayerCreature* hero = ...PeekPControlledCreature();   // as above
    const NInventory::CTCInventoryBase* inventory = 0;              // [ebp-20h] (miscDef's slot; miscDef lives on in esi)
    hero->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);   // key temp [ebp+14h]
    // 0x005ADC5E
    bool isSpecialShop = shop->IsBarberShop() || shop->IsTattooShop() || shop->IsTitleShop();   // byte [ebp+0Fh]
    if (buying) {                                                   // je 0x005ADD94
        if (!isSpecialShop) {                                       // jne 0x005AE279 (jump-threaded past the stock block)
            // 0x005ADC9B
            if (InventoryTradeOpen) textComponent.Reset(...PC_TEXT...); else textComponent.Reset(...UI_TEXT...);   // temp [ebp+0Ch]
            CText* ownsLine = (CText*)textComponent.Get();          // edi
            long owned = inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex);   // 0x005BDF08
            text = bank.GetTextBySymbol(miscDef->TradeAlreadyOwnsString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(owned);  // +0x8C
            container->AddChild(textComponent);
            ownsLine->SetValue(text, true, true);                   // then jmp 0x005ADFED
        }
    } else {
        // 0x005ADD94
        if (InventoryTradeOpen) textComponent.Reset(...PC_TEXT...); else ...UI_TEXT...;   // temp [ebp+14h]
        CText* profitLine = (CText*)textComponent.Get();            // [ebp+14h]
        long profit = price - inventory->GetBoughtPrice(itemDefIndex);   // 0x005BEAD5; edi; `js` on the sub
        if (profit >= 0) {   // GFIntToWideString's ecx/edx set up before the branch (both arms start with it)
            text = bank.GetTextBySymbol(miscDef->TradeProfitString.ToCharString()) + Colon + Space + GFIntToWideString(profit);   // +0x84
            if (profit != 0) { B *= 0.5f; R *= 0.5f; }              // 0x005ADE7F green; tail shared at 0x005ADFB5
        } else {             // 0x005ADEDB
            text = bank.GetTextBySymbol(miscDef->TradeLossString.ToCharString()) + Colon + Space + GFIntToWideString(-profit);    // +0x88, neg edx
            B *= 0.5f; G *= 0.5f;                                   // red
        }
        // 0x005ADFC2
        container->AddChild(textComponent);
        profitLine->SetValue(text, true, true);
    }
    // 0x005ADFE4
    if (!isSpecialShop) {   // 0x005ADFE4 `jne 0x005AE5D6`: on this (!buying) path the weapon test below is known false, so it is threaded straight to the end block
        // 0x005ADFED
        if (InventoryTradeOpen) textComponent.Reset(...PC_TEXT...); else ...UI_TEXT...;   // temp [ebp+0Ch]
        CText* stockLine = (CText*)textComponent.Get();
        long stock = shop->GetStockQuantity(itemDefIndex);          // 0x007726B0 [ebp+0Ch]
        long maxStock = shop->GetMaxStock(itemDefIndex);            // 0x00772720 [ebp-34h]
        text = bank.GetTextBySymbol(miscDef->TradeNumberInStockString.ToCharString()) + Colon + Space + GFIntToWideString(stock);  // +0x90
        text += L"/" + GFIntToWideString(maxStock);                 // 0x0099BFF0, L"/" 0x0122DE74
        container->AddChild(textComponent);
        stockLine->SetValue(text, true, true);
        // 0x005AE125
        if (InventoryTradeOpen) textComponent.Reset(...PC_TEXT...); else ...UI_TEXT...;
        CText* deliveryLine = (CText*)textComponent.Get();
        long days = shop->GetDaysTillRestock(itemDefIndex);         // 0x00772790 -> [ebp-1Ch]
        text = bank.GetTextBySymbol(miscDef->TradeDeliveryString.ToCharString()) + Colon + Space;   // +0x94
        if (days != 0) {                                            // je 0x005AE223
            CWideString daysText = bank.GetTextBySymbol("TEXT_GUI_MENU_DELIVERY_DAYS");   // 0x012538A8, [ebp+0Ch]
            NGameText::ConstructString(daysText, days);             // 0x009C7B70
            text += daysText;
        } else {
            text += bank.GetTextBySymbol(miscDef->TradeNoDeliveryString.ToCharString());  // +0x98
        }
        // 0x005AE258
        container->AddChild(textComponent);
        deliveryLine->SetValue(text, true, true);
    }
    // 0x005AE279
    if (buying && CTCInventoryItem::IsWeapon(itemDefIndex)) {      // dead at run time
        CDefPointer<const CThingBaseDef> thingDef;                  // [ebp-1Ch]
        GetDefinitionManager().GetDef(itemDefIndex, &thingDef);     // 0x006869D0, 0x0042AF3C
        CDefPointer<const CWeaponDef> weaponDef;                    // [ebp+0Ch]
        thingDef->GetSubDef(&weaponDef);                            // 0x0042B1A6 (no null test on thingDef)
        if (weaponDef->Class == WC_HEAVY) {                         // +0x2C == 2
            CTCHeroExperience* experience;      // [ebp-18h]
            CTCInventoryExperience* invExperience;   // [ebp-48h]
            if (ParentThing->GetTC(&experience) && ParentThing->GetTC(&invExperience)) {   // 0x68 / 0x16; ParentThing reloaded each time
                CDefPointer<const CHeroCombatDef> combatDef = experience->GetPHeroCombatDef();   // [ebp-60h]; null test, then inc;inc;dec
                if (!combatDef.IsNull()) {
                    for (it = combatDef->HeavyWeapons.StatThresholds.begin(); it != end; ++it) {   // [ebp+10h]; header [edi+13Ch]; ++ = 0x004292C0
                        // 0x005AE380
                        if (experience->GetTrainableStatLevel(it->first) < it->second) {   // 0x006D6520, jge 0x005AE590
                            if (InventoryTradeOpen) textComponent.Reset(...PC_TEXT...);   // temp [ebp-30h]
                            else                    textComponent.Reset(...UI_TEXT...);   // temp [ebp-34h]; only ~CCharString tail-merged (0x005AE3F5)
                            CText* reqLine = (CText*)textComponent.Get();   // [ebp+14h]
                            std::vector<CWideString> args;                  // [ebp-74h]
                            args.push_back(invExperience->GetExperienceStatName(it->first));     // 0x0056F26F, 0x004128A0
                            args.push_back(bank.GetTextBySymbol("TEXT_GUI_MENU_LEVEL"));         // 0x01253894
                            args.push_back(GFIntToWideString(it->second));
                            args.push_back(bank.GetTextBySymbol("TEXT_GUI_MENU_REQUIRED"));      // 0x01253798
                            CWideString fmt = bank.GetTextBySymbol("TEXT_GUI_MENU_STAT_LEVEL");  // 0x01253878, [ebp-44h]
                            text = NGameText::ConstructString(fmt, args);   // 0x009C7C20 -> movzx ax,al -> operator=(wchar_t) 0x0099B860 (sic)
                            textComponent->GetStates()[0].Colour.B *= 0.5f;   // red (on textComponent, not reqLine)
                            textComponent->GetStates()[0].Colour.G *= 0.5f;
                            container->AddChild(textComponent);
                            reqLine->SetValue(text, true, true);
                        }   // ~fmt 0x0099B510, ~args 0x00412130
                    }
                }   // combatDef release (no null test, 0x005AE5A8)
            }
        }   // 0x005AE5B4 weaponDef release (NO null test); 0x005AE5C3 thingDef release (null test)
    }
    // 0x005AE5D6
    if (InventoryTradeOpen) textComponent.Reset(...PC_TEXT...); else ...UI_TEXT...;   // temp [ebp+10h], cross-jumped 0x005AE5F2
    CText* descLine = (CText*)textComponent.Get();                  // edi
    text = CWideString::NullString;
    inventory->GetItemDescription(text, itemDefIndex, true);        // [ebp-20h], slot 78
    container->AddChild(textComponent);
    descLine->SetValue(text, true, true);                           // 0x005AE654
}
// 0x005AE65A destructors (reverse decl): stockDef [ebp-24h] (null test), def [ebp-28h] (RELEASE_POINTEE form + Object=0), pop ebx,
// miscDef via esi (null test), ~text, textComponent ReleaseData 0x004291DE; return container.
return container;
```
Control-flow detail: after the !buying arm (0x005ADFE4) retail tests isSpecialShop and jumps to 0x005AE5D6 when set (the weapon
block's `buying` test is known false on that path). The buying arm jumps to 0x005ADFED (not special) or to 0x005AE279 (special: skips
the stock block; 0x005AE279 then re-tests `buying` and IsWeapon). The source nesting that gives this is likely:
```
if (buying) { if (!isSpecialShop) { owns } } else { profit }
if (!isSpecialShop) { stock; delivery }
if (buying && IsWeapon(i)) { ... }
```
with the !buying + special path threaded to 0x005AE5D6 because `buying` is known false there (jump threading of the weapon test).
Each "line" pattern (Reset/Get/text=/AddChild/SetValue) is identical to 0x005C90FC's.

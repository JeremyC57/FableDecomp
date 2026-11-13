# OUTLINE 0x005AAF28 CTCInventoryTrade::ConstructTradeDescriptionClothing(long itemDefIndex, unsigned long count, bool buying, bool special)

## Frame facts
- `push ebp; mov ebp,esp; sub esp,74h; push ebx; push esi; mov ebx,ecx; push edi` -- saved order ebx, esi, edi; frame 0x74;
  epilogue `pop edi; pop esi; pop ebx; leave; ret 10h`. No EH states, no /GS. Pragmas: optimize("y",off) + optimize("s",on).
- edi = 0 from 0x5AAF3B until 0x5AB0F2 (branch A) / until 0x5ABD1D (branch B): the zero used for the null stores.

## Frame map (named locals first; temporaries share words, listed as seen)
| slot | local | type | ctor / dtor |
|---|---|---|---|
| -0x04 | text | CWideString | ctor 0x99AED0 @0x5AAFD0, dtor @0x5AC0D7 (after the def releases) |
| -0x08 | mgr | NUISystem::CManager* | = GetInstance() @0x5AAF36 |
| -0x0C | (this spill) | CTCInventoryTrade* | |
| -0x10 | container | NUISystem::CComponent* | return value |
| -0x14 | bank | const NGameText::CDataBank& | `[[0x13B86A0]+0x14]` @0x5AB0C9 |
| -0x18 | price | long | Sell/BuyPrice; later temps |
| -0x1C | shop | CTCShop* (=0) | GetBuildingThing()->GetTC(&shop); later GFInt temp (A) / experience (CTCHeroExperience*) in heavy block |
| -0x20 | defaultPrice | long | then (branch B) inventory `const NInventory::CTCInventoryBase*` (=0) |
| -0x24 | miscDef | CDefPointer<const NUISystem::CUIMiscThingsDef> (=0 @0x5AAFDF) | value lives in esi from 0x5AB0E9/0x5AB396; slot reused for line pointers / temps; release at 0x5AC0C4 via esi |
| -0x28 | stockDef | CDefPointer<const CStockItemDef> (=0 @0x5AB0A1) | release @0x5AC097 (null-tested `dec [ecx+4]`) |
| -0x2C | def | CDefPointer<const CDefClassBase> = GetDefinitionManager().GetPDefFromGlobalIndex(itemDefIndex) | release @0x5AC0A8 (`add eax,4; dec [eax]; cmp [eax],0` + `and [ebp-2Ch],0`, same as 0x5C991F) |
| -0x30 | CCharString temp (container name) | | |
| -0x34..-0x64 | CWideString / CCharString temps | | |
| -0x68 | map key temp (loop) | unsigned long | |
| -0x6C | combatDef spill | CDefPointer<const CHeroCombatDef> | inc/inc/dec copy @0x5ABDA7, release @0x5ABFDA |
| -0x74/-0x70 | textComponent | CCountedPointer<NUISystem::CComponent> (Data,Info = 0 @0x5AAF49) | ReleaseData 0x4291DE @0x5AC0DF (last) |
| +0x10 | (buying, then temps: key, inventory in A, weaponDef in heavy block) | | |
| +0x14 | (special, then temps: category in B, thingDef in heavy block, stock/days) ; byte +0x17 = `bool specialShop` | | |
Declaration order implied by the dtor run (reverse): textComponent, text, miscDef, def, stockDef.

## Pseudo-C++ in retail order
```cpp
NUISystem::CComponent* CTCInventoryTrade::ConstructTradeDescriptionClothing(long itemDefIndex, unsigned long count, bool buying, bool special)
{
    NUISystem::CManager* mgr = NUISystem::CManager::GetInstance();                    // 0x5AAF36
    NUISystem::CComponent* container;
    CCountedPointer<NUISystem::CComponent> textComponent;
    if (InventoryTradeOpen) {                                                         // 0x5AAF3D (+0x218), je 0x5AAF80
        container = mgr->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT", false);   // 0x5AAF67
        textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));   // push 0x12537D8, jmp 0x5AAFA8
    } else {                                                                           // 0x5AAF80
        container = mgr->CreateComponent("UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE", false);  // 0x5AAF91
        textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    }   // arms cross-jumped at 0x5AAFA8 (shared CCharString ctor [ebp-28h], CreateComponent, Reset 0x429C6F @0x5AAFC0, ~CCharString)
    CWideString text;                                                                  // 0x5AAFD0
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;                            // 0x5AAFDF
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);   // 0x5AAFED/0x5AAFF4
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);                         // 0x5AB008 / 0x5AB00F (0x42B312)
    NUISystem::CText* line = static_cast<NUISystem::CText*>(textComponent.Get());      // 0x5AB017 `mov ebx,[ebp-74h]` (read BEFORE GetBuildingThing)
    CTCShop* shop = 0;                                                                 // 0x5AB01A
    GetBuildingThing()->GetTC(&shop);                                                  // 0x5AB01D; expanded GetTC 0x53: test 0x5AB029, LowerBound 0x40F020 @0x5AB042
    long price;
    if (buying && !special)                                                            // 0x5AB05B / 0x5AB061
        price = shop->GetCurrentSellPrice(itemDefIndex, 1);                            // 0x5AB06F, jmp 0x5AB083
    else
        price = shop->GetCurrentBuyPrice(itemDefIndex, 1);                             // 0x5AB07E
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(itemDefIndex);   // 0x5AB090/0x5AB097 (args pushed before GetDefinitionManager)
    CDefPointer<const CStockItemDef> stockDef;                                          // 0x5AB0A1
    long defaultPrice;
    if (!def.IsNull() && def->GetSubDef(&stockDef))                                    // 0x5AB09F / 0x5AB0AA (0x59350C)
        defaultPrice = stockDef->DefaultPrice;                                         // 0x5AB0B3 (+0x28)
    else
        defaultPrice = 1;                                                              // 0x5AB0BE
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();       // 0x5AB0C9 (`cmp special` is scheduled first)
    if (special) {                                                                     // je 0x5AB393
        // ---- branch A 0x5AB0DA ----
        text = bank.GetTextBySymbol("TEXT_GUI_PRICE") + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);
            // CCharString temp [ebp+10h] @0x5AB0E4, GFInt @0x5AB0F5, GetText @0x5AB10B, + x3 @0x5AB115/11F/129, operator= @0x5AB132, dtors to 0x5AB162
        text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(defaultPrice) + miscDef->CloseBracket;
            // GFInt @0x5AB171, + @0x5AB180/18A/194, += 0x99B8D0 @0x5AB19D
        container->AddChild(textComponent);                                            // 0x5AB1CB (+0xEC)
        line->SetValue(text, true, true);                                              // 0x5AB1DD (+0x240, ecx=ebx)
        if (price < defaultPrice) {                                                    // 0x5AB1E6 jge 0x5AB246
            textComponent->GetStates()[0].Colour.B *= 0.5f;                            // 0x5AB1F8 / 0x42D5B1 / ftol 0x5AB217 (key `and [ebp+10h],0` re-stored before the byte store)
            textComponent->GetStates()[0].Colour.G *= 0.5f;                            // 0x5AB22B ..., jmp 0x5AB2A7 (shared fmul/ftol/store tail)
        } else if (price > defaultPrice) {                                             // 0x5AB246 jle 0x5AB2B4
            textComponent->GetStates()[0].Colour.B *= 0.5f;                            // 0x5AB25B
            textComponent->GetStates()[0].Colour.R *= 0.5f;                            // 0x5AB28E (+0x1A), tail 0x5AB2A7
        }
        if (InventoryTradeOpen) textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));   // 0x5AB2B4 (this reloaded from [ebp-0Ch], `push 0` here)
        else                    textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));   // cross-jumped at 0x5AB2D1
        NUISystem::CText* descriptionLine = static_cast<NUISystem::CText*>(textComponent.Get());   // 0x5AB2F5 -> [ebp-24h]
        const CThingPlayerCreature* player = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().PeekPControlledCreature();   // 0x5AB303 / 0x5AB30A
        const NInventory::CTCInventoryBase* inventory = 0;                             // 0x5AB312 [ebp+10h]
        player->PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory);  // 0x5AB318; test 0x5AB323; LowerBound const 0x4365B0 @0x5AB338; type kept in ebx
        text = L"";                                                                    // 0x5AB35A (0x99B800, 0x0122D70C)
        inventory->GetItemDescription(text, itemDefIndex, true);                       // 0x5AB36C vslot 78 (+0x138)
        container->AddChild(textComponent);                                            // 0x5AB37B
        descriptionLine->SetValue(text, true, true);                                   // jmp 0x5AC091 (shared call [eax+240h] + epilogue)
    } else {
        // ---- branch B 0x5AB393 ----
        text = bank.GetTextBySymbol(miscDef->TradeCostString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);
            // GFInt [ebp-40h] @0x5AB39C, ToCharString(+0x80) -> [ebp-24h] @0x5AB3B4, GetText @0x5AB3C1, + @0x5AB3CB/3D5/3DF, = @0x5AB3E8
        text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(defaultPrice) + miscDef->CloseBracket;   // 0x5AB41D..0x5AB454
        container->AddChild(textComponent);                                            // 0x5AB482
        line->SetValue(text, true, true);                                              // 0x5AB494
        if ((price > defaultPrice && buying) || (price < defaultPrice && !buying)) {    // 0x5AB49A..0x5AB4B4 (see branch map)
            textComponent->GetStates()[0].Colour.B *= 0.5f;                            // 0x5AB4B6 (key `mov [ebp+14h],edi`)
            textComponent->GetStates()[0].Colour.G *= 0.5f;                            // 0x5AB4E8, jmp 0x5AB56E
        } else if ((price < defaultPrice && buying) || (price > defaultPrice && !buying)) {   // 0x5AB50F reuses flags of 0x5AB4AB
            textComponent->GetStates()[0].Colour.B *= 0.5f;                            // 0x5AB517
            textComponent->GetStates()[0].Colour.R *= 0.5f;                            // 0x5AB549, tail 0x5AB56E
        }
        const CThingPlayerCreature* player = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().PeekPControlledCreature();   // 0x5AB57B (ebx)
        const NInventory::CTCInventoryBase* inventory = 0;                             // 0x5AB594 [ebp-20h]
        ETCInterfaceType inventoryType = CTCInventoryItem::GetInventoryType(itemDefIndex);   // 0x5AB597 -> [ebp+14h] (kept in memory here)
        player->PeekTC(inventoryType, &inventory);                                     // 0x5AB5A3 .. 0x5AB5D3 (key copy [ebp-40h])
        bool specialShop = shop->IsBarberShop() || shop->IsTattooShop() || shop->IsTitleShop();   // 0x5AB5D6..0x5AB5FE -> byte [ebp+17h]
        if (!buying) {                                                                 // 0x5AB602 jne 0x5AB860
            if (InventoryTradeOpen) Reset(PC) else Reset(UI);                          // 0x5AB60C, temp [ebp-24h]
            NUISystem::CText* profitLine = static_cast<NUISystem::CText*>(textComponent.Get());   // 0x5AB64C -> [ebp-24h]
            long profit = price - inventory->GetBoughtPrice(itemDefIndex);             // 0x5AB658 (0x5BEAD5); ebx
            if (profit >= 0) {                                                         // js 0x5AB754
                text = bank.GetTextBySymbol(miscDef->TradeProfitString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(profit);   // 0x5AB66C..0x5AB6BB (ToCharString +0x84 -> [ebp-18h])
                if (profit != 0) {                                                     // 0x5AB6F0 je 0x5AB83E
                    textComponent->GetStates()[0].Colour.B *= 0.5f;                    // 0x5AB6F8 (key [ebp-44h])
                    textComponent->GetStates()[0].Colour.R *= 0.5f;                    // 0x5AB72A, jmp 0x5AB831
                }
            } else {                                                                   // 0x5AB754
                text = bank.GetTextBySymbol(miscDef->TradeLossString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(-profit);   // `neg edx`, GFInt [ebp-18h], ToCharString +0x88 -> [ebp-34h]
                textComponent->GetStates()[0].Colour.B *= 0.5f;                        // 0x5AB7DA
                textComponent->GetStates()[0].Colour.G *= 0.5f;                        // 0x5AB80C, falls into 0x5AB831
            }
            container->AddChild(textComponent);                                        // 0x5AB83E
            profitLine->SetValue(text, true, true);                                    // 0x5AB85A
        }
        if (inventory->GetInterfaceType() == TCI_INVENTORY_CLOTHING) {                 // 0x5AB860 vslot 21, cmp 12h
            if (InventoryTradeOpen) Reset(PC) else Reset(UI);                          // 0x5AB871
            NUISystem::CText* armourLine = static_cast<NUISystem::CText*>(textComponent.Get());   // 0x5AB8B1 ebx
            text = CWideString::NullString;                                            // 0x5AB8BC
            static_cast<const CTCInventoryClothing*>(inventory)->GetItemDescriptionArmourRating(text, itemDefIndex);   // 0x5AB8CB (0x5B6767)
            if (text != CWideString::NullString) {                                     // 0x5AB8D8 (0x99B0F0)
                container->AddChild(textComponent);                                    // 0x5AB8EA
                armourLine->SetValue(text, true, true);                                // 0x5AB8FC
            }
        }
        if (buying && !specialShop) {                                                  // 0x5AB902 / 0x5AB90C (jne straight to 0x5ABCC9: threaded)
            if (InventoryTradeOpen) Reset(PC) else Reset(UI);                          // 0x5AB916
            NUISystem::CText* ownedLine = static_cast<NUISystem::CText*>(textComponent.Get());   // 0x5AB95C ebx (load between push and call)
            long owned = inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex);    // 0x5AB95F
            text = bank.GetTextBySymbol(miscDef->TradeAlreadyOwnsString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(owned);   // 0x5AB969..0x5AB9B5
            container->AddChild(textComponent);                                        // 0x5AB9F3
            ownedLine->SetValue(text, true, true);                                     // 0x5ABA05
        }
        if (!specialShop) {                                                            // 0x5ABA0B jne 0x5ABCC9
            if (InventoryTradeOpen) Reset(PC) else Reset(UI);                          // 0x5ABA15 (temp [ebp+14h])
            NUISystem::CText* stockLine = static_cast<NUISystem::CText*>(textComponent.Get());   // 0x5ABA5B ebx
            long quantity = shop->GetQuantity(itemDefIndex);                           // 0x5ABA5E (0x7726B0) -> [ebp+14h]
            long maxStock = shop->GetMaxStock(itemDefIndex);                           // 0x5ABA6C -> [ebp-24h]
            text = bank.GetTextBySymbol(miscDef->TradeNumberInStockString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(quantity);   // 0x5ABA7A..0x5ABAC6
            text += L"/" + GFIntToWideString(maxStock);                                // 0x5ABB01 GFInt, 0x5ABB0F op+(wchar_t*,..) 0x99BFF0, += 0x5ABB18
            container->AddChild(textComponent);                                        // 0x5ABB36
            stockLine->SetValue(text, true, true);                                     // 0x5ABB48
            if (InventoryTradeOpen) Reset(PC) else Reset(UI);                          // 0x5ABB4E
            NUISystem::CText* deliveryLine = static_cast<NUISystem::CText*>(textComponent.Get());   // 0x5ABB94 ebx
            long days = shop->GetDaysTillRestock(itemDefIndex);                        // 0x5ABB97 -> [ebp+14h]
            text = bank.GetTextBySymbol(miscDef->TradeDeliveryString.ToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator;   // 0x5ABB9F..0x5ABBDB
            if (days != 0)                                                             // 0x5ABC00 je 0x5ABC73
                text += GFIntToWideString(days) + miscDef->SpaceSeparator + bank.GetTextBySymbol(miscDef->TradeDaysString.ToCharString());   // 0x5ABC05..0x5ABC49 (ToCharString +0x9C first)
            else
                text += bank.GetTextBySymbol(miscDef->TradeNoDeliveryString.ToCharString());   // 0x5ABC73 (+0x98); both arms share ~CCharString @0x5ABCA3
            container->AddChild(textComponent);                                        // 0x5ABCB1
            deliveryLine->SetValue(text, true, true);                                  // 0x5ABCC3
        }
        if (buying && CTCInventoryItem::IsWeapon(itemDefIndex)) {                      // 0x5ABCC9 / 0x5ABCD6
            CDefPointer<const CThingBaseDef /*T unproven*/> thingDef;                  // [ebp+14h]=0 @0x5ABCE6
            GetDefinitionManager().GetDef(itemDefIndex, &thingDef);                    // 0x5ABCF1 / 0x5ABCF8 (0x42AF3C)
            CDefPointer<const CWeaponDef> weaponDef;                                   // [ebp+10h]=0 @0x5ABD00
            thingDef->GetSubDef(&weaponDef);                                           // 0x5ABD08 (0x42B1A6), no null test
            if (weaponDef->Class == 2) {                                               // 0x5ABD10 jne 0x5ABFE6
                CTCHeroExperience* experience;                                         // [ebp-1Ch]
                CTCInventoryExperience* experienceInventory;                           // [ebp-44h]
                if (ParentThing->GetTC(&experience) && ParentThing->GetTC(&experienceInventory)) {   // 0x5ABD1A (push 68h;pop ebx) / 0x5ABD58 (push 16h;pop ebx), LowerBound 0x40F020
                    CDefPointer<const CHeroCombatDef> combatDef = experience->GetPHeroCombatDef();   // 0x5ABD96 edi/[ebp-6Ch]; test before the inc/inc/dec @0x5ABDA7
                    if (!combatDef.IsNull()) {
                        for (std::map<EHeroTrainableStatType,long>::const_iterator it = combatDef->HeavyWeapons.StatThresholds.begin();
                             it != combatDef->HeavyWeapons.StatThresholds.end(); ++it) {     // begin 0x5ABDB9, ++ 0x5ABFC6 (0x4292C0), end re-read 0x5ABFCD
                            if (experience->GetTrainableStatLevel(it->first) < it->second) {   // 0x5ABDD0 / cmp [ebx+14h] jge 0x5ABFC5
                                if (InventoryTradeOpen) Reset(PC) /*temp [ebp-18h]*/ else Reset(UI) /*temp [ebp-34h]*/;   // 0x5ABDDE: arms NOT sharing the CCharString temp; merge at ~CCharString 0x5ABE3C
                                NUISystem::CText* requiredLine = static_cast<NUISystem::CText*>(textComponent.Get());   // 0x5ABE41 -> [ebp-24h]
                                text = experienceInventory->GetExperienceStatName(it->first) + miscDef->SpaceSeparator
                                     + bank.GetTextBySymbol("TEXT_GUI_MENU_EXP_LEVEL") + miscDef->SpaceSeparator
                                     + GFIntToWideString(it->second) + bank.GetTextBySymbol("TEXT_GUI_MENU_REQUIRED");   // 0x5ABE44..0x5ABEDD
                                textComponent->GetStates()[0].Colour.B *= 0.5f;                // 0x5ABF3A (key [ebp-68h])
                                textComponent->GetStates()[0].Colour.G *= 0.5f;                // 0x5ABF71
                                container->AddChild(textComponent);                            // 0x5ABFA9 (colour BEFORE AddChild here)
                                requiredLine->SetValue(text, true, true);                      // 0x5ABFBC
                            }
                        }
                    }   // combatDef release 0x5ABFDA
                }
            }   // ~weaponDef 0x5ABFE6 (no null test, as 0x5C9913), ~thingDef 0x5ABFF3 (null-tested)
        }
        if (inventory->GetInterfaceType() == TCI_INVENTORY_CLOTHING) {                 // 0x5AC004 (ebx=inventory), jne 0x5AC097
            if (InventoryTradeOpen) Reset(PC) else Reset(UI);                          // 0x5AC017 (temp [ebp+10h], `push 0`)
            NUISystem::CText* noArmourLine = static_cast<NUISystem::CText*>(textComponent.Get());   // 0x5AC058 edi
            text = CWideString::NullString;                                            // 0x5AC063
            static_cast<const CTCInventoryClothing*>(inventory)->GetItemDescriptionNoArmourRating(text, itemDefIndex);   // 0x5AC071 (0x5B62FE)
            container->AddChild(textComponent);                                        // 0x5AC07F
            noArmourLine->SetValue(text, true, true);                                  // 0x5AC091 (branch A joins here)
        }
    }
    return container;                                                                  // 0x5AC097: ~stockDef, ~def (+`and`), ~miscDef (esi), ~text, textComponent.ReleaseData; mov eax,[ebp-10h]
}
```
Reset(PC)/Reset(UI) = `textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE"/"UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false))`
written as an if/else over `InventoryTradeOpen` with one CreateComponent per arm (retail `cmp [this+218h]; push -1; je; push PC; jmp; push UI; call ctor`,
as 0x5C90FC with IsLiveGUIMode). `mgr` is the named local [ebp-8] (GetInstance is called once).

## Branch map (B colour test, 0x5AB49A)
`cmp price,def; jle L4A8; cmp buying,0; jne BG(0x5AB4B6)` / `L4A8: cmp price,def; jge L50F; cmp buying,0; jne BR(0x5AB517); [fall] BG` /
`L50F: jle SKIP(0x5AB57B); cmp buying,0; jne SKIP; [fall] BR`.

## Cold blocks / jump tables
None: no switch, no out-of-line cold blocks; everything sits before the epilogue at 0x5AC0E4.

## Open questions for the author
- The heavy-weapon loop's Reset arms use distinct CCharString temps ([ebp-18h] / [ebp-34h]) unlike every other Reset pair: may need a
  different spelling (e.g. separate statements) or will follow from slot pressure; check against 0x5C90FC loop body first.
- Line-pointer locals must be read where retail reads them (before the next opaque call), not at their use.
- Overlay needs: CTCInventoryTrade ConstructTradeDescription* declarations; CTCShop GetQuantity / GetMaxStock / GetDaysTillRestock
  (QBEJJ@Z) declarations; GetBoughtPrice / GetNumberOfItemsOfTypeInInventory macros in NInventory_CTCInventoryBase.h.

# OUTLINE 0x005A9CBE CTCInventoryTrade::ConstructTradeDescriptionWeapon(long defIndex, unsigned long /*unused*/, bool a, bool b)

## Frame
- `push ebp ; mov ebp,esp ; sub esp,7Ch ; push ebx ; push esi ; mov ebx,ecx ; push edi`; `pop edi ; pop esi ; pop ebx ; leave ; ret 10h`.
- No EH, no cookie. this in ebx, saved to [ebp-0Ch] (all later `this` reads go through [ebp-0Ch]).

## Frame map (ebp-relative)
| slot | holds |
|---|---|
| -04 | CWideString text (default ctor 0x005A9D66; dtor 0x005AAF11) |
| -08 | NUISystem::CManager* mgr = CManager::GetInstance() (0x005A9CCC; edi then spilled) |
| -0C | this |
| -10 | CComponent* description (return value; vslot +0xEC AddChild target) |
| -14 | const NGameText::CDataBank& bank (CMainGameComponent::Get() +0x14, 0x005A9E5F) |
| -18 | long price (0x005A9E1F); later CCharString/CWideString temps; in the weapon block CTCHeroExperience* exp |
| -1C | CTCShop* shop (=0 at 0x005A9DB0); in the weapon block CDefPointer<const CThingBaseDef> thingDef (=0 at 0x005AAB0A) |
| -20 | long defaultPrice (0x005A9E4F / =1 at 0x005A9E54); later CText* line pointers / CCharString temps / maxStock |
| -24 | CDefPointer<const CUIMiscThingsDef> miscDef (=0 at 0x005A9D75; ENREGISTERED in edi after GetDef); later IntToWide temp; then const NInventory::CTCInventoryBase* inventory (=0 at 0x005AA32A) |
| -28 | CCharString temp ("UI_MISC_THINGS_DEF"/template); then CDefPointer<const CStockItemDef> stockDef (=0 at 0x005A9E37; dtor 0x005AAED1) |
| -2C | CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(defIndex) (hidden result; temp-form dtor 0x005AAEE2) |
| -30 | CCharString temp (first CreateComponent) |
| -34..-44 | CWideString / CCharString expression temps; -40 also LowerBound key (0x53); -44 also CTCInventoryExperience* invExp |
| -48..-6C | the 10 wide temps of the "required" line (0x005AAC90..0x005AAD5C) |
| -70 | map key / int->float temp of the loop fade |
| -74 | CDefPointer<const CHeroCombatDef> combatDef (esi) |
| -7C/-78 | CCountedPointer<NUISystem::CComponent> textComponent (2 dwords zeroed 0x005A9CDF/E2; ReleaseData 0x005AAF19) |
| +10 / +14 | dead arg slots reused for CWideString / CCharString temps and map keys; +14 also `inventory` (first) and weaponDef (CDefPointer<const CWeaponDef>, =0 at 0x005AAB24); +17 bool special |

## Body (retail order)
```
CComponent* CTCInventoryTrade::ConstructTradeDescriptionWeapon(long defIndex, unsigned long, bool a, bool b)
{
  NUISystem::CManager* mgr = NUISystem::CManager::GetInstance();                 // 0x005A9CCC
  CCountedPointer<CComponent> textComponent;                                     // [ebp-7C] zeroed
  CComponent* description;
  if (InventoryTradeOpen) {                                                      // 0x005A9CD3 (push -1 / lea ecx hoisted above je)
     description = mgr->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT", false);   // 0x005A9CFD
     textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
  } else {                                                                        // 0x005A9D16
     description = mgr->CreateComponent("UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE", false);                    // 0x005A9D27
     textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
  }                                   // arms cross-jump at 0x005A9D3E (one CCharString ctor / CreateComponent 0x005A9D4D / Reset 0x005A9D56)
  CWideString text;                                                              // 0x005A9D66
  CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;                        // 0x005A9D75
  long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", true);  // 0x005A9D83/8A
  CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);                     // 0x005A9D9E / 0x005A9DA5 (0x0042B312)
  CText* priceLine = (CText*)textComponent.Get();   // `mov ebx,[ebp-7Ch]` 0x005A9DAD (cached for both arms' SetValue)
  CTCShop* shop = 0;                                                             // 0x005A9DB0
  GetBuildingThing()->GetTC(TCI_SHOP, &shop);                                    // 0x005A9DB3 + expanded GetTC 0x005A9DBF..DEE (key [ebp-40h])
  long price;
  if (a && !b) price = shop->GetCurrentSellPrice(defIndex, 1);                   // 0x005A9DF1..E05
  else         price = shop->GetCurrentBuyPrice(defIndex, 1);                    // 0x005A9E0C..14 (tail shared at 0x005A9E19)
  CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(defIndex);   // 0x005A9E26/2D
  CDefPointer<const CStockItemDef> stockDef;                                     // 0x005A9E37 (between the null test's cmp and je)
  long defaultPrice;
  if (!def.IsNull() && def->GetSubDef(&stockDef)) defaultPrice = stockDef->DefaultPrice;   // 0x005A9E35..4F
  else defaultPrice = 1;                                                         // 0x005A9E54
  const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();   // 0x005A9E5F (interleaved with the b test)
  if (b) {                                                                       // je 0x005AA129 (false arm placed second)
    // 0x005A9E70  (misc in edi, &SpaceSeparator in esi)
    text  = bank.GetTextBySymbol("TEXT_GUI_PRICE") + miscDef->ColonSeparator + miscDef->SpaceSeparator + GFIntToWideString(price);
          // CCharString temp [ebp+14h] built FIRST (0x005A9E7A), IntToWide [ebp-34h], GT [ebp-38h], op+ [ebp-3Ch],[ebp-40h],[ebp+10h], op= 0x005A9EC8
    text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(defaultPrice) + miscDef->CloseBracket;   // 0x005A9EFD..F33
    description->AddChild(textComponent);                                       // 0x005A9F61 (+0xEC)
    priceLine->SetValue(text, true, true);                                       // 0x005A9F73 (+0x240, ecx = ebx)
    if (price < defaultPrice) {                                                  // 0x005A9F7F jge 0x005A9FDC
      textComponent->GetStates()[0].Colour.B *= 0.5f;                            // 0x005A9F81..B6 key `and [ebp+14h],0`
      textComponent->GetStates()[0].Colour.G *= 0.5f;                            // 0x005A9FB8..DA -> jmp 0x005AA03D (shared fmul/ftol/store)
    } else if (price > defaultPrice) {                                           // 0x005A9FDC jle 0x005AA04A
      textComponent->GetStates()[0].Colour.B *= 0.5f;                            // 0x005A9FE4
      textComponent->GetStates()[0].Colour.R *= 0.5f;                            // 0x005AA01B (+0x1A)
    }
    // 0x005AA04A
    if (InventoryTradeOpen) textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else                    textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
                                       // cross-jumped `je ; push PC ; jmp ; push UI ; call` (0x005AA059..7E), temp [ebp+14h]
    CText* descLine = (CText*)textComponent.Get();                               // [ebp-20h] 0x005AA08E
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();  // 0x005AA091..A0
    const NInventory::CTCInventoryBase* inventory = 0;                           // `and [ebp+14h],0` 0x005AA0A8
    hero->PeekTC(CTCInventoryItem::GetInventoryType(defIndex), &inventory);      // 0x005AA0AE (fastcall) + test 0x005AA0B9 + const LowerBound 0x005AA0CE
    text = L"";                                                                  // 0x005AA0F0 (operator=(const wchar_t*), 0x0122D70C)
    inventory->GetItemDescription(text, defIndex, true);                         // 0x005AA102 (+0x138)
    description->AddChild(textComponent);                                       // 0x005AA111
    descLine->SetValue(text, true, true);                                        // jmp 0x005AAECB (shares the last block's call)
    return description;                                                          // -> dtor tail 0x005AAED1
  }
  // 0x005AA129 (b == false)
  text  = bank.GetTextBySymbol(miscDef->TradeCostString.ConvertToCharString()) + miscDef->ColonSeparator + miscDef->SpaceSeparator
          + GFIntToWideString(price);          // IntToWide [ebp-24h] (miscDef's slot), CCharString [ebp-34h], GT [ebp-38h], op+ -3C,-40,+14
  text += miscDef->SpaceSeparator + miscDef->OpenBracket + GFIntToWideString(defaultPrice) + miscDef->CloseBracket;  // 0x005AA1B3
  description->AddChild(textComponent);                                         // 0x005AA218
  priceLine->SetValue(text, true, true);                                         // 0x005AA22A (ebx)
  // 0x005AA230: key store `mov [ebp+14h],esi` (esi = 0 live)
  if ((price > defaultPrice && a) || (price < defaultPrice && !a)) {             // 0x005AA233 jle / 0x005AA238 jne->24C ; 0x005AA241 jge / 0x005AA246 jne->2AD
    B *= 0.5f; G *= 0.5f;                                                        // 0x005AA24C..A3 (red), jmp 0x005AA304
  } else if ((price < defaultPrice && a) || (price > defaultPrice && !a)) {      // 0x005AA2A5 `jle 0x005AA311 ; cmp a ; jne 0x005AA311`
    B *= 0.5f; R *= 0.5f;                                                        // 0x005AA2AD..30F (green)
  }   // check the exact condition spelling with tools/bt.sh: the two compares are re-done (no CSE of the flags except 0x005AA2A5)
  // 0x005AA311
  const CThingPlayerCreature* hero = ...GetMainPlayer().PeekPControlledCreature();   // -> ebx
  const NInventory::CTCInventoryBase* inventory = 0;                             // [ebp-24h] 0x005AA32A
  hero->PeekTC(CTCInventoryItem::GetInventoryType(defIndex), &inventory);       // type saved [ebp+14h], key copy [ebp-40h] 0x005AA342
  bool special = shop->IsBarberShop() || shop->IsTattooShop() || shop->IsTitleShop();   // 0x005AA36C..98 -> byte [ebp+17h]
  if (!a) {                                                                      // 0x005AA398 jne 0x005AA5F6
    (PC/UI if-else) textComponent.Reset(...);                                    // 0x005AA3A2 (temp [ebp-20h])
    CText* line = textComponent.Get();                                           // [ebp-20h] 0x005AA3EB
    long profit = price - inventory->GetBoughtPrice(defIndex);                   // 0x005AA3EE (0x005BEAD5); ebx
    if (profit >= 0) {                                                           // js 0x005AA4EA
      text = bank.GetTextBySymbol(miscDef->TradeProfitString.ConvertToCharString()) + Colon + Space + GFIntToWideString(profit);  // 0x005AA402
      if (profit != 0) { B *= 0.5f; R *= 0.5f; }                                 // 0x005AA486 je 0x005AA5D4; keys [ebp-44h]; jmp 0x005AA5C7
    } else {
      text = bank.GetTextBySymbol(miscDef->TradeLossString.ConvertToCharString()) + Colon + Space + GFIntToWideString(-profit);  // 0x005AA4EA (`neg edx`)
      B *= 0.5f; G *= 0.5f;                                                      // 0x005AA570..5D2
    }
    description->AddChild(textComponent); line->SetValue(text, true, true);      // 0x005AA5D4..F0
  }
  // 0x005AA5F6   (ebx = &CWideString::NullString hoisted above the jne, 0x005AA601)
  if (inventory->GetInterfaceType() == TCI_INVENTORY_WEAPONS) {                  // +0x54
    (PC/UI) Reset; line = Get() [ebp-20h]; text = CWideString::NullString;       // 0x005AA60C..56
    ((const CTCInventoryWeapons*)inventory)->GetItemDescriptionDamage(text, defIndex);   // 0x005AA665
    if (text != CWideString::NullString) { description->AddChild(textComponent); line->SetValue(text, true, true); }  // 0x005AA66E..99
    (PC/UI) Reset; line = Get(); text = CWideString::NullString;                 // 0x005AA699
    ((const CTCInventoryWeapons*)inventory)->GetItemDescriptionWeight(text, defIndex);   // 0x005AA6F2
    if (text != CWideString::NullString) { AddChild; SetValue; }                 // 0x005AA6FB..726
  }
  // 0x005AA726   (b-test jumps straight to 0x005AAAED when a && special: jump threading)
  if (a && !special) {                                                           // 0x005AA726 / 0x005AA730
    (PC/UI) Reset (temp [ebp-20h]); CText* line = Get() (ebx, 0x005AA780);
    text = bank.GetTextBySymbol(miscDef->TradeAlreadyOwnsString.ConvertToCharString()) + Colon + Space
           + GFIntToWideString(inventory->GetNumberOfItemsOfTypeInInventory(defIndex));   // 0x005AA783 (count fetched before IntToWide)
    AddChild; line->SetValue(text, true, true);                                  // 0x005AA811..29
  }
  if (!special) {                                                                // 0x005AA82F
    (PC/UI) Reset (temp [ebp+14h]); line = Get() (ebx);
    long stock = shop->GetStockQuantity(defIndex);                               // 0x005AA882 -> [ebp+14h]
    long maxStock = shop->GetMaxStock(defIndex);                                 // 0x005AA890 -> [ebp-20h]
    text  = bank.GetTextBySymbol(miscDef->TradeNumberInStockString.ConvertToCharString()) + Colon + Space + GFIntToWideString(stock);  // 0x005AA898
    text += L"/" + GFIntToWideString(maxStock);                                  // 0x005AA91F (op+ 0x0099BFF0, edx = L"/")
    AddChild; line->SetValue;                                                    // 0x005AA951..6C
    (PC/UI) Reset (temp [ebp+14h]); line = Get() (ebx);                          // 0x005AA972
    long days = shop->GetDaysTillRestock(defIndex);                              // 0x005AA9BB -> [ebp+14h]
    text = bank.GetTextBySymbol(miscDef->TradeDeliveryString.ConvertToCharString()) + Colon + Space;   // 0x005AA9C3..A1F (no number)
    if (days != 0)                                                               // 0x005AAA24 je 0x005AAA97
      text += GFIntToWideString(days) + miscDef->SpaceSeparator + bank.GetTextBySymbol(miscDef->TradeDaysString.ConvertToCharString());  // 0x005AAA29
    else
      text += bank.GetTextBySymbol(miscDef->TradeNoDeliveryString.ConvertToCharString());   // 0x005AAA97; shared ~CCharString 0x005AAAC7
    AddChild; line->SetValue;                                                    // 0x005AAACC..E7
  }
  // 0x005AAAED
  if (a && CTCInventoryItem::IsWeapon(defIndex)) {                               // 0x005AAAED / 0x005AAAFA
    CDefPointer<const CThingBaseDef> thingDef;                                   // [ebp-1Ch] (shop's slot) `and [ebp-1Ch],0`
    GetDefinitionManager().GetDef(defIndex, &thingDef);                          // 0x005AAB15 / 0x005AAB1C
    CDefPointer<const CWeaponDef> weaponDef;                                     // [ebp+14h]
    thingDef->GetSubDef(&weaponDef);                                             // 0x005AAB2C (no null test)
    if (weaponDef->Class == 2) {                                                 // 0x005AAB34 jne 0x005AAE1D
      CTCHeroExperience* exp; CTCInventoryExperience* invExp;
      if (ParentThing->GetTC(TCI_HERO_EXPERIENCE, &exp)                         // 0x005AAB3E `push 68h ; pop ebx`, non-const LowerBound; -> [ebp-18h]
          && ParentThing->GetTC(TCI_INVENTORY_EXPERIENCE, &invExp)) {            // 0x005AAB7C `push 16h ; pop ebx`; -> [ebp-44h]
        CDefPointer<const CHeroCombatDef> combatDef = exp->GetPHeroCombatDef();  // 0x005AABBA (test esi before `inc;inc;dec`) [ebp-74h]
        if (!combatDef.IsNull()) {                                               // je 0x005AAE1D
          for (std::map<EHeroTrainableStatType,long>::const_iterator it = combatDef->HeavyWeapons.StatThresholds.begin();
               it != combatDef->HeavyWeapons.StatThresholds.end(); ++it) {      // 0x005AABDD head [esi+13Ch]; ++ 0x005AADFD; test 0x005AAE04
            if (exp->GetTrainableStatLevel(it->first) < it->second) {            // 0x005AABF4 jge 0x005AADFC
              if (InventoryTradeOpen) textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));  // 0x005AAC10 temp [ebp-34h]
              else                    textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));  // 0x005AAC39 temp [ebp-38h]
                                       // NOT cross-jumped here: two calls, merge at ~CCharString 0x005AAC60
              CText* line = textComponent.Get();                                 // [ebp-20h] 0x005AAC72
              text = invExp->GetExperienceStatName(it->first) + miscDef->SpaceSeparator
                     + bank.GetTextBySymbol("TEXT_GUI_MENU_EXP_LEVEL") + miscDef->SpaceSeparator
                     + GFIntToWideString(it->second) + miscDef->SpaceSeparator
                     + bank.GetTextBySymbol("TEXT_GUI_MENU_REQUIRED");             // 0x005AAC68..D0C: CCharStrings REQUIRED [-40h] then EXP_LEVEL [-3Ch]
                     // built first; GT(REQ) [-6Ch], IntToWide [-68h], GT(EXP) [-64h], StatName [-60h], op+ -5C..-48; op= 0x005AAD0C
              textComponent->GetStates()[0].Colour.B *= 0.5f;                    // 0x005AAD71 key [ebp-70h]
              textComponent->GetStates()[0].Colour.G *= 0.5f;                    // 0x005AADA8
              description->AddChild(textComponent);                             // 0x005AADE0
              line->SetValue(text, true, true);                                  // 0x005AADF3
            }
          }
        }                                                                        // combatDef release 0x005AAE11
      }
    }
    // 0x005AAE1D weaponDef release (`dec [ecx+4]` with NO null test), 0x005AAE2A thingDef release (with test)
  }
  // 0x005AAE3B  (ebx = inventory)
  if (inventory->GetInterfaceType() == TCI_INVENTORY_WEAPONS) {                  // +0x54, jne 0x005AAED1
    (PC/UI) Reset (temp [ebp+14h]); CText* line = Get() (esi);                   // 0x005AAE4E..8F
    text = CWideString::NullString;                                              // 0x005AAE9A
    ((const CTCInventoryWeapons*)inventory)->GetItemDescriptionNoDamageNorWeight(text, defIndex, a);   // 0x005AAEAB
    description->AddChild(textComponent);                                       // 0x005AAEB9
    line->SetValue(text, true, true);                                            // 0x005AAECB (also the b-arm's call)
  }
  return description;          // 0x005AAED1: ~stockDef, ~def (temp form + `and [ebp-2Ch],0`), ~miscDef (edi), ~text, textComponent.ReleaseData
}
```
Notes
- Every "Colour.X *= 0.5f" = `movzx eax,byte ; mov [tmp],eax ; fild [tmp] ; fmul [0x0122F59C] ; call __ftol2 ; mov [byte],al`, preceded by
  `GetStates()` (+0x148 on textComponent's data) and `map::operator[](key 0)` 0x0042D5B1. B=+0x18, G=+0x19, R=+0x1A.
- Fade pairs share their second half by cross-jumping (0x005AA03D, 0x005AA304, 0x005AA5C7).
- The `(PC/UI)` creations: `if (InventoryTradeOpen) textComponent.Reset(mgr->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
  else textComponent.Reset(mgr->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));` -- the 0x005C90FC spelling; VC cross-jumps the
  arms (`je ; push PC ; jmp ; push UI ; call`). mgr is re-read from [ebp-08h] each time.
- CText* line locals are read BEFORE AddChild (0x005C90FC lesson); the fades use textComponent (`mov ecx,[ebp-7Ch]`), not the line local.
- Literal-to-CCharString conversion temporaries are constructed at the start of their full-expression (0x005A9E7A, 0x005AAC75/84);
  ConvertToCharString results are not (evaluated in right-to-left argument order).
- No jump tables. No cold blocks after the epilogue. Only loop: the StatThresholds walk 0x005AABEE..0x005AAE0B.

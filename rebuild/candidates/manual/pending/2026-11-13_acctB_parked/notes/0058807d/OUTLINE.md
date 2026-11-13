# OUTLINE 0x0058807D CTCInventory::OpenPCInventoryMenu()  (see $W/scout/ann.txt for the annotated listing)

## Frame
`push ebp ; lea ebp,[esp-78h] ; sub esp,8Ch ; push ebx ; push esi ; push edi` ... `pop edi ; pop esi ; pop ebx ; add ebp,78h ; leave ; ret`.
No EH states, no /GS. Pragmas: optimize("y",off) + optimize("s",on). ebx = list (CComponent*) for most of the body; esi/edi scratch.

## Frame map (ebp-relative; slots are reused by disjoint scopes)
| slot | local (type) |
|---|---|
| -0x14 | `CComponent* list` (address taken by Components.push_back(list)) |
| -0x10,-0x0C,-0x08,+0x00,+0x04,+0x3C | CCharString temporaries of the hero-suit `||/&&` expression (TROUSERS,BOOTS,SHIRT,NO_HAT,GLOVES,NAKED) |
| -0x04 | &Components (this+0x0C) cached |
| +0x08 | CDefPointer<const CTattooDef> tattoo (returned) |
| +0x0C | CDefPointer<const CAbilityDef> ability |
| +0x10 | CDefPointer<const CHairCardDef> hair (returned) |
| +0x14 | CDefPointer<const CDefClassBase> hairDef (GetPDefFromGlobalIndex result) |
| +0x18 | long itemsCount |
| +0x1C | long haircutCount |
| +0x20 | CTCInventoryClothing* clothing (TCI 18) |
| +0x24 | long weaponsCount |
| +0x28 | CDefPointer<const CHeroSuitDef> suit |
| +0x2C | CDefPointer<const CAppearanceModifierDef> appearance |
| +0x30 | CCharString temp for CreateComponent; later CTCInventory* inventory (TCI 17) |
| +0x34 | CTCInventoryWeapons* heroWeapons (PeekTC 19 on hero); in loop CDefPointer<const CAugmentationDef> aug |
| +0x38 / +0x60 | CCountedPointer<CAction> action (clothing+style use 0x38, weapons+items use 0x60) |
| +0x40 | CViewport/CComponent* viewport (frame block); later long facialHairCount |
| +0x44 | CComponent* goldAmount (frame block); later long tattooCount |
| +0x48..+0x50 | std::vector<std::pair<long,long> > items |
| +0x54 | unsigned long i (item loop); later CViewport* viewport2 |
| +0x58 | this |
| +0x5C | unsigned long i (find loop); later long clothingCount |
| +0x64 | CManager* manager; later scratch (CDefPointer<const CThingObjectDef> objDef / appearance mod / button / styleCount / CWideString temp) |
| +0x6B | bool (name== result; later the suit-test bool) |
| +0x6C | CDefPointer<const CThingBaseDef> thingDef (loop); later CWideString text |
| +0x70 | temp-dtor flag word (compiler) |
| +0x74 | GetName CCharString temp; CTCHeroStats* stats + CWideString temp; CTCInventoryWeapons* weapons (TCI 19); CActionParam* temp |

## Body (retail order)
```
0x0058807D  DeleteMenuComponents();                       // 0x00587A41
0x0058809A  CurrentOpenMenu = (CURRENT_OPEN_MENU)3;
            CComponent* list = 0;
0x005880A4  CManager* manager = CManager::GetInstance();  // [ebp+64h]
0x005880B4  for (unsigned long i = 0; i < Components.size(); ++i) {   // rotated: size test first (je 0x588136)
0x005880BF      if (list) break;                            // -> 0x005892F7 (jne)
0x005880C7      if (Components[i]->GetName() == "PC_INVENTORY_LIST")  // slot +0x150, inline CCharString==, temp [ebp+74h]
0x00588116          list = Components[i];
            }
0x0058812E  if (list) goto TAIL (0x005892F7);   // i.e. `if (!list) { ... }` -- whole build block below
0x00588136  list = manager->CreateComponent(CCharString("PC_INVENTORY_LIST"), false);  // explicit temp at [ebp+30h], dtor after
0x00588160  CComponent* frame = list->FindChild("PC_UI_FRAME");                        // esi
0x0058817A  if (frame) {
0x00588180    CComponent* viewport = list->FindChild("PC_DESCRIPTION_VIEWPORT");        // [ebp+40h]
0x0058819B    if (viewport) frame->AddPositionChild(viewport);
0x005881A8    c = FindChild("PC_TITLE_INVENTORY"); if (c) frame->AddPositionChild(c);
0x005881CD    CComponent* gold = FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN_TOP_LEVEL");    // [ebp+44h]
0x005881E8    if (gold) {
0x005881EE      CThingPlayerCreature* hero = CMainGameComponent::Get()->...PlayerManager->GetMainPlayer()->GetPControlledCreature();
0x00588202      CTCHeroStats* stats = 0; hero->GetTC(TCI_HERO_STATS, &stats);            // shape A (0x0040F020, jle)
0x00588241      static_cast<CText*>(gold)->SetValue(GFIntToWideString(stats->Money), true, true);  // +0x240; temp [ebp+74h]
0x00588269      frame->AddPositionChild(gold);
              }
0x00588276    for each of: PC_GOLD_COIN_SPRITE_TOP_LEVEL, PC_RING_PIC_WEAPONS, PC_WEAPON_DESC, PC_WEAPON_SUBTITLE,
              PC_RING_PIC_ITEMS, PC_ITEMS_DESC, PC_ITEMS_SUBTITLE, PC_RING_PIC_CLOTHING, PC_CLOTHING_DESC, PC_CLOTHING_SUBTITLE,
              PC_RING_PIC_STYLE_CARDS, PC_STYLE_CARDS_DESC, PC_STYLE_CARDS_SUBTITLE, PC_STYLE_CARDS_TITLE, PC_RING_PIC_HAIRCUTS:
                  c = list->FindChild(name); if (c) frame->AddPositionChild(c);
0x005884A1    c = FindChild("PC_HAIRCUTS_DESC");    if (c) { frame->AddPositionChild(c); viewport->AddFakeChild(c); }  // NO viewport null test
0x005884D4    PC_HAIRCUTS_SUBTITLE, PC_RING_PIC_FACIAL_HAIR (plain)
0x0058851E    PC_FACIAL_HAIR_DESC  (AddPosition + AddFakeChild)
0x00588551    PC_FACIAL_HAIR_SUBTITLE, PC_RING_PIC_TATTOOS (plain)
0x0058859B    PC_TATTOOS_DESC      (AddPosition + AddFakeChild)
0x005885CE    PC_TATTOOS_SUBTITLE  (plain)
            }
0x005885F3  CTCInventoryWeapons* weapons = 0; CTCInventory* inventory = 0; CTCInventoryClothing* clothing = 0;  // [74h],[30h],[20h]
0x005885FE  ParentThing->GetTC(TCI_INVENTORY_WEAPONS, &weapons);   // shape A (jle; `push 13h ; pop edi`)
0x00588638  ParentThing->GetTC(TCI_INVENTORY, &inventory);         // shape A
0x00588672  ParentThing->GetTC(TCI_INVENTORY_CLOTHING, &clothing); // shape B (jg, end kept in esi, `mov esi,eax`) -- see note
0x005886AC  const CThingPlayerCreature* hero = ...GetMainPlayer()->PeekPControlledCreature();   // esi
0x005886C5  CTCInventoryWeapons* heroWeapons = 0;   // [34h]  (zeroed together with the vector)
            std::vector<std::pair<long,long> > items; // [48h] begin/end/cap zeroed
0x005886D3  GetAllItems(items);                       // this
0x005886DC  for (it = items.begin(); it != items.end(); ) { if (CTCInventoryItem::IsBook(it->first)) it = items.erase(it); else ++it; }
0x00588706  hero->PeekTC(TCI_INVENTORY_WEAPONS, &heroWeapons);  // shape B, const LowerBound 0x004365B0
0x0058873A  heroWeapons->GetAllItems(items);          // no null test
0x00588746  if (clothing) clothing->GetAllItems(items);
0x00588758  long weaponsCount=0 [24h], itemsCount=0 [18h], clothingCount=0 [5Ch], haircutCount=0 [1Ch], facialHairCount=0 [40h],
            tattooCount=0 [44h];
            for (unsigned long i = 0; i < items.size(); ++i) {             // [54h]; rotated, entry test je 0x588C03
0x0058877C    CDefPointer<const CThingBaseDef> thingDef;                   // [6Ch]
              GetDefinitionManager().GetDef(items[i].first, &thingDef);
0x005887A2    CDefPointer<const CAugmentationDef> aug;                     // [34h]
              if (thingDef->GetSubDef(&aug)) continue;                     // 0x005887B5: dtors aug, thingDef inline, jmp 0x588BEE
0x005887E0    CDefPointer<const CHeroSuitDef> suit; CDefPointer<const CAppearanceModifierDef> appearance;  // [28h],[2Ch]
0x005887EE    if (CTCInventoryItem::IsWeapon(items[i].first)) weaponsCount += items[i].second;     // -> 0x588BC8
0x00588806    else if (thingDef->GetSubDef(&suit)) {
0x00588819      for (it = suit->SuitParts.begin(); it != suit->SuitParts.end(); ++it) {   // ++ = _M_increment 0x004292C0 at 0x5889CA
0x0058882A        if (clothing->GetNumberOfItemsOfTypeInInventory(it->second) > 0) {
0x0058883D          if (items[i].first == GetDefinitionManager().GetDefGlobalIndexFromName(CCharString("HERO_SUIT_NAKED"))
                        || (it->second != ...("OBJECT_HERO_NO_HAT") && it->second != ...("OBJECT_HERO_NO_SHIRT")
                            && it->second != ...("OBJECT_HERO_NO_GLOVES") && it->second != ...("OBJECT_HERO_NO_TROUSERS")
                            && it->second != ...("OBJECT_HERO_NO_BOOTS"))) {     // bool at [6Bh]; flag-word temp dtors 0x588958..0x5889C4
0x005889E4            clothingCount += items[i].second; break;           // -> 0x588BB5
                    }
                  }
                }
              }
0x005889F3    else if (thingDef->GetSubDef(&appearance)) {
0x00588A02      if (appearance->SuitPart > 0 && appearance->SuitPart <= 5) clothingCount += items[i].second;  // -> 0x588BA7
              }
0x00588A28    else if (!CTCInventoryItem::IsExpression(items[i].first)) {
0x00588A3B      CDefPointer<const CHairCardDef> hair = CTCInventoryItem::GetHairCardDef(items[i].first);     // [10h]
0x00588A49      CDefPointer<const CTattooDef> tattoo = CTCInventoryItem::GetTattooCardDef(items[i].first);   // [08h]
0x00588A57      if (hair.IsNull()) {
0x00588A60        if (tattoo.IsNull()) {
0x00588A6B          CDefPointer<const CThingObjectDef> objDef;                       // [64h]
                    CMainGameComponent::Get()->PeekDefinitionManager().GetDef(items[i].first, &objDef);
0x00588A8D          CDefPointer<const CAbilityDef> ability;                          // [0Ch]
                    if (objDef.IsNull() || !objDef->GetSubDef(&ability)) itemsCount += items[i].second;
0x00588AB2          // ~ability, ~objDef (dec [x+4] form)                -> 0x588B6C
                  } else if (!tattoo->IsInvalidCustomTattoo())          // 0x00588B59
                    tattooCount += items[i].second;
                } else {
0x00588ADD        CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(hair->[+0x28]);  // [14h]
0x00588AF8        if (!def.IsNull()) {
                    CDefPointer<const CAppearanceModifierDef> mod;                   // [64h]
                    def->GetSubDef(&mod);                                            // result ignored
0x00588B06          if (!mod.IsNull()) {
0x00588B0D            if ((mod->CoversBodyAreaFlags & 0x80) || (mod->CoversBodyAreaFlags & 0x100)) facialHairCount += items[i].second;
                      else if (mod->CoversBodyAreaFlags & 0x10) haircutCount += items[i].second;
0x00588B33            // ~mod
                    }
0x00588B3D          // ~def (add/dec/cmp form + `mov [ebp+14h],0`)
                  }
                }
0x00588B6C      // ~tattoo, ~hair (add/dec/cmp form + `and [x],0`)
              }
0x00588BA4    // ~appearance, 0x588BB5 ~suit, 0x588BC8 ~aug, 0x588BD9 ~thingDef
0x00588BEE  }  // ++i, cmp size, jb 0x58877C
0x00588C03  CViewport* viewport2 = list->FindChild("PC_DESCRIPTION_VIEWPORT");   // [54h]
            // --- clothing entry ---
0x00588C19  CComponent* entry = list->FindChild("PC_MENU_ENTRY_CLOTHING");        // esi
            viewport2->AddFakeChild(entry->FindChild("PC_CLOTHING_DESC"));        // no null tests
0x00588C56  if (clothing && clothingCount > 0) {
0x00588C6A    CComponent* button = entry->FindChild("PC_CLOTHING_BUTTON");
              CText* text = (CText*)button->FindChild("PC_TEXT_CLOTHING");
0x00588C9C    CWideString label(text->GetValue());                                // [6Ch]
0x00588CAF    label += CTCInventoryStats::GetXString();  label += GFIntToWideString(clothingCount);
0x00588CE4    text->SetValue(label, true, true);
0x00588CF6    CText* sub = (CText*)entry->FindChild("PC_CLOTHING_SUBTITLE"); if (sub) sub->SetValue(label, true, true);
0x00588D22    CCountedPointer<CAction> action(new CAction);                       // [38h]
              action->ActionType = UI_ACTION_TYPE_CONSTRUCT_CLOTHING_LIST;        // 69
              action->UIActionParams.push_back(&static_cast<CActionParam&>(*entry));
              action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
0x00588D75    entry->AddPreAction(action);
              // ~action (ReleaseData 0x00429160), ~label
            } else
0x00588D95    entry->MakeUnavailable();                                           // slot 92 (+0x170)
0x00588D9F  // weapons entry: PC_MENU_ENTRY_WEAPONS / PC_WEAPON_DESC; if (weapons && weaponsCount > 0) PC_WEAPONS_BUTTON /
            //   PC_TEXT_WEAPONS / PC_WEAPON_SUBTITLE, action [60h] type 68 (entry, list) ; else MakeUnavailable (0x588F18)
0x00588F22  // items entry: PC_MENU_ENTRY_ITEMS / PC_ITEMS_DESC; if (inventory && itemsCount > 0) PC_ITEMS_BUTTON / PC_TEXT_ITEMS /
            //   PC_ITEMS_SUBTITLE, action [60h] type 70 (entry, list) ; else MakeUnavailable (0x58909B)
0x005890A5  // style cards: entry = FindChild("PC_MENU_ENTRY_STYLE_CARDS"); viewport2->AddFakeChild(entry->FindChild("PC_STYLE_CARDS_DESC"));
0x005890DF  long styleCount = facialHairCount + tattooCount + haircutCount;   // eax=[40h]; ecx=[44h]; add; add [1Ch]  -> [64h]
            if (inventory && styleCount > 0) {          // NB tests `inventory` ([30h]), not a style TC
              PC_STYLE_CARDS_BUTTON / PC_TEXT_STYLE_CARDS, label += GetXString(), += GFIntToWideString(styleCount), SetValue;
              PC_STYLE_CARDS_SUBTITLE;
0x005891B7    action [38h] type 240 (UI_ACTION_TYPE_CONSTRUCT_PC_STYLE_CARDS_LIST);
              push_back(list) FIRST, then push_back(entry);
0x0058920A    push_back(new CActionParamSLONG(haircutCount)); push_back(new CActionParamSLONG(facialHairCount));
              push_back(new CActionParamSLONG(tattooCount));     // vtable 0x012366F4 hoisted into edi; temp ptr in [74h]
0x005892A8    entry->AddPreAction(action);  ~action; ~label
            } else entry->MakeUnavailable();   // 0x005892C8
0x005892D2  list->Initialise();                  // +0xAC
            Components.push_back(list);          // 0x00486024 via [ebp-4], &list
0x005892E8  // ~items: if (begin) free(begin)  -- end of the `if (!list)` block
TAIL 0x005892F7  (identical to OpenPCSkillsMenu's tail)
            SCROLL_LIST_UP, SCROLL_DESCRIPTION_UP, SCROLL_LIST_DOWN, SCROLL_DESCRIPTION_DOWN: c=list->FindChild(); if (c) c->ObserveEvent(0x19);
0x00589386  PC_BACK_CLICKABLE, PC_CLOSE_CLICKABLE: if (c) { CManager::GetInstance()->AddObserver(c); c->ObserveEvent(0x19); }
0x005893EE  list->ChangeState(5);
```
No jump tables, no cold blocks after the epilogue.

## Notes for the author
- The three ParentThing GetTC calls differ: two compile shape A (`cmp [eax],key ; jle` keep / `mov eax,edi`), the third (CLOTHING)
  and the hero PeekTC shape B (`cmp [eax],edi ; jg` keep end in esi, `mov esi,eax`). Try the GETTC_EXPANDED form for all first;
  if the third differs, it is probably because its result is the last live use (register pressure), not a different spelling.
- `continue` in the augmentation branch duplicates the two dtors inline (0x005887B5..0x005887DB) then jumps to the ++i block.
- The weapons branch skips the suit/appearance dtors (known null): keep suit/appearance declared AFTER the aug test, before IsWeapon.
- GetName / FindChild take CCharString by value (`push ecx ; mov ecx,esp`): write FindChild("...") as OpenPCSkillsMenu does.

# OUTLINE 0x005F1B10 CTCInventoryAbilities::ConstructPCMagicList(NUISystem::CComponent* parent) -> CComponent*

## Frame
- `push ebp; lea ebp,[esp-74h]; sub esp,124h; push ebx; push esi; push edi` ... `pop edi; pop esi; pop ebx; add ebp,74h; leave; ret 4`.
- Locals span [ebp-B0h, ebp+74h). No EH, no cookie. Lp<x>=[ebp+x], Lm<x>=[ebp-x]. Full symbolic map: $W/scout/map/ev0.txt.
- Whole-function slots: Lp20 this; Lp60 CManager* ui; Lp2c CComponent* frame; Lma8 const CDataBank* bank (PeekComponent()+0x14);
  Lm1c std::list<EHeroAbility> owned; Lm18 const CDefPointer<const CUIMiscThingsDef> misc (released at 0x005F3BAD inline);
  Lm20 attackCount, Lm24 surroundCount, Lm28 physicalCount (unsigned long); Lp1c root (returned); Lmb0 / Lma4 CCountedPointer<CComponent>
  bottom-right / bottom-left position dummies (ReleaseData at the very end 0x005F3B9D/0x005F3BA8); Lp70 later CComponent* viewport.
- Section temporaries (slots reused): CCharString temps Lp70/Lp6c/Lp58/Lp64/Lm8/Lmc/Lp68/Lp5c; CCountedPointer<CAction> Lp0, Lp24, Lp8,
  Lp4c, Lp10, Lp40, Lp30(spell sec), Lm14, Lm7c, Lm3c, Lm34; CCountedPointer<CComponent> Lm14 (title), Lp38, Lp30 (expr sec), Lp10 (spell sec),
  Lp40 entry, Lp8 ring, Lm88, Lm58, Lm44, Lm50; CCountedPointer<CMenuEntry> Lm90; CCountedPointer<CText> Lm74; CWideString Lp48 caption,
  Lp7c displayName (parent's slot reused); CDefPointer<const CThingObjectDef> Lp48 + CDefPointer<const CAbilityDef> Lp70 (expr sec);
  bool Lp1b isAbilityItem; CDeletion Lp38 (list at Lp3c).

## Body (retail order)
```
// 005F1B10
CManager* ui = CManager::GetInstance();                                    // 41E5F2
CComponent* frame = parent->FindChild("PC_UI_FRAME");                      // slot3, by-value CCharString
const NGameText::CDataBank& bank = PeekComponent().GetTextBank();          // 686D60 then [eax+14h]
std::list<EHeroAbility> owned;                                             // 5F88ED
const CDefPointer<const NUISystem::CUIMiscThingsDef> misc = GetMiscThingsDef();   // 5EE3B3
for (long n = 1; n < 20; ++n)                                              // 5F1B6E..5F1B9C (xor/inc esi)
    if (Abilities[n].first >= 0) owned.push_back((EHeroAbility)n);         // 5F88C8 insert(end)
AssignedAbilities.clear(); AssignedAbilitiesSave.clear();                  // 5F8F89 x2 (erase(begin,end))
unsigned long received = min(ReceivedAbilityOrder.size(), misc->TotalSpellsInPalettes);   // 5F1BC2 (`ja` picks; stored Lp6c)
for (unsigned long n = 0; n < received; ++n) {                             // Lp1c, 5F1BF5..5F1C32
    AssignedAbilities.push_back(ReceivedAbilityOrder[n]);                  // 5F9BCD
    AssignedAbilitiesSave.push_back(ReceivedAbilityOrder[n]); }
unsigned long assigned = AssignedAbilities.size();                         // 5F1C3D
if (assigned < misc->TotalSpellsInPalettes)                                // jae 5F1C8C
  for (unsigned long n = 0; n < misc->TotalSpellsInPalettes - assigned; ++n) {   // Lp6c; `sub; je` pre-test 5F1C53
    EHeroAbility e1 = 0 /*Lp5c*/, e2 = 0 /*Lm4*/;  AssignedAbilities.push_back(e1); AssignedAbilitiesSave.push_back(e2); }
CategoriesLists.clear();                                                   // 5F8890
unsigned long surround=0 /*Lm24*/, physical=0 /*Lm28*/, attack=0 /*Lm20*/;
for (it in owned) { c = GetCategory(*it);                                  // 5F1CAF..5F1CD4
    if (c==SURROUND) ++surround; if (c==PHYSICAL) ++physical; if (c==ATTACK) ++attack; }
// three unrolled list builders, order SURROUND(5F1CD6) / ATTACK(5F1D85) / PHYSICAL(5F1E2C), each:
if (surround > 0) {
    CList* list = (CList*)ui->CreateComponent("PC_SUB_LIST", false);
    list->InitialiseOffsets(surround);                                      // +270h
    CCountedPointer<CComponent> title(ui->CreateComponent("PC_TITLE_SURROUND", false));   // Lm14
    list->AddNonScrollingChild(title);                                      // +244h
    if (frame) frame->AddPositionChild(title.Get());
    CategoriesLists[SPELL_CATEGORY_SURROUND] = list;                        // 5F9E60 with key temp Lp5c=1
}   // same with "PC_TITLE_ATTACK" key 0 (attack), "PC_TITLE_PHYSICAL" key 2 (physical)
// 5F1ED6
root = ui->CreateComponent("PC_MAGIC_CATEGORIES", false);                  // Lp1c, kept in ebx
CCountedPointer<CComponent> bottomRight(ui->CreateComponent("PC_POSITION_DUMMY_BOTTOM_RIGHT", false));  // Lmb0
root->AddNonScrollingChild?(bottomRight) -- slot145 +244h on root (CList-like)  // 5F1F3C
bottomRight->AddPositionChild(root->FindChild("PC_SPELL_CONTAINMENT_CONTAINER"));        // 5F1F67
CCountedPointer<CComponent> bottomLeft(ui->CreateComponent("PC_POSITION_DUMMY_BOTTOM_LEFT", false));    // Lma4
root->(slot145)(bottomLeft); bottomLeft->AddPositionChild(root->FindChild("PC_EXPRESSIONS_ITEMS_CONTAINMENT_CONTAINER"));
// 5F1FE7 expression quick-slot section
CTCInventory* inventory; ParentThing->GetTC(&inventory) /*type 17, expanded*/;   // Lp54
for (unsigned long n = 0 /*Lp48*/; n < inventory->QuickAccessItems.size(); ++n) {   // 5F2031 .. jb 5F2031 at 5F266C; skip -> 5F2672
    CCharString name = "PC_EXPRESSION_ITEM_CONTAINMENT_" + GFIntToCharString(n + 1);   // Lp68; n+1 kept in Lm2c
    CComponent* slot = root->FindChild(name);                               // Lp6c (edi)
    if (slot) {                                                            // else 5F264D
      CCountedPointer<CAction> a0(new CAction); a0->ActionType=OBSERVE_EVENT(245);   // Lp0
      a0->AddParam(&slot->ActionParamBase /*slot+18h*/, a0); a0 params.push_back(new CActionParamULONG(0x19));
      root->AddActionOnSelected(a0);                                        // +10Ch on root (ebx)
      CCountedPointer<CAction> a1(new CAction); a1->ActionType=245; same with ULONG 0x1E; root->AddActionOnSelected(a1);   // Lp24
      CCountedPointer<CAction> a2(new CAction); a2->ActionType=ASSIGN_SPELL_TO_ITEM_SLOT_PC(285);   // Lp8
      a2->AddParam(slot+18h, a2); push_back(new CActionParamULONG(n)); slot->AddAction(a2);   // +108h
      if (inventory->QuickAccessItems[n] == 0) {                            // 5F21F7, else 5F2241
          CCountedPointer<CComponent> inside(ui->CreateComponent("PC_EXPRESSION_ITEM_INSIDE_CONTAINER",false));  // Lp38
          slot->AddChild(inside);  /* jmp 5F262D */
      } else {
          CCountedPointer<CComponent> icon(ui->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER",false)); // Lp30
          CComponent* sprite = icon->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
          if (sprite && inventory->QuickAccessItems[n] != 0)
              ((CSprite*)sprite)->SetGraphicIndex(inventory->GetQuickAccessItemGraphicIndex(n));   // 581FA4, +244h
          CCountedPointer<CAction> drop(new CAction); drop->ActionType=UNASSIGN_EXPRESSION_ITEM_PC(252);   // Lp4c
          drop->AddParam(slot+18h, drop); push_back(new CActionParamULONG(n)); icon->AddActionOnDroppedNowhere(drop);
          CDefPointer<const CThingObjectDef> thingDef;  /*Lp48*/  bool isAbility = false; /*Lp1b*/
          PeekComponent().PeekDefinitionManager().GetDef(inventory->QuickAccessItems[n], &thingDef);   // 42B040
          CDefPointer<const CAbilityDef> abilityDef;  /*Lp70*/
          if (thingDef && thingDef->GetSubDef(&abilityDef)) {               // 5936C2
              isAbility = true; drop.Reset(new CAction); drop->ActionType=SET_SPELL_TO_ASSIGN_PC(246);
              push_back(new CActionParamSLONG(abilityDef->?)) /*5F23B5, +8 then type 3*/ ;
          } else { drop.Reset(new CAction); drop->ActionType=SET_EXPRESSION_ITEM_TO_ASSIGN_PC(253); push_back(new SLONG(item)); }
          // cold arm 5F23D3..5F23FA jumps back to 5F23B3: the else arm is laid out after the if arm
          icon->AddActionOnDroppedNowhere(drop);                            // 5F2418 (second time, +12Ch)
          CCountedPointer<CAction> obs(new CAction); obs->ActionType=245; AddParam(icon? (neg/sbb null-adjust of Lp30+18h), obs);
          push_back(ULONG 0x19); root->AddActionOnSelected(obs);            // Lp10, 5F24A2
          CCountedPointer<CAction> swap(new CAction);                       // Lp40
          if (isAbility) { swap->ActionType=SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC(329); push_back(SLONG ...); push_back(SLONG n);
                           swap->AddParam(slot+18h); }                     // 53FDB0
          else { swap->ActionType=SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC(327); push_back(SLONG ...); push_back(SLONG n); AddParam(slot+18h); }
          icon->AddActionOnDropped(swap);                                   // +128h
          slot->AddChild(icon);                                             // 5F25EA
          // dtors: swap, obs, abilityDef release(5F2607), thingDef release(5F2618), drop, icon
      }
      // dtors 5F262D: inside/icon CCP, a2, a1, a0
    }
}   // ~name at 5F264D
// 5F2672 spell quick-slot section, Lm4 = n+1, Lp54 = n
for (unsigned long n = 0; n < misc->TotalSpellsInPalettes; ++n) {          // jbe 5F2BA9; loop back 5F2BA3
    CCharString name = "PC_SPELL_CONTAINMENT_" + GFIntToCharString(n + 1);  // Lm8
    CComponent* slot = root->FindChild(name);                              // Lp6c; null -> 5F2B8C
    a0 (Lp0): 245 + ULONG 0x19 -> root->AddActionOnSelected; a1 (Lp24): 245 + ULONG 0x1E -> root->AddActionOnSelected;
    a2 (Lp30): ASSIGN_SPELL_PC(244) + AddParam(slot+18h,a2) + ULONG(n) -> slot->AddAction(a2);
    if (n < ReceivedAbilityOrder.size()) {                                  // jae 5F2B74
      if (ReceivedAbilityOrder[n] == 0) { CCP inside(CreateComponent("PC_SPELL_INSIDE_CONTAINER")) /*Lp38*/; slot->AddChild(inside); }
      else {                                                               // 5F28AB
        CCP icon(CreateComponent("PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER")) /*Lp10*/;
        if (CComponent* s = icon->FindChild("PC_SPELL_ASSIGNMENT_ICON_SPRITE"))
            s->SetGraphicIndex(Abilities[ReceivedAbilityOrder[n]].second->IconGraphicIndex);   // +244h
        Lp40: UNASSIGN_SPELL_PC(248) + AddParam(slot+18h) + ULONG(n) -> icon->AddActionOnDroppedNowhere;
        Lp40.Reset(new CAction) SET_SPELL_TO_ASSIGN_PC(246) + SLONG(ReceivedAbilityOrder[n]) -> icon->AddActionOnDroppedNowhere;
        Lp4c: 245 + AddParam(icon+18h null-adjusted) + ULONG 0x19 -> root->AddActionOnSelected;
        Lp8 : SET_SPELL_TO_ASSIGN_SWAPPING_PC(328) + SLONG + SLONG(n) + AddParam(slot+18h) (53FDB0) -> icon->AddActionOnDropped;
        slot->AddChild(icon);   // 5F2B4E ; dtors Lp8, Lp4c, Lp40, Lp10
      }
    }
    // dtors 5F2B74: Lp30, Lp24, Lp0; ~name 5F2B8C
}
// 5F2BA9
viewport /*Lp70*/ = parent->FindChild("PC_DESCRIPTION_VIEWPORT");
if (viewport) { for d in {"PC_DESCRIPTION_ATTACK_SPELLS","PC_DESCRIPTION_PHYSICAL_SPELLS","PC_DESCRIPTION_SURROUND_SPELLS"}
                  if (c = root->FindChild(d)) viewport->AddFakeChild(c); }   // +238h, three unrolled ifs
if (frame) {                                                               // je 5F2DCA
  for s in {"PC_TITLE_MAGIC_CATEGORIES","PC_RING_PIC_MAGIC_ATTACK","PC_SUBTITLE_ATTACK_SPELLS","PC_DESCRIPTION_ATTACK_SPELLS",
            "PC_RING_PIC_MAGIC_SURROUND","PC_SUBTITLE_SURROUND_SPELLS","PC_DESCRIPTION_SURROUND_SPELLS",
            "PC_RING_PIC_MAGIC_PHYSICAL","PC_SUBTITLE_PHYSICAL_SPELLS","PC_DESCRIPTION_PHYSICAL_SPELLS"}
      if (c = root->FindChild(s)) frame->AddPositionChild(c);              // 10 unrolled
  if (viewport) frame->AddPositionChild(viewport);                          // 5F2DB7
}
UnassignedIcons.clear(); AbilitiesMenuEntries.clear();                     // 579353 x2
// 5F2DF6 per-ability loop over owned (iterator in esi/Lp5c), end test 5F3678
for (it in owned) {
  CWideString caption /*Lp48*/, displayName /*Lp7c*/;
  CTCInventoryExperience* exp;  if (ParentThing->GetTC(&exp) /*22, expanded*/) {    // skip -> 5F2F63
     caption = displayName = exp->GetExperienceAbilityName(*it);           // two operator= in that order: Lp7c then Lp48
     caption += misc->SpaceSeparator; caption += GFIntToWideString(Abilities[*it].first + 1);
     displayName += CWideString(L" - ") + bank.GetTextBySymbol("TEXT_GUI_MENU_LEVEL") + CWideString(L" ");
     displayName += GFIntToWideString(Abilities[*it].first + 1); }
  CCountedPointer<CComponent> entry(ui->CreateMenuEntryComponent(caption, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));   // Lp40
  CCountedPointer<CComponent> icon(ui->CreateComponent("PC_SPELL_ASSIGNMENT_ICON", false));   // Lp10
  CComponent* sprite = icon->FindChild("PC_SPELL_ASSIGNMENT_ICON_SPRITE");   // kept in Lp6c
  CCountedPointer<CComponent> faded(ui->CreateComponent("PC_SPELL_ICON_FADED", false));       // Lp38
  Lm14: SET_SPELL_TO_ASSIGN_PC(246) + SLONG(*it) -> icon->AddActionOnDropped;     // 5F30B7
  Lm7c: 246 + SLONG(...)                      -> icon->AddActionOnDroppedNowhere; // 5F3124
  if (icon) icon->slot152(0);                                                    // 5F3135
  sprite->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex); faded->SetGraphicIndex(same);
  entry->AddChild(icon); entry->AddChild(faded);
  AbilitiesMenuEntries[*it] = entry.Get();                                         // 5F9EA5
  if (std::find(AssignedAbilities.begin(), AssignedAbilities.end(), *it) == AssignedAbilities.end()) {   // 5F8150
      CCountedPointer<CComponent> un(ui->CreateComponent("PC_UNASSIGNED_SPELL_ICON", false));   // Lm88
      entry->AddChild(un); UnassignedIcons[*it] = un.Get(); }
  CCountedPointer<CMenuEntry> menuEntry; menuEntry.SetStaticCast(entry);           // 48524C ShareData
  CCountedPointer<CComponent> sub(ui->CreateComponent("PC_SUBTITLE", false));       // Lm58
  CCountedPointer<CText> subText; subText.SetStaticCast(sub); subText->SetValue(displayName, true, true);   // 429460, +240h
  entry->AddChild(sub); if (frame) frame->AddPositionChild(sub.Get());
  CCountedPointer<CComponent> pcIcon(ui->CreateComponent("PC_ICON", false));        // Lm44
  if (pcIcon->GetType() == 0) pcIcon->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
  entry->AddChild(pcIcon); if (frame) frame->AddPositionChild(pcIcon.Get());
  CCountedPointer<CComponent> ring;                                                // Lp8
  switch (GetCategory(*it)) {   // 5F3337: 0->5F339E ATTACK, 1->5F3374 SURROUND, 2->5F334A PHYSICAL (laid out PHYSICAL,SURROUND,ATTACK), default->5F33F2
    case PHYSICAL: ring.Reset(ui->CreateComponent("PC_RING_PIC_MAGIC_PHYSICAL", false)); break;
    case SURROUND: ring.Reset(ui->CreateComponent("PC_RING_PIC_MAGIC_SURROUND", false)); break;
    case ATTACK:   ring.Reset(ui->CreateComponent("PC_RING_PIC_MAGIC_ATTACK", false)); break; }
  if (ring) { entry->AddChild(ring); if (frame) frame->AddPositionChild(ring.Get()); }   // 5F33C3.. (check exact nesting in raw)
  CCountedPointer<CComponent> desc(ui->CreateComponent("PC_DESCRIPTION", false));   // Lm50, 5F33F2
  ((CText*)desc.Get())->SetValue(GetAbilityPDef(*it)->LevDescription[Abilities[*it].first], CWideString(L""), true);  // +23Ch; CDefPointer temp Lm2c released inline
  entry->AddChild(desc); viewport->AddFakeChild(desc.Get()); frame->AddPositionChild(desc.Get());   // 5F348C/5F349E/5F34B0 (check null tests)
  CList* catList = (CList*)CategoriesLists[GetCategory(*it)];                       // 5F9E60
  catList->AddScrollingChildAndSetOffsets(entry);                                   // +260h
  Lm3c: 245 + AddParam(icon+18h null-adj, Lm3c) + ULONG 0x19 -> catList->AddActionOnSelected;  // 5F3568
  Lm34: IGNORE_EVENT(249) + same + ULONG 0x19 -> catList->AddActionOnInactive;      // 5F35F2
  // dtors 5F35FB..5F366F: Lm34, Lm3c, Lm50, Lp8, Lm44, Lm74, Lm58, Lm90, Lm7c, Lm14, Lp38, Lp10, Lp40, ~Lp7c, ~Lp48
}
// 5F3687 final categories, UNROLLED (key temp Lp7c; Lp38 CCP/CDeletion):
it = CategoriesLists.find(ATTACK);                                                  // 5F840C
if (it != end) { CCountedPointer<CComponent> c(it->second);
   CComponent* e = GetMenuEntryFromCategory(ATTACK, root); e->AddChild(c);
   CText* t = (CText*)e->FindChild("UI_MENU_ENTRY_ATTACK_SPELLS_TEXT");
   t->SetValue(t->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(attack), true, true);
   CText* s = (CText*)e->FindChild("PC_SUBTITLE_ATTACK_SPELLS");
   s->SetValue(s->GetValue() + GetXString() + GFIntToWideString(attack), true, true); }   // null tests: check raw
else { CComponent* e = GetMenuEntryFromCategory(ATTACK, root); e->MakeUnavailable();
   CComponent* arrow = e->FindChild("UI_SELECTION_ARROW_RIGHT");
   if (arrow) { CDeletion d; d.Method = ...; arrow->SetDeletion(d); } }             // 42BED4 / 42CD84 / ~list 42ABCA
// then SURROUND (5F384E, "UI_MENU_ENTRY_SURROUND_SPELLS_TEXT", "PC_SUBTITLE_SURROUND_SPELLS", surround)
// then PHYSICAL (5F39FF, "UI_MENU_ENTRY_PHYSICAL_SPELLS_TEXT", "PC_SUBTITLE_PHYSICAL_SPELLS", physical)
// 5F3B97: ~bottomLeft, ~bottomRight, misc release, ~owned; return root;
```
Open points for the author (read raw $W/dis_all.txt): the SLONG values at 0x005F23B5 / 0x005F24D9 / 0x005F2557 / 0x005F2AC0 / 0x005F3079
/ 0x005F30E8 (which def field / slot index), the exact null tests around 0x005F33C3..0x005F34B0 and in the final text updates, the
CDeletion Method value (copy ConstructScreen's console twin), and slot 152 on the assignment icon (see ConstructPCItemLists).

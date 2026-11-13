# OUTLINE -- 0x005EEFF2 CTCInventoryAbilities::GetMagicListUIComponent(long& count, NUISystem::CComponent* owner)
Leaf: ?GetMagicListUIComponent@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@AAJPAV23@@Z   (returns the "UI_MAGIC_CATEGORIES" root)
Annotated listing (strings + callee names): $W/scout/ann.txt  (made by $W/scout/ann.py)

## Frame facts
- `push ebp ; lea ebp,[esp-0x70] ; sub esp,0xF4 ; push ebx ; push esi ; mov ebx,ecx ; push edi` -- BIASED frame (ebp = entry_esp-4-0x70),
  same form as siblings ConstructScreen 0x005F519F (`lea ebp,[esp-74h]`), ContructItemLists 0x005F077F / ConstructPCItemLists 0x005F1526
  (`lea ebp,[esp-68h]`): those rows use `#pragma optimize("s", on)` ONLY (no "y" off) and `// Flags:` empty.
- Epilogue: `pop edi ; pop esi ; pop ebx ; add ebp,70h ; leave ; ret 8`. Saved order push ebx, esi, edi (pop reverse).
- NO SEH / no fs:0 / no EH states although many destructors run => TU built WITHOUT /EHsc (siblings' Flags empty). No /GS cookie.
- Args: ebp+0x78 = `long& count` (slot later reused to hold the return value `root`), ebp+0x7C = `owner` (slot later reused as the
  std::list iterator node and as scratch). ebp+0x7F (top byte of owner slot) = the empty allocator temporary passed to the list ctor.
- `this` spilled to [ebp+0x48] early (that slot is reused later as CWideString displayName); after 0x5EFAF1 `this` lives in esi.
- size idioms: `xor esi,esi ; inc esi` (n=1), `and [ebp+x],0`, `or ebx,-1` (ebx = -1 = CCharString ctor's length arg, held all function).

## Frame map (ebp-relative; 8-byte CCountedPointer = {Data, RefCount block})
| slot | local | type / ctor-dtor |
|---|---|---|
| -0x84 | selection | CCountedPointer<CComponent> 0x429C15 / ReleaseData 0x4291DE |
| -0x7C | indication | CCountedPointer<CComponent> |
| -0x74 | tmp GFIntToWideString (caption level) | CWideString, dtor 0x99B510 |
| -0x70 | tmp GetExperienceAbilityName result | CWideString |
| -0x6C, -0x58 | tmps of `operator+` (0x99BE70) | CWideString |
| -0x68 | set (action 0xCE) | CCountedPointer<CAction> ctor 0x42AA29 / ReleaseData 0x429160 |
| -0x60 | actions ("UI_LIST_MAGIC_ACTIONS") | CCountedPointer<CComponent> |
| -0x54 | exclusive (action 0xCA) | CCountedPointer<CAction> |
| -0x4C | show (action 0xCD) | CCountedPointer<CAction> |
| -0x44 | bank | const NGameText::CDataBank* (PeekComponent()+0x14) |
| -0x40..-0x38 | reordered | std::vector<EHeroAbility>(n) ctor 0x5F8EC0; dtor inline `if(start) free(start)` |
| -0x34 | text | CCountedPointer<CText> ShareData 0x429460 / dtor 0x42925C |
| -0x2C | description | CCountedPointer<CComponent> |
| -0x24,-0x20,-0x1C,-0x18 | CCharString temps (name/icon/unassigned/description templates) | 0x99EBF0 / 0x99EAE0 |
| -0x14 | palette ("UI_SPELL_CONTAINER_LIST_TEMPLATE") | CCountedPointer<CComponent> |
| -0x0C | container | CCountedPointer<CComponent> |
| -0x04 | `empty2` EHeroAbility(0) in fill loop; later the GetAbilityPDef CDefPointer temp | |
| +0x00 | entry (menu entry) | CCountedPointer<CComponent> |
| +0x08 | owned | std::list<EHeroAbility> (4 bytes: sentinel ptr); _List_base ctor 0x5F88ED, dtor 0x5F8908 |
| +0x0C | ring (category lists) / name (menu loop) | CCountedPointer<CComponent> |
| +0x18 | spell (palette loop) / menuEntry (menu loop) | CCountedPointer<CComponent> / CCountedPointer<CMenuEntry> (ShareData 0x48524C, ReleaseData 0x4850F9) |
| +0x20 | paletteList (raw CSpellContainerList*) then viewport (CViewport*) | |
| +0x24 | misc | const CDefPointer<const CUIMiscThingsDef> (GetMiscThingsDef 0x5EE3B3 hidden ret) |
| +0x2C | button (category lists) / icon (menu loop) | CCountedPointer<CComponent> |
| +0x34 | attackCount (cat 0) | unsigned long |
| +0x38 | surroundCount (cat 1) | unsigned long |
| +0x3C..+0x4C | CWideString temps (title/subtitle rebuild, level text) | |
| +0x48 | displayName (menu loop) | CWideString |
| +0x50 | title (category lists) / unassigned icon / `children` CCountedPointer / CDeletion (8 bytes: Method +0x50, list +0x54) | |
| +0x5C | `empty` EHeroAbility / list-push value / map key / CWideString L"" (desc) / entry ptr | |
| +0x60 | ui | NUISystem::CManager* |
| +0x64 | loop n / physicalCount (cat 2) | |
| +0x68 | loop n / caption CWideString (menu loop) / CCharString temp | |
| +0x6C | CCharString temp | |

## Block-by-block (retail order)
```
0x5EEFF2 prologue
0x5EF005 bank = &PeekComponent().GetTextBank();            // call CTCBase::PeekComponent 0x686D60, +0x14 -> [ebp-0x44]
0x5EF010 std::list<EHeroAbility> owned;                    // _List_base(const allocator&) 0x5F88ED (out of line), alloc temp [ebp+0x7F]
0x5EF01C NUISystem::CManager* ui = NUISystem::CManager::GetInstance();   // 0x41E5F2 -> [ebp+0x60]
0x5EF024 const CDefPointer<const NUISystem::CUIMiscThingsDef> misc = GetMiscThingsDef();  // 0x5EE3B3, hidden ret &[ebp+0x24]
0x5EF02F for (long n = 1; n < 20; ++n)                      // esi; `xor esi,esi; inc esi`; Abilities = this+0x160 (8-byte elems)
            if (Abilities[n].first >= 0) owned.push_back((EHeroAbility)n);   // push_back -> list::insert(end(), x) OUT OF LINE 0x5F88C8
0x5EF05F AssignedAbilities.clear();      // this+0x17C (edi) -> vector<EHeroAbility>::erase(begin,end) 0x5F8F89 out of line
0x5EF071 AssignedAbilitiesSave.clear();  // this+0x188 (esi) -> erase 0x5F8F89
0x5EF083 received = ReceivedAbilityOrder.size() > misc->TotalSpellsInPalettes(+0x148) ? misc->Total.. : ReceivedAbilityOrder.size();
         // retail: `cmp size,total ; ja -> [0x68]=total ; else recompute size -> [0x68]`  (std::min<unsigned long>(size,total) gives this shape? verify; else ternary)
0x5EF0B6 for (unsigned long n = 0; n < received; ++n) {     // [ebp+0x64]
            AssignedAbilities.push_back(ReceivedAbilityOrder[n]);      // 0x5F9BCD out of line (VECTOR_PUSHBACK_OUTOFLINE)
            AssignedAbilitiesSave.push_back(ReceivedAbilityOrder[n]);  }
0x5EF0F5 unsigned long assigned = AssignedAbilities.size();   // ebx
         if (assigned < misc->TotalSpellsInPalettes) {        // jae 0x5EF142
            EHeroAbility empty = 0 /*[ebp+0x5C]*/, savedEmpty = 0 /*[ebp-4]*/;   // stored ONCE before loop
            for (unsigned long n = 0; n < misc->TotalSpellsInPalettes - assigned; ++n) {   // [ebp+0x68]; total reloaded
               AssignedAbilities.push_back(empty); AssignedAbilitiesSave.push_back(savedEmpty); } }
0x5EF142 for (unsigned long n = 0; n < AssignedAbilities.size() && n < AssignedAbilitiesSave.size(); ++n)  // [ebp+0x68]
            if (n % misc->TotalSpellsInContainer(+0x14C) == 1 && n < AssignedAbilities.size()-1 && n < AssignedAbilitiesSave.size()-1) {
               swap AssignedAbilities[n] <-> [n+1]; swap AssignedAbilitiesSave[n] <-> [n+1];   // inline, base reloaded between (temp swap)
            }
0x5EF1C3 CCountedPointer<CComponent> palette  (ui->CreateComponent("UI_SPELL_CONTAINER_LIST_TEMPLATE", false));  // [ebp-0x14]
0x5EF1F5 CCountedPointer<CComponent> container(ui->CreateComponent("UI_SPELL_CONTAINER_TEMPLATE", false));       // [ebp-0x0C]
0x5EF221 CCountedPointer<CComponent> selection(ui->CreateComponent("UI_SPELL_SELECTION_TEMPLATE", false));       // [ebp-0x84]
0x5EF250 CCountedPointer<CComponent> indication(ui->CreateComponent("UI_UNSELECT_FOR_MAGIC_ASSIGNMENT_LIST", false)); // [ebp-0x7C]
         // each CreateComponent: CCharString temp at [ebp+0x68] (ctor 0x99EBF0(str,-1)), CreateComponent 0x41DB1D(const CCharString&, bool), dtor
0x5EF27C CSpellContainerList* paletteList = (CSpellContainerList*)palette.Get();   // [ebp+0x20]
         paletteList->InitialiseOffsets(AssignedAbilities.size() / misc->TotalSpellsInContainer + 1);   // +0x250 slot 148
         paletteList->AddSpellSelectionChild(selection);   // +0x238 slot 142
         paletteList->AddSpellIndicationChild(indication); // +0x23C slot 143
0x5EF2C4 ((CSpellContainer*)container.Get())->SetEmptySlotDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_SPELL_EMPTY_TEMPLATE"));
         // container ptr cached BEFORE the CCharString ctor ([ebp+0x68]); Get 0x44C6B0, GetDefGlobalIndexFromName 0x9AD410, slot 145 (+0x244)
0x5EF2FC std::vector<EHeroAbility> reordered(AssignedAbilities.size());   // 0x5F8EC0 out of line (VECTOR_N_OUTOFLINE)
0x5EF30D for (unsigned long n = 0; n < AssignedAbilities.size(); ++n)    // [ebp+0x64]
            reordered[AssignedAbilities.size() - (n/C + 1)*C + n%C] = AssignedAbilities[n];   // C = misc->TotalSpellsInContainer, one div
0x5EF35B unsigned long paletteSize = reordered.size();   // [ebp+0x5C]; `and [ebp+0x64],0`
         for (unsigned long n = 0; n < paletteSize; ++n) {
0x5EF371    if (n % C == 0 && n != 0) {
0x5EF388       paletteList->AddChildAndSetOffsets(container);    // +0x24C slot 147
0x5EF397       container.Reset(ui->CreateComponent("UI_SPELL_CONTAINER_TEMPLATE", false));   // Reset 0x429C6F, CCharString temp [ebp+0x38]
0x5EF3C4       ((CSpellContainer*)container.Get())->SetEmptySlotDef(GetDefGlobalIndexFromName("UI_SPELL_EMPTY_TEMPLATE"));  // temp [ebp+0x34]
            }
0x5EF3FC    CCountedPointer<CComponent> spell;   // [ebp+0x18], {0,0} inline
            if (reordered[n] != 0) {
0x5EF412       spell.Reset(ui->CreateComponent("UI_SPELL_TEMPLATE", false));   // temp [ebp+0x6C]
0x5EF43E       ((CSprite*)spell.Get())->SetGraphicIndex(Abilities[reordered[n]].second->IconGraphicIndex /*+0x44*/);  // +0x244
            } else
0x5EF45D       spell.Reset(ui->CreateComponent("UI_SPELL_EMPTY_TEMPLATE", false));  // temp [ebp+0x58]
0x5EF489    ((CSpellContainer*)container.Get())->AddChildAndSetOffsets(spell);   // container ptr from [ebp+0x68] (cached), +0x24C
0x5EF498    ~spell (ReleaseData)
         }
0x5EF4AF paletteList->AddChildAndSetOffsets(container);
0x5EF4BE CCountedPointer<CComponent> actions(NUISystem::CManager::GetInstance()->CreateComponent("UI_LIST_MAGIC_ACTIONS", false)); // [ebp-0x60]; GetInstance called AGAIN (not ui)
0x5EF4EF CComponent* assignment = actions->FindChild("UI_MENU_ENTRY_MAGIC_ACTION_ASSIGN");   // slot 3 (+0xC), CCharString BY VALUE (built on stack)
0x5EF50B CCountedPointer<CAction> exclusive(new CAction); exclusive->ActionType = 0xCA;   // new(0x10) 0xBFEA1A, CAction() 0x42BE50, CCP ctor 0x42AA29
0x5EF530 CCountedPointer<CAction> show(new CAction);      show->ActionType = 0xCD;
0x5EF559 CCountedPointer<CAction> set(new CAction);       set->ActionType = 0xCE;
0x5EF582 assignment->AddAction(exclusive); AddAction(show); AddAction(set);   // slot 66 (+0x108)
0x5EF5B2 CategoriesLists.clear();     // this+0x194, tree clear 0x5F8890 out of line (TREE_CLEAR_OUTOFLINE)
0x5EF5C0 unsigned long surroundCount=0 /*+0x38*/, physicalCount=0 /*+0x64*/, attackCount=0 /*+0x34*/;
         for (std::list<EHeroAbility>::iterator it = owned.begin(); it != owned.end(); ++it) {   // if empty -> jumps straight to 0x5EFA11
            EUISpellCategory c = GetCategory(*it);   // 0x5EDE37
            if (c == SPELL_CATEGORY_SURROUND) ++surroundCount;   // 1
            if (c == SPELL_CATEGORY_PHYSICAL) ++physicalCount;   // 2
            if (c == SPELL_CATEGORY_ATTACK)   ++attackCount; }   // 0
--- three UNROLLED blocks (written out, not a loop: three distinct key stores 1/0/2, three title strings) ---
0x5EF600 if (surroundCount) {     // je 0x5EF75C
            CList* list = (CList*)ui->CreateComponent("UI_SUB_LIST", false);   // esi; temp [ebp+0x58]
            list->InitialiseOffsets(surroundCount);                            // +0x270 slot 156
            CCountedPointer<CComponent> ring(ui->CreateComponent("UI_RING_PIC_SPELL_ASSIGNMENT", false));  // [ebp+0xC]
            list->AddNonScrollingChild(ring);                                  // +0x244 slot 145
            CCountedPointer<CComponent> title(ui->CreateComponent("UI_SURROUND_CATEGORIES_TITLE", false)); // [ebp+0x50]
            list->AddNonScrollingChild(title);
            CCountedPointer<CComponent> button(ui->CreateComponent("UI_BACK_FOR_LIST", false));   // [ebp+0x2C]
            list->AddNonScrollingChild(button);
            button.Reset(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
            list->AddNonScrollingChild(button);
            CategoriesLists[SPELL_CATEGORY_SURROUND] = list;   // key temp [ebp+0x5C]=1, map::operator[] 0x5F9E60
         }   // ~button ~title ~ring
0x5EF75C if (attackCount)  { ... "UI_ATTACK_CATEGORIES_TITLE" ...  CategoriesLists[SPELL_CATEGORY_ATTACK] = list; }   // key via `and [ebp+0x5C],0`
0x5EF8B5 if (physicalCount){ ... "UI_PHYSICAL_CATEGORIES_TITLE" ... CategoriesLists[SPELL_CATEGORY_PHYSICAL] = list; } // key 2
0x5EFA11 count = attackCount + physicalCount + surroundCount;   // retail: [0x34]+[0x64] then +[0x38]
0x5EFA1F CComponent* root = ui->CreateComponent("UI_MAGIC_CATEGORIES", false);   // stored into [ebp+0x78] (count's slot)
0x5EFA48 CViewport* viewport = (CViewport*)owner->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");   // slot 3, by-value CCharString; -> [ebp+0x20]
         if (viewport) {
            if (CComponent* d = root->FindChild("UI_ATTACK_SPELLS_DESC"))   viewport->AddFakeChild(d);   // +0x238 slot 142
            if (CComponent* d = root->FindChild("UI_PHYSICAL_SPELLS_DESC")) viewport->AddFakeChild(d);
            if (CComponent* d = root->FindChild("UI_SURROUND_SPELLS_DESC")) viewport->AddFakeChild(d); }
0x5EFAF1 UnassignedIcons.clear();        // this+0x1A0, tree clear 0x579353 (shared COMDAT)
         AbilitiesMenuEntries.clear();   // this+0x1AC, 0x579353
0x5EFB0A for (it = owned.begin(); it != owned.end(); ++it) {   // node kept in [ebp+0x7C]; loop test at 0x5F004D, entry jmp 0x5F0055
0x5EFB16    CWideString caption;      // [ebp+0x68]  ctor 0x99AED0 (constructed FIRST)
            CWideString displayName;  // [ebp+0x48]
0x5EFB26    CTCInventoryExperience* experience;   // GetTC expanded inline (CTHING_GETTC_EXPANDED): bitset<274>::test(0x16) 0x410DE0 on ParentThing+0x20,
            if (ParentThing->GetTC(&experience) /*type 22*/) {   //   CVectorMap::LowerBound 0x40F020 on ParentThing+0x44; not-found -> 0x5EFC80
0x5EFB6B       caption = displayName = experience->GetExperienceAbilityName(*it);  // 0x56F2BD (hidden ret [ebp-0x70]); CHAINED: operator= 0x99B7D0 on
                                                                                  // displayName, its RESULT pushed to caption's operator=
0x5EFB94       caption += misc->SpaceSeparator;                          // misc+0x3C, operator+= 0x99B8D0
0x5EFBA3       caption += GFIntToWideString(Abilities[*it].first + 1);   // 0x405F00 fastcall(ret ecx, value edx)
0x5EFBCF       displayName += CWideString(L" - ") + bank->GetTextBySymbol("TEXT_GUI_MENU_LEVEL") + CWideString(L" ");
               // temps in order: CCharString [+0x58], CWideString(L" " @0x122D820) [+0x40], GetTextBySymbol 0x9C95E0 [+0x44],
               // CWideString(L" - " @0x122E070) [+0x3C], op+ -> [-0x58], op+ -> [-0x6C]; dtors -0x6C,-0x58,+0x3C,+0x44,+0x40,+0x58
0x5EFC55       displayName += GFIntToWideString(Abilities[*it].first + 1);   // temp [+0x4C]
            }
0x5EFC80    CCountedPointer<CComponent> entry(ui->CreateMenuEntryComponent(caption, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                              0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));   // 0x428494; [ebp+0]
            // "" is the shared literal 0x122D70E; spell exactly as ContructItemLists 0x005F077F does
0x5EFCE5    AbilitiesMenuEntries[*it] = entry.Get();   // this+0x1AC, operator[] 0x5F9EA5 (entry ptr read BEFORE the call into [ebp+0x28])
0x5EFD02    if (std::find(AssignedAbilities.begin(), AssignedAbilities.end(), *it) == AssignedAbilities.end()) {   // find 0x5F8150 (__fastcall COMDAT, FABLE_RETAIL_STLP_FIND)
0x5EFD1F       CCountedPointer<CComponent> icon(ui->CreateComponent("UI_UNASSIGNED_SPELL_ICON", false));   // [ebp+0x50], temp [-0x1C]
               entry->AddChild(icon);                   // slot 59 (+0xEC)
               UnassignedIcons[*it] = icon.Get();       // this+0x1A0, 0x5F9EA5
            }
0x5EFD7F    CCountedPointer<CMenuEntry> menuEntry; menuEntry.SetStaticCast(entry);   // [ebp+0x18] {0,0} + ShareData 0x48524C(entry.Data, entry.RefCount)
0x5EFD95    entry->AddChild(palette);                                              // +0xEC
0x5EFDA4    menuEntry->AddUIActionParam(new CActionParamSLONG(*it));               // new(0xC); inline ctor: vtbl 0x12366F4, Type 3, value; slot 151 (+0x25C)
0x5EFDD8    menuEntry->AddUIActionParam(new CActionParamULONG(Abilities[*it].second->IconGraphicIndex));   // vtbl 0x122F844, Type 5
0x5EFE19    menuEntry->AddUIActionParam(palette.Get());                            // CComponent* -> CActionParam* (base at +0x18; neg/sbb/and null-guard)
0x5EFE37    CCountedPointer<CComponent> name(ui->CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));   // [ebp+0xC], temp [-0x24]
0x5EFE63    CCountedPointer<CText> text; text.SetStaticCast(name);                // [ebp-0x34], ShareData 0x429460
            text->SetValue(displayName, true, true);                              // slot 144 (+0x240)
0x5EFE8C    entry->AddChild(name);
0x5EFE9C    CCountedPointer<CComponent> icon(ui->CreateComponent("UI_ICON_TRADE_TEMPLATE", false));   // [ebp+0x2C], temp [-0x20]
0x5EFEC8    if (icon->GetType() == 0)  /* slot 65 +0x104 */
               ((CSprite*)icon.Get())->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);   // +0x244
0x5EFEFA    entry->AddChild(icon);
0x5EFF09    entry->AddChild(actions);
0x5EFF19    CCountedPointer<CComponent> description(ui->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", false));  // [ebp-0x2C], temp [-0x18]
0x5EFF45    ((CText*)description.Get())->SetValue(GetAbilityPDef(*it)->LevDescription[Abilities[*it].first], CWideString(L""), true);
            // slot 143 (+0x23C) SetValue(unsigned long textId, const CWideString&, bool); L"" = GLOBAL literal 0x122D70C (NOT a stack array);
            // CWideString temp [ebp+0x5C] built first; GetAbilityPDef 0x5EDFD9 hidden ret [ebp-4]; LevDescription = def+0x60 (vector<long>)
            // temp CDefPointer released inline WITH null check + zeroing (`add eax,4; dec [eax]; cmp [eax],0`), then ~CWideString
0x5EFFBB    entry->AddChild(description);
0x5EFFCA    if (viewport) viewport->AddFakeChild(description.Get());   // +0x238
0x5EFFDE    ((CList*)CategoriesLists[GetCategory(*it)])->AddScrollingChildAndSetOffsets(entry);   // key temp [ebp+0x28], 0x5F9E60, slot 152 (+0x260)
0x5F000D    dtors: description, icon, text(0x42925C), name, menuEntry(0x4850F9), entry, displayName, caption
         }
--- three UNROLLED category finishes: ATTACK(0), SURROUND(1), PHYSICAL(2) (written out) ---
0x5F005E { EUISpellCategory key = SPELL_CATEGORY_ATTACK;   // key in [ebp+0x7C]; edi=&CategoriesLists
           std::map<..>::iterator found = CategoriesLists.find(key);   // _M_find 0x5F840C out of line (TREE_MFIND_DECL?), compare with header node [edi]
           if (found != CategoriesLists.end()) {
0x5F007B      CCountedPointer<CComponent> children(found->second);          // node+0x14, [ebp+0x50]
              CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_ATTACK, root);   // 0x5EE288; -> [ebp+0x5C]
              menu->AddChild(children);
0x5F00A5      CText* t = (CText*)menu->FindFirstChildOfTypeNonRecursive((EType)6);   // slot 97 (+0x184)
              if (t) t->SetValue(t->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(attackCount), true, true);
                 // eval: GFIntToWideString [+0x3C], GetXString 0x5CC772 [+0x44], GetValue slot 142, op+ [+0x40], op+ [+0x4C], SetValue slot 144
0x5F011F      CText* s = (CText*)menu->FindChild("UI_ATTACK_SUBTITLE");
              if (s) s->SetValue(s->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(attackCount), true, true);
0x5F01A0   } else {    // ~children first, then jmp 0x5F0213
0x5F01AA      CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_ATTACK, root);
              menu->MakeUnavailable();                                       // slot 92 (+0x170)
              CComponent* arrow = menu->FindChild("UI_SELECTION_ARROW_RIGHT");
              if (arrow) { NUISystem::CDeletion d; /*0x42BED4 [ebp+0x50]*/ d.Method = 1 /*UI_DELETE_THIS_INSTANCE*/;
                           arrow->SetDeletion(d); /* by value: copy ctor 0x42CD84, slot 62 +0xF8 */ }   // ~d = ~list 0x42ABCA on [ebp+0x54]
         } }
0x5F0213 same for SPELL_CATEGORY_SURROUND (1) with surroundCount and "UI_SURROUND_SUBTITLE"
0x5F03CB same for SPELL_CATEGORY_PHYSICAL (2) with physicalCount and "UI_PHYSICAL_SUBTITLE"  (`this` moves esi->... note physical block uses esi for menu)
0x5F0571 return root;  dtors in order: set, show, exclusive (0x429160), actions, reordered (inline free), indication, selection,
         container, palette (0x4291DE), misc (inline, NO null check: `dec [ecx+4]; jne; call [vtbl+4]`), owned (~_List_base 0x5F8908)
0x5F05D8 mov eax,[ebp+0x78] ; epilogue ; ret 8
```
No jump tables. No cold blocks after the epilogue (function ends at 0x5F05E4; OpenInventory 0x5F05E5 follows).

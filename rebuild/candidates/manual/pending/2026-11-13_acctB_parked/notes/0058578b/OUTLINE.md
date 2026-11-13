# OUTLINE 0x0058578B CTCInventory::OpenInventory()  (read with $W/dis_annot.txt; every call named there)

## Frame
- `push ebp; lea ebp,[esp-78h]; sub esp,0F4h; and [ebp+64h],0; push ebx; push esi; mov ebx,ecx; push edi; mov [ebp-14h],ebx`
  epilogue `pop edi; pop esi; pop ebx; add ebp,78h; leave; ret`. No SEH / no /GS / no EH states. Pragmas y-off + s-on.
- Saved order ebx, esi, edi. this: ebx then [ebp-14h]. top_level component: edi + [ebp-78h].
## Frame map (ebp-relative; slots are reused across scopes)
| slot | local |
|---|---|
| -78h | CComponent* top_level ("UI_TOP_LEVEL_MENU"); &it pushed to Components.push_back at the end |
| -74h | CCountedPointer<CComponent> scroll_viewport ("UI_SCROLLING_VIEWPORT_TEXT_AREA"); raw copy in [ebp+64h] after |
| -64h | CCountedPointer<CComponent> mini_map ("UI_MAIN_MENU_MINI_MAP_TEMPLATE") |
| -5Ch | CCountedPointer<CComponent> mini_map_sprite ("UI_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE") |
| -54h / +04h | ring pics (stats branch) "UI_RING_PIC_EXP_STATS" / "UI_RING_PIC_PERSONALITY"; +04h also style-cards action |
| -4Ch,-24h,-2Ch,-44h,-3Ch | CCountedPointer<CAction> back actions (A..E, below) |
| -34h | CCountedPointer<CComponent> desc_container ("UI_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_FOR_LIST") |
| -1Ch | CWideString (default-constructed at 0x00585C8F, dtor at end) -- earlier same slot read as uninitialised CTCHeroStats* |
| -18h | CDefPointer<const NUISystem::CUIMiscThingsDef> misc_def (=0, GetDef fills) |
| -14h | this ; -10h clothing TC (TCI_INVENTORY_CLOTHING 0x12) |
| +4Ch weapons TC (0x13), +34h inventory TC (0x11, after the gold block), +30h map TC (0x1B), +14h abilities TC (0x6F), +50h stats TC (0x14) | all init 0 at 0x005859C9 (order of stores: 4C,34,30,14,50,-10) |
| +3Ch | CManager* manager = GetInstance() |
| +1Ch | &QuestManager->ActiveQuestCards (list head ptr); later counter |
| +20h..+28h | std::vector<std::pair<long,long>> items (freed with free() at 0x005876AD) |
| +30h | loop index i ; +2Ch weapons count ; +18h items count ; +44h clothing count ; +10h, +1Ch, +40h style counts ; +00h style total |
| +64h | conditional-destruct flags (quest loops) / later raw scroll_viewport ptr |
| +40h,+68h | quest-card CDefPointer copies (loops) / +68h later CWideString text |
| +5Ch(+60h) | scratch CCountedPointer<CComponent>/<CAction>, CDeletion (Method +5Ch, list +60h) |
| +70h,+74h,+0Ch | CWideString / CCharString temporaries |

## Body (retail order)
1. 0x005857A2 `NUISystem::CManager::OnOpenMenus()`(static). 0x005857A9 `GetPlayerManager()` -> 0x00449980 (index) ->
   `[0x013B8388]->+2Ch[idx*8]` ->vslot 0x48 (). 
2. 0x005857C6 if ([0x013B8394]) three calls vslot 0xB8 (0x10 / 0x40 / 0x80, 0.0f, [0x01237584]) -- args pushed as (id, 0.0f, k).
3. 0x00585830 `[[0x013B86A0]+28h]+2Ch` ->vslot 0x130 (0).
4. 0x00585845 `CPlayerGui([0x013B8790]).GetPHeroInfoScreensManager()->SetCurrentScreenType(this->CBase::GetScreenType())`.
5. 0x00585865 `NInventory::CTCInventoryBase::OpenInventory();`
6. 0x0058586C `SetInventoryAnimRenderBox(C2DBoxF(GetPDef()->[+A4h], GetPDef()->[+ACh]));` (two copies esi,edi; release edi then esi).
7. 0x005858CD `manager = CManager::GetInstance();` then `GetInstance()->vslot 0x74()`, `GetInstance()->vslot 0x54(this)`,
   `GetInstance()->vslot 0x68(ParentThing)` (FrontEnd notifications).
8. 0x005858FD `top_level = GetInstance()->CreateComponent("UI_TOP_LEVEL_MENU", false);`
9. 0x0058592B `if (CComponent* gold = top_level->FindChild("UI_GOLD_AMOUNT_TOP_LEVEL"))` { hero = MainPlayer.PeekPControlledCreature();
   hero_stats via PeekTC(TCI_HERO_STATS) (else-arm reads the uninitialised pointer [ebp-1Ch]);
   `gold->SetText(GFIntToWideString(hero_stats->Gold(+3Ch)), true, true)` (vslot 0x240) }.
10. 0x005859BF six `ParentThing->GetTC(...)` lookups (expanded, LowerBound 0x0040F020): weapons 0x13->+4Ch, inventory 0x11->+34h,
    map 0x1B->+30h, abilities 0x6F->+14h, stats 0x14->+50h, clothing 0x12->[-10h].
11. 0x00585B2F mini_map = CCountedPointer(GetInstance()->CreateComponent("UI_MAIN_MENU_MINI_MAP_TEMPLATE",false));
    mini_map_sprite = CCountedPointer(manager->CreateComponent("UI_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE",false));
    0x00585B8F `if (mini_map_sprite->vslot 0x104() == 0)` (GetType) `sprite->vslot 0x244(GetMiniMapGraphic(GetWorldMap().GetRegion(
    GetWorldMap().GetRegionNumberMapIsIn(ParentThing->MapNumber(+8Eh short)))))`.
    0x00585BE0 `if (map_tc) map_tc->CreateMiniMapMarkers(mini_map.raw, region same expr)`.
    0x00585C1B `mini_map->AddChild(mini_map_sprite)` (vslot 0xEC); `top_level->FindChild("UI_MINI_MAP_VIEWPORT")->AddChild(mini_map)`.
12. 0x00585C4E desc_container = CCountedPointer(GetInstance()->CreateComponent("UI_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_FOR_LIST",false)).
13. 0x00585C80 quests = &CQuestManager::Get()->ActiveQuestCards ([0x013B89FC]+5Ch); CWideString w(-1Ch); misc_def:
    `GetDef(GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1), &misc_def)`.
14. Loop 1 0x00585CDF..0x00585DD1 (for it in quests, list walk `it=*it`): if IS_THING_ALIVE(*it) { card = PeekTC<CTCQuestCard>(GetPItem)
    (0x6C, const LowerBound); bool show = card->IsCore() || (card->IsOptional() && !card->GetPDef()->IsVignette); flags bits 1/2;
    if (show) goto LIST (0x00585ED9) }. Falls through when none:
15. 0x00585DD7 NO QUESTS: text = CCountedPointer(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE",false));
    desc_container->AddChild(text); text->SetText(TextBank.GetTextBySymbol("TEXT_GUI_MENU_CHECKGUILD"), true,true)
    (bank = [[0x013B86A0]+14h]); release text. -> 0x00585E62.
    LIST 0x00585ED9 loop 2 over quests (same test, flags 4/8, card kept in [ebp+44h]): if shown {
      entry(+5Ch) = Reset(manager->CreateComponent(IsCore() ? "UI_CORE_ICON_AND_NAME" : "UI_OPTIONAL_ICON_AND_NAME", false));
      if (name = entry->FindChild("UI_QUEST_NAME")) { bool empty = (card->GetQuestObjective() == L"");
        name->SetText(card->GetQuestName() + misc_def->W44 + misc_def->W3C + (empty ? card->GetQuestSummary() : card->GetQuestObjective()), true, true);
        desc_container->AddChild(entry); }  release entry }  -> after loop jmp 0x00585E62.
16. 0x00585E62 scroll_viewport = CCountedPointer(manager->CreateComponent("UI_SCROLLING_VIEWPORT_TEXT_AREA",false));
    scroll_viewport->vslot 0x238(desc_container.raw) (AddFakeChild); top_level->vslot 0x244(desc_container); top_level->vslot 0x244(scroll_viewport).
17. 0x00585EBD..0x0058647C five back actions, each `CCountedPointer<CAction> a(new CAction);` (cold `xor eax,eax` null arm at 0x005861D4):
    A(-4Ch) type 3 CHANGESTATE params {top_level->FindChild("UI_TOP_LEVEL_MENU_SCREEN") as CActionParam* (null-safe +18h), new CActionParamState(2)};
    B(-24h) same with "UI_TABLE_TEXT_LEFT"; C(-2Ch) "UI_THUMB_STICK_UP_DOWN"; D(-44h) "UI_BOTTOM_BACKDROP";
    E(-3Ch) type 0xD4 CHANGE_ALPHA params {new CActionParamSLONG(3)? (vptr 0x012366F4, [+4]=3? check: `mov [eax+4],esi(=3)`), top_level->FindChild("UI_SCREEN_BACKDROP")}.
    CActionParamState ctor inline: `[+8]=0,[+4]=2,vptr` ; SLONG: `[+4]=3, vptr, [+8]=value`.
18. 0x00586481 MAP entry: entry=top_level->FindChild("UI_MENU_ENTRY_MAP"); if (map_tc==0) skip to 0x0058669F (SKILLS) -- NOTE map gates QUESTS+LOGBOOK too.
    if (MainPlayer.GetPControlledCreature()->IsChild()) { entry->MakeUnavailable() (0x170); arrow=entry->FindChild("UI_SELECTION_ARROW_RIGHT"); if(arrow){CDeletion d; d.Method=1; arrow->SetDeletion(d);} }
    else { act(new CAction) type 0x49 CONSTRUCT_MAP_LIST params {entry, top_level}; entry->AddPreAction(act) (0x114) }
    0x00586594 QUESTS: entry=FindChild("UI_MENU_ENTRY_QUESTS"); act 0x48 {entry, top_level}; AddPreAction.
    0x0058661D LOGBOOK: if (entry=FindChild("UI_MENU_ENTRY_LOGBOOK")) act 0xE4 {entry (no null test), top_level}.
19. 0x0058669F SKILLS: entry=FindChild("UI_MENU_ENTRY_SKILLS"); if (abilities_tc) act 0x47 {entry, top_level} else if(entry){MakeUnavailable; arrow deletion}.
20. 0x00586788 counts: hero=PeekPControlledCreature(); vector<pair<long,long>> items; this->GetAllItems(items);
    erase IsBook entries (0x005867C2 loop, erase out of line); weapons=hero PeekTC(0x13) (const, default 0) -> weapons->GetAllItems(items);
    if (clothing_tc) clothing_tc->GetAllItems(items).
    for (i=0; i<items.size(); ++i) (guarded `je 0x00586ACE` on size 0 = `if(size)`+loop form): def=GetDefinitionManager().GetDef<CThingBaseDef>(items[i].first);
      CDefPointer aug; if (def->GetSubDef(&aug)) {skip} else { suit=0, appm=0;
        if (IsWeapon(first)) weapons_n += second;
        else if (def->GetSubDef<CHeroSuitDef>(&suit)) { for (set it over suit->+28h) if (clothing_tc->GetNumberOfItemsOfTypeInInventory(*it) > 0) { clothing_n += second; break; } }
        else if (def->GetSubDef<CAppearanceModifierDef>(&appm)) { if (appm->+4Ch in 1..5) clothing_n += second; }
        else if (!IsExpression(first)) { hair=GetHairCardDef(first); tattoo=GetTattooCardDef(first);
             if (!hair) { if (!tattoo) items_n += second; else tattoo_n(+40h) += second; }
             else { CDefPointer d = GetPDefFromGlobalIndex(hair->+28h); if (d) { am=0; d->GetSubDef<CAppearanceModifierDef>(&am); if (am) { f=am->+40h; if (f&0x80 || f&0x100) n1C+=second; else if (f&0x10) n10+=second; } } }
        } }  (releases in reverse: d, tattoo, hair, appm, suit, aug, def).
21. 0x00586ACE WEAPONS entry (template for ITEMS / CLOTHING / STYLE): e=FindChild("UI_MENU_ENTRY_WEAPONS"); viewport(+64h)->AddFakeChild(e->FindChild("UI_WEAPON_DESC"));
    if (weapons_tc && weapons_n > 0) { b=e->FindChild("UI_WEAPONS_BUTTON"); t=b->FindChild("UI_WEAPONS_TEXT"); CWideString s(t->vslot 0x238 /*GetValue*/());
      s += CTCInventoryStats::GetXString(); s += GFIntToWideString(weapons_n); t->SetText(s,true,true);
      if (sub=e->FindChild("UI_WEAPON_SUB_TITLE")) sub->SetText(s,true,true); act 0x44 {e (no null test), top_level}; e->AddPreAction(act); }
    else { e->MakeUnavailable(); arrow deletion }.
    0x00586C9B ITEMS (inventory_tc +34h, items_n +18h, act 0x46; else-arm: arrow FIRST then MakeUnavailable at 0x00586E66).
    0x00586E6C CLOTHING (clothing_tc -10h, clothing_n +44h, act 0x45; else MakeUnavailable then arrow).
    0x00587039 STYLE CARDS: total(+00h) = n1C + tattoo_n + n10; if (inventory_tc && total>0) {... act 0x11E with 2 component params
      + three CActionParamSLONG(n10), (n1C?), (tattoo_n?) -- check order at 0x005871A0..0x00587237 } else MakeUnavailable only (no arrow).
22. 0x00587266 if (stats_tc) { st=FindChild("UI_MENU_ENTRY_STATS"); ex=FindChild("UI_MENU_ENTRY_EXPERIENCE"); pe=FindChild("UI_MENU_ENTRY_PERSONALITY");
      if (d=pe->FindChild("UI_PERSONALITY_DESC")) viewport->AddFakeChild(d); if (d=ex->FindChild("UI_EXPERIENCE_DESC")) viewport->AddFakeChild(d);
      ring1(-54h)=CP(GetInstance()->CreateComponent("UI_RING_PIC_EXP_STATS")); top_level?->AddChild(ring1); ring2(+04h) "UI_RING_PIC_PERSONALITY" AddChild;
      act(-4h) 0x4C {ex, top_level}; ex->AddPreAction; ex->AddAction(A),(B),(C),(D) via 0x108;
      act(+5Ch) 0x4D {pe, top_level}; pe->AddPreAction; pe->AddAction(?) 0x108 with -4Ch ... (see 0x0058747E..) ; releases }
    else (0x005874AC) { ex, pe found; arrow on pe; descs; ring pics; deletions on arrows; MakeUnavailable(ex/pe) }  join 0x00587659.
23. 0x0058765E `Components.push_back(top_level)`; for (i<Components.size()) { Components[i]->vslot 0xAC(); Components[i]->vslot 0xC0(5); }
24. Epilogue dtors 0x005876A4: items storage free; E,D,C,B,A ~CCountedPointer<CAction>; scroll_viewport; misc_def release (inline);
    CWideString -1Ch; desc_container; mini_map_sprite; mini_map.
Cold / out-of-order: every `new CAction` null arm (`xor eax,eax`) sits inline after its jmp; no blocks after the ret; no jump tables.

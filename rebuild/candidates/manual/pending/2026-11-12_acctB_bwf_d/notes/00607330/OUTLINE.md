# OUTLINE 0x00607330 CTCInventoryMap::ConstructLogbookList(NUISystem::CComponent* parent) -- retail order

## Frame
push ebp; mov ebp,esp; and esp,-8; sub esp,10Ch; push ebx,esi,edi (that order); no SEH, no /GS cookie; epilogue
pop edi,esi,ebx; mov esp,ebp; pop ebp; ret 4. parent = [ebp+8]. Slots (esp after pushes; many reused by the compiler):
- [54h] this (spill) | [24h] manager = CManager::GetInstance() | [88h] categories (return value) | [D8h] viewport
- [98h..A0h] std::map<ECategory, vector<CHeroLogBookEntry>> log_entries (copy; [98h] header node, [9Ch] count)
- [44h] num_basics [48h] num_objects [2Ch] num_towns [34h] num_hero [3Ch] num_combat [30h] num_quests [64h] num_story (zeroed in that
  order = declaration order) | [43h] bool has_inventory (bit 17 of thing+0x20)
- [A4h..ACh] std::vector<long> books | [70h..78h] std::vector<std::pair<long,long> > items (inside the inventory if)
- [80h]/[84h] CCountedPointer<CComponent> books_list (UI_SUB_LIST; ptr,data) | [38h] books_list as CList* (`mov [esp+3Ch],ecx` @0x006078AF)
- book loop: [58h] iterator, [7Ch] book id, [8Ch] CWideString description, [5Ch]/[60h] CCountedPointer entry, [0Ch]/[10h] mesh,
  [74h] copied pointer, [28h] CDefPointer def, [B0h] CDefPointer<const CAppearanceDef>, [E4h] CRightHandedSet (GetStartingRightHandedSet),
  [4Ch]/[50h] description text, [E0h] copy, [20h]/[1Ch]/[6Ch]/[68h]/[18h]/[14h] name/sprite pointers
- category blocks: CCharString temps at [44h]/[28h]..., title/back/ring CCountedPointers at [1Ch/20h], [14h/18h], [0Ch/10h], key [38h].

## Body
0x00607330  manager = CManager::GetInstance();                     // esi -> [24h]
0x0060734A  std::map<ECategory, std::vector<CHeroLogBookEntry> > log_entries(
               CMainGameComponent::PMainGameComponent->PPlayerManager(+1C)->GetMainPlayer().PHeroLogBook->LogEntries);  // 0x00619C50 copy
0x00607371  CComponent* categories = manager->CreateComponent(CCharString("UI_LOGBOOK_CATEGORIES"), false);   // [88h]
0x006073AB  CComponent* viewport = categories->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");   // [D8h]
0x006073CC  if (viewport) {   // 8 x: if (CComponent* d = categories->FindChild("UI_LOGBOOK_<X>_DESC")) viewport->AddFakeChild(d);
               order BASICS 0x006073D2, OBJECTS, TOWNS, HERO, COMBAT, QUESTS, STORY, BOOKS 0x006074D5 (slot +0x238) }
0x006074FC  CategoriesList.clear();   // expanded: if(size) {_M_erase 0x00613AE0(root); header reset; size=0}
0x0060752F  long num_basics=0,num_objects=0,num_towns=0,num_hero=0,num_combat=0,num_quests=0,num_story=0;
            for (it = log_entries.begin(); it != log_entries.end(); ++it)   // loop head 0x00607560, ++it expanded 0x00607649..0x00607684
              switch (it->first) {      // jump table 0x006096B4
                case CATEGORY_BASICS:  num_basics  = it->second.size(); break;  // 0x00607573
                case CATEGORY_OBJECTS: num_objects = ...; break;                // 0x00607594
                case CATEGORY_TOWNS:   num_towns   = ...; break;                // 0x006075B5
                case CATEGORY_HERO:    num_hero    = ...; break;                // 0x006075D3
                case CATEGORY_COMBAT:  num_combat  = ...; break;                // 0x006075F1
                case CATEGORY_QUEST:   num_quests  = ...; break;                // 0x0060760F
                case CATEGORY_STORY:   num_story   = ...; break;                // 0x0060762D
              }                                                                 // TUTORIAL / default -> 0x00607649
0x0060768C  std::vector<long> books;
            if (thing has TC 17 (bit test thing+0x20 >> 17)) {                 // has_inventory [43h]; je 0x00607776
0x006076BD    inventory = GetTC(17) (expanded LowerBound 0x0040F020 on thing+0x44);
0x006076E2    std::vector<std::pair<long,long> > items; inventory->GetAllItems(items);
0x00607707    for (each item) if (CTCInventoryItem::IsBook(item.first)) books.push_back(item.first);  // _M_insert_overflow 0x00440990
0x0060775C    } // items freed
0x00607769  if (books.empty()) {                                              // jne 0x0060782A (books non-empty skips)
0x00607776    if (CComponent* entry = categories->FindChild("UI_MENU_ENTRY_LOGBOOK_BOOKS")) {
0x00607796      entry->MakeUnavailable();                                     // +0x170
0x006077A0      if (CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT")) {
                  CDeletion deletion; deletion.Method = UI_DELETE_THIS_INSTANCE; arrow->SetDeletion(deletion);  // +0xF8, ~list 0x0042ABCA
            } } }
0x0060782A  CCountedPointer<CComponent> books_list(CManager::GetInstance()->CreateComponent("UI_SUB_LIST", false));  // [80h]/[84h]
0x00607897  books_list(as CList)->InitialiseOffsets(books.size());          // +0x270
0x006078B9  for (it = books.begin(); it != books.end(); ++it) {             // body 0x006078D0, back-edge 0x0060801E
              long book = *it;
              CWideString description = CTCInventoryItem::GetDescription(book);
0x006078E6    CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(description, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
0x006079AF    CCountedPointer<CComponent> mesh(manager->CreateComponent("UI_ITEM_MODEL_TEMPLATE_NEW", false));  // copy taken at [74h] (inc count)
0x00607A2D    CDefPointer<..> def; if (book > 0) def = GetDefinitionManager()->GetPDefFromGlobalIndex(book);
0x00607A8A    mesh->SetMeshIndex(def->Graphic.BankIndex);  // +0x23C, def+0x70
0x00607A98    mesh->SetOrientation(CTCInventoryItem::GetStartingRightHandedSet(book));  // +0x250
0x00607ABA    if (def->Graphic.Type == 4) { CDefPointer<const CAppearanceDef> app; def->GetSubDef(&app);
                mesh->SetAnimationIndex(app->AnimationSet(+0x34).GetAnim("DEFAULT", 0, CAnimFilterNoGroup()).<+0>);  // +0x248
                mesh->SetMeshType(4); }                                    // +0x24C; app released
0x00607B5D    entry->AddChild(mesh);                                         // +0xEC
0x00607B6E    CCountedPointer<CComponent> desc_text(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", false));
0x00607BF6    desc_text->SetValue(CTCInventoryItem::GetDetails(book), true, true);  // +0x240
0x00607C21    entry->AddChild(desc_text);
0x00607C32    if (CComponent* v = parent->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA")) v->AddFakeChild(desc_text.Get());   // +0x238
0x00607C60    if (CTCInventoryItem::GetEngineGraphic(book).BankIndex != 0) {   // == 0 -> 0x00607E3A
0x00607C7A      name = CreateComponent("UI_TEXT_ITEM_NAME_TEMPLATE_NEW"); name->SetValue(description,..) (+0x240); entry->AddChild(name);
0x00607D1B      sprite = CreateComponent("UI_ITEM_SPRITE_TEMPLATE");
0x00607D7C      if (sprite->GetType() == 0 /*UI_TYPE_SPRITE*/) sprite->SetGraphicIndex(GetEngineGraphic(book).BankIndex); // +0x104, +0x244
0x00607DB3      entry->AddChild(sprite); }
              else {                                                          // 0x00607E3A
                name = CreateComponent("UI_TEXT_WEAPONS_NAME_TEMPLATE_NEW"); name->SetValue(description,..); entry->AddChild(name); }
0x00607F1D    books_list->AddScrollingChildAndSetOffsets(entry);            // +0x260
            } // dtors 0x00607F30..0x00608005 (reverse declaration order)
0x00608024  title = CreateComponent("UI_BOOKS_CATEGORIES_TITLE"); books_list->AddNonScrollingChild(title);   // +0x244
0x00608095  back  = CreateComponent("UI_BACK_FOR_LIST");          books_list->AddNonScrollingChild(back);
0x0060810A  if (CComponent* e = categories->FindChild("UI_MENU_ENTRY_LOGBOOK_BOOKS")) e->AddChild(books_list);  // +0xEC
0x00608181..0x00609058  seven identical blocks, order BASICS(3) 0x00608181, OBJECTS(4) 0x006083A2, TOWNS(5) 0x006085C1, HERO(6),
            COMBAT(7), QUESTS(0) (key stored from ebx=0 at 0x00608DAF), STORY(1) 0x00608E5x:
              if (num_X) { CList* list = (CList*)manager->CreateComponent("UI_SUB_LIST", false);   // RAW pointer, no CCountedPointer
                           list->InitialiseOffsets(num_X);
                           CCountedPointer<CComponent> title(CreateComponent("UI_X_CATEGORIES_TITLE")); list->AddNonScrollingChild(title);
                           CCountedPointer<CComponent> back(CreateComponent("UI_BACK_FOR_LIST"));       list->AddNonScrollingChild(back);
                           CCountedPointer<CComponent> ring(CreateComponent("UI_RING_PIC_LOGBOOK_X_FOR_LIST")); list->AddNonScrollingChild(ring);
                           CategoriesList[CATEGORY_X] = list; }        // operator[] 0x00618BE0, key temp [38h]
0x00609058  for (it = log_entries.begin(); it != log_entries.end(); ++it) {       // head 0x00609070, ++ 0x00609410
              CList* list = (CList*)CategoriesList[it->first];
              list->SetDefaultChild((long)it->second.size() - 1);   // +0x278 (verify slot name)
              for (unsigned long i = 0; i < it->second.size(); ++i) {  // body 0x006090E0, [2Ch] i, [58h] byte offset i*12
                CHeroLogBookEntry entry = it->second[i];              // 3 CWideString copies [74h]/[78h]/[7Ch] (+ one more copy [38h] of the first)
                CCountedPointer<CComponent> menu_entry(manager->CreateMenuEntryComponent(<copy of entry name>, 0,1,"UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",0,"",0,"PC_BUTTON_BASTARD"));
                CCountedPointer<CComponent> desc(CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW"));
                viewport->AddFakeChild(desc.Get());                    // [D8h] +0x238 (0x00609212)
                desc->SetValue(entry.<text>, true, true); menu_entry->AddChild(desc);
                CCountedPointer<CComponent> name(CreateComponent("UI_TEXT_WEAPONS_NAME_TEMPLATE_NEW"));
                name->SetValue(entry.<name>, true, true); menu_entry->AddChild(name);
                list->AddScrollingChildAndSetOffsets(menu_entry);      // +0x260
              } }
0x00609451  for (long cat = 0; cat <= 7; ++cat) {                       // [24h]; ++ 0x006095E7
              if (cat == CATEGORY_TUTORIAL) continue;                  // cmp 2 -> 0x006095E7
              it = CategoriesList.find(cat);                           // EXPANDED tree find 0x00609460..0x0060949E
              if (it != end) { CCountedPointer<CComponent> p(it->second);   // 0x006094AA
                               GetMenuEntryFromCategory(cat, categories)->AddChild(p); }   // 0x006094EC
              else { CComponent* e = GetMenuEntryFromCategory(cat, categories);         // 0x00609543
                     e->MakeUnavailable();
                     if (CComponent* arrow = e->FindChild("UI_SELECTION_ARROW_RIGHT")) { CDeletion d; d.Method = 1; arrow->SetDeletion(d); } }
            }
0x006095F9  ~books_list; ~books (free); ~log_entries (_M_erase 0x0047C6A0 + header free)
0x006096A4  return categories;
NOTE the GetMenuEntryFromCategory argument ORDER in retail: (cat, categories) with ecx=this.
NOTE CHeroLogBookEntry field order (which CWideString is name / text / abbreviated) must come from CHeroLogBook.h + 0x0060910E..0x0060927x.

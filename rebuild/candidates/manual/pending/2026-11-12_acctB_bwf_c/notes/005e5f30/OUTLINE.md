# OUTLINE 0x005E5F30 void CTCQuestCompletionUI::ActivateReal()  (retail order; addresses = block starts / calls)

## Frame facts
- `push ebp; mov ebp,esp; and esp,-8; sub esp,0x8c; mov eax,__security_cookie; push ebx; push esi; push edi; mov [esp+0x94],eax`
  -> /GS cookie at esp+0x94, saved ebx/esi/edi (in that push order), aligned frame of 0x8c bytes. No EH states (no /EHsc). Plain `ret`.
- Epilogue 0x5E78A0: `mov ecx,[esp+0x94]; call __security_check_cookie; pop edi; pop esi; pop ebx; mov esp,ebp; pop ebp; ret`.
- esi = this. ebx = 0 until 0x5E6334, then pmenu (spilled to esp+0x28).

## Frame map (esp offsets after the 3 pushes; scoped temporaries share slots)
| slot | holds |
|---|---|
| +0x0C | CCharString temporaries (CreateComponent / GetDef names); CWideString `text` of the bars; CDefPointer<const CThingObjectDef> pobjdef (0x5E71AD, set 0); CAnimFilterNoGroup filter (0x5E723B, vptr 0x122FFDC -> 0x122FFCC at its end) |
| +0x10 | bool byte: materialised HasTC result (0x5E6130...); the event bool (=1, 0x5E75EE); CCharString temps in some cases |
| +0x14 | CCharString / CWideString temps; the map iterator from find() (0x5E68B6); pviewport (description viewport, 0x5E7064) |
| +0x18/+0x1C | CCountedPointer<NUISystem::CComponent> {Data, Info}: phelpers, then the bar pointers, pmodel, psprite; also CDeletion {Method, AssociatedParents} (0x5E6FCE) |
| +0x20 | LowerBound key temp (4 / 0x68 / 0x1B); the (long) ftol result; picon in the death path; the CDefPointer<CUIMiscThingsDef> temp of the sound blocks; pcursor |
| +0x24 | Info of the pmodel copy (0x5E718F, refcount++), helper Data spill |
| +0x28 | pmenu (the PC_QUEST_COMPLETION_MENU_* component) |
| +0x2C | rTextBank (const NGameText::CDataBank&, 0x5E6452), CDefPointer<const CAppearanceDef> papp (0x5E720B) |
| +0x30 | pmanager = NUISystem::CManager::GetInstance() (0x5E621C) |
| +0x34 | pscreen (FindChild result, 0x5E6496) |
| +0x38/+0x3C | CCountedPointer pminimap (0x5E7352, out-of-line ctor); also CCharString temps ("DEFAULT", "UI_MOUSE_POINTER") |
| +0x40 | CDefPointer<const NUISystem::CUIMiscThingsDef> pmiscthingsdef (=0 at 0x5E6418, released at 0x5E7886) |
| +0x44/+0x48 | CCountedPointer<CComponent> pcontainer (={0,0} at 0x5E7082, Reset at 0x5E70D1, released at 0x5E785F) |
| +0x4C..+0x6B | CSoundInit init (x3) / CRightHandedSet rhs (hidden return of GetStartingRightHandedSet, 0x5E71E2) |
| +0x6C..+0x93 | CGameEvent ev(9) |
| +0x94 | /GS cookie |

## Body
```
0x5E5F4D CUserProfileManager::Get().CaptureBackBufferForUIBackground();
0x5E5F59 AccumFadeTime = 0;                                         // [esi+0xb4] = ebx(0)
0x5E5F61 CMainGameComponent::Get()->GetDisplayEngine().Engine3D->vslot76(+0x130)(0);   // [[[13b86a0]+0x28]+0x2c]
0x5E5F75 if (PSoundManager && Completion != 0 && Completion != 1) {    // 0x13B8394
           PSoundManager->SetGainFader(0x10, 0.0f, 500.0f); (0x40 ...); (0x80 ...);   // slot 46, 0x43fa0000 = 500.0f, ebx = 0
         }
0x5E5FCA ExperienceCountingSound = GoldCountingSound = RenownCountingSound = 0;   // +0xc8 +0xcc +0xd0 (stored between cmp and ja)
0x5E5FE2 switch (Completion) {  // T1 0x5E78B4; each case makes its own CDefPointer temp (slots 0x34, 0x0c, 0x38, 0x14, 0x2c)
  case 0 (0x5E5FEF): music = NUISystem::CManager::GetInstance()->GetMiscThingsDef()->QuestStartScreenMusic;    // +0x204
  case 1 (0x5E6014): ... ->QuestCompleteScreenMusic (+0x208);   case 2 (0x5E6036): QuestFailureScreenMusic (+0x20c);
  case 3 (0x5E6058): DeathScreenMusic (+0x210);                 default (0x5E607A): QuestStartScreenMusic (+0x204)
}  // edi = music; common temp-dtor tail 0x5E609A (CDefPointer release: if(p){if(--p->RefCount==0) p->vslot1()})
0x5E60B2 if (Completion != 0 && Completion != 1) {
           GetWorld().GetMusicManager().ClearUIOverridingMusicSet();   // 0x686A70 GetWorld, [+0x6c], 0x6BE030
           GetWorld().GetMusicManager().SetUIOverridingMusicSet(music); // 0x6BDFE0
         }
0x5E60DF InitialGold = TargetGoldGained = GoldGainedDisplayed = InitialExperience = TargetExperienceGained = ExperienceGainedDisplayed
         = InitialRenown = TargetRenownGained = RenownGainedDisplayed = 0; PExperienceText = PRenownText = PGoldText = 0;
         // store order: +64 +60 +80 +6c +68 +78 +74 +70 +7c +b0 +a8 +ac (all ebx)
0x5E6112 if (Completion == 0 && QuestCardDef != 0) {      // StartQuest snapshot, compare SaveInitialValues 0x5E4860
  0x5E6124 if (Thing->GetTC(TCI_HERO_STATS,&stats)) StartQuestRenown[QuestCardDef] = stats->RenownTotal;         // +0x7c, map +0x9c, operator[] 0x5E9760
  0x5E6175 if (Thing->GetTC(TCI_HERO_STATS,&stats)) StartQuestGold[QuestCardDef] = stats->TotalMoneyEarned;      // +0x48, map +0x90
  0x5E61C6 if (Thing->GetTC(TCI_HERO_EXPERIENCE,&exp)) StartQuestExperience[QuestCardDef] = exp->TotalExperiencePoints; // 0x68, +0x10, map +0x84
}  // each: HasTC byte to [esp+0x10]; key 4/0x68 to [esp+0x34] after the test; LowerBound 0x40F020; value in edi before the operator[] call
0x5E6217 pmanager = NUISystem::CManager::GetInstance();          // [esp+0x30]
0x5E6220 switch (Completion) {  // T2 0x5E78C4; menu = pmanager->CreateComponent(menuName,false), then PFrame = menu->FindChild(frameName)
  case 0: "PC_QUEST_COMPLETION_MENU_START" / "PC_UI_QUEST_START_FRAME"
  case 1: "PC_QUEST_COMPLETION_MENU_COMPLETE" / "PC_UI_QUEST_FINISHED_FRAME"
  case 2: "PC_QUEST_COMPLETION_MENU_FAIL" / "PC_UI_QUEST_FAILED_FRAME"
  case 3: "PC_QUEST_COMPLETION_MENU_DEATH" / "PC_UI_QUEST_DEATH_FRAME"
  default (0x5E6310): "PC_QUEST_COMPLETION_MENU_START" / "PC_UI_QUEST_START_FRAME"
}  // tail 0x5E6334/0x5E6349: pmenu(ebx,[esp+0x28]) = result; ~name; PFrame(+0xb8) = pmenu->FindChild(CCharString(frameName))
0x5E635B if (Completion != 2 && Completion != 3) {
  0x5E6368 p = pmenu->FindChild("SCROLL_QUEST_COMPLETION_DESCRIPTION_UP");
           if (p) { NUISystem::CManager::GetInstance()->AddObserver(p /*CObserver base +4*/); p->ObserveEvent((EEvent)0x19); }
  0x5E639B p = pmenu->FindChild("SCROLL_QUEST_COMPLETION_DESCRIPTION_DOWN"); -> shared tail 0x5E63E4 (same observe code)
} else {
  0x5E63A7 "SCROLL_DESCRIPTION_UP" (same), 0x5E63DA "SCROLL_DESCRIPTION_DOWN" -> tail 0x5E63E4
}
0x5E640D CDefPointer<const NUISystem::CUIMiscThingsDef> pmiscthingsdef;   // [esp+0x40] = 0
         long idx = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);  // 0x44C6B0, 0x9AD390
0x5E6443 const NGameText::CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();  // [+0x14] -> [esp+0x2c]
         CGameDefinitionManager::Get().GetDef(idx, &pmiscthingsdef);    // 0x42B312   (same opening as GetQuestDescription)
0x5E6462 pscreen = pmenu->FindChild((Completion != 0 && Completion != 1) ? "PC_QUEST_COMPLETION_MENU_SCREEN"
                                                                        : "PC_QUEST_COMPLETION_MENU_SCREEN_START_SUCCESS");  // [esp+0x34]
0x5E6494 if (pscreen) {
  CCountedPointer<NUISystem::CComponent> phelpers;  // {0,0} at 0x18/0x1c
  switch (Completion) {   // T3 0x5E78D4, no default (ja -> 0x5E6692)
   case 0: case 1 (0x5E64BB): phelpers.Reset(pmanager->CreateComponent("PC_START_SUCCESS_HELPERS", false)); pscreen->AddChild(phelpers);
           if (PFrame) PFrame->AddPositionChild(phelpers.Get());
           if (h = phelpers->FindChild("PC_CONTINUE_HELPER")) h->ObserveEvent((EEvent)0x19);  break;   // tail 0x5E6658
   case 2 (0x5E6536): "PC_FAILURE_HELPERS_NO_TELEPORT", then FindChild "PC_RESPAWN_HELPER" + observe; break;
   case 3 (0x5E65B1): "PC_DEATH_HELPERS", FindChild "PC_RESPAWN_HELPER" + observe (in place, 0x5E6623),
           then FindChild "PC_QUIT_HELPER" + observe (tail 0x5E6658);
  }
  0x5E666C ~phelpers (inline release: if(Info){--RefCount; if(0){DeleteFunc(Data); operator delete(Info);}})
}
0x5E6692 ptitle = pmenu->FindChild("PC_TITLE_QUEST_COMPLETION"); if (PFrame) PFrame->AddPositionChild(ptitle);
0x5E66BD if (ptitle) {
  if (Completion == 2) s = "TEXT_GUI_MENU_QUEST_FAILED"; else if (Completion == 0) s = "TEXT_GUI_MENU_QUEST_STARTING";
  else if (Completion == 3) s = "TEXT_GUI_QUESTCARD_FAILURE_TEXT"; else goto skip;
  0x5E6734 ptitle->SetValue(rTextBank.GetTextBySymbol(s), true, true);   // CText slot 144; CWideString temp at 0x20; ~ 0x99B510; ~s
}
0x5E675E if (Completion == 0 || Completion == 1) {          // RENOWN bar
  CCountedPointer<CComponent> pbar(pmanager->CreateComponent("PC_QUEST_COMPLETION_BAR_RENOWN", false));  // ctor INLINE (new 0xc at 0x5E6799)
  ptext = pbar->FindChild("UI_QUEST_COMPLETION_BAR_TEXT");    // edi
  if (PFrame) PFrame->AddPositionChild(pbar.Get());
  pmenu->AddNonScrollingChild(pbar);                        // slot 145 (+0x244)
  0x5E6814 if (PCardDef.Get()) { CWideString text = GFIntToWideString((long)RenownGainedDisplayed); ptext->SetValue(text, true, true); }
  0x5E687A if (ptext) PRenownText = ptext;
  0x5E6884 if (Completion == 0) { if (PCardDef.Get()) TargetRenownGained = RenownReward; }
  else if (QuestCardDef) {
    it = StartQuestRenown.find(QuestCardDef);               // 0x5E8EB0
    if (it != StartQuestRenown.end()) {
      if (Thing->GetTC(TCI_HERO_STATS,&stats)) TargetRenownGained = stats->RenownTotal - it->second;
      InitialRenown = 0; if (TargetRenownGained < 0) TargetRenownGained = 0;
      StartQuestRenown.erase(it);                           // OUT OF LINE 0x5E8F50 (iterator by value)
    } else {
      if (Thing->GetTC(TCI_HERO_STATS,&stats)) TargetRenownGained = stats->RenownTotal;
      InitialRenown = 0; if (TargetRenownGained < 0) TargetRenownGained = 0;
    }
  }
  0x5E6987 ~pbar
}
0x5E69AD if (Completion == 0 || Completion == 1) {          // GOLD bar: same shape with
  "PC_QUEST_COMPLETION_BAR_GOLD", ctor INLINE (0x5E69E6); 0x5E6A63 if (PCardDef) { <empty walk of Boasts: for(n=Boasts.begin();n!=end;++n);>
  text = GFIntToWideString((long)GoldGainedDisplayed /*+0x80*/) ... }  PGoldText (+0xac); Completion 0: TargetGoldGained = GoldReward;
  else StartQuestGold (+0x90) find; found: TargetGoldGained = stats->TotalMoneyEarned - it->second; InitialGold = 0 (edi); clamp >= 0;
  erase EXPANDED (0x5E6B8D: _Rebalance_for_erase(node,&hdr->_M_parent,&hdr->_M_left,&hdr->_M_right); if(y) free(y); --StartQuestGold._M_node_count);
  not found: TargetGoldGained = stats->TotalMoneyEarned; InitialGold = 0; clamp.   0x5E6C0A ~pbar
}
0x5E6C30 if (Completion == 1) {                             // EXPERIENCE bar
  "PC_QUEST_COMPLETION_BAR_EXPERIENCE", ctor INLINE (0x5E6C63); FindChild TEXT; PFrame->AddPositionChild; pmenu->AddNonScrollingChild(pbar);
  0x5E6CDE if (ptext) { PExperienceText = ptext; text = GFIntToWideString((long)ExperienceGainedDisplayed /*+0x78*/);
                        PExperienceText->SetValue(text, true, true); }       // re-reads +0xb0; NO PCardDef test
  0x5E6D45 if (QuestCardDef) { it = StartQuestExperience.find(QuestCardDef);  // ebx = &StartQuestExperience
    found: if (GetTC(TCI_HERO_EXPERIENCE,&exp)) TargetExperienceGained = exp->TotalExperiencePoints - it->second; InitialExperience = 0;
           erase EXPANDED (0x5E6DC2, --[ebx+4]);   // no clamp
    else:  if (GetTC(...)) TargetExperienceGained = exp->TotalExperiencePoints; InitialExperience = 0; }
  0x5E6E33 ~pbar
}
0x5E6E5D psub = pmenu->FindChild("PC_QUEST_COMPLETION_SUBTITLE"); if (PFrame) PFrame->AddPositionChild(psub);
0x5E6E88 if (Completion == 3) psub->SetValue(rTextBank.GetTextBySymbol("TEXT_QST_DEATH_YOU_HAVE_DIED"), true, true);
0x5E6ED8 else if (PCardDef.Get()) psub->SetValue(PCardDef->QuestName /*+0x28*/, CWideString(L"") /*0x122D70C, 0x99B6B0*/, true);  // slot 143
0x5E6F0E picon = pmenu->FindChild("PC_QUEST_ICON");   // string pushed before the Completion==3 branch (common prefix)
  if (Completion != 3) {
    if (PFrame) PFrame->AddPositionChild(picon);
    0x5E6F43 if (PCardDef.Get()) { if (PCardDef->IsCore) picon->SetGraphicIndex(pmiscthingsdef->CoreGraphic);       // +0x58, +0xc0
                                    else if (PCardDef->IsVignette) ...VignetteGraphic (+0x59, +0xc4); else ...OptionalGraphic (+0xc8); }
  } else {  // 0x5E6FA4
    [esp+0x20] = picon; if (PFrame) PFrame->AddPositionChild(picon);
    NUISystem::CDeletion deletion; deletion.Method = UI_DELETE_THIS_INSTANCE;   // implicit ctor: malloc(0xc) sentinel inline
    picon->SetDeletion(deletion);   // by value: arg list copy-built with _M_insert_dispatch 0x42B4C3; slot 62 (+0xf8)
    // ~deletion -> list dtor 0x42ABCA on [esp+0x1c]
  }
0x5E7032 pviewport = pmenu->FindChild((Completion != 0 && Completion != 1) ? "PC_DESCRIPTION_VIEWPORT_QUEST_FAILED"
                                                                            : "PC_DESCRIPTION_VIEWPORT_QUEST_COMPLETION");
         if (pviewport) PFrame->AddPositionChild(pviewport);          // NOTE: no PFrame null test here (0x5E706A)
0x5E707D CCountedPointer<CComponent> pcontainer;                       // [esp+0x44/0x48] = 0
         pcontainer.Reset(pmanager->CreateComponent((Completion != 0 && Completion != 1) ? "PC_TEXT_CONTAINER_DESCRIPTION_TEMPLATE_FOR_LIST"
                                       : "PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_FOR_QUEST_COMPLETION", false));   // Reset 0x429C6F
         if (PFrame) PFrame->AddPositionChild(pcontainer.Get());
         if (pviewport) pviewport->vslot142(+0x238)(pcontainer.Get());     // CViewport::AddFakeChild
         pmenu->AddNonScrollingChild(pcontainer);
0x5E711C GetQuestDescription(pcontainer.Get());                       // 0x5E4BE0
0x5E7128 if (RenderedObjectDef != 0 && Completion != 3) {             // +0x34; Completion==3 -> map block 0x5E72F2
  CCountedPointer<CComponent> pmodel(pmanager->CreateComponent("PC_QUEST_CARD_MODEL", false));  // OUT-OF-LINE ctor 0x429C15 at [0x18]
  if (PFrame) PFrame->AddPositionChild(pmodel.Get());
  CCountedPointer<...> pmesh = pmodel;   // copy: Info to [0x24], ++RefCount; Data (CMesh*) in edi
  CDefPointer<const CThingObjectDef> pobjdef;  GetDefinitionManager().GetDef(RenderedObjectDef, &pobjdef);   // 0x6869D0, 0x42B040
  pmesh->SetMeshIndex(pobjdef->Graphic.BankIndex);                                  // +0x70, slot 143
  0x5E71D3 if (RenderedObjectDef != QuestCardDef) {
    CRightHandedSet rhs = CTCInventoryItem::GetStartingRightHandedSet(RenderedObjectDef); pmesh->SetOrientation(rhs);  // slot 148
    if (pobjdef->Graphic.Type == 4) {                                                 // byte +0x7d
      CDefPointer<const CAppearanceDef> papp; pobjdef->GetSubDef(&papp);              // 0x42B0A2, [0x2c]
      pmesh->SetAnimationIndex(papp->Animation.GetAnim(CCharString("DEFAULT"), 0, CAnimFilterNoGroup()).<first ulong>);  // 0x5DC340, slot 146
      pmesh->SetMeshType((EEngineGraphicType)4);                                      // slot 147
      // ~papp (null-tested)
    }
  }
  0x5E7286 pmenu->AddNonScrollingChild(pmodel);
  ~pobjdef (0x5E7297: dec [ebx+4] with NO null test -- ebx = the def), release pmesh copy (0x5E72A3), release pmodel (0x5E72BE)  -> 0x5E74EE
}
0x5E72E9 else if (Completion == 3) {   // 0x5E72F2
  if (Thing->HasTC(TCI_INVENTORY_MAP) /*test [thing+0x20],0x8000000 in place*/) {
    CTCInventoryMap* pmap = <GetTC expanded, key 0x1B at [0x20]>;
    CCountedPointer<CComponent> pminimap(NUISystem::CManager::GetInstance()->CreateComponent("PC_MAIN_MENU_MINI_MAP_TEMPLATE", false));  // 0x429C15, [0x38]
    CCountedPointer<CComponent> psprite(pmanager->CreateComponent("PC_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE", false));                     // 0x429C15, [0x18]
    if (PFrame) PFrame->AddPositionChild(pminimap.Get());
    0x5E73AA if (psprite->GetType() == 0) {   // slot 65
      ((CSprite*)psprite.Get())->SetGraphicIndex(pmap->GetMiniMapGraphic(&GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(Thing->CurrentMapNumber))));
    }
    0x5E73FF pmap->CreateMiniMapMarkers(pminimap.Get(), GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(Thing->CurrentMapNumber)));  // 0x603000
    pminimap->AddChild(psprite);                                    // slot 59
    0x5E7445 pvp = pscreen->FindChild("PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT");
    if (pvp) { if (PFrame) PFrame->AddPositionChild(pvp); pvp->AddChild(pminimap); } else pmenu->AddNonScrollingChild(pminimap);
    0x5E749A release psprite (then its words zeroed: [0x18]=[0x1c]=0), release pminimap
  }
}
0x5E74EE CCharString cursor("UI_MOUSE_POINTER");  pcursor = NUISystem::CManager::GetInstance()->CreateComponent(cursor, false);  // [0x20]
         Components.push_back(pcursor);     // inline: if(finish!=eos){if(finish)*finish=x; ++finish;} else _M_insert_overflow(finish,x,__true_type(),1,true) 0x485DBC
         ~cursor;  Components[0]->Initialise();  // slot 43 through *begin
0x5E7559 Components.push_back(pmenu);  pmenu->Initialise();  pmenu->ChangeState(5);     // slot 48
0x5E75A1 Active = true;  GetPlayerManager().GetMainPlayer().AddMode((EPlayerMode)0x25, false);   // 0x686D40, 0x449970, 0x6345C0
0x5E75BE CGameEvent ev(9); ev.AddUBYTE(true) /*bool at [0x10]*/;  CMainGameComponent::Get()->AddGameEvent(ev);   // 0x9F1760, 0x41649C
0x5E7608 if (Completion == 0) CQuestManager::Get().AddActivatedQuestName(ScriptName);                 // PClass 0x13B89FC, 0x4B2FE0
         else if (Completion == 1 || Completion == 2) CQuestManager::Get().OnQuestFinishScreenDisplayed(ScriptName);   // 0x4B0160
0x5E7639 ReallyActive = true;  (stored between cmp and je)
0x5E7639 if (InitialGold != TargetGoldGained) GoldCountingSound = PlayCountUp();          // +0x64 v +0x60 -> +0xcc
0x5E76F3 if (InitialRenown != TargetRenownGained) RenownCountingSound = PlayCountUp();    // +0x74 v +0x70 -> +0xd0
0x5E77A9 if (InitialExperience != TargetExperienceGained) ExperienceCountingSound = PlayCountUp(); // +0x6c v +0x68 -> +0xc8
   PlayCountUp() expanded each time: CSoundInit init; init.PlaybackType = 1; init.GainFaderBits = 0x20;
     mgr = NUISystem::CManager::GetInstance(); CASoundEmitter& em = PSoundManager->Get2DEmitter();   // slot 9
     CDefPointer<...> d = mgr->GetMiscThingsDef();   // [0x20]
     result = em.vslot4(PSoundManager->GetPSoundBank(1), d->CountUpSound /*+0x1ac*/, 1, init, false);   // ~d (null-tested)
0x5E785F ~pcontainer (release; then [0x44]=[0x48]=0);  ~pmiscthingsdef (0x5E7886)
0x5E78A0 epilogue
```
Inline expansions that already match elsewhere: GFIntToWideString + CText::SetValue (GetQuestDescription 0x5E4BE0); GetTC expanded
(Activate 0x5E4740, CThing.h FABLE_RETAIL_CTHING_GETTC_EXPANDED); CCountedPointer release (engine/CCountedPointer.h); tree erase
(stlport/fable_map.h FABLE_RETAIL_STLP_TREE_ERASE_BODY); CGameEvent inline (TeleportHeroToBoastingArea 0x5FB4B0); SetGainFader
(CTCInventoryExperience::OpenInventory 0x56FAD7).

# OUTLINE: void CTCInGameMenu::OpenInGameMenu()  @0x004830A1 .. 0x00484F78 (ret)
Detail sources: $W/scout/skel.txt (every call/branch/string, labels L_xxxxxxxx), $W/scout/ann.txt (full listing annotated),
$W/scout/frame.txt (every [ebp+-X] slot with count, first use, ctor/dtor seen).

## Frame facts
- Prologue `push ebp; lea ebp,[esp-78h]; sub esp,230h; push ebx; push esi; push edi` (biased frame, optimize("y",off)+("s",on)).
  Epilogue `pop edi; pop esi; pop ebx; add ebp,78h; leave; ret` (0x00484F71). Locals span [ebp-1B8h]..[ebp+77h] (0x230 bytes).
- No SEH / EH states (TU without /EHsc), no /GS cookie. Saved order ebx, esi, edi.
- Single fall-through end: the outer-scope locals' dtors run at 0x00484EF6..0x00484F6C (ReleaseData calls, reverse decl order):
  [ebp-1A8h],[ebp-1B0h],[ebp-17Ch],[ebp-188h],[ebp-1B8h],[ebp-194h],[ebp-0ACh] CCountedPointer<CComponent>; [ebp-1Ch]
  CCountedPointer<CList>; [ebp-0BCh] CCP<CComponent>; [ebp+14h] CCountedPointer<CList>; [ebp-0CCh] CCP<CComponent>;
  [ebp-14h] CCountedPointer<CAction>.  => declaration order (first constructed first): action(-14h), subList?(-0CCh) ...
  read the ctor order 0x0048355B(-14h) 0x004837B6(-0CCh) 0x004837E8(-0BCh) 0x0048380A(-1Ch/+14h ShareData) 0x00483846(-0ACh)
  0x004838A8(-194h) 0x0048392B(-1B8h) 0x0048396F(-188h) 0x0048399F(-17Ch) 0x00483A15(-1B0h) 0x00483A45(-1A8h).

## Frame map (main slots)
| slot | local | evidence |
|---|---|---|
| [ebp+48h] | this (spilled) | 0x004830B3 |
| [ebp+6Ch] | compiler cond-temp flags (bits 1..0x20) | 0x004830B6 =0 |
| [ebp+68h] | CComponent* pauseMenu = CManager::CreateComponent("UI_LIST_PAUSE_MENU") (esi reused) | 0x0048318E |
| [ebp+54h] | CComponent* textArea = pauseMenu->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA") (viewport, +0x238 target) | 0x004831D7 |
| [ebp+60h],[ebp+44h],[ebp+10h] | FindChild results top-level screen / entries (UI_MENU_ENTRY_OPTIONS/AUDIO/SAVE..) | 0x00483502.. |
| [ebp+64h] | CComponent* helpPic / later CWideString saveName | 0x00483542, then CWideString ops |
| [ebp+5Ch] | CCP<CComponent> (UIGraphics / then loop index i) | 0x004835C7, 0x00483B20 |
| [ebp-14h] | CCountedPointer<CAction> action (new CAction) | 0x0048355B |
| [ebp+14h], [ebp-1Ch] | CCountedPointer<CList> subList / list (ShareData = assignment from CCP<CComponent>) | 0x0048380A.. |
| [ebp-0CCh],[ebp-0BCh],[ebp-0ACh],[ebp-194h],[ebp-1B8h],[ebp-188h],[ebp-17Ch],[ebp-1B0h],[ebp-1A8h] | CCountedPointer<CComponent> from CreateComponent: UI_SUB_LIST x2, UI_SCREENSHOT_VIEWPORT, UI_RING_PIC_SAVE_FOR_LIST, ..LOAD_FOR_LIST, UI_BACK_FOR_LIST, UI_SELECT_FOR_LIST, UI_TITLE_SAVE_NEW, UI_TITLE_LOAD_NEW | 0x004837B6..0x00483A45 |
| [ebp+70h] | CCharString temp (13x) / nonEmptyCount (0x00483AB1) / CCP | many |
| [ebp+50h] | count-loop index j; later CWideString | 0x00483AB7 |
| [ebp+5Ah] | bool (operator!= result; then `allEmpty`=1 at 0x00483B19) | |
| [ebp+5Bh] | bool hsFileExists (CAFile::PathExists(path + L".hs")) | 0x00483C82/0x00483CE7 |
| [ebp+77h] | bool scratch (conditions) / canSaveFishing at the end | |
| loop body (per file i) | [ebp-0Ch] CCP<CComponent> fileDesc ("UI_TEXT_FILE_DESCRIPTION"), [ebp-0A4h] CCP<CText>, [ebp-24h] CCP<CAction>, [ebp+1Ch] CCP<CAction>, [ebp+30h] CCP<CComponent> (Reset), [ebp+24h] CCP<CComponent> menuEntry, [ebp+3Ch] CCP<CComponent> | loop dtors 0x00484D4C..0x00484D97 |

## Body (retail order)
P0 0x004830B9 open + pause:
```
NUISystem::CManager::OnOpenMenus();                         // 0x0041D122
Signatures.clear();                                          // this+0xE0, 0x00485AEF (map<CWideString,bool>)
GPInputManager->PJoysticks[GetPlayerManager().GetMainPlayerJoystickDeviceNumber()]->slot18();  // +0x2C [n*8], +0x48
if (PSoundManager) { PSoundManager->slot46(0x10, 0.0f, k); (0x40 ...); (0x80 ...); }  // k = float @0x01237584; reload global each call
CMainGameComponent::Get()->PDisplayEngine->Engine3D->slot76(0);   // +0x28,+0x2C,+0x130 (push edi=0)
CurStartPauseTime = GFGetTime();                             // this+0x68
CPlayerGui::Get().Close(true);                               // 0x00438400 on 0x013B8790
InGameMenuOpen = true;                                       // +0x0C
CManager* mgr = CManager::GetInstance();
CComponent* menu = mgr->CreateComponent(CCharString("UI_LIST_PAUSE_MENU"), ...)  // read 0x00483186..0x004831AE for args
```
P1 0x004831B3 descriptions: `CComponent* area = menu->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");` then for each of
UI_RETURN_TO_GAME_DESC, UI_SAVE_DESC, UI_LOAD_DESC, UI_XLIVE_DESC, UI_OPTIONS_DESC, UI_AUDIO_OPTIONS_DESC, UI_HELP_DESC,
UI_QUIT_DESC, UI_QUIT_CONFIRM_DESC: `area->slot142_0x238(menu->FindChild(name));` (unconditional); then UI_SIGN_IN_DESC,
UI_FRIENDS_DESC, UI_APPEAR_OFFLINE_DESC with `if (c) area->...(c)` (0x00483317..0x00483383; ebx becomes 0 here).
`menu->slot26_0x68(this->owner /*+4*/);` (0x00483389).
P2 0x00483391 `if (CQuestManager::Get()->IsBetweenQuestStartAndFinishScreens())`: UI_SAVE_TEXT / UI_SAVE_SUB -> SetValue("TEXT_GUI_MENU_SAVE_HERO",1,1);
UI_SAVE_DESC -> SetValue("TEXT_GUI_MENUGUIDE_PAUSEMENU_SAVE_HERO"); UI_RING_PIC_SAVE -> `if(p) p->SetValue(p->slotA8()->/*CCP temp*/...)`
with inline CCountedPointer release at 0x004834B3..0x004834C4.
P3 0x004834C7 entries: top = FindChild("UI_TOP_LEVEL_PAUSE_SCREEN"); options/audio/save/help = FindChild("UI_MENU_ENTRY_*");
`CCountedPointer<CAction> action(new CAction);` + 2 params pushed (list push_back 0x0042B646; `new(0xC)` param),
0x004835BF `if (help) { CCP gfx = NLocalisation::GetUIGraphics(); if (gfx) { UI_RING_PIC_HELP, UI_SPRITE_HELP1..6 -> if(c) c->SetValue(...) } }`
(0x004835CD..0x00483768, release of gfx at 0x00483754).
0x00483768 `options->AddAction(action); audio->AddAction(action); if (help) help->AddAction(action);`
P4 0x00483798 save/load sub-lists: CreateComponent("UI_SUB_LIST") x2 -> CCP<CComponent>, assigned into CCP<CList> (ShareData 0x00429486),
UI_SCREENSHOT_VIEWPORT, two SetValue (+0x244), UI_RING_PIC_SAVE_FOR_LIST (+ quest-gated picture as P2, 0x004838D2..0x0048390E),
UI_RING_PIC_LOAD_FOR_LIST, UI_BACK_FOR_LIST, UI_SELECT_FOR_LIST (4x SetValue), UI_TITLE_SAVE_NEW, UI_TITLE_LOAD_NEW (2x SetValue).
P5 0x00483A7B `GetFileNameList();` then
```
saveList->slot156_0x270(FileNames.size() - (CUserProfileManager::Get().IsDebugProfile() ? 0 : 1));  // jne/dec edx
long nonEmpty = 0;
for (unsigned long j = 0; j < FileNames.size(); ++j)            // 0x00483ABC..0x00483B03
    if (FileNames[j] != CUserProfileManager::Get().GetEmptySlotName()) ++nonEmpty;   // bool stored [ebp+5Ah] then dtor
loadList->slot156_0x270(nonEmpty);                            // [ebp-1Ch]
bool allEmpty = true;   // [ebp+5Ah]=1
for (unsigned long i = 0; i < FileNames.size(); ++i) {        // body 0x00483B29 .. 0x00484DA8 (jb back)
```
P6 loop body (read skel.txt 0x00483B29-0x00484DAB):
- fileDesc = CreateComponent("UI_TEXT_FILE_DESCRIPTION"); tmpl = CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF");
  CCountedPointer<CText> text = tmpl (ShareData 0x00429460); text->SetValue(CWideString(L"INVALID FILE DESCRIPTION STRING"),..) (+0x240);
  area->0x238(...).
- action2 = new CAction; param new(0xC) (vt 0x012366F4, value 3?) push_back; `new CActionParamCCountedPointer<CComponent>(fileDesc)` -> AddParam.
- `CWideString name(FileNames[i]); CWideString hs; bool hsExists=false; hs = GetPathNameFromFileName(name,?) + L".hs"; if (CAFile::PathExists(hs)) hsExists=true;`
- action3 = new CAction; `bool ok = VerifySignatureCached(GetPathNameFromFileName(name,..)) && (!hsExists || VerifySignatureCached(GetPathNameFromFileName(hs..)))`
  (cond-temp bit 1); `if (ok && name != GetEmptySlotName())` -> CreateComponent("UI_RING_PIC_DRAW_FROM_VIEWPORT") into [ebp+30h] via Reset + 3 AddParam (0x00483DB4..0x00483E5A).
- 0x00483E5F `if (!pic) { pic.Reset(CreateComponent("UI_RING_PIC_SAVE_VIEWPORT")); if (IsBetweenQuest... && pic) { 3 AddParams } }`
- 0x00483F2B area->0x238(pic?); `bool manual = !IsDebugProfile() && name != GetEmptySlotName() && name != GetAutoSaveFileName();` (bits 2,4)
  `if (manual) name = GetManualSaveFileName(i?);` (0x00483FC5)
- 0x00483FF4 `if (name != GetAutoSaveFileName())` { display-name selection 0x00484027..0x004840E2 (wcscmp / wstring == against
  literal 0x0122D70C, GetSaveDisplayNameFromFileName(name,i), GetEmptySlotName) ; template = isEmpty&&quest ? "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW"
  : "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW" (0x004840ED..0x0048410B); entry = mgr->CreateMenuEntryComponent(&display, .., "PC_BUTTON_BASTARD", .., L"" 0x0122D70E ..) (0x00484159) -> CCP [ebp+3Ch];
  - 0x0048416F `if (!isEmpty)` overwrite branch: sub = CreateComponent("UI_SUB_LIST"); sub->SetName("COMPONENT_SAVE"); entry->AddChild(sub);
    UI_CONFIRM_FOR_LIST / UI_BACK_FOR_LIST (`if(p) SetValue`), UI_MENU_ENTRY_OVERWRITE_SAVE_GAME (+0x240), FindChild("UI_OVERWRITE_DESCRIPTION")
    (quest -> SetValue("TEXT_GUI_MENU_OVERWRITE_DESCRIPTION_HERO")), area->0x238; UI_TITLE_NEW: if(title) { CCP a=..->slotA8(), b=..; text =
    GetTextBySymbol("TEXT_GUI_MENU_SAVE") + .. + .. (3x operator+ 0x0099BE70) ; SetValue(+0x240); SetValue(+0x244)}; releases; jmp 0x00484790.
  - 0x004844B9 else if (quest) confirm-save branch (same shape: UI_SUB_LIST/COMPONENT_SAVE/UI_CONFIRM_FOR_LIST/UI_BACK_FOR_LIST/
    UI_MENU_ENTRY_CONFIRM_SAVE_GAME/UI_CONFIRM_DESCRIPTION/UI_TITLE_NEW+TEXT_GUI_MENU_SAVE), jmp 0x004844AC.
  - 0x00484701 else: `CCountedPointer<CAction> a(new CAction); entry->SetName? ("COMPONENT_SAVE" +0x14C); ->0x25C(param); AddAction(a)`.
  - 0x00484790 saveList->AddChild(entry) (+0xEC); entry->AddActionOnSelected(action3) (+0x10C); ... }
- 0x004847C6 load side: `loadList->slot152_0x260(..)`; `if (name != GetEmptySlotName()) { loadList->AddChild(...); CCP<CMenuEntry> me = ...(ShareData 0x0048524C); new(0xC) param -> me->0x25C; me->AddActionOnSelected; }`
  display = GetSaveDisplayNameFromFileName(name,i); entry2 = CreateMenuEntryComponent(..., "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", "PC_BUTTON_BASTARD") -> [ebp+24h];
  `bad = !(VerifySignatureCached(..) && (!hsExists || VerifySignatureCached(..)))` (bit 8) -> `if (bad) { entry2->MakeUnavailable(); arrow = entry2->FindChild("UI_SELECTION_ARROW_RIGHT"); if (arrow) arrow->SetDeletion(CDeletion()); }`
  (0x00484956..0x004849C2; CDeletion default ctor 0x0042BED4 at [ebp-15Ch], copy 0x0042CD84, list dtor 0x0042ABCA [ebp-158h]).
  then loadList->0x25C, AddChild, AddActionOnSelected (0x004849C7..0x004849F4); repeat VerifySignature test (bit 0x10, result unused-ish) ;
  `if (name != GetEmptySlotName() && verified(bit 0x20))` confirm-load sub-menu: UI_CONFIRM_LOAD_LIST, UI_TITLE_NEW +
  GetTextBySymbol("TEXT_GUI_MENU_LOAD") + operator+ x3, SetValue(+0x240 via edi), SetValue +0x244, AddChild, FindChild("UI_LOAD_CONFIRM_DESC") -> area->0x238;
  (0x00484A86..0x00484CF5). 0x00484CFD: me2 ShareData(CMenuEntry) + new(0xC) param -> 0x25C, AddActionOnSelected.
- per-iteration dtors 0x00484D4C..0x00484D97 then `++i` (0x00484DA2).
P7 0x00484DB1 after loop:
```
bool fishing = false;  // [ebp+77h]
CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
if (IS_THING_ALIVE(hero) && hero has TC 0xB1 (TCI_FISHING; inline GetTC, FABLE_RETAIL_CTHING_GETTC_EXPANDED))
    fishing = tc->field_0x14 != 0;   // setne
CComponent* save = menu->FindChild("UI_MENU_ENTRY_SAVE");
if (!allEmpty?[ebp+5Ah] && !fishing) save->AddChild(subList [ebp-0CCh]);   // 0x00484E3E
else { save->FindChild("UI_SELECTION_ARROW_RIGHT") -> if(arrow) arrow->SetDeletion(CDeletion()); save->MakeUnavailable(); }  // 0x00484E51..0x00484EA1
menu->FindChild("UI_MENU_ENTRY_LOAD")->AddChild([ebp-0BCh]);   // 0x00484EAE
menu->Initialise(); menu->ChangeState(5);                       // +0xAC, +0xC0
Components.push_back(menu);                                     // this+0x58, 0x00486024, arg = &[ebp-154h] (menu spilled)
```
(Check 0x00484E31: [ebp+5Ah] is the "allEmpty"/"saveBlocked" flag cleared inside the loop -- trace its writes 0x00483AE4, 0x00483B19, 0x0048402A.)
No jump tables. No cold blocks after the epilogue.

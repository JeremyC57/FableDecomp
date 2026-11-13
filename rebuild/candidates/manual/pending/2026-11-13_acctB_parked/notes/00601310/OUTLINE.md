# OUTLINE 0x00601310 CTCInventoryMap::ConstructQuestsScreenPC(NUISystem::CComponent* screen) -- 6931 bytes, 0x601310..0x602E23

All facts come from $W/dis_all.txt and the annotated listing $W/scout/map/ann.txt. S<hex> means [esp+hex] at body depth, after
`sub esp,78h` and the 4 pushes. At that depth S88 is the return address, S8C is `screen` and S2C first holds `this`.

## Frame
- Prologue: `sub esp,78h ; push ebx ; push ebp ; push esi ; push edi`.
- The function has no frame pointer, no SEH and no /GS. It ends with `ret 4`.
- esi = 0 from 0x601336 to the end.
- Epilogue: `mov eax,edi` (edi = menu) `; pop edi/esi/ebp/ebx ; add esp,78h ; ret 4` at 0x602E17.
- Callee-saved register use:
  - edi: the current object.
  - ebp: the scroll-up component in the groups, the menu from 0x601A3A on, and the viewport after 0x60277E.
  - ebx: vptrs, the textBank (0x60278D), and the manager.

| slot | holds | type / lifetime |
|---|---|---|
| S10 | menu | `NUISystem::CList*` from manager->CreateComponent("PC_QUESTS_MENU", false); also the return value |
| S14 | frame | CComponent* menu->FindChild("PC_UI_FRAME_QUESTS") |
| S18 | manager | NUISystem::CManager* GetInstance() |
| S24 | CCharString temp | "PC_QUESTS_MENU" (CreateComponent's const& arg), destroyed at 0x60134F |
| S28 | down component (FindChild "SCROLL_*_DOWN") in each group; later the description vptr spill (0x6029A5) | |
| S2C | `this` (0x601317, last read 0x6027DD); then a CCharString temp for every later string arg | |
| S58/S5C, S48/S4C, S30/S34, S38/S3C | the 4 CCountedPointer<CAction> of each listen/ignore group (Data/Info) | block-scoped, re-used by all 4 groups |
| S30 | CWideString temp from GetTextBySymbol, at 0x6029A9 / 0x602BD8 (after the groups are dead) | |
| S38 | textBank spill (`const NGameText::CDataBank&`), from 0x60279B | |
| S20 | pastText | CWideString, first declared of the tail, destroyed last (0x602E12) |
| S80/S84 | pastEntry | CCountedPointer<CComponent>, default ctor at 0x60270A; dtor CALLED 0x429C53 at 0x602E09 |
| S78/S7C | pastList | CCountedPointer<CComponent>(ConstructPastQuestsListPC(screen, menu)), inline ctor at 0x60271D, deleter 0x428B10 |
| S50/S54 | description | CCountedPointer<CComponent>, zeroed at 0x602785 |
| S68/S6C | subtitle | CCountedPointer<CComponent>, zeroed at 0x60279F |
| S60/S64 | ringPic | CCountedPointer<CComponent>, zeroed at 0x6027A7 |
| S1C | curText | CWideString |
| S40/S44 | curEntry | CCountedPointer<CComponent>, zeroed at 0x6027E1 |
| S70/S74 | curList | CCountedPointer<CComponent>(ConstructQuestReviewListPC(screen, menu)), inline ctor at 0x6027EE |

The end-of-function destruction order is:
1. curList S70 (0x602CEB)
2. curEntry S40 (0x602D10)
3. curText S1C (0x602D49)
4. ringPic S60
5. subtitle S68
6. description S50
7. pastList S78
8. pastEntry S80 (call 0x429C53)
9. pastText S20

So the declaration order is pastText, pastEntry, pastList, description, subtitle, ringPic, curText, curEntry, curList.

## Inline expansions (repeated 16 times, once per action)
`CCountedPointer<CAction> a(new CAction);` expands as:
- `push 10h ; call new`.
- If non-null, the inline CAction ctor:
  - [0]=0;
  - [4] = malloc(0Ch) header node, self-linked;
  - [8] = the same;
  - byte [0Ch]=1.
- Store Data. If non-null, `new(0Ch)` CCPPointerInfo {1, 0x429F43, Data}. Info goes to the slot, or 0.

Then:
- `a->ActionType = X` is stored after the CCPPointerInfo.
- `a->AddParam(comp, a)`:
  - `comp ? comp+18h : 0`;
  - `push &a ; push param ; ecx = a.Data ; call 0x53FDE0`.
- `a->UIActionParams.push_back(new CActionParamULONG(UI_EVENT_MOUSE_MOVEMENT))`:
  - `new(0Ch)`, then vptr 0x122F844, type 5, value 0x19;
  - malloc(0Ch) node, value stored at node+8 (null-checked);
  - link before the header.

Each destructor is inline: `if(Info){if(--RefCount==0){DeleteFunc(Info->Data); delete Info;}} Data=0; Info=0;`.

## Body, in retail order
```
NUISystem::CManager* manager = NUISystem::CManager::GetInstance();            // 0x60131B call 0x41E5F2 -> S18
NUISystem::CList* menu = (NUISystem::CList*)manager->CreateComponent("PC_QUESTS_MENU", false);   // 0x601340 -> S10
NUISystem::CComponent* frame = menu->FindChild("PC_UI_FRAME_QUESTS");          // 0x601367 slot 3 -> S14

// --- group 1, 0x60136A: the screen's list scroll buttons, installed on the menu
{   CComponent* up   = screen->FindChild("SCROLL_LIST_UP");                     // 0x601388 -> ebp
    CComponent* down = screen->FindChild("SCROLL_LIST_DOWN");                   // 0x6013A0 -> S28
    CCountedPointer<CAction> observeUp(new CAction);   // 0x6013A3, S58: OBSERVE, AddParam(up) 0x60143B, push_back ULONG 0x19
    CCountedPointer<CAction> ignoreUp(new CAction);    // 0x60148C, S48: IGNORE,  AddParam(up) 0x601523, push_back
    menu->AddActionOnSelected(ignoreUp);               // 0x601584 slot 67
    menu->AddActionOnInactive(observeUp);              // 0x601593 slot 70
    CCountedPointer<CAction> observeDown(new CAction); // 0x601599, S30: OBSERVE, AddParam(down) 0x601631
    CCountedPointer<CAction> ignoreDown(new CAction);  // 0x601682, S38: IGNORE,  AddParam(down) 0x601719
    menu->AddActionOnSelected(ignoreDown);             // 0x60177A
    menu->AddActionOnInactive(observeDown);            // 0x601789
}   // dtors 0x60178F..0x60183B: ignoreDown S38, observeDown S30, ignoreUp S48, observeUp S58

// --- group 2, 0x60183B: the screen's description scroll buttons. Same statements as group 1:
//     "SCROLL_DESCRIPTION_UP" 0x601855 -> ebp, "SCROLL_DESCRIPTION_DOWN" 0x60186D -> S28.
//     observeUp S58 (AddParam 0x601908), ignoreUp S30 (0x6019F0); OnSelected(ignoreUp) 0x601A52, OnInactive(observeUp) 0x601A62;
//     observeDown S38 (0x601B00), ignoreDown S48 (0x601BEC); OnSelected(ignoreDown) 0x601C4A, OnInactive(observeDown) 0x601C5A.
//     Dtors 0x601C64..0x601D0C: S48, S38, S30, S58.

// --- group 3, 0x601D0C: the FRAME's list scroll buttons, installed on the menu with the slots REVERSED
{   CComponent* up   = frame->FindChild("SCROLL_LIST_UP");                      // 0x601D23
    CComponent* down = frame->FindChild("SCROLL_LIST_DOWN");                    // 0x601D3B -> S28
    observeUp S58 (AddParam 0x601DD6), ignoreUp S30 (0x601EBE);
    menu->AddActionOnSelected(observeUp);   // 0x601F20  <-- observe on Selected
    menu->AddActionOnInactive(ignoreUp);    // 0x601F30
    observeDown S38 (0x601FCE), ignoreDown S48 (0x6020BA);
    menu->AddActionOnSelected(observeDown); // 0x602118
    menu->AddActionOnInactive(ignoreDown);  // 0x602128
}   // dtors 0x602132..0x6021DA: S48, S38, S30, S58

// --- group 4, 0x6021DA: frame->FindChild("SCROLL_DESCRIPTION_UP" 0x6021F1 / "SCROLL_DESCRIPTION_DOWN" 0x602209). Same as group 3:
//     AddParam 0x6022A4 / 0x60238C / 0x60249C / 0x602588; OnSelected(observeUp) 0x6023EE, OnInactive(ignoreUp) 0x6023FE,
//     OnSelected(observeDown) 0x6025E6, OnInactive(ignoreDown) 0x6025F6. Dtors 0x602600..0x6026A8.

frame->AddPositionChild(menu->FindChild("PC_TITLE_QUESTS_MENU"));             // 0x6026BC / 0x6026C6 slot 124, no null test
CWideString pastText = <CMainGameComponent 0x13B86A0>->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_PAST_QUESTS"); // 0x6026EF -> S20
CCountedPointer<CComponent> pastEntry;                                          // 0x60270A
CCountedPointer<CComponent> pastList(ConstructPastQuestsListPC(screen, menu));  // 0x602718 (push menu; push screen; ecx=this)
menu->InitialiseOffsets(2);                                                     // 0x60275B slot 156
NUISystem::CViewport* viewport = (NUISystem::CViewport*)screen->FindChild("PC_DESCRIPTION_VIEWPORT"); // 0x60277B -> ebp
const NGameText::CDataBank& textBank = ...GetTextBank();                        // 0x602780 [g]+0x14 -> ebx, spilled to S38
CCountedPointer<CComponent> description;                                        // 0x602785 S50
CCountedPointer<CComponent> subtitle;                                           // S68
CCountedPointer<CComponent> ringPic;                                            // S60
CWideString curText = <global>->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_CURRENT_QUESTS"); // 0x6027C6 (re-reads 0x13B86A0) -> S1C
CCountedPointer<CComponent> curEntry;                                           // 0x6027E1 S40
CCountedPointer<CComponent> curList(ConstructQuestReviewListPC(screen, menu));  // 0x6027E9 (push menu; push screen)
// Null Data: 0x6027F4 je 0x6028D0 (Info=0) -> else arm 0x6028D4.
// A failed new: 0x6028C7 Info=0, then the test at 0x602824.
if (curList.Get()) {                                                            // 0x602824 cmp [S70],0 ; je 0x6028D4
    curEntry.Reset(manager->CreateMenuEntryComponent(curText, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f,
                   CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));    // 0x602872 / Reset 0x60287C
    curEntry->AddChild(curList);                                                // 0x602895 slot 59
    curEntry->SetName("CURRENT_QUESTS_ENTRY");                                  // 0x6028B6 slot 83 ; jmp 0x60293C
} else {                                                                        // 0x6028D4
    curEntry.Reset(manager->CreateMenuEntryComponent(curText, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f,
                   CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));    // 0x602918 / 0x602922
    curEntry->MakeUnavailable();                                                // 0x602936 slot 92
}
description.Reset(manager->CreateComponent("PC_DESCRIPTION", false));          // 0x60293C, 0x602956 / Reset 0x602960
frame->AddPositionChild(description.Get());                                     // 0x602979
((NUISystem::CText*)description.Get())->SetValue(textBank.GetTextBySymbol("TEXT_GUI_MENUGUIDE_QUESTS_CURRENTQUESTS"), true, true);
    // 0x602995 push 1; push 1; GetTextBySymbol 0x6029A9 (ecx = ebx textBank, ret -> S30); slot 144 0x6029B5; ~CWideString 0x6029BF
viewport->AddFakeChild(description.Get());                                      // 0x6029D3 slot 142
curEntry->AddChild(description);                                                // 0x6029E4
subtitle.Reset(manager->CreateComponent("PC_CURRENTQUESTS_SUBTITLE", false));  // 0x602A06 / 0x602A10
frame->AddPositionChild(subtitle.Get());  curEntry->AddChild(subtitle);         // 0x602A2B / 0x602A3C
ringPic.Reset(manager->CreateComponent("PC_RING_PIC_CURRENT_QUESTS", false));  // 0x602A5A / 0x602A64
frame->AddPositionChild(ringPic.Get());   curEntry->AddChild(ringPic);          // 0x602A7B / 0x602A8C
if (pastList.Get()) {                                                           // 0x602A92 ; je 0x602B0B
    pastEntry.Reset(manager->CreateMenuEntryComponent(pastText, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f,
                    CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));   // 0x602ADA / 0x602AE7
    pastEntry->AddChild(pastList);                                              // 0x602B03 ; jmp 0x602B71 (no SetName here)
} else {                                                                        // 0x602B0B
    pastEntry.Reset(manager->CreateMenuEntryComponent(pastText, ... "PC_MENU_ENTRY_TEMPLATE" ...)); // 0x602B47 / 0x602B54
    pastEntry->MakeUnavailable();                                               // 0x602B6B
}
description.Reset(manager->CreateComponent("PC_DESCRIPTION", false));          // 0x602B71..0x602B93
frame->AddPositionChild(description.Get());                                     // 0x602BAA
((CText*)description.Get())->SetValue(textBank.GetTextBySymbol("TEXT_GUI_MENUGUIDE_QUESTS_PASTQUESTS"), true, true); // 0x602BD8 / 0x602BE0
viewport->AddFakeChild(description.Get());                                      // 0x602BFE
pastEntry->AddChild(description);                                               // 0x602C12
subtitle.Reset(manager->CreateComponent("PC_PASTQUESTS_SUBTITLE", false));     // 0x602C34
frame->AddPositionChild(subtitle.Get());  pastEntry->AddChild(subtitle);        // 0x602C59 / 0x602C6D
ringPic.Reset(manager->CreateComponent("PC_RING_PIC_PAST_QUESTS", false));     // 0x602C8B
frame->AddPositionChild(ringPic.Get());   pastEntry->AddChild(ringPic);         // 0x602CAC / 0x602CC0
menu->AddScrollingChildAndSetOffsets(curEntry);                                 // 0x602CD3 slot 152
menu->AddScrollingChildAndSetOffsets(pastEntry);                                // 0x602CE5
return menu;                                                                    // dtors 0x602CEB..0x602E12, then the epilogue 0x602E17
```

## Notes
- **Argument order of CreateMenuEntryComponent at retail (0x60282E):**
  - The const& template string (S2C) is constructed FIRST.
  - Then the by-value "PC_BUTTON_BASTARD", 0.0f, the by-value "", 0.0f, &template, 1.0f (0x3F800000), 0.0f, &text.
  - `ecx = manager` (S18, or ebx after 0x6029FA).
- The vptr of `description` is loaded before the GetTextBySymbol call and spilled to S28 (0x602993 / 0x6029A5). In the past half
  it is kept in ebx (0x602BC4). Expect this to follow from the expression order: object, then args.
- The 4 groups have no shared helper. CManager::AddActionsListenIgnoreOnList 0x0041DC9D is in another TU, and groups 3 and 4
  use reversed slots. Write the groups out.

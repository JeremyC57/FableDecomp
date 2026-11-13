# DOSSIER 0x00601310 -- CTCInventoryMap::ConstructQuestsScreenPC (6931 bytes)

Scout: identity / callees / members / siblings. No earlier scout existed ($W/prior/ is empty; agent2094 holds only its brief, no
work), so this is a fresh scout. Working files are in $W/scout/:
- map/ann.txt: the annotated listing. Every [esp+N] is rewritten as S<hex>, the slot at body depth (after the 4 pushes).
  The two vtable calls are corrected through tools/ovr.txt.
- map/slots.txt: the first and last use of each slot.
- calls_resolved.tsv: the callees.
- tools/acts.py: the CAction map.

## 1. Identity (3 sources) -- the manifest label is WRONG
- The manifest label `_global::ConstructQuestsScreen_MainMenu` is descriptive and wrong.
- Real name: **CTCInventoryMap::ConstructQuestsScreenPC**.
  - Mangled leaf: **?ConstructQuestsScreenPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z**.
  - Signature: public, non-virtual, thiscall. `NUISystem::CComponent* ConstructQuestsScreenPC(NUISystem::CComponent* screen)`.
- Sources for the name:
  1. FableWin.pdb at 0x020ABAB0 (ghidra_out/_fablewin_api.tsv and naming_stragglers/FableWin.exe.signatures.tsv). /Od size is
     8744 bytes, 2048 instructions.
  2. Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, Ego_r 0x00655BC0). The console twin ?ConstructQuestsScreen@CTCInventoryMap@@ is
     at Ego_r 0x006522D0 and FableWin 0x02083ED0.
  3. The landed caller rebuild/src/compiled/00/41/NUISystemCManager_Action_0041e6d3.cpp:1720, in case
     UI_ACTION_TYPE_CONSTRUCT_PC_QUESTS_LIST: `CComponent* list = map->ConstructQuestsScreenPC(screen);`. Its local class model
     at line 295 says `// 0x00601310`.
  4. The landed callee CTCInventoryMap_ConstructQuestReviewListPC_005fc8a0.cpp:6 says it is "called from
     ConstructQuestsScreenPC (0x006027E9)". That is our call site.
- The only xref is 0x00421F52, inside Action. The function is not in any vtable.
- The param is the screen / container the action passes. It is FindChild'ed for SCROLL_* and PC_DESCRIPTION_VIEWPORT, and it is
  passed as `container` to the two list builders. `return menu;`: edi = the "PC_QUESTS_MENU" component, `mov eax,edi` at
  0x602E17.
- The PDB parameter name is not available. Descriptive name: `screen`, or `container` as the siblings use.
- Header: engine/CTCInventoryMap.h declares neither ConstructQuestsScreen nor ConstructQuestsScreenPC. The author needs an
  overlay opt-in that declares it public. A suggested name is FABLE_RETAIL_CTCINVENTORYMAP_QUESTSSCREENPC, in the style of the
  _PASTQUESTS / _QUESTREVIEW blocks at lines 102-120. That pair is private (`AAE`) and called from here, so define
  FABLE_RETAIL_CTCINVENTORYMAP_PASTQUESTS and FABLE_RETAIL_CTCINVENTORYMAP_QUESTREVIEW too.
- The console twin ConstructQuestsScreen 0x005FE090 is NOT landed. It is the next-closest model, the same TU.

## 2. Retail shape (xdis 00601310 0x1b23 / dis_all.txt)
- Prologue: `sub esp,78h ; push ebx ; push ebp ; push esi ; push edi`. Callee-saved pushes come AFTER the sub.
  - No frame pointer, so the build is /Oy (default): no optimize("y",off).
  - No SEH: there is no fs:0 and no EH state stores, although there are many destructors. So /EHsc is not in effect, as with the
    other landed CTCInventoryMap rows (no "// Flags" line on any of them).
  - No /GS cookie.
- Epilogue: `mov eax,edi ; pop edi ; pop esi ; pop ebp ; pop ebx ; add esp,78h ; ret 4` at 0x602E17..0x602E20, the end of the
  row.
- The body is one straight line with if/else diamonds. There are no loops, no jump tables, and no cold blocks after the ret.
- esi = 0 for the whole body (`xor esi,esi` at 0x601336). Every null test is `cmp reg,esi`.
- This is a SPEED TU (/O2, no optimize("s")):
  - The CAction ctor is inline at all 16 sites. There is no call to 0x0042BE50.
  - list::push_back is inline at all 16 sites: malloc(0xC) node and link. There is no call to 0x0042B646.
  - The CCountedPointer(T*) ctor is inline at all 18 sites: new CCPPointerInfo with deleter 0x429F43 or 0x428B10.
  - The CCountedPointer dtor is inline everywhere EXCEPT the last but one destruction (S80, see OUTLINE).
- The landed same-class neighbours carry no optimize pragma, only `#pragma auto_inline(off/on)` around same-TU template bodies.
  The /Os CTCInventoryStats::ConstructExperienceListPC 0x005D7081 CALLS all these helpers, so do NOT copy its
  optimize("s") / ("y",off) pragmas or its FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR. Copy its statement text only.

## 3. Callees (direct calls, from calls_resolved.tsv)
| addr | n | name (PDB spelling) | landed row | note |
|---|---|---|---|---|
| 0x0041E5F2 | 1 | NUISystem::CManager::GetInstance | 00/41/NUISystemCManager_GetInstance_0041e5f2.cpp | called out of line |
| 0x0041DB1D | 7 | ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z | 00/41/..._CreateComponent_0041db1d.cpp | (name, false) |
| 0x00428494 | 4 | ?CreateMenuEntryComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCWideString@@MMABVCCharString@@MV5@MV5@@Z | not landed (declared in NUISystem_CManager.h:97) | ret 20h |
| 0x00429C6F | 10 | ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z | 00/42/..._Reset_00429c6f.cpp | CALLED at all 10 sites |
| 0x00429C53 | 1 | ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ (5-byte jmp to ReleaseData 0x4291DE) | 00/42/..._CCountedPointer_00429c53.cpp | only the S80 destructor |
| 0x0053FDE0 | 16 | ?AddParam@CAction@NUISystem@@QAEXPAVCActionParam@2@AAV?$CCountedPointer@VCAction@NUISystem@@@@@Z | 00/53/NUISystemCAction_AddParam_0053fde0.cpp | the reloc table also binds the alias AddUIActionParam; use AddParam (engine/NUISystem_CAction.h) |
| 0x005FDDD0 | 1 | ?ConstructPastQuestsListPC@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@0@Z | 00/5f/CTCInventoryMap_ConstructPastQuestsListPC_005fddd0.cpp | (screen, menu) |
| 0x005FC8A0 | 1 | ?ConstructQuestReviewListPC@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@0@Z | 00/5f/CTCInventoryMap_ConstructQuestReviewListPC_005fc8a0.cpp | (screen, menu) |
| 0x009C95E0 | 4 | ?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z | 00/9c/... | hidden return |
| 0x0099EBF0 | 35 | ??0CCharString@@QAE@PBDJ@Z (const char*, -1) | 00/99/CCharString_Constructor_0099ebf0.cpp | |
| 0x0099EAE0 | 16 | ??1CCharString@@QAE@XZ | 00/99/CCharString_CCharString_0099eae0.cpp | |
| 0x0099B510 | 4 | ??1CWideString@@QAE@XZ | 00/99/CWideString_Destructor_0099b510.cpp | |
| 0x00BFEA1A | 50 | operator new | | |
| 0x00BFEA0E | 48 | malloc | | STLport node / list-header allocation |
| 0x00BFE9BC | 22 | operator delete | | inline CCPPointerInfo delete |

Address-taken deleters, not called:
- 0x00429F43 = ?DeleteData@?$CCountedPointer@VCAction@NUISystem@@@@SIXPAX@Z.
  - Sources: FableWin 0x01ABC82B and Ego_r 0x00438DF1. The body is `if(p){~CAction 0x53FD00; delete p;}`.
  - **NAME_FIX candidate:** it is landed as rebuild/src/compiled/00/42/CTCMapwho_OnDie_00429f43.cpp (CTCMapwho::OnDie, wrong).
- 0x00428B10 = CCountedPointer<NUISystem::CComponent>::DeleteData. The manifest agrees. The landed file
  global_DeleteData_00428b10.cpp has the leaf `@DeleteIfSet@4`, a descriptive mismatch.
- Both deleters come from the header's `DeleteData` static. Nothing to write.

Virtual calls. The CComponent slots are in engine/NUISystem_CComponent.h:
| off | slot | name | object |
|---|---|---|---|
| +0x0C | 3 | FindChild(CCharString) by value (the string is built in the arg slot: `push ecx; mov ecx,esp`) | menu, screen, frame |
| +0xEC | 59 | AddChild(CCountedPointer<CComponent>&) | curEntry / pastEntry |
| +0x10C | 67 | AddActionOnSelected(CCountedPointer<CAction>&) | menu |
| +0x118 | 70 | AddActionOnInactive(CCountedPointer<CAction>&) | menu |
| +0x14C | 83 | SetName(const CCharString&) | curEntry |
| +0x170 | 92 | MakeUnavailable() | the entry in each else-arm |
| +0x1F0 | 124 | AddPositionChild(CComponent*) | frame |
| +0x238 | 142 | NUISystem::CViewport::AddFakeChild(CComponent*) (engine/NUISystem_CViewport.h:34) | viewport |
| +0x240 | 144 | NUISystem::CText::SetValue(const CWideString&, bool, bool) (engine/NUISystem_CText.h:23), called (text, true, true) | description |
| +0x260 | 152 | NUISystem::CList::AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>&) (rtti CList 0x012462E4 slot 152 = 0x0053AD07) | menu |
| +0x270 | 156 | NUISystem::CList::InitialiseOffsets(unsigned long) (slot 156 = 0x00536B4F), called InitialiseOffsets(2) | menu |

So the menu is used as a NUISystem::CList* (engine/NUISystem_CList.h), the description as a CText*, and the viewport as a
CViewport*.

Globals:
- 0x013B86A0 = CMainGameComponent::PMainGameComponent. Its +0x14 is the PTextBank object word
  (engine/CMainGameComponent.h:61, GetTextBank() at line 234, which is plain).
- Retail reads the global afresh for each of the two menu-text lookups (0x6026DC and 0x6027B4).
- Once, at 0x602780/0x60278D, it caches `[g]->+0x14` in ebx and spills it to S38. That value serves the two
  TEXT_GUI_MENUGUIDE_* lookups. So there is a named `const NGameText::CDataBank& textBank = ...GetTextBank();` local, declared
  around the viewport line. The two menu texts are spelled `CMainGameComponent::...->GetTextBank().GetTextBySymbol(..)` inline.
- To be confirmed by compile: whether that local is a reference or the lookups call GetTextBank each time.

String literals (all ASCII CCharString):
- PC_QUESTS_MENU, PC_UI_FRAME_QUESTS
- SCROLL_LIST_UP, SCROLL_LIST_DOWN, SCROLL_DESCRIPTION_UP, SCROLL_DESCRIPTION_DOWN
- PC_TITLE_QUESTS_MENU
- TEXT_GUI_MENU_PAST_QUESTS, TEXT_GUI_MENU_CURRENT_QUESTS
- PC_DESCRIPTION_VIEWPORT
- PC_MENU_ENTRY_BRANCH_TEMPLATE, PC_MENU_ENTRY_TEMPLATE, PC_BUTTON_BASTARD, "" (0x0122D70E)
- CURRENT_QUESTS_ENTRY, PC_DESCRIPTION
- TEXT_GUI_MENUGUIDE_QUESTS_CURRENTQUESTS, TEXT_GUI_MENUGUIDE_QUESTS_PASTQUESTS
- PC_CURRENTQUESTS_SUBTITLE, PC_PASTQUESTS_SUBTITLE
- PC_RING_PIC_CURRENT_QUESTS, PC_RING_PIC_PAST_QUESTS

Enums:
- 0xF5 = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT (245) and 0xF9 = UI_ACTION_TYPE_IGNORE_EVENT (249), from
  engine/NUISystem_EActionType.h:139/143.
- ULONG param 0x19 = NUISystem::UI_EVENT_MOUSE_MOVEMENT (25), from engine/NUISystem_enums.h:108.
- CActionParamULONG type 5 = UI_ACTION_PARAM_TYPE_ULONG, vptr 0x0122F844 (engine/NUISystem_CActionParamULONG.h). That header
  needs `#define FABLE_RETAIL_CACTIONPARAMULONG`.

## 4. Members touched
- `this` is used only as the ecx of the two private list builders. It is spilled to S2C at entry and that slot is later reused
  for CCharString temporaries.
- No CTCInventoryMap member is read or written.
- NUISystem::CAction (engine/NUISystem_CAction.h, sizeof 0x10 under FABLE_RETAIL_STLP_LIST):
  - ActionType +0: 0 in the ctor, then OBSERVE/IGNORE.
  - UIActionParams +4: list header node malloc(0xC), self-linked; push_back.
  - ThingActionParams +8: list header node.
  - Valid +0xC = 1.
- **The inline ctor needs FABLE_RETAIL_CACTION_INLINE_CTOR.** The repo header does NOT have it. It is an overlay block in
  /opt/fable_lane/w22/agent4007/hdr/engine/NUISystem_CAction.h, lines 23-31:
  `CAction() : ActionType((EActionType)0), Valid(true) { }`. Copy that overlay file.
- CCountedPointer<T> (engine/CCountedPointer.h): Data +0, Info +4. CCPPointerInfo: RefCount +0, DeleteFunc +4, Data +8.
  - The ctor stores Data, then tests the data register and does `Info = new CCPPointerInfo(1, DeleteData, Data)`. A failed new
    gives Info=0.
  - The inline release is `if(Info){--RefCount; if(!RefCount){DeleteFunc(Info->Data); delete Info;}} Data=0; Info=0;`, with
    Data zeroed BEFORE Info at every site.
  - Use FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA. All the NUISystem rows (Action, ExperienceListPC) define it, and it gives
    ~CCountedPointer() { ReleaseData(); } with Data-first zeroing.
  - Retail's one out-of-line dtor call goes to 0x429C53, the ??1 COMDAT. That proves the dtor is a separate inline function
    (RELEASEDATA's `{ ReleaseData(); }`).
- CComponent's CActionParam base sits at +0x18. AddParam(comp, act) converts with a null guard (`cmp ebp,esi; je -> xor eax`),
  as in ExperienceListPC.

## 5. Callee-body visibility / opt-ins
- **Reset(T*)** is CALLED (0x429C6F) at all 10 sites, but the default header body would be expanded in a speed TU.
  - Options: FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL (CCountedPointer.h:204) gives a declaration only, which produces a
    call to the same mangled COMDAT. The other option is a visible but noinline body (overlay).
  - Probe both. Decl-only changes what c2 knows about the object (the frame); see the CCountedPointer VISIBLE_CALLED_CTOR note
    in NUISystemCManager_Action_0041e6d3.cpp:9.
- **~CCountedPointer<CComponent>** is inline 8 times in the epilogue run and called once, for S80 (pastEntry, the second-to-last
  destroyed).
  - This is a VC7.1 inline-budget artefact, like ConstructQuestListPC's sixth CAction ctor (agent4007 hdr README: "calls
    0x0042BE50 at the sixth once VC7.1's inline budget is spent").
  - Expect it to fall out of the right statement order. If it does not, see the c2058 / "static frequency" lessons in
    /opt/fable_lane/w22/agent2050/CHUNK_BRIEF.md.
- The CAction ctor, push_back, the CCountedPointer(T*) ctor and the CActionParamULONG ctor are all inline at every site. They
  need FABLE_RETAIL_STLP_LIST and FABLE_RETAIL_CACTION_INLINE_CTOR (overlay).
  - Check that FABLE_RETAIL_STLP_LIST's push_back gives `malloc(0xC); if(node+8) *(node+8)=x; link before head`. The list node
    allocator is malloc, the same as agent4007's matched 0x0061B610.
- GetInstance (0x41E5F2) is called out of line. Its landed row exists, so use the header declaration (NUISystem_CManager.h,
  FABLE_RETAIL_CMANAGER as agent4007 uses).
- CCharString and CWideString ctors / dtors are called. Use FABLE_RETAIL_CCHARSTRING and FABLE_RETAIL_CWIDESTRING as all
  siblings do.
- Same-TU neighbours, from egor order and the retail addresses:
  - before: 0x005FA590..0x005FE840 (the CTCInventoryMap rows, ConstructQuestsScreen 0x5FE090, ConstructPastQuestsListPC
    0x5FDDD0, ConstructRegionDescriptionShops 0x6001E0, UpdateVisitedRegions 0x600B60);
  - after: OnCreate 0x00602E30 and ProcessButtonAReleased 0x00602F30.
  - The two list builders are private members of the same class, defined earlier in the TU, but retail CALLS them. They are big,
    so nothing more is needed.

## 6. Sibling landed rows to copy idioms from
1. **rebuild/src/compiled/00/5d/CTCInventoryStats_ConstructExperienceListPC_005d7081.cpp** (lines 66-100).
   - It has the exact statement text of an OBSERVE/IGNORE listen-ignore block:
     `CCountedPointer<CAction> observeUp(new CAction); observeUp->ActionType = ...OBSERVE_EVENT; observeUp->AddParam(scrollListUp, observeUp); observeUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));`
     The ignore one follows, then `list->AddActionOnSelected(ignoreUp); list->AddActionOnInactive(observeUp);`.
   - Its TU is /Os + ("y",off) + CALLED_CTOR. Do NOT copy those.
   - Its defines to copy: FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CWIDESTRING,
     FABLE_RETAIL_STLP_LIST, FABLE_RETAIL_CMANAGER, FABLE_RETAIL_CACTIONPARAMULONG.
2. **CTCInventoryMap_ConstructQuestReviewListPC_005fc8a0.cpp / _ConstructPastQuestsListPC_005fddd0.cpp**: same class and TU, no
   optimize pragma. Defines: FABLE_RETAIL_CCHARSTRING, _CWIDESTRING, _CTCBASE, _CTHING, _STLP_LIST, _CTCINVENTORYBASE,
   _CTCINVENTORYMAP_QUESTREVIEW / _PASTQUESTS.
3. **CTCInventoryAbilities_ConstructPCExpressionCategoryList_005f45b5.cpp:87**: the CreateMenuEntryComponent argument spelling
   `(caption, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD"))`
   (that row is /Os).
4. **/opt/fable_lane/w22/agent4007/src/0061b610.cpp + hdr/**: the matched 12 KB speed-TU sibling (CTCInventoryQuests, the same
   PC/console quest-UI family) with inline CAction ctors. Start the overlay from its hdr/ (NUISystem_CAction.h at least).
5. **NUISystemCManager_Action_0041e6d3.cpp**: the caller, for its CList / CCountedPointer handling and its inline_depth notes.

## 7. Open points for the author
- Do the four listen/ignore groups each sit in their own `{ }` block? The evidence says yes: each group's 4 dtors run before the
  next group's FindChild calls, and the FindChild calls of group N+1 come AFTER group N's destructors. So the up/down FindChild
  pair belongs inside or just before each block, not at the top.
- In groups 3 and 4 (the frame's buttons) the slot use is reversed: AddActionOnSelected(observe), AddActionOnInactive(ignore).
  In groups 1 and 2 (the screen's buttons) it is OnSelected(ignore), OnInactive(observe).
- `if (Data)` vs `if (data)` in the CCountedPointer(T*) ctor (FABLE_RETAIL_CCOUNTEDPOINTER): retail tests the register that
  holds the value it just stored. Both spellings may compile the same; probe.

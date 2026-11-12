# DOSSIER 0x004830A1 (7896 bytes, ends 0x00484F79) -- scout 2026-11-12
(No earlier scout existed: $W/prior/ is empty, agent4011 has empty src/probe/test; nothing to reuse.)
Scout files: $W/scout/ann.txt (full listing + strings + landed callee files), $W/scout/skel.txt (calls/branches/strings
only, 1028 lines, branch targets as L_xxxxxxxx), $W/scout/calls.txt (60 direct callees, count, first site, landed file, label).

## 1. Identity -- the manifest label is WRONG
- Real name: **CTCInGameMenu::OpenInGameMenu()**, mangled `?OpenInGameMenu@CTCInGameMenu@@QAEXXZ` (public, void, no params,
  non-const). Leaf `OpenInGameMenu`.
- Evidence: (a) retail ends `pop edi; pop esi; pop ebx; add ebp,78h; leave; ret` = plain `ret`, no args -- the manifest's
  3-arg OpenInGameMenuPC(CComponent*,CComponent*) would be `ret 8`; (b) Ego_r.pdb `0x0048FB01 ?OpenInGameMenu@CTCInGameMenu@@QAEXXZ`
  sits right after `0x0048CD2B ?OpenInGameMenuPC` (retail: OpenInGameMenuPC 0x004804DE + 11203 = 0x004830A1, i.e. this row
  directly follows it); FableWin.pdb 0x01AA64D9 / thunk 0x017FE748 same name; (c) only caller 0x006362B2 is the landed
  rebuild/src/compiled/00/63/CPlayer_InitModeNavigateInGameMenu_00636270.cpp which already declares
  `void OpenInGameMenu(); // 0x004830A1` and calls `menu->OpenInGameMenu()` (TCI_IN_GAME_MENU 0xC6 GetTC).
- The real OpenInGameMenuPC is 0x004804DE (ghidra_out labels, chunk 880; header CTCInGameMenu.h opt-in FABLE_RETAIL_CTCINGAMEMENU_OPENPC).
- Report back for NAME_FIXES: 0x004830A1 manifest label OpenInGameMenuPC -> OpenInGameMenu.

## 2. Retail shape
- `push ebp ; lea ebp,[esp-78h] ; sub esp,230h ; push ebx ; push esi ; push edi` = biased frame = frame-pointer size TU:
  `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (exactly as landed siblings CloseInGameMenu 0x0047D78B,
  VideoOptionsChanged 0x0047E651, GlobalUpdate 0x0047E9AE; GOTCHAS: "s" alone gives the biased frame too -- siblings use both).
- NO SEH prologue (no fs:0, no EH states) although many CCountedPointer / CWideString / CCharString locals are destroyed:
  the TU is built WITHOUT /EHsc (siblings carry only `// Flags: /GS`). NO /GS cookie in this row (no __security_check_cookie
  call; no char arrays) -- keep `// Flags: /GS` like the siblings anyway (no buffer -> no cookie).
- Register roles: esi = this until ~0x483a7b, then this is reloaded from [ebp+48h]; ebx = 0 for the whole body (`xor edi,edi`
  first then ebx=0 after 0x4833xx); edi = this in the save-slot loop.
- `[ebp+6Ch]` is the compiler's conditional-temporary flag word (bits 1,2,4,8,0x10,0x20 tested before CWideString dtors at
  0x483d67, 0x483f9b/0x483fb0, 0x484941, 0x484a38, 0x484af8): temporaries created inside `&&`/`||`/`?:` -- spell those
  conditions as ONE expression with the call temporaries inline (e.g. `VerifySignatureCached(GetPathNameFromFileName(name,false))`
  inside `a && (b || c)`), do not hoist them to named locals.
- 15 `operator new` sites: `new(0x10) CAction` (4), `new(0xC)` action params (vtable 0x012366F4 value 3 = CActionParamSLONG-like,
  0x012366EC value 2/..., 0x012366FC EBOOL), `new(0x10) CActionParamCCountedPointer<CComponent>` (3).

## 3. Callees (landed file -> PDB name; mismatches flagged)
| addr | landed file | PDB / real name | note |
|---|---|---|---|
| 0x0041D122 | NUISystemCManager_OnOpenMenus_0041d122 | ?OnOpenMenus@CManager@NUISystem@@SIXXZ (static) | ok |
| 0x00485AEF | CTCCarryable_OnKill_00485aef **WRONG NAME** | Signatures.clear(): map<CWideString,bool> (this+0xE0, Ego_r CTCInGameMenu::Signatures) tree clear | flag NAME_FIX; STLport map clear out of line |
| 0x00686D40 | CTCBase_GetPlayerManager | ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ | 2 calls |
| 0x00449980 | global_Catch0042ae0a_00449980 **WRONG NAME** | ?GetMainPlayerJoystickDeviceNumber@CPlayerManager@@QBEJXZ (Ego_r 0x00445140 right after GetMainPlayer; retail after GetMainPlayer 0x00449970; body = GetPlayer(index)->GetJoystickDeviceNumber()) | NAME_FIX |
| GPInputManager 0x013B8388 | | ->PJoysticks (+0x2C, vector<CCountedPointer<CJoystick>>, 8-byte elems)[n]->slot 18 (+0x48) | inline vector index |
| PSoundManager 0x013B8394 | engine/CASoundManager.h | slot 46 (+0xB8) x3 (0x10,0x40,0x80, 0.0f, [0x01237584] float) | `if (PSoundManager)` once, reloaded per call |
| CMainGameComponent::PMainGameComponent 0x013B86A0 | | ->PDisplayEngine(+0x28)->Engine3D(+0x2C) slot 76 (+0x130)(0) | as CloseInGameMenu |
| 0x009E1BC0 | global_GFGetTime | GFGetTime() -> stored to CurStartPauseTime this+0x68 (fstp qword) | |
| 0x00438400 | CPlayerGui_Close | ?Close@CPlayerGui@@QAEX_N@Z, `(1)` on CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass 0x013B8790 | |
| 0x0041E5F2 | NUISystemCManager_GetInstance | NUISystem::CManager::GetInstance | |
| 0x0041DB1D | NUISystemCManager_CreateComponent | CManager::CreateComponent(CCharString by value?, ...) x26 | |
| 0x00428494 | (unlanded) | NUISystem::CManager::CreateMenuEntryComponent (engine_api: CComponent* (CWideString*,float,float,CCharString*,float,CCharString,float,CCharString)) x2 | |
| 0x0099EBF0 / 0x0099EAE0 | CCharString ctor(const char*,long=-1) / dtor | | 78 / 38 |
| 0x0099B510 / B6B0 / B720 / AED0 / B7D0 / B0F0 | CWideString dtor / ctor(const wchar_t*) / copy / default / operator= / operator!= | | |
| 0x0099BF30 / 0x0099BE70 | operator+(CWideString,const wchar_t*) / operator+(CWideString,CWideString) | | |
| 0x00BFE94C | wcscmp | | |
| 0x00404280 / 0x0040F0A0 | std_operator (wstring ==) -- check: they compare CWideString against literal 0x0122D70C | | verify spelling |
| 0x004AF290 | CQuestManager_IsBetweenQuestStartAndFinishScreens | ?IsBetweenQuestStartAndFinishScreens@CQuestManager@@QBE_NXZ on CQuestManager::PClass 0x013B89FC | x5 |
| 0x0040D2A0 | CUserProfileManager_Get | ?Get@CUserProfileManager@@SIAAV1@XZ | x19 |
| 0x00407030 | CThingFilterIsUnconscious_operator (ICF ret-false body) | most probably ?IsDebugProfile@CUserProfileManager@@QBE_NXZ (only Ego_r QBE_NXZ that fits a `bool` gate on the profile; ONE source -- descriptive) | x2 |
| 0x00406A80 | ..GetEmptySlotName | ?GetEmptySlotName@CUserProfileManager@@QBE?AVCWideString@@XZ | x7 |
| 0x00406690 | ..GetAutoSaveFileName | ?GetAutoSaveFileName@...QBE?AVCWideString@@XZ | x2 |
| 0x00406610 | ..GetManualSaveFileName | ?GetManualSaveFileName@...QBE?AVCWideString@@J@Z | x1 |
| 0x00408120 | ..GetPathNameFromFileName | ?GetPathNameFromFileName@...QBE?AVCWideString@@ABV2@_N@Z | x5 |
| 0x00407E10 | ..GetSaveDisplayNameFromFileName | ?GetSaveDisplayNameFromFileName@...QBE?AVCWideString@@ABV2@J@Z | x2 |
| 0x00999230 | CAFile_PathExists | CAFile::PathExists | `.hs` file |
| 0x0047ED0A | CTCInGameMenu_VerifySignatureCached (landed, label FUN_) | ?VerifySignatureCached@CTCInGameMenu@@QAE_NABVCWideString@@@Z | x8, same TU |
| 0x0047FE83 | (unlanded) | ?GetFileNameList@CTCInGameMenu@@AAEXXZ (private; Ego_r) | same TU, earlier; called |
| 0x00415640 | NLocalisation_GetUIGraphics | NLocalisation::GetUIGraphics | |
| 0x009C95E0 | NGameTextCDataBank_GetTextBySymbol | ?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z | x2 |
| 0x0042BE50 | NUISystemCAction_CAction | ??0CAction@NUISystem@@QAE@XZ | x4 |
| 0x0042AA29 | CCountedPointer00429f43_.. | ??0?$CCountedPointer@VCAction@NUISystem@@@@QAE@PAVCAction@NUISystem@@@Z | x4 |
| 0x00429C15 | CCountedPointerNUISystemCComponent_CCountedPointer | ??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAV..@Z | x25 (called ctor: FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR) |
| 0x004291DE / 0x00429160 / 0x00429288 / 0x0042925C / 0x004850F9 | ReleaseData of CCountedPointer<CComponent>/<CAction>/<CList>/<CText>/<CMenuEntry> | landed names: 0x00429288 / 0x0042925C files are named Destval...COptimisedPrimitive (WRONG file names; Leaf says ~CCountedPointer<CList>/<CText>) | dtors = ReleaseData calls: FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA |
| 0x00429486 / 0x00429460 / 0x0048524C | ShareData (CList / CText? / CMenuEntry) -- inline operator=(CCountedPointer<Derived>) / static_cast assignment | | |
| 0x00429C6F | CCountedPointer<CComponent>::Reset | | x2 |
| 0x0042B646 | list<CActionParam*>::push_back | | x3 |
| 0x0053FDB0 | NUISystem::CAction::AddParam(CActionParam*) | ?AddParam@CAction@NUISystem@@QAEXPAVCActionParam@2@@Z | x7 |
| 0x004854DF | CActionParamCCountedPointer<CComponent>(CCountedPointer<CComponent>&) | | x3 |
| 0x0042BED4 | NUISystemCDeletion_CDeletion (label says CAction::CAction -- WRONG label) | ??0CDeletion@NUISystem@@QAE@XZ | x2 |
| 0x0042CD84 | CDeletion copy ctor ??0CDeletion@NUISystem@@QAE@ABV01@@Z | | x2 (by-value arg to SetDeletion slot 62) |
| 0x0042ABCA | list<CComponent*> dtor (CDeletion member) | | x2 |
| 0x00449970 / 0x00487DC0 / 0x0043DBD0 | GetMainPlayer / GetPControlledCreature / IS_THING_ALIVE<CThingPlayerCreature*> | | |
| 0x00410DE0 + 0x004365B0 | bitset<274>::test + CVectorMap::LowerBound = inline CThing::GetTC/PeekTC expansion for TCI 0xB1 = TCI_FISHING (177) | FABLE_RETAIL_CTHING_GETTC_EXPANDED + FABLE_RETAIL_STLP_BITSET_TEST_DECL (as OpenPCOptionsMenu) | |
| 0x00486024 | stdvector_pushback | vector<CComponent*>::push_back on this+0x58 (Components) | called, not expanded |

Component vtable slots used (engine/NUISystem_CComponent.h): +0x0C FindChild(CCharString) [by-value CCharString built at esp],
+0x68 slot 26, +0xA8 ForceState? (used as getter returning ptr: verify -- 0x483498 result deref'ed then +0x244 call; in
OpenPCOptionsMenu the same `+0xA8` returns the UI def / font: read that row), +0xAC Initialise, +0xC0 ChangeState(5),
+0xEC AddChild(CCountedPointer<CComponent>&), +0xF8 SetDeletion(CDeletion), +0x108 AddAction, +0x10C AddActionOnSelected,
+0x14C SetName(const CCharString&), +0x170 MakeUnavailable; viewport +0x238 (one CComponent* arg: AddViewportChild-like as in
OpenPCOptionsMenu), CText +0x240 SetValue(const CWideString&,bool,bool), +0x244 SetValue(const CCharString&,bool,bool);
CList +0x25C/+0x260/+0x270 (slots 151/152/156: unnamed in headers -- take names from rebuild/include/engine/NUISystem_CList.h /
CMenuEntry.h or the OpenPCOptionsMenu row); CYesNo/CMenuEntry +0x25C = AddUIActionParam-like (see CMenuEntry.h line 46-64).

## 4. Members of CTCInGameMenu touched (retail offsets; engine/CTCInGameMenu.h)
- +0x04: CTCBase's owner pointer (pushed to component slot +0x68 at 0x483389) -- in CTCBase.h.
- +0x0C InGameMenuOpen = true (0x48317B) -- header has it.
- +0x28/+0x2C FileNames (vector<CWideString>, begin/end, element 4 bytes) -- header has only pad here: NEEDS an overlay
  `#ifdef FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD` / new opt-in naming `std::vector<CWideString> FileNames; // +0x28` (+0x34
  FileTimes vector<CDateAndTime>, +0x40/+0x4C handle vectors, then Components +0x58 already there).
- +0x58 Components (vector<CComponent*>) push_back -- header has it under FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD.
- +0x68 CurStartPauseTime = GFGetTime() -- has it (SAVELOAD).
- +0xE0 Signatures map<CWideString,bool> (.clear()) -- header SAVELOAD has `unsigned char Signatures[12]` (opaque): needs a typed
  overlay or a call to the out-of-line clear via a declared helper.
- Needed opt-ins: FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCINGAMEMENU, FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD (and a new
  declaration `void OpenInGameMenu(); // 0x004830A1` plus `void GetFileNameList(); // 0x0047FE83` private). Hdr overlay in $W/hdr/.

## 5. Visibility
- Same TU (CTCInGameMenu.cpp, retail 0x0047CFxx..0x00484F79; unaligned starts = size TU): VerifySignatureCached 0x0047ED0A,
  GetFileNameList 0x0047FE83, OpenInGameMenuPC 0x004804DE all defined EARLIER yet called (big bodies, not inlined).
- STLport out of line: map clear 0x00485AEF, vector<CComponent*>::push_back 0x00486024 (keep it a call: FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
  as VideoOptionsChanged, or the plain push_back per hand880 lesson 1), list<CActionParam*>::push_back 0x0042B646, list<CComponent*> dtor.
- CCountedPointer ctors called (FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR), dtors = ReleaseData calls (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA).
- Inline expanded: CThing::GetTC<CTCFishing>-style lookup (FABLE_RETAIL_CTHING_GETTC_EXPANDED), vector size/index, the inline
  `if (p && --p->RefCount==0) p->DeleteFunc` releases (0x4834B3..0x4834C4 pattern `cmp [eax],ebx; jne; call [eax+4]`) on
  CCountedPointer temporaries returned by value from slot +0xA8 -- copy from OpenPCOptionsMenu.

## 6. Sibling rows to copy idioms from
- rebuild/src/compiled/00/58/CTCInventory_OpenPCOptionsMenu_0058af10.cpp (13 KB, same family, matched): defines list at its
  lines 38-68 (CCOUNTEDPOINTER_RELEASEDATA, CCHARSTRING, CWIDESTRING, CTCBASE, CTCINGAMEMENU, CTHING_GETTC_EXPANDED,
  STLP_BITSET_TEST_DECL, STLP_LIST, CCOUNTEDPOINTER_CALLED_CTOR, CACTIONPARAMSLONG/ULONG/EBOOL, CMAINGAMECOMPONENT(_TEXTBANK)),
  `#pragma optimize("s", on)` (plus "y" off -- check its line 6 comment).
- rebuild/src/compiled/00/47/CTCInGameMenu_CloseInGameMenu_0047d78b.cpp (same TU, sound manager, PMainGameComponent engine slot,
  CPlayerGui) -- `optimize("y",off)`+`("s",on)`, Flags /GS.
- rebuild/src/compiled/00/47/CTCInGameMenu_VideoOptionsChanged_0047e651.cpp (same TU, CAction/CYesNo, STLP_VECTOR_PUSHBACK_NOINLINE).
- rebuild/src/compiled/00/47/CTCInGameMenu_GlobalUpdate_0047e9ae.cpp (same TU; GETTC_EXPANDED, CINTELLIGENTPOINTER, CPLAYERMANAGER).
- rebuild/src/compiled/00/47/CTCInGameMenu_ConstructSaveGameFileDescription_0047eee4.cpp, ..._AddRegionAndTimeInfo_0047f58b.cpp.
- Family: /opt/fable_lane/w22/agent4007 (ConstructQuestList, matched; hdr/ overlay with CAction/CCountedPointer/CList opt-ins).

# DOSSIER 0x00598A1C -- NUISystem::CFrontEndManager::BuildComponents(bool)

## 0. Headline
* Manifest label `CFrontEndManager::Init2` is WRONG (made up / propagated). Real name:
  `?BuildComponents@CFrontEndManager@NUISystem@@QAEX_N@Z` = `void NUISystem::CFrontEndManager::BuildComponents(bool)` (public, non-const, __thiscall, `ret 4`).
  Evidence (2 sources): (a) FableWin mangled names `ghidra_out/_fablewin_api.tsv` + `ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv`
  (only CFrontEndManager member taking a single bool and returning void besides CloseVirtualKeyboard / SetQuitFromGame (tiny setters);
  FableWin optimised copy at 0x00605219 is 5280 bytes v our 4928); (b) sole caller 0x0042EA62 in the landed
  `rebuild/src/compiled/00/42/CNewFrontendGameComponent_Init2_0042e98f.cpp` line 117: `m_pFrontEndManager->BuildComponents(!WindowsMediaPlayerInstalled);`
  (its header comment names 0x00598A1C BuildComponents). The bool = "media player is NOT installed" -> show the media-player error screen first.
  No Ego_r entry (egor_pdb_names has no BuildComponents); parameter name unknown (suggest `showMediaPlayerError` / descriptive).
* **Closest model: the MATCHED in-game twin `rebuild/src/compiled/00/58/CTCInventory_OpenPCOptionsMenu_0058af10.cpp`** (0x0058AF10, 13 KB,
  same resolution / anti-aliasing switch / detail-slider / text-slider / refresh-rate code, same callees, same 17-entry jump table).
  Copy its spelling of those four blocks and its #define list (lines 38-68) almost verbatim; only the def names (UI_ vs PC_ prefixes),
  the source of the components (m_mapScreens[...] instead of `list`) and the text bank owner (CNewFrontendGameComponent) differ.

## 1. Retail shape
* Extent 0x00598A1C..0x00599D17 code (0x12FC) + jump table 0x00599D18..0x00599D5B (17 dwords) = 4928 bytes. Next function 0x00599D5C DoPressStart (landed).
* Prologue: `push ebp ; lea ebp,[esp-74h] ; sub esp,0C4h ; push ebx ; push esi ; push edi` -- the biased size-TU frame (twin: `lea ebp,[esp-78h]; sub esp,0E4h`).
  Epilogue `pop edi ; pop esi ; pop ebx ; add ebp,74h ; leave ; ret 4`.
* NO SEH (no fs:0 anywhere) => built without /EHsc (as the twin and all landed CFrontEndManager rows). No /GS cookie.
* Size idioms: `push 2 ; pop edx`, `and dword ptr [ebp+7Ch],0`, `or edi,-1` (edi = -1 = CCharString length default for whole function),
  `xor ebx,ebx` (ebx = 0 until 0x00599169). => `#pragma optimize("s", on)`. The twin uses `("s", on)` alone; most CFrontEndManager siblings add
  `#pragma optimize("y", off)` -- try twin's form first, then add ("y", off) if the prologue differs.
* Param: `[ebp+7Ch]` = arg 1 (bool). Its dword slot is reused at the end as the FRONTEND_SCREEN key temp for map[] and
  `[ebp+7Fh]` (top byte of the param slot) holds the std::allocator temporary of the list ctor (normal VC7.1 slot reuse).
* `this` spilled at [ebp+34h]; esi = &m_mapScreens (this+0x54) from 0x00598B93 to the end.

## 2. Callees (address | landed file | landed name | PDB / correct name)
| addr | x | landed file | PDB / real name | note |
|---|---|---|---|---|
| 005955AB | 1 | 00/59/NUISystemCFrontEndManager_GetProfileNames_005955ab.cpp | ?GetProfileNames@CFrontEndManager@NUISystem@@IAEXAAV?$vector@VCWideString@@...@Z (protected) | ok |
| 0099EBF0 | 45 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long = -1) | ok |
| 0099EAE0 | 34 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | manifest params garbage, name ok |
| 0099EC30 | 2 | 00/99/CCharString_CCharStringcopy_0099ec30.cpp | CCharString(const CCharString&) (by-value FindChild arg from *name) | ok |
| 0059B5D7 | 35 | 00/59/stdmap_operator_0059b5d7.cpp | map<FRONTEND_SCREEN,NUISystem::CComponent*>::operator[] (??A?$map@W4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@...) | out of line, same TU (after DoPressStart) |
| 0041E5F2 | 24 | 00/41/NUISystemCManager_GetInstance_0041e5f2.cpp | NUISystem::CManager::GetInstance() static __fastcall | ok |
| 0041DB1D | 24 | 00/41/NUISystemCManager_CreateComponent_0041db1d.cpp | ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z | always (name, false) |
| 009C95E0 | 3 | 00/9c/NGameTextCDataBank_GetTextBySymbol_009c95e0.cpp | `const CWideString NGameText::CDataBank::GetTextBySymbol(const CCharString&) const` (hidden result) | ok |
| 0099BFF0 | 1 | 00/99/global_operator_0099bff0.cpp | ??H@YI?AVCWideString@@PB_WABV0@@Z  operator+(const wchar_t*, const CWideString&) __fastcall | ok |
| 0099BF30 | 19 | 00/99/global_operator_0099bf30.cpp | operator+(const CWideString&, const wchar_t*) __fastcall | ok |
| 0099BE70 | 4 | 00/99/global_operator_0099be70.cpp | operator+(const CWideString&, const CWideString&) __fastcall | ok |
| 0099B8D0 | 1 | 00/99/CWideString_operator_0099b8d0.cpp | CWideString::operator+=(const CWideString&) | ok |
| 0099B510 | 33 | 00/99/CWideString_Destructor_0099b510.cpp | ~CWideString | ok |
| 0099B7D0 | 15 | 00/99/CWideString_operatorassign_0099b7d0.cpp | CWideString::operator=(const CWideString&) | ok |
| 0099B800 | 1 | 00/99/CWideString_operatorwchartconst_0099b800.cpp | CWideString::operator=(const wchar_t*) | ok |
| 0099AED0 | 1 | 00/99/CWideString_CWideString_0099aed0.cpp | CWideString::CWideString() | ok (out of line, as in twin) |
| 00405F00 | 18 | 00/40/global_GFIntToWideString_00405f00.cpp | ?GFIntToWideString@@YI?AVCWideString@@J@Z (__fastcall, edx = value) | manifest says CGuiControlNumber -- wrong, twin declares the free fn |
| 00BFEA1A | 1 | 00/bf/global_operatornew_00bfea1a.cpp | operator new(unsigned) | `new CAction` (0x10 bytes) |
| 0042BE50 | 1 | 00/42/NUISystemCAction_CAction_0042be50.cpp | ??0CAction@NUISystem@@QAE@XZ | manifest "CRumbleDef" is wrong |
| 0042AA29 | 1 | 00/42/CCountedPointer00429f43_CCountedPointer00429f43_0042aa29.cpp | CCountedPointer<NUISystem::CAction>::CCountedPointer(CAction*) | **landed name `CCountedPointer_00429f43` is a placeholder; twin calls it CCountedPointer<CAction>(CAction*)** |
| 00429160 | 1 | 00/42/CCountedPointer_ReleaseData_00429160.cpp | ?ReleaseData@?$CCountedPointer@VCAction@NUISystem@@@@QAEXXZ (the ~CCountedPointer<CAction> body) | ok |
| 00429C15 | 2 | 00/42/CCountedPointerNUISystemCComponent_CCountedPointer_00429c15.cpp | ??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z | out of line |
| 004291DE | 2 | 00/42/CCountedPointerNUISystemCComponent_ReleaseData_004291de.cpp | ?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ | dtor body |
| 0040D2A0 | 2 | 00/40/CUserProfileManager_Get_0040d2a0.cpp | ?Get@CUserProfileManager@@SIAAV1@XZ | ok |
| 00406880 | 1 | 00/40/CModeBlock_GetAnimFilter_00406880.cpp | ?GetDisplayModes@CUserProfileManager@@QBEABV?$vector@VCDisplayModeDesc@@...@XZ (folded `lea eax,[ecx+80h]`) | **landed name CModeBlock::GetAnimFilter is a fold; use GetDisplayModes (twin does)** |
| 009A4EC0 | 1 | 00/9a/global_GFGetSystemManager_009a4ec0.cpp | ?GFGetSystemManager@@YIPAVCSystemManager@@XZ | then ->GetDisplayManager() = [eax+60h] inline |
| 009C05C0 | 1 | 00/9c/CDisplayManager_EnumerateAntiAliasingModes_009c05c0.cpp | ?EnumerateAntiAliasingModes@CDisplayManager@@QAEXAAV?$CArray@W4ESurfaceMultisampleType@@@@@Z | ok |
| 00594FDB | 1 | 00/59/CFrontEndManager_GenerateAboutMessage_00594fdb.cpp | CWideString CFrontEndManager::GenerateAboutMessage() (hidden result) | Leaf line just `GenerateAboutMessage` |
| 00556570 | 1 | 00/55/CObservable_AddExclusiveObserver_00556570.cpp | NUISystem::CRedefinerList::SetAcceptButton(CComponent*) | **landed name AddExclusiveObserver / manifest SetMember_dword0x18c are wrong; twin + opt-in FABLE_RETAIL_CREDEFINERLIST_SETACCEPTBUTTON say SetAcceptButton -> NAME_FIXES candidate** |
| 004292C0 | 5 | 00/42/stdRbtreebase_Rbtreeincrement_004292c0.cpp | STLport _Rb_global<bool>::_M_increment(_Rb_tree_node_base*) (cdecl, `push node; call; pop ecx`) | map-iterator ++ |
| 0044BFF0 | 7 | 00/44/stdvector_pushback_0044bff0.cpp | vector<CCharString>::push_back(const CCharString&) | out of line |
| 0043336A | 1 | 00/43/stdvectorCCharString_erase_0043336a.cpp | vector<CCharString>::erase(iterator, iterator) | = names.clear() |
| 0042ABAF | 1 | 00/42/stdlistNUISystemCComponent_list_0042abaf.cpp | ??0?$list@PAVCComponent@NUISystem@@...@QAE@ABV?$allocator@...@Z | out of line, allocator temp at [ebp+7Fh] |
| 0042ABCA | 1 | 00/42/stdlistNUISystemCComponent_list_0042abca.cpp | ~list<CComponent*> | ok |
| 00414EA0 | 1 | 00/41/stdvectorCCharString_vector_00414ea0.cpp | ~vector<CCharString> | ok |
| 00412130 | 1 | 00/41/CCombatWheel_ResetRings_00412130.cpp | ~vector<CWideString> (fold; LoadProfile row says so) | landed file name is the fold |
| 0059899A | 1 | 00/59/NUISystemCFrontEndManager_LoadProfile_0059899a.cpp | ?LoadProfile@CFrontEndManager@NUISystem@@IAEXABVCWideString@@@Z | immediately precedes this row (same TU) |
| 0059672A | 1 | 00/59/NUISystemCFrontEndManager_InitScreenStack_0059672a.cpp | ?InitScreenStack@CFrontEndManager@NUISystem@@QAEXPAVCComponent@2@@Z | ok |
| 00BFEA14 | 1 | 00/bf/global_free_00bfea14.cpp | free | inline CArray dtor `if (Data) free(Data)` |

Virtual slots (CComponent vtable, rebuild/include/engine/NUISystem_CComponent.h; same set the twin uses):
+0x0C slot 3 FindChild(CCharString) (by value: `push ecx; mov ecx,esp` + ctor into the arg); +0x18 slot 6 FindAllChildrenOfType(EType, list<CComponent*>&);
+0x34 SetColour(const CRGBColour&); +0x38 GetColour() const (hidden result); +0xAC Initialise(); +0xD0 GetParent() const; +0xEC AddChild(CCountedPointer<CComponent>&);
+0x11C slot 71 AddActionOnLeftClicked(CCountedPointer<CAction>&); +0x148 GetStates() -> map<unsigned long,CUIState>&; +0x174 MakeUnavailableDynamically();
+0x184 FindFirstChildOfTypeNonRecursive(EType) (arg 6 = UI_TYPE_TEXT); CObserver base (+4) slot 7 (+0x1C) SetPreventObserving(bool);
CText (NUISystem_CText.h): +0x238 slot 142 `const CWideString& GetValue() const`, +0x240 slot 144 `SetValue(const CWideString&, bool, bool)`.

## 3. Members / globals
* this (NUISystem::CFrontEndManager, engine/CFrontEndManager.h): +0x14 MediaPlayerErrorString (CWideString), +0x54 m_mapScreens
  (std::map<FRONTEND_SCREEN,CComponent*>), +0xC0 m_bQuitFromGame (bool). Opt-ins: `FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR` (full class with
  real containers -- the one to use, landed by 0x005954AD) or `FABLE_RETAIL_CFRONTENDMANAGER` (padded; no map). Neither declares
  BuildComponents / GetProfileNames / LoadProfile / InitScreenStack / GenerateAboutMessage: add them in the $W/hdr overlay under the opt-in.
  FRONTEND_SCREEN enum in engine/FRONTEND_SCREEN.h (all values used below exist; -1 = FRONTEND_SCREEN_MEDIA_PLAYER_ERROR).
* CNewFrontendGameComponent::m_pNewFrontendGameComponent 0x013B871C; `CNewFrontendGameComponent::Get()->GetTextBank()` inline
  (engine/CNewFrontendGameComponent.h: m_pTextBank +0x60, reads `mov eax,[13B871Ch]; mov ecx,[eax+60h]`). Twin used CMainGameComponent instead.
* CUserProfileManager (engine/CUserProfileManager.h): ProfileName +0x08 via inline `?GetProfileName@CUserProfileManager@@QBEABVCWideString@@XZ`
  (FableWin/Ego_r; NOT in the header -- add to overlay); GetDisplayModes out of line under `FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYMODES`.
* CDisplayModeDesc (16 B): Dimensions.X +0, .Y +4, BitsPerPixel +8, RefreshRate +0xC.
* CSlider (engine/NUISystem_CSlider.h, needs FABLE_RETAIL_C2DVECTOR): Min +0x18C, Max +0x194 (compare .X). CTextSlider: MaxChild +0x19C.
* CUIState.Colour +0x18 (node +0x2C, alpha byte node+0x2F); CRGBColour A = byte +3 (`shr byte,1` = `A /= 2`).
* CAction (engine/NUISystem_CAction.h, needs FABLE_RETAIL_STLP_LIST): ActionType +0 = 229 UI_ACTION_TYPE_GO_TO_MAIN_MENU_FROM_START.
* CArray<ESurfaceMultisampleType> (CDisplayManager.h under FABLE_RETAIL_CDISPLAYMANAGER_ANTIALIASING): 12 B at [ebp+18h], inline zero ctor, inline `if(Data) free(Data)` dtor.
* Wide literals: L"X" 0x01238478, L"TEXT_GUI_MENU_OFF" 0x012511CC (assigned as TEXT, not looked up -- same as twin), L" " 0x0122D820, L"\n\n" 0x01249F28.
* Old local models: none (no draft). The landed CFrontEndManager siblings mostly model CCharString/CWideString/CFrontEndManager LOCALLY;
  do NOT copy that -- use the shared headers like the twin.

## 4. Visibility / opt-ins (copy the twin's list, lines 38-68)
* `FABLE_RETAIL_STLP_MAP` -- map::operator[] body visible (retail instantiates it in THIS TU at 0x0059B5D7, out of line). Required for the
  key-temp slot reuse ([ebp+6Ch] key then reused for the returned reference; [ebp+7Ch] param slot reused as the final key) -- lesson of GotoPreviousScreen 0x00597BF2.
* `FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR` (CCountedPointer<T>(T*) out of line: 0x0042AA29, 0x00429C15) + `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA` (dtor calls ReleaseData).
* `FABLE_RETAIL_STLP_LIST` (list<CComponent*> ctor/dtor out of line, CAction layout), `FABLE_RETAIL_CREDEFINERLIST_SETACCEPTBUTTON`,
  `FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYMODES`, `FABLE_RETAIL_CDISPLAYMANAGER` + `_ANTIALIASING`, `FABLE_RETAIL_C2DVECTOR`, `FABLE_RETAIL_CCHARSTRING`, `FABLE_RETAIL_CWIDESTRING`.
* Neighbours in the same TU: LoadProfile 0x0059899A (just before), DoPressStart 0x00599D5C (just after), Update 0x00599E3F, Action 0x0059A238, map[] 0x0059B5D7 -- all out of line.

## 5. Siblings to copy idioms from
* rebuild/src/compiled/00/58/CTCInventory_OpenPCOptionsMenu_0058af10.cpp -- THE twin (no "Flags:" line; `#pragma optimize("s", on)`; defines above). Its Shape notes:
  AA switch has its own `case SURFACE_MULTISAMPLE_NONE` (so table starts at 0) + default with the same text; CText named after the
  IsNull test; colour is a named local; `A /= 2`.
* 00/59/CFrontEndManager_GotoPreviousScreen_00597bf2.cpp -- `("s",on)` + `("y",off)`, FABLE_RETAIL_STLP_MAP for m_mapScreens[] (key-slot reuse explanation).
* 00/59/NUISystemCFrontEndManager_CFrontEndManager_005954ad.cpp -- `// Flags: /Os`, FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR + COBSERVER_DESTRUCTOR (shared class model).
* 00/59/CFrontEndManager_GenerateAboutMessage_00594fdb.cpp, 00/59/NUISystemCFrontEndManager_LoadProfile_0059899a.cpp -- `("s",on)`+`("y",off)`, plain no-EH.
* 00/42/CNewFrontendGameComponent_Init2_0042e98f.cpp -- the caller (declares BuildComponents(bool)).

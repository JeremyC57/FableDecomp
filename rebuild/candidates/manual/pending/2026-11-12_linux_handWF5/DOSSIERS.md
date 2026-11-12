## 0x009ADAE0

# DOSSIER 0x009ADAE0 (417 bytes) -- CDefinitionManager::InstantiateSubDef

## 1. Retail shape (xdis 009adae0 0x1b1)
- __thiscall, `ret 0xc` = hidden result ptr + className + parentIndex. No frame pointer (no push ebp/mov ebp,esp): /Oy on.
  No SEH frame (no fs:0) although CDefPointer locals have dtors -- same as landed sibling InstantiateDef 0x009ADC90 (Flags: none).
- `sub esp,8`: exactly two locals: [esp+0x14 after pushes] = spilled `this`, [esp+0x10] = shared temp (CDefString class-name temp,
  then the push_back value temp of the record pointer, then of global index).
- `info` (GetPDefClassInfo result) is homed in the dead className arg slot ([esp+0x20]); the `__true_type` tag argument of both
  _M_insert_overflow calls is the address of the parentIndex arg slot (`lea ecx,[esp+0x2c]` after two pushes = [esp+0x24]);
  parentIndex itself is re-read from [esp+0x24] at 0x9adbe5.
- Two epilogues; the miss (info==0) path is placed last (0x9adc3d): `if (info) {...return} ...return null` like InstantiateDef.
- Order on hit: global_index = InstantiatedDefs.size() (ebx, sar 2), class_index = info->GlobalIndices.size() (edi), THEN
  AllocFunc call (`call [eax+4]`, no args, __fastcall per header), Template bit cleared (`and al,0xfd` = bit1 of +0x24),
  PDefManager=this (+0x1c), GlobalIndex=ebx (+0x20). No increment after the alloc => adopting ctor CDefPointer(T*).
- operator new(0x10) then, inside the non-null branch, CDefString::SetString(info) into the [esp+0x10] temp
  (ctor args evaluated after allocation), copied to rec+0, rec+4 = -1 (default CDefString), rec+8 = class_index, rec+0xc = def + inc.
- push_back(record) on InstantiatedDefs (grow 0x009B4BD0, ecx=edi=&InstantiatedDefs), push_back(global_index) on
  info->GlobalIndices (grow 0x00464ACE). Both through a memory temp at [esp+0x10].
- Inline GetPDefFromGlobalIndex(parentIndex): LoadDef(parentIndex) 0x009AD6E0, copy InstantiatedDefs[parentIndex]->PDef
  (`test edi,edi ; je ; inc [edi+4]`), then parent->AddSubDef(info->Name, global_index, parentIndex) = vtable +0x30 (slot 12),
  args pushed parentIndex, ebx, info.
- Result: `mov [ebx],esi ; inc [esi+4]` (return def copy), then parent dtor (`dec [edi+4] ; jne ; call [edx+4]`, no null test),
  then def dtor (`dec [esi+4]`, no null test).
- Miss path: LoadDef(parentIndex); temp copy of InstantiatedDefs[parentIndex]->PDef with null test, increment via edx
  (`mov edx,[ecx+4]; inc edx; mov eax,edx; dec eax; mov [ecx+4],edx; mov [ecx+4],eax; jne -> call [eax+4]`) = copy + immediate
  release of a discarded GetPDefFromGlobalIndex temporary; then `*result = 0`.

## 2. Name / signature (two sources, manifest label CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 00b05ad0) and FableWin.pdb (_fablewin_api.tsv / naming_stragglers 017dd656, 03061140):
  `?InstantiateSubDef@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@ABVCCharString@@K@Z`
  => protected: CDefPointer<CDefClassBase> InstantiateSubDef(const CCharString&, unsigned long)  (non-const).
- Callers (xref.py): 0x009AE1D9, 0x009AE27B, both inside CDefinitionManager::CompileSubDefinition 0x009AE150 (landed,
  rebuild/src/compiled/00/9a/CDefinitionManager_CompileSubDefinition_009ae150.cpp, local model declares a non-template
  `CDefPointer InstantiateSubDef(const CCharString& name, unsigned long parentIndex)` -- template arg missing there; NAME_FIXES candidate).
- Not declared in rebuild/include/engine/CDefinitionManager.h: must be added (inside the FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
  block, next to InstantiateDef) or a new nested opt-in.

## 3. Callees
| addr | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x009AD580 | 00/9a/CDefinitionManager_GetPDefClassInfo_009ad580.cpp | GetPDefClassInfo | ?GetPDefClassInfo@CDefinitionManager@@IAEPAVCDefClassInfo@1@ABVCCharString@@@Z (header) | draft `FindClassInfo` WRONG |
| [info+4] | -- | CDefClassInfo::AllocFunc (CDefClassInit::TAllocFunc, __fastcall) | | draft `Create` WRONG |
| 0x00BFEA1A | 00/bf/global_operatornew_00bfea1a.cpp | operator new (toolchain_library) | ??2@YAPAXI@Z | ok |
| 0x009D5060 | 00/9d/CDefString_SetString_009d5060.cpp | CDefString::SetString | ?SetString@CDefString@@QAEXABVCCharString@@@Z | draft `Set` WRONG |
| 0x009B4BD0 | 00/9b/stdvector_Minsertoverflow_009b4bd0.cpp | vector<CUnprovenElement_009b4bd0*>::_M_insert_overflow | element is CDefinitionManager::CInstantiatedDefInfo* (callers InstantiateDef/InstantiateSubDef) -- landed Leaf uses placeholder, NAME_FIXES candidate | draft modelled locally |
| 0x00464ACE | 00/46/stdvectorunsignedlong_Minsertoverflow_00464ace.cpp | vector<unsigned long>::_M_insert_overflow | ok | local |
| 0x009AD6E0 | 00/9a/CDefinitionManager_LoadDef_009ad6e0.cpp | LoadDef(unsigned long) non-const | ?LoadDef@CDefinitionManager@@IBEXK@Z = protected CONST | draft non-const WRONG |
| vtbl +0x30 | slot 12 0x0042DA30 | CDefClassBase::AddSubDef(const CCharString&, long, long) (CDefClassBase.h l.92; CParentDefClassBase override ?AddSubDef@CParentDefClassBase@@UAEXABVCCharString@@JJ@Z) | draft `AddSubDef(const CCharString&, unsigned long, unsigned long)` -- args are long,long |
| vtbl +4 | slot 1 | CIVCountedPointeeBase::Delete (via CDefPointer::Release) | draft `Release` |
- GetPDefFromGlobalIndex(long) (protected, non-const, Ego_r 00b05970 `?GetPDefFromGlobalIndex@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z`)
  is INLINED here (twice); retail also has out-of-line COMDATs 0x009AD9E0 / 0x009ADA10 (landed, local models). Header only declares it:
  author must supply an inline body `{ LoadDef(index); return InstantiatedDefs[index]->PDef; }` visible in the TU.

## 4. Members
All in shared headers; draft models ALL of these locally (must switch): CDefString, CArray, __true_type, CDefClassBase,
CDefPointer (non-template!), CInstantiatedDefInfo, CDefClassInfo, CDefinitionManager.
- CDefinitionManager.h (opt-in FABLE_RETAIL_CDEFINITIONMANAGER + _INSTANTIATEDEF): InstantiatedDefs +0xa4 CArray<CInstantiatedDefInfo*>;
  nested CDefClassInfo { CCharString Name +0; TAllocFunc AllocFunc +4; CArray<unsigned long> GlobalIndices +8 };
  nested CInstantiatedDefInfo { CDefString ClassName +0; CDefString InstantiationName +4; unsigned long ClassIndex +8;
  CDefPointer<CDefClassBase> PDef +0xc }, ctor (CDefString, CDefString, unsigned long, const CDefPointer<CDefClassBase>&) = FableWin 0x03061650.
- CDefClassBase.h (FABLE_RETAIL_DEF_CLASS_BASE): PDefManager +0x1c, GlobalIndex (unsigned long) +0x20, bitfields uchar Setup:1, Template:1,
  DefaultValsApplied:1 at +0x24 (struct_layouts_egor.tsv agrees). IVRefCount +4 in CIVCountedPointeeBase.h (AddRef inline).
- CDefPointer.h: FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT -- its comment already cites THIS row (no inc after 0x009ADB22, copies at
  0x009ADB64/0x009ADC16, dtor 0x009ADC25). Release variants: default, _RELEASE_SELFDELETING_POINTEE, _RELEASE_COUNTREF, _RELEASE_POINTEE.
- CDefString.h: FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING (inline CDefString(const CCharString&){SetString}), default ctor TablePos(-1)
  (needs FABLE_RETAIL_CDEFSTRINGTABLE, FABLE_RETAIL_CCHARSTRING).

## 5. Visibility
- Inline in this TU (must be): CDefPointer ctors/dtor, CInstantiatedDefInfo ctor, CArray::size/push_back, CDefString ctor,
  GetPDefFromGlobalIndex(long) (both expansions inline; out-of-line copies exist elsewhere at 0x009AD9E0/0x009ADA10).
- Out of line: GetPDefClassInfo, LoadDef, SetString, both _M_insert_overflow (STLport grow paths called, i.e. push_back inline).
- Same TU neighbours (definitions.cpp order): GetPDefClassInfo 0x9AD580 < LoadDef 0x9AD6E0 < ClearInstantiatedDefs 0x9AD950 <
  GetPDefFromGlobalIndex 0x9AD9E0/0x9ADA10 < GetPDefFromInstantiationName 0x9ADA40/0x9ADA90 < THIS < InstantiateDef 0x9ADC90.
  LoadDef body defined earlier in TU, but it is called (not inlined) -- no opt-in exists for its body; declaration suffices
  (InstantiateDef matched with declarations only).

## 6. Siblings
- 0x009ADC90 InstantiateDef (rebuild/src/compiled/00/9a/CDefinitionManager_InstantiateDef_009adc90.cpp): MATCH, `// Flags: none`,
  no pragmas. Defines: FABLE_RETAIL_CCHARSTRING, _CDEFSTRINGTABLE, _CDEFSTRING_CTOR_SETSTRING, _CSYMBOLMAP, _CSYMBOLMAP_ADDSYMBOLDIRTY,
  _DEF_CLASS_BASE, _CDEFPOINTER_POINTER_CTOR_ADOPT, _CRESOURCELIST, _CRESOURCE, _CDEFINITIONMANAGER, _CDEFINITIONMANAGER_INSTANTIATEDEF;
  includes fable_string.h, engine/CDefString.h, engine/CDefClassBase.h, engine/CDefinitionManager.h. Nearly identical body -- COPY IT:
  `long global_index = InstantiatedDefs.size(); long class_index = info->GlobalIndices.size(); CDefPointer<CDefClassBase> pdef(info->AllocFunc());
  ... CInstantiatedDefInfo* def_info = new CInstantiatedDefInfo(info->Name, <name>, class_index, pdef); InstantiatedDefs.push_back(def_info);
  info->GlobalIndices.push_back(global_index);` Its notes: long indices -> conversion temp in dead arg slot; record built in its own statement.
- 0x009ADA90 GetPDefFromInstantiationName: local model, `return InstantiatedDefs[index]->PDef;` / `return CDefPointer<CDefClassBase>();`.
- 0x009AD580 GetPDefClassInfo: `#pragma auto_inline(off)` around a noinline CVectorMap::LowerBound template only.
- 0x009AE150 CompileSubDefinition: the caller, local model.

## 7. Old draft model errors
- Everything modelled locally (non-template CDefPointer, own CArray/__true_type, CDefClassBase with fake v00..v11) -> use shared headers.
- `CDefPointer def(info->Create(), 0)` -> `CDefPointer<CDefClassBase> pdef(info->AllocFunc());` (adopting ctor).
- `def->Recompiled = false` on bool bitfields -> header `Template` (unsigned char :1, bit1). Retail is `pdef->Template = false;`
  (load / and 0xfd / store interleaved with `push 0x10`).
- Record built inline inside push_back with a custom 3-arg ctor that does `++def->RefCount` -> use the header 4-arg ctor:
  `new CInstantiatedDefInfo(info->Name, CDefString(), class_index, pdef)` (InstantiationName -1 = default CDefString), in its own statement.
- `info->Name()` cast accessor -> `info->Name` member.
- GetPDefFromGlobalIndex inline body + LoadDef must be `const` member (IBEXK); AddSubDef args long,long.
- `long globalIndex` already right (LESSONS.md: long vs unsigned long changes the push_back temp); class_index should also be `long`
  (InstantiateDef uses long for both).
- Draft frame 0x10 vs retail 8: retail shares one 4-byte temp for CDefString + both push_back values; the separate `def_info`
  statement form of InstantiateDef is what made that sharing work there.

## 0x00499AA0

# DOSSIER 0x00499AA0 CProgressDisplay::StartProgress (418 bytes)

## 1. Retail shape (xdis 00499aa0 0x1b2)
- __thiscall, `ret 18h` (6 bool args). No frame pointer (no push ebp), no SEH, `sub esp,8` (one 8-byte frame temp at [esp+8]/[esp+0Ch] = the GetFont result) + CCharString slot at [esp+14h] shared by both strings. ebx = 0 cached. /O2 speed TU (no optimize("s") idioms; plain jcc/fld/fstp). Entry 16-aligned.
- Arg mapping (esp+14h.. after sub8/push ebx/push esi): a1 -> FullScreen +7A, a2 -> ForRelaunch +7E, a3 -> ForFrontEnd +7F, a4 -> BlackScreen +7B, a5 -> ForFileInstaller +80, a6 -> last arg of RenderProgress. InitialiseTextures(a2, a3) via own vtable +10h (slot 4).
- Flow: if(!0x01375445) return; if(!0x013B89D3){ if(Initialised +79) return; if(0x013B89C1) 0x013B89C8 = GFGetTime()+3.0 (0x012368F0); } ... matches old draft.
- GetFont block: CCharString(lit 0x01231168 "ENG_ARIAL_16", -1) at [esp+14h]; GFGetSystemManager(); mov eax,[eax+84h]; push &name; push &tmp([esp+8]); call GetFont 0x009E2C80; `push eax` (returned pointer, NOT lea of a named local); mov ecx,0x013B8998; call 0x00419463 (operator= OUT OF LINE). Then INLINE release of the temp reloading [esp+0Ch] each time (dec; reload; cmp; ecx=[eax+8]; call [eax+4]; reload; push; call 0x00BFE9BC), then lea ecx,[esp+14h]; **mov [esp+8],ebx ; mov [esp+0Ch],ebx** (Data=0 then Info=0, the default dtor order) ; call ~CCharString 0x0099EAE0.
- Then if(!DebugDisplay +78){ CCharString(lit 0x01238888 "Initialising progress display",-1) at [esp+14h]; RenderProgress(str, -1.0f (push 0BF800000h), false (ebx), a6); ~CCharString }.

## 2. Name / signature (two sources)
- vtable 0x012388B4 slot 1 = 0x00499AA0 (rebuild/manifest/rtti-vtables.tsv); Ego_r.pdb ?StartProgress@CProgressDisplay@@UAEX_N00000@Z (egor_pdb_names.tsv 004a46b0) and FableWin.pdb same symbol (_fablewin_api.tsv 01813616/018adad0). Shared header rebuild/include/engine/CProgressDisplay.h already declares `virtual void StartProgress(bool,bool,bool,bool,bool,bool); // 0x00499AA0` (base CAProgressDisplay slot 1, pure).
- Mangled leaf: ?StartProgress@CProgressDisplay@@UAEX_N00000@Z  -> `void CProgressDisplay::StartProgress(bool,bool,bool,bool,bool,bool)`, public virtual, non-const.
- Manifest label `CProgressDisplay::CProgressDisplay::StartProgress` doubles the class; correct is CProgressDisplay::StartProgress. Only xref: the vtable slot (0x012388B8).
- Parameter names are not in the PDB; draft names (fullScreen, forRelaunch, forFrontEnd, blackScreen, forFileInstaller, renderFlag) are descriptive, derived from the member each is stored into.

## 3. Callees
| addr | landed row | landed name | PDB name | draft spelling issue |
|---|---|---|---|---|
| 0x009E1BC0 | 00/9e/global_GFGetTime_009e1bc0.cpp | double __fastcall GFGetTime() | ?GFGetTime@@YINXZ | ok |
| 0x0099EFE0 | 00/99/CCharString_operator_0099efe0.cpp | CCharString::operator=(const char*) | ??4CCharString (fable_string.h: `const CCharString& operator=(const char*)`) | draft returns CCharString& (non-const) -- use fable_string.h |
| 0x0099EBF0 / 0x0099EAE0 | CCharString_Constructor_0099ebf0 / CCharString_CCharString_0099eae0 | CCharString(const char*, long=-1) / ~CCharString | ??0CCharString@@QAE@PBDJ@Z | use fable_string.h + FABLE_RETAIL_CCHARSTRING (default -1) |
| CMainGameComponent vtable slot 9 (+24h) = 0x004197B0 | 00/41/CMainGameComponent_IsEditorActive_004197b0.cpp | IsEditorActive | ?IsEditorActive@CMainGameComponent@@UBE_NXZ (FableWin 0182730a; header comment) | **draft: `IsDebugDisplayForced()` -- WRONG, and missing const** |
| 0x013B86A0 (global) | -- | | ?PMainGameComponent@CMainGameComponent@@1PAV1@A (labels: CMainGameComponent::PMainGameComponent) | **draft: g_pMainGameComponent -- WRONG**; use CMainGameComponent::Get() / PMainGameComponent |
| own slot 4 | 0x0049A0B0 | InitialiseTextures(bool,bool) | ?InitialiseTextures@CProgressDisplay@@UAEX_N0@Z | ok |
| 0x00A0FDB0 | 00/a0/global_WordWrapSetOption_00a0fdb0.cpp | WordWrap_SetOption(unsigned int, unsigned long) (leaf YIXIK) | ?WordWrap_SetOption@@YIXII@Z (Ego_r 00b796a0, FableWin 01836332): (unsigned int, unsigned int) | landed row's 2nd param type disagrees with PDB (NAME_FIXES-worthy, bytes unchanged); call is (0, 1) ecx=0 edx=1 |
| 0x009A4EC0 | 00/9a/global_GFGetSystemManager_009a4ec0.cpp | CSystemManager* __fastcall GFGetSystemManager() | | declared in engine/CSystemManager.h:95 |
| 0x009E2C80 | 00/9e/CFontManager_GetFont_009e2c80.cpp | CFontManager::GetFont | ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z | **draft: `CFontBankBank::GetFont` / `CSystemManager::FontBank` -- WRONG**; real: GFGetSystemManager()->PFontManager->GetFont(...) (CCountedPointer<CFontManager> +84h, operator-> inline) |
| 0x00419463 | 00/41/CCountedPointerCTexture_operator_00419463.cpp | (file mislabelled CTexture; labels_*.tsv says CCountedPointer<CFontBank>::operator=) | ??4?$CCountedPointer@VCFontBank@@@@QAEAAV0@ABV0@@Z (FableWin 017f946e/018aa587) | ok in draft; it is a COMDAT from the CMainGameComponent TU (0x41xxxx, unaligned), forwards to ShareData 0x004190E2 |
| 0x00BFE9BC | global_operatordelete_00bfe9bc.cpp | operator delete (import thunk) | | inline in the counted-pointer release |
| 0x00499880 | 00/49/CProgressDisplay_RenderProgress_00499880.cpp | RenderProgress | ?RenderProgress@CProgressDisplay@@AAEXABVCCharString@@M_N1@Z (private) | ok |

## 4. Members / globals
- Shared header rebuild/include/engine/CProgressDisplay.h (sizeof 0x88, no pack needed: vptr + 3 STLport vectors 0xC, doubles 8-aligned at +60). It names every member touched: LoadingScreenIndex +28 long, TextTagIndex +44 long, TextTag +4C / LastTextDisplayed +50 CCharString, GuildSealFrameIndex +54, StageCount +58, LastUpdateTime +60 / LastScreenUpdate +68 / LastProgressUpdate +70 double, DebugDisplay +78, Initialised +79, FullScreen +7A, BlackScreen +7B, DisplayText +7C, DisplayResetProgressTipText +7D, ForRelaunch +7E, ForFrontEnd +7F, ForFileInstaller +80, FileInstallerFinished +81; static PFont CCountedPointer<CFontBank> 0x013B8998. It self-defines FABLE_RETAIL_CAPROGRESSDISPLAY and needs FABLE_RETAIL_CTEXTURE.
- CSystemManager.h: PFontManager at +84h needs FABLE_RETAIL_CSYSTEMMANAGER + FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER (without the nested macro it sits at +7C).
- CMainGameComponent.h: IsEditorActive slot 9 needs FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR; static PMainGameComponent + `static Get()` (line 175, under FABLE_RETAIL_STEALTH_DRAW? check) -- landed 0x0049DAA0 CWorld::GetCurrentSoundCamera uses `CMainGameComponent::Get()->IsEditorActive()`.
- CFontManager.h: GetFont declared (no layout). fable_string.h: CCharString.
- Globals (no PDB data names found; landed spellings): 0x01375445 g_bProgressDisplayEnabled (also g_ProgressDisplayEnabled in StopProgress -- inconsistent), 0x013B89D3 g_bProgressDisplaySuppressed, 0x013B89C1 g_bAutoSaveProgressShown (DrawRetailDisplay), 0x013B89C8 g_AutoSaveProgressEndTime, 0x013B85F0 g_ProgressDebugDisplay (draft: g_bProgressDebugDisplay -- differs from the one landed spelling), 0x013B8648 g_bCompileFrontendDefinitions (majority). Use the DrawRetailDisplay 0x00498490 spellings where present.
- Draft models LOCALLY (all have shared headers): CCharString (fable_string.h), CCPPointerInfo/CCountedPointer (engine/CCountedPointer.h), CSystemManager (engine/CSystemManager.h), CFontBankBank (= CFontManager, engine/CFontManager.h), CMainGameComponent (engine/CMainGameComponent.h), CProgressDisplay (engine/CProgressDisplay.h).

## 5. Visibility
- CCountedPointer<CFontBank>::operator= is CALLED (0x00419463) though the dtor is expanded. engine/CCountedPointer.h has NO decl-only opt-in for operator= (only CALLED_CTOR, VISIBLE_CALLED_CTOR/DTOR, DTOR_DECL_ONLY, RELEASEDATA(_DECL_ONLY), RESET_*). With the shared header VC7.1 will expand operator= (as it does in CFrontendGameComponent::InitGraphics 0x00494840). Author needs a new per-TU opt-in (e.g. FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY, or a __declspec(noinline) visible body like VISIBLE_CALLED_DTOR) -- coordinator header change. The visible-noinline variant matters: a visible body lets c2 see operator= does not keep the address.
- Dtor must stay inline default body (Data=0 then Info=0, matches retail store order).
- Same TU (FableWin CProgressDisplay.cpp TU 0x018AD6E0..0x018B5330): RenderProgress 0x00499880 is defined earlier (immediately before, 0x00499880 < 0x00499AA0) -- retail CALLS it (private, 489 bytes, not inlined). InitialiseTextures is called virtually.
- No STLport helpers called.

## 6. Siblings (same TU, landed)
- 00/49/CProgressDisplay_DrawRetailDisplay_00498490.cpp -- best model: shared engine/CProgressDisplay.h, CSystemManager, CMainGameComponent, CFontBank includes; defines FABLE_RETAIL_CCHARSTRING, _CWIDESTRING, _CTEXTURE, _CSYSTEMMANAGER, _CMAINGAMECOMPONENT(+_TEXTBANK), _CFONTBANK etc.; no pragmas; global spellings g_bAutoSaveProgressShown / g_AutoSaveProgressEndTime / g_bProgressDisplaySuppressed.
- 00/49/CProgressDisplay_RenderProgress_00499880.cpp (local pack(4) model, CBaseClass include), _DisplayProgress_00499c50.cpp (local model; globals g_bProgressDisplayEnabled / g_bProgressDisplaySuppressed), _StopProgress_00499d90.cpp, _Constructor_00499ce0.cpp. None use optimize pragmas (speed TU, no /Oy-).
- PFont=GetFont analogues: 00/49/CFrontendGameComponent_InitGraphics_00494840.cpp (`MainFont = mgr->GetFont("ENG_ARIAL_16")` implicit-conversion temporary; operator= expanded there), 00/41/CMainGameComponent_InitialiseFonts_004168dc.cpp (size TU; calls 0x00419463 and an out-of-line dtor).

## 7. Old draft model errors
- Residue (410v418): the GetFont result temporary's dead `Data=0; Info=0` are dropped by our build; retail keeps them. Draft (in a block with a named CCharString) used the full-expression temporary; a named CCountedPointer kept the stores but passed `lea` of the local instead of eax. Untried forms that keep eax AND a scope-lived object: (a) `const CCountedPointer<CFontBank>& font = mgr->GetFont(name); PFont = font;` (reference-extended temporary: address is the returned eax, destroyed at block end like a named local); (b) the literal implicitly converted inside the full expression `PFont = GFGetSystemManager()->PFontManager->GetFont("ENG_ARIAL_16");` (InitGraphics 0x00494840 form); (c) a visible-but-noinline operator= instead of a bodiless declaration.
- Wrong names: IsDebugDisplayForced (real IsEditorActive, const), g_pMainGameComponent (real CMainGameComponent::PMainGameComponent), CFontBankBank/FontBank (real CFontManager / PFontManager counted pointer), CCharString::operator=(const char*) non-const return.
- Control flow, arg mapping, member offsets and the RenderProgress call (text, -1.0f, false, a6) all match retail.

## 0x00EF4320

# DOSSIER 0x00EF4320 -- NScript::CPersonalScript_EmptyScript::CMeanBandit::Main (420 B)

## 1. Retail shape (xdis 00ef4320 0x1b4)
- __thiscall, plain `ret` (no args), NO frame pointer (sub esp,0x10; push ebx/esi/edi), NO SEH frame, no /Oy- needed.
- Locals 0x10 = the CIsThingAlive filter {vptr 0x012C32F0, CScriptThing Thing +4} at [esp+0x10] after pushes.
- &Me (esi+8) held in edi for the whole function; ebx used only as the cached Game vtable at the two Reset* sites.
- Filter prelude: lea ecx,&filter.Thing; vptr store 0x012C32F0; call 0x004ABE90 (OUT-OF-LINE CScriptThing copy);
  push &filter; call 0x00F35B10 SetPredicate; call 0x00E241B0 (~CIsThingAlive OUT OF LINE). => ctor inline, dtor out of line.
- Control flow equals the old draft exactly: NewScriptFrame; if(IsActiveThreadTerminating()) return; rotated
  while(!IsActiveThreadTerminating()) { EntitySetAsHavingBoundHands(Me,true); while(!IsHeroNaked()){NewScriptFrame; if(term) return;}
  if(term) return; EntitySetAsHavingBoundHands(Me,false); SetStealStealableItems(Me,true) [0xcd0]; SetCombatNearbyBreakOffRange(Me,0.0f) [0xcc4, `push 0`];
  while(IsHeroNaked()){NewScriptFrame; if(term) return;} if(term) return; ResetStealStealableItems(Me) [0xcd8]; ResetCombatNearbyBreakOffRange(Me) [0xccc]; NewScriptFrame; }
- COPY MIX (the whole residue): Set* sites (0xef43d8, 0xef4408) expand the copy INLINE straight into the arg slot:
  `mov [eax],0x01231710` (CBaseClass vptr) then `mov [eax],0x01238C8C`, Data/Info copy, `test/je/inc [Info]`, THEN load Game vtable.
  Reset* sites (0xef4474, 0xef448d) load Game vtable into ebx FIRST, then call 0x004ABE90 out of line.

## 2. Name / identity (manifest label CORRECT)
- Mangled: ?Main@CMeanBandit@CPersonalScript_EmptyScript@NScript@@EAEXXZ (private virtual void Main()).
  Source 1: ghidra_out/egor_pdb_names.tsv 00f6ea30 (Ego_r.pdb). Source 2: ghidra_out/_fablewin_api.tsv 017d7cf1 / 03af3ca0 (FableWin.pdb).
  Source 3: xref.py -> only ref is .rdata 0x012F648C = slot 1 (Main) of the CMeanBandit entity vtable 0x012F6488 (slot 0 = landed
  sdd 0x00EF4300, slot 6 0x012F64A0 = landed OnInterrupted 0x00EF42D0 -- see that row's Identity comment).
- Shared class: engine/CPersonalScript_EmptyScript.h line 121 `class CPersonalScript_EmptyScript::CMeanBandit : public CEntityScriptBase`
  (guarded by FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES; declares private `virtual void Main();`). Landed sibling
  OnInterrupted models the class locally; CTestPerson Main 0x00EF4B40 uses the shared header via that macro.

## 3. Callees (draft spelling vs PDB)
| addr | landed row | PDB / landed name | draft spelling |
|---|---|---|---|
| 0x004ABE90 | rebuild/src/compiled/00/4a/CScriptThing_CScriptThing_004abe90.cpp | CScriptThing::CScriptThing(const CScriptThing&) | ok |
| 0x00F35B10 | 00/f3/CEntityScriptBase_SetPredicate_00f35b10.cpp | ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z | WRONG: RegisterFilter |
| 0x00F35B30 | 00/f3/CEntityScriptBase_IsActiveThreadTerminating_00f35b30.cpp | ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ (const) | WRONG: IsDone (non-const) |
| 0x00E241B0 | 00/e2/NScriptCIsThingAlive_CIsThingAlive_00e241b0.cpp | ??1CIsThingAlive@NScript@@UAE@XZ | ok-ish (draft class not in NScript, no CPredicateBase base) |
| 0x004AA840 | 00/4a/CScriptThing_CScriptThing_004aa840.cpp | ??1CScriptThing@@UAE@XZ | not called directly here (callee frees by-value args) |
| Game slot 7 +0x1c | 0x006E7410 | NewScriptFrame() const | ok |
| slot 188 +0x2f0 | 0x0089B330 | bool IsHeroNaked() const | ok |
| slot 494 +0x7b8 | 0x0089D420 | EntitySetAsHavingBoundHands(CScriptThing&, bool) const  (header: NON-const ref) | draft const& -- use header |
| slot 817 +0xcc4 | 0x008A54E0 | SetCombatNearbyBreakOffRange(CScriptThing, float) | ok |
| slot 819 +0xccc | 0x008A56C0 | ResetCombatNearbyBreakOffRange(CScriptThing) | ok |
| slot 820 +0xcd0 | 0x008A57B0 | SetStealStealableItems(CScriptThing, bool) | ok |
| slot 822 +0xcd8 | 0x008A5980 | ResetStealStealableItems(CScriptThing) | ok |
Slot names: rebuild/include/engine/CGameScriptInterfaceBase.h lines 30,211,517,840-845 (and CGameScriptInterface.h). Base class
model: CEntityScriptBase.h has `CGameScriptInterfaceBase* Game` (not CGameScriptInterface).

## 4. Members
- this+0x04 Game: CEntityScriptBase (engine/CEntityScriptBase.h; PDB CGameScriptInterfaceBase* Game; opt-in
  FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS turns it into a reference -- not needed, codegen same).
- this+0x08 Me: CScriptThing (Data +0x0c, Info +0x10 read by the inline copies). struct_layouts_egor: CEntityScriptBase 20 B, CMeanBandit 28 B
  (ParentClass +0x14, PMasterData +0x18 -- unused here).
- CScriptThing (engine/CScriptThing.h, needs FABLE_RETAIL_NSCRIPT, includer defines CCountedPointer first via engine/CCountedPointer.h):
  opt-ins FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (drop the declared copy so TU expands it), _IMPLICIT_ASSIGNMENT, _ORIGINAL_ACCESS, _NULLCOORD/_NULLSTRING.
- CIsThingAlive (engine/CIsThingAlive.h, NScript, : CPredicateBase, Thing +4): ctor only declared -> row defines
  `inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }` after include (GUARD1 0x00D6A220 idiom);
  do NOT define the dtor inline (retail calls 0x00E241B0). Opt-in FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS; CPredicateBase.h has
  FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (not wanted: no 0x012C2FF4 store here anyway since dtor is out of line).
- Old draft models LOCALLY (all have shared headers): CBaseClass, CScriptThing, CCPPointerInfo, CGameScriptInterface (918-slot stub),
  CIsThingAlive, CMeanBandit (as standalone class, not nested in NScript::CPersonalScript_EmptyScript).

## 5. Callee visibility
- CScriptThing copy 0x004ABE90 is a COMDAT (unaligned-ish, 0x4ABExx band) that VC7.1 expanded in some TUs and called in others
  (CScriptThing.h comment lines 107-109). Here it is BOTH: inline at Set*, called at Reset* and inside the filter ctor.
- SetPredicate / IsActiveThreadTerminating are not inlined (CEntityScriptBase TU 0x00F35Bxx); ~CIsThingAlive out of line in this module.
- No STLport helpers.

## 6. Sibling landed rows (copy their idioms)
- 00/ef/NScriptCPersonalScriptEmptyScriptCMeanBandit_OnInterrupted_00ef42d0.cpp (same class; local CMeanBandit on CEntityScriptBase).
- 00/ef/NScriptCPersonalScriptEmptyScriptCGhostDude_Main_00ef45b0.cpp (next row, same skeleton: `SetPredicate(NScript::CIsThingAlive(Me))`,
  rotated while + wait loops; local CScriptThing with tagged `CScriptThing(const CScriptThing&, InlineCopy) : CBaseClass(), PImp(other.PImp) {}`).
- 00/ef/NScriptCPersonalScriptEmptyScriptCWalker_Main_00ef3aa0.cpp (alive-filter prelude, block-scoped `{ CIsThingAlive filter(Me); SetPredicate(filter); }`, out-of-line ~CIsThingAlive).
- 00/d1/NScriptCQBreakSiegeScriptCBanditFirstWave_Main_00d1fa00.cpp -- KEY: same mix (filter copy out of line, SetCombatNearbyBreakOffRange
  arg copy inline) solved by `Game->SetCombatNearbyBreakOffRange(CScriptThing(Me, CScriptThing::InlineCopy()), ...)` with
  the tag ctor in a local CScriptThing; the plain by-value `Me` then binds to declared out-of-line 0x004ABE90.
  Also 00/d7/NScriptCQHeroSoulsArenaScriptCDyingVillager_Main_00d7f460.cpp line 1016 (same tag idiom).
- 00/ef/NScriptCPersonalScriptEmptyScriptCTestPerson_Main_00ef4b40.cpp: shared-header route (FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES,
  engine/CGameScriptInterfaceBase.h), `// Flags: /d2inlT36` inline-budget lever. No pragma optimize in this neighbourhood.

## 7. Old draft model errors
- Control flow / loop forms / call order: CORRECT (matches disasm block for block). Do not restructure.
- Root cause of DIFFER(492v420): draft's CScriptThing copy ctor is inline in-class, so all 4 by-value copies (and the filter's) expand.
  Retail: filter + Reset* copies call 0x004ABE90, only the two Set* copies inline. Fix: declared out-of-line copy (shared header default)
  + tagged InlineCopy constructor at the two Set* call sites (BanditFirstWave idiom), CBaseClass() inline so the 0x01231710 store appears.
  Alternative lever: FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY + /d2inlT budget (first two inlined, later ones called) -- untested.
- Names: RegisterFilter->SetPredicate, IsDone->IsActiveThreadTerminating (const); class must be NScript::CPersonalScript_EmptyScript::CMeanBandit
  (private virtual Main) and CIsThingAlive must be NScript::CIsThingAlive : CPredicateBase.
- Types: SetCombatNearbyBreakOffRange 2nd arg float 0.0f (push 0); EntitySetAsHavingBoundHands takes CScriptThing& (non-const) per header.

## 0x00A11D10

# DOSSIER 0x00A11D10 CPhysicsMeshObject::GetNavigationLevelAt (422 bytes)

## 1. Retail shape (xdis 00a11d10 0x1b6)
- __thiscall, `ret 4` (one arg: const C3DVector&), NO frame pointer (`sub esp,40h`), no SEH, no callee-saved pushes.
- `mov eax,ecx` (this kept in eax), `mov ecx,[eax+4]` (Mesh raw pointer), `mov edx,[ecx+54h]; cmp edx,[ecx+58h]; je -> xor eax,eax`
  = empty() of a begin/end pair (STLport vector::empty is begin()==end(); NOT size()==0, which would be sub/sar).
- Frame 0x40 = [esp+0] float inv (the reciprocal, stored and reloaded with `fmul [esp]` for every cofactor),
  [esp+4..0xC] one C3DVector temp (the transformed point, passed by address `lea eax,[esp+4]; push eax`),
  [esp+0x10..0x3C] ONE CMatrix3x4 local (E11 @+0x10 .. E43 @+0x3C). Only E11,E12,E21,E22,E31,E32,E43 are stored; E13,E23,E33,E41,E42
  stay on the x87 stack. => model: `CMatrix3x4 inv; inv.SetInverse(PositionMatrix); return Mesh->GetNavigationLevelAt(inv * pos);`
- 1.0f constant @ 0x0122DED8 (`fdivr`). Tail: `call 0x00A73D10` (thiscall on Mesh, ecx still = Mesh), `add esp,40h; ret 4`.
- Arithmetic order observed (m = PositionMatrix): cofactors A=E32*E13-E33*E12, B=E23*E12-E22*E13, C=E22*E33-E32*E23 (note fld E32,fmul E23);
  det = C*E11 + B*E31 + A*E21; then r.E11=C*inv, r.E21=(E23*E31-E21*E33)*inv, r.E31=(E21*E32-E22*E31)*inv, r.E12=A*inv,
  r.E22=(E33*E11-E13*E31)*inv, r.E32=(E31*E12-E32*E11)*inv, r.E13=B*inv, r.E23=(E21*E13-E23*E11)*inv, r.E33=(E22*E11-E21*E12)*inv;
  r.E41=-(r.E11*E41 + r.E31*E43 + r.E21*E42) (same for E42/E43); point: X = r.E31*p.Z + r.E21*p.Y + r.E11*p.X + r.E41 (Z,Y,X, then E4k).
  This is exactly the body of CMatrix3x4::SetInverse in rebuild/include/engine/CMatrix3x4.h (VC7.1 reorders the det/translation sums itself;
  two landed byte-exact rows use that very body: 0x00B393D0 and 0x00AEE240).

## 2. Identity (two+ sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 00b7f840) and FableWin.pdb (_fablewin_api.tsv 017d6d2e; naming_stragglers signatures)
  both: `?GetNavigationLevelAt@CPhysicsMeshObject@@QBEJABVC3DVector@@@Z` = `long CPhysicsMeshObject::GetNavigationLevelAt(const C3DVector&) const`.
  Manifest label CPhysicsMeshObject::GetNavigationLevelAt is CORRECT. Leaf for the row: `?GetNavigationLevelAt@CPhysicsMeshObject@@QBEJABVC3DVector@@@Z`.
- Return type is `long` (J), NOT `const CNavigationLevel*` as the old draft has. Returns 0 when no navigation volumes.
- Shared header rebuild/include/engine/CPhysicsMeshObject.h already declares it under FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATION
  (`long GetNavigationLevelAt(const C3DVector& pos) const; // 0x00A11D10`).
- Callers (xref): 0x0063EFE1 (landed rebuild/src/compiled/00/63/CThingFindNavigationLayerAt_operator_0063efb0.cpp -- its local model declares
  the method NON-const: a signature fix for the coordinator, NAME_FIXES-style), 0x0063F6CB, 0x0063F9EB (ForEachInRoughArea<...,CThingFindNavigationLayerAt>, 0x0063F7C0 per header comment).
  Not virtual (Q = public non-virtual), no vtable slot.

## 3. Callees
- 0x00A73D10 `long CPhysicsMesh::GetNavigationLevelAt(const C3DVector&) const` -- PDB ?GetNavigationLevelAt@CPhysicsMesh@@QBEJABVC3DVector@@@Z
  (egor 00c12640, FableWin 01842ace); landed rebuild/src/compiled/00/a7/CPhysicsMesh_GetNavigationLevelAt_00a73d10.cpp (name matches, returns long).
  Old draft: return type wrong (`const CNavigationLevel*`), members NavFirst/NavLast are made-up names.
- No other calls. Inline (no retail out-of-line copy): CMatrix3x4::SetInverse (FableWin ?SetInverse@CMatrix3x4@@QAEXABV1@@Z, 0x01837D1D),
  CMatrix3x4::operator*(const C3DVector&) const (FableWin ??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z, 0x017EFEF0) -- note it is a MEMBER
  `matrix * vector`, not the draft's free `vector * matrix`. CIVCountedPointer<const CPhysicsMesh>::operator-> (FableWin ??C...QBEPBVCPhysicsMesh@@XZ),
  CPhysicsMesh::GetNavigationContainmentVolumes() const (FableWin 01821d3d, returns const CVectorMap<long,C3DMeshVolume,...>&) + empty().
  There is NO GetInverse in either PDB: the draft's `GetInverse()` is a made-up name.

## 4. Members
- CPhysicsMeshObject (struct_layouts_egor / header, size 0x40): Mesh +0x04 CIVCountedPointer<const CPhysicsMesh> (opt-in FABLE_RETAIL_CPHYSICSMESHOBJECT
  -> typed member + GetMesh(); needs FABLE_RETAIL_CIVCOUNTEDPOINTER), PositionMatrix +0x08 CMatrix3x4 (opt-in FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX),
  Scale +0x38, OO_Scale +0x3C (not read). Members are private -- fine inside a member function.
- CPhysicsMesh +0x54/+0x58 = NavigationContainmentVolumes (PDB +0x68 = 104; CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> >,
  0x10 bytes, derives CArray -> STLport vector, _M_start +0 / _M_finish +4). rebuild/include/engine/CPhysicsMesh.h models it only as
  `unsigned char NavigationContainmentVolumes[16]` (private, no opt-in) and does NOT declare GetNavigationLevelAt or GetNavigationContainmentVolumes.
  The landed sibling 0x00A11C20 worked around the same gap with a LOCAL `class CPhysicsMesh : public CIVCountedPointeeBase` (pad + typed member +
  inline accessor). Author needs either that idiom or a new opt-in in CPhysicsMesh.h (coordinator).
- CMatrix3x4 E11..E43 (engine/CMatrix3x4.h); SetInverse is under FABLE_RETAIL_CMATRIX3X4 (that macro also switches TransformPoint to __forceinline
  forms; harmless). No operator*(const C3DVector&) in the shared header -- landed rows define it locally (00b814b0/00b81640 `C3DVector operator*(const C3DVector& v) const`
  summing X,Y,Z order; retail here wants E3k*Z + E2k*Y + E1k*X + E4k -- probe; the ZYX form is the TRANSFORMPOINT_ZYX_SUMS spelling).
  Caution: 0x00A56530 is an out-of-line C3DVector-returning operator* called by 0x00759EC0 (check which class) -- here it is inlined.
- Locally modelled classes in old draft that have shared headers: C3DVector (engine/C3DVector.h), CMatrix3x4 (engine/CMatrix3x4.h),
  CPhysicsMesh (engine/CPhysicsMesh.h), CPhysicsMeshObject (engine/CPhysicsMeshObject.h).

## 5. Callee visibility
- 0x00A73D10 is in another TU (address far away, CPhysicsMesh source) -- declaration only.
- SetInverse / operator* / operator-> / GetNavigationContainmentVolumes / empty are inline (no retail copies) -- must be visible, use FABLE_FORCEINLINE
  (owner 2026-11-01 helper rule; old NOTES: plain inline left GetInverse out of line, 74 bytes).
- Same TU neighbours (CPhysicsMeshObject.cpp, 0x00A114B0..0x00A12CC0) all landed with `// Flags: none`, no pragmas.

## 6. Siblings (same class / TU)
- 0x00A11C20 GetBoundingBoxXEdge: Flags none; defines FABLE_RETAIL_DEF_CLASS_BASE, _C3DVECTOR, _C3DBOUNDINGBOX, _CMATRIX3X4_TRANSFORMVECTOR,
  _CIVCOUNTEDPOINTER, _CPHYSICSMESHOBJECT, _CPHYSICSMESHOBJECT_MATRIX, _CPHYSICSMESHOBJECT_EDGES; local CPhysicsMesh with pad; `Mesh->PeekBoundingBox()`.
- 0x00A114B0 SetPositionMatrix: Flags none; FABLE_RETAIL_CMATRIX3X4_SETIDENTITY + _CPHYSICSMESHOBJECT_SETPOSITIONMATRIX.
- 0x00A12890 IsPointContainedInBoundingBox: _CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS.
- 0x00A11EC0 GetCentreOfGravity (older local-model row, inline Rotate helper changes sum order).
- Byte-exact SetInverse users: 00/b3/..._00b393d0.cpp, 00/ae/CParticleEmitter_Initialise_00aee240.cpp.
- Parked same-family rows (same inverse): 0x00A11F20, 0x00A128E0, 0x00A12AB0 (hand267 differ/). Solve this one and they follow.

## 7. Old draft model errors
- Return type `const CNavigationLevel*` -> `long`.
- Made-up GetInverse() returning a CMatrix3x4 by value (frame 0x70 v 0x40) -> named local + `inv.SetInverse(PositionMatrix)` (PDB inline name).
- Free `operator*(const C3DVector&, const CMatrix3x4&)` -> member `CMatrix3x4::operator*(const C3DVector&) const`, `inv * pos`.
- NavFirst/NavLast made-up -> NavigationContainmentVolumes (empty() via GetNavigationContainmentVolumes()).
- Mesh as raw pointer -> CIVCountedPointer<const CPhysicsMesh> operator-> (no AddRef: just reads [this+4]).
- All four classes modelled locally despite shared headers.

## 0x007F53B0

# DOSSIER 0x007F53B0 -- CTCSoundAtmosphere::FrameUpdate (NOT ...Village)

## 1. Retail shape (xdis 007f53b0 0x1b8)
- 424 bytes (0x7F53B0..0x7F5557 `ret`, int3 pad to 0x7F5560 -> 432 padded). thiscall, no args, plain `ret`.
- NO frame pointer (`sub esp,28h` first), NO SEH prologue -> default /Oy, no optimize("y",off). Not size-optimised
  (loop-top `lea esp,[esp]` align pad at 0x7F53F9, `jmp` into aligned loop top) -> speed TU, no pragmas.
- Push order: ebx (this, also stored [orig-0x10]); edi (atmos iterator) pushed only inside `if`; ebp, esi pushed only once the
  vector is non-empty. Epilogue tail is shared (0x7F5541) by the "no sound / no atmos processor" path.
- Frame words (E = esp after all 4 pushes): E+0x10 i; E+0x14/E+0x18 CCharString("SOUND_THEME") temps; E+0x1c def handle
  (zeroed `mov [esp+24h],0` = CDefPointer ctor before the "SOUND_THEME" temp), later REUSED by `fstp` for the gain;
  E+0x20 copied key CCharString; E+0x24 GetString result; E+0x28 this; E+0x2c atmos processor; E+0x30 count; E+0x34 CDefString.
- Control flow:
  if (PSoundManager [0x013B8394] && GetWorld().PAtmosProcessor) {        // GetWorld called TWICE (test, then load into local)
    CAtmosProcess* atmos = GetWorld().PAtmosProcessor;
    for (it = PDef->SoundAtmospheres.begin(); it != PDef->SoundAtmospheres.end(); ++it) {   // PDef re-read from this at loop bottom
      long count = CGameDefinitionManager::Get().GetNoInstantiatedDefsOfType("SOUND_THEME");   // per outer iteration
      for (long i = 1; i < count; ++i) {                                 // `cmp esi,1 ; jle` guard, i lives in memory
        CDefPointer<const CSoundThemeDef> theme;                        // = 0
        CGameDefinitionManager::Get().GetDef<CSoundThemeDef>("SOUND_THEME", i, &theme);   // OUT OF LINE call 0x006B25E0
        // copy-construct CCharString(*it) (0x0099EC30) -> E+0x20; ebx = theme->AtmosGroupMap header (end()) read AFTER the copy;
        // _M_find(copy) 0x0042B467 (ecx = theme+0x54); ~copy; `cmp ebp(found),ebx(end) ; je skip`
        if (found != end) {
          float gain = GetGain(*it);                                    // this vslot 28 (+0x70), push edi
          if (gain != -1.0f) {                                          // fucompp vs 0x0122DEE0; `test ah,44h ; jnp` = skip if equal
            atmos->MixAtmos(theme->GetInstantiationName().GetString(), *it, gain, 0.0f);   // push 0 = 0.0f time
          }                                                             // ~CCharString(GetString result)
        }
        // theme release: `dec [esi+4] ; jne ; call [vtbl+4]` -- NO null test, NO `Object=0` store, esi cached since the GetDef call
      }
    }
  }
  TimeUntilNextUpdate -= 1.0f / CMainGameComponent::GetConstantFPS();   // fild [0x01375550]; fdivr 1.0f [0x0122DED8]; fsubr [ebx+0Ch]
- The tail runs unconditionally (both paths) -- matches draft.

## 2. Identity
- vtable refs (xref.py): 0x01241140 = CTCSoundAtmosphereVillage vtable 0x0124111C slot 9, AND 0x01271E20 = CTCSoundAtmosphere
  vtable 0x01271DFC slot 9 (rtti-vtables.tsv). Same body in BOTH vtables => base-class FrameUpdate, Village does not override.
- PDB: Ego_r `?FrameUpdate@CTCSoundAtmosphere@@UAEXXZ` (egor_pdb_names.tsv, Ego_r addr 0x007EE910) and FableWin
  `?FrameUpdate@CTCSoundAtmosphere@@UAEXXZ` (_fablewin_api.tsv 0x0185091C / 0x0278F5E0). Neither PDB has a FrameUpdate for
  CTCSoundAtmosphereVillage (Village has only GetGain / OnInitialActivate / ctor / dtor / GetClassTypeName / Alloc).
  CTCBase slot 9 = FrameUpdate (Ego_r ?FrameUpdate@CTCBase@@UAEXXZ; landed CTCDoor::FrameUpdate pattern).
- MANIFEST LABEL IS WRONG (class): real name CTCSoundAtmosphere::FrameUpdate, leaf `?FrameUpdate@CTCSoundAtmosphere@@UAEXXZ`,
  `virtual void FrameUpdate()`, public, non-const. (egor_pdb_names addresses are Ego_r build addresses, not retail -- ignore
  the unrelated _Insertion_sort entry at "007f53b0" there.)
- Neighbours confirm the TU: CTCSoundAtmosphere ctor 0x007F5370, OnInitialActivate 0x007F53A0 (just before), GetSubDef 0x007F5560
  and scalar-deleting dtor 0x007F5620 (just after). Village GetGain 0x007F5150 / OnInitialActivate 0x007F50B0 precede.

## 3. Callees (landed file / landed name / PDB name; X = draft spelling wrong)
- 0x00686A70 rebuild/src/compiled/00/68/CTCBase_GetWorld_00686a70.cpp -- `CWorld& CTCBase::GetWorld() const` (?GetWorld@CTCBase@@IBEAAVCWorld@@XZ,
  protected). X draft: returns CWorld* and local CWorld.
- 0x0044C6B0 .../00/44/CGameDefinitionManager_Get_0044c6b0.cpp -- `static CGameDefinitionManager& __fastcall CGameDefinitionManager::Get()`
  (?Get@CGameDefinitionManager@@SIAAV1@XZ). X draft: CDefinitionManager::Get returning pointer.
- 0x009AD3B0 .../00/9a/CDefinitionManager_GetNumberOfDefsOfType_009ad3b0.cpp (file leaf invented) -- PDB
  `long CDefinitionManager::GetNoInstantiatedDefsOfType(const CCharString&) const` (?GetNoInstantiatedDefsOfType@CDefinitionManager@@QBEJABVCCharString@@@Z),
  header engine/CDefinitionManager.h under FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED. X draft: GetNumberOfDefsOfType, non-const.
- 0x006B25E0 .../00/6b/CDefinitionManager_GetDef_006b25e0.cpp -- `template<class T> bool CDefinitionManager::GetDef(const CCharString&, long,
  CDefPointer<const T>*) const`, T=CSoundThemeDef (??$GetDef@VCSoundThemeDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCSoundThemeDef@@@@@Z).
  Header declares it (CDefinitionManager.h:46); body opt-in FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_BODY exists but HERE retail
  CALLS it out of line -> do NOT define the body macro (unless the body is needed for the dtor shape, see 7). X draft: local CSoundThemeDefPtr.
- 0x0099EBF0 CCharString(const char*, long=-1) (rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp); "SOUND_THEME" 0x01235E60.
- 0x0099EC30 CCharString copy ctor (.../00/99/CCharString_CCharStringcopy_0099ec30.cpp); 0x0099EAE0 ~CCharString. Use fable_string.h + FABLE_RETAIL_CCHARSTRING.
- 0x0042B467 .../00/42/stdRbtreeCCharString..._Mfind_0042b467.cpp -- STLport `_Rb_tree<CCharString,pair<const CCharString,long>,...>::_M_find<CCharString>(const CCharString&) const`
  (map<CCharString,long> instantiation; out-of-line COMDAT at an unaligned address = inline member emitted out of line).
  stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_MFIND_DECL (line 177, declaration-only -> call) and FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
  (line 201, `template<class KT> iterator find(const KT&){return iterator(_M_find(key));}`). X draft: CGroupMap::find.
- 0x009D49B0 .../00/9d/CDefStringTable_GetString_009d49b0.cpp -- CDefStringTable::GetString on GDefStringTable 0x013CA828
  (?GDefStringTable@@3VCDefStringTable@@A, ESTABLISHED_SPELLINGS.tsv), reached through the inline CDefString::GetString()
  (engine/CDefString.h, FABLE_RETAIL_CDEFSTRINGTABLE). X draft: g_DefStringTable.
- def vslot 10 (+0x28): `virtual CDefString CDefClassBase::GetInstantiationName() const` (engine/CDefClassBase.h:90, landed 0x0042DA10). X draft: GetSymbol.
- 0x006B1900 .../00/6b/CAtmosProcess_MixAtmos_006b1900.cpp -- `void CAtmosProcess::MixAtmos(const CCharString& from, const CCharString& to,
  float mix, float time)` (?MixAtmos@CAtmosProcess@@QAEXABVCCharString@@0MM@Z). engine/CAtmosProcess.h declares it (line 56); the
  FABLE_RETAIL_CATMOSPROCESS_MIXATMOS_BODY opt-in makes it inline -- retail CALLS it here, so do NOT define that macro.
  X draft: CSoundAtmosphereManager::SetThemeLevel(..., long flags) -- the 4th arg is float time = 0.0f.
- this vslot 28 (+0x70): `virtual float GetGain(const CCharString&)` (Ego_r ?GetGain@CTCSoundAtmosphereVillage@@UAEMABVCCharString@@@Z;
  Village vtable slot 28 = 0x007F5150 landed). X draft: GetGroupLevel.
- def vslot 1 (+4): Delete on last release (CDefPointer dtor).
- Globals: 0x013B8394 = PSoundManager (`extern CASoundManager* PSoundManager`, engine/CASoundManager.h:249) -- X draft g_SoundEnabled.
  0x01375550 = CMainGameComponent::ConstantFPS via inline `static long __fastcall GetConstantFPS()` (engine/CMainGameComponent.h:183,
  FABLE_RETAIL_CMAINGAMECOMPONENT) -- X draft g_FrameRate. Floats: -1.0f 0x0122DEE0, 1.0f 0x0122DED8.

## 4. Members
- this (CTCSoundAtmosphere, Ego_r 0x14): CTCBase vptr/ParentThing +4/flags +8; TimeUntilNextUpdate float +0x0C;
  PDef CDefPointer<const CSoundAtmospheresDef> +0x10 (struct_layouts_egor). NO shared header for CTCSoundAtmosphere(Village):
  sibling rows model it locally on the shared CTCBase (FABLE_RETAIL_CTCBASE), as in GetGain 0x007F5150. X draft: "Village" / UpdateTimer.
- CSoundAtmospheresDef +0x28 SoundAtmospheres vector<CCharString> (begin +0x28, end +0x2c): egor 40; header only in
  engine/_quarantine/CSoundAtmospheresDef.h (quarantined -> not includable; model locally with the real name). X draft: CVillageSounds/Groups.
- CSoundThemeDef +0x54 AtmosGroupMap map<CCharString,long> (egor says +0x58 multimap because Ego_r containers are bigger; retail
  +0x54 per engine/_quarantine/CSoundThemeDef.h:33/49 and the xdis `lea ecx,[esi+54h]`). Map's first word = STLport _M_header
  node pointer = end(). X draft: Groups.
- CSoundThemeDef ref count +4, Delete vslot 1 (CDefClassBase via FABLE_RETAIL_DEF_CLASS_BASE, CDefPointer.h).
- CWorld +0x24 PAtmosProcessor (CCountedPointer<CAtmosProcess>::Data): engine/CWorld.h:304 (FABLE_RETAIL_CWORLD) /
  :427 (flattened default) `PAtmosProcessor_Data`. X draft: AtmosManager.
- Draft models LOCALLY classes that have shared headers: CCharString (fable_string.h), CDefStringTable (engine/CDefStringTable.h),
  CDefinitionManager / CGameDefinitionManager (engine/CDefinitionManager.h, CGameDefinitionManager.h), CWorld (engine/CWorld.h),
  CTCBase (engine/CTCBase.h), CDefPointer (engine/CDefPointer.h), CAtmosProcess (engine/CAtmosProcess.h), PSoundManager
  (engine/CASoundManager.h), CMainGameComponent.h, CDefString.h, CDefClassBase.h.

## 5. Callee visibility
- Same TU (defined earlier, visible): CTCSoundAtmosphere ctor 0x007F5370, OnInitialActivate 0x007F53A0, the Village functions
  0x007F50B0/0x007F5150. GetGain is reached virtually so visibility does not matter.
- Out-of-line inline/template COMDATs called: _M_find 0x0042B467 (STLport), GetDef<CSoundThemeDef> 0x006B25E0, MixAtmos 0x006B1900
  (inline in class per CAtmosProcess.h comment, out of line here). Keep all three as plain calls: MFIND_DECL yes, GETDEF_INDEX_BODY
  no (probably), MIXATMOS_BODY no.
- Inline expanded in place: GetConstantFPS, CDefString::GetString (only the GDefStringTable call shows), CTCBase::GetWorld is a call.

## 6. Siblings (copy idioms)
- rebuild/src/compiled/00/7f/CTCSoundAtmosphereVillage_GetGain_007f5150.cpp -- Flags: none; macros FABLE_RETAIL_CCHARSTRING, _CCLASS,
  _DEF_CLASS_BASE, _CTCBASE, _CTHING... ; local `class CTCSoundAtmosphere : public CTCBase { virtual float GetGain(const CCharString&);
  float TimeUntilNextUpdate; const CSoundAtmospheresDef* PDef; }` -- reuse this model, add `virtual void FrameUpdate()`.
- rebuild/src/compiled/00/7f/CTCSoundAtmosphere_OnInitialActivate_007f53a0.cpp (local CDefPointer model, no pragmas).
- rebuild/src/compiled/00/6b/CAtmosProcess_UpdateDayNightAtmosSounds_006b1a20.cpp and CAtmosProcess_SetAtmosSoundsAsMuted_006b1d30.cpp:
  the SAME inner loop idiom: `long count = CGameDefinitionManager::Get().GetNoInstantiatedDefsOfType("SOUND_THEME"); for (long i = 1;
  i < count; ++i) { CDefPointer<const CSoundThemeDef> theme; CGameDefinitionManager::Get().GetDef<CSoundThemeDef>("SOUND_THEME", i, &theme);
  MixAtmos(theme->GetInstantiationName().GetString(), ...); }`. Macros: FABLE_RETAIL_CCHARSTRING, _CDEFSTRINGTABLE, _DEF_CLASS_BASE,
  _CRESOURCELIST, _CDEFINITIONMANAGER, _CDEFINITIONMANAGER_NOINSTANTIATED, _CDEFINITIONMANAGER_GETDEF_INDEX_BODY, _CGAMEDEFINITIONMANAGER,
  _CWORLD, _CMAINGAMECOMPONENT, _CMAINGAMECOMPONENT_GETWORLD. `class CSoundThemeDef : public CDefClassBase { };` locally.
- rebuild/src/compiled/00/6b/CAtmosProcess_Update_006b2260.cpp (CWorld/CTCBase/CMainGameComponent macro set).

## 7. Old draft model errors
- Wrong class/name (Village::FrameUpdate -> CTCSoundAtmosphere::FrameUpdate) and nearly every callee/member spelling (see 3/4).
- Find key: retail COPY-CONSTRUCTS CCharString(*it) before _M_find and destroys it right after; the end() header word is loaded after the
  copy. Draft passes `CCharString(*group)` as a temp too, but via a made-up `find` returning void* and `end` read BEFORE the temp;
  likely real spelling: an inline taking the name BY VALUE (e.g. `theme->AtmosGroupMap.find(CCharString(*it)) != theme->AtmosGroupMap.end()`
  or a helper with a by-value CCharString parameter). Order of `end` load vs copy matters.
- Def handle: retail caches the def pointer in esi right after GetDef, reuses its frame word (E+0x1c) for `gain`, and releases with
  no null test/no zero store. A CDefPointer whose dtor tests null (`if(Object){..;Object=0;}`) won't give that; siblings 0x006B1A20/1D30
  got the uncaptured, null-test-free release only with GETDEF_INDEX_BODY (GetDef body visible) -- here GetDef is still a call, so try
  first the header CDefPointer with GETDEF_INDEX_BODY defined (the body might be visible yet not inlined), else a local raw
  `const CSoundThemeDef* t = theme...` model. Open question.
- Draft's this/iterator register swap: retail this=ebx (re-loaded from its frame word after the inner loop, since ebx is reused for
  the map end), it=edi, i/count in memory, found=ebp, def=esi. Atmos processor is a named local loaded by a 2nd GetWorld() call.
- MixAtmos 4th arg is float 0.0f (push 0), not long flags; gain local stored with fstp (float), compared `!= -1.0f`.
- Frame: retail 0x28 bytes + 4 pushes; draft was +4 larger (one extra temp, likely the find-result/end or CDefString temp).

## 0x009692C0

# DOSSIER 0x009692C0 CAIStateGroup_TavernKeeper::ReceivePayment (426 B)

## 1. Retail shape (xdis 009692c0 0x1ba)
- __thiscall, no args, plain `ret` (3 epilogues: 0x969307 false, 0x96943c true, 0x969469 false). Returns bool (`xor al,al` / `mov al,1`).
- No frame pointer (`sub esp,0C4h ; push esi`), no SEH (no /EHsc frame although there are dtor'd temps -> TU has no EH, or temps with
  dtors are not unwound: matches siblings). Flags: none needed.
- Partial prologue: only esi pushed up front; ebx/ebp/edi pushed at 0x969308 AFTER the early-exit tests. The first exit block
  (0x9692f0: FreeBarPoint; SetPushableByHero(1); return false) is the FALL-THROUGH of the GetPaymentCancelledMessage test
  (`je 0x969308` jumps to the main body). WalkToPoint failure `je 0x9692f0` shares it.
- Frame 0xC4: locals+0x00 CCharString temp; locals+0x04..0x0f C3DVector pos (then CSoundPair temp reuses +0x04/+0x08);
  locals+0x10..0xc3 CCreatureAction_PlayAnimation temp (0xB4 bytes = retail sizeof).
- Registers in main body: esi=this, ebp=&PPayee (+0x2c, kept for the alive test), ebx=AddMessage result, edi=creature then endFrame.
- Wait loop is rotated: test GetPaymentFinishedMessage once (0x96937e, jne success), then loop {DoNothing; GetFrame()>end?; IsStopping?;
  PPayee.GetPItem()==0?; GetPItem()->[+0x91]&1?; GetPaymentFinishedMessage? je loop}. All four fail tests jump to one block 0x96943d:
  GetWorld().[+0x60].RemoveMessage(ebx); FreeBarPoint(); SetPushableByHero(true); return false.
- endFrame: `mov edi,[0x1375550] ; call GetFrame ; lea edx,[edi+edi*4] ; lea edi,[eax+edx*4]` = GetFrame() + ConstantFPS*20 (FPS loaded BEFORE the call).
- Success: CCharString("RECEIVE_MONEY_AT_BAR",-1) built FIRST (0x9693d4) before `push 0` (PerformActionAndWait's bool) -> implicit
  const char* -> const CCharString& conversion (LESSONS hand259, 0x00969AE0), not an explicit CCharString temp.
  Ctor pushes: (creature=[[esi+4]+0x18], str, CSoundPair{0,0}, false, true, false, 0, 0); then PerformActionAndWait(action,false);
  then ~CCreatureActionBase 0x693EF0 on the action (implicit dtor), ~CCharString 0x99EAE0, FreeBarPoint, return true.
  NOTE: the true path does NOT call SetPushableByHero(true).

## 2. Name / signature (two sources)
- Ego_r PDB (ghidra_out/egor_pdb_names.tsv 0098df00) and FableWin (_fablewin_api.tsv + naming_stragglers 017c29f5/02c484f0):
  `?ReceivePayment@CAIStateGroup_TavernKeeper@@QAE_NXZ` -> public `bool ReceivePayment()`, non-const, non-virtual. Manifest label CORRECT.
- Caller: Update 0x00969470 (landed rebuild/src/compiled/00/96/CAIStateGroupTavernKeeper_Update_00969470.cpp line 129/165, `if (!ReceivePayment())`).
- Retail TU order (functions.tsv): ...IsRunnable 968e00, FillJug 968fa0 (unlanded), ReceivePayment 9692c0, Update 969470.

## 3. Callees (landed file | landed name | PDB) -- draft spellings flagged
| addr | landed row | PDB (Ego_r) | draft spelling |
|---|---|---|---|
| 0x97E330 | 00/97/CAIStateGroupBase_SetPushableByHero_0097e330.cpp | ?SetPushableByHero@CAIStateGroupBase@@IAEX_N@Z (protected) | ok |
| 0x968230 | 00/96/CAIStateGroupTavernKeeper_WalkToPoint_00968230.cpp | ?WalkToPoint@...@@QAE_NABVC3DVector@@ABVCRightHandedSet@@@Z | WRONG: draft takes pointers; PDB is `bool WalkToPoint(const C3DVector&, const CRightHandedSet&)` (same bytes) |
| 0x968510 | 00/96/CAIStateGroupTavernKeeper_GetPaymentCancelledMessage_00968510.cpp | ?GetPaymentCancelledMessage@...@@QAE_NXZ | ok |
| 0x968460 | 00/96/CAIStateGroupTavernKeeper_GetPaymentFinishedMessage_00968460.cpp | ?GetPaymentFinishedMessage@...@@QAE_NXZ | WRONG: draft `HasBeenPaid` |
| 0x968950 | 00/96/CAIStateGroupTavernKeeper_FreeBarPoint_00968950.cpp | ?FreeBarPoint@...@@QAEXXZ | ok |
| 0x8FCFC0 | 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (const, returns CWorld&) | WRONG: draft `GetPWorld()` returning pointer |
| (inline) | CWorld.h FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER(_CONST) | GetMessageEventManager() -> *[world+0x60] | WRONG: draft `PMessageManager` member |
| 0x493B5B | 00/49/CMessageEventManager_RemoveMessage_00493b5b.cpp | ?RemoveMessage@CMessageEventManager@@QAEXPBVCMessageEvent@@@Z | ok |
| 0x687540 | 00/68/CMessageEventManager_AddMessage_00687540.cpp | ?AddMessage@...@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z | WRONG: draft takes `C3DVector*`, `long type`; PDB `(EMessageEventType, unsigned long, const C3DVector&, CThing*, CThing*, float, float, long)`; type 0x30 = MESSAGE_EVENT_PAYMENT_REQUEST (EMessageEventType.h:53) |
| 0xA01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | ok (via CIntelligentPointer<CThingAICreature>) |
| 0x49D870 | 00/49/CWorld_GetFrame_0049d870.cpp | ?GetFrame@CWorld@@SIJXZ (static __fastcall) | WRONG: draft `GetWorldFrame` |
| [0x1375550] | CMainGameComponent.h:183 inline `static long GetConstantFPS()` | ?ConstantFPS@CMainGameComponent@@1JA | WRONG: draft `g_FramesPerSecond` |
| 0x97E200 | 00/97/CAIStateGroupBase_AIActionDoNothing_0097e200.cpp | ?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ | draft returns long (should be EAIActionReturn) |
| 0x8FD010 | 00/8f/CAIStateGroupBase_IsStopping_008fd010.cpp | ?IsStopping@CAIStateGroupBase@@IBE_NXZ (const) | draft non-const |
| 0x99EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | ??0CCharString@@QAE@PBDJ@Z | ok |
| 0x8424F0 | 00/84/CCreatureActionPlayAnimation_CCreatureActionPlayAnimation_008424f0.cpp | ??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@ABVCSoundPair@@_N33JJ@Z | WRONG: draft takes `CThingCreatureBase*`; PDB takes `CThingCreatureBase&` |
| 0x97DB70 | 00/97/CAIStateGroupBase_AIActionPerformActionAndWait_0097db70.cpp | ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z | WRONG: draft takes pointer + returns long; PDB const ref, EAIActionReturn |
| 0x693EF0 | 00/69/CCreatureActionBase_CCreatureActionBase_00693ef0.cpp | ??1CCreatureActionBase@@UAE@XZ | called directly = PlayAnimation implicit dtor |
| 0x99EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok |
| IS_THING_ALIVE | engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE (template, inline) | | draft local forceinline copy -> use shared |

## 4. Members (struct_layouts_egor.tsv; engine/CAIStateGroup_TavernKeeper.h under FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER)
- +0x04 CAIStateGroupBase::PBrain (CAIBrain*); creature via inline GetCreature() (FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE +
  FABLE_RETAIL_CAIBRAIN_GETCREATURE + _CREATURE_REFERENCE -> CThingAICreature&, PBrain->Creature at +0x18).
- +0x2c PPayee CIntelligentPointer<CThingAICreature>; +0x38 JugPos C3DVector; +0x44 JugRHSet CRightHandedSet;
  +0x64 PaymentRequestMessage (`const CMessageEvent*` in the retail header). Header is private-member; the retail header declares
  only GetPaymentFinishedMessage/GetPaymentCancelledMessage -- WalkToPoint, FreeBarPoint, ReceivePayment are NOT declared there
  yet (the author must add them, e.g. extend the header under a new nested opt-in such as FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER_RECEIVEPAYMENT, or follow how Update 0x969470 declares them).
- CThing +0x91 bit0 = dead flag, read by IS_THING_ALIVE (ThingAlive.h).
- CCreatureAction_PlayAnimation: retail sizeof 0xB4 (engine/CCreatureAction_PlayAnimation.h, FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION +
  FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR, the latter needed for the direct ~CCreatureActionBase call, chunk Q9 0x0092B900).
  Ego_r says 0xC0 (non-STLport containers) -- do not use. The draft's 0xAC is WRONG -> this is the "frame slots 8 apart" residue.
- CSoundPair (engine/CSoundPair.h, FirstSound/LastSound) default {0,0}.
- Classes the draft models LOCALLY that have shared headers: CThing, CBaseIntelligentPointer/CIntelligentPointer, CCharString, C3DVector,
  CRightHandedSet, CSoundPair, CCreatureActionBase, CCreatureAction_PlayAnimation, CMessageEventManager, CWorld, CAIBrain,
  CAIStateGroupBase, CAIStateGroup_TavernKeeper (+ unused CTCTavernBar/CInterfaceMap/GFFloatRandom leftovers to delete).

## 5. Callee visibility
- Same TU, earlier in retail: WalkToPoint 968230, GetPaymentFinishedMessage 968460, GetPaymentCancelledMessage 968510, FreeBarPoint 968950
  (all out-of-line calls in retail; not inline). Visible bodies may matter for escape analysis (GOTCHAS Q24) -- keep as declarations unless a probe shows otherwise.
- Inline in retail (no call emitted): CWorld::GetMessageEventManager, CMainGameComponent::GetConstantFPS, CAIStateGroupBase::GetCreature,
  CAIBrain::GetCreature, IS_THING_ALIVE, CSoundPair ctor, C3DVector(float,float,float), CIntelligentPointer<T>::GetPItem wrapper.
- Out of line: CBaseIntelligentPointer::GetPItem 0xA01B50 (twice in alive test), CWorld::GetFrame, CCharString ctor/dtor, ~CCreatureActionBase.
- No STLport helpers called.

## 6. Sibling rows to copy
- 00/96/CAIStateGroupTavernKeeper_GetPaymentCancelledMessage_00968510.cpp and ..._GetPaymentFinishedMessage_00968460.cpp: VERIFIED_MATCH,
  `// Flags: none`, full retail macro block (FABLE_RETAIL_CCHARSTRING, CTHING*, CINTELLIGENTPOINTER(+_DEREFERENCE, _GET_ORIGINAL_CHAIN,
  _COMPARE_NULL_INT), CAISTATEGROUPBASE(+_MEMBERS, _GETCREATURE, _GOTOBED_WORLD, _WAITMESSAGE, _PEEK), CAISTATEGROUP_TAVERNKEEPER,
  CAIBRAIN_GETCREATURE/_SUSPENDABLE_BASE/_CREATURE_REFERENCE, CWORLD, CWORLD_MESSAGEEVENTMANAGER(_CONST), CMESSAGEEVENT*,
  CMESSAGEEVENTMANAGER_REMOVE, IS_THING_ALIVE). Uses `GetWorld().GetMessageEventManager().RemoveMessage(pevent)`. Best template.
  Add: FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION (AIAction_PerformActionAndWait const&), FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION(+_IMPLICIT_DTOR),
  FABLE_RETAIL_CMAINGAMECOMPONENT, CSoundPair, C3DVector; check AIAction_DoNothing (CAIStateGroupBase.h:84) gate.
- 00/96/CAIStateGroupWhisperPotionCombat_DisappearJump_0096af60.cpp: had the SAME "first fail block sunk" gap (hand259 LESSONS) and was
  matched by moving the early part into a FABLE_FORCEINLINE helper (JumpIntoSky) returning bool, with `if (!JumpIntoSky()) {...; return;}`.
  Also shows `CMainGameComponent::GetConstantFPS()` and temp `AIAction_PerformActionAndWait(CCreatureAction_X(GetCreature()), false)`.
- 00/96/CAIStateGroupTavernKeeper_LeanOnBar_009689c0.cpp: exits written out in full; still uses local models and pointer WalkToPoint (old spelling).
- 00/96/CAIStateGroupTavernKeeper_Update_00969470.cpp (the caller).

## 7. Old draft model errors
1. sizeof(PlayAnimation) 0xAC (pad 0x9c+0x0c) vs retail 0xB4 -> frame slot shift ("8 apart"). Use the shared header.
2. Wrong spellings (HasBeenPaid, GetPWorld, PMessageManager, GetWorldFrame, g_FramesPerSecond) and pointer params where PDB has const refs
   (WalkToPoint, AddMessage pos, PlayAnimation creature, PerformActionAndWait action).
3. RemoveMessage order residue: draft held the manager in a named local `requests`; retail loads arg (edx=[esi+64]) then ecx=[eax+60];
   write it as one expression `GetWorld().GetMessageEventManager().RemoveMessage(PaymentRequestMessage);`.
4. Creature: draft named local `CThing* creature = PBrain->PCreature;` -- retail loads [esi+4]->+0x18 early into edi before the pos init;
   per LESSONS an inline GetCreature() argument (`&GetCreature()`) loads early and picks retail's registers.
5. Block placement of the first exit: draft `if (!WalkToPoint || Cancelled) {...} else {...}`; tried duplicated exits (b,c,d) without success.
   Untried: the DisappearJump fix -- an inline helper (structure the walk+cancel test as a FABLE_FORCEINLINE bool helper or a nested
   `if (WalkToPoint(...)) { if (!GetPaymentCancelledMessage()) {...main...} }` then a single fail tail), and dropping the `else`.
6. pos: retail zeroes X,Y,Z ascending into locals+4 before GetWorld(); C3DVector(0.0f,0.0f,0.0f) named local or temp in the AddMessage call; keep zero stores before the GetWorld() call (draft order already right).
7. Unused local models (CInterfaceMap, GetInterface, CTCTavernBar, GFFloatRandom) -- delete.

## 0x0094D780

# DOSSIER 0x0094D780 (427 B): CAIStateGroup_TrollBase::Initialise

## 1. Retail shape (xdis.py 0094d780 0x1bb)
- thiscall, `ret 0x14`, so 5 stack args. No frame pointer (no push ebp/mov ebp,esp): /Oy (frame pointer omitted). No SEH prologue. Pushes ebx, ebp, esi and edi. ebp=0 is a "null" register (`xor ebp,ebp`).
- No `sub esp`: every local is homed in a dead ARGUMENT slot. [esp+0x24] (arg5 `name` after the 4 pushes) holds first the GetTC key 0x34, then the TC pointer, and finally the AoE def CDefPointer, which stays alive to the end. [esp+0x20] (arg4 slot) is reused for the GetDef out def AND then the GetSubDef out def (`mov esi,[esp+20h]` copies the first def to esi before the second call overwrites the slot). This is the "lane gap" hand259 recorded. The landed fix is the 0x00750BE0 idiom (section 5).
- Flow: CAIStateGroupBase::Initialise(args) (direct call). GetTC(TCI_PERCEIVE_THING=0x34): the bit test `test [eax+24h],100000h`, then the NON-const LowerBound 0x40F020 and the `it==end || key<it->key ? end` ternary. A missing TC reads the unset local [esp+0x24]. Then `TROLL_MAX_THROWING_DISTANCE = perceive->GetGiveUpChaseRadius()` (fstp [edi+24h]). Then `CDefPointer<const CAreaOfEffectAttackDef> aoe` (zeroed by `mov [esp+28h],ebp`, arg5 slot). `if (PCreature->PBaseDef->GetSubDef(&aoe))` (ecx=[creature+70h]); failing that, release aoe and return (NO defaults). `long idx = 0` (ebx). `if (aoe->ExplosionIndex(+0x40)) idx = it; else { if (!aoe->TrapIndex(+0x3c)) goto defaults; GetDef(TrapIndex,&thingDef) && thingDef->GetSubDef<CTrapDef>(&trap) ? idx = trap->ExplosionDefIndex(+0x50); release trap, thingDef }`. `if (!idx) goto defaults`. `GetDef(idx,&thingDef2) && thingDef2->GetSubDef<CExplosionDef>(&expl)`: MIN=r, STOP_POUNDING=r, STOP_THROWING=r+[0x0122DED8] (bytes start 00 00; presumably 1.0f), all with r=expl->ExplosionRadius(+0x2c). Release expl, then thingDef2. The common exit releases aoe.
- Defaults tail (0x94D910, placed after `ret`, jumps back into the epilogue so it ALSO releases aoe): MAX=28.0f (0x41E00000), MIN=8.0f, STOP_POUNDING=11.0f (0x41300000), STOP_THROWING=8.0f. Note: 8.0f is shared in eax, and the store order is +24,+28,+2c,+30 with eax loaded first.
- Each def release is inline `cmp r,ebp ; je ; dec [r+4] ; jne ; mov ; call [vt+4]`, i.e. the CDefPointer dtor. Order: inner (sub-def) before outer.

## 2. Name / signature (manifest label is correct apart from its doubled class prefix)
- `?Initialise@CAIStateGroup_TrollBase@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z`
  - Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, donor 0x00971F00)
  - FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv 0x01796C8D)
  - rtti-vtables.tsv CAIStateGroup_TrollBase vtable 0x0128C008 slot 1 = 0x0094D780, which overrides CAIStateGroupBase slot 1 = 0x008FCEC0 (Initialise).
- `public: virtual void __thiscall CAIStateGroup_TrollBase::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)`.
- No code callers: only the vtable refs at 0x0128C00C, 0x0128C048 (EarthTroll) and 0x0128D0A4 (RockTroll) inherit it.

## 3. Callees (landed file / landed name / PDB name). The old draft MISNAMES most of them.
| addr | landed row | correct name | draft spelling |
|---|---|---|---|
| 0x008FCEC0 | 00/8f/CAIStateGroupBase_Initialise_008fcec0.cpp | ?Initialise@CAIStateGroupBase@@UAEX...@Z | OK |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (non-const QAE) via the inline CThing::GetTC | local CInterfaceMap::LowerBound (WRONG, model locally) |
| 0x00744460 | 00/74/DrawGetSeaLevel_DrawGetSeaLevel_00744460.cpp (address stub `C00744460::M`, `fld [ecx+4Ch] ; ret`) | Ego_r ?GetGiveUpChaseRadius@CTCPerceiveThing@@QBEMXZ; +0x4C = Ego CTCPerceiveThing.GiveUpChaseRadius (struct_layouts, offset 76) | `GetSightRange` (WRONG: SightRadius is +0x40) |
| 0x008C79B0 | 00/8c/CDefClassBase_GetSubDefCAreaOfEffectAttackDef_008c79b0.cpp | ??$GetSubDef@VCAreaOfEffectAttackDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCAreaOfEffectAttackDef@@@@@Z (class-name literal in the body) | `CDefHolder::GetCreatureSubDef<CTrollCreatureDef>` (WRONG) |
| 0x0044C6B0 | 00/44/CGameDefinitionManager_Get_0044c6b0.cpp | ?Get@CGameDefinitionManager@@SIAAV1@XZ (static __fastcall, returns ref) | OK |
| 0x0042AF3C (x2) | 00/42/CDefinitionManager_GetDefT_0042af3c.cpp (landed as address-named GetDef_0042af3c) | CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const CThingBaseDef>*) const (rebuild/include/engine/CTCActionUseBase.h:34, CTCCameraCollision.h:21 and the landed 0x00750BE0 all say GetDef<CThingBaseDef>; 119 callers; `ret 8`) | `GetSpecialAbilityDef` / `GetWeaponDef` (WRONG) |
| 0x00788E53 | 00/78/CDefClassBase_GetSubDefCTrapDef_00788e53.cpp | ??$GetSubDef@VCTrapDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTrapDef@@@@@Z | `CSpecialAbilityDef::GetAbilitySubDef` (WRONG) |
| 0x00751190 | 00/75/CDefClassBase_GetSubDefCExplosionDef_00751190.cpp | ??$GetSubDef@VCExplosionDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCExplosionDef@@@@@Z | `CWeaponDef::GetThrowRangeSubDef` (WRONG) |
- The virtual slot 1 (+4) on defs is the CDefPointer release (destroy at refcount 0).
- NAME_FIXES candidate: 0x00744460 is landed as the address stub `C00744460::M` in file DrawGetSeaLevel_*. Its real name is CTCPerceiveThing::GetGiveUpChaseRadius() const, with two sources: the Ego_r name and the Ego offset +0x4C.

## 4. Members
- this (CAIStateGroup_TrollBase, rebuild/include/engine/CAIStateGroup_TrollBase.h, generated FLAT with no base class, size 0x34): TROLL_MAX_THROWING_DISTANCE +0x24, TROLL_MIN_THROWING_DISTANCE +0x28, TROLL_STOP_POUNDING_DISTANCE +0x2c, TROLL_STOP_THROWING_DISTANCE +0x30 (all float). PEnemy +0x14 is CIntelligentPointer<CThingCreatureBase>. The header offers no opt-in macros. The landed siblings either model `class CAIStateGroup_TrollBase : public CAIStateGroupBase` locally (0x0094D930) or derive `_Methods : CAIStateGroup_TrollBase` from the generated header (0x0094DBC0).
- CAIStateGroupBase (engine/CAIStateGroupBase.h): PBrain +0x04 (CAIBrain*).
- CAIBrain (engine/CAIBrain.h): PCreature +0x18 (the siblings model it as CThing*).
- CThing (engine/CThing.h): component bitset (+0x20 words; bit 52 is in word +0x24), Components CVectorMap +0x44, PBaseDef +0x70 `CDefPointer<const CThingBaseDef>`. Opt-in macros: FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC/PeekTC __forceinline, line 324ff) and FABLE_RETAIL_STLP_BITSET_INDEX (bitset test). Use GetTC (NON-const, which reaches 0x40F020). PeekTC would use the const 0x4365B0.
- CTCPerceiveThing (engine/CTCPerceiveThing.h:75): `float GetGiveUpChaseRadius() const;` is declared but has NO inline body and no opt-in macro, so an out-of-line call is fine (retail calls it). GiveUpChaseRadius is at line 116.
- Retail CDefClassBase is 4 bytes SHORTER than Ego_r (landed 0x008C6CE0 note). So:
  - CAreaOfEffectAttackDef: TrapIndex at retail +0x3C (Ego +0x40), ExplosionIndex at retail +0x40 (Ego +0x44).
  - CTrapDef: ExplosionDefIndex at retail +0x50 (Ego +0x54).
  - CExplosionDef: ExplosionRadius at retail +0x2C (landed 0x00750BE0 line 43: `float ExplosionRadius; // +0x2C`; engine/CExplosionDef.h is padded, size 0x70).
  - There are no shared headers for CAreaOfEffectAttackDef or CTrapDef (local model needed; 0x008C79B0 defines CAreaOfEffectAttackDef : CParentDefClassBase under FABLE_RETAIL_DEF_CLASS_BASE).
- The old draft models these LOCALLY although shared headers exist: CThing (engine/CThing.h), CAIBrain (engine/CAIBrain.h), CAIStateGroupBase (engine/CAIStateGroupBase.h), CDefPointer (engine/CDefPointer.h), CTCPerceiveThing (engine/CTCPerceiveThing.h), CVectorMap (engine/CVectorMap.h; the draft has CInterfaceMap), CGameDefinitionManager / CDefinitionManager, CExplosionDef and CDefClassBase. Its fictional classes are CTrollCreatureDef, CSpecialAbilityDef, CAbilityDef, CWeaponDef, CThrowRangeDef and CDefHolder.

## 5. Callee visibility (Q32 lesson) -- the key to the arg-slot homing
- GetDef 0x0042AF3C and GetSubDef<CExplosionDef> 0x00751190 are out of line in retail, but the landed 0x00750BE0 (CTCExplodingObject ctor, same 0x75 neighbourhood as 0x751190) gives them same-TU "out-slot-only" STAND-IN bodies under `#pragma auto_inline(off)` + `__declspec(noinline)` (the body only writes `out->Object` from an extern `Find_*` call). VC7.1 then knows that the def pointer's address does not escape, which lets it home locals in dead arg slots (the "landed 0x00848730 idiom"). See 00/75/CTCExplodingObject_CTCExplodingObject_00750be0.cpp lines 44-92. This is exactly what hand259 called a "lane gap". Apply it to GetSubDef<CAreaOfEffectAttackDef>, GetDef<CThingBaseDef>, GetSubDef<CTrapDef> and GetSubDef<CExplosionDef> (and maybe to the stub CAIStateGroupBase::Initialise or GetGiveUpChaseRadius, to free the arg slots).
- GetTC is __forceinline in the shared CThing.h (owner decision 2026-11-01). LowerBound 0x40F020 is out of line (engine/CVectorMap.h).
- CDefPointer release inline: engine/CDefPointer.h (the 0x008C79B0 row uses FABLE_RETAIL_DEF_CLASS_BASE with engine/CParentDefClassBase.h and engine/CDefPointer.h).

## 6. Sibling landed rows (same TU: CAIStateGroup_TrollBase, 0x0094D680..0x0094DD90)
- 00/94/CAIStateGroupTrollBase_IsStillRunnable_0094d930.cpp: `#define FABLE_RETAIL_STLP_BITSET_INDEX` + engine/CVectorMap.h, `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED` + engine/CThing.h, FABLE_RETAIL_CINTELLIGENTPOINTER + engine/CIntelligentPointer.h; local CAIBrain / CAIStateGroupBase / `CAIStateGroup_TrollBase : CAIStateGroupBase`; uses `PBrain->PCreature->PeekTC(TCI_TARGETING,&t)`. This is the best template for the GetTC part.
- 00/94/CAIStateGroupTrollBase_ForceRiseUp_0094dbc0.cpp: engine/CAIStateGroup_TrollBase.h + GETTC_EXPANDED, `_Methods` derivation.
- 00/94/CAIStateGroupTrollBase_GetEnemyPredictedPos_0094d680.cpp, _HasEnemyBeenKnockedDown_0094d9c0.cpp, CAIStateGroupRockTroll_IsRunnable_0094dac0.cpp: older local-model rows (fable_compiler.h, #pragma intrinsic(sqrt), auto_inline(off) stand-ins).
- 00/75/CTCExplodingObject_CTCExplodingObject_00750be0.cpp: the GetDef + GetSubDef<CExplosionDef> stand-in idiom (section 5).
- 00/8f/CAIStateGroupBase_Initialise_008fcec0.cpp: the base Initialise signature.

## 7. Old draft model errors
- Wrong def chain: the "troll creature def / special ability / weapon / throw range" chain is fictional. The real chain is PBaseDef->GetSubDef<CAreaOfEffectAttackDef>; ExplosionIndex(+0x40), or else TrapIndex(+0x3C), then GetDef<CThingBaseDef>, GetSubDef<CTrapDef> and ExplosionDefIndex(+0x50); then GetDef<CThingBaseDef>(explosionIdx), GetSubDef<CExplosionDef> and ExplosionRadius(+0x2C).
- GetSightRange is wrong: it is GetGiveUpChaseRadius (+0x4C).
- Missing visible stand-in bodies for the out-of-line def callees: this is the arg-slot homing the draft could not reproduce. Retail has no sub esp at all.
- The GetTC was a hand-rolled local template: use the shared FABLE_RETAIL_CTHING_GETTC_EXPANDED GetTC (non-const).
- GetWeaponDef was an inline cast wrapper around the same function: in retail both calls are the one GetDef<CThingBaseDef> 0x42AF3C with a `CDefPointer<const CThingBaseDef>*` out.
- Control flow is otherwise right (GetSubDef<AoE> failure returns without defaults; the defaults block is reached from `TrapIndex==0` and `idx==0` and still releases aoe). Keep a single goto/label or an equivalent so the defaults tail lands after `ret`.

## 0x0064C869

# DOSSIER 0x0064C869 NPlayerGui::CDrawSpiritTargetingCursor::Draw (428 B)

## 1. Retail shape (xdis.py 0064c869 0x1bc)
- __thiscall, `ret 4`, `push ebp; mov ebp,esp; sub esp,0x74` -> #pragma optimize("y",off). No SEH prologue (no /EH frame: temporaries'
  dtors are plain calls). Size idioms: `push 8; pop ebx`, `and [ebp-4],0`, `leave` -> #pragma optimize("s",on). Neighbour row
  CDrawWorldTime::Draw 0x0064CA15 is `// Flags: /O1 /Oy-`.
- Frame map: [ebp-4] i (loop counter kept in MEMORY, `inc [ebp-4]; cmp [ebp-4],4; jl`), [ebp-8] CPlayer& player, [ebp-0xc] targeting TC*,
  [ebp-0x10] SHARED slot: hero (address taken by IS_THING_ALIVE) -> TC key (`mov [ebp-10h],ebx`=8) -> guiDef ref; [ebp-0x18] CRGBColour
  (hidden-return out); [ebp-0x20] CIntelligentPointer temp (8B); [ebp-0x2c] C3DVector pos; [ebp-0x74] CEnginePrimitiveSprite (0x48).
- edi = i*8 computed ONCE (`mov edi,[ebp-4]; shl edi,3`) and reused for every PTargetedThings[i] (ebx=&PTargetedThings[i] for the !=).
- for(i=0;i<4;) loop with every early-out jumping to the `inc` (continue form; top-test absent -> do/for rotated by compiler).

## 2. Identity (manifest label is right, class needs namespace)
- egor_pdb_names.tsv 0x0068DAF6 and _fablewin_api.tsv 0x017CB01E: `?Draw@CDrawSpiritTargetingCursor@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z`
  => `void NPlayerGui::CDrawSpiritTargetingCursor::Draw(const CEnginePrimitivePrepareInfo& info)`, public, NON-virtual, non-const.
- Single caller 0x004397E2 (not landed; xref.py) -- presumably CPlayerGui's draw dispatcher.
- Class: RTTI .?AVCDrawSpiritTargetingCursor@NPlayerGui@@ : CDrawBase, vptr 0x0125A5E8 (landed ctor 0x0064DD4D header comment).
  Members (struct_layouts_egor.tsv): only PTargetedThings vector<CIntelligentPointer<CThing>> (PDB +0x2c, retail +0x28: CDrawBase retail sizeof 0x28).
- NO shared header NPlayerGui_CDrawSpiritTargetingCursor.h exists. Model it like NPlayerGui_CDrawHighlightingTargetingCursor.h
  (`class X : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo&); private: std::vector<CIntelligentPointer<CThing> > PTargetedThings; };`).

## 3. Callees (landed file | landed/PDB name | draft spelling)
| addr | landed row | PDB name | draft |
|---|---|---|---|
| 0x004498C0 | 00/44/CPlayerManager_GetPlayer_004498c0.cpp | ?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z returns CPlayer& | CPlayer* -- WRONG type |
| 0x00487DC0 | 00/48/CPlayer_GetPControlledCreature_00487dc0.cpp | ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ (const) | "GetCharacterThing" returning CThing* -- WRONG |
| 0x00487520 | 00/48/CPlayer_IsMainPlayer_00487520.cpp | ?IsMainPlayer@CPlayer@@QBE_NXZ (called out of line here) | ok |
| 0x0043DBF0 | 00/43/global_ISTHINGALIVECThingBuilding_0043dbf0.cpp | ??$IS_THING_ALIVE@PBVCThingPlayerCreature@@@@YI_NABQBVCThingPlayerCreature@@@Z | draft used CThing* -> wrong instantiation; hero must be `const CThingPlayerCreature*` |
| 0x00410DE0 | 00/41/STLbitset_test_00410de0.cpp | std::bitset<274>::test | (inside GetTC/PeekTC) |
| 0x004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound **const** | const => PeekTC on a const hero |
| vslot +0x84 | -- | CTCTargetingBase::GetPTargetedThing() const (slot 33; CTCTargetingBase.h; same use in sibling 0x0064BFBA) | "GetTarget" |
| 0x005CBDD2 | 00/5c/global_operator_005cbdd2.cpp | ??$?9VCThing@@@@YI_NPBVCBaseObject@@ABV?$CIntelligentPointer@VCThing@@@@@Z operator!=(const CBaseObject*, const CIntelligentPointer<CThing>&) | "TargetDiffers" -- WRONG name |
| 0x004A96F0 | 00/4a/CIntelligentPointer_CThing_CIntelligentPointer_004a96f0.cpp | ??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z | ok |
| 0x004A9760 | 00/4a/CIntelligentPointerCThingCreatureBase_operator_004a9760.cpp | ??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z | ok |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | ??1CBaseIntelligentPointer@@UAE@XZ (virtual dtor) | draft non-virtual |
| 0x004CC340 | 00/4c/global_ISTHINGALIVECIntelligentPointerCThing_004cc340.cpp | IS_THING_ALIVE<CIntelligentPointer<CThing> > | ok |
| 0x00438570 | 00/43/CPlayerGui_PeekGuiDef_00438570.cpp | ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ static fastcall, returns const& | draft free fn returning ptr |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | ok (draft returns CThing*) |
| 0x007EA2F0 | 00/7e/NTargeting_GetTargetingPositionOnThing_007ea2f0.cpp | ?GetTargetingPositionOnThing@NTargeting@@YI?AVC3DVector@@ABVCWorld@@ABVCThing@@M@Z | "GetTargetPosition(camera,...)" -- WRONG; arg1 is *World (+0xc) |
| 0x00449B60 | 00/44/CPlayerManager_GetMultiplayerColour_00449b60.cpp | GetMultiplayerColour(long) const -> CRGBColour by value (hidden return ptr) | non-const in draft |
| 0x006558CE | 00/65/CEnginePrimitiveSprite_CEnginePrimitiveSprite_006558ce.cpp | 15-arg ctor (CEnginePrimitiveSprite.h line 26) | ok shape |
| vslot +0x50 | -- | CIEngine::AddPrimitive(const CEnginePrimitive&) slot 20 (CIEngine.h:94) on info.Engine | "Renderer->AddPrimitive" |

## 4. Members
- this+0x08 CDrawBase::PlayerManager, +0x0C CDrawBase::World (CWorld*; draft called it "PCamera"), +0x10 CDrawBase::Component
  (CMainGameComponent*; draft "POwner"), +0x18 CDrawBase::Drawing (draft "Drawn"; FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING offers
  inline SetAsDrawing(true)), +0x28 PTargetedThings (vector; begin at +0x28). Header: engine/NPlayerGui_CDrawBase.h.
- Component+0x28 = CMainGameComponent::PDisplayEngine (scoped_ptr<CDisplayEngine>); inline
  `PeekDisplayEngine() const` exists under FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE (CMainGameComponent.h:253).
- DisplayEngine+0x70 byte = CDisplayEngine::LetterBox (CLetterBoxModeInfo, CDisplayEngine.h:86 as `unsigned char LetterBox[0x20]`);
  the bool test is FableWin `?IsLetterBoxMode@CDisplayEngine@@QBE_NXZ` (inline; NOT in any header -- author adds an inline accessor reading
  the first byte). Draft's "State->Hidden" = `Component->PeekDisplayEngine().IsLetterBoxMode()`.
- player+0x28 = CPlayer::Number; inline `GetNumber() const` under FABLE_RETAIL_PLAYER_GETNUMBER (CPlayer.h:165).
- hero+0x20 UsingComponents, +0x44 Components (CThing.h); target+0x60 CThing::PhysicsTC; PhysicsTC+0x34 = CTCPhysicsBase::Radius
  (struct_layouts_egor; draft said "Height").
- info+0x04 = Interpolate.Current.WFInterpolate (CInterpolationInfo/Set layouts; CEnginePrimitivePrepareInfo.h has Interpolate as opaque
  bytes [0x34]); info+0x34 = CEnginePrimitivePrepareInfo::Engine (CIEngine*).
- guiDef+0x740 = CPlayerGuiDef::TargetingCursorSectionEnemyEmptyGraphic (CPlayerGuiDef.h:509, static-asserted).
- CRGBColour (engine/CRGBColour.h, B,G,R,A): args pushed are byte+2 (R), +1 (G), +0 (B) -> sprite(..., 2, 0xff, c.R, c.G, c.B, 0.0f, 0, 0).
- Draft models LOCALLY classes that have shared headers: C3DVector, CRGBColour, CThing, CIntelligentPointer/CBaseIntelligentPointer,
  CPlayer, CPlayerManager, CPlayerGuiDef, CEnginePrimitiveSprite, CEnginePrimitivePrepareInfo, CTCTargeting(Base), CDrawBase pieces.

## 5. Callee visibility / opt-ins (copy sibling 0x0064BFBA's set)
- TC lookup: expanded inline with const LowerBound out of line -> FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED + FABLE_RETAIL_STLP_BITSET_TEST_DECL;
  call `hero->PeekTC(TCI_TARGETING, &targeting)` with `const CTCTargetingBase* targeting`. The expanded body's key-local/map-pointer/ternary
  shape matches retail exactly (`mov [ebp-10h],ebx` after test, `add edi,44h; mov edi,[edi+4]`, `cmp/je/cmp/jle/mov eax,edi`).
- CIntelligentPointer: FABLE_RETAIL_CINTELLIGENTPOINTER, _CTOR_OUTOFLINE, _ASSIGN_OUTOFLINE, _COMPARE (operator!= 0x005CBDD2 decl).
- IS_THING_ALIVE: both instantiations called out of line; sibling declares `template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T&);`
  (ThingAlive.h has FABLE_RETAIL_IS_THING_ALIVE / _NOINLINE variants).
- PeekGuiDef: FABLE_RETAIL_CPLAYERGUI + FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF. CPlayer/CPlayerManager: FABLE_RETAIL_PLAYER, FABLE_RETAIL_CPLAYERMANAGER.
- NTargeting.h declares GetTargetingPositionOnThing; CTCTargetingBase.h slot via FABLE_RETAIL_CTCBASE + FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE (sibling).

## 6. Siblings
- rebuild/candidates/manual/pending/2026-11-08_linux_hand867/differ/0064bfba.cpp -- CDrawHighlightingTargetingCursor::Draw, DIFFER 2223v2223
  md=40 (frame-slot only): near-twin idioms (PeekTC(TCI_TARGETING,&targeting), `targeting->GetPTargetedThing() != PTargetedThing`,
  `PTargetedThing = targeting->GetPTargetedThing();`), `#pragma optimize("s", on)` + long FABLE_RETAIL_* list (lines 48-95).
- 00/64/NPlayerGuiCDrawSpiritTargetingCursor_CDrawSpiritTargetingCursor_0064dd4d.cpp -- same class ctor, `#pragma optimize("s", on)`.
- 00/64/NPlayerGuiCDrawWorldTime_Draw_0064ca15.cpp -- next address, `// Flags: /O1 /Oy-`.
- 00/64/NPlayerGuiCDrawSpiritScores_Draw_00644424.cpp -- `#pragma optimize("y",off)` + `("s",on)`, GetMultiplayerColour use.

## 7. Old draft model errors
- Wrong types: hero must be `const CThingPlayerCreature*` (GetPControlledCreature const, IS_THING_ALIVE<const CThingPlayerCreature*>),
  player `CPlayer&` (GetPlayer returns ref; [ebp-8] holds it), targeting const via PeekTC (const LowerBound 0x004365B0).
- Call order in retail: GetPlayer -> GetPControlledCreature -> IsMainPlayer -> IS_THING_ALIVE(hero) -> PeekTC.
- Uses PTargetedThings[i] repeatedly; retail CSEs i*8 into edi -- try a named `CIntelligentPointer<CThing>& target = PTargetedThings[i];`
  (NOTES "next") or `PTargetedThings.begin()` reads; retail re-reads begin `[esi+28h]` each use (so no cached begin pointer, only i*8).
- Z math order: fld pos.Z; fadd Radius; fadd 0.35f => `pos.Z = pos.Z + target->PhysicsTC->Radius + 0.35f` (not `+= r+0.35f`). 0.35f at 0x0125A264.
- GetTargetingPositionOnThing(*World, *target, info.Interpolate.Current.WFInterpolate): target from `PTargetedThings[i].GetPItem()` (ebx), PeekGuiDef called BEFORE GetPItem and after `Drawing = true`.
- Letterbox test (`!Component->PeekDisplayEngine().IsLetterBoxMode()`) inline, after IS_THING_ALIVE(PTargetedThings[i]).
- Colour arg is `player.GetNumber()` inline (player reloaded from [ebp-8]).

## 0x0095E0B0

# DOSSIER 0x0095E0B0 (428 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 0095e0b0 0x1bc)
- thiscall, `ret` (no args), NO frame pointer (`sub esp,0CCh ; push ebx/ebp/esi/edi`), NO SEH prologue -> plain /O2 /Oy, no EH state.
- Frame 0xcc = bool inRange (esp+0x13, frame+3) + one shared 0xc8 action slot at esp+0x14 (frame+4..+0xcc).
  All three action temporaries (Aim 0xb0, Fire 0xc8, Reload 0xb0) reuse the SAME slot -> the frame is set by the 0xc8 Fire action.
- Registers: esi=this, edi=&PTarget (this+0x14), ebx=constant 1 (from the first alive test, reused as `push ebx` for `true` and
  ACTION_REPEAT_UNTIL_FINISHED), later ebx=result; ebp = the outgoing IP-copy slot (first two copies), later creature.
- Flow: alive(PTarget) INLINE (GetPItem twice, `mov cl,[eax+91h] ; mov ebx,1 ; test bl,cl`) -> Strafe(1, copy, copy, 15.0f, &inRange, 5, true)
  (both by-value copies INLINE: base ctor 0xA01B10, vptr 0x01237F1C, GetPItem(edi), SetPItem) ==2 -> alive INLINE again
  (`test byte [eax+91h],bl`) -> Aim: push 0, copy INLINE, creature=[[esi+4]+18h], ctor 0x718650, call 0x97DCB0, ebx=result,
  ~IP at action+0xa8 (0xA01C10, i.e. implicit ~CCreatureAction_AimMissileWeapon inlined), ~CCreatureActionBase 0x693EF0;
  ==2 -> alive OUT OF LINE 0x69CE20 -> Fire: GetPItem(edi) (ebx) and creature (ebp) fetched FIRST, then push 0, copy OUT OF
  LINE 0x654D58, push target, push creature, ctor 0x8D5110, call 0x97DCB0, dtor 0x78ADF0 (out of line) ; ==2 -> alive 0x69CE20
  -> Reload: creature (ebx) fetched before push 0, copy 0x654D58, ctor 0x718710, call 0x97DCB0 (result ignored), dtor 0x929E60
  (out of line) -> every exit lands on one `mov ecx,esi ; call 0x8FCF40` (Terminate) then epilogue.
- The inline->out-of-line switch for IS_THING_ALIVE and the IP copy ctor mid-function is the VC7.1 inline budget/depth
  behaviour (NOTES hand257 line 49 already tried: "an inline IS_THING_ALIVE whose budget runs out").

## 2. Name / signature (label is RIGHT)
- Ego_r.pdb (egor_pdb_names.tsv 009826b0): ?Update@CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets@@MAEXXZ -> protected virtual void Update().
- FableWin (_fablewin_api.tsv 018158cb / 02c23140): same mangled name.
- Vtable 0x0128C894 slot 12 = 0x0095E0B0 (rtti-vtables.tsv); slot 13 InitStateGroup 0x0095E0A0, slot 11 IsRunnable 0x0095E260 --
  same InitStateGroup/Update/IsRunnable order as Ego_r (9826a0/9826b0/982870). Only xref: the vtable (.rdata 0x0128C8C4).
- Mangled leaf: ?Update@CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets@@MAEXXZ

## 3. Callees (landed file / PDB name; draft disagreements flagged)
| addr | landed file | PDB spelling | draft |
|---|---|---|---|
| 0xA01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | draft returns CThing* (WRONG, CBaseObject*) |
| 0xA01B10 | 00/a0/..._CBaseIntelligentPointer_00a01b10.cpp | ??0CBaseIntelligentPointer@@QAE@XZ | ok |
| 0xA01B90 | 00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp | ?SetPItem@..@@QAEXPAVCBaseObject@@@Z | draft takes CThing* (WRONG) |
| 0xA01C10 | 00/a0/..._00a01c10.cpp | ??1CBaseIntelligentPointer@@UAE@XZ | ok; draft member is `Data`, header calls it PData (+4) |
| 0x97EF60 | 00/97/CAIStateGroupBase_AIActionProjectileAttackAndStrafeToThingProximit_0097ef60.cpp | ?AIAction_ProjectileAttackAndStrafeToThingProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@1MAA_NW4EMovementType@@_N@Z | draft: `bool*` (WRONG: bool&), declared on local RangedMethods subclass (WRONG: CAIStateGroupBase protected) |
| 0x97DCB0 | 00/97/CAIStateGroupBase_AIActionPerformActionTargetFaceAndWait_0097dcb0.cpp | ?AIAction_PerformActionTargetFaceAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@V?$CIntelligentPointer@$$CBVCThing@@@@_N@Z | draft: CCreatureActionBase* (WRONG: const&), `&Temp()` |
| 0x718650 | 00/71/CCreatureActionAimMissileWeapon_..._00718650.cpp | ??0CCreatureAction_AimMissileWeapon@@QAE@AAVCThingCreatureBase@@@Z | draft ptr (WRONG: reference) |
| 0x8D5110 | 00/8d/CCreatureActionFireMissileWeapon_..._008d5110.cpp | ??0CCreatureAction_FireMissileWeapon@@QAE@AAVCThingCreatureBase@@PBVCThing@@@Z | draft (ptr, CThing*) WRONG: (CThingCreatureBase&, const CThing*) |
| 0x78ADF0 | 00/78/..._0078adf0.cpp | ??1CCreatureAction_FireMissileWeapon@@UAE@XZ (implicit, out of line) | ok |
| 0x718710 | 00/71/CCreatureActionReloadMissileWeapon_..._00718710.cpp | ??0CCreatureAction_ReloadMissileWeapon@@QAE@AAVCThingCreatureBase@@@Z | draft ptr (WRONG) |
| 0x929E60 | 00/92/CCreatureActionReloadMissileWeapon_..._00929e60.cpp | ~CCreatureAction_ReloadMissileWeapon (implicit, out of line) | ok |
| 0x693EF0 | 00/69/CCreatureActionBase_CCreatureActionBase_00693ef0.cpp | ??1CCreatureActionBase | ok (called directly only after Aim: Aim dtor fully inlined) |
| 0x69CE20 | 00/69/global_IsThingAlive0069ce20_0069ce20.cpp (landed leaf ?IsThingAlive_0069ce20@@YI_NAAV?$CIntelligentPointer@VCThing@@@@@Z, descriptive) | FableWin has ??$IS_THING_ALIVE@V?$CIntelligentPointer@$$CBVCThing@@@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@@Z (0x0184443c / 0x01f2f960) -> NAME_FIXES candidate: IS_THING_ALIVE<CIntelligentPointer<const CThing> >, __fastcall, const& | draft IS_THING_ALIVE ok in spirit |
| 0x654D58 | 00/65/CIntelligentPointer_constCThing_CIntelligentPointer_00654d58.cpp | ??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@ABV0@@Z (copy ctor COMDAT) | draft local ctor |
| 0x8FCF40 | 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | ?Terminate@CAIStateGroupBase@@QAEXXZ | draft on local subclass |
- Enums (rebuild/include, engine/fable_ai_action_enums.h): AI_ACTION_FINISHED=2, ACTION_REPEAT_UNTIL_FINISHED=1,
  AI_MOVEMENT_RUNNING=5. Draft's made-up AI_ACTION_DONE / AI_REPEAT_1 / MOVE_5 are WRONG spellings.

## 4. Members
- this+0x04 CAIStateGroupBase::PBrain (CAIBrain*) -> +0x18 CAIBrain::Creature (CThingAICreature*; engine/CAIBrain.h,
  FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE makes it `CThingAICreature&`; FABLE_RETAIL_CAIBRAIN_GETCREATURE +
  FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE give CAIStateGroupBase::GetCreature() const -> CThingAICreature&).
- this+0x14 PTarget (CIntelligentPointer<const CThing>) -- engine/CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets.h
  (generated, sizeof 0x1c, PTarget opaque `unsigned char[8]`, NO FABLE_RETAIL_* opt-in; base flattened). Ego_r name PTarget.
  NOTE: landed sibling IsRunnable 0x0095E260 calls it `PTargetThing` -- wrong per PDB (NAME_FIXES candidate).
- CThing +0x91 bit0 = dead flag (read via IS_THING_ALIVE -> IsDead()); engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE
  (body `thing!=0` / `!thing->IsDead()`; FABLE_RETAIL_IS_THING_ALIVE_NOINLINE variant).
- Action objects (shared headers, retail sizes): CCreatureActionBase.h (0xa8, FABLE_RETAIL_CCREATUREACTIONBASE),
  CCreatureAction_AimMissileWeapon.h (0xb0, PAmmo +0xa8 CIntelligentPointer<CThingObject>), CCreatureAction_FireMissileWeapon.h
  (0xc8 under FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON; PTarget +0xa8 .. PSpecialShotDef +0xc4). No CCreatureAction_ReloadMissileWeapon.h
  in engine/ (Ego_r 0xbc -> retail 0xb0, IP member at +0xa8, matches `lea ecx,[esp+0BCh]` pattern).
- Draft models LOCALLY with shared headers available: CThing, CAIBrain (as CAIBrainRetail), CBaseIntelligentPointer,
  CIntelligentPointer (as CIntelligentPointerThing), CCreatureActionBase, CCreatureAction_AimMissileWeapon,
  CCreatureAction_FireMissileWeapon, plus a RangedMethods subclass instead of the real class + CAIStateGroupBase.h.

## 5. Callee visibility / opt-ins
- engine/CAIStateGroupBase.h: FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS (+Terminate), _AIACTIONS, _TARGETFACE
  (PerformActionTargetFaceAndWait decl), _GETCREATURE. NO opt-in yet for AIAction_ProjectileAttackAndStrafeToThingProximity
  (only _PROJECTILESTRAFE for ...ToPos) -> author must add one (or declare per row like the landed 0x97EF60 row does).
- engine/CIntelligentPointer.h: FABLE_RETAIL_CINTELLIGENTPOINTER, _COPY (inline copy ctor), _COPY_OUTOFLINE (decl only,
  0x654D58; introduced for row 0x00960120 DeedReactionRoot::Update). They are alternative declarations of one ctor; retail
  needs inline for the first 3 copies and out-of-line for the last 2 -> use _COPY (inline body) and rely on the inline budget, as
  the 0x69CE20 IS_THING_ALIVE switch shows the same budget exhaustion at the same point.
- Aim dtor is implicit and fully inlined (~IP member + ~CCreatureActionBase call); Fire/Reload dtors out of line (COMDATs
  0x78ADF0 / 0x929E60) -> Fire/Reload classes need user-declared/non-inline dtor or budget exhaustion.
- No STLport helpers called.

## 6. Siblings
- 00/96/CAIStateGroupBanditKingThrowWeapons_Update_00961640.cpp (Flags: none): defines FABLE_RETAIL_CAISTATEGROUPBASE,
  _MEMBERS, _GETCREATURE, _AIACTIONS, _PERFORMACTION, _TARGETFACE, FABLE_RETAIL_CAIBRAIN_GETCREATURE, _CREATURE_REFERENCE,
  FABLE_RETAIL_CINTELLIGENTPOINTER(+_GET_ORIGINAL_CHAIN,_CONVERT_MUTABLE,_DEREFERENCE), FABLE_RETAIL_CCREATUREACTIONBASE;
  idiom `AIAction_PerformActionTargetFaceAndWait(CCreatureAction_X(GetCreature(),...), Player, false);`
- 00/97/CAIStateGroupCreatureTakeCover_AttackTargetThing_00979f70.cpp (RELOCATION_MATCH): same aim/fire pattern,
  `AIAction_PerformActionAndWait(CCreatureAction_FireMissileWeapon(*shooter, PTargetThing.GetPItem()), false)`; exits written as
  separate Terminate()/return copies which VC7.1 merges.
- 00/97/..._0097ef60.cpp (the Strafe callee; uses #pragma auto_inline(off/on), FABLE_RETAIL_CINTELLIGENTPOINTER_COPY).
- Same class: 00/95/..._IsRunnable_0095e260.cpp, 00/95/..._InitStateGroup_0095e0a0.cpp (stub), 00/90/... ctor/dtor/GetStateGroupType.

## 7. Draft model errors
- Fire action modelled 0xb8 (0xa8+0x10) -> frame 0xbc; retail Fire is 0xc8 -> frame 0xcc. Use the shared header.
- Fire: retail evaluates PTarget.GetPItem() and the creature BEFORE the by-value IP copy; Reload: creature before the copy.
  Suggests named locals (`const CThing* target = PTarget.GetPItem();` / creature reference) before the call, or the
  Fire ctor args evaluated first; Aim (inline copy) loads creature after the copy.
- esi/edi swap: draft's `Target()` cast helper; use the real PTarget member (CIntelligentPointer<const CThing>) so edi=&PTarget.
- Pointer-vs-reference signatures (Strafe bool&, TargetFace const&, action ctors CThingCreatureBase&), GetPItem/SetPItem types.
- Control flow (nested ifs, all failures -> one Terminate, last result ignored) matches retail.

## 0x00A11F20

# DOSSIER 0x00A11F20 CPhysicsMeshObject::IsSpherePartContainedInBoundingBox (428 B)

## 1. Retail shape (xdis 00a11f20 0x1bc)
- __thiscall, `ret 8` (2 stack args: const C3DVector& pos, float radius). No frame pointer (esp-relative), no SEH, `sub esp,40h` frame.
- Frame (0x40): [esp+0]=radius*OO_Scale (computed FIRST, 0x00A11F23..2A); [esp+4..0xC]=local mesh-space C3DVector;
  [esp+0x10..0x3C]=a LOCAL CMatrix3x4 (inverse) — E11@+10 E12@+14 E13@+18 E21@+1C E22@+20 E23@+24 E31@+28 E32@+2C E33@+30 E41@+34 E42@+38 E43@+3C.
  Only E11,E21,E31,E12,E22,E32,E43 are stored; E13,E23,E33,E41,E42 stay on the x87 stack. So: an in-place `CMatrix3x4 inv; inv.SetInverse(PositionMatrix);`
  (frame 0x40), NOT a returned temporary (old draft got 0x70).
- 1/det (`fdivr [0x0122DED8]` = 1.0f) is homed into the dead `radius` parameter slot [esp+0x48] (0x00A11F70) -> radius must be dead after the first scale.
- Inverse = exactly the header CMatrix3x4::SetInverse body (FABLE_RETAIL_CMATRIX3X4): cofactors (E32*E13-E12*E33), (E12*E23-E22*E13), (E22*E33-E32*E23) computed once, det = E11*c + E21*c + E31*c,
  then E11,E21,E31,E12,E22,E32,E13,E23,E33 in header order; translation row E41/E42/E43 = -(... ) with fchs; retail's evaluation order
  for E4k is inv.E3k*m.E43, +inv.E2k*m.E42, +inv.E1k*m.E41 (0x00A12006..23).
- Transform (0x00A1205C..0x00A120BD): each row = E1k*p.X, + E3k*p.Z, + E2k*p.Y, + E4k (X,Z,Y order, all three rows) -> local at [esp+4].
  This matches the header's `TransformPoint(const C3DVector&,C3DVector&)` ACCUMULATED Y form (dst=E12*x; +=E32*z; +=E22*y; +=E42) for all
  three rows (header X row is the `E11*x+(E21*y+E31*z)+E41` form -- probe). FableWin.pdb has ??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z
  (CMatrix3x4::operator*(const C3DVector&) const -> C3DVector) — likely the original spelling `inv * pos`; no shared-header body exists for it.
- Tail: `mov ecx,[ecx+4]` (Mesh raw pointer, operator->, no AddRef/copy), push &local, push &local, push localRadius;
  `call 0x00A17690` = Mesh->HasBoundingBoxIntersectedFromTo(localRadius, local, local); returns its bool. `add esp,40h ; ret 8`.

## 2. Name / signature (two sources) — manifest label CORRECT
- Ego_r.pdb (egor_pdb_names.tsv 00b7fa90, Ego address space) and FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv 017bb15a / 031d25b0):
  `?IsSpherePartContainedInBoundingBox@CPhysicsMeshObject@@QBE_NABVC3DVector@@M@Z`
  = `bool CPhysicsMeshObject::IsSpherePartContainedInBoundingBox(const C3DVector&, float) const`.
- rebuild/manifest/functions.tsv: 00a11f20 thiscall bool (CPhysicsMeshObject*, C3DVector*, float). Not virtual.
- Caller: 0x00A12897 = landed CPhysicsMeshObject::IsPointContainedInBoundingBox (rebuild/src/compiled/00/a1/CPhysicsMeshObject_IsPointContainedInBoundingBox_00a12890.cpp:
  `return IsSpherePartContainedInBoundingBox(pos, 0.0f);`). Header param names there: (pos, radius).

## 3. Callees
- 0x00A17690 CPhysicsMesh::HasBoundingBoxIntersectedFromTo — landed rebuild/src/compiled/00/a1/CPhysicsMesh_HasBoundingBoxIntersectedFromTo_00a17690.cpp;
  PDB (Ego_r 00b75210 + FableWin 01849798/03198e30) `?HasBoundingBoxIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z`
  = bool (float, const C3DVector& from, const C3DVector& to) const, `ret 0Ch`. Called with from==to==local.
  OLD DRAFT WRONG: it invented `static IsSphereInLocalBoundingBox(const CPhysicsMeshObject*, ...)` — wrong name, wrong class, wrong arg shape.
- Shared header rebuild/include/engine/CPhysicsMesh.h does NOT declare HasBoundingBoxIntersectedFromTo (only HasBoundingSphereIntersectedFromTo) and has no opt-in for it.
  Precedent: CPhysicsMesh_GetSphereIntersectionFromTo_00a187c0.cpp declares it locally (line 33). Either add a declaration opt-in to CPhysicsMesh.h (coordinator) or include CPhysicsMesh.h and note the gap.
- Constant 0x0122DED8 = 1.0f (the `1.0f / det`).

## 4. Members (struct_layouts_egor.tsv CPhysicsMeshObject, size 64)
- +0x04 Mesh : CIVCountedPointer<const CPhysicsMesh>  -> header opt-in FABLE_RETAIL_CPHYSICSMESHOBJECT (+ FABLE_RETAIL_CIVCOUNTEDPOINTER); operator-> is unconditional in CIVCountedPointer.h.
- +0x08 PositionMatrix : CMatrix3x4 -> FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX.
- +0x3C OO_Scale : float (always declared). Scale +0x38 unused.
- Method declaration: FABLE_RETAIL_CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS (declares IsSpherePartContainedInBoundingBox(const C3DVector& pos, float radius) const and IsPointContainedInBoundingBox).
- CMatrix3x4 inline SetInverse: FABLE_RETAIL_CMATRIX3X4 (rebuild/include/engine/CMatrix3x4.h ~line 111). Also FABLE_RETAIL_CMATRIX3X4 gives TransformPoint(const&,&) whose Y/Z rows are the accumulated form.
- Old draft LOCAL classes that have shared headers: C3DVector (engine/C3DVector.h), CMatrix3x4 (engine/CMatrix3x4.h), CPhysicsMesh (engine/CPhysicsMesh.h),
  CIVCountedPointer (engine/CIVCountedPointer.h), CPhysicsMeshObject (engine/CPhysicsMeshObject.h). All must go.

## 5. Callee visibility
- HasBoundingBoxIntersectedFromTo is at a HIGHER address in another TU (CPhysicsMesh.cpp range 0x00A17600+; CPhysicsMeshObject range 0x00A114B0..0x00A12CC0) — plain out-of-line call, declaration only.
- SetInverse / operator*(C3DVector) / TransformPoint: FableWin-only inline members, no out-of-line copy in Ego_r or retail -> expand in place (FABLE_FORCEINLINE per owner 2026-11-01 helper rule, as the header does).
- No STLport involvement.

## 6. Sibling landed rows (rebuild/src/compiled/00/a1/)
- CPhysicsMeshObject_GetBoundingBoxCentre_00a11ae0.cpp: Flags none; defines FABLE_RETAIL_CPHYSICSMESHOBJECT_CENTRE, FABLE_RETAIL_CMATRIX3X4, FABLE_RETAIL_DEF_CLASS_BASE,
  FABLE_RETAIL_C3DVECTOR, FABLE_RETAIL_C3DBOUNDINGBOX, FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR, FABLE_RETAIL_CIVCOUNTEDPOINTER, FABLE_RETAIL_CPHYSICSMESHOBJECT,
  FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX, FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES; uses Mesh->... and a hand-written per-row transform (`m.E11*xx+(m.E21*yy+m.E31*zz)+m.E41`).
- CPhysicsMeshObject_GetBoundingBoxXEdge_00a11c20.cpp: Flags none; same macro set minus CENTRE/CMATRIX3X4.
- CPhysicsMeshObject_IsPointContainedInBoundingBox_00a12890.cpp: FABLE_RETAIL_CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS; includes C3DVector.h, CPhysicsMeshObject.h.
- CPhysicsMesh_GetSphereIntersectionFromTo_00a187c0.cpp: calls HasBoundingBoxIntersectedFromTo, local declaration.
- Other landed neighbours: GetCentreOfGravity 0x00A11EC0 (immediately before this row), SetPositionMatrix 0x00A114B0, ctor 0x00A12CC0.
- Parked same-family rows (inverse): 0x00A11D10 GetNavigationLevelAt, 0x00A128E0 IsInContainmentVolume, 0x00A12AB0 PeekRoughProportion... (hand267 NOTES.md lines 63-66).
- Landed matching SetInverse use: rebuild/src/compiled/00/b3/CEngineInternalAttachedPrimitive_GetChildInverseTransformStaticMesh_00b393d0.cpp (same body as header).

## 7. Old-draft model errors
1. Returned CMatrix3x4 temporary via GetInverse() (frame 0x70 v 0x40) -> use a local + SetInverse (in place).
2. Wrong callee: fabricated static IsSphereInLocalBoundingBox(this,...) -> Mesh->HasBoundingBoxIntersectedFromTo(localRadius, local, local) with ecx=[this+4].
3. Transform term order: draft E31*Z+E21*Y+E11*X+E41; retail E11*X, E31*Z, E21*Y, +E41 (accumulated form, all rows).
4. Cofactor/translation spellings: use header SetInverse verbatim (it matches the retail cofactor CSE and store order).
5. All types modelled locally despite shared headers; CIVCountedPointer copy ctor/dtor irrelevant (no copy here).
Suggested skeleton:
  float scaledRadius = radius * OO_Scale;   // first
  CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix);
  C3DVector local; (inverse.TransformPoint(pos, local) or inverse*pos with accumulated rows)
  return Mesh->HasBoundingBoxIntersectedFromTo(scaledRadius, local, local);


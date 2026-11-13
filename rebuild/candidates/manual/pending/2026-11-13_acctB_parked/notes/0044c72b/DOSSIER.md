# DOSSIER 0x0044C72B CGameDefinitionManager::InitAndCompile (8995 bytes) -- identity scout

No prior work existed for this row ($W/prior/ and /opt/fable_lane/w22/agent4022/{src,test,probe,hdr} are all empty), so this was scouted from scratch.
Helper outputs in $W: defclasses.tsv (all 138 registrations: string, Alloc address, class, PDB-Alloc check), addlist.txt (the 138 statements
ready to paste), allocs.tsv (raw Alloc disassembly facts), parse.py/allocs.py/final.py/strs.py (the scripts that produced them).

## 1. Retail shape  (python3 $S/w17/xdis.py 0044c72b 0x2323; $W/dis_all.txt)
- Extent: 0x0044C72B .. 0x0044EA4D inclusive (`ret` at 0x0044EA4D; next byte 0x0044EA4E is another COMDAT `mov eax,ecx ; ret`). 0x2323 = 8995: length confirmed.
- thiscall, no arguments, plain `ret` (void). `this` -> esi for the whole function (`mov esi,ecx` 0x0044C736).
- Frame pointer: `push ebp ; mov ebp,esp ; sub esp,2Ch ; push ebx ; push esi ; push edi` ... `pop edi ; pop esi ; pop ebx ; leave ; ret`.
  => `#pragma optimize("s", on)` + `#pragma optimize("y", off)`, exactly what the same-TU landed ctor 0x0044C6C2 and the sibling
  0x004336BC use. Unaligned entry (0x...72B) = size-optimised COMDAT TU.
- NO SEH prologue, no EH states, no /GS cookie although many destructible temporaries exist: TU built without /EHsc (same as sibling 0x004336BC, which has no `// Flags:` line). Do not add /EHsc.
- Single straight-line body: no conditional branches at all, no jump tables, no cold blocks.

## 2. Name / signature
- `?InitAndCompile@CGameDefinitionManager@@UAEXXZ` = `public: virtual void __thiscall CGameDefinitionManager::InitAndCompile(void)`.
  Sources: (a) ghidra_out/egor_pdb_names.tsv 00451fca (Ego_r.pdb) and FableWin ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv
  0x01913d9d (17396-byte debug body); (b) rebuild/manifest/rtti-vtables.tsv: CGameDefinitionManager vtable 0x01232C24 slot 2 = 0x0044C72B;
  (c) xref.py: only reference is the vtable (.rdata 0x01232C2C). Manifest label is RIGHT.
- Header rebuild/include/engine/CGameDefinitionManager.h already declares `virtual void InitAndCompile();`.

## 3. Callees (all landed)
| addr | landed file | landed name | PDB / real name | note |
|---|---|---|---|---|
| 0x0099B6B0 | 00/99/CWideString_CWideString_0099b6b0.cpp | CWideString(const wchar_t*) | ??0CWideString@@QAE@PBG@Z | `platform(L"pc\\")` |
| 0x0099B510 | 00/99/CWideString_Destructor_0099b510.cpp | ~CWideString | ok | |
| 0x009A76A0 | 00/9a/NBankFileManager_GetHeaderDir_009a76a0.cpp | NBankFileManager::GetHeaderDir | ?GetHeaderDir@NBankFileManager@@YI?AVCWideString@@XZ | __fastcall, hidden result in ecx |
| 0x0041A080 | 00/41/NResourceDirectoryNames_GetDefDir_0041a080.cpp | NResourceDirectoryNames::GetDefDir | ?GetDefDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ | __fastcall, hidden result |
| 0x0099BF30 | 00/99/global_operator_0099bf30.cpp | operator+(const CWideString&, const wchar_t*) | ??H@YI?AVCWideString@@ABV0@PBG@Z | __fastcall: ecx=result, edx=left, right pushed |
| 0x0099BE70 | 00/99/global_operator_0099be70.cpp | operator+(const CWideString&, const CWideString&) | ??H@YI?AVCWideString@@ABV0@0@Z | __fastcall |
| 0x004128A0 | 00/41/stdvectorCWideString..._pushback_004128a0.cpp | vector<CWideString>::push_back | ?push_back@?$vector@VCWideString@@...QAEXABVCWideString@@@Z | out of line (7 calls) |
| 0x00412130 | 00/41/CCombatWheel_ResetRings_00412130.cpp | **ResetRings (junk)** | ??1?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAE@XZ | NAME FIX: it is ~vector<CWideString> (manifest functions.tsv already says ~vector) |
| 0x00433DF0 | 00/43/stdvectorCWideString_operator_00433df0.cpp | vector<CWideString>::operator= | ??4?$vector@VCWideString@@...QAEAAV01@ABV01@@Z | out of line |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long) | ok | 144 calls, length arg -1 (edi) |
| 0x0099EC30 | 00/99/CCharString_CCharStringcopy_0099ec30.cpp | CCharString(const CCharString&) | ok | 138 (CDefClassInit::Name init) |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok | 282 |
| 0x009B0AC0 | 00/9b/CDefinitionManager_AddDefClass_009b0ac0.cpp | CDefinitionManager::AddDefClass | ?AddDefClass@CDefinitionManager@@QAEXABVCDefClassInit@@@Z | thiscall ret 4; 138 calls |
| 0x005DD670 | 00/5d/CAnimationEntry_StatRegisterClass_005dd670.cpp | CAnimationEntry::StatRegisterClass | ?StatRegisterClass@CAnimationEntry@@SIXXZ | static __fastcall no args (manifest class `_global` is stale) |
| 0x0067F9B0 | 00/67/CPhysicalPrimitiveInit_StatRegisterClass_0067f9b0.cpp | CPhysicalPrimitiveInit::StatRegisterClass | ?StatRegisterClass@CPhysicalPrimitiveInit@@SIXXZ | static __fastcall |
| 0x009E9F40 | 00/9e/NProgressDisplay_DisplayProgress_009e9f40.cpp | NProgressDisplay::DisplayProgress | ?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z (FableWin 0x02fb35a0) | __fastcall: ecx=&text, dl=bool3, stack float then bool4 |
| 0x009B08C0 | 00/9b/CDefinitionManager_Compile_009b08c0.cpp | CDefinitionManager::Compile | ?Compile@CDefinitionManager@@QAEXXZ | (egor_pdb_names lists 009b08c0 as CEngine ctor: that is an Ego_r address, NOT retail -- ignore) |
| 0x0043FEB0 | 00/43/CDefinitionManager_GetDef_0043feb0.cpp | GetDef<CGlobalDef> | ??$GetDef@VCGlobalDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCGlobalDef@@@@@Z | ok |
| 0x00430096 | 00/43/CDefinitionManager_GetDef00430096_00430096.cpp | GetDef<CEngineDef> | ??$GetDef@VCEngineDef@@@...@$$CBVCEngineDef@@@@@Z | ok |
| 0x00451354 | 00/45/CDefinitionManager_GetDef00451354_00451354.cpp | **GetDef_00451354** | ??$GetDef@VCEnvironmentDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCEnvironmentDef@@@@@Z | NAME FIX |
| 0x004513B9 | 00/45/CDefinitionManager_GetDef004513b9_004513b9.cpp | **GetDef_004513b9** (manifest sig says CEnvironmentDef) | ??$GetDef@VCHeroStatsDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCHeroStatsDef@@@@@Z | NAME FIX; FableWin orders GetDef<CEnvironmentDef> 0x0199e1ab then GetDef<CHeroStatsDef> 0x0199e24e, matching 0x451354 / 0x4513B9 and the +0xD8/+0xDC targets |
- 138 address-taken Alloc functions (`mov [ebp-10h], offset Alloc`): every one is `static CDefClassBase* __fastcall X::Alloc()`
  (`?Alloc@X@@SIPAVCDefClassBase@@XZ`); all 138 class names verified present as ?Alloc@... in egor_pdb_names.tsv / fablewin_pdb_names.tsv
  ($W/defclasses.tsv column 6 = yes for all). Class from the Alloc body: vtable store (RTTI) or the ctor it calls/jumps to. Six are landed
  (00/43 CControlsDef 0x432180, NUISystem::CUIMiscThingsDef 0x432C40, NUISystem::CUIDef 0x433670, CEngineDef 0x4343F0,
  CEngineVideoOptionsDef 0x434540, CConfigOptionsDefaultsDef 0x434660); the rest are unaligned same-TU COMDATs (inline statics emitted
  because their address is taken) and are not in functions.tsv. Only declarations are needed (relocation operand only).
  Odd ones: "ENGINE" -> CEngineDef; "UI_ICONS_DEF" 0x432E00 -> NUISystem::CUIIconsDef (vtable 0x1230F54 / Ego ?Alloc@CUIIconsDef@NUISystem@@);
  "THING" 0x44C0F0 -> CThingBaseDef; "CREATURE" -> CThingCreatureDef; "SWITCH" -> CThingSwitchDef; "HERO_STATS" -> CHeroStatsDef
  (string 0x1235E08 held in ebx, reused for the last GetDef); "VILLAGER_INTERACTION" -> CVillagerInteractionsDef; "SOUND_SETUP" -> CSoundDef;
  "CONTROL_SCHEME" -> CControlsDef; "UI"/"UI_MISC_THINGS_DEF"/"UI_LOCALE_GRAPHICS_DEF"/"UI_ICONS_DEF" are NUISystem:: classes.

## 4. Members touched
- this+0x40 SymbolPathList, this+0x4C CompilePathList: Ego_r `vector<CWideString>` (struct_layouts_egor.tsv CDefinitionManager; Ego offsets
  are +4 per 16-byte debug vector, retail 12-byte STLport). Shared header rebuild/include/engine/CDefinitionManager.h has them only as
  `unsigned char SymbolPathList[0xc]` (static-asserted +0x40) -- NO typed opt-in. Landed Compile 0x009B08C0 reads them via a cast accessor;
  sibling 0x004336BC models them in a local class as std::vector<CWideString>. Author needs either a cast (`*(std::vector<CWideString>*)SymbolPathList = headers;`)
  or a new overlay opt-in in $W/hdr (e.g. FABLE_RETAIL_CDEFINITIONMANAGER_PATHLISTS typing both as the vector) -- bytes are identical (call 0x433DF0 with ecx=esi+0x40 / esi+0x4C).
- this+0xD0/+0xD4/+0xD8/+0xDC: four `CDefPointer<const T>` (T = CGlobalDef, CEngineDef, CEnvironmentDef, CHeroStatsDef), only their
  addresses are passed (`lea eax,[esi+0D0h] ; push eax`). Neither PDB has the members (forward record only) -> descriptive names.
  Shared header CGameDefinitionManager.h: FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBAL gives `CDefPointer<const CGlobalDef> PGlobalDef` +0xd0
  only; _GLOBALDEF / _HERODSTATSDEF give raw `const T*` (wrong type for GetDef deduction). No opt-in has all four CDefPointers ->
  add an overlay opt-in (e.g. FABLE_RETAIL_CGAMEDEFINITIONMANAGER_DEFPOINTERS: PGlobalDef, PEngineDef, PEnvironmentDef, PHeroStatsDef,
  same names the dtor row 0x00450BFF describes as "four cached def pointers"). The landed ctor 0x0044C6C2 models the class LOCALLY with
  `CUnprovenField_0044c6c2_d0..dc` (placeholder names, against the 2026-11-10 rule) -- do not copy those names.
- Opt-ins already present: FABLE_RETAIL_CDEFINITIONMANAGER (+ needs FABLE_RETAIL_CRESOURCELIST, FABLE_RETAIL_DEF_CLASS_BASE),
  FABLE_RETAIL_CDEFINITIONMANAGER_ADDDEFCLASS (declares AddDefClass), CDefClassInit.h (inline ctor `CDefClassInit(const CCharString&, TAllocFunc) : Name(name), AllocFunc(allocFunc) {}`),
  GetDef(const CCharString&, long, CDefPointer<const T>*) const template declared (line 52, declaration-only unless _GETDEF_INDEX_BODY is defined -- keep it OFF: retail calls all four out of line).
- Locally modelled classes in the sibling 0x004336BC that have shared headers: CWideString / CCharString (fable_string.h), CDefinitionManager (engine/CDefinitionManager.h), SDefinitionType (= CDefClassInit, engine/CDefClassInit.h).

## 5. Callee-body visibility
- CDefClassInit ctor: inline (header body); retail expands it at ALL 138 sites (copy-ctor call + `mov [ebp-10h],Alloc`); its out-of-line
  copy 0x0042DAE0 is never called here (RegisterTCs 0x004EE23F calls it after 8 expansions -- if the author's build outlines some sites, the
  inline budget differs; keep the expression form identical to retail: one CDefClassInit temporary per statement).
- CCharString / CWideString ctors, dtors, operator+: out of line (no inline bodies; do not enable any *_FORCEINLINE_* string opt-in).
- vector<CWideString>: default ctor INLINE (three zero stores, `xor eax,eax` ...), push_back / operator= / ~vector OUT OF LINE (0x4128A0 /
  0x433DF0 / 0x412130). Declare them without bodies exactly as sibling 0x004336BC does (its local std::vector template is the proven idiom).
- GetDef<T>: out of line (declaration only). AddDefClass, Compile, DisplayProgress, StatRegisterClass: other TUs, declaration only.
- The Alloc statics are only address-taken: declarations suffice.

## 6. Sibling rows / idioms
- **0x004336BC CStartupDefinitionManager::InitAndCompilePlatformSpecific** (00/43/..._004336bc.cpp): the same function shape (path vectors,
  `platform(L"pc\\")`, push_back of GetHeaderDir()/GetDefDir() concatenations, AddDefClass-of-temporary x5, two vector assignments, Compile()).
  `#pragma optimize("s", on)` + `#pragma optimize("y", off)`, no Flags line. Copy its declarations, but respell: AddDefinitionType -> AddDefClass,
  SDefinitionType -> CDefClassInit (Name/AllocFunc), Create*Def -> X::Alloc (`__fastcall`, returns CDefClassBase*) -- its spellings have no PDB source.
- 0x0044C6C2 CGameDefinitionManager ctor (same TU, just before): `#pragma optimize("s",on)` / `#pragma optimize("y",off)`, rebuild_abi.h, CDefPointer.h with FABLE_RETAIL_DEF_CLASS_BASE.
- 0x00450BFF ~CGameDefinitionManager (file named ..._CGameDefinitionManager_00450bff.cpp but Leaf ??1): `#pragma optimize("s", on)`.
- 0x009B0AC0 AddDefClass and 0x009B08C0 Compile (Flags /O2 /Oy /W3) for the callee prototypes.
- Same-TU neighbours 0x0044C0C0..0x0044C6AB (CXxxDef ctors / GetSizeofClass) all `#pragma optimize("s",on)`.

## Name fixes found (also in $W/NAME_FIXES.tsv)
- 0x00412130: ResetRings / CCombatWheel -> std::vector<CWideString>::~vector.
- 0x00451354: GetDef_00451354 -> CDefinitionManager::GetDef<CEnvironmentDef>; 0x004513B9: GetDef_004513b9 -> GetDef<CHeroStatsDef>.
- 0x004336BC sibling body: AddDefinitionType / SDefinitionType / Create*Def -> AddDefClass / CDefClassInit / X::Alloc.
- 0x0044C6C2: CUnprovenField_0044c6c2_d0..dc placeholders -> descriptive PGlobalDef / PEngineDef / PEnvironmentDef / PHeroStatsDef.

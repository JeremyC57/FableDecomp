# DOSSIER 0x0073B130 -- CTCCreatureModeManager::RegisterModes (6673 bytes)
(no prior scout existed: $W/prior/ was empty at scout time; nothing to reuse)

## 1. Retail shape (python3 $S/w17/xdis.py 0073b130 0x1a21, $W/dis_all.txt)
- Prologue `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,0Ch ; push ebx` -- the aligned-frame form VC7.1 emits by itself
  (the landed sibling-in-shape 0x005DD670 CAnimationEntry::StatRegisterClass has the same `and esp,0FFFFFFF8h` with NO pragma).
- Epilogue `pop ebx ; mov esp,ebp ; pop ebp ; ret` (0x0073CB3C..0x0073CB40): static __fastcall, no args, plain `ret`.
- No SEH frame, no /GS cookie, no EH states (nothing with a destructor lives in the frame).
- 16-aligned entry, `mov ecx,offset map` reloaded before every call (no `mov esi,offset`): NOT optimize("s") -- unlike
  CTCBase::RegisterTCs (0x004EE23F, optimize("s")). The class-neighbour rows (0x0073A8E0, 0x0073AE10, 0x0073AFC0, 0x0073B0D0,
  0x0073B120) carry no `#pragma optimize` / `// Flags:` either.
- ebx = 1 the whole function (the Dirty `true`, and the push 1 / push true args of _M_insert_overflow).

## 2. Identity
- Name: CTCCreatureModeManager::RegisterModes, leaf `?RegisterModes@CTCCreatureModeManager@@SIXXZ` = public static
  `void __fastcall RegisterModes()`. Sources: Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00751CF0), FableWin.pdb
  (fablewin_pdb_names / FableWin.exe.signatures.tsv 0x024B44C0, /Od body 3704 bytes), manifest label agrees;
  the only caller is CTCBase::RegisterTCs (call at 0x004F9129, landed rebuild/src/compiled/00/4e/CTCBase_RegisterTCs_004ee23f.cpp,
  which declares it locally `static void __fastcall RegisterModes();`). egor_signatures' "Init_RegisterGameComponents" is wrong/descriptive.
- The map: `CTCCreatureModeManager::RegisteredModes` (Ego_r + FableWin "dynamic initializer for 'CTCCreatureModeManager::RegisteredModes'"),
  retail object at 0x013BAD44 (vector start +0 0x13BAD44, finish +4 0x13BAD48, end_of_storage +8 0x13BAD4C, Compare +0xC 0x13BAD50,
  Dirty +0xD 0x13BAD51). Type (Ego_r TPI, struct_layouts_egor.tsv): CVectorMap<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo,
  CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> >. Landed AllocateMode 0x0073A8E0 uses the same object.
- CRegisteredModeInfo (4 bytes): member `AllocFunc`; FableWin.pdb ctor ??0CRegisteredModeInfo@CTCCreatureModeManager@@QAE@P6IPAVCModeBase@1@AAVCThing@@@Z@Z
  => `CRegisteredModeInfo(CModeBase* (__fastcall* allocFunc)(CThing&))`, so AllocFunc's type is `CModeBase* (__fastcall*)(CThing&)`.
  FLAG: struct_layouts_egor.tsv renders it `_func___cdecl_CModeBase_ptr_CThing_ptr` and the landed 0x00743270 / 0x00743B30 local models
  spell `CModeBase* (__cdecl*)(CThing*)` -- the PDB mangling (P6I = __fastcall, AAVCThing = CThing&) wins; Alloc names confirm:
  ?Alloc@CModeCook@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z (static __fastcall, CThing&).
- FableWin.pdb (unoptimised) has, out of line: CVectorMap::Insert(const unsigned long&, const CRegisteredModeInfo&) (?Insert@...QAEXABKABV...@Z, 47 i),
  pair(const unsigned long&, const CRegisteredModeInfo&) (32 i), CRegisteredModeInfo ctor (24 i), Update (30 i), Sort (124 i);
  NO make_pair for this pair => the source is `RegisteredModes.Insert(MODE_X, CRegisteredModeInfo(&NCreatureMode::CModeX::Alloc))`
  with Insert building `value_type(key,value)` (header opt-in FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE), then `RegisteredModes.Update()`.
- Retail also holds an out-of-line Insert COMDAT 0x00743E60 (landed, CVectorMap_Insert_00743e60.cpp) that NOTHING references; RegisterModes
  expands all 85 Inserts (no call to 0x743E60).

## 3. Callees (all out of line)
| retail | landed row | landed name | PDB name |
|---|---|---|---|
| 0x00743270 | 00/74/stdvector_Minsertoverflow_00743270.cpp | vector<pair<..>>::_M_insert_overflow (5 args, ret 14h) | STLport _M_insert_overflow(pos,x,const __false_type&,fill_len,atend) (Ego_r has Dinkum _Insert_n; retail is STLport) |
| 0x00743B30 | 00/74/stdvector_pushback_00743b30.cpp | vector<pair<..>>::push_back | Ego_r/FableWin name it ?push_back@?$CArray@U?$pair@K..@@QAEXABU..@Z (CArray::push_back) -- landed spells std::vector::push_back; CArray::push_back just forwards and is inlined, so retail's COMDAT is the STLport vector::push_back. OK |
| 0x007441D0 | 00/74/global_SortSElement8007441d0_007441d0.cpp | "SGI sort<SElement8*,Compare>" (generated name) | std::sort<pair<unsigned long,CRegisteredModeInfo>*, CKeyPairCompareLess<..> > __fastcall (Ego_r ??$_Sort@... family). FLAG: landed name is generic; real is std::sort instantiation |
| 85 allocators | see OUTLINE table | (by address only, `mov ecx,offset` / immediate) | NCreatureMode::CModeX::Alloc (FableWin: 85 ?Alloc@CMode*@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z -- exactly the 85 registered) |
- Alloc names were resolved from each Alloc's constructor call (manifest/landed ctor rows) and, for the three that call the plain
  CModeBase ctor 0x0073EAC0, from the vtable stored after it (rtti-vtables.tsv): 0x00742430 -> CModeNull (vt 0x01267588),
  0x00742460 -> CModeBasic (vt 0x012675BC; manifest label "CModeBasic::Clone" is WRONG, it is CModeBasic::Alloc),
  0x007427B0 -> CModeBlock (vt 0x01267C3C); 0x00744160 -> CModeWalkInAllDirections (vt 0x012687AC; its ctor callee 0x008EAC40 is the
  CModeHoverBase base ctor). 0x00742730 (key MODE_SNEAK) builds CModeCrouch. Only 21 of the 85 Allocs are landed rows (none needed:
  they are referenced as addresses only).

## 4. Members / globals touched
- Only the static map 0x013BAD44 (start/finish/eos/Compare/Dirty as above) -- no `this`.
- Header: rebuild/include/engine/CTCCreatureModeManager.h declares NEITHER RegisterModes, CRegisteredModeInfo NOR RegisteredModes:
  the author must add an opt-in (e.g. FABLE_RETAIL_CTCCREATUREMODEMANAGER_REGISTRY, overlay in $W/hdr = full copy of the current header +
  #ifdef additions) with: `class CModeBase;` already nested; `class CRegisteredModeInfo { public: CRegisteredModeInfo(CModeBase* (__fastcall* allocFunc)(CThing&)) : AllocFunc(allocFunc) {} CModeBase* (__fastcall* AllocFunc)(CThing&); };`
  `static void __fastcall RegisterModes();` `static CVectorMap<unsigned long, CRegisteredModeInfo> RegisteredModes;` (declaration only --
  defining it in the row would emit a dynamic initializer). Include engine/CVectorMap.h under that opt-in (CTCBase.h already does under
  FABLE_RETAIL_CTCBASE_REGISTRY).
- CVectorMap.h opt-ins available: FABLE_RETAIL_CVECTORMAP_INSERT (Insert(value_type)/Update with sort inline),
  FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE (Insert(const K&,const V&), __forceinline under _MSC_VER), FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
  (value_type(key,value) temp, no make_pair -- matches FableWin), FABLE_RETAIL_CVECTORMAP_SORT (Sort()/Update()), FABLE_RETAIL_CVECTORMAP_CTOR.
  fable_containers.h: FABLE_RETAIL_STLP_SORT_FASTCALL (std::sort declared __fastcall, decl only -> call 0x007441D0).
  Do NOT set FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE / _NOINLINE: push_back must stay a plain inline (expanded at the first 70, called after).
- Mode classes: NCreatureMode::* -- only 6 have shared headers (NCreatureMode_CModeBriarRoseConcentrate.h, _CModeDrained.h,
  _CModeGuildmasterPowerUpShield.h, _CModeScorpionKingVulnerable.h, _CModeSitFloor.h, _CModeThunderLightningStorm.h), none declares Alloc.
  Keys: NCreatureMode::EMode in engine/NCreatureMode.h (MODE_* values 0..95, verified against every immediate).
  Precedent for declaring 451 classes by registration interface only: RegisterTCs row (TC_REGISTRATION_INTERFACE macro). Same approach is
  acceptable here (NOTES: header debt), or add `static CModeBase* __fastcall Alloc(CThing&);` opt-ins to the 6 headers.

## 5. Visibility / inline budget
- Insert expanded at all 85 sites (Dirty=true stores sunk to the next block, `mov byte ptr [13BAD51h],bl`).
- push_back (plain STLport inline in fable_containers.h) expanded at sites 1..70 and CALLED (0x00743B30) at 71..85: VC7.1's caller budget,
  same phenomenon reproduced flag-free in 0x005DD670 (17/26) and RegisterTCs (41/41/8).
- _M_insert_overflow is the STLport out-of-line member (vector.c) -> call 0x00743270 with (pos=finish, &x, &__false_type tag, 1, true):
  args pushed `push ebx(1=atend) ; push ebx(1=fill_len) ; lea ecx,[tag] ; push ecx ; lea edx,[pair] ; push edx ; push eax(finish)`.
  The tag is __false_type (pair is not known POD) -- as landed push_back 0x00743B30 notes.
- Placement new null test (`test eax,eax ; je`) inside the expanded push_back = STLport _Construct with operator new(size_t, void*).
- Update(): Dirty known true after the last Insert, so `if(Dirty)` folds; std::sort(begin,end,Compare) __fastcall (ecx=start, edx=finish,
  Compare byte zero-extended and pushed), then Dirty=false. Sort is expanded (no Sort COMDAT call).

## 6. Siblings / models
- 0x005DD670 CAnimationEntry::StatRegisterClass (rebuild/src/compiled/00/5d/CAnimationEntry_StatRegisterClass_005dd670.cpp): BEST shape model:
  no pragma, defines FABLE_RETAIL_CVECTORMAP_INSERT, _INSERT_KEYVALUE, FABLE_RETAIL_STLP_SORT_FASTCALL; same aligned frame; push_back split by budget.
- 0x004EE23F CTCBase::RegisterTCs: _INSERT_KEYVALUE + _INSERT_VALUE_TYPE + _SORT + STLP_SORT_FASTCALL (but optimize("s") TU -- do not copy the pragma).
- 0x00743E60 CVectorMap<..CRegisteredModeInfo>::Insert (landed, local model) shows the Insert text `push_back(pair<K,V>(key,value)); Dirty=true;`.
- Class neighbours 0x0073A8E0 AllocateMode, 0x0073AFC0 SetupInitialMode (#pragma auto_inline(off/on) around a helper, includes engine/CVectorMap.h),
  0x0073B0D0 ResetModes (FABLE_RETAIL_CTCBASE), 0x0073B110 OnCreate, 0x0073B120 InitialActivate: no optimize pragma, no Flags.

## Scheduling note (for diffs)
- Block 82 (0x0073CA82, MODE_SUMMONER_DIE): the Dirty store comes BEFORE the pair stores and the stores use [esp+8]/[esp+0Ch]
  before `lea edx,[esp+8] ; push edx` (other call blocks push first, then store at [esp+0Ch]/[esp+10h]). Pure c2 scheduling; expect it
  for free if the statement list is right.

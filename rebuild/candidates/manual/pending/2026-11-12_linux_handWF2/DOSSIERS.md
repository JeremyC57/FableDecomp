## 0x00868CE0

# DOSSIER 0x00868CE0 -- CHitParameters::SetWeapon(CThing&)

## 1. Retail shape (xdis 00868ce0 0x178; body ends 0x00868E45 `ret 4`, 0x168 = 360 bytes, then int3 pad)
- thiscall, `ret 4`, NO frame pointer (`sub esp,18h ; push esi`): do NOT use optimize("y",off). No SEH/EH frame.
- Speed-optimised, not size: loop head aligned with `mov edi,edi` filler (0x00868D8E). No optimize("s").
- Frame 0x18 (draft got 0x1C). Slot map (local L = [esp+4+L] right after `push esi`):
  - L0..7: the CIntelligentPointer<CThing> temporary (vptr 0x01238C6C at L0, PData L4). After it dies, L0 is reused for
    the PeekTC key (0xBF, stored AFTER the bit test, 0x00868D42), the CDefPointer<const CThingBaseDef> out word, and the
    CDefPointer<const CAugmentationDef> out word (all `[esp+10h]` inside the loop).
  - L8: `this` spilled (`mov [esp+0Ch],esi` at 0x00868CEA, re-read `mov ecx,[esp+18h]` at 0x00868DCC for Augmentations).
  - L0xC..0x17: local std::vector<long> (zeroed with the ebp zero register AFTER LowerBound, 0x00868D6C..74).
  - push_back's __false_type tag temp is addressed at the dead `weapon` arg slot (`lea ebp,[esp+34h]` = E+4, 0x00868DF0).
- `weapon` is re-read from its arg slot every iteration (`mov ecx,[esp+30h]`, 0x00868D97) for PeekDefinitionManager.
- Flow: temp ptr ctor(0xA01B10) ; vptr ; SetPItem(&weapon) ; GetPItem(temp) ; SetPItem(this+0x58) ; ~temp(0xA01C10) ;
  GetPItem(this+0x58) ; test sign of [thing+0x34] (bit 0xBF of UsingComponents) jns -> exit ;
  key=0xBF ; LowerBound(&key) on map=thing+0x44 via pointer local (`lea esi,[eax+44h]` ... `mov esi,[esi+4]` = end) ;
  end-or-greater ternary, `mov ecx,[eax+4]` = TC (no null test) ; vector ctor ; GetAugmentations(vec) ;
  for (it=begin; it!=end; ++it) { def=0 (after pushes) ; if (weapon.PeekDefinitionManager().GetDef(*it,&def)) { edi=def ;
  sub=0 ; if (def->GetSubDef(&sub)) { esi=sub ; inline push_back(&sub->Type) [edx = esi+0x2C, i.e. by reference] } ;
  release sub (null-tested) } ; release def (null-tested, via edi) } ; vector dtor = free(_M_start) if non-null.
  Note: GetSubDef is called even though `def` is not null-tested (operator-> on CDefPointer); release is null-tested.
  Loop is `do-while` rotated with an entry test `begin==end` (0x00868D81..87) -- normal VC7.1 for-loop over iterators.

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv / egor_signatures.tsv, Ego_r addr 0x0083EC60): `?SetWeapon@CHitParameters@@QAEXAAVCThing@@@Z`
  = `void CHitParameters::SetWeapon(CThing& weapon)` (public, non-const, non-virtual, ref param).
- Ego_r order ClearAllDamage 0x0083EC50 -> SetWeapon 0x0083EC60 -> AddDamageWithAttribute 0x0083EDD0 matches retail
  0x00868A40 ClearAllDamage ... 0x00868CA0 AddDamageWithAttribute -> 0x00868CE0 (same TU, same order).
- Landed caller rebuild/src/compiled/00/83/CThingGameObject_GetShotHitParameters_00838ec0.cpp:115 declares
  `void SetWeapon(CThing& weapon); // 0x00868CE0`. Manifest label is RIGHT. Mangled leaf above.
- Spelling disagreement: draft and landed caller CThingGameObject_GetMeleeHitParameters_00838bc0.cpp:210 use `CThing*` (wrong
  per PDB: AAVCThing = reference). Hand back a NAME_FIXES note for 0x00838BC0's declaration.
- Callers (xref): 0x007C8426, 0x0081181C, 0x00838BED (GetMeleeHitParameters), 0x00838FB2 (GetShotHitParameters).

## 3. Callees (landed file / landed name / PDB name; draft spelling flagged)
- 0x00A01B10 CBaseIntelligentPointer::CBaseIntelligentPointer() -- 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01b10.cpp, ??0CBaseIntelligentPointer@@QAE@XZ. OK.
- 0x00A01B90 SetPItem(CBaseObject*) -- 00/a0/..._SetPItem_00a01b90.cpp. Draft param `CThing*` (PDB PAVCBaseObject) -- use shared header.
- 0x00A01B50 GetPItem() const -- returns CBaseObject* (draft: CThing*). Use header's Get()/GetPointer().
- 0x00A01C10 ~CBaseIntelligentPointer() VIRTUAL (??1..@@UAE@XZ); draft declares non-virtual.
- vtable 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv, <CThing>).
- 0x004365B0 CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) const --
  00/43/CVectorMap_LowerBound_004365b0.cpp. Draft: local CTCMap::LowerBound(const unsigned long&) with a same-TU body -- WRONG model; use engine/CVectorMap.h via CThing PeekTC.
- 0x00766920 CTCObjectAugmentations::GetAugmentations(std::vector<long>&) const -- 00/76/CTCObjectAugmentations_GetAugmentations_00766920.cpp,
  ?GetAugmentations@CTCObjectAugmentations@@QBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z. Draft: CTCObjectAugmentations (ok) non-const, CRetailVector<long>.
- 0x004C7990 CThing::PeekDefinitionManager() const -> const CGameDefinitionManager& -- 00/4c/CThing_PeekDefinitionManager_004c7990.cpp,
  ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ. Draft says "GetDefinitionManager" -- WRONG NAME.
- 0x0042AF3C CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const CThingBaseDef>*) const -- 00/42/CDefinitionManager_GetDefT_0042af3c.cpp,
  FableWin ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z. Draft: non-template, CObjectAugmentationDef in comment -- WRONG.
- 0x0042B248 CDefClassBase::GetSubDef<CAugmentationDef>(CDefPointer<const CAugmentationDef>*) const -- 00/42/CDefClassBase_GetSubDefCAugmentationDef_0042b248.cpp,
  ??$GetSubDef@VCAugmentationDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCAugmentationDef@@@@@Z. Draft: GetSubDef<CAbilityDef> -- WRONG.
- 0x00869AC0 std::vector<EObjectAugmentationType>::_M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool) --
  00/86/stdvector_Minsertoverflow_00869ac0.cpp. Draft uses __true_type -- WRONG tag type (STLport treats the enum as non-POD).
- 0x00BFEA14 free (vector dtor / deallocate).  Def release: virtual slot 1 with refcount at +4 (CDefPointer dtor).

## 4. Members
- this+0x3C CHitParameters::Augmentations: Ego_r `vector<EObjectAugmentationType>` at PDB +0x40 (struct_layouts_egor.tsv; retail -4 because
  DamageMap is a 12-byte STLport map). engine/CHitParameters.h spells it `unsigned char Augmentations[12]` -- NO opt-in gives the real vector
  type. The author must add one (e.g. FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS -> `std::vector<EObjectAugmentationType> Augmentations;`
  with stlport/fable_containers.h + engine/EObjectAugmentationType.h) and a SetWeapon(CThing&) declaration (none exists in the header;
  the members are private, so the method must be declared in the class). FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP gives the real map at +0x30.
- this+0x58 CHitParameters::PWeapon: CIntelligentPointer<CThing> (PDB +0x60). Header already has it (`PThingHitting,PWeapon`). Needs
  FABLE_RETAIL_CINTELLIGENTPOINTER (packed retail class) and FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (operator= = GetPItem into local then SetPItem).
- CThing+0x20 UsingComponents bitset<274> (bit 0xBF -> dword +0x34 sign bit), +0x44 Components CVectorMap (PDB +0x48; CThing.h asserts 0x44).
  Opt-ins: FABLE_RETAIL_CTHING, FABLE_RETAIL_CTHING_GETTC_EXPANDED (PeekTC<T>(T**) with key-copied-inside-test + map pointer local +
  ternary end-or-greater = exactly retail 0x00868D2D..60), maybe FABLE_RETAIL_CTHING_MOREACCESS (PeekDefinitionManager return type), FABLE_RETAIL_CTHING_HASTC_FORCEINLINE.
- CTCObjectAugmentations (engine/CTCObjectAugmentations.h, non-AUGMENTATION_PARTICLES branch): StatGetInterfaceType()=TCI_OBJECT_AUGMENTATIONS(191=0xBF).
  GetAugmentations is NOT declared in that branch (only in the FABLE_RETAIL_AUGMENTATION_PARTICLES full branch) -- author must add a declaration opt-in.
- CAugmentationDef::Type EObjectAugmentationType at +0x2C (Ego_r struct_layouts: Type @44). engine/CAugmentationDef.h, opt-in FABLE_RETAIL_CAUGMENTATIONDEF.
  Draft calls it CAbilityDef::AugmentationType -- WRONG.
- Locally modelled in the draft but with shared headers: CHitParameters, CThing, CBaseIntelligentPointer/CIntelligentPointer (CThingPointer),
  CDefinitionManager, CDefClassBase, CDefBase/CDefPointer (DefOut/DefHolder), CTCObjectAugmentations, CAugmentationDef (as CAbilityDef),
  CVectorMap (CTCMap/STCEntry), std::vector (CRetailVector), __true_type.

## 5. Callee-body visibility
- GetDef<CThingBaseDef> 0x0042AF3C and GetSubDef<CAugmentationDef> 0x0042B248 are out-of-line COMDATs whose bodies retail's compiler saw:
  the def out word is reused across both lookups (slot L0) and the def is held in edi -> needs the visible noinline bodies:
  FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (engine/CDefinitionManager.h:83/102,
  `__declspec(noinline)` template bodies). The draft's hand-made same-TU stand-ins did this (388->361 bytes); use the shared macros.
- CVectorMap::LowerBound const 0x004365B0: out of line (shared CVectorMap.h declares it); CThing GETTC_EXPANDED reproduces the shape.
- push_back: INLINE in retail (no call); shared fable_containers.h push_back default (no PUSHBACK_OUTOFLINE/NOINLINE) with _IsPODType() -> __false_type.
- _M_insert_overflow 0x00869AC0: out-of-line COMDAT 2 functions after (landed). Leave declared-only (default).
- CIntelligentPointer(T*) ctor and operator=: INLINE (only base ctor/dtor/Get/SetPItem are calls) -- do NOT use _CTOR_OUTOFLINE/_ASSIGN_OUTOFLINE.
- PeekDefinitionManager 0x004C7990: out of line (call each iteration).
- GetAugmentations 0x00766920: out of line, different TU.

## 6. Siblings to copy
- 00/76/CTCObjectAugmentations_GetDamageMultiplierFromAugmentationSet_007661c0.cpp: the same GetDef<CThingBaseDef>/GetSubDef<CAugmentationDef>
  loop on shared headers. Macros: FABLE_RETAIL_BASE_OBJECT, _CTCBASE, _DEF_CLASS_BASE, _CCHARSTRING, _CDEFSTRINGTABLE, _STLP_LIST,
  _CDEFINITIONMANAGER, _CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, _CDEFCLASSBASE_GETSUBDEF_BODY, _CGAMEDEFINITIONMANAGER, _CRESOURCELIST,
  _CRESOURCE, _CAUGMENTATIONDEF; includes fable_string.h, engine/CThingBaseDef.h, CAugmentationDef.h, CGameDefinitionManager.h,
  CTCObjectAugmentations.h. Body: `CDefPointer<const CThingBaseDef> def; mgr.GetDef(idx,&def); CDefPointer<const CAugmentationDef> a; def->GetSubDef(&a);`
- 00/86/global_GetArmourWeightsFromThing_00868e50.cpp (next function, same TU): Flags /Gr (file-static fastcall only), macros
  FABLE_RETAIL_DEF_CLASS_BASE, _CCHARSTRING, _CTHING_MOREACCESS, _CTHING, _CTHING_GETTC_EXPANDED, _CTCBASE, _CDEFINITIONMANAGER,
  _CGAMEDEFINITIONMANAGER, _CRESOURCELIST, ...; uses `thing.PeekTC(TCI_..., &ptc)` and `def_man.GetDef(idx,&pdef)` same-TU template bodies.
  NB its /Gr flag is for its own fastcall; SetWeapon is thiscall, /Gr harmless for members.
- 00/94/CAIStateGroupSitForMeal_Update_00945bd0.cpp: `PCookingPot = pot;` with FABLE_RETAIL_CINTELLIGENTPOINTER + _CINTELLIGENTPOINTER_ASSIGN.
- 00/86/unattributed_AssignTarget008685f0_008685f0.cpp = really CHitParameters::SetThingHitting(CThing&) (same TU, identical opening shape,
  member +0x50 = PThingHitting) -- landed on local stand-ins; its label is wrong (name fix candidate: ?SetThingHitting@CHitParameters@@QAEXAAVCThing@@@Z, also
  CHitParameters.h line ~93 under FABLE_RETAIL_CHITPARAMETERS_CTOR).
- 00/5b/NInventoryCItem_UpdateItemStats_005bedcb.cpp: other GetAugmentations caller (local models; size TU).

## 7. Draft model errors
- Signature CThing* -> CThing& (PDB). `weapon->` -> `weapon.`.
- `PWeapon = CThingPointer(weapon)` in a block: write `PWeapon = &weapon;` (CIntelligentPointer<CThing>(T*) temp + inline operator= via _ASSIGN).
- HasTC/GetTC hand-split -> one `const CTCObjectAugmentations* ptc; if (PWeapon->PeekTC(&ptc)) {` (GETTC_EXPANDED; the key store after the test is what
  frees L0 order). Probably via `PWeapon.Get()` / operator-> (GetPItem on this+0x58, no null test).
- vector type: local is std::vector<long> (GetAugmentations param), declared after PeekTC (zeroing after LowerBound).
- Defs: CDefPointer<const CThingBaseDef> pdef; `if (weapon.PeekDefinitionManager().GetDef(*it,&pdef))` { CDefPointer<const CAugmentationDef> paug;
  `if (pdef->GetSubDef(&paug)) Augmentations.push_back(paug->Type);` } -- draft's DefOut/DefHolder split-scope trick is unnecessary with visible noinline bodies;
  it was the cause of the extra frame word. Both GetDef and GetSubDef results ARE tested (`test al,al; je`).
- Type names: CAbilityDef/AugmentationType -> CAugmentationDef::Type; __true_type -> __false_type; GetDefinitionManager -> PeekDefinitionManager;
  LowerBound(const unsigned long&) -> CVectorMap LowerBound(const ETCInterfaceType&) const.
- Draft `// Retail: ... length 358` -- actual 360 bytes (0x00868CE0..0x00868E47).

## 0x00E2EAC0

# DOSSIER 0x00E2EAC0 -- NScript::CV_AmbushScamScript::TricksterApproach

## 1. Retail shape (xdis 00e2eac0 0x17b; real length 0x16B = 363 bytes, int3 pad from 0x00E2EC2B)
- thiscall (`mov esi,ecx`), plain `ret` (x3 epilogues), NO frame pointer, NO SEH prologue. `sub esp,10h; push esi; push edi`.
- edi = 0 zero register for the whole body. Frame: [esp+8 at entry-of-body] dead zero dword (stored `mov [esp+0Ch],edi` right
  after `push eax`, never read); trickster CScriptThing at esp+0Ch (vptr), +10h PImp.Data, +14h PImp.Info.
- Flow:
  trickster = GetTrickster() (hidden-ret ptr pushed, 0x00E2EC30)
  if IsActiveThreadTerminating() -> EXIT_A
  LOOP (0xE2EAE2): if (!PImp.Data || !PImp.Data->vslot75(+0x12C)) -> DEAD   [inline CScriptThing::IsAlive]
        if (Game->GetHealth(trickster) /*slot264 +0x420*/ <= 1e-4f [0x0129BA3C], `test ah,41h; jnp`) -> DEAD
        if (IsDistanceBetweenThingsUnder(trickster, Game->GetHero() /*slot70 +0x118*/, TricksterApproachProximity +0x5C)) -> NEAR
        Game->NewScriptFrame() (slot7 +0x1C); if (!IsActiveThreadTerminating()) goto LOOP   (rotated loop)
  EXIT_A (0xE2EB50): inline ~CScriptThing copy #1 (vptr=0x01238C8C, counted release, op delete 0xBFE9BC, zero Data/Info, ~CBaseClass 0x99A2E0); ret
  NEAR (0xE2EB94): if IsActiveThreadTerminating() { call ~CScriptThing 0x004AA840 OUT OF LINE; ret }  DoTricksterIntro(+0x4E)=1; goto TAIL
  DEAD (0xE2EBB4): if IsActiveThreadTerminating() { inline dtor copy #2 (tail-merged into #1 at 0xE2EB75/7D); }  TricksterKilledBeforeAmbush(+0x57)=1
  TAIL (0xE2EBEA): if (!IsActiveThreadTerminating()) goto EXIT_A; else inline dtor copy #3 (merged into #1); 
- Not size-optimised (no `optimize("s")` idioms), no /Oy- . Same TU as Main 0x00E2B090 (landed, no pragmas, no // Flags line).

## 2. Identity (proven, 3 sources)
- Ego_r.pdb: 00e7c340 ?TricksterApproach@CV_AmbushScamScript@NScript@@QAEXXZ (egor_pdb_names.tsv + egor_signatures.tsv).
- FableWin.pdb: 01802a0f / 03923a30 same mangled name (_fablewin_api.tsv, FableWin.exe.signatures.tsv); a cross-match table in
  ghidra_out maps e2eac0 -> ?TricksterApproach@CV_AmbushScamScript@NScript@@QAEXXZ (score 0.74, 97.1, true).
- Sole caller: 0x00E2B772 in landed Main (rebuild/src/compiled/00/e2/NScriptCVAmbushScamScript_Main_00e2b090.cpp line 84:
  `TricksterApproach(); TricksterApproachComplete = true;`).
- Signature: `void NScript::CV_AmbushScamScript::TricksterApproach()` -- public, non-const, non-virtual, no params. Manifest label CORRECT.
  Already declared in rebuild/include/engine/CV_AmbushScamScript.h.

## 3. Callees
| addr | landed row | landed/PDB name | note |
|---|---|---|---|
| 0x00E2EC30 | NOT landed (pending 2026-11-03_linux_hand730 DIFFER 1053v1336) | ?GetTrickster@CV_AmbushScamScript@NScript@@QAE?AVCScriptThing@@XZ | declared in shared header; draft OK |
| 0x00CB7940 | 00/cb/CScriptBase_IsActiveThreadTerminating_00cb7940.cpp | CScriptBase::IsActiveThreadTerminating (QAE_NXZ) | engine/CScriptBase.h line 35 |
| 0x00CBE2FF | 00/cb/global_IsDistanceBetweenThingsUnder_00cbe2ff.cpp | ?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z (__fastcall, ret 4) | draft OK; declare yourself (no header) as CAS_Ambusher::Main 0x00E2FD30 does |
| 0x004AA840 | 00/4a/CScriptThing_CScriptThing_004aa840.cpp | ??1CScriptThing@@UAE@XZ | out of line only at NEAR+terminating |
| 0x0099A2E0 | 00/99/CBaseClass_CBaseClass_0099a2e0.cpp | ~CBaseClass | via engine/CBaseClass.h |
| 0x00BFE9BC | 00/bf/global_operatordelete_00bfe9bc.cpp | operator delete | |
| Game slot 7 / 70 / 264 | engine/CGameScriptInterfaceBase.h lines 30/93/287 | NewScriptFrame() const / `CScriptThing& GetHero()` (NON-const, returns non-const ref) / `float GetHealth(const CScriptThing&) const` | draft had `const CScriptThing& GetHero() const` -- wrong spelling vs header |
| PImp.Data slot 75 (+0x12C) | 00/4a/CScriptThing_IsAlive_004ab130.cpp (out-of-line copy) | CScriptThing::IsAlive() const | expanded inline here |
Draft spelling disagreements: member `GSI` should be `Game` (CLevelScriptBase<T>::Game +0x40, type CGameScriptInterfaceBase*);
`CQuestScriptBase`/`Owner` are invented -- real base is CLevelScriptBase<CQ_SunnyvaleMasterData> (Game +0x40, MasterScriptData +0x44).

## 4. Members
All in rebuild/include/engine/CV_AmbushScamScript.h (Ego_r.pdb layout minus 0x14): DoTricksterIntro bool +0x4E, TricksterKilledBeforeAmbush
bool +0x57, TricksterApproachProximity float +0x5C; Game +0x40 from engine/CLevelScriptBase.h. CScriptThing PImp (CCountedPointer, Data +4,
Info +8) from engine/CScriptThing.h + engine/CCountedPointer.h.
Opt-ins used by the landed Main in this same TU (copy these): FABLE_RETAIL_NSCRIPT, FABLE_RETAIL_CLEVELSCRIPTBASE, FABLE_RETAIL_CCHARSTRING,
FABLE_RETAIL_SUSPENDABLE_PROCESS, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CDEFSTRINGTABLE, FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
(+ CENTITYSCRIPTBINDING one, Main-specific). CScriptThing.h also offers FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY / _ORIGINAL_ACCESS / _NULLCOORD / _NULLSTRING (not needed).
Draft models LOCALLY (all have shared headers -- replace): CCPPointerInfo/CCountedPointer (engine/CCountedPointer.h), CBaseClass (engine/CBaseClass.h),
CScriptThing (engine/CScriptThing.h), CGameScriptInterface (engine/CGameScriptInterfaceBase.h), CScriptBase (engine/CScriptBase.h),
CQuestScriptBase (-> engine/CLevelScriptBase.h), CV_AmbushScamScript (engine/CV_AmbushScamScript.h).

## 5. Callee visibility
- ~CScriptThing: plain inline, defined after the include: `inline CScriptThing::~CScriptThing() { }` (exact line in Main 0x00E2B090 and
  CV_KnotholeGladeGatesScript::WatchForCutscene1 0x00EA2720). Retail mixes 3 inline copies + 1 out-of-line call -> VC7.1 per-site frequency
  rule (docs/pipeline/INLINE_DEPTH_REVISIT.md, last section); Main matched by writing EVERY terminate check as
  `do { if (IsActiveThreadTerminating()) return; } while (0);` (23 sites, RELOCATION_MATCH).
- IsAlive: inline after include: `inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }` (as 0x00EA2720, 0x00E2FD30).
- GetTrickster 0x00E2EC30 sits right after this function (same TU, defined LATER) -> called out of line; IsActiveThreadTerminating, IsDistanceBetweenThingsUnder: other TUs.
- Fallback per EXTRA_RULES (owner 2026-11-08): a per-row /d2inlT<N> / /d2inlS<N> flag, else scoped `#pragma inline_depth`, with line 2 `// Inline-budget: ...`.
  No STLport helpers involved.

## 6. Siblings (same TU / class)
- 0x00E2B090 Main -- header-based, opt-ins above, macro terminate form, plain-inline dtor; no pragmas, no Flags. BEST template.
- 0x00E2FD30 CAS_Ambusher::Main -- GetTrickster + inline IsAlive + IsDistanceBetweenThingsUnder decl (local class models, older style).
- 0x00E2FC50 WatchForAllAmbushersDead -- FORCEINLINE vector dtor trick (inline-once band).
- 0x00E2E520 WatchForHeroAttack -- GetHealth/1e-4f, two-site expansion via `do{...}while(0)` terminates; older local models.
- 0x00EA2720 CV_KnotholeGladeGatesScript::WatchForCutscene1 -- another CLevelScriptBase-header row with inline dtor/IsAlive after include.

## 7. Draft model errors
- Local models everywhere (see 4); `GSI` misnamed; GetHero const-ness wrong.
- Terminate checks written as plain `if (...) return;` -- the residue is exactly the inline-expansion-per-site gap the Main row fixed with
  `do { if (IsActiveThreadTerminating()) return; } while (0);` at every site (initial, loop-end, dead arm, near arm, tail).
- Flow otherwise right: initial check then rotated loop body (alive||health check -> dead arm; distance -> near arm; NewScriptFrame; check),
  dead arm sets +0x57, near arm +0x4E, both `break` to a final terminate check. The dead zero dword at frame base is unexplained by the draft
  (likely a VC temp/flag; might be GetHero()'s reference-binding or an unused local) -- check if it appears once the header GetHero (non-const
  CScriptThing&) is used.
- Header comment says 368 bytes; real is 363.

## 0x005590C0

# DOSSIER 0x005590C0 -- NUISystem::CScrollBarOutside::OnLeftClicked (365 bytes)

## 1. Retail shape (xdis 005590c0 0x17d)
- thiscall, `sub esp,0x40 ; push esi ; mov esi,ecx`, NO frame pointer (/Oy on, no optimize("y",off)), NO SEH frame
  (the CDefPointer temporary's dtor has no EH state -- same as CClickable::OnLeftClicked 0x0055AF60), plain `ret` (no args).
- Two epilogues (`pop esi ; add esp,0x40 ; ret`) at 0x5591F8 and 0x559228: the "under" path duplicates the
  ObserveEvent(0x1c) tail and returns early.
- Prologue 0x5590C0..0x55910E is BYTE-IDENTICAL (modulo frame offsets) to CClickable::OnLeftClicked 0x0055AF60:
  `StateBeforeLeftClicked = <+0x148>; ChangeState(LoadAndReturnDef()->LeftClickedState);` then the def release
  `mov edx,[eax+4]; mov ecx,eax; add eax,4; dec edx; test edx,edx; mov [eax],edx; jne; call [vt+4]`
  (DecRef/GetRefCount split through the count base at +4 -- copy the landed model in
  rebuild/src/compiled/00/55/NUISystemCClickable_OnLeftClicked_0055af60.cpp: CDefPointer + CClickableDef::ReleaseTemporary()).
- Default /O2 flags (siblings say "Flags: none" / "base /O2 /Oy (no pragma)"); (long) casts -> __ftol2 0x00BFEA70.
- Data flow (stack offsets pre-push):
  mouse = PMouseCursor->GetPosition()                  [esp+0x14] (Y 0x18)   vslot 16 +0x40
  pos   = PScrollBar->GetTargetRelativeRenderPosition() [esp+0x0c] (Y 0x10)   vslot 122 +0x1e8
  zoom  = PScrollBar->GetTargetRelativeRenderZoom()     [esp+0x1c] (Y 0x20)   vslot 123 +0x1ec
  top   = (long)pos.Y                                    [esp+0x38]
  CManager::GetInstance()->GetUIScale()  -> [esp+4]   RESULT DEAD
  PScrollBar->GetSize()                  -> [esp+0x24] RESULT DEAD (slot 23 +0x5c)
  CManager::GetInstance()->GetUIScale()  -> [esp+0x24] (reuses slot); its .Y copied with INTEGER moves to [esp+4]
  PScrollBar->GetSize()                  -> [esp+0x2c]; .Y used
  bottom = (long)(scaleY * size.Y * zoom.Y + pos.Y)  (fld scaleY; fmul size.Y; fmul zoom.Y; fadd pos.Y; ftol)
  if (bottom < mouse.Y) { ExecuteActions(PActionsOnLeftClickedUnder); ObserveEvent(0x1c); return; }  (fild/fcomp, test ah,5 ; jp)
  if (top > mouse.Y) ExecuteActions(PActionsOnLeftClickedAbove);                                     (test ah,0x41 ; jne)
  ObserveEvent(0x1c);   // CObserver subobject at this+4, vslot 3 (+0xc)
- The 4-call pattern = two scaled-size products whose X half is dead: landed CMouseCursor::GetScaledSize 0x0055C600
  (rebuild/src/compiled/00/55/CMouseCursor_GetScaledSize_0055c600.cpp) spells it
  `size.X = GetInstance()->GetUIScale().X * size.X; size.Y = GetInstance()->GetUIScale().Y * size.Y;`.
  Here each product calls PScrollBar->GetSize() again: likely `scaled.X = UIScale().X * PScrollBar->GetSize().X;
  scaled.Y = UIScale().Y * PScrollBar->GetSize().Y;` (X dead-stored and eliminated), evaluation order UIScale before GetSize.
- Order: PScrollBar (+0x194) is tested before the cursor; the cursor is fetched (GetInstance()->+0xB8) BEFORE the if.

## 2. Identity (proven, 3 sources)
- Ego_r.pdb (egor_pdb_names.tsv 005d2250) and FableWin.pdb (_fablewin_api.tsv 0183cd54): `?OnLeftClicked@CScrollBarOutside@NUISystem@@UAEXXZ`
  -> `public: virtual void __thiscall NUISystem::CScrollBarOutside::OnLeftClicked(void)`. Ego_r size 0x5d23c0-0x5d2250 = 368 ~ 365.
- rtti-vtables.tsv: CScrollBarOutside primary vtable 0x0124B54C slot 146 = 0x005590C0 (xref: abs .rdata 0x0124B794 = 0x124B54C+146*4).
  Neighbours: slot 43 Initialise 0x005590A0, 152 SetScrollBar 0x005590B0, 150/151 AddActionOnLeftClickedAbove/Under 0x559400/0x559490.
- Manifest label correct (only missing the NUISystem namespace). No callers other than the vtable.

## 3. Callees (landed file / PDB name / draft spelling)
- 0x0041E5F2 rebuild/src/compiled/00/41/NUISystemCManager_GetInstance_0041e5f2.cpp -- `static NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance()`
  (?GetInstance@CManager@NUISystem@@SIPAV12@XZ). DRAFT WRONG: `CFrontEndManager::GetInstance`, and it returns CManager* (draft casts to a fake CFrontEndManagerData).
- 0x0041CF47 rebuild/src/compiled/00/41/NUISystemCManager_GetUIScale_0041cf47.cpp -- `C2DVector NUISystem::CManager::GetUIScale() const`
  (?GetUIScale@CManager@NUISystem@@QBE?AVC2DVector@@XZ). DRAFT WRONG: non-const, class CManager outside namespace.
- 0x00BFEA70 __ftol2 (toolchain_library; compiler-emitted from (long) casts).
- vslot 108 +0x1b0: `CDefPointer<const NUISystem::CUIDef> CComponent::LoadAndReturnDef() const` (Ego_r 005aa830). DRAFT WRONG: GetStyleDef / CStyleDefPointer.
- vslot 48 +0xc0: `CComponent::ChangeState(unsigned long)` (impl CChangingStateComponent::ChangeState(K) Ego_r 005a79f0, retail 0x0052CF40). DRAFT WRONG: PlaySound(long).
- vslot 131 +0x20c: `CComponent::ExecuteActions(std::list<CCountedPointer<CAction> >*)` (Ego_r 005aaff0; retail 0x005309B0). DRAFT WRONG: RunActions(CActionList*).
- vslot 122 +0x1e8: `C2DVector CComponent::GetTargetRelativeRenderPosition() const` (Ego_r 005a8a80, FableWin 0178ff37). DRAFT WRONG: GetRenderPosition (that is slot 25 and returns const&).
- vslot 123 +0x1ec: `C2DVector CComponent::GetTargetRelativeRenderZoom() const` (Ego_r 005a88b0, FableWin 017b81ee). DRAFT WRONG: GetRenderSize.
- vslot 23 +0x5c: `C2DVector CComponent::GetSize() const` (Ego_r 005ac4e0). Draft name OK, missing const.
- vslot 16 +0x40 on the cursor (a CComponent*): `C2DVector CComponent::GetPosition() const` (Ego_r 005a8450). Draft modelled a fake CMouseCursor class; PMouseCursor is typed `NUISystem::CComponent*`.
- CObserver vslot 3 (+0xc on this+4): `void CObserver::ObserveEvent(EEvent)` (Ego_r 005af180; retail 0x0052DA20). Draft name OK; type should be EEvent, arg (EEvent)0x1c.
- def deleter: vslot 1 of the CUIDef (CIVCountedPointee delete-self).

## 4. Members
- this+0x148: retail CChangingStateComponent::TargetState (unsigned long). Shared header engine/NUISystem_CChangingStateComponent.h
  (CurrentState +0x144, TargetState +0x148; PDB CurrentState 372/TargetState 376 minus the uniform retail 0x30 shift).
  NOTE: the landed CClickable::OnLeftClicked local model calls +0x148 "CurrentState" -- header/PDB say TargetState. Draft "State" wrong.
- this+0x16c: CClickable::StateBeforeLeftClicked (long) -- PDB 412-0x30; engine/NUISystem_CClickable.h. Draft "ClickState" wrong.
- this+0x194 PScrollBar (NUISystem::CComponent*), +0x198 PActionsOnLeftClickedAbove, +0x19c PActionsOnLeftClickedUnder
  (std::list<CCountedPointer<CAction> >*) -- engine/NUISystem_CScrollBarOutside.h (sizeof 0x1a0 asserted). Draft types (CActionList*) wrong.
  The header does NOT declare the OnLeftClicked override ("Overrides are not declared"); CClickable.h declares `virtual void OnLeftClicked();`.
- def+0x20c: CUIDef::LeftClickedState (long) -- engine/NUISystem_CUIDef.h line 134, static-asserted 0x20c. Draft "ClickSound" wrong.
- CManager+0xB8: `NUISystem::CComponent* PMouseCursor` (protected) -- engine/NUISystem_CManager.h; only opt-in is FABLE_RETAIL_CMANAGER_MOUSECURSOR
  (SetMouseCursor inline). No getter exists in the header and none in the PDB publics (an inline accessor would not be listed); a getter
  name would be descriptive/unproven. FABLE_RETAIL_CMANAGER gives GetUIScale() const.
- Opt-in macros seen: FABLE_RETAIL_CMANAGER, FABLE_RETAIL_CMANAGER_MOUSECURSOR, FABLE_RETAIL_CCLICKABLE_SHARED_ACTION,
  FABLE_RETAIL_COBSERVER_DESTRUCTOR (CComponent.h), FABLE_RETAIL_CUIDEF, FABLE_RETAIL_C2DVECTOR(_FORCEINLINE_CTOR),
  FABLE_RETAIL_CDEFPOINTER_RESET / _RELEASE_SELFDELETING_POINTEE (engine/CDefPointer.h; dtor is FABLE_DEFPTR_INLINE).
  C2DVector.h has NO operator*(const C2DVector&) (only *= ) -- draft's local operator* is invented.
- Draft models locally (shared headers exist): C2DVector, CIVCountedPointeeBase/CStyleDef (=CUIDef), CStyleDefPointer (=CDefPointer),
  CManager, CMouseCursor/CComponent, CObserver, CScrollBarOutside. Caveat: every landed NUISystem sibling (0x55AF60, 0x54DCC0,
  0x559230) still uses local models ("CComponent is quarantined" note in 0x0054DCC0), so a shared-header build may not be feasible.

## 5. Callee visibility
- GetInstance 0x41E5F2 and GetUIScale 0x41CF47 are out-of-line in another TU (CManager .cpp, size-optimised): declare only.
- All other callees are virtual -> visibility irrelevant. ExecuteActions/ChangeState/ObserveEvent defined elsewhere.
- Only inline-visible pieces: the CDefPointer dtor + CUIDef release (must be inline, split DecRef/GetRefCount form), C2DVector copy/ctor,
  and the (unproven-name) PMouseCursor accessor. No STLport helpers called.

## 6. Siblings
- 0x0055AF60 CClickable::OnLeftClicked -- same prologue + release; local CDefPointer/ReleaseTemporary model; no pragmas.
- 0x0055C600 CMouseCursor::GetScaledSize -- the UIScale().X*size.X / .Y*size.Y idiom.
- 0x0054DCC0 CFrontEndButton::OnUnhovered -- LoadAndReturnDef + CUIDefPointer FABLE_FORCEINLINE dtor + local CComponent slot table, CObserver at +4 with ObserveEvent slot 3; includes fable_compiler.h.
- 0x00559230 CScrollBarOutside::GetDataFromDef (same TU) -- local CDefPointer, engine/CCountedPointer.h, no pragmas.
- 0x005595A0 / 0x005595B0 adjustor thunks -- "Flags: none", include engine/NUISystem_CScrollBarOutside.h.

## 7. Old draft's model errors
1. Only 3 of the 4 calls: GetUIScale is called ONCE in the draft (and its operator* consumes the first GetSize). Retail calls
   GetUIScale, GetSize, GetUIScale, GetSize; first pair dead. That alone is the 5-byte gap (360 vs 365).
2. Def release `if (old && --old->IVRefCount == 0)` -> retail is load/dec/test/store with ecx=def kept; use the 0x55AF60 model.
3. Wrong slot names/types (above): PlaySound, RunActions, GetStyleDef, GetRenderPosition/Size, CFrontEndManager, ClickSound, State, ClickState.
4. scaleY is copied via integer moves into its own slot before the second GetSize call -> spelled as a float copy of `GetUIScale().Y`
   (member-wise assignment of the product, not a fused C2DVector operator*).
5. Control flow (both compares, early-return duplicate tail, top computed before the UIScale calls) is already right; keep it.

## 0x00541FD0

# DOSSIER 0x00541FD0 -- NUISystem::CIconText::GetDataFromDef (366 bytes)

## 0. Headline: an exact landed TWIN exists
`rebuild/src/compiled/00/54/NUISystemCDefString_GetString_0054ed90.cpp` = NUISystem::CText::GetDataFromDef @ 0x0054ED90
(365 bytes, landed; its manifest label "CDefString::GetString" is wrong, the file header says Ego_r ?GetDataFromDef@CText@NUISystem@@QAEXXZ).
Disassembly of 0x54ED90 is instruction-for-instruction the same shape as 0x541FD0 (same LoadAndReturnDef slot +0x1b0,
GetString 0x9D49B0 then GFGetSystemManager 0x9A4EC0, [eax+0x84], GetFont 0x9E2C80, ebx/edi operator=, temp release,
the two retained zero stores `mov [esp+14h],0 ; mov [esp+18h],0` before ~CCharString, pointer-register 16-byte box copy,
alignment switch |= 8/0x10/0x20 into +0x12E). Its body (no pragmas, no Flags line, no FABLE_RETAIL defines) is:
```
CDefPointer_CUIDef def = LoadAndReturnDef();
PFont = GFGetSystemManager()->GetFontBank()->GetFont(def->Font.GetString());   // ONE full expression
Window = def->TextWindow; ... SetValue(...); switch (def->Alignement) {...}
```
with `struct CDefString { long Index; CCharString GetString() const { return GDefStringTable.GetString(Index); } };`
(inline accessor) and `CSystemManager::GetFontBank() const { return FontBank; }` (inline getter at +0x84), shared
`engine/CCountedPointer.h` default (no opt-ins). That twin keeps the zero stores AND the GetString-before-GFGetSystemManager order
-- exactly the residue the old draft could not fix (draft called g_DefStringTable.GetString directly / used a named local).

## 1. Retail shape (xdis 00541fd0 0x17e)
- __thiscall, plain `ret` (0x0054213D), no args. NO frame pointer (sub esp,10h; push ebx/esi/edi) -> default /Oy. No SEH frame
  (no fs:0) even though temporaries have dtors -> nothing throws-visible / EH not generated here (same as twin).
- Locals: def CDefPointer [esp+0Ch], CCharString name [esp+10h], CCountedPointer<CFontBank> temp [esp+14h..1Bh].
- Sequence: vcall [vt+1B0h](&def) ; GDefStringTable(0x013CA828).GetString(def->Font.TablePos @+0x50) ; GFGetSystemManager() ;
  mgr+0x84 ; GetFont(&temp,&name) ; PFont(+0x164/+0x168) = temp via ShareData shape (edi=Info, ebx=Data loaded first; cmp Info;
  release reloading [esi+168h]; store Data,Info; `test edi` = param info, inc [edi]) ; temp dtor (release, Data=0 then Info=0) ;
  ~CCharString 0x99EAE0 ; Window(+0x198) = def->TextWindow(+0xA4) 4-dword copy via lea pointers ; PositionOffset(+0x1A8) =
  def->PositionOffset(+0x142, unaligned) ; vcall [vt+240h](def+0x54) = SetValue(const CWideString&) ; switch def->Alignement(+0x1FC)
  {0:|=8, 1:|=0x10, 2:|=0x20} on byte +0x12E ; def release WITHOUT null test: `mov esi,[ecx+4]; lea eax,[ecx+4]; dec esi; mov edx,esi;
  test edx,edx; mov [eax],esi; jne; call [vt+4]` (CDefPointer.h default Release produces this load/dec/store form per its header note).
- Differences vs CText twin: no ScaleText byte, SetValue takes ONE arg (CIconText slot 144), extra PositionOffset copy.

## 2. Name / identity (two+ sources)
- Ego_r.pdb: 0x005BB900 ?GetDataFromDef@CIconText@NUISystem@@QAEXXZ (ghidra_out/egor_pdb_names.tsv); naming_stragglers lists
  005bb900 with length 366 = retail length; bsim_port_audit_05final.tsv maps 541fd0 -> same symbol (0.69).
- FableWin.pdb: 0x02262EA0 (+ jmp thunk 0x01802A5F) ?GetDataFromDef@CIconText@NUISystem@@QAEXXZ (_fablewin_api.tsv).
- PDB source order (Ego_r): ... FitInWindow, SetValue(CCharString), SetValue(CWideString), GetDataFromDef, SetValue(ulong), ctor
  = retail order 541A00, 541E60, 541F20, 541FD0, 542140, 542330.
- Shared header already declares it: engine/NUISystem_CIconText.h `void GetDataFromDef(); // Ego_r ?GetDataFromDef@...`.
- Mangled leaf: ?GetDataFromDef@CIconText@NUISystem@@QAEXXZ -> public, non-virtual, non-const, void(void). Manifest label CORRECT.
- Callers (xref): 0x005423CF in ctor CIconText(long) 0x00542330; 0x00542490 (copy ctor region). NAME FIX: the landed ctor
  rebuild/src/compiled/00/54/CIconText_CIconText_00542330.cpp (line ~70) and NUISystemCIconText_vectordeletingdestructor_00543b30.cpp
  (line 70) call 0x00541FD0 `UpdateFont()` -- no UpdateFont exists for CIconText in either PDB -> NAME_FIXES.tsv: 00541FD0 UpdateFont -> GetDataFromDef.

## 3. Callees
| addr | landed row | landed name | PDB | draft spelling |
|---|---|---|---|---|
| vt+0x1B0 slot 108 | (CComponent) | LoadAndReturnDef() const -> CDefPointer<const NUISystem::CUIDef> (NUISystem_CComponent.h:177) | Ego_r | draft: returns local CDefPointer of local CIconTextDef -- WRONG type, use CUIDef |
| 0x009D49B0 | 00/9d/CDefStringTable_GetString_009d49b0.cpp | CDefStringTable::GetString(long) const | ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z | draft non-const, global misnamed `g_DefStringTable`; real = GDefStringTable (?GDefStringTable@@3VCDefStringTable@@A) |
| 0x009A4EC0 | 00/9a/global_GFGetSystemManager_009a4ec0.cpp | GFGetSystemManager (__fastcall) | Ego_r | ok |
| 0x009E2C80 | 00/9e/CFontManager_GetFont_009e2c80.cpp | CFontManager::GetFont(const CCharString&) | ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z (Ego_r 0x00B2C2D0) | draft `CGraphicBank::GetGraphic` -- WRONG (it's the font manager) |
| mgr+0x84 | engine/CSystemManager.h | PFontManager (CCountedPointer<CFontManager>), GetFontManager() inline | Ego_r ?GetFontManager@CSystemManager@@QAEPAVCFontManager@@XZ | draft `PGraphicBank` WRONG |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok |
| 0x00BFE9BC | global_operatordelete_00bfe9bc.cpp | operator delete | ok |
| vt+0x240 slot 144 | 00/54/NUISystemCIconText_SetValue_00541f20.cpp | CIconText::SetValue(const CWideString&) | ?SetValue@CIconText@NUISystem@@UAEXABVCWideString@@@Z | draft `SetText` WRONG |
| def vt+4 slot 1 | | CDefPointeeBase/CIVCountedPointeeBase::Delete() const | | draft `Delete` ok |
| count block +4 | | CCPPointerInfo::DeleteFunc | | draft m_pfnDestroy (local) |

## 4. Members (this = CIconText, header engine/NUISystem_CIconText.h; Ego_r layout minus 0x30)
- +0x12E byte: CComponent `FlagsSetTwo` (NUISystem_CComponent.h:247; bits 3/4/5 = left/centre/right). Draft: `AlignmentFlags` WRONG name.
- +0x164/+0x168: `CCountedPointer<CFontBank> PFont` (Ego_r +404). Draft: `Icon` WRONG.
- +0x198: `C2DBoxF Window` (Ego_r +464). Draft: `CRGBAColour Colour` WRONG type/name.
- +0x1A8: `C2DVector PositionOffset` (Ego_r +480). Draft: `Offset`.
- Header opt-ins: FABLE_RETAIL_CICONTEXT_VSLOTS, _EMBEDDEDICON_CTOR / _ARGS_CTOR (irrelevant). Members are `protected` (fine in member fn).
  Includers define FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE (CanScroll 0x5419F0, GetDynamicSize, MakeAvailable).
Def = NUISystem::CUIDef (engine/NUISystem_CUIDef.h, retail branch needs FABLE_RETAIL_CUIDEF which auto-defines FABLE_RETAIL_C2DVECTOR):
  Font CDefString +0x50 (TablePos), TextValue CWideString +0x54, TextWindow C2DBoxF +0xA4, PositionOffset C2DVector +0x142 (packed),
  Alignement ETextAlignement +0x1FC (LEFT=0, CENTER=1, RIGHT=2). CDefString.h needs FABLE_RETAIL_CDEFSTRINGTABLE + FABLE_RETAIL_CCHARSTRING;
  its inline `GetString() const { return GDefStringTable.GetString(TablePos); }` and `operator CCharString()` exist (default branch = plain inline;
  opt-ins _FORCEINLINE_GETSTRING / _FORCEINLINE_CONVERSION / _OUTOFLINE_CONVERSION available).
Locally modelled in the draft though shared headers exist: CCharString/CWideString (fable_string.h), CCountedPointer + count block
  (engine/CCountedPointer.h, CCPPointerInfo), CDefPointer (engine/CDefPointer.h), CIconTextDef (= NUISystem::CUIDef), CSystemManager
  (engine/CSystemManager.h, FABLE_RETAIL_CSYSTEMMANAGER for GetFontManager, FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER for +0x84 placement),
  CFontManager (engine/CFontManager.h), CDefStringTable (engine/CDefStringTable.h), C2DVector, CIconText itself, CRGBAColour (should be C2DBoxF).
Sibling with shared headers for the same calls: CTable::GetDataFromDef 00/55/CTable_GetDataFromDef_00551340.cpp defines
  FABLE_RETAIL_DEF_CLASS_BASE, _CCHARSTRING, _CDEFSTRINGTABLE, _C2DVECTOR, _C2DBOXF, _CUIDEF, _STLP_LIST, _CCOUNTEDPOINTER; `CDefPointer<const CUIDef> def = LoadAndReturnDef();`.
  CAUTION: FABLE_RETAIL_CCOUNTEDPOINTER only changes the (T*) ctor; FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA changes ShareData to reload Info
  (retail here tests the PARAM `test edi,edi` -> do NOT define _RELEASEDATA). Default dtor = release then Data=0, Info=0 = retail order.

## 5. Callee visibility
- CCountedPointer operator=/ShareData/ReleaseData/dtor: inline in header (default branch) -> expanded in retail (yes).
- CDefPointer members: __forceinline (FABLE_DEFPTR_INLINE) -> expanded (yes, matches).
- CDefString::GetString / CSystemManager::GetFontManager: inline accessors -> expanded; GetString 0x9D49B0, GetFont 0x9E2C80,
  GFGetSystemManager are out-of-line calls. No STLport helpers called.
- No same-TU out-of-line neighbours called (SetValue is via vtable).

## 6. Sibling rows
- 0x0054ED90 CText::GetDataFromDef (twin, above): no defines, local models, one-expression font assignment.
- 0x00551340 CTable::GetDataFromDef: `// Flags: none`, shared headers + defines listed above.
- 0x0055B040 CClickable::GetDataFromDef: FABLE_RETAIL_CDEFSTRINGTABLE/_CCHARSTRING/_CWIDESTRING/_DEF_CLASS_BASE, NUISystem_CClickable.h + NUISystem_CUIDef.h + CDefPointer.h.
- 0x00541390 CDynamicList::GetDataFromDef (same TU neighbourhood): local models, CUIDef packed, CIVCountedPointeeBase.h.
- 0x005419F0 CanScroll / 0x00543A60 GetDynamicSize: FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE + engine/NUISystem_CIconText.h.
No row in this class uses pragmas (no optimize("s"), no /Oy-). 0x00543700 has `// Flags: /Gr` (STL copy; unrelated).

## 7. Old draft model errors
1. Split the font lookup into a block with a named `CCharString name` + named bank + separate `Icon = ...` statement, and called the table
   directly: the twin shows the retail spelling is ONE full expression `PFont = GFGetSystemManager()->GetFontManager()->GetFont(def->Font.GetString());`
   with GetString as an inline CDefString member (that gives GetString-before-GFGetSystemManager and keeps the temp's zero stores).
2. Wrong identities: Icon/CGraphic/CGraphicBank::GetGraphic/PGraphicBank -> PFont/CFontBank/CFontManager::GetFont/PFontManager; SetText -> SetValue;
   Colour(CRGBAColour with user copy ctor) -> Window (C2DBoxF, plain struct copy -- twin's plain `struct C2DBoxF{float x4}` gives the pointer-register copy);
   Offset -> PositionOffset; AlignmentFlags -> FlagsSetTwo; IconName -> Font (CDefString); CIconTextDef -> CUIDef; g_DefStringTable -> GDefStringTable.
3. Switch with case 0/1/2 is right (retail `sub eax,0; je; dec; je; dec; jne`), use ETextAlignement LEFT/CENTER/RIGHT.
4. Hand-written CCountedPointer operator= loaded other.Info/Data as locals; header's ShareData(other.Data, other.Info) gives the edi/ebx order.

## 0x008C2AD0

# DOSSIER 0x008C2AD0 -- CCombatAction_WillHealInto::FinishAction (370 bytes)

## 1. Retail shape (xdis 008c2ad0 0x182)
- __thiscall, `ret` (no args), void. No frame pointer (ebp = this; /Oy on). No SEH prologue (no fs:0) despite CCharString /
  action temporaries with dtors -> TU built without EH unwind tables (same as landed siblings 0x008C29F0/0x008C2A80, no `// Flags`).
- Frame `sub esp,0x120`; push ebx,ebp,esi,edi; ebx=0 kept as the zero register. Locals: [esp+0x10] effectType (long, =0;
  slot later reused by the "" CCharString), [esp+0x14] CDefPointer<const CSpecialEffectsDef> effects (=0, FUNCTION scope:
  released at the very end 0x8c2c25 after the action temporary dtor), [esp+0x18] "HEAL" CCharString, [esp+0x1c] the
  0x104?-byte WillHealRelease temporary (bottom of frame). edi = &PCreature (this+8) kept across the body.
- Flow: GetPItem -> [creature+0x70] PBaseDef copied with add-ref (`inc [esi+4]`), GetSubDef(&effects), release of the copy
  (`dec [esi+4]; jne; call [vtbl+4]`) -> `mov esi,[esp+14h]; cmp esi,ebx; je` -> CCharString("HEAL",-1) ->
  effects->GetEffectFromString(str,effectType) -> ~CCharString -> if true: CTCDParticleEmitter::Create(effectType,
  creature->GetPos(), false) (__fastcall ecx/edx, push 0) -> if (effect && !Dead(+0x91 bit0) && HasTC(75) (`mov ecx,[eax+28h];
  test ch,8`)) { esi=[eax+5Ch] PTCDriver; CCharString(""); emitter->AttachToThing(*GetPItem(), 10, str, 0, 0.0f) }.
  Then creature=GetPItem() (esi) BEFORE CCombatActionBase::FinishAction() (direct, non-virtual); if !HasTC(153 TCI_HEAL_LIFE)
  (`test dword [esi+30h],2000000h`, in place, no bool materialised) { AddTC(CCharString("CTCHealLife"), true, 0)->SetAICast(
  this->SpellLevel [+0x114]) }; creature->SetCurrentAction(CCombatAction_WillHealRelease(*creature)); temp dtor 0x0062E900;
  effects released last.
- Known residue (STATUS/NOTES of pending/2026-10-21_linux_hand242 line 45): DIFFER 381v370, ours `mov eax,[def]; cmp eax,ebx;
  mov esi,eax` vs retail `mov esi,[def]; cmp esi,ebx`.

## 2. Name / signature
- vtable 0x012948AC (CCombatAction_WillHealInto, rebuild/manifest/rtti-vtables.tsv) slot 4 = 0x008C2AD0; slot 4 of
  CCombatActionBase is FinishAction (0x00859A20 `?FinishAction@CCombatActionBase@@MAEXXZ`). xref: only the .rdata vtable word 0x012948BC.
- PDB: Ego_r `?FinishAction@CCombatAction_WillHealInto@@UAEXXZ` (Ego_r 0x008FAF00), FableWin _fablewin_api.tsv 0x017DC3C8/0x02A11440 same.
- Manifest label is RIGHT (doubled class prefix only). Leaf: `?FinishAction@CCombatAction_WillHealInto@@UAEXXZ`
  = public virtual void FinishAction(). Base declares it protected virtual (MAE).
- Member: SpellLevel (long) -- struct_layouts_egor.tsv CCombatAction_WillHealInto +300 (0x12c PDB) = retail +0x114 (retail CCombatActionBase 0x114).
  No engine/CCombatAction_WillHealInto.h header exists (siblings model the derived class locally).

## 3. Callees (retail addr | landed file | landed name | PDB name | draft spelling)
- 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const -> CBaseObject* |
  same | draft: CCreaturePointer::GetPItem -> CThingCreatureBase*  ** WRONG: use engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER) as siblings do.
- 0x00669580 | 00/66/CDefClassBase_GetSubDefCSpecialEffectsDef_00669580.cpp | `??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z`
  = bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>*) const | draft: non-const, by REFERENCE, non-const T ** WRONG spelling.
- 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString::CCharString(const char*, long) | ok.
- 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | CCharString::~CCharString | ok.
- 0x00669D30 | 00/66/CCRC_CalcAndLookup_00669d30.cpp (landed as `CCRC::CalcAndLookup`, Leaf "CalcAndLookup") | PDB
  `?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z` (Ego_r 0x00526CA0; functions.tsv names it too; landed
  row 0x008C1F70 cites agentNF16 Ego_r/FableWin) | draft ok (bool GetEffectFromString(const CCharString&, long&) const).
  ** NAME_FIX candidate: landed 0x00669D30 spelling CCRC::CalcAndLookup.
- 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | Leaf `?GetPos@CThing@@QBEABVC3DVector@@XZ` | draft calls it GetPosition ** WRONG -> GetPos.
- 0x006E0880 | NOT landed | functions.tsv CTCDParticleEmitter::Create __fastcall; Ego_r `?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z`
  = static CThing* __fastcall Create(long, const C3DVector&, bool) | draft: free `CreateEffectThing(long,const C3DVector*,bool)` ** WRONG name.
- 0x006E0BE0 | 00/6e/CTCDParticleEmitter_AttachToThing_006e0be0.cpp | `?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z`
  (CThing&, long, const CCharString&, long, float) | draft passes CThing* ** WRONG (pointer vs reference).
- 0x00859A20 | 00/85/CCombatActionBase_FinishAction_00859a20.cpp | `?FinishAction@CCombatActionBase@@MAEXXZ` (header opt-in FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION) | ok.
- 0x004C9D60 | 00/4c/CThing_AddTC_004c9d60.cpp | Ego_r `?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z`
  = CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*) | draft returns CTCHealLife*, params non-const ** WRONG.
- 0x007C3EA0 | 00/7c/AddExclusiveObserver_AddExclusiveObserver_007c3ea0.cpp landed as `C_007c3ea0::M(unsigned long)` (stores +0x18) |
  real: `?SetAICast@CTCHealLife@@QAEXJ@Z` (Ego_r 0x007C2CC0) -- second source: struct_layouts_egor CTCHealLife +24 (0x18) = long
  SpellLevel, exactly the field the body stores. ** NAME_FIX: 0x007C3EA0 -> CTCHealLife::SetAICast(long).
  AddTC's CTCBase* result needs a cast to CTCHealLife* (static_cast or a dynamic-cast helper that folds; retail has none).
- 0x008C3930 | 00/8c/CCombatActionWillHealRelease_CCombatActionWillHealRelease_008c3930.cpp | `??0CCombatAction_WillHealRelease@@QAE@AAVCThingCreatureBase@@@Z`
  (CThingCreatureBase&) | draft takes pointer ** WRONG.
- 0x006644F0 | 00/66/CThingCreatureBase_SetCurrentAction_006644f0.cpp | `?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z` |
  draft param `const CCombatActionBase&` ** WRONG (CCreatureActionBase).
- 0x0062E900 | 00/62/CCombatActionBase_CCombatActionBase_0062e900.cpp | CCombatActionBase::~CCombatActionBase (Ego_r ??1CCombatActionBase@@UAE@XZ) --
  the temporary's implicit ~CCombatAction_WillHealRelease inlines to it; siblings model it as the temp struct's dtor @0x0062E900.
- vtable slot 1 (+4) of the def on release = CDefPointer release (header CDefPointer.h).

## 4. Members touched
- this+0x08 PCreature (CIntelligentPointer<CThingCreatureBase>, CCombatActionBase.h with FABLE_RETAIL_CINTELLIGENTPOINTER).
- this+0x114 SpellLevel (long) -- local derived class (no header).
- CThing (engine/CThing.h, retail CThing 16 bytes narrower than PDB): +0x20 UsingComponents bitset<274> (words +0x28 = types
  64..95, +0x30 = 128..159), +0x5C PTCDriver (CTCDriverBase*), +0x70 PBaseDef (CDefPointer<const CThingBaseDef>), +0x91 Dead (bit 0 of the bitfield run).
  Header offers: HasTC(ETCInterfaceType) (FABLE_FORCEINLINE via FABLE_RETAIL_CTHING_HASTC_FORCEINLINE, used for in-place tests),
  IsDead()/IsAlive(), forceinline `PeekTCDriver<T>(T**) const` (always), plain-inline `GetTCDriver<T>(T**)` under
  FABLE_RETAIL_CTHING_GETTCDRIVER (Ego_r ??$GetTCDriver@VCTCDParticleEmitter@@ ... out-of-line copy 0x004ACB50 -- inlined here),
  GetPBaseDef()/HasBaseDef() under FABLE_RETAIL_CTHING_HASBASEDEF, FABLE_RETAIL_CTHING (GetPos etc).
- CDefBase refcount +0x04, Destroy slot 1: engine/CDefPointer.h (FABLE_RETAIL_CDEFPOINTER_RELEASE_*, _RESET, FABLE_RETAIL_DEF_CLASS_BASE).
- ETCInterfaceType.h: TCI_PARTICLE_EMITTER = 75 (0x4B), TCI_HEAL_LIFE = 153 (0x99). CTCDParticleEmitter.h StatGetInterfaceType()=75.
- Draft models LOCALLY although shared headers exist: CCharString, CDefPointer, CDefClassBase, CSpecialEffectsDef,
  CTCDParticleEmitter, CThing, CThingCreatureBase, CBaseIntelligentPointer/CIntelligentPointer (CCreaturePointer), CCombatActionBase, C3DVector.

## 5. Callee visibility (Q32 lesson) -- the most likely fix for the residue
- engine/CDefClassBase_GetSubDef.h has FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE: a VISIBLE __declspec(noinline) body;
  its comment says exactly: "The visible body tells VC7.1 the callee only writes through the out pointer, so the caller keeps the def
  it reads back in a register". Our residue is the reload of `effects` into esi -> try a visible same-TU GetSubDef body.
  Every landed neighbour does this: 0x008C0FB0 JackFloatUp::PerformAction, 0x008C1F70 BriarRoseInvulnerableTeleport::Initialise,
  0x008C9A40 AttachEffectToStaff, 0x008C4B80 ScorpionKing::Initialise -- each defines GetSubDef (and usually GetEffectFromString)
  as `#pragma auto_inline(off)` + `__declspec(noinline)` bodies in the TU. NOTES says a "GetSubDef stand-in" was tried; verify
  it was a body that only writes `*out` (Find_... stand-in like 0x008C0FB0), AND try GetEffectFromString visible too.
- CSpecialEffectsDef.h: FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY (visible GetEffectFromString body).
- 0x004ACB50 GetTCDriver<CTCDParticleEmitter> is out of line elsewhere but INLINED here (in-place `test ch,8` + `mov esi,[eax+5Ch]`).
- GetPItem 0x00A01B50 and CCombatActionBase::FinishAction are out-of-line calls. ~CCombatAction_WillHealRelease inlined to ~CCombatActionBase call.

## 6. Sibling landed rows (same TU neighbourhood 0x008C2xxx)
- 0x008C29F0 CCombatActionWillShieldInto_FinishAction: no pragmas/flags; FABLE_RETAIL_CINTELLIGENTPOINTER + engine/CIntelligentPointer.h;
  local CCombatActionBase {vptr; pad; CBaseIntelligentPointer PCreature +8; pad to 0x114; void FinishAction();}; creature fetched
  into a named local before the base call; temporary struct `_Retail` with `unsigned char _body[size]`, ctor, dtor @0x0062E900 and
  `operator const CCreatureActionBase&()`; SetCurrentAction(const CCreatureActionBase&).
- 0x008C2A80 CCombatActionWillEnflameInto_FinishAction: identical idiom (temp 0x11c bytes).
- 0x008C2980 CCombatActionWillForcePushInto_FinishAction (54 lines), 0x008C2810 StrikeResponseMazeDoForcePush::FinishAction, 0x008C2550 JackOfBladesIntoRush::FinishAction.
- 0x008C0FB0 CCreatureActionJackFloatUp_PerformAction: closest body (GetSubDef<CSpecialEffectsDef> via CDefClassReference
  temp, GetEffectFromString, Create, Dead test, HasTC(0x4b) forceinline, PTCDriver, AttachToThing(*creature, ...)).
- Same class: 0x008C0560 CCombatActionWillHealInto_Initialise; 0x008C3930 WillHealRelease ctor.

## 7. Old-draft model errors vs disassembly
- GetSubDef declared only (invisible) -> likely cause of `mov eax ; mov esi,eax` residue (see 5).
- Callee spellings wrong (GetPosition, CreateEffectThing, CCreaturePointer, pointer-vs-reference params, AddTC return type,
  SetCurrentAction param type, GetSubDef ref/const) -- byte-neutral mostly but must be fixed for naming rules.
- CDefPointer modelled as non-const T; PDB is CDefPointer<const CSpecialEffectsDef>.
- `effect->PParticleEmitter` -> CThing::PTCDriver (+0x5C) via GetTCDriver/PeekTCDriver<CTCDParticleEmitter>.
- Control flow / frame / slots otherwise already match per NOTES (block-scoped effectType fixed slot sharing).

## 0x005F11A9

# DOSSIER 0x005F11A9 CTCInventoryAbilities::UnAssignAbility (375 B)

## 1. Retail shape (xdis 005f11a9 0x187)
- __thiscall, `ret 4` (one arg: unsigned long slot). Frame pointer: `push ebp; mov ebp,esp; sub esp,0xc; push ebx/esi/edi`, `leave`.
  No SEH prologue (no /EHsc state; counted-pointer/CDeletion dtors called plainly).
- Frame: [ebp-4] = address-taken copy of the removed ability (passed &[ebp-4] to map::operator[] twice);
  [ebp-0xc..-8] = CCountedPointer icon (8 B), later reused for the CDeletion (Method at -0xc, list at -8);
  [ebp+8] (slot param slot, dead after `edi=slot*4`) = CCharString temp, later holds &UnassignedIcons (esi saved there).
- Size idioms (optimize "s"): `and [mem],0` zero-stores, `push ecx; mov ecx,esp` in-place by-value args, loop recomputes size.
- Loop: `size==0` pre-test (sar; je), body `cmp [eax],edx; je SKIP` (jumps straight past the `ability!=0` test and the icon block),
  index ecx++ / pointer eax+=4, re-reads begin/finish each iteration. IDENTICAL to landed sibling 0x005F1320 (lines 0x5f1348..0x5f1383).
- BEST TEMPLATE: landed rebuild/src/compiled/00/5f/CTCInventoryAbilities_UnAssignAbilityPC_005f1320.cpp (same TU, next function,
  same body structure minus the head branch and plus a container tail) and ..._AssignAbility_005f0c5f.cpp.

## 2. Name / signature (manifest label is CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @0x0057fe3e) and FableWin.pdb (_fablewin_api.tsv / naming_stragglers signatures @0x01ec8104):
  `?UnAssignAbility@CTCInventoryAbilities@@QAEXK@Z` -> public, non-const, void UnAssignAbility(unsigned long).
- Already declared in rebuild/include/engine/CTCInventoryAbilities.h under FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN:
  `void UnAssignAbility(unsigned long slot); // 0x005F11A9`.
- Retail address order: AssignAbility 0x5F0C5F, AssignAbilityPC 0x5F0DC5, UnAssignAbility 0x5F11A9, UnAssignAbilityPC 0x5F1320 (one TU).
- Only caller: 0x00420AC5 (`call 5f11a9; jmp 426dfc`, a switch-case stub inside an unmanifested function); no vtable slot.

## 3. Callees (landed file / landed name / PDB) -- draft spelling disagreements flagged
- 0x0099EBF0 CCharString(const char*, long) -- 00/99/CCharString_Constructor_0099ebf0.cpp (string 0x01256608 "UI_UNASSIGNED_SPELL_ICON", same as AssignAbility). Built in place from `"..."` literal via FABLE_RETAIL_CCHARSTRING.
- 0x0099EAE0 ~CCharString -- 00/99/CCharString_CCharString_0099eae0.cpp.
- 0x0041E5F2 NUISystem::CManager::GetInstance -- 00/41/NUISystemCManager_GetInstance_0041e5f2.cpp.
- 0x0041DB1D NUISystem::CManager::CreateComponent(const CCharString&, bool) -- ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z.
- 0x00429C15 CCountedPointer<NUISystem::CComponent>::CCountedPointer(CComponent*).
- 0x004291DE CCountedPointer<CComponent>::ReleaseData (?ReleaseData@...QAEXXZ) -- the dtor inlines to this call; opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA. DRAFT WRONG: declares it as ~CCountedPointer.
- 0x005F9EA5 std::map<EHeroAbility,NUISystem::CComponent*>::operator[](const EHeroAbility&) -- 00/5f/stdmap_operator_005f9ea5.cpp.
- 0x005F8444 std::_Rb_tree::_M_find (the out-of-line body behind map::find; returns node ptr) -- 00/5f/stdRbtree_Mfind_005f8444.cpp. DRAFT: models `find` returning a node; use real `UnassignedIcons.find(...)` iterator + `!= end()`.
- 0x0057926B _Rb_tree::erase(iterator) -- landed as 00/57/global_ListNodeRemoveByData_0057926b.cpp under made-up name `MapEraseState::ListNode_RemoveByData`; siblings reach it via FABLE_RETAIL_STLP_TREE_ERASE (`UnassignedIcons.erase(found)`). Possible NAME_FIXES candidate (landed name is behavioural, not PDB).
- 0x0042BED4 NUISystem::CDeletion::CDeletion() (??0CDeletion@NUISystem@@QAE@XZ), 0x0042CD84 CDeletion(const CDeletion&), 0x0042ABCA std::list<CComponent*>::~list (CDeletion's dtor body). DRAFT WRONG: `SActionParamPair` pair<ulong,list> -> real type NUISystem::CDeletion { EDeletionMethod Method; list }, value 1 = CDeletion::UI_DELETE_THIS_INSTANCE.
- vslot 3 (+0x0C) NUISystem::CComponent::FindChild(CCharString) -- correct in draft.
- vslot 60 (+0xF0) CComponent::AddChildDynamically(CCountedPointer<CComponent>&). DRAFT WRONG: `SetContents_slot60(const CCountedPointer&)`.
- vslot 62 (+0xF8) CComponent::SetDeletion(NUISystem::CDeletion) by value. DRAFT WRONG: `SetActionParameter_slot62`.
  (rebuild/include/engine/NUISystem_CComponent.h lines 72/129/131.)

## 4. Members (retail offsets; header rebuild/include/engine/CTCInventoryAbilities.h, Ego_r names from struct_layouts_egor.tsv)
- +0x17C std::vector<EHeroAbility> AssignedAbilities (PDB +0x1A4) -- reads begin/finish (+0x180).
- +0x188 std::vector<EHeroAbility> AssignedAbilitiesSave (PDB +0x1B4). DRAFT WRONG name: PreviousAbilities.
- +0x1A0 std::map<EHeroAbility,CComponent*> UnassignedIcons (PDB +0x1D4). DRAFT WRONG: AbilityIcons.
- +0x1AC std::map<EHeroAbility,CComponent*> AbilitiesMenuEntries (PDB +0x1E4). DRAFT WRONG: AbilityContainers.
- Maps are real std::map only under FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC (else byte arrays).
- Opt-ins to copy verbatim from siblings: FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA, FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC,
  _ASSIGN, FABLE_RETAIL_CDELETION, FABLE_RETAIL_STLP_TREE_ERASE, FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCINVENTORYBASE,
  FABLE_RETAIL_CTCINVENTORYABILITIES, FABLE_RETAIL_CCHARSTRING; includes CTCInventoryAbilities.h, NUISystem_CManager.h,
  NUISystem_CComponent.h, NUISystem_CDeletion.h. CCountedPointer::Get() exists (CCountedPointer.h:154).
- Draft models LOCALLY (all have shared headers -> strict verifier rejects): CCharString, CCountedPointer, NUISystem::CComponent,
  NUISystem::CManager, CDeletion (as SActionParamPair), list, EHeroAbility, map (CAbilityMap), vector, CTCInventoryAbilities.

## 5. Callee visibility
- vector operator[]/size inline (STLport header). map::operator[] 0x5F9EA5 and _M_find 0x5F8444 are COMDATs at unaligned
  addresses emitted out of line in this TU neighbourhood; map::find inlines to the _M_find call; erase(iterator) out of line
  (FABLE_RETAIL_STLP_TREE_ERASE). CCountedPointer dtor visible inline -> ReleaseData call. CDeletion ctor/copy/dtor out of line
  (FABLE_RETAIL_CDELETION). Siblings 0x5F0C5F and 0x5F1320 already prove this visibility set byte-exact.

## 6. Sibling landed rows (rebuild/src/compiled/00/5f/)
- CTCInventoryAbilities_UnAssignAbilityPC_005f1320.cpp: `#pragma optimize("s", on)` only (inside #ifdef _MSC_VER), no Flags line,
  opt-ins as listed in 4. Head: `EHeroAbility ability = AssignedAbilities[slot]; ...=0; ...=0; EHeroAbility removed = ability;`
  then `bool stillAssigned=false; for(i<size) if(AssignedAbilities[i]==ability){stillAssigned=true;break;}`
  `if(!stillAssigned && ability!=0){ CCountedPointer<CComponent> icon(CManager::GetInstance()->CreateComponent("..",false));
   AbilitiesMenuEntries[removed]->AddChildDynamically(icon); UnassignedIcons[removed]=icon.Get(); }`
  `if(AssignedAbilities[slot]!=0){ iterator found=UnassignedIcons.find(AssignedAbilities[slot]); if(found!=UnassignedIcons.end()){
   CComponent* icon=found->second->FindChild(".."); if(icon){CDeletion deletion; deletion.Method=CDeletion::UI_DELETE_THIS_INSTANCE;
   icon->SetDeletion(deletion);} UnassignedIcons.erase(found);} }`  -- this tail is byte-for-byte the shape of 0x5F11A9's tail
   (0x5F129B..0x5F1319), only the string differs ("UI_UNASSIGNED_SPELL_ICON").
- CTCInventoryAbilities_AssignAbility_005f0c5f.cpp: same pragmas/opt-ins, same string.
- CTCInventoryAbilities_AssignAbilityPC_005f0dc5.cpp: same + STLP_LIST / CACTIONPARAM* opt-ins.
- CTCInventoryAbilities_AssignAbilitySwappingPC_005f4519.cpp and _SetAbilityToButton_005f6423.cpp use `#pragma optimize("y", off)` + ("s", on);
  CycleAssignedAbilities_005f64dd uses `// Flags: /O1 /Oy-`. The UnAssign/Assign neighbours matched with ("s", on) alone -> copy those.

## 7. Old draft model errors vs disassembly
- Wrong names everywhere (members, vslots, CDeletion, ReleaseData) and all-local types -> rewrite on the shared header.
- Head: draft keeps `previous` local and an IsAbilityAssigned forceinline helper. Retail: loads A=Assigned[slot] (ecx) and
  S=Save[slot] (eax), cmp, `mov edx,ecx; mov [ebp-4],edx` (address-taken copy homed at ebp-4), then
  if != : re-reads Assigned begin, stores eax (S); else `and Assigned[slot],0; and Save[slot],0` (each begin re-read).
  Suggested spelling (PC sibling idiom): `EHeroAbility ability = AssignedAbilities[slot];
  if (ability != AssignedAbilitiesSave[slot]) AssignedAbilities[slot] = AssignedAbilitiesSave[slot]; else { AssignedAbilities[slot]=(EHeroAbility)0; AssignedAbilitiesSave[slot]=(EHeroAbility)0; }
  EHeroAbility removed = ability;` (removed = the [ebp-4] home used by operator[]; ability stays in edx for loop and test).
  Variant if Save is reloaded: `EHeroAbility saved = AssignedAbilitiesSave[slot]; if (ability != saved) AssignedAbilities[slot] = saved;`
  The draft's residue (slot word [ebp+8] used as home) came from homing the original variable rather than a separate `removed` copy.
- Loop: use the sibling's inline bool/break loop, not a helper; `ability != 0` tested after loop (retail `test edx,edx`).
- Tail: find on `AssignedAbilities[slot]` element reference (retail pushes &element, edi), compare to end(), erase(found) by value.

## 0x006978D0

# DOSSIER 0x006978D0 CGameCameraManager::GetInterpolatedCamera (376 bytes)

## 1. Retail shape (xdis 006978d0 0x188)
- __thiscall, `ret 10h` = hidden CCamera* return + 3 dwords: [entry+4]=ret ptr, +8 = float interp, +0xC = const CCamera& (copied, "new"), +0x10 = const CCamera& ("old").
- No frame pointer (sub esp,68h; push esi; push edi). No SEH. Plain /O2 (no /Oy-, no optimize("s") idioms).
- esi = arg3, edi = arg4. `this` (ecx) is never read.
- Flow: local CCamera copy-constructed from arg3 (call 0x49D380, local at entry-0x38) -> arg3.GetFOV() (0xA0BEB0) spilled into the dead arg3 slot,
  arg4.GetFOV() -> SetFOV(old + (new-old)*t, push 0) -> WorldPosition X,Y,Z written straight into the local camera (+0,+4,+8; each through
  `fstp tmp ; mov eax,tmp ; fstp st(0) ; mov [cam+N],eax`) -> Up vector temp (entry-0x5c) computed FIRST, then Forward temp (entry-0x50) ->
  SetView(&forward,&up) (0xA0BE30; `push &up` first, ecx=local) -> copy ctor 0x49D380(result=[entry+4], &local) -> return result. NO NRVO.
- Every blend is `fld OLD ; fld NEW ; fsub st(1) ; fmul t ; fadd st(1) ; fstp ; fstp st(0)` = (new-old)*t+old with OLD = arg4, NEW = arg3.
  This is EXACTLY GFGetInterpolatedValue(old_val,new_val,interp) = (new_val-old_val)*interp+old_val (engine/GFGetInterpolatedValue.h,
  FableWin.pdb ?GFGetInterpolatedValue@@YIMMMM@Z 0x01D8E920, inline fastcall, by-value floats). Same x87 pattern in the landed sibling
  CGameCameraBase::GetRenderCamera 0x006FD5E0 (fld [esi+3Ch]=Last ; fld [esi+4]=Internal ; fsub st(1) ; fmul ; fadd st(1)).
- FOV ordering: new.GetFOV() called before old.GetFOV() = right-to-left arg evaluation of GFGetInterpolatedValue(old.GetFOV(), new.GetFOV(), t).
- Up/Forward: some operand dwords are pre-copied to frame locals through int regs (to.Up.Z [edi+14h], from.Up.Y [esi+10h], from.Up.Z [esi+14h],
  from.Fwd.Y [esi+1Ch], from.Fwd.Z [esi+20h], to.Fwd.Z [edi+20h]) - the by-value float args of the inlined GFGetInterpolatedValue being materialised;
  the X and Y components use `fld old.X ; fld old.Y` kept on the x87 stack (fsub st(2) / fadd st(2)). Up evaluated before Forward = right-to-left
  evaluation of SetView(forward_expr, up_expr) args (or up declared first).

## 2. Name / signature (two sources)
- Ego_r.pdb egor_pdb_names.tsv 0x006BE4E0 and FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv 0x02202AE0, 553 bytes debug):
  `?GetInterpolatedCamera@CGameCameraManager@@ABE?AVCCamera@@MABV2@0@Z` = PRIVATE (A), const (B), thiscall:
  `CCamera CGameCameraManager::GetInterpolatedCamera(float, const CCamera&, const CCamera&) const`. Manifest label is CORRECT.
- Caller: only 0x00697F43 inside CGameCameraManager::GetRenderCamera 0x00697F30 (Ego_r ?GetRenderCamera@CGameCameraManager@@UBE?AVCCamera@@M@Z 0x006BECB0):
  pushes OldCamera (this+0x3C) last arg, Camera (this+4) middle, interp first: GetInterpolatedCamera(interpolate, Camera, OldCamera).
  So param 2 = the current (new) camera, param 3 = the old camera; result = new-copy with values interpolated old->new by t.
  Suggested param names (descriptive, mirror sibling): `const float interpolate`, `const CCamera& camera`, `const CCamera& old_camera`.
- NAME FIX: landed caller rebuild/src/compiled/00/69/CGameCameraManager_GetRenderCamera_00697f30.cpp calls it `Helper6978d0` with local `CameraState`
  stand-ins -> should be GetInterpolatedCamera (NAME_FIXES.tsv row).
- vtable: not virtual (private non-virtual), no slot.

## 3. Callees
| retail | PDB name | landed row | draft spelling |
|---|---|---|---|
| 0x0049D380 | ??0CCamera@@QAE@ABV0@@Z (copy ctor, Ego_r 0x00429100) | rebuild/src/compiled/00/49/CCamera_CCamera_0049d380.cpp | ok |
| 0x00A0BEB0 | ?GetFOV@CCamera@@QBEMXZ (Ego_r 0x00B4C9A0) | rebuild/src/compiled/00/a0/CCamera_GetFOV_00a0beb0.cpp (byte-identical to GetHorizontalFOV 0x00A0BE90; told apart by PDB order) | WRONG: draft says GetHorizontalFOV (that is 0x00A0BE90). Must be GetFOV. |
| 0x00A0BED0 | ?SetFOV@CCamera@@QAEXMM@Z | rebuild/src/compiled/00/a0/CCamera_SetFOV_00a0bed0.cpp | ok (second arg 0.0f; retail `push 0`) |
| 0x00A0BE30 | ?SetView@CCamera@@QAEXABVC3DVector@@0@Z (forward, up) | not landed | ok |
| inline | GFGetInterpolatedValue (FableWin 0x01D8E920, no Ego_r copy) | engine/GFGetInterpolatedValue.h | draft invented `Blend` |

## 4. Members (all via CCamera; struct_layouts_egor.tsv CCamera 56 bytes)
- WorldPosition C3DVector +0, RHSet CRightHandedSet +0xC {Up +0xC, Forward +0x18}, HorizontalFOV +0x2C (via GetFOV). All PRIVATE in
  rebuild/include/engine/CCamera.h -> a CGameCameraManager member can only use accessors:
  - `PeekWorldPos()` (always), `PeekViewDirection()` (always, = RHSet.GetForward()), `PeekUpDirection()` under FABLE_RETAIL_CCAMERA_UP_DIRECTION
    (needs FABLE_RETAIL_CRIGHTHANDEDSET for GetUp()).
  - Copy ctor declared + `MoveTo(const C3DVector&){WorldPosition=p;}` + PointAt under FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION (needed: without
    a declared copy ctor VC inlines the implicit copy instead of calling 0x49D380).
  - SetView under FABLE_RETAIL_CCAMERA_SETVIEW.
  - NO opt-in yet declares CCamera::GetFOV() const or SetFOV(float,float): the author must add one (e.g. FABLE_RETAIL_CCAMERA_FOV, declarations only).
- CGameCameraManager.h: Camera +4 / OldCamera +0x3C (only the caller touches them). The header has no GetInterpolatedCamera declaration: add a private
  declaration opt-in. Default branch of the header is the flattened `#pragma pack(1)` layout; the LOOKAROUND branch (used by sibling 0x697CC0) is the
  full original class (derives CAIGameCameraBase).
- Old draft models LOCALLY: C3DVector, CRightHandedSet, CCamera, CGameCameraManager - all four have shared headers (engine/C3DVector.h,
  engine/CRightHandedSet.h, engine/CCamera.h, engine/CGameCameraManager.h).
- C3DVector.h also offers `Interpolate(v,t)` = *this+(v-*this)*t under FABLE_RETAIL_CPARTICLE_SPRITE (particle opt-in; vector-op form, not the retail
  per-component GFGetInterpolatedValue pattern - try only if the scalar form fails).

## 5. Callee visibility
- GetFOV / SetFOV / SetView / copy ctor live in the CCamera TU (0x00A0BE30.., 0x0049D380 is the copy-ctor COMDAT) - NOT visible bodies; declare only.
- GFGetInterpolatedValue: inline (header body), must be visible.
- PeekWorldPos/PeekViewDirection/PeekUpDirection/MoveTo: inline accessors (FableWin-only, no Ego_r copies), visible.
- Same TU neighbours (CGameCameraManager source): 0x00697700 GetControlAngleXY, 0x006977F0 GetViewWidthAtPosition, 0x00697AB0.., 0x00697CC0
  UpdateRightStickLookAround, 0x00697F30 GetRenderCamera (defined AFTER this one). No STLport calls.

## 6. Sibling landed rows
- rebuild/src/compiled/00/6f/CGameCameraBase_GetRenderCamera_006fd5e0.cpp (THE template: same idiom; defines FABLE_RETAIL_CGAMECAMERABASE_RENDER,
  FABLE_RETAIL_CAIGAMECAMERABASE, FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION, FABLE_RETAIL_C3DVECTOR; includes engine/GFGetInterpolatedValue.h;
  `CCamera passcam(InternalCam); C3DVector vecman; vecman.X=GFGetInterpolatedValue(Last...X, ...X, interpolate); ... passcam.MoveTo(vecman); ... return passcam;`).
- rebuild/src/compiled/00/69/CGameCameraManager_UpdateRightStickLookAround_00697cc0.cpp (Flags: none; FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND,
  FABLE_RETAIL_CAIGAMECAMERABASE, FABLE_RETAIL_CRIGHTHANDEDSET, FABLE_RETAIL_C3DVECTOR, FABLE_RETAIL_CCAMERA_SETVIEW, FABLE_RETAIL_CCAMERA_UP_DIRECTION,
  ...; uses Camera.PeekViewDirection()/PeekUpDirection()/SetView(forward,up)).
- rebuild/src/compiled/00/69/CGameCameraManager_GetCameraModeType_00697ab0.cpp (FABLE_RETAIL_CGAMECAMERAMANAGER).
- rebuild/src/compiled/00/69/CGameCameraManager_IsPosCameraInsideThing_00697490.cpp (FABLE_RETAIL_CRIGHTHANDEDSET, FABLE_RETAIL_C3DVECTOR, ..._INSIDETHING).
- rebuild/src/compiled/00/a0/CCamera_SetFOV_00a0bed0.cpp / CCamera_GetFOV_00a0beb0.cpp (local CCamera models, callees).
- No pragmas / no flags in any of these.

## 7. Old draft model errors
- Invented `Blend` helpers instead of GFGetInterpolatedValue(old,new,t): the operand order matters for x87 (old kept in st(1)).
  Draft's `to + (from - to)*t` is algebraically right but as a __forceinline taking const refs / a C3DVector-returning helper it doesn't produce the
  by-value-arg materialisation retail shows.
- Wrong FOV callee: GetHorizontalFOV (0xA0BE90) instead of GetFOV (0xA0BEB0).
- Direct member access (camera.WorldPosition, from.RHSet.Up) on a class whose members are private in the shared header; should go through
  PeekWorldPos / PeekUpDirection / PeekViewDirection and MoveTo.
- Vector blend forms: retail builds Up then Forward, likely as per-component GFGetInterpolatedValue arguments (C3DVector(x,y,z) temporaries or
  separately named vectors) passed to SetView(forward, up); position written straight into the camera (MoveTo of an inline-built C3DVector, or a
  vecman temp like the sibling that the optimiser forwarded). Try sibling form first: `CCamera camera(new_cam); camera.SetFOV(GFGetInterpolatedValue(
  old.GetFOV(), new.GetFOV(), interpolate), 0.0f); C3DVector vecman; vecman.X=GFGet(old.PeekWorldPos().X, new.PeekWorldPos().X, interpolate)...;
  camera.MoveTo(vecman); camera.SetView(C3DVector(GFGet(...ViewDirection X..)...), C3DVector(GFGet(...UpDirection...)...)); return camera;`
- All local struct models must be replaced with the shared headers.

## 0x00EB3F40

# DOSSIER 0x00EB3F40 -- NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Main (381 bytes)

## 1. Retail shape (xdis 00eb3f40 0x18d)
- __thiscall, plain `ret` (no N). No frame pointer (sub esp,14h; push esi/edi; ebx pushed LATE at 0x00EB3F90 after the first
  IsActiveThreadTerminating early-out -> shrink-wrapped; first `jne 0xEB40B7` skips `pop ebx`). No SEH frame, no /Oy- pragma.
  Normal /O2 (loops aligned: `mov edi,edi` pad at 0x00EB404E). No size-opt idioms.
- Frame: L+0 = dead zero dword (`mov [esp+0Ch],0` at 0x00EB3F4F), L+4..L+0x13 = CIsThingAlive temporary
  (vptr 0x012C32F0 stored inline at L+4, CScriptThing copy 0x004ABE90 into L+8), SetPredicate(&L+4), ~CIsThingAlive 0x00E241B0 out of line.
- 0x00EB3FF4 `mov bl,al` after DoesAssassinTrustHero: result never read afterwards (dead), but it costs ebx.
- Control flow: NewScriptFrame; if term return; if MyEntityHandle.IsValid() Release(); while(!StartScriptingEntity(Me,MyEntityHandle,4))
  {NewScriptFrame; if term return;} (rotated: call once, then loop); if term return; trust=DoesAssassinTrustHero();
  loop A (NOT rotated: test at top 0x00EB3FF6, `jmp` back at 0x00EB4032): while(AssassinState==0||==1){NewScriptFrame; if term ret; if term ret; DoWatchingForTrouble();}
  if term ret; loop B (rotated, constant 3 hoisted to edi): while(AssassinState==3){NewScriptFrame; if term ret; DoRemindingHeroToKillSheriff();}
  if term ret; if(AssassinState==2){ if term ret; AttackHero(); } if term ret; for(;;){NewScriptFrame; if term ret;}
- TWIN: unlanded CMurderTwistAssassinGuard::Main @ 0x00EB55D0 (vtable 0x012F036C) has the SAME prologue incl. dead zero and the
  same `call 0x00EB03A0 ; mov bl,al` (0x00EB567F/0x00EB5684). So both dead values are a property of the generated-script source /
  TU, not noise. Old lane (pending/2026-10-21_linux_hand247 NOTES l.46, STATUS l.20): DIFFER 378v381 after 5 forms, exactly these 3 bytes+regmap.

## 2. Identity
- Name: `?Main@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@EAEXXZ` (private virtual void, no args, non-const).
  Sources: Ego_r egor_pdb_names.tsv l.41155; FableWin _fablewin_api.tsv / FableWin.exe.signatures (017fd1b8, 03a62d60, debug 748 B);
  bsim_port_audit_final.tsv eb3f40 -> same symbol; vtable 0x012F00BC slot 1 (xref: `.rdata 012f00c0` only ref);
  labels_anniversary_2026-11-04_waves23.tsv (reg "MurderTwistAssassin", Alloc 0x00EB3C60). Manifest label is CORRECT.
- Shared header already declares it: rebuild/include/engine/CV_MurderTwistFinalScript.h under
  `FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN` (`private: virtual void Main(); // 0x00EB3F40`).

## 3. Callees (retail addr | landed row | correct PDB name | old-draft spelling)
- 0x004ABE90 CScriptThing copy ctor | 00/4a/CScriptThing_CScriptThing_004abe90.cpp | ??0CScriptThing@@QAE@ABV0@@Z | ok (called out of line, inside inline CIsThingAlive ctor).
- 0x00F35B10 | 00/f3/CEntityScriptBase_SetPredicate_00f35b10.cpp | ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z | draft "RegisterFilter" WRONG.
- 0x00E241B0 | 00/e2/NScriptCIsThingAlive_CIsThingAlive_00e241b0.cpp | ??1CIsThingAlive@NScript@@UAE@XZ | ok (must stay out of line).
- slot 7 (+0x1C) NewScriptFrame() const 0x006E7410 (CGameScriptInterfaceBase.h l.30; PDB ..UBEXXZ const) | draft non-const.
- slot 8 (+0x20) StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority) 0x0089B5B0; arg 4 = SCRIPT_AI_PRIORITY_HIGHEST (133 landed rows define LOWEST0..HIGHEST4).
- 0x00F35B30 | 00/f3/CEntityScriptBase_IsActiveThreadTerminating_00f35b30.cpp | ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ (const) | draft "IsDone" WRONG.
- 0x00CD23B9 | 00/cd/CScriptGameResourceObjectScriptedThingBase_IsValid_00cd23b9.cpp | ?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ (const) | draft "IsScripting" WRONG.
- 0x00CD2770 | 00/cd/CMemoryDataOutputStream_Clear_00cd2770.cpp (landed under propagated WRONG name) | ?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ (Ego_r l. 00cb4574) | draft "Release" ok. -> NAME_FIXES candidate for the landed row.
- 0x00EB03A0 | 00/eb/NScriptCVMurderTwistFinalScript_DoesAssassinTrustHero_00eb03a0.cpp | ?DoesAssassinTrustHero@CV_MurderTwistFinalScript@NScript@@QAE_NXZ | ok.
- 0x00EB40C0 | 00/eb/..CMurderTwistAssassin_DoWatchingForTrouble_00eb40c0.cpp | ?DoWatchingForTrouble@CMurderTwistAssassin@...@@AAEXXZ | ok.
- 0x00EB4F80 | 00/eb/..CMurderTwistAssassin_DoRemindingHeroToKillSheriff_00eb4f80.cpp | ?DoRemindingHeroToKillSheriff@CMurderTwistAssassin@...@@AAEXXZ | ok.
- 0x00EB53A0 | 00/eb/CMurderTwistSheriff_Init_00eb53a0.cpp (landed as CMurderTwistSheriff::Init -- WRONG, propagated) |
  ?AttackHero@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@AAEXXZ: FableWin.exe.signatures 03a66470 AttackHero@CMurderTwistAssassin
  264 B hash 31995e7c == AttackHero@CMurderTwistAssassinGuard 03a68930 (identical bodies -> ICF; Ego_r keeps only the Guard symbol f20b40,
  placed among the Assassin methods). Only caller is this Main (xref 00eb4093). Header declares `void AttackHero();` (no addr). draft "DoAttackingHero" WRONG.
  -> NAME_FIXES.tsv: 00eb53a0 CMurderTwistSheriff::Init -> NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::AttackHero.

## 4. Members
- this+0x04 Game (CEntityScriptBase.h: CGameScriptInterfaceBase* Game; reference under FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS).
- this+0x08 Me (CScriptThing, CEntityScriptBase.h).
- this+0x14 ParentClass (CV_MurderTwistFinalScript*, protected, CV_MurderTwistFinalScript.h assassin opt-in; Ego_r struct_layouts l.56121).
- this+0x1C MyEntityHandle (CScriptGameResourceObjectScriptedThingBase, Ego_r l.56123).
- ParentClass+0x4C AssassinState (EAssassinState, enumerators ASSASSIN_STATE_0..3 descriptive) -- draft "State" (long) WRONG name/type.
- Opt-ins: FABLE_RETAIL_NSCRIPT, FABLE_RETAIL_CLEVELSCRIPTBASE, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_CCHARSTRING (header contract),
  FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN; optional FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS, FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS,
  FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS / _NOVTABLE (CPredicateBase.h; NOVTABLE drops base vtable store -- retail here has none in the expanded ctor).
- Locally modelled in old draft though shared headers exist: CCharString, CScriptThing, CBaseObject, CScriptGameResourceObjectScriptedThingBase,
  CGameScriptInterface, CIsThingAlive, CV_MurderTwistFinalScript, CMurderTwistAssassin (+ missing CEntityScriptBase base). All must go to
  engine/CV_MurderTwistFinalScript.h, CIsThingAlive.h, CEntityScriptBase.h, CGameScriptInterfaceBase.h.

## 5. Visibility
- CIsThingAlive ctor is inline (FableWin-only symbol; header says the row defines it after include: `inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t) : Thing(t) {}`), copy ctor 0x004ABE90 called out of line, dtor 0x00E241B0 out of line (do NOT define inline dtor body).
- Same-TU neighbours (retail order): Alloc 0x00EB3C60, GetParentScript 0x00EB3D00, OnPredicateFail 0x00EB3D10, Init 0x00EB3E40, Main, DoWatchingForTrouble 0x00EB40C0,
  IsFeelingThreatened 0x00EB4710, DoDealWithHero 0x00EB4760, DoReminding 0x00EB4F80, AttackHero 0x00EB53A0, Guard Alloc 0x00EB5450, Guard Main 0x00EB55D0.
  Callees after Main (DoWatching/DoReminding/AttackHero) are called, not inlined -- no visibility issue. DoesAssassinTrustHero 0x00EB03A0 is earlier in TU (called, 0x1xx B).
- No STLport helpers.

## 6. Siblings
- 00/eb/NScriptCVMurderTwistFinalScriptCMurderTwistAssassin_DoWatchingForTrouble_00eb40c0.cpp (RELOCATION_MATCH): no pragmas; defines
  FABLE_RETAIL_{CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR,NSCRIPT,CLEVELSCRIPTBASE,BASE_OBJECT,CCHARSTRING,DEF_CLASS_BASE,CDEFSTRINGTABLE,CV_MURDERTWISTFINALSCRIPT_ASSASSIN},
  includes engine/CV_MurderTwistFinalScript.h; generated-script idiom `TRetVoid _Fail_Val; ... if (IsActiveThreadTerminating()) return _Fail_Val.Return();` (engine/TRetVoid.h, sizeof 1).
- 00/d8/NScriptCQHeroSoulsNostroScriptCUndeadSoldier_Main_00d88f90.cpp: same entity-Main skeleton, scoped named filter `{CIsThingAlive filter(Me); SetPredicate(filter);}` -- its retail has NO dead zero.
- 00/e9/..CMHiddenBooty_Main_00e93120.cpp, 00/ed/..CSwordOfStrength_Main_00ed54d0.cpp: retail HAS a dead zero dword; both use temporary form `SetPredicate(CIsThingAlive(Me));`
  (but survey of 60 landed alive-Mains: temp form usually gives no zero -> the zero is not explained by the temp form alone; in e93120 it is a cond-dtor flag word).
- 00/ec/..CWomanToAttract_Main_00ecf7f0.cpp: dead zero too; uses ORIGINAL_MEMBERS/ORIGINAL_ACCESS/CPREDICATEBASE_NOVTABLE/CSCRIPTTHING_IMPLICIT_COPY opt-ins.
- 00/eb/..CMurderTwistAssassin_Init_00eb3e40.cpp, CMurderTwistAssassin_OnPredicateFail_00eb3d10.cpp: older local-model style.

## 7. Old draft model errors
- Fake `long unused = 0;` for the dead zero; untested idea: zero may come from a value-returning temp / ORIGINAL_MEMBERS models
  (e.g. CPredicateBase : std::unary_function base adds nothing; try FABLE_RETAIL_*_ORIGINAL_* set as in 0x00ECF7F0, TRetVoid _Fail_Val local,
  temporary-form SetPredicate). Copy whatever eventually works for twin 0x00EB55D0.
- `bool trusts` dead: keep it as a named local (generated code); retail keeps `mov bl,al`.
- Wrong names: RegisterFilter/IsDone/IsScripting/DoAttackingHero/State (see 3/4); Game typed CGameScriptInterface* instead of CGameScriptInterfaceBase*;
  NewScriptFrame non-const; class not derived from CEntityScriptBase; Main declared public non-virtual-in-base.
- Control flow of draft matches retail (loop A head-tested, double term check, loop B, if==2, final for(;;)).

## 0x008D5300

# DOSSIER 0x008D5300 -- CCreatureAction_ReloadMissileWeapon::PerformAction (383 bytes)

## 1. Retail shape (xdis 008d5300 0x18f)
- __thiscall virtual, plain `ret` (no args). No frame pointer (`sub esp,14h` first; no push ebp/mov ebp,esp) -> no /Oy- pragma.
  No SEH prologue. Not size-optimised (long-form jcc, no `push 46h/pop` idioms).
- `this` spilled to [esp+4] at entry (`mov [esp+4],ecx`); PCreature (this+8) is RE-FORMED from the spill for every GetPItem
  (0x008D53AF, 0x008D53BB). Callee-saved regs pushed lazily (shrink-wrapped): esi at 0x8D5319, ebx at 0x8D533F, ebp+edi at
  0x8D5361 (inside the GetSubDef branch). ebx = carrying for the whole body; esi = &PAmmo (this+0xA8) after Create;
  ebp = slot (MissileLoadCarrySlot), edi = ammo def (Ammo) then the existing thing.
- Frame (0x14): [esp+4] this spill; [esp+0xC]-ish shared word = GetTC key (0x46) then CDefPointer<const CWeaponDef> (=0 store at
  0x8D536A); [esp+0x18] CCharString(""); [esp+0x1C] CIntelligentPointer<CThingObject> temporary; [esp+0x18] reused for `existing`.
- Control flow: creature = PCreature.GetPItem(); HasTC(0x46) (`test byte [eax+28h],40h`) -> expanded GetTC (key store AFTER test,
  LowerBound 0x40F020, end-or-greater ternary, component straight into ebx) -> GetThingInPrimarySlot -> null test -> HasTC(0x48)
  (`test ch,1` on dword [eax+28h]) -> CDefPointer=0 -> PBaseDef(+0x70)->GetSubDef(&def) -> `ammo=def->Ammo(+0xA4); slot=def->MissileLoadCarrySlot(+0x94)`
  -> `if (ammo>0 && slot>0)` (jle tests: edi first, then ebp) -> PAmmo = Create(...) -> IS_THING_ALIVE(PAmmo) out of line ->
  existing = GetThingInCarrySlot(slot); if IS_THING_ALIVE(existing) {RemoveThing(*existing); existing->Kill(true);} ->
  PutThingInCarrySlot(*PAmmo, slot, true). Def release at the join: `test ecx,ecx; je; dec [ecx+4]; jne; mov eax,[ecx]; call [eax+4]`
  (Object=0 store dead-eliminated) = default in-place CDefPointer::Release.
- Create call order (0x8D539F..0x8D53E5): CCharString("" 0x0122D70E,-1) built FIRST; GetPItem->esi (player: `movsx ecx,byte [esi+90h]`
  = GetPlayerNumber()); GetPItem->eax; push &name,0,0,player; ecx=eax -> GetPos() 0x4C73D0; edx=pos, ecx=edi(ammo) -> Create.
  i.e. right-to-left arg evaluation of `CThingObject::Create(Ammo, PCreature->GetPos(), PCreature->GetPlayerNumber(), 0, 0, name)`.
  Then ctor CIntelligentPointer<CThingObject>(CThingObject*) 0x65E133 on temp [esp+0x1C], operator= 0x7B82C0 (ecx=&PAmmo, push &temp),
  ~CBaseIntelligentPointer 0xA01C10 on temp, then ~CCharString 0x99EAE0. (Same order as a block-scoped `CCharString name("")`.)

## 2. Identity
- Name CORRECT: ?PerformAction@CCreatureAction_ReloadMissileWeapon@@UAEXXZ (`virtual void PerformAction()`, no params, non-const).
  Source 1: rebuild/manifest/rtti-vtables.tsv line 9621: vtable 0x012656A4 slot 3 = 0x008D5300 (slot 4 = 0x008D5480 FinishAction, landed).
  Source 2: ghidra_out/egor_pdb_names.tsv (Ego_r 0x0090C170, next to FinishAction 0x0090C310) + _fablewin_api.tsv (0x02AD9F80 / FinishAction 0x02ADA2C0).
- No direct callers (xref: only the vtable .rdata 0x012656B0 = 0x012656A4+0xC).
- Class layout: Ego_r CCreatureAction_ReloadMissileWeapon (188 bytes) has one own member `CIntelligentPointer<CThingObject> PAmmo` (Ego_r +180;
  retail +0xA8 since retail CCreatureActionBase is 0xA8). The draft's CIntelligentPointer<const CThing> is WRONG: pointee is CThingObject.

## 3. Callees (landed file / landed name / PDB name)
| addr | landed row | landed name | PDB / note |
|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const -> CBaseObject* | same. Draft returns CThing* (cast needed in real headers; use PCreature-> / Get()). |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound (non-const) | via CThing::GetTC expanded. Draft's local CInterfaceMap + its own out-of-line LowerBound body is a local stand-in: use engine/CVectorMap.h. |
| 0x006A1440 | 00/6a/CTCCarrying_GetThingInPrimarySlot_006a1440.cpp | CTCCarrying::GetThingInPrimarySlot() const | ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ -- draft omits const. |
| 0x0042B1A6 | 00/42/CDefClassBase_GetSubDefCWeaponDef_0042b1a6.cpp | CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<const CWeaponDef>*) const | Ego_r ??$GetSubDef@..QBE_NPAV?$CDefPointer@$$CBV..; draft: non-const, CDefPointer<CWeaponDef> (no const) -- WRONG. Unaligned COMDAT => out of line. |
| 0x0099EBF0 / 0x0099EAE0 | CCharString_Constructor_0099ebf0 / CCharString_CCharString_0099eae0 | CCharString(const char*, long=-1) / ~CCharString | OK. |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const (7 bytes, `mov eax,[ecx+60h]; add eax,0Ch; ret`) | ?GetPos@CThing@@QBEABVC3DVector@@XZ. Draft's 4-arg `GetNamedPosition(player,0,0,name)` is WRONG (and NOTES.md line 56 of hand243 is wrong): those 4 pushes are Create's stack args. |
| 0x00703210 | 00/70/CThingObject_Create_00703210.cpp | static __fastcall CThingObject::Create(long defIndex, const C3DVector& position, long playerNumber, const CRightHandedSet* rhSet, unsigned long flags, const CCharString& scriptName), ret 0x10 | ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z. Draft: 2-param `CThing::Create(long,const C3DVector*)` -- WRONG class, params, return type. Header: engine/CThingObject.h under FABLE_RETAIL_CTHINGOBJECT_CREATE. |
| 0x0065E133 | 00/65/CIntelligentPointer_CThingObject_CIntelligentPointer_0065e133.cpp | CIntelligentPointer<CThingObject>::CIntelligentPointer(T*) | FableWin ??0?$CIntelligentPointer@VCThingObject@@@@QAE@PAVCThingObject@@@Z family; draft spells it CIntelligentPointer<const CThing> -- WRONG pointee. Out of line (unaligned COMDAT). |
| 0x007B82C0 | 00/7b/CIntelligentPointerCThingCreatureBase_operator_007b82c0.cpp | CIntelligentPointer<T>::operator= ("pointee unproven", 24 identical bodies) | here T = CThingObject (operator=(const CIntelligentPointer<CThingObject>&)); landed file name says CThingCreatureBase -- candidate NAME_FIXES note only (shared body). |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | ~CBaseIntelligentPointer | OK. |
| 0x0078B2A0 | 00/78/global_IsThingAlive0078b2a0_0078b2a0.cpp | made-up `IsThingAlive_0078b2a0` (claims CIntelligentPointer<CThing>) | This call proves it is ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingObject@@@@@@YI_NABV?$CIntelligentPointer@VCThingObject@@@@@Z (FableWin _fablewin_api.tsv 0x018528F7 / 0x01DE6D80): PAmmo is CIntelligentPointer<CThingObject> (Ego_r layout) and COMDATs are one-per-name. -> NAME_FIXES candidate. |
| 0x006A11F0 | 00/6a/CTCCarrying_GetThingInCarrySlot_006a11f0.cpp | CTCCarrying::GetThingInCarrySlot(long) const | ?GetThingInCarrySlot@CTCCarrying@@QBEPAVCThing@@J@Z; declared in engine/CTCCarrying.h line 55. Draft omits const. |
| 0x0040DCE0 | 00/40/global_ISTHINGALIVECThingBuilding_0040dce0.cpp | IS_THING_ALIVE<CThing*>(CThing* const&) | ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z. OK. |
| 0x006A19B0 | 00/6a/CTCCarrying_RemoveThing_006a19b0.cpp | landed "RemoveThing(CThing* thing)" | PDB ?RemoveThing@CTCCarrying@@QAEXAAVCThing@@@Z = CThing& (by reference). Draft uses CThing* -- WRONG spelling. Header: CTCCarrying.h FABLE_RETAIL_CTCCARRYING_REMOVETHING. |
| 0x004C9B80 | 00/4c/CThing_Kill_004c9b80.cpp | CThing::Kill(bool) | ?Kill@CThing@@QAEX_N@Z. OK. |
| 0x006A1F80 | not landed (functions.tsv: PutThingInCarrySlot, accepted) | -- | ?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z = (CThing&, long, bool). Draft uses CThing* -- WRONG. Header: CTCCarrying.h FABLE_RETAIL_CTCCARRYING_PUTTHING. |
| [vptr+4] | -- | CDefPointee Delete (def release) | via CDefPointer dtor (engine/CDefPointer.h). |

## 4. Members
- this+0x08 PCreature: CCreatureActionBase (engine/CCreatureActionBase.h), Ego_r `CIntelligentPointer<CThingCreatureBase> PCreature`.
  Opt-ins: FABLE_RETAIL_CCREATUREACTIONBASE (+ _FULL_MEMBERS, as 0x008D55A0 uses) or the slim FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE.
- this+0xA8 PAmmo: own member, `CIntelligentPointer<CThingObject>` (Ego_r). Header engine/CCreatureAction_ReloadMissileWeapon.h does NOT
  exist (only FireMissileWeapon / AimMissileWeapon); declare the class locally deriving from the shared CCreatureActionBase (as FinishAction 0x008D5480 does locally).
- CThing (engine/CThing.h): UsingComponents bitset<274> +0x20 (HasTC; TCI_CARRYING 0x46 bit 6 of +0x28, TCI_WEAPON 0x48 bit 8 of +0x28),
  Components CVectorMap +0x44 (GetTC), PhysicsTC +0x60 (GetPos), PBaseDef CDefPointer<const CThingBaseDef> +0x70 (`PBaseDef->GetSubDef(&def)`),
  PlayerNumber signed char +0x90 (GetPlayerNumber() inline, `movsx`), Dead bit +0x91 (IsDead). Opt-ins: FABLE_RETAIL_CTHING,
  FABLE_RETAIL_CTHING_GETTC_EXPANDED (key-store-inside-test, ternary end-or-greater -- matches retail exactly).
- CWeaponDef (engine/CWeaponDef.h, needs FABLE_RETAIL_DEF_CLASS_BASE): MissileLoadCarrySlot +0x94 (long), Ammo +0xA4 (long).
  Draft's names `AmmoCarrySlot` / `AmmoDefIndex` are invented -- WRONG; Ego_r has them at +0xA0/+0xB0 (Dinkumware maps), header has retail offsets.
- Locally modelled in the draft but with shared headers: CThing, C3DVector, CCharString (fable_string.h), CDefPointer, CDefClassBase
  (+CDefClassBase_GetSubDef.h), CWeaponDef, CVectorMap ("CInterfaceMap"), CBaseIntelligentPointer/CIntelligentPointer (engine/CIntelligentPointer.h),
  CTCCarrying, CCreatureActionBase, CThingObject, IS_THING_ALIVE (engine/ThingAlive.h).

## 5. Callee-body visibility / out-of-line evidence
- GetSubDef<CWeaponDef> 0x0042B1A6 (unaligned COMDAT): FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE + engine/CDefClassBase_GetSubDef.h
  (used for this very instantiation by 0x00845CE0 DropWeapon). Try both NOINLINE and declaration-only.
- IS_THING_ALIVE both forms called OUT of line here (0x78B2A0, 0x40DCE0) although sibling FinishAction 0x8D5480 (same TU) inlines
  IS_THING_ALIVE(PAmmo) -> use FABLE_RETAIL_IS_THING_ALIVE + FABLE_RETAIL_IS_THING_ALIVE_NOINLINE (engine/ThingAlive.h; template on `const T&`,
  so IS_THING_ALIVE(PAmmo) instantiates CIntelligentPointer<CThingObject> and IS_THING_ALIVE(existing) instantiates CThing*).
- CIntelligentPointer<CThingObject>(T*) and operator= out of line: FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE and
  FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE (engine/CIntelligentPointer.h lines 39-50; row 0x0064BFBA precedent).
- LowerBound 0x40F020 out of line (handled by GETTC_EXPANDED). GetPos 0x4C73D0 out of line (declaration only in CThing.h).
- CDefPointer members are __forceinline (header) -> release inline, matches.

## 6. Siblings
- 0x008D5480 FinishAction (same class, next function): rebuild/src/compiled/00/8d/CCreatureActionReloadMissileWeapon_FinishAction_008d5480.cpp
  -- FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED, engine/CVectorMap.h, local CCreatureActionBase/CTCCarrying stand-ins, no pragmas.
- 0x008D55A0 CCreatureAction_FireMissileWeapon::Initialise (same TU) -- `// Flags: none`; FABLE_RETAIL_CTCBASE, _CTHING, _CTHINGCREATUREBASE,
  _CINTELLIGENTPOINTER (+_COMPARE_NULL_INT, _GET_ORIGINAL_CHAIN), _IS_THING_ALIVE, _CCREATUREACTIONBASE(+_FULL_MEMBERS), _CCHARSTRING,
  _DEF_CLASS_BASE, _CTCCARRYING, _CTCWEAPON, _C3DVECTOR; includes engine/CThingCreatureBase.h, ThingAlive.h, CTCCarrying.h, CTCWeapon.h.
  Uses `PCreature->GetTC(&ptc_carry)` style. Best idiom donor.
- 0x008D58D0 FireMissileWeapon::ResetCreatureUseProjectileWeaponState -- GETTC_EXPANDED + FABLE_RETAIL_CINTELLIGENTPOINTER.
- 0x00849E80 CCreatureAction_PutItemIntoPocket::PerformAction -- same Create pattern: `{ CCharString scriptName(""); item =
  CThingObject::Create(ItemDefIndex, PCreature->GetPos(), PCreature->GetPlayerNumber(), 0, 0, scriptName); }` then
  `carrying->PutThingInCarrySlot(*item, slot, true)`.
- 0x00845CE0 DropWeapon -- GETSUBDEF_NOINLINE + CWeaponDef.h + CDefClassBase_GetSubDef.h includes.

## 7. Draft model errors
1. Create modelled as CThing::Create(long, const C3DVector*) + an invented 4-arg CThing::GetNamedPosition: actually 6-arg
   CThingObject::Create(Ammo, PCreature->GetPos(), PCreature->GetPlayerNumber(), 0, 0, name) with GetPos() no-arg. Bytes coincide but the
   source is wrong (and register allocation can differ: a call result feeding edx vs a stack arg).
2. PAmmo type CIntelligentPointer<const CThing> -> CIntelligentPointer<CThingObject>; temp should be built from the CThingObject* return.
3. CDefPointer<CWeaponDef> -> CDefPointer<const CWeaponDef>; GetSubDef is const; via PBaseDef (CDefPointer<const CThingBaseDef>) not raw CDefClassBase*.
4. RemoveThing/PutThingInCarrySlot take CThing& (pass *existing, *PAmmo); GetThingInPrimarySlot/GetThingInCarrySlot are const.
5. Field names AmmoCarrySlot/AmmoDefIndex -> MissileLoadCarrySlot/Ammo. Member names m_pDef/m_player/m_presentBits/m_interfaces -> PBaseDef/PlayerNumber/UsingComponents/Components.
6. Residue (DIFFER 379v383): VC hoisted &PCreature into ebp; retail keeps `this` spilled and re-forms this+8 per access. Using PCreature->
   (CIntelligentPointer operator->, inline Get) for each access, as sibling 0x00849E80 does, rather than a cached `CThing* creature` local
   may change this; also the local `existing` must be a stack word (address taken by IS_THING_ALIVE<CThing*> out of line).

## 0x009ADC90 (not landed)

# DOSSIER 0x009ADC90 CDefinitionManager::InstantiateDef (382 bytes incl. pad; body 0x9ADC90..0x9ADE0D)

## 1. Retail shape (xdis 009adc90 0x18e)
- thiscall, `ret 0Ch` (3 args), returns bool (`mov al,1` / `xor al,al`).
- No frame pointer (`sub esp,8; push ebx,ebp,esi,edi`; ebp = this) => plain speed TU, /Oy on. No SEH prologue (no fs:0) => no EH state
  in this function (no local with a dtor live across a call; `new` result is not guarded by an EH state either -> TU without /EHsc or nothing throws).
  Two locals: [E-8] = `this` spill (reloaded at 0x9ADD53/0x9ADDCD), [E-4] = CDefString temp.
- TWO separate epilogues: success `mov al,1;pop..;ret 0Ch` at 0x9ADDF6 and the miss path at 0x9ADE02 (`pop edi/esi/ebp; xor al,al; pop ebx`).
- Dead-arg-slot homing (key!): after GetPDefClassInfo(arg0) the result `info` is stored INTO arg0's slot [E+4] (0x9ADCA9);
  the bool arg2 slot [E+0Ch] is reused for the second CDefString temp, then for the push_back value (entry pointer, then globalIndex).
- Order: info=GetPDefClassInfo(className); if(!info) return false; edi = InstantiatedDefs.size() (+0xA8-+0xA4 >>2);
  ebx = info->GlobalIndices.size() (info+0xC - info+8 >>2); esi = info->AllocFunc() (call [eax+4], cdecl, no args);
  esi->Template = isTemplate (`shl cl,1; xor; and 2; xor` = 1-bit bitfield bit1 at +0x24);
  if (esi->IsParentDef()) esi->SetInstantiationName(CDefString(instanceName))  (vslot 0x40 = slot16, vslot 0x2c = slot11, by-value CDefString built
  in place: `push ecx; mov ecx,esp; push name; call 0x9D5060`);
  esi->PDefManager = this (+0x1c); esi->GlobalIndex = edi (+0x20);
  p = operator new(0x10) (0xBFEA1A); if (p) { InstantiationName temp at [E-4] = CDefString(instanceName) FIRST, then ClassName temp at [E+0Ch] =
  CDefString(info->Name) (pushes [E+4] = info itself: CDefClassInfo::Name is at +0) -> stores +0 ClassName, +4 InstantiationName, +8 ebx, +0xC esi,
  `inc [esi+4]` (unconditional AddRef, no null test) } else p=0;
  InstantiatedDefs.push_back(p) (inline STLport push_back; grow = 0x9B4BD0 `ret 14h` args (last,&val,tag,1,true));
  info->GlobalIndices.push_back(globalIndex) (grow = 0x464ACE);
  PSymbolMap(+0x58)->AddSymbolDirty(instanceName, globalIndex); PDefinitionSymbolMap(+0x60)->AddSymbolDirty(info->Name /*[E+4]*/, instanceName, globalIndex);
  `dec [esi+4]; jne; call [vtbl+4]` = CIVCountedPointeeBase release-and-Delete (slot 1 Delete 0x0042D930), NO null test; return true.
- Argument right-to-left evaluation of the entry ctor: CDefString(instanceName) is built before CDefString(info->Name) =>
  `new CInstantiatedDefInfo(CDefString(info->Name), CDefString(instanceName), classIndex, pdef)` (or equivalent member-init order).

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @Ego_r 0x00B05CD0) and FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv 0x0181D661 /
  0x030616A0): `?InstantiateDef@CDefinitionManager@@IAE_NABVCCharString@@0_N@Z` = protected `bool InstantiateDef(const CCharString&, const CCharString&, bool)`.
  Manifest label is RIGHT. Ego_r donor order GetPDefClassInfo(B058E0) -> InstantiateDef(B05CD0) matches retail 0x9AD580 -> 0x9ADC90.
- Parameter ROLES (draft has them SWAPPED): arg0 = class name (fed to GetPDefClassInfo), arg1 = instance name, arg2 = isTemplate (sets CDefClassBase::Template).
  Callers agree: rebuild/src/compiled/00/9a/CDefinitionManager_ParseStringForSymbols_009ade90.cpp:85/92 `InstantiateDef(className, instanceName, true/false)`;
  rebuild/src/compiled/00/9b/CDefinitionManager_AddDefClass_009b0ac0.cpp:142 `InstantiateDef(init.Name, nullName, false)`. xref: 0x9ADF5B, 0x9AE03A, 0x9B0BD9.

## 3. Callees
| addr | landed row | landed name | PDB | draft spelling |
|---|---|---|---|---|
| 0x009AD580 | 00/9a/CDefinitionManager_GetPDefClassInfo_009ad580.cpp | CDefinitionManager::GetPDefClassInfo(const CCharString&) `ret 4` | `?GetPDefClassInfo@CDefinitionManager@@IAEPAVCDefClassInfo@1@ABVCCharString@@@Z` (protected, non-const) | WRONG: `FindClassInfo` |
| info->+4 | - | CDefClassInfo::AllocFunc (`CDefClassBase* (__cdecl*)()`) | struct_layouts: CDefClassInfo+4 AllocFunc | `Create` (wrong name) |
| vslot 16 (+0x40) | - | CDefClassBase::IsParentDef() const, rebuild/include/engine/CDefClassBase.h:96 (0x0042DA70) | Ego_r | WRONG: `NeedsName` |
| vslot 11 (+0x2c) | - | CDefClassBase::SetInstantiationName(CDefString) (CDefClassBase.h:91, 0x0042DA20) | Ego_r | WRONG: `SetName` |
| 0x009D5060 | 00/9d/CDefString_SetString_009d5060.cpp | CDefString::SetString(const CCharString&) `ret 4`, no `this` return | `?SetString@CDefString@@QAEXABVCCharString@@@Z` | WRONG: draft calls it the out-of-line ctor `CDefString(const CCharString&)`. Retail = inline ctor whose body calls SetString (no TablePos init first). |
| 0x00BFEA1A | 00/bf/global_operatornew_00bfea1a.cpp | operator new | - | ok |
| 0x009B4BD0 | 00/9b/stdvector_Minsertoverflow_009b4bd0.cpp | vector<T*>::_M_insert_overflow(pos,x,__true_type,n,atend) `ret 14h` | STLport (element placeholder CUnprovenElement; really CInstantiatedDefInfo*) | draft `InsertGrow` (wrong) |
| 0x00464ACE | 00/46/stdvectorunsignedlong_Minsertoverflow_00464ace.cpp | vector<unsigned long>::_M_insert_overflow | STLport | draft `InsertGrow` on `long` (element is unsigned long) |
| 0x009E5420 | 00/9e/CSymbolMap_AddSymbolDirty_009e5420.cpp | CSymbolMap::AddSymbolDirty(const CCharString&, long) `ret 8` | `?AddSymbolDirty@CSymbolMap@@QAEXABVCCharString@@J@Z` | ok |
| 0x009E54E0 | 00/9e/CSymbolMap_AddSymbolDirty_009e54e0.cpp | CSymbolMap::AddSymbolDirty(group, name, long) `ret 0Ch` | `...@@QAEXABVCCharString@@0J@Z` | ok, but draft passes className; retail passes info->Name |
| vslot 1 (+4) | - | CIVCountedPointeeBase::Delete() const 0x0042D930 (CIVCountedPointeeBase.h:29) | | draft `Destroy` (wrong) |

## 4. Members (struct_layouts_egor.tsv; retail offsets = Ego_r minus 4 per 16-byte vector)
- CDefinitionManager (rebuild/include/engine/CDefinitionManager.h, opt-in FABLE_RETAIL_CDEFINITIONMANAGER, needs FABLE_RETAIL_CRESOURCELIST +
  FABLE_RETAIL_DEF_CLASS_BASE): PSymbolMap `CCountedPointer<CSymbolMap>` +0x58 (Data +0x58), PDefinitionSymbolMap +0x60, DefClassInfo CVectorMap +0x78,
  InstantiatedDefs `CArray<CInstantiatedDefInfo*>` +0xA4. In the retail header these are `unsigned char[...]` blobs (lines 73-75) -- no typed
  members, no InstantiateDef / GetPDefClassInfo / CDefClassInfo / CInstantiatedDefInfo declarations exist yet (an opt-in must be added, e.g.
  FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF), or the row keeps a local model (verifier warns). The non-retail branch (line ~214) has typed
  PSymbolMap_Data / PDefinitionSymbolMap_Data but byte InstantiatedDefs.
- CDefinitionManager::CDefClassInfo (24 bytes): +0 `CCharString Name`, +4 `AllocFunc` (cdecl `CDefClassBase*()`), +8 `CArray<unsigned long> GlobalIndices`.
- CDefinitionManager::CInstantiatedDefInfo (16): +0 `CDefString ClassName`, +4 `CDefString InstantiationName`, +8 `ulong ClassIndex`,
  +0xC `CDefPointer<CDefClassBase> PDef`.
- CDefClassBase (engine/CDefClassBase.h, FABLE_RETAIL_DEF_CLASS_BASE): +0x1c `CDefinitionManager* PDefManager`, +0x20 `ulong GlobalIndex`,
  +0x24 bitfields `Setup:1` (bit0), `Template:1` (bit1), `DefaultValsApplied:1` (bit2). Refcount `IVRefCount` +4 from CIVCountedPointeeBase
  (mutable ulong); inline AddRef(); Release() -- opt-in FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES gives `if(--IVRefCount==0){Delete();...}`.
- CDefString (engine/CDefString.h): private `long TablePos`; `CDefString(const CCharString&)` declared but NO inline body and NO SetString
  declaration in the header -> needs a new opt-in (inline ctor `{ SetString(s); }` + `void SetString(const CCharString&); // 0x009D5060`).
- CSymbolMap (engine/CSymbolMap.h, FABLE_RETAIL_CSYMBOLMAP): no AddSymbolDirty declarations yet -> needs a nested opt-in.
- CArray (engine/CArray.h) is `public std::vector<T>` (STLport 12-byte) -> push_back inline from STLport, grows via _M_insert_overflow.
- Draft models LOCALLY (all have shared headers): CCharString, CDefString, CDefClassBase, CDefinitionManager, CSymbolMap, CArray/vector
  (CRetailVector_*), CDefClassInfo / SInstantiatedDefInfo (wrong name: PDB is CInstantiatedDefInfo, nested in CDefinitionManager).

## 5. Callee-body visibility
- GetPDefClassInfo 0x9AD580 is an aligned, earlier function in the SAME TU (Ego_r order) yet called out of line (98 bytes; not inlined).
- SetString 0x9D5060 (CDefString TU) out of line; the CDefString(const CCharString&) ctor INLINE (expanded around the call).
- STLport push_back inline both times; _M_insert_overflow out of line (0x9B4BD0, 0x464ACE) -- standard STLport visibility.
- CIVCountedPointeeBase::AddRef / Release inline (`inc [esi+4]`, `dec [esi+4]; jne; call [edx+4]`).
- CInstantiatedDefInfo ctor (if any) fully inline; no out-of-line ctor in PDB lists.
- Note FableWin has `??0?$CDefPointer@VCDefClassBase@@@@QAE@PAVCDefClassBase@@@Z` (CDefPointer(T*) ctor), but retail's AddRef has NO null test
  and the final release has NO null test, while engine/CDefPointer.h copy ctor/dtor do test -> model pdef as a RAW CDefClassBase* and the entry
  store as raw pointer + `pdef->AddRef()` (as landed LoadDef 0x9AD6E0 models PDef with an inline raw assignment).

## 6. Siblings (same class / neighbourhood, same TU)
- 00/9a/CDefinitionManager_LoadDef_009ad6e0.cpp (0x9AD6E0): local CInstantiatedDefInfo/CDefClassInfo/CDefPointer model, no pragmas.
- 00/9a/CDefinitionManager_GetPDefClassInfo_009ad580.cpp: `#pragma auto_inline(off/on)` around helpers; home-in-dead-arg-slot idiom documented.
- 00/9a/CDefinitionManager_GetNumberOfDefsOfType_009ad3b0.cpp, _GetPDefFromInstantiationName_009ada40.cpp, _GetPDefFromGlobalIndex_009ade10.cpp (next function).
- 00/9a/CDefinitionManager_ParseStringForSymbols_009ade90.cpp (caller, same TU).
- 00/9a/CDefinitionManager_DecompressDefinitionData_009ad5f0.cpp: `// Flags: none` + FABLE_RETAIL_* defines (CCHARSTRING etc.).
- None use optimize pragmas -> default speed /O2 /Oy, no EH frame.

## 7. Old draft model errors
1. Params swapped: (name, className) -> must be (className, instanceName, isTemplate).
2. Uses className after the lookup; retail uses info->Name (CDefClassInfo +0) for the ClassName CDefString and for the 3-arg AddSymbolDirty.
   This is why className's slot dies and info is homed in it -- the recorded residue ("retail spills the class-info pointer") is exactly this.
3. CDefClassInfo +0 modelled as `long ClassIndex`; it is `CCharString Name`. Instances vector = GlobalIndices (CArray<unsigned long>).
4. CDefString(const CCharString&) modelled as out-of-line ctor 0x9D5060; it is SetString (inline ctor -> SetString).
5. Entry ctor arg order: retail builds InstantiationName temp first (right-to-left), ClassName second; the draft's member-init ctor builds Name first.
6. Wrong names: FindClassInfo, Create, NeedsName, SetName, Destroy, Recompiled, SInstantiatedDefInfo, InsertGrow; vector elements `long` vs `unsigned long`.
7. "one shared return false exit" comment is wrong: retail has two epilogues (early `return false` before everything).


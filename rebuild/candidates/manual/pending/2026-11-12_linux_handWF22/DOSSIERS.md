## 0x007E1FC0

# DOSSIER 0x007E1FC0 -- CTCAmbientBird::FrameUpdate (843 bytes)

## 1. Retail shape (xdis 007e1fc0 0x35b)
- `__thiscall`, `this`=esi, plain `ret` (no args), frame `sub esp,44h` -- NO frame pointer (no push ebp/mov ebp,esp), NO SEH
  prologue (the CCharString temp is destroyed with a direct dtor call, no EH state), so no /EHsc unwinding needed in this body
  (CCharString has no EH frame because nothing after the ctor can throw-under-tracking -- just mirror landed TC rows: default flags).
- Registers: esi=this, edi saved at entry; ebp pushed only locally around the two GetPos calls in the take-off arm
  (0x007E20FC `push ebp ; mov ebp,[esi+4]` ... `pop ebp` at 0x007E2114) -- the parent thing is held in ebp briefly.
- Three exits, each its own `pop edi; pop esi; add esp,44h; ret` (0x7E1FF7 kill, 0x7E20CC flying, 0x7E2305 take-off/fall-through).
- Flying arm (0x7E1FD2-0x7E20D1): `GetPos().Z - Flying-height(+0x10) > 40.0f` (fcomp [0x012700EC], test ah,41h, jne) -> Kill(false); else
  heading = PhysicsTC->slot70(hidden ptr) * 0.8f (0x012700E8), target = offset + GetPos() (sum computed Z,Y,X), then
  GetWorldMap()->slot16(&target) -> long, GetWorldMap()->slot10(long) -> bool; if false target = (pos.X, pos.Y, pos.Z+0.8f)
  (copied as ints, Z through fadd); PhysicsTC->slot30(&target). Draft says this arm already matches.
- Take-off arm (0x7E20D2-): hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature(); `if (!hero || hero dead (+0x91 bit0)) return;`
  away = this-parent.GetPos() - hero.GetPos() (hero pos in edi, parent pos in eax).
  Products CSE'd into frame temps: Z*Z -> [esp+14], Y*Y -> [esp+10], X*X -> [esp+0C] (fst), sum X2+Y2+Z2 -> [esp+18] (fst).
  `if (sum < 31.36f)` (0x012700FC, test ah,5 ; jp skip). Then Flying=1, +0x10 = GetPos().Z (stored as int copy),
  `if (UsingComponents bit 0x83)` (test byte [thing+30h],8) -> expanded GetTC: LowerBound 0x0040F020 on &thing->Components(+0x44)
  with key 0x83 in a stack word [esp+8], end test `it==end || 0x83 < it->first ? end : it`, `->second` (no null test),
  then CCharString("ST_FLY"? string at 0x012700F4, len -1) in [esp+8] (same slot as the key!), PlayAnimation(str, 1, true), ~CCharString.
  Normalise: `if (DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO < sum)` (fld [0x0129BA3C]; fmul same; fcomp [esp+18] -- REUSES the stored
  distance sum) -> GFFastOneOverSqrt of a RE-ADDED sum `[0C]+[10]+[14]` (X2+Y2+Z2, products reused, sum NOT reused), Newton
  `(3.0f - r*r*value) * r * 0.5f` (0x0122DED4, 0x0122F59C), result X,Y,Z kept on x87 (st2,st1,st0); else fld 0,1,0
  (0x0122DEDC/0x0122DED8/0x0122DEDC). Join at 0x7E227D with forward on the x87 stack.
  RHS at [orig esp+34]: Up=(0,0,1) via immediate stores, Forward=X,Y,Z; axis at [orig esp+28] = forward ^ up computed with
  `fmul [0.0f]` (up constants propagated; *1.0 folded). push -0.17777778f (0xBE360B61), push &axis, ecx=&rhs, call 0x00A22F80.
  Then PhysicsTC->slot75(&rhs), PhysicsTC->slot38(false).

## 2. Identity (name is RIGHT)
- Vtable: rebuild/manifest/rtti-vtables.tsv `CTCAmbientBird 0123dc0c slot 9 = 007e1fc0`; CTCBase.h slot 9 = `virtual void FrameUpdate()`.
  Slot 6 (0x007E1F70) is the landed OnInitialActivate, slot 8 HasFrameUpdate (0x004D5F32).
- PDB: Ego_r `?FrameUpdate@CTCAmbientBird@@UAEXXZ` (egor_pdb_names.tsv 0x007DDBA0, after OnInitialActivate 0x007DDB50 -- same order as
  retail 0x007E1F70 -> 0x007E1FC0); FableWin `_fablewin_api.tsv` 0x0180FCC3 / 0x027891C0 same mangling.
- Mangled leaf: `?FrameUpdate@CTCAmbientBird@@UAEXXZ` -- public virtual, void(void), non-const.
- Callers: only the vtable data ref (xref: `.rdata 0123dc30`).

## 3. Callees (landed file / landed name / PDB) -- DRAFT SPELLING ERRORS FLAGGED
| addr | landed file | PDB / landed name | draft spelling |
|---|---|---|---|
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | `?GetPos@CThing@@QBEABVC3DVector@@XZ` CThing::GetPos() const | **GetPosition -- WRONG, use GetPos** |
| 0x004C9B80 | 00/4c/CThing_Kill_004c9b80.cpp | `?Kill@CThing@@QAEX_N@Z` | Kill(bool) ok |
| 0x00686D30 | 00/68/CTCBase_GetWorldMap_00686d30.cpp | `?GetWorldMap@CTCBase@@IAEAAVCWorldMap@@XZ` protected, returns CWorldMap& | draft returns pointer -- use reference |
| 0x00686D50 | 00/68/CTCBase_PeekPlayerManager_00686d50.cpp | `?PeekPlayerManager@CTCBase@@IBEABVCPlayerManager@@XZ` const | **GetPlayerManager -- WRONG** (0x00686D40 is GetPlayerManager) |
| 0x00449960 | 00/44/CPlayerManager_GetMainPlayer_00449960.cpp | `?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ` const -> const CPlayer& | draft returns CPlayer* -- WRONG type |
| 0x00487DD0 | 00/48/CPlayer_PeekPControlledCreature_00487dd0.cpp | `?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ` | **GetCharacterThing -- WRONG** |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | `CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<...> >::LowerBound` (non-const) | local CTCMap::LowerBound -- wrong class |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString::CCharString(const char*, long=-1) | ok |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | CCharString::~CCharString | ok |
| 0x007F90B0 | 00/7f/CTCAnimatingObject_PlayAnimation_007f90b0.cpp | `?PlayAnimation@CTCAnimatingObject@@QAEXABVCCharString@@J_N@Z` (name, long priority, bool looping) | **(bool,bool) -- WRONG**: call is PlayAnimation(name, 1, true) |
| 0x00A22F80 | 00/a2/CRightHandedSet_Rotate_00a22f80.cpp | `?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z` | **RotateAboutAxis -- WRONG** |
| PhysicsTC slot 30 +0x78 | engine/CTCPhysicsBase.h | `virtual void SetPosition(const C3DVector&)` | ok |
| PhysicsTC slot 38 +0x98 | CTCPhysicsBase.h | `virtual void EnableGravity(bool)` (count: CTCBase 0-27, GetOffsetVector 28, GetOffsetVector3D 29, SetPosition 30, ResetPosition 31, ResetRHSet 32, SetPositionToNavigateTo 33, SetForcedToMovePos 34, ResetMovementState 35, EnablePhysics 36, IsPhysicsEnabled 37, EnableGravity 38) | **SetPhysics -- WRONG, it is EnableGravity(false)** |
| PhysicsTC slot 70 +0x118 | CTCPhysicsBase.h (comment: "slot 70 0x006B1410 the no-arg overload") | `virtual C3DVector GetFacingVector() const` | **GetHeading -- WRONG** |
| PhysicsTC slot 75 +0x12C | CTCPhysicsBase.h (GetRHSet(float) 71, GetRHSet() 72, GetLastRHSet 73, SetFacingAnglesFromRHSet 74) | `virtual void SetRHSet(const CRightHandedSet&)` | **SetOrientation -- WRONG** |
| CWorldMap slot 10 +0x28 | engine/CWorldMap.h (FABLE_RETAIL_CWORLDMAP) | `virtual bool IsMapUsed(long) const` | **IsValidSection -- WRONG** |
| CWorldMap slot 16 +0x40 | CWorldMap.h (comment: retail vtable 0x01244AEC slot 16 = GetMapNumberAt(const C3DVector&) 0x004FC0C0) | `virtual long GetMapNumberAt(const C3DVector&) const` | **GetSection -- WRONG** |

Flying-arm source therefore: `long map = GetWorldMap().GetMapNumberAt(target); if (!GetWorldMap().IsMapUsed(map)) ...`.
Note the parent's PhysicsTC is CThing::PhysicsTC (+0x60, `GetPhysicsTC()` accessor in CThing.h), NOT a "placement".

## 4. Members
- CTCAmbientBird (no shared header engine/CTCAmbientBird.h; struct_layouts_egor size 20): +0x0C `bool Flying`, +0x10 `float InitialHeight`
  (**draft's `TakeOffZ` is wrong; the landed 0x007E1F70 row already spells InitialHeight**). Base CTCBase (engine/CTCBase.h,
  FABLE_RETAIL_CTCBASE): +0x04 `CThing* ParentThing`, +0x08 ToBeRemoved, +0x09 IsInGlobalUpdateTCs.
  Declare `class CTCAmbientBird : public CTCBase { bool Flying; float InitialHeight; public: virtual void FrameUpdate(); }` locally
  (no header exists) -- as 0x007E1F70 does.
- CThing (engine/CThing.h; static asserts UsingComponents==0x20, Components==0x44, PhysicsTC==0x60, PBaseDef==0x70):
  +0x20 `std::bitset<274> UsingComponents` (bit 131 = TCI_ANIMATING_OBJECT in engine/ETCInterfaceType.h, word +0x30 bit 3),
  +0x44 `CVectorMap<ETCInterfaceType,CTCBase*,...> Components`, +0x60 `CTCPhysicsBase* PhysicsTC`, +0x91 bit0 `Dead` (IsDead() const, line 280).
  Opt-ins: FABLE_RETAIL_CTHING (forceinline GetTC(ETCInterfaceType) -> `*Components.Find(type)->second`),
  FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC<T>/PeekTC<T> with LowerBound out of line, end-or-greater test -- matches retail),
  HasTC variants: default, FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (test word in place), FABLE_RETAIL_CTHING_HASTC_MATERIALISED.
  Retail tests the bit in place (`test byte ptr [eax+30h],8 ; je`), so HasTC must inline as a direct test.
  Hero is `const CThingPlayerCreature*` (engine/CThingPlayerCreature.h, a CThing).
- CPlayer (engine/CPlayer.h, FABLE_RETAIL_PLAYER; line 214 `const CThingPlayerCreature* PeekPControlledCreature() const; // 0x00487DD0`).
- CPlayerManager (engine/CPlayerManager.h, FABLE_RETAIL_CPLAYERMANAGER: `const CPlayer& GetMainPlayer() const; // 0x00449960`).
- CTCBase.h: `CWorldMap& GetWorldMap(); // 0x00686D30` (line 149), `const CPlayerManager& PeekPlayerManager() const;` under
  FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER (line 174).
- C3DVector (engine/C3DVector.h, FABLE_RETAIL_C3DVECTOR): `DELTA_NOTIONAL_ZERO` (0x0129BA3C, extern const float),
  `GFastOneOverSqrtTable[128]` (0x013CE558), forceinline `GFFastOneOverSqrt` (variants: NEWTON_GROUPED / NEWTON_SQUARE_PAREN /
  NEWTON_STEP_PAREN / NEWTON_PAREN / ORIGINAL_UNION), `IsNonZero()` variants (default line 434 `X*X+Y*Y+Z*Z > DELTA*DELTA`),
  `GetSquaredMagnitude()` variants (e.g. FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ), `Normalise()` variants,
  `operator^` cross product (several paren variants, lines 186-202) and `Cross`.
- CRightHandedSet (engine/CRightHandedSet.h): FABLE_RETAIL_CRIGHTHANDEDSET + FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
  (`CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward)`, built in place -- exactly retail's
  immediate stores), FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE (`void Rotate(const C3DVector&, float)`).
- Classes the old draft models LOCALLY that have shared headers: CCharString (fable_string.h), C3DVector, CRightHandedSet,
  CThingPlayerCreature/CThing (CThing.h), CTCPhysicsBase ("CThingPlayerCreature"/"CThingPlacement" stand-in), CTCAnimatingObject
  (check engine/CTCAnimatingObject.h), CVectorMap (engine/CVectorMap.h; draft "CTCMap"), CPlayer, CPlayerManager, CWorldMap,
  CTCBase (draft folds it into CTCAmbientBird). Also draft's GetSafeNormalised / CrossProduct are made-up; use header IsNonZero /
  Normalise / operator^ (FableWin names).

## 5. Callee-body visibility
- All non-virtual callees are out of line in retail and landed separately; none are same-TU neighbours except OnInitialActivate
  0x007E1F70 (defined just before; irrelevant). No STLport map/tree helpers. LowerBound 0x0040F020 is a COMDAT called out of line
  (GETTC_EXPANDED models that). CRightHandedSet ctor: Ego_r out-of-line COMDAT 0x00487050 exists but retail here builds in place
  -> use CTOR_INLINE. C3DVector math, IsNonZero, Normalise, GFFastOneOverSqrt, operator^, operator*, operator+, operator- all
  expanded (FABLE_VECTOR_INLINE / FABLE_FORCEINLINE).

## 6. Sibling landed rows (idioms)
- rebuild/src/compiled/00/7e/CTCAmbientBird_OnInitialActivate_007e1f70.cpp -- same TU; local `struct CTCAmbientBird : CTCBase { bool Flying; float InitialHeight; }`, includes engine/CThing.h, nested ifs; no pragmas.
- rebuild/src/compiled/00/79/CTCSummonSpell_CreateMinion_0079acc0.cpp -- closest idiom set: FABLE_RETAIL_CTHING, _GETTC_EXPANDED,
  FABLE_RETAIL_CTCPHYSICSBASE, FABLE_RETAIL_CRIGHTHANDEDSET, _CTOR_INLINE, FABLE_RETAIL_C3DVECTOR, _SQUAREDMAGNITUDE_XYZ, FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CCHARSTRING.
- rebuild/src/compiled/00/7b/CTCFishing_GetPFishingRodDef_007b0880.cpp -- PeekPlayerManager().GetMainPlayer() chain.
- rebuild/src/compiled/00/7e/CTCFineDialogue_GlobalUpdate_007e2a60.cpp -- FABLE_RETAIL_CTHING_GETTC_EXPANDED + HASTC_MATERIALISED, CPLAYERMANAGER, PLAYER opt-ins.
- rebuild/src/compiled/00/a2/CRightHandedSet_Rotate_00a22f80.cpp -- Rotate callee (`// Flags: /d2inlS279`, only for that row).
- rebuild/src/compiled/00/7c/NThingShotCTCShotDefinitionBase_OnCollisionProper_007c1000.cpp -- CWorldMap GetMapNumberAt use with FABLE_RETAIL_CWORLDMAP.

## 7. Old draft model errors
- Names: GetPosition->GetPos, GetPlayerManager->PeekPlayerManager (const), GetCharacterThing->PeekPControlledCreature,
  RotateAboutAxis->Rotate, GetHeading->GetFacingVector, SetOrientation->SetRHSet, SetPhysics->EnableGravity,
  GetSection->GetMapNumberAt (returns long), IsValidSection->IsMapUsed, TakeOffZ->InitialHeight, PlayAnimation args (1,true) are (long priority, bool looping).
- Types: const chain (const CPlayerManager& -> const CPlayer& -> const CThingPlayerCreature*), GetWorldMap returns a reference.
- Normalise residue: retail compares `DELTA*DELTA` against the SAME stored distance-squared local (IsNonZero CSE'd with the
  `distanceSquared` variable), then recomputes only the SUM (not the products) inside the fast normalise -> spell it as
  `if (away.IsNonZero()) { forward = away; forward.Normalise(); } else forward = C3DVector(0,1,0);` (or GetSquaredMagnitude-based
  local), with forward a fresh local so X/Y/Z stay on x87 across the join. The draft's single GetSafeNormalised recomputed
  everything (872 v 843). Products are computed Z*Z, Y*Y, X*X but summed X2+Y2+Z2 -- default `X*X+Y*Y+Z*Z` spellings.
- GetTC: retail has no null test on the found component and the LowerBound key word shares the stack slot with the CCharString
  temporary -> HasTC + GetTC (FABLE_RETAIL_CTHING forceinline GetTC returning CTCBase&, static_cast to CTCAnimatingObject&) or GetTC_EXPANDED.
- Cross product: retail computes `forward ^ up` with up literal (fmul by 0.0f kept) -- header operator^ on (forward, up).

## 0x004BD6E0

# DOSSIER 0x004BD6E0 (845 bytes) -- NGraphicAppearance::CTCGraphicAppearance::SetPrimitive

## 1. Retail shape (xdis 004bd6e0 0x35d)
- __thiscall, `ret 0Ch` (3 stack args), NO frame pointer (`sub esp,44h` then push ebx/ebp/esi/edi; ebp = this). No SEH prologue
  although locals have destructors -> TU built without /EHsc (siblings in this TU use `// Flags: none`-style, no pragmas).
- Frame 0x44. Locals (offsets after the 4 pushes): +0x10 set<CCharString> header ptr, +0x14 node_count; +0x1C C3DAnimationInterpolator
  (vptr 0x01239840, StartState +0x20 (vptr 0x01239838, Data +0x24), EndState +0x28 (Data +0x2C)); +0x30..+0x53 CGraphicAppearanceDrawInfo.
- Dead `info` arg slot ([esp+58h]) is reused for: the float info+4, every TC key temp (3 / 0xE9 / 0x5E), and the "all succeeded" bool.
- Loop-head alignment `lea esp,[esp+0]` at 0x4BD8C9, `mov edi,edi` at 0x4BD95E (normal VC7.1 /O2 padding).

## 2. Identity (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @Ego 0x004C2C00):
  `?SetPrimitive@CTCGraphicAppearance@NGraphicAppearance@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC3DPositionInterpolator@@ABVC3DOrientationInterpolator@@@Z`
  -> PROTECTED, non-virtual, void, all three params `const &`.
- Fable.exe signatures (naming_stragglers/Fable.exe.signatures.tsv:4687) = NGraphicAppearance::CTCGraphicAppearance::SetPrimitive 845.
- Landed caller rebuild/src/compiled/00/4b/NGraphicAppearanceCTCGraphicAppearance_Draw_004be490.cpp (tail jmp 0x4BE4CD) declares exactly
  `void SetPrimitive(const CEnginePrimitivePrepareInfo&, const C3DPositionInterpolator&, const C3DOrientationInterpolator&)`; second caller
  Draw(info) 0x004BE4E0 (call 0x4BE564). xref: only these two.
- Manifest label name is right but signature in manifest/engine_api (pointers) and old draft (pointers, no namespace, public) is wrong.

## 3. Callees
| retail | landed row | landed name | PDB / correct | draft spelling |
|---|---|---|---|---|
| 0x0099A2D0 | 00/99/CBaseClass_CBaseClass_0099a2d0.cpp | CBaseClass::CBaseClass | same (engine/CBaseClass.h) | `CBase` WRONG |
| 0x0099A2E0 | 00/99/CBaseClass_CBaseClass_0099a2e0.cpp | CBaseClass::~CBaseClass | same | `CBase` WRONG |
| PTCAnimation vslot 29 (+0x74) | (pure; GetAnims row 00/4b/..._GetAnims_004bc000.cpp) | CTCAnimationBase::GetMeshAnims(C3DAnimationBlendState&,C3DAnimationBlendState&) const | engine/CTCAnimationBase.h line 13 | `CTransformSource::GetShared` WRONG |
| 0x004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (CONST) | CThing.h GETTC_EXPANDED says const=0x4365B0 | `CTCMap::LowerBound` WRONG |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | same, NON-const | CThing.h | `LowerBoundLights` WRONG |
| TC3 vslot 29 (+0x74) | -- | CTCAppearanceMorphBase::GetMeshTextureMorphs() const -> CMeshMorphSet* (Ego_r 0x006E6D00/0x0073E5F0 overrides `UBEPAVCMeshMorphSet@CTCAppearanceMorphBase@@XZ`) | TC 3 = TCI_APPEARANCE_MORPH | `CTCDecals::GetDecals` WRONG |
| 0x0070A570 | 00/70/CTCTextureDecal_PeekEngineDecals_0070a570.cpp | CTCTextureDecal::PeekEngineDecals() -> const vector<CEngineTextureDecal>& (QAE, non-const) | TC 0xE9 = TCI_TEXTURE_DECAL | `CTCLights::GetLights` WRONG |
| 0x00706860 | 00/70/CTCActivationReceptorBase_AddTrigger_00706860.cpp (BAD label) | landed "CTCActivationReceptorBase_AddTrigger" | Ego_r: `?GetHiddenMaterialNames@CTCHeroAttachableAppearanceModifiers@@QBE_NAAV?$set@VCCharString@@...@std@@@Z` (TC 0x5E = TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS = 94; arg is the local set<CCharString>&). -> NAME_FIXES candidate | `CTCActivationReceptor::GetTriggers` WRONG |
| 0x00BFEA0E / 0x00BFEA14 | 00/bf/global_malloc / global_free | malloc / free | CRT | ok |
| component vslot 4 (+0x10) | -- | CAGraphicAppearanceComponent::IsAppearanceSentEveryRenderFrame() | engine/NGraphicAppearance_CAGraphicAppearanceComponent.h | `NeedsUpdate` WRONG |
| component vslot 0 | -- | CAGraphicAppearanceComponent::OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) | same header | `Prepare` WRONG |
| 0x004ABA70 | 00/4a/stdRbtree_Merase_004aba70.cpp | _Rb_tree<CCharString,...,_Identity,...>::_M_erase | Ego_r leaf in that row | `CTriggerSet::_M_erase` (type wrong) |
| shared-data vslot 1 (+4) | -- | Delete of CIVCountedPointer<C3DAnimationBlendState::CSharedData> | C3DAnimationBlendState.h | `CRefCountedShared::Destroy` |
- Global 0x013B8A18 is WRITTEN here (`= &info`); referenced only in this TU (0x4BC38E..0x4BD6E9). It is a file-static
  `const CEnginePrimitivePrepareInfo*`: readers (RefreshPrimitivePos 0x4BC340, RefreshPrimitiveAnimationAndPos 0x4BCFE0,
  SetPrimitiveGraphics 0x4BD620, SetAttachedPrimitiveGraphics 0x4BD690) read +0x34 = CEnginePrimitivePrepareInfo::Engine.
  No PDB name found (not in egor_pdb_names / _fablewin_api). Landed rows spell it `g_pEngineGlobals_013b8a18` -- keep that one name
  (one address one name); retype as `const CEnginePrimitivePrepareInfo*` is the accurate model. Draft's `g_CurrentPrepareInfo` is a second name: don't.

## 4. Members
- this (CTCGraphicAppearance, retail 0x5C; header engine/NGraphicAppearance_CTCGraphicAppearance.h, layout under
  FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT, static assert 0x5c):
  +0x04 CTCBase::ParentThing (CThing*) [draft `Thing`], +0x24 PTCAnimation (CTCAnimationBase*) [draft `TransformSource` WRONG],
  +0x3C Components = std::list<CAGraphicAppearanceComponent*> [draft calls it `Primitives` WRONG -- Primitives is +0x38],
  +0x58 UpdatePrimitiveReasons (uchar) [draft `DetailMode`], +0x59 bit2 (mask 4) ResendPrimsEveryRenderFrame [draft `Flags59 & 4`].
  MOVEMENT needs CEngineGraphic etc. (see IsResourcesLoaded_004be3e0 defines: FABLE_RETAIL_CTCBASE, FABLE_RETAIL_STLP_VECTOR,
  FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT ...). Header does NOT declare SetPrimitive, GetUpdatePrimitiveReasons, GetAnims,
  CGraphicAppearanceDrawInfo -> needs a new opt-in (e.g. FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETPRIMITIVE).
- info: engine/CEnginePrimitivePrepareInfo.h (FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO gives typed Interpolate). Reads info+4 =
  Interpolate.Current.WFInterpolate (float; CInterpolationInfo.Current is CInterpolationInfoSet {GTPredictedTimeSinceLastRenderFrame +0,
  WFInterpolate +4}). Draft's `info->Position` is WRONG (it's a float).
- CThing (engine/CThing.h): UsingComponents +0x20 bitset, Components +0x44 CVectorMap; FABLE_RETAIL_CTHING_GETTC_EXPANDED (with
  FABLE_RETAIL_CTHING_HASTC_FORCEINLINE / ONHIT gate) gives GetTC(id,T**) (non-const LowerBound 0x40F020) and PeekTC(id,const T**)
  (const LowerBound 0x4365B0) bodies exactly as retail: key copied into a local inside the test, map through a pointer local,
  `(it==end||type<it->first)?end:it` ternary. TC3 and TC0x5E use PeekTC (const), TC0xE9 uses GetTC.
- CGraphicAppearanceDrawInfo (Ego_r, 36 bytes; no shared header) = the local at +0x30:
  +0 float Interpolate (= info.Interpolate.Current.WFInterpolate), +4 C3DPositionInterpolator* Pos (=&position), +8 C3DOrientationInterpolator* Orientation,
  +0xC C3DAnimationInterpolator* Anim (=&anims local), +0x10 vector<CEngineTextureMorph>* PTextureMorphs (=0),
  +0x14 vector<CEngineTextureDecal>* PTextureDecals (=0), +0x18 set<CCharString>* HiddenMaterials (=&hiddenMaterials),
  +0x1C ulong ReasonsForDraw (=GetUpdatePrimitiveReasons()), +0x20 bool HiddenMaterialsUsed (=false), +0x21 bool TextureMorphsUsed (NOT written by retail).
- CMeshMorphSet (Ego_r 32; retail: MeshParts CArray 12-byte STLport vector +0, OriginalMeshTextureMorphs CEngineTextureMorphSet +0xC whose
  first member Morphs is CArray<CEngineTextureMorph>): retail tests [ret+0xC] != [ret+0x10] and stores ret+0xC -> PTextureMorphs = &set->OriginalMeshTextureMorphs.Morphs
  (CArray = vector, address-of the vector).
- PeekEngineDecals result: tests [v]!=[v+4] (begin!=end) -> PTextureDecals=&v.
- Hidden materials: after GetHiddenMaterialNames(hidden) (bool result IGNORED), `if (hidden node_count != 0) HiddenMaterialsUsed = true` (reads count [esp+14h]).
- C3DAnimationInterpolator / C3DAnimationBlendState: shared headers exist (engine/C3DAnimationInterpolator.h, C3DAnimationBlendState.h)
  but declare ctor/dtor OUT OF LINE (0x4C2E20/0x4C2E90, 0x4C2DD0/0x4C2DF0). Retail INLINES them here (implicit ctor: CBaseClass(), vptr,
  member ctors; implicit dtor: Data release `dec [obj+4]`, Delete via slot 1 at 0, zero Data, ~CBaseClass; no vptr re-stores).
  Landed 0x4C2E20 / 0x4C2E90 rows model them as compiler-generated (local models) -> the author needs an opt-in that makes both classes'
  ctor/dtor implicit (none exists today; C3DAnimationBlendState.h has FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA for a typed CIVCountedPointer Data).
- Locally modelled classes in old draft that have shared headers: CBase(->CBaseClass.h), CThing(CThing.h), CTCBase(CTCBase.h),
  CTCGraphicAppearance(NGraphicAppearance_CTCGraphicAppearance.h), CEnginePrimitivePrepareInfo, CAppearancePrimitive(->CAGraphicAppearanceComponent.h),
  CTransformSource(->CTCAnimationBase.h), CAppearanceTransform/CAppearanceSharedPointer(->C3DAnimationInterpolator.h / C3DAnimationBlendState.h),
  CTCMap(->CVectorMap.h), CTCActivationReceptor(->CTCHeroAttachableAppearanceModifiers.h), STL trees/lists (->stlport/fable_containers.h).

## 5. Visibility (what retail inlined)
- GetAnims (protected, out of line at 0x004BC000, landed) is INLINED here: `if (PTCAnimation) PTCAnimation->GetMeshAnims(anims.StartState, anims.EndState);`
  (no call to 0x4BC000) -> same TU, defined earlier (0x4BC000 < 0x4BD6E0); write the body inline or make GetAnims an inline member.
- GetUpdatePrimitiveReasons() const (protected, inline, PDB name per Draw_004be490 row): THIS row proves the constant is 4
  (`mov eax,4` at 0x4BD7C4): `if (!UpdatePrimitiveReasons && ResendPrimsEveryRenderFrame) return 4; return UpdatePrimitiveReasons;`
  (landed Draw row returns 1 -- only tested for truth there; byte-neutral, but the real constant is 4).
- C3DAnimationInterpolator / BlendState ctors/dtors inline (above). CIVCountedPointer release inline.
- std::set<CCharString> ctor inline (malloc(0x14) header, color 0, parent 0, left=right=header, count 0; the `[hdr]=0` store before malloc is
  STLport _Rb_tree_base ctor), dtor inline clear(): `if(count){_M_erase(root); left=hdr; parent=0; right=hdr; count=0}` + free(header);
  only _M_erase 0x4ABA70 out of line. std::list<CAGraphicAppearanceComponent*> ctor/push_back/clear/dtor all inline (malloc 0xC nodes,
  free per node). Check stlport/fable_containers.h FABLE_RETAIL_STLP_LIST / FABLE_RETAIL_STLP_SET (tree ctor/dtor out-of-line opt-ins
  FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE / _CLEAR_OUTOFLINE must NOT be defined).
- LowerBound x2 out of line (COMDATs) -- as CThing GETTC_EXPANDED models.

## 6. Siblings (same TU, retail 0x4BBFxx-0x4C5Dxx)
- 00/4b/NGraphicAppearanceCTCGraphicAppearance_Draw_004be490.cpp (caller; local layout model, inline GetUpdatePrimitiveReasons, no pragmas).
- 00/4b/NGraphicAppearanceCTCGraphicAppearance_IsResourcesLoaded_004be3e0.cpp: shared-header idiom; defines FABLE_RETAIL_CTCBASE,
  FABLE_RETAIL_STLP_VECTOR, FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT, _HELPER_BODY, _HELPERNAMES, ...
- 00/4b/NGraphicAppearanceCTCGraphicAppearance_GetAnims_004bc000.cpp (GetMeshAnims slot 29 model).
- 00/4b/NGraphicAppearanceCTCGraphicAppearance_OnKill_004bf8f0.cpp / ClearAllPrimitives_004bf9a0.cpp (component-list loop re-reading
  end() `cmp ebx,[ebp+3Ch]` each iteration -- same as this row's first loop).
- 00/4b/..._SetAttachedPrimitiveGraphics_004bd690.cpp / _RefreshPrimitiveAnimationAndPos_004bcfe0.cpp (global 0x13B8A18 spelling).
- None use #pragma optimize; no /EHsc.

## 7. Old draft model errors
- Signature: pointers instead of `const &`; no namespace; public instead of protected. `position = info->Position` is a misreading: retail
  stores info.Interpolate.Current.WFInterpolate (float) into the dead info slot; setup field0 is that float, field1 is &position (arg2).
- Setup struct field order / meaning wrong (Info/Position/Orientation/Transform/Decals/Lights/Triggers/DetailMode/HasTriggers) -> use
  CGraphicAppearanceDrawInfo {Interpolate, Pos, Orientation, Anim, PTextureMorphs, PTextureDecals, HiddenMaterials, ReasonsForDraw,
  HiddenMaterialsUsed, TextureMorphsUsed}; TextureMorphsUsed left unwritten.
- TC assignments swapped: TC3 = appearance morph texture morphs (field +0x10), TC0xE9 = texture decals (field +0x14), TC0x5E = hidden materials.
- Uses `Primitives` for what is Components (+0x3C); loop body: call IsAppearanceSentEveryRenderFrame FIRST (unconditionally), then
  `ResendPrimsEveryRenderFrame |= sent` (retail: test old bit || al, shl 2 / xor / and 4 / xor merge) -- draft's `(Flags59&4)||needsUpdate`
  is equivalent but bit-merge form should come from the real bitfield.
- Retry: `bool allDrawn=true` lives in the info slot; `if (!c->OnAppearanceDraw(*this, drawInfo)) allDrawn=false;` then
  `if (allDrawn) UpdatePrimitiveReasons = 0;` (empty retry list jumps straight to the clear).
- Locals: the draft's 3 independent CBase objects should be ONE C3DAnimationInterpolator (StartState/EndState members); GetAnims inlined.
- Early-out `if (Components.empty()) return;` happens BEFORE any local construction (matches draft) and after `g = &info` and the float load.

## 0x00A24520

# DOSSIER 0x00A24520 (847 bytes) -- C3DMeshLODInfo::BuildMesh

## 1. Retail shape (xdis 00a24520 0x35f)
- thiscall, `ret 8` (two stack args), returns bool in al (`xor al,al` / `mov al,1`).
- NO frame pointer (`sub esp,34h; push ebx; push ebp; push esi; mov ebp,ecx; ... push edi`): ebp = this. /Oy on (no `optimize("y",off)`).
- NO SEH prologue even though two locals have destructors (CCharString @esp+14 -> ~CCharString 0x0099EAE0; CMemoryDataInputStream @esp+28 -> ~CDataStream 0x00993B10 called directly) => TU built without /EH, same as the same-file siblings 0x00A24D70 / 0x00A26AD0 (`// Flags: none`, prologue `sub esp,0Ch; push esi` without SEH).
- Speed-optimised: loop top 16-aligned with `lea esp,[esp]` filler at 0x00A246BC. No `optimize("s")` idioms.
- Frame: [esp+11]/[12]/[13] = bools type==5 / type==2 / type==4 (from `GetEntryDataType(index) & 0xF`); [14] = CCharString symbol; [18] = bank->TextureBank (+0x3D4); [1C] = mesh id (old g_NextMeshID); [20] = old *0x013D2D6C; [24] = old *0x013D2D70; [28..] stream. Arg1 [esp+48] is NEVER read. Arg2 [esp+4C] (the flag) is pushed to the first LoadBinary only, then its home slot is REUSED as the spill of the primitive index i (0x00A246B2 `mov [esp+4c],esi`, 0x00A2473D reload, 0x00A24756 store).
- Registers: ebp=this, edi=&this->Mesh (`lea edi,[ebp+1Ch]` held to the end), ebx=pool (+0x370) before the loop then `bl`=ok (`mov bl,1`; `and bl,al`), esi=bank then i (reloaded after Materials clobbers it).

## 2. Identity (two sources)
- Ego_r.pdb `ghidra_out/egor_pdb_names.tsv` + `egor_signatures.tsv`: `?BuildMesh@C3DMeshLODInfo@@AAE_N_N0@Z` (Ego_r 0x00B60960); FableWin.pdb `_fablewin_api.tsv` / `naming_stragglers/FableWin.exe.signatures.tsv` same mangled name (0x0179B260, 0x0313AF30).
- => `private: bool __thiscall C3DMeshLODInfo::BuildMesh(bool, bool)` -- PRIVATE (AAE), non-const, non-virtual. Manifest label correct.
- Callers (xref): 0x00A2547E in C3DMeshLODInfo::OnResourceLoaded 0x00A25470 (`push 0; push 1` = BuildMesh(true,false)); 0x00A25F77 and 0x00A260FE in C3DMeshInfo::GetMesh 0x00A25E70 (`BuildMesh(true,false)` and `BuildMesh(true, (flags>>6)&1)`). First arg always true; unused in retail body.
- Old draft name "unusedFlag, flag" is fine; landed callers call the params (fromFileData, stats) / (a, b) -- names unproven.

## 3. Callees (landed row / landed name / PDB)
| addr | landed row | landed spelling | PDB (Ego_r) | flag |
|---|---|---|---|---|
| 0x009CC210 | 00/9c/CBankFile_GetEntryDataType_009cc210.cpp | CBankFile_Retail::GetEntryDataType(ul) const | ?GetEntryDataType@CBankFile@@QBEKK@Z | ok (CMeshDataBank : CBankFile under FABLE_RETAIL_CMESHDATABANK_BASES + CBANKFILE_ENTRYQUERY) |
| 0x009D5530 | 00/9d/CBankFileAsyncData_GetData_009d5530.cpp | void* CBankFileAsyncData::GetData() | ?GetData@CBankFileAsyncData@@QAEPAXXZ | ok |
| 0x00994930 | 00/99/CMemoryDataInputStream_CMemoryDataInputStream_00994930.cpp | (const unsigned char*, long) | ??0CMemoryDataInputStream@@QAE@PBXK@Z (const void*, unsigned long) | landed spelling differs from PDB; header CMemoryDataInputStream.h (FABLE_RETAIL_CMEMORYDATAINPUTSTREAM) has the PDB one |
| 0x009FC4F0 | 00/9f/CResourceList_MakeFreeSpace_009fc4f0.cpp | bool MakeFreeSpace(ul) | ?MakeFreeSpace@CResourceList@@QAE_NK@Z | draft declared void -- returns bool (result ignored) |
| 0x009CC1D0 | 00/9c/CBankFile_GetEntrySymbol_009cc1d0.cpp | CCharString GetEntrySymbol(ul) const | ?GetEntrySymbol@CBankFile@@QBE?AVCCharString@@K@Z | draft on CMeshDataBank non-const |
| 0x00BFEA1A | operator new | | | |
| 0x00A89190 | 00/a8/C3DMesh2_C3DMesh2_00a89190.cpp | C3DMesh2::C3DMesh2() | ??0C3DMesh2@@QAE@XZ | ok |
| 0x00A299C0 | 00/a2/CCountedPointer00a281e0_Reset_00a299c0.cpp | CCountedPointer_00a281e0::Reset (deleter 0x00A281E0) | = CCountedPointer<C3DMesh2>::Reset (template; out-of-line COMDAT) | use shared engine/CCountedPointer.h Reset |
| 0x00A8AD40 | NOT LANDED | -- | ?LoadBinary@C3DMesh2@@QAE_NAAVCMemoryDataInputStream@@PAVCMovableResourceMemoryPool@@PBVCGraphicDataBank@@_N@Z | DRAFT WRONG: 3rd param is `const CGraphicDataBank*` (= bank->TextureBank +0x3D4), not "unsigned long flags" |
| 0x00A02730 | 00/a0/CHandleBasedMemoryPool_Defragment_00a02730.cpp | CHandleBasedMemoryPool::Defragment() | ?Defragment@CHandleBasedMemoryPool@@QAEXXZ | draft puts it on CMovableResourceMemoryPool (base call is fine if it derives) |
| 0x00994570 | 00/99/CMemoryDataInputStream_OpenMemorySource_00994570.cpp | OpenMemorySource(const void*, ul) | ?OpenMemorySource@CMemoryDataInputStream@@QAEXPBXK@Z | ok |
| 0x00A89380 | 00/a8/C3DMesh2_CalculateTextureChangeFlags_00a89380.cpp | | ?CalculateTextureChangeFlags@C3DMesh2@@QAEXXZ | ok; condition is `if (texture_bank)` not `if (flags)` |
| 0x00A2EC40 | 00/a2/C3DMeshStats_InitialiseMeshGenerators_00a2ec40.cpp | (CMeshDataBank*, const CCharString&) | ?InitialiseMeshGenerators@C3DMeshStats@@QAEXABVCMeshDataBank@@ABVCCharString@@@Z | PDB takes `const CMeshDataBank&` (draft/landed pointer) |
| 0x00A89250 | 00/a8/C3DMesh2_LockPrimitive_00a89250.cpp | LockPrimitive(long) | ?LockPrimitive@C3DMesh2@@QAEPAVC3DPrimitive2@@K@Z (unsigned long) | landed `long` vs PDB `unsigned long` |
| 0x00A892A0 | 00/a8/C3DMesh2_UnlockPrimitive_00a892a0.cpp | UnlockPrimitive(long) | ?UnlockPrimitive@C3DMesh2@@QAEXK@Z (unsigned long) | same |
| 0x00A92870 | 00/a9/C3DPrimitive2_UploadAnimatedBuffers_00a92870.cpp | bool (ul) | ?UploadAnimatedBuffers@C3DPrimitive2@@QAE_NK@Z | ok |
| 0x00A932C0 | 00/a9/C3DPrimitive2_UploadRepeatedBuffers_00a932c0.cpp | bool (ul flags, ul reps) | ?UploadRepeatedBuffers@C3DPrimitive2@@QAE_NKK@Z | ok |
| 0x00A93320 | 00/a9/C3DPrimitive2_UploadStaticBuffers_00a93320.cpp | bool (ul) | ?UploadStaticBuffers@C3DPrimitive2@@QAE_NK@Z | ok (egor_pdb_names maps Ego_r addr only; ignore that collision) |
| 0x00BFE9BC | operator delete | | | |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | | |
| 0x00993B10 | 00/99/CDataStream_CDataStream_00993B10.cpp | CDataStream::~CDataStream | ??1CDataStream@@UAE@XZ | stream dtor = direct call to ~CDataStream (CDataInputStream.h documents this) |
| 0x009FC380 | 00/9f/CResourceBank_AddResource_009fc380.cpp | CResourceBank_Retail::AddResource(CResource*) | ?AddResource@CResourceBank@@QAEXAAVCResource@@@Z | PDB takes `CResource&`; shared CResourceBank.h already declares the reference form |
| 0x00A893C0 | 00/a8/C3DMesh2_Sizeof_00a893c0.cpp | Sizeof() const | ?Sizeof@C3DMesh2@@QBEKXZ | ok |
| 0x009D5620 | 00/9d/CBankFileAsyncData_Sizeof_009d5620.cpp | Sizeof() const | ?Sizeof@CBankFileAsyncData@@QBEKXZ | ok |
| 0x009FBF20 | 00/9f/CResource_SetResourceSize_009fbf20.cpp | CResource::SetResourceSize(ul) | ?SetResourceSize@CResource@@QAEXK@Z | ok |
- Indirect: `call [eax+4]` with ecx=[eax+8] = CCPPointerInfo::DeleteFunc(Data) inside inline ReleaseData (twice).
- Globals: 0x01397248 = g_NextMeshID (landed name in C3DMeshInfo_BeginLoading_00a256f0 / BeginLoadingStats_00a24a80); 0x013D2D70 = g_ParticleMemoryPool (landed name in 4 rows, e.g. 00/b8/CEngineClothSimulationStaticMesh_RunSimulation_00b81f30.cpp) -- receives bank->ClothMemoryPool (+0x368); 0x013D2D6C has NO landed name (draft: g_MeshDataMemoryPool) -- receives bank->MeshDataMemoryPool (+0x370). No PDB global symbol found for either in egor/fablewin tsvs (likely file-statics); keep landed names; the 0x13D2D6C name is descriptive -- mark in NOTES.

## 4. Members
- this (C3DMeshLODInfo, engine/C3DMeshLODInfo.h, flat, no opt-ins; PDB 0x34): +0x1C Mesh (CCountedPointer<C3DMesh2>: Mesh_Data/Mesh_Info), +0x24 FileData (CCountedPointer<CBankFileAsyncData>), +0x2C MeshInfo (C3DMeshInfo*), +0x30 LOD (uchar). Base CResource at +0 (0x1C bytes); SetResourceSize is a CResource member. The header has no base class and no BuildMesh decl; landed siblings use `#define C3DMeshLODInfo C3DMeshLODInfoLayout` + derive (ConvertToMeshStats 0x00A24870, OnResourceLoaded 0x00A25470).
- MeshInfo->+4 Bank, +8 Index: CMeshDataBankEntry (engine/CMeshDataBankEntry.h under FABLE_RETAIL_CMESHDATABANKENTRY; base of C3DMeshInfo). Inline GetDataType() = `PeekBank()->GetEntryDataType(Index)` (non-BUILD) / `GetBank()->...` (BUILD). Bank/GetBank private under FABLE_RETAIL_C3DANIMATIONINFO_BUILD (friend C3DAnimationInfo only) -- watch access.
- CMeshDataBank (engine/CMeshDataBank.h; opt-ins FABLE_RETAIL_CMESHDATABANK (CResourceBank types), _BASES (CBankFile base), C3DANIMATIONINFO_BUILD, _TEXTUREBANK (GetTextureBank() inline), ...): +0x170 MeshResourceBank, +0x1EC MeshStatsResourceBank (CResourceBank; ResourceList at +4 => MakeFreeSpace ecx = +0x1F0; `GetResourceList()` accessor in CResourceBank.h), +0x368 ClothMemoryPool_Data, +0x370 MeshDataMemoryPool_Data (CCountedPointer<CMovableResourceMemoryPool> Data), +0x3D4 TextureBank (CGraphicDataBank*), +0x3D8 DP3BumpEnable (bool). The header offsets equal retail offsets: the draft's "0x74 below PDB view" comment is obsolete.
- CBankFileAsyncData (engine/CBankFileAsyncData.h): +0x10 Length, inline `GetLength()`; GetData() out of line.
- CMemoryDataInputStream (engine/CMemoryDataInputStream.h, FABLE_RETAIL_CMEMORYDATAINPUTSTREAM; base CDataInputStream FABLE_RETAIL_CDATAINPUTSTREAM): stream+8 = StreamSize, inline GetLength() -> MakeFreeSpace(stream.GetLength()) pushes [esp+30].
- C3DMesh2 (engine/C3DMesh2.h): +0x8C Materials, +0x90 PrimitiveCount (draft: NumPrimitives), +0xE8 MeshID; accessors GetMaterial(i) under FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS, GetMeshID under BUILD. Header does NOT declare ctor/LoadBinary/LockPrimitive/UnlockPrimitive/Sizeof/CalculateTextureChangeFlags -- they must be added (opt-in) or declared via a derived helper.
- C3DPrimitive2 +4 MaterialIndex (long, PDB). C3DMeshMaterial (0x30) +0x20 MapFlags (ulong; retail `test byte ptr [..+20h],2`).
- Draft models LOCALLY all of these although shared headers exist: CCountedPointer, SCountBlock, CDataInputStream, CMemoryDataInputStream, CBankFileAsyncData, CMovableResourceMemoryPool, CResourceList, CResourceBank, CMeshDataBank, C3DMeshMaterial, C3DPrimitive2, C3DMeshStats, C3DMesh2, CResource, C3DMeshInfo, C3DMeshLODInfo (CCharString has no engine header; use FABLE_RETAIL_CCHARSTRING as the BUILD siblings do).

## 5. Visibility
- C3DMeshLODInfo::Sizeof() const (?Sizeof@C3DMeshLODInfo@@QBEKXZ) is INLINE here: tail `p ? p->Sizeof()+8 : 8` mesh first (esi) then FileData, `lea ecx,[eax+esi+34h]` -> SetResourceSize(Sizeof()). Out-of-line copy landed at 0x00A244E0 (just before BuildMesh, same TU, unreferenced) + twin 0x00A2B1C0. Copy its body order (00/a2/C3DMeshLODInfo_Sizeof_00a244e0.cpp).
- CCountedPointer<C3DMesh2>::Reset: called OUT of line (0x00A299C0) for `Reset(new C3DMesh2)`. Release on failure and FileData release are INLINE with Info zeroed BEFORE Data ([edi+4]=0 then [edi]=0; [ebp+28]=0 then [ebp+24]=0) = the shared header's DEFAULT ReleaseData (do NOT define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA, which is Data-first). Same-file sibling 0x00A24D70 spells its release `FileData.Reset(0)` and retail inlines it there.
- GetLength() of stream / FileData, GetEntryDataType via GetDataType(), GetTextureBank(), GetResourceList(): inline accessors exist in the headers listed in 4.
- No STLport callees.

## 6. Siblings (same source file, MeshDataBank cluster 0x00A23C00-0x00A2F...)
- 00/a2/C3DAnimationInfoCAnimationData_BuildSharedAnimation_00a24d70.cpp and C3DAnimationInfo_BuildAnimationInstance_00a26ad0.cpp: VERIFIED_MATCH, `// Flags: none`, no pragmas, defines FABLE_RETAIL_BASE_OBJECT, CCHARSTRING, C3DANIMATIONINFO_BUILD, C3DANIMATIONINFO, CMESHDATABANKENTRY, CMESHDATABANK_BASES, CMESHDATABANK, CBANKFILE_ENTRYQUERY, DEF_CLASS_BASE, CIVCOUNTEDPOINTER, C3DANIMATION2, CMOVABLERESOURCEMEMORYPOOL, CHANDLEBASEDMEMORYPOOL, SCOPED_PTR, CRESOURCE_RESOURCELIST, CRESOURCELIST; includes CBankFileAsyncData.h, C3DAnimationInfo.h, CMovableResourceMemoryPool.h, C3DMesh2.h. Closest analogue (build-from-file-data with Reset(new), Defragment retry, FileData.Reset(0), bank resource AddResource(*this)). Best template.
- 00/a2/C3DMeshLODInfo_ConvertToMeshStats_00a24870.cpp: Layout-alias + derive idiom, Sizeof inline helper.
- 00/a2/C3DMeshLODInfo_OnResourceLoaded_00a25470.cpp, C3DMeshInfo_GetMesh_00a25e70.cpp (uses `#pragma auto_inline(off/on)` locally), C3DMeshInfo_BeginLoadingStats_00a24a80.cpp (g_NextMeshID++ idiom).

## 7. Old draft model errors vs disassembly
1. LoadBinary 3rd arg is `const CGraphicDataBank* texture bank` (bank->TextureBank +0x3D4), and `if (flags) CalculateTextureChangeFlags()` is `if (texture_bank)`; +0x3D8 is DP3BumpEnable (not UseSoftwareVertices).
2. Local structs everywhere instead of shared headers (strict verifier rejects); CResourceBank::AddResource / InitialiseMeshGenerators take references in the PDB.
3. Register residue: retail spills i into the dead param-2 home and keeps ok in bl; esi is reused for `Mesh->Materials` (reload of `[edi]` then `[+8C]` each iteration) and i is reloaded from the spill. Draft spilled ok. Ideas: make i the long-lived value the allocator spills (e.g. `unsigned long i` declared before ok / the loop counter as an outer-scope local), keep `ok` a bool updated with `ok &= ...` (retail `and bl,al`), access materials through `Mesh->GetMaterial(prim->MaterialIndex)->MapFlags & 2` (fresh Mesh load each iteration), and `MeshInfo->...->DP3BumpEnable` re-read through this->MeshInfo (retail reloads [ebp+2C]->[+4]).
4. Loop form: entry test `PrimitiveCount == 0` (jbe) then top `if(!ok) break`-style test, bottom `cmp i,PrimitiveCount; jb top` with PrimitiveCount reloaded => `for (i = 0; i < Mesh->PrimitiveCount && ok; ++i)` (draft order is right).
5. Upload dispatch: four branches, repeated (type 2) and type 4 BOTH call UploadRepeatedBuffers(flags, 0x20) separately; animated/static get `flags | 8`; uploadFlags = 2 when (MapFlags & 2) && DP3BumpEnable else 0.
6. Global-pool swap order: read *0x13D2D70 first (edx), then id = g_NextMeshID++, then save *0x13D2D6C, store 0x13D2D6C = bank->MeshDataMemoryPool, store 0x13D2D70 = bank->ClothMemoryPool. Restore after the loop only (the second-LoadBinary failure path jumps straight to the release and does NOT restore -- draft matches).
7. TextureBank and pool are read into locals AFTER the MakeFreeSpace block and BEFORE GetEntrySymbol (retail reads +0x3D4, +0x370 fresh, not the earlier cached global value).
8. Success: AddResource(*this) on `LOD == 4 ? MeshStatsResourceBank : MeshResourceBank` via MeshInfo->Bank re-read in each arm (`push ebp` hoisted), then FileData release (Info-first), then SetResourceSize(Sizeof()) inline.

## 0x0054FBC0

# DOSSIER 0x0054FBC0 -- NUISystem::CText::SetValue(unsigned long, const CWideString&, bool)

## 1. Retail shape (xdis 0054fbc0 0x360)
- __thiscall, virtual, `ret 0xc` (0x0054FEDF). 848 bytes; next fn 0x0054FF10 (GetValue, `lea eax,[ecx+15c]; ret`).
- NO frame pointer (sub esp,0x14; push ebx/ebp/esi/edi). NO SEH prologue although CWideString temporaries with dtors
  live across calls -> TU built without /EHsc (no unwind). FitInWindow 0x0054F8E0 (same TU) has the same prologue style.
- ebp = 0 constant (xor ebp,ebp) used as zero for compares and as the loop index of the GAME group loop; `this` in ebx,
  spilled to [esp+0x10] (reloaded at 0x0054FE6C after the front-end loop clobbers ebx).
- Argument slots reused: textID [esp+0x28], text [esp+0x2c], flag [esp+0x30] (after 4 pushes). The by-value
  CIVCountedPointer results land in the `text` slot (0x2c), GetText results in the `flag` slot (0x30, plain path) or
  frame slots; FormatText hidden result in flag slot (0x30 after push).
- `test textID: cmp esi,ebp ; jbe` -> textID unsigned (K), `if (textID)` / `textID > 0`.
- Flow: if !textID -> Value = text (0x0054FEF9). elif CMainGameComponent instance (0x013B86A0) -> game bank (+0x14);
  elif CNewFrontendGameComponent instance (0x013B871C) -> front-end bank (+0x60); else Value = text (0x0054FEE2).
  Both bank arms: if (bank.GetEntryDataType(id)) group else plain. The front-end PLAIN arm is cross-jumped into the game
  plain arm (0x0054FD73 `je 0x54fc05` with ecx = FE bank and args pushed) -> write both plain arms identically
  (`Value = bank.GetPTextEntry(id,true)->GetText();`), compiler tail-merges.
  NOTE the args `push 1; lea edx,[esp+30]; push esi; push edx` are pushed BEFORE the branch on GetEntryDataType: both
  GetPTextEntry and GetPGroupEntry share (result, id, true) -> common-subexpression hoist, natural from two calls.
- GAME group arm: `edi = &GetPGroupEntry(id,true)->Entries` taken, then temporary released (no copy!) -> source keeps a
  reference/pointer to the group's Entries after the temporary dies (`const CArray<long>& ids = ...->Entries;`).
  Value = g_EmptyWideString (0x013BCA24, operator= 0x0099B7D0); loop `for (i=0; i<ids.size(); ++i)` with size recomputed
  each pass from [edi+4]-[edi] (sar 2, signed elems, unsigned `jb` compare); pre-test before loop (jbe).
  Body: if (Value.GetLength()) Value += flag ? L" "(0x01249F28) : L"\n"(0x01231728) (the ?: selects the pushed pointer,
  one call 0x0099B940); Value += bank.GetPTextEntry(ids[i],true)->GetText() (0x0099B8D0) then ~CWideString, release.
  The game loop RE-READS the global instance and +0x14 each pass (0x0054FCD8).
- FRONT-END group arm: COPIES the vector: `CArray<long> ids(FE.GetPGroupEntry(id,true)->Entries)` via vector copy ctor
  0x00514160 (frame slot esp+0x18), temp released, Value = empty, size hoisted ONCE into ebx (start in ebp, i in edi),
  loop same body (re-reads FE global per pass), then inline vector dtor `if (start) free(start)` (0x00BFEA14).
- Tail (0x0054FE70): if (CMainGameComponent instance) Value = CSubtitleRenderer::FormatTextWithGameActionNames(Value, 0);
  then Size.Y = (float)FitInWindow(Value, Window) (fild/fstp +0xd0); Size.X = PFont->GetStringExtents(Value.GetAsCharArray(), false).X
  (vcall slot 1 [vtbl+4], hidden result, `push 0` = the bool; first dword -> +0xcc). Order: FitInWindow then extents.

## 2. Name / signature
- PDB (Ego_r / FableWin, ghidra_out/egor_pdb_names.tsv @005c9db0, _fablewin_api.tsv): ?SetValue@CText@NUISystem@@UAEXKABVCWideString@@_N@Z
  = public virtual `void NUISystem::CText::SetValue(unsigned long, const CWideString&, bool)`.
- vtable 0x01249CCC slot 143 (rebuild/manifest/rtti-vtables.tsv); xref: only reference is the vtable (.rdata 0x01249F08).
  Header rebuild/include/engine/NUISystem_CText.h already declares it at slot 143. Manifest label CText::SetValue is RIGHT.
- Caller 0x0054F410 SetValue(const CWideString&,bool,bool) (landed) calls it via +0x23c with keepSize as 3rd arg; the
  third parameter selects " " vs "\n" join (landed caller names it keepSize -- name descriptive; draft: spaceSeparated).

## 3. Callees (landed file / landed name / PDB) -- draft spelling mismatches flagged
- 0x009CC210 rebuild/src/compiled/00/9c/CBankFile_GetEntryDataType_009cc210.cpp ?GetEntryDataType@CBankFile@@QBEKK@Z (NGameText::CDataBank : CBankFile). OK.
- 0x009C9070 .../NGameTextCDataBank_GetPTextEntry_009c9070.cpp ?GetPTextEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCTextEntry@NGameText@@@@K_N@Z.
  **DRAFT WRONG: called it GetPGroupEntry returning CGroupEntry.**
- 0x009C8890 .../NGameTextCDataBank_GetPGroupEntry_009c8890.cpp ?GetPGroupEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCGroupEntry@NGameText@@@@K_N@Z.
  **DRAFT WRONG: GetPGroupListEntry / CGroupListEntry.**
- 0x009C7A40 .../NGameTextCTextEntry_GetText_009c7a40.cpp ?GetText@CTextEntry@NGameText@@QBE?AVCWideString@@XZ. **DRAFT WRONG: CGroupEntry::GetText.**
- 0x0099B7D0 CWideString::operator=(const CWideString&); 0x0099B8D0 ??YCWideString@@QAEABV0@ABV0@@Z; 0x0099B940 operator+=(const wchar_t*);
  0x0099B220 ?GetLength@CWideString@@QBEJXZ; 0x0099B510 ~CWideString; all in rebuild/src/compiled/00/99/.
- 0x0099AF10 ?GetAsCharArray@CWideString@@QBEPBGXZ (no params). **DRAFT WRONG: GetString(long); the `push 0` before it is the extents bool.**
- 0x00514160 rebuild/src/compiled/00/51/stdvector_vector_00514160.cpp: STLport vector<long> copy ctor (CArray<long> copy). Draft's CIDVector is made up.
- 0x004917B5 .../00/49/CSubtitleRenderer_FormatTextWithGameActionNames_004917b5.cpp
  ?FormatTextWithGameActionNames@CSubtitleRenderer@@SI?AVCWideString@@ABV2@PAV?$vector@VCWideString@@...@std@@@Z
  = static __fastcall CWideString (const CWideString&, std::vector<CWideString>*). **DRAFT WRONG: free fn with bool flag; 2nd arg is a null vector pointer.**
  Not declared in engine/CSubtitleRenderer.h (grep finds no decl) -> declare in the CSubtitleRenderer class locally or add.
- 0x0054F8E0 .../00/54/NUISystemCText_FitInWindow_0054f8e0.cpp ?FitInWindow@CText@NUISystem@@IAEJAAVCWideString@@ABVC2DBoxF@@@Z (protected, returns long). Not in the shared header.
- vcall [PFont vtbl+4]: CFontBank slot 1 `virtual C2DExtentsF GetStringExtents(const wchar_t*, bool) const = 0` (engine/CFontBank.h:36).
  (FABLE_RETAIL_CFONTBANK_STRING_HELPERS offers the out-of-line non-virtual GetStringExtents(const CWideString&,bool) 0x0048DAA2 -- retail does NOT call it here, it calls slot 1 directly with GetAsCharArray.)
- free 0x00BFEA14 (vector dtor, inline).
- Release: inline CIVCountedPointer dtor -> pointee slot 1 Delete ([vtbl+4]).

## 4. Members / globals
- this: NUISystem::CText, shared header rebuild/include/engine/NUISystem_CText.h with `#define FABLE_RETAIL_CTEXT_VSLOTS`:
  Value CWideString +0x15c (Ego_r +396; draft called it Text), PFont CCountedPointer<CFontBank> +0x160 (Data at +0), Window C2DBoxF +0x178.
- Size C2DVector +0xcc (X +0xcc, Y +0xd0) from engine/NUISystem_CComponent.h:232 (Ego_r CComponent.Size +220). Draft named them Width/LineCount (wrong).
- Game instance 0x013B86A0: CMainGameComponent::Get() (static PMainGameComponent), text bank: engine/CMainGameComponent.h
  `const NGameText::CDataBank& GetTextBank() const` (PTextBank counted ptr object word +0x14; FableWin ?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ inline).
  Check which macro guards GetTextBank (near CMainGameComponent.h:237-240).
- Front end 0x013B871C: engine/CNewFrontendGameComponent.h, private static m_pNewFrontendGameComponent, inline Get() and GetTextBank() (+0x60).
  Landed sibling 0x0054F410 instead uses a local stand-in g_pNewFrontendGameComponent -- the shared header is the right model.
- Group ids: NGameText::CGroupEntry::Entries CArray<long> +0x1c (engine/NGameText_CDataBank.h, FABLE_RETAIL_NGAMETEXT_GROUPENTRY;
  also needs FABLE_RETAIL_DEF_CLASS_BASE). CArray : std::vector (engine/CArray.h).
- Empty string 0x013BCA24 (draft g_EmptyWideString; check fable_string.h for the established spelling of the static empty CWideString).
- Classes the draft models LOCALLY that have shared headers: CWideString (fable_string.h), CIVCountedPointee/CIVCountedPointer
  (engine/CIVCountedPointeeBase.h, engine/CIVCountedPointer.h), CTextBank (=NGameText::CDataBank, engine/NGameText_CDataBank.h),
  CMainGameComponent, CFrontEnd (=CNewFrontendGameComponent), CFont (=CFontBank, engine/CFontBank.h), C2DBoxF, NUISystem::CText, CArray.
- Header caveat: NUISystem_CText.h derives CText from CTextContainer, but ghidra_out/rtti_bases.tsv:2517 says CText's direct base is
  CChangingStateComponent; harmless only while FABLE_RETAIL_CTEXTCONTAINER_MEMBERS is NOT defined (it would put Window at +0x15c and shift).

## 5. Visibility / opt-ins
- Release idiom in retail: `mov esi,[eax+4]; mov ecx,eax; add eax,4; dec esi; mov [eax],esi; mov edx,esi; cmp edx,0; jne; call [vtbl+4]`
  (count loaded BEFORE the ecx copy, decremented in a register). CIVCountedPointer.h documents exactly this for
  FABLE_RETAIL_CIVCOUNTEDPOINTER + FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER + FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
  (row 0x00A9BBE0). Alternative FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT (`dec [eax]` memory form -- the draft's residue, i.e. NOT retail here).
  Null stores after release appear at 0x0054FC85, 0x0054FD2C, 0x0054FE4E but not at 0x0054FC16/0x0054FDAB (dead-store variance).
- Same TU, defined earlier: FitInWindow 0x0054F8E0, SetValue 0x0054F370/0x0054F410, GetCharWidthW 0x0054F4B0, ForceWidth 0x0054FB10 -- all called out of line.
- CWideString members are out of line (0x0099xxxx library TU). vector<long> copy ctor 0x00514160 out of line; vector dtor inline (free).
- CMainGameComponent::Get / GetTextBank and CNewFrontendGameComponent::Get / GetTextBank inline (globals re-read each use).

## 6. Siblings (rebuild/src/compiled/00/54/)
- NUISystemCText_SetValue_0054f410.cpp (slot 144, calls this row; local models; spells CMainGameComponent::Get(), GetGameTextDataBank inline).
- NUISystemCText_SetValue_0054f370.cpp (slot 145). NUISystemCText_ForceWidth_0054fb10.cpp (Value +0x15c, GetAsCharArray).
- NUISystemCText_GetDynamicSize_0054ff50.cpp / _MakeAvailable_0054ff90.cpp: `#define FABLE_RETAIL_CTEXT_VSLOTS` + `#include "engine/NUISystem_CText.h"`, `return Size;`.
- NUISystemCText_FitInWindow_0054f8e0.cpp (includes engine/CCountedPointer.h). No `// Flags:` / pragmas in these rows (default flags, no /EHsc evidence).

## 7. Old draft model errors
- Wrong callee names/types (GetPTextEntry vs GetPGroupEntry, CTextEntry, GetAsCharArray, FormatText signature/class).
- Member names Text/Width/LineCount -> Value/Size.X/Size.Y; local CText layout instead of shared header.
- Release through `--Obj->RefCount` memory form -> retail is register-count Release (see 5).
- Game group loop must NOT copy the vector and must re-evaluate size each pass; FE group loop copies (CArray<long>) and hoists size.
- Draft globals g_pFrontEnd/CFrontEnd/CTextBank* are made up -> CNewFrontendGameComponent::Get()->GetTextBank(), CMainGameComponent::Get()->GetTextBank().
- Extents call: draft `Measure(GetString(0))` slot 1 with one arg; retail passes (GetAsCharArray(), false) with hidden C2DExtentsF result.

## 0x004C0560

# DOSSIER 0x004C0560 (851 bytes) -- NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint

## 0. Biggest fact
A complete original-spelling body ALREADY EXISTS in the shared header
`rebuild/include/engine/NGraphicAppearance_CTCGraphicAppearance_HelperPoint.h` (Codex pass17 "Final039",
"native004C0560..004C08B3, donor01D0C5D0"). The landed caller GetFocalPos 0x004C1060
(`rebuild/src/compiled/00/4c/NGraphicAppearanceCTCGraphicAppearance_GetFocalPos_004c1060.cpp`) includes it with:
`FABLE_RETAIL_STLP_SORT_FASTCALL, FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY, FABLE_RETAIL_CTHING,
FABLE_RETAIL_CTHING_GETTC_EXPANDED, FABLE_RETAIL_C3DVECTOR, FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT,
FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL` then `#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"` and the
HelperPoint header. Start from that (copy the define set), not from the old draft. No record found that this body was
ever graded against 0x004C0560 (hand301 STATUS only has the old DIFFER(786v851) draft).

## 1. Retail shape (xdis 004c0560 0x363)
- `__thiscall`, `ret 14h` (5 args: name +0x4, n +0x8, flags +0xC, point& +0x10, interpolate +0x14 in the caller frame).
- No frame pointer (`sub esp,0B0h ; push ebx ; push ebp ; push esi ; push edi`), ebp = `this`, also homed to [esp+24h]
  because ebp is later reused as the CMeshInfo iterator. No SEH prologue although C3DMeshObject / C3DAnimationInterpolator
  locals have dtors -> TU built WITHOUT /EHsc (lane default). No optimize("s") idioms; speed /O2 (`mov edi,edi`,
  `lea ecx,[ecx]` loop-alignment pads at 0x4C060D / 0x4C066E).
- Two exits: false tail 0x4C0780 (`xor al,al`), true tail 0x4C08A4 (`mov al,1`); every exit frees the vector buffer
  (`free` 0x00BFEA14) inline.
- Bool slots: force_accurate in al, reduced_lod [esp+11h] (stored 0 before the flags&1 test), ignore_animation [esp+12h],
  return_if_not_loaded [esp+13h]. Retail order = header body order (reduced_lod=false; force_accurate=true; if(flags&1)...;
  ignore_animation=false; if(flags&4)...).
- `start_loading` computed as `shr ebx,1 ; not bl ; and bl,1` (clobbers flags in ebx; reloaded from [esp+0CCh] at 0x4C075D
  on the continue path) -> `bool start_loading=(flags&2)==0; bool return_if_not_loaded=!start_loading||(flags&32)!=0;`.

## 2. Name / signature (manifest label is RIGHT)
- `?GetHelperPoint@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@M@Z`
  - Ego_r.pdb: ghidra_out/egor_pdb_names.tsv `004c1780` (Ego_r address space, NOT retail; Ego_r neighbour order
    GetHelperPoint 4c1780 / GetDummyObject 4c1b40 / GetDummyObjectAssumingObjectAt 4c1f80 = retail 4c0560 / 4c08c0 / 4c0c80).
  - FableWin.pdb: ghidra_out/_fablewin_api.tsv `017b4260` (a thunk address).
  - Callers: GetFocalPos 0x004C1060 (calls at 0x4C107A / 0x4C1094, landed), GetStaticFocalPosIfLoaded 0x004C11F0 (landed,
    cites 0x004C0560 with this signature), + ~78 more (xref.py: 80 call sites).
- `bool GetHelperPoint(const CCharString& name, long n, unsigned long flags, C3DVector& point, float interpolate) const;`
  Declared in NGraphicAppearance_CTCGraphicAppearance.h under `FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL`.
- Old draft wrong: `C3DVector*` (PDB `AAV` = reference), non-const (PDB `QBE`), class not in namespace NGraphicAppearance.

## 3. Callees (retail addr -> landed row -> correct spelling ; draft spelling)
| addr | landed row | correct (PDB / header) | draft |
|---|---|---|---|
| 0x004BD2E0 | 00/4b/NGraphicAppearanceCTCGraphicAppearance_IsToReduceLODForDummiesAndHelpers_004bd2e0.cpp | `IsToReduceLODForDummiesAndHelpers() const` (protected, IBE) | ok name, missing const |
| 0x004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound -- via inline CThing::PeekTC (GETTC_EXPANDED: bit test [thing+28h]&0x4000000 = TC 0x5A, Components +0x44) | local CTCMap::LowerBound (WRONG model) |
| 0x0070B470 | 00/70/CWorld_DrawGetEnvironment_0070b470.cpp (ICF body `mov eax,[ecx+0Ch]`) | CTCAnimationComplex::PeekAnimation() const -> const CAnimateThing& (engine/CTCAnimationComplex.h line 20, declared out of line; header body uses it) | CTCAnimate::GetAnimateThing (WRONG) |
| 0x0070C100 | 00/70/CAnimateThing_AnimsCanBeIgnored_0070c100.cpp | CAnimateThing::AnimsCanBeIgnored() const | ok |
| 0x004C46C0 | 00/4c/stdvectorNGraphicAppearanceCMeshInfo_reserve_004c46c0.cpp | std::vector<NGraphicAppearance::CMeshInfo>::reserve (from inline GetAllMeshes) | local CMeshEntryVector::reserve (WRONG) |
| slot 2 (+8) | -- | CAGraphicAppearanceComponent::GetMeshesUsed(std::vector<CMeshInfo>&) const (engine/NGraphicAppearance_CAGraphicAppearanceComponent.h), loop over `std::list<CAGraphicAppearanceComponent*> Components` (+0x3C) | CMeshProvider::GetMeshes (WRONG) |
| 0x004C5C80 | 00/4c/global_SortSElement12004c5c80_004c5c80.cpp (placeholder name) | std::sort<CMeshInfo*,CSortPrimaryMeshesFirst> __fastcall (flags&8) | SortMeshesFrontToBack (made up) |
| 0x004C5CE0 | 00/4c/global_SortSElement12004c5ce0_004c5ce0.cpp (placeholder) | std::sort<CMeshInfo*,CSortPrimaryMeshesLast> __fastcall (flags&16) | SortMeshesBackToFront (made up) |
| 0x00A24260 | 00/a2/CMeshDataBank_PeekMeshInfo_00a24260.cpp | CMeshDataBank::PeekMeshInfo(unsigned long) const on g_pMeshDataBank 0x013B8A04 (via inline IsResourcesLoadedForMesh) | C3DMeshManager::GetMeshInfo / g_MeshManager (WRONG) |
| 0x00A23E40 | 00/a2/C3DMeshInfo_IsLoaded_00a23e40.cpp | C3DMeshInfo::IsLoaded() const | ok |
| 0x00A256F0 | 00/a2/C3DMeshInfo_BeginLoading_00a256f0.cpp | C3DMeshInfo::BeginLoading(unsigned long,bool,bool) | (long,long,long) WRONG types |
| 0x0099A2D0 / 0x0099A2E0 | 00/99/CBaseClass_CBaseClass_0099a2d0.cpp / _0099a2e0.cpp | CBaseClass::CBaseClass / ~CBaseClass | CBase (WRONG) |
| 0x004BC4F0 | 00/4b/NGraphicAppearanceCTCGraphicAppearance_GetMeshObject_004bc4f0.cpp | `GetMeshObject(const CMeshInfo&,float,C3DMeshObject&) const` (protected, IBE) | CreateAbilityMeshObject (catalog label, WRONG) |
| 0x00A36100 | 00/a3/C3DMeshStatsOwner00a36100_HasDummyObject_00a36100.cpp (made-up class) | C3DMeshObject::HasHelperPoint(const CCharString&) const (engine/C3DMeshObject.h; calls 0xA2EA20 = C3DMeshStats::HasHelperPoint per landed 0x004BE170). NAME_FIXES candidate | HasHelper |
| 0x00A36140 | 00/a3/C3DMeshObject_GetNoHelperPoints_00a36140.cpp | C3DMeshObject::GetNoHelperPoints(const CCharString&) const | GetHelperCount (WRONG) |
| 0x00A361A0 | 00/a3/C3DMeshObject_GetHelperPointPositionStatic_00a361a0.cpp | C3DMeshObject::GetHelperPointPositionStatic(const CCharString&,long) const | ok |
| 0x00A36350 | 00/a3/C3DMeshObject_GetHelperPointPositionAnimated_00a36350.cpp | C3DMeshObject::GetHelperPointPositionAnimated(const CCharString&,long,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&) const | GetHelperPointPosition(...,long frame) WRONG (last arg is C3DAnimationManager& = PeekWorld().GetAnimationManager(), inline `mov eax,[eax+44h]`) |
| 0x004C3200 | 00/4c/C3DMeshObject_C3DMeshObject_004c3200.cpp | C3DMeshObject::~C3DMeshObject (`??1C3DMeshObject@@UAE@XZ`) | ok |
| 0x004BBF00 | 00/4b/global_GetRenderPositionForThing_004bbf00.cpp | anon-namespace `C3DVector __fastcall GetRenderPositionForThing(const CThing&,float)` (file static) | GetThingInterpolatedPosition (WRONG) |
| 0x004C2E20 / 0x004C2E90 | 00/4c/C3DAnimationInterpolator_C3DAnimationInterpolator_004c2e20/2e90.cpp | C3DAnimationInterpolator ctor/dtor (out of line here) | ok |
| 0x004BC000 | 00/4b/NGraphicAppearanceCTCGraphicAppearance_GetAnims_004bc000.cpp | `bool GetAnims(C3DAnimationInterpolator&) const` (protected) | void GetAnims(C3DAnimationInterpolator*) WRONG |
| 0x00686A80 | 00/68/CTCBase_PeekWorld_00686a80.cpp | CTCBase::PeekWorld() const -> const CWorld& | GetDisplayEngine (WRONG) |
| 0x00BFE9BC / 0x00BFEA14 | operator delete / free | (counted-pointer block / vector buffer) | |

## 4. Members
- this: CTCGraphicAppearance (header NGraphicAppearance_CTCGraphicAppearance.h, `_MOVEMENT` gives the full retail layout,
  sizeof 0x5C): ParentThing +0x04 (CTCBase, `GetParentThing()`), Components `std::list<CAGraphicAppearanceComponent*>` +0x3C.
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING + _GETTC_EXPANDED): TC bitset +0x20 (word +0x28 bit 26 = TC 0x5A
  CTCAnimationComplex), Components CVectorMap +0x44.
- C3DMeshObject (engine/C3DMeshObject.h; Ego_r 120 bytes): vptr 0x01239860, MeshInfo +4, Mesh CCountedPointer<const C3DMeshStats> +8
  (Data +8 / Info +0xC), PositionMatrix +0x10, InversePositionMatrixSetup +0x40, Scale +0x74. Local frame object at [esp+48h].
- C3DMeshStats::AnimatedFlag: retail +0x7A (Ego_r +0x82, struct_layouts_egor.tsv `C3DMeshStats 132 130 bool AnimatedFlag`).
- C3DAnimationInterpolator (engine/C3DAnimationInterpolator.h, 20 bytes): StartState +4, EndState +0xC; frame [esp+28h].
- NGraphicAppearance::CMeshInfo (engine/NGraphicAppearance_CMeshInfo.h): Index +0 (12-byte stride). Comparators
  CSortPrimaryMeshesFirst / Last (1-byte empty functors, retail zero-inits a byte temp at [esp+14h] and pushes the dword).
- CWorld::PAnimationManager +0x44 (engine/CWorld.h `GetAnimationManager()const` inline, lines 319/442).
- Draft models LOCALLY although shared headers exist: CCharString (fable_string.h), C3DVector, CMeshEntry(=CMeshInfo),
  CMeshEntryVector(=std::vector), C3DMeshInfo, CMeshDataBank, CBase(=CBaseClass), C3DMeshStats, CCountedPointer, CMatrix3x4,
  C3DAnimationBlendState, C3DAnimationInterpolator, C3DMeshObject, CAnimateThing, CTCAnimationComplex, CVectorMap, CThing,
  CAGraphicAppearanceComponent, CWorld, CTCBase, CTCGraphicAppearance. All must go.

## 5. Visibility / inlining (what retail expands vs calls)
- Inlined in retail: GetAllMeshes (0x004BD420 landed, same TU earlier), IsResourcesLoadedForMesh (0x004BC090, same TU,
  inline in IsResourcesLoaded row 0x004BE3E0 -- copy its inline definitions), CThing::PeekTC (forceinline in CThing.h),
  PeekWorld()->GetAnimationManager (inline), **C3DMeshObject() ctor** (CBaseClass ctor call, vptr store, Mesh Data/Info=0,
  InversePositionMatrixSetup=false; interleaved with GetMeshObject arg pushes at 0x4C06E0..0x4C06F9), **C3DMeshObject::IsAnimated()
  const** (`mov eax,[obj+8] ; mov cl,[eax+7Ah]`, FableWin has ?IsAnimated@C3DMeshObject@@QBE_NXZ and ?IsAnimated@C3DMeshStats@@QBE_NXZ),
  **~C3DMeshObject on the loop-continue path only** (0x4C071B..0x4C0754: vptr store, CCountedPointer release Data-first zeroing
  `mov [50h],edi ; mov [54h],edi`, call ~CBaseClass).
- Called out of line: ~C3DMeshObject 0x004C3200 at both early returns (0x4C07A8 n>=count, 0x4C088E success), the
  C3DAnimationInterpolator ctor/dtor, GetMeshObject, GetAnims, IsToReduceLOD..., GetRenderPositionForThing, std::sort (fastcall).
- engine/C3DMeshObject.h currently DECLARES `C3DMeshObject();` and `bool IsAnimated()const;` without bodies and no inline
  dtor -> compiling the header body as-is will CALL them. Needs an opt-in in C3DMeshObject.h giving inline ctor/IsAnimated
  (C3DMeshStats needs AnimatedFlag at +0x7A) and an inline/visible dtor. The split inline-at-continue / call-at-returns is
  the VC7.1 inline-budget pattern (GOTCHAS: wf19 0x00D17A80 "postorder walk ... different depths at different early
  returns"; wf21 "single-site do{}while(0)"; Q31 force-inline then fix register permutation).
- Opt-ins that exist: FABLE_RETAIL_STLP_SORT_FASTCALL (stlport/fable_containers.h:1242), FABLE_RETAIL_CTHING_GETTC_EXPANDED,
  FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (GOTCHAS Q23), CCountedPointer: FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA /
  _DTOR_DECL_ONLY / _VISIBLE_CALLED_DTOR (CCountedPointer.h 106-129), FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL (forced by the
  HelperPoint header), FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY / _FOCAL / _MOVEMENT.

## 6. Siblings (same TU, retail order: ... SetAsHighlighted 4C04C0, GetHelperPoint 4C0560, GetDummyObject 4C08C0 (pending),
   GetDummyObjectAssumingObjectAt 4C0C80 (pending), GetFocalPos 4C1060, GetFocalPosIfLoaded 4C11D0, GetStaticFocalPosIfLoaded 4C11F0)
- 0x004C1060 GetFocalPos -- define set above; no pragmas/Flags.
- 0x004BE3E0 IsResourcesLoaded -- defines CTCBASE, STLP_VECTOR, CTCGRAPHICAPPEARANCE_MOVEMENT/_HELPER_BODY/_HELPERNAMES,
  CMESHDATABANK*, C3DMESHINFO_ISLOADED; inline GetAllMeshes + IsResourcesLoadedForMesh definitions (copy them).
- 0x004BE170 HasHelperPoint, 0x004BDE90 GetNoHelperPoints, 0x004BC670 GetRenderMeshObject (C3DMeshObject local, out-of-line
  dtor 0x4C3200) -- older local-model rows, no pragmas.
- 0x0076E2F0 CTCWeaponTrail::CalculateInterpolatedWeaponHelpers -- another C3DMeshObject local with inline ctor, out-of-line dtor.
- No CTCGraphicAppearance landed row uses #pragma optimize or a non-default Flags line.

## 7. Old draft model errors
- Pointer/non-const signature; wrong namespace; ~20 local stand-in classes and made-up callee names (table above).
- GetHelperPointPosition last arg modelled as `GetDisplayEngine()->Frame` long: really C3DAnimationManager& from
  PeekWorld().GetAnimationManager().
- Load logic: retail `return_if_not_loaded = !start_loading || (flags&32)` and inline IsResourcesLoadedForMesh
  (BeginLoading((unsigned long)-1,false,false)); draft's mustBeLoaded shape is equivalent but types differ.
- Comparators: must be `CSortPrimaryMeshesFirst()` / `Last()` temporaries (zeroed byte at [esp+14h]).
- Dtor split (inline at continue, call 0x4C3200 at returns) not reproduced; this homed at [esp+24h] not reproduced.

## 0x0063B3F0

# DOSSIER 0x0063B3F0 CWorldMap::CreateLineSegment (856 bytes, 0x0063B3F0..0x0063B747, int3 pad to 0x0063B750)

## 1. Retail shape (xdis.py 0063b3f0 0x368)
- `__thiscall`, `ret 0x10` (4 stack args), **esp frame, no ebp frame** (no `push ebp; mov ebp,esp`; ebp is a GP reg). No SEH. `sub esp,30h; push ebx,ebp,esi` ... `push edi` (late, at 0x63B40A, after the `*from` loads).
- Register homes: esi=from, edi=to (until 0x63B637 where edi=segStart arg), ebx=(long)to->X, ebp=(long)from->X then **(long)to->Y**.
- Frame (offsets after all 4 pushes; locals = esp+0x10..0x3F): +0x11 valid(bool, init 0, set 1 right before IsPosInMap), +0x12 crossesX(init 1), +0x13 crossesY(init 1), +0x14 int temp (fromX, later reused as ftol temp for (long)from->Y), +0x18 `this` spill, +0x1C xHit{X,Y,Z}, +0x28 yHit{X,Y,Z}, +0x34 end{X,Y,Z} (= *from copy). **dY spilled to the dead `from` arg slot (+0x44), floorX=(float)fromX to the dead `to` arg slot (+0x48)**; +0x44 reused again for |dy| temps in the distance compare, +0x48 for an ftol temp in the Y block.
- x87: dX and dZ stay on the FPU stack across both blocks (dX=st1/st3, dZ=st0/st2), dY is the only spilled delta; both are popped with two `fstp st0` at 0x63B5AD.
- Float constants: 1.0f @0x0122DED8, 0.0f @0x0122DEDC. Compares: `d > 0` -> `fcomp; test ah,41h; jne`; `0.0f == expr` -> `fld 0; fucompp; test ah,44h; jp` (constant on the LEFT, i.e. `0.0f == x` -- draft already has this); max -> `fcom; test ah,41h; je`; final `a < b` -> `fcomp st1; test ah,5; jp`.
- Return: `mov al,[esp+5]` after pops = the memory-resident `valid` byte.
- No `optimize("s")` idioms visible (no push/pop constant tricks); plain /O2 code.

## 2. Identity (two sources, label is RIGHT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv): `006e7530 ?CreateLineSegment@CWorldMap@@IBE_NPBVC3DVector@@0PAV2@1@Z` (also egor_signatures.tsv). Ego_r order IsWalkableAt 006e7520 -> CreateLineSegment 006e7530 -> FindMaxGroundZInCell 006e78e0 -> FindMaxThemeZInCell -> PointIsWithinCell* -> PointIsWithinLineSegment; retail order (rebuild/manifest/functions.tsv) 0x0063B3F0 -> FindMaxGroundZInCell 0x0063B750 (landed) -> FindMaxThemeZInCell 0x0063B7F0 -> PointIsWithinCell* 0x0063B890.. -> PointIsWithinLineSegment 0x0063BAB0: same order.
- Callers (xref.py): 0x0063DA17 in FindLastUnblockedGroundPositionAndReportCollision 0x0063D960, 0x0063DBE7 in FindLastUnblockedThemePositionAndReportCollision 0x0063DB30 (both landed, both call `CreateLineSegment(&pos, &to, &next, &cell)` and loop while it returns true).
- Signature (from the mangling, `IBE`): **protected, const, __thiscall**, `bool CreateLineSegment(const C3DVector* from, const C3DVector* to, C3DVector* segStart, C3DVector* segEnd) const`. arg3 = the crossing point (callers call it `next`), arg4 = the point identifying the cell just traversed (callers call it `cell`). Not in FableWin tables (not in _fablewin_api.tsv / FableWin signatures).
- Shared header already declares it: rebuild/include/engine/CWorldMap.h:219-238 under `FABLE_RETAIL_CWORLDMAP` + nested `FABLE_RETAIL_CWORLDMAP_LINESEGMENT` (protected:, exact spelling above). No landed row uses `FABLE_RETAIL_CWORLDMAP_LINESEGMENT` yet. `IsPosInMap(const C3DVector&) const` is declared at CWorldMap.h:110 (inside the FABLE_RETAIL_CWORLDMAP branch). Example of a row using FABLE_RETAIL_CWORLDMAP: rebuild/src/compiled/00/63/CWorldMap_GetNavigationLayerAt_0063e590.cpp.

## 3. Callees
| target | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00BFEA70 (x7) | CRT (toolchain) | `__ftol2` (sibling rows: "__ftol2"/"_ftol") | VC7.1 CRT `_ftol2` | draft comment says `_ftol`; it is emitted by `(long)float` casts, nothing to declare |
| 0x004FC600 (thiscall, push &segStart) | rebuild/src/compiled/00/4f/CWorldMap_IsPosInMap_004fc600.cpp | CWorldMap::IsPosInMap(const C3DVector&) const | `?IsPosInMap@CWorldMap@@QBE_NABVC3DVector@@@Z` (Ego_r 0x004CFDB0) | matches |
No vtable calls. IsPosInMap is a different TU (0x004FC600, WorldMap.cpp area) -> out of line, not visible; its body must NOT be visible (it isn't inlined).

## 4. Members
None read or written. `this` is only spilled (+0x18) and passed to IsPosInMap. C3DVector: struct_layouts_egor.tsv:6844 X/Y/Z float at 0/4/8, size 12 -> `#include "engine/C3DVector.h"` (its copy ctor default branch is `:X(o.X),Y(o.Y),Z(o.Z)`; inline ops via FABLE_VECTOR_INLINE).
Draft models LOCALLY: `C3DVector` (shared header engine/C3DVector.h exists -> must use it) and `CWorldMap` (engine/CWorldMap.h exists; use `FABLE_RETAIL_CWORLDMAP` + `FABLE_RETAIL_CWORLDMAP_LINESEGMENT`, or at least a minimal class without fake virtuals since no member/vtable is touched).

## 5. Visibility
- IsPosInMap 0x004FC600: separate TU, out of line, declaration only.
- Neighbouring same-TU rows (FindMaxGroundZInCell 0x0063B750 etc.) are defined AFTER this one; nothing earlier in the TU is called. No STLport helpers. The only plausible inline helper is a max-of-abs-deltas (retail inlines it twice: `|p.X-from.X|`, `|p.Y-from.Y|`, `a > b ? a : b` with b spilled to +0x44). No PDB name found for it; draft's `MaxAbs2D` is made up (fine as a static inline, but say so in NOTES).

## 6. Sibling landed rows (rebuild/src/compiled/00/63/)
- CWorldMap_FindMaxGroundZInCell_0063b750.cpp -- next function in the TU; `#include "engine/C3DVector.h"`, local CWorldMap, no pragmas, no Flags line.
- CWorldMap_PointIsWithinCellBR_0063b890.cpp / CWorldMap_PointIsWithinLineSegment_0063bab0.cpp -- same protected `IBE` pointer-arg family; `engine/C3DVector.h`, minimal local class with `protected:`, no pragmas. Note their comments: truncated X kept "in the dead first-parameter slot" -- same arg-slot packing as here.
- CWorldMap_GetHighestThemeSizeZInArea_0063b0f0.cpp -- float + ftol neighbour; `fable_compiler.h`, `<math.h>`, `engine/C3DVector.h`; LESSONS.md idiom "scoped 2-float copies" gives packed frames (also 0063bf20, 0063c230).
- Callers CWorldMap_FindLastUnblocked{Ground,Theme}...0063d960/0063db30 -- `<math.h>`, `#pragma auto_inline(off/on)` around a stand-in; their comments: "`found` lives in the dead `from` argument slot, the lower Z in the dead `to` slot" -- VC7.1 packs into dead arg slots in this TU routinely.
- No CWorldMap 0x0063xxxx row carries a `// Flags:` line or `optimize` pragma -> default lane flags.

## 7. Old draft vs retail
Control flow of the draft matches retail exactly (verified block by block): init order valid/crossesX/crossesY/end=*from; (long)from->X, (long)to->X compare; (long)to->Y computed BEFORE (long)from->Y; d=to-from; floorX=(float)fromX; xHit.X=floorX; yHit.Y=(float)(long)from->Y; X block / Y block (Y block re-truncates from->Y, does not reuse yHit.Y); end.X/end.Y re-tests with `(long)end.X == toX` giving `xor bl,bl / mov bl,[crossesX]`; both/x-only/y-only/neither tails; neither-tail `*segStart=*to; end=*to;` skips the valid=true/IsPosInMap block; `*segStart=*from; end=*from;` reloads `*from` twice (two separate copies).
Residue (NOTES.md 2026-10-23_linux_hand311): DIFFER 836v856 -- frame 8 bytes larger (dY and floorX got own slots instead of the dead from/to arg slots) and inv*t re-associated.
Likely model errors:
1. `C3DVector d(...)` as a struct: retail treats dX/dY/dZ as three independent scalars (dX,dZ on x87, dY spilled to an arg slot). Try `float dx=..., dy=..., dz=...;` (or keep the d vector but name scalars) -- the extra 8 bytes are exactly dY+floorX slots.
2. Multiplication: retail computes `inv = 1.0f/fabs(dX)` (`fld dX; fabs; fdivr 1.0`), then `(dY*inv)*t` and `(inv*dZ)*t` separately; ours CSEs `inv*t`. Suggests retail scaled the direction first, e.g. `C3DVector step = d * (1.0f / fabsf(d.X));` via the header `operator*(float)` (dead X component dropped), then `xHit.Y = step.Y * t + from->Y`. Same for Y block: `fld st0 (inv); fmul dX; fmul t; fadd from.X`.
3. Local C3DVector/CWorldMap stand-ins must go (shared headers). MaxAbs2D is a made-up helper; `__forceinline` helper is allowed only for asm per ASM_RULES -- use plain `inline`/static inline or write the ternaries inline.
4. Name fix for coordinator: landed caller 0x0063D960 declares CreateLineSegment WITHOUT `protected:` (public -> mangles `QBE`, PDB is `IBE`); 0x0063DB30 has it protected (correct).

## 0x00F3A890

# DOSSIER 0x00F3A890 CRangeCompressor::CalcBestCompressionForBlock (857 bytes)

## 1. Retail shape (xdis 00f3a890 0x369, saved $W/dis.txt)
- __thiscall, `ret 0xc` (3 args). No frame pointer (`sub esp,0xc4; push ebx,ebp,esi,edi`) -> default /O2 /Oy, no pragmas.
- NO SEH prologue although 5 vectors with dtors are live -> no /EHsc (default `// Flags: none`).
- Frame (after the 4 pushes): [0x10] = `this` spill, later reused for `best` (0x7fffffff); [0x14]/[0x18]/[0x1c] =
  &arrays[i] / &entries[i] / i (strength-reduced loop); entries[5] at 0x20..0x97 (24 B each);
  arrays[5] at 0x98..0xd3 (12 B each). Args: values=[esp+0xd8], bytes=[esp+0xdc] (first read as 0xd8 before push edi), entry=[esp+0xe0].
- ebx = 0 for the whole body; ebp = entries[0].Flags (0 / 0x20 / 0x40) and is KEPT in ebp across the
  vector::operator= call, then stored into entries[1].Flags (0x00F3A984 `mov [esp+0x40],ebp`); ebp then = this.
- Ctor zero-stores: entries[0].Flags (as ebp=0), entries[2..4].Flags; entries[1] has NO ctor store (dead, killed by the copy).
- arrays[] zeroed (15 stores) after `push edi` (values) and before `call 0x45bc09` => arrays declared after the if/else.
- Call order: operator=(arrays[0], values); TestRedundantBitStrip(values, arrays[1], entries[1], bytes);
  TestRangeBias(arrays[1], arrays[2], entries[2], bytes); entries[3]=entries[0] with Flags `&~1 |0x10` (Flags reread from
  memory [0x20] here); TestRangeBias(values, arrays[3], entries[3], bytes); TestRedundantBitStrip(arrays[3], arrays[4], entries[4], bytes).
- Loop i<5: inlined CalcHeaderSizeNeeded (esi=2; blockSize ecx = 4 if &0x40 else (&0x20)!=0 +1 via setne/inc;
  `&0x11 -> esi=ecx+2`, `&2 -> inc`, `&4 -> +ecx`, `&8 -> +ecx`) + call CalcMemoryNeededBitCompressed(&arrays[i]);
  `if (size < best) { bestIndex(edi)=i; best=size; }` (jge skip, signed).
- `entry = entries[bestIndex]` (6-dword copy into ebp=entry), then inlined CalcBitsNeeded(arrays[bestIndex]):
  edx=*begin, count=(end-begin)>>2, `cmp eax,ebx; jbe` (unsigned), or-loop counting down; bit scan cl=31..0 `1<<cl`,
  hit -> esi=bit+1 else esi=0 (xor esi,esi pre-set); `mov [ebp+4],esi` (entry.BitCount).
- Array dtor loop: 5 elements from the end (esi -= 0xc), `if (begin) free(begin)` (call 0x00BFEA14, cdecl, add esp,4).
- Return best ([0x10]).

## 2. Identity (manifest label CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00BDCA60) and FableWin.pdb (_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv,
  0x03319590): `?CalcBestCompressionForBlock@CRangeCompressor@@AAEJABV?$CArray@K@@JAAUCScriptEntry@1@@Z`.
- = private (AAE) non-virtual thiscall `long CalcBestCompressionForBlock(const CArray<unsigned long>&, long, CScriptEntry&)`.
  Already declared exactly so in rebuild/include/engine/CRangeCompressor.h (private section). Not virtual (no vtable slot).
- Caller: 0x00F3ABF0 CalcCompressionScript (call at 0x00F3AD00; parked DIFFER 605v605 in the same pending dir).

## 3. Callees
| retail | landed row | landed name | PDB name | draft disagreement |
|---|---|---|---|---|
| 0x0045BC09 | rebuild/src/compiled/00/45/stdvector_operator_0045bc09.cpp | std::vector<unsigned long>::operator= | `??4?$vector@KV?$allocator@K@std@@@std@@QAEAAV01@ABV01@@Z` | draft declares `CArray<unsigned long>::operator=` -- WRONG: CArray has no own operator=, it is the inherited STLport vector::operator= |
| 0x00F3A710 | 00/f3/CRangeCompressor_TestRedundantBitStrip_00f3a710.cpp | TestRedundantBitStrip | `?TestRedundantBitStrip@CRangeCompressor@@AAEXABV?$CArray@K@@AAV2@AAUCScriptEntry@1@J@Z` | ok |
| 0x00F3A5D0 | 00/f3/CRangeCompressor_TestRangeBias_00f3a5d0.cpp | TestRangeBias | `?TestRangeBias@...AAEXABV?$CArray@K@@AAV2@AAUCScriptEntry@1@J@Z` | ok |
| 0x00F3A270 | 00/f3/CRangeCompressor_CalcMemoryNeededBitCompressed_00f3a270.cpp | CalcMemoryNeededBitCompressed | `?CalcMemoryNeededBitCompressed@CRangeCompressor@@AAEJABV?$CArray@K@@@Z` | ok |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp (toolchain_library thunk) | free | msvcrt _free | ok (reached via allocator::deallocate `if(p) free(p)` in stlport/fable_containers.h:351) |
| inlined | none in TLC (no out-of-line copy in manifest) | GetBlockSizeFromFlags / CalcHeaderSizeNeeded / CalcBitsNeeded | Ego_r 0x00BDC090 `?GetBlockSizeFromFlags@CRangeCompressor@@AAEJK@Z`, 0x00BDC440 `?CalcHeaderSizeNeeded@CRangeCompressor@@AAEJABUCScriptEntry@1@@Z`, 0x00BDC480 `?CalcBitsNeeded@CRangeCompressor@@AAEJABV?$CArray@K@@@Z` | spellings ok; bodies must be visible + inline |

## 4. Members
- CScriptEntry (24 B; struct_layouts_egor.tsv `CScriptEntry`): +0 ulong Flags, +4 long BitCount, +8 ulong RangeBias,
  +0xC long RangeShift, +0x10 ulong ShuffleMask, +0x14 ulong OrMask. Shared header rebuild/include/engine/CRangeCompressor.h
  names all six exactly, ctor `CScriptEntry() : Flags(0) {}` (FableWin ??0CScriptEntry). No opt-in macros in that header.
  This function touches Flags (+0, r/w) and BitCount (+4, write on `entry`); whole-struct copies.
- Draft's local names WRONG vs PDB: BitsNeeded->BitCount, Pad08->RangeBias, GapStart->RangeShift, KeptMask->ShuffleMask, SetMask->OrMask.
- CArray<unsigned long>: rebuild/include/engine/CArray.h = `class CArray : public std::vector<T>` (STLport 12 B: _M_start,
  _M_finish, _M_end_of_storage). Opt-ins there: FABLE_RETAIL_CARRAY (begin()-based operator[]), _AT, _SIZE_CTOR_*,
  _GETASCARRAY_START, _DEALLOCATEUNUSEDMEMORY, _PUSHBACK(_NOINLINE).
- CRangeCompressor has no data members (Ego_r). `this` unused except passing to callees.
- Draft models LOCALLY (headers exist -> must switch): `CArray<T>` (engine/CArray.h), `CRangeCompressor` + `CScriptEntry`
  (engine/CRangeCompressor.h). (The landed siblings are local models too -- they predate the header; do not copy that.)

## 5. Callee-body visibility (KEY)
- The parked residue is exactly the chunk-741 lesson recorded in fable_containers.h:457-462 (FABLE_RETAIL_STLP_VECTOR_C, row
  0x00B149D0 CPSCEmitterGeneric::ReadBinary): with STLport `_vector.c` operator= body VISIBLE, VC7.1 still calls it out of line
  (COMDAT 0x0045BC09, unaligned addr) but knows it captures nothing, so caller values stay in registers across the call
  (here: Flags in ebp, the entries[1] ctor zero-store dropped). Declaration-only (the draft) reloads -> 844v857.
  => `#define FABLE_RETAIL_STLP_VECTOR_C` before including engine/CArray.h. Landed users: 00/b1/NParticleEngineCPSCEmitterGeneric_ReadBinary_00b149d0.cpp
  (also defines FABLE_RETAIL_CARRAY), 00/b1/..._DatabaseSerialise_00b13530.cpp, 00/ad/CVertexGroupBlockList_RemoveAllSubsets_00ad9770.cpp.
- Same TU (Ego_r order 0xBDC090..0xBDD210 == TLC 0xF39E50..0xF3AFC0): GetBlockSizeFromFlags, CalcHeaderSizeNeeded,
  CalcBitsNeeded are defined earlier in the TU and fully INLINED in TLC (no out-of-line copy exists). Define them in the row as
  `inline` out-of-class definitions of the header's declarations (bodies: the draft's are right; GetBlockSizeFromFlags matches landed Decompress 0x00F39ED0).
- CalcMemoryNeededBitCompressed / TestRangeBias / TestRedundantBitStrip are earlier in the same TU but called out of line:
  keep declaration-only (header).
- vector dtor (`_Destroy` + `_Vector_base` deallocate `if(p) free(p)`) and default ctor inline from the header - matches retail.

## 6. Siblings (same class / TU, all `// Flags:` none, no pragmas, no opt-ins, local models)
- 0x00F3A270 CalcMemoryNeededBitCompressed: same or-loop + bit-scan shape as the inlined CalcBitsNeeded (mask=values[0], `for i<Size()` unsigned, count-down bit loop).
- 0x00F3A5D0 TestRangeBias, 0x00F3A710 TestRedundantBitStrip, 0x00F39ED0 Decompress, 0x00F3AFC0 Compress: no pragmas; plain /O2 /Oy.
- Opt-in idiom source: 00/b1/NParticleEngineCPSCEmitterGeneric_ReadBinary_00b149d0.cpp (`#define FABLE_RETAIL_CARRAY`, `#define FABLE_RETAIL_STLP_VECTOR_C`, then `#include "engine/CArray.h"`).

## 7. Old-draft model errors
1. operator= declaration-only (local CArray) -> the whole 844v857 residue (Flags reload, entries[1] zero-store, this kept in ebp). Fix: header + FABLE_RETAIL_STLP_VECTOR_C.
2. Local CArray/CRangeCompressor/CScriptEntry instead of engine/CArray.h + engine/CRangeCompressor.h; wrong member names (see 4).
3. Draft's statement order otherwise matches retail (entries[5] then if/else, then arrays[5], arrays[0]=values, entries[1]=entries[0], ...); keep it.
   The inline helpers are in-class in the draft; with the header they must be `inline` definitions outside the class.
4. Loop/compare forms match (signed `size < best`, unsigned count in CalcBitsNeeded); the bit-scan may be written `return bit+1;`/`return 0;` (CalcBitsNeeded returns). Try CalcMemoryNeeded's `break` form only if needed.

## 0x009C9280

# DOSSIER 0x009C9280 NGameText::CDataBank::GetTextEntryFromGroup (861 bytes)

## 1. Retail shape (xdis 009c9280 0x36d)
- __thiscall, `ret 0x14` (5 args), NO frame pointer (sub esp,0x20; push ebx,ebp,esi,edi), NO SEH/EH state (no /EHsc frame).
- Frame 0x20 of locals (draft got 0x18). Slot map (esp after 4 pushes):
  +0x10 temp-destroy flag word (bit0 = the `speaker + "_1_5"` CCharString temp was built: `or eax,1` / `and eax,-2`; conditional temp of `a==b || a==b+"_1_5"`),
  +0x14 loop index i, +0x18 GetPGroupEntry result temp (CIVCountedPointer), +0x1c CCharString temp, +0x20 `id` local (address taken -> push_back(&id)),
  +0x24/+0x28/+0x2c local vector<long> start/finish/eos.
  Dead arg slots reused: groupID slot (+0x34) holds the GROUP counted pointer local; `found` slot (+0x3c) holds the fallback-loop try counter AND then the GetPTextEntry temp.
- Order: `*found=false` first (stored before any test). `if (groupID==0)` (cmp/jbe, unsigned) -> return 0 (no free). GetEntryDataType(groupID)!=1 -> return 0.
  Then vector={0,0,0} and group=0 initialised, THEN 2nd GetEntryDataType(groupID)==1 && Entries[groupID]!=0 (inline GetGroupInfo, PDB ?GetGroupInfo@CDataBank@NGameText@@QBEPBVCDataBankGroupEntry@2@K@Z) -> else return 0 (no free: start known 0).
- group = GetPGroupEntry(groupID, allowPending): temp at +0x18; `mov eax,[eax]`; if obj {++ref; group=obj} (no old-release: known null); then temp release `mov edx,[eax+4]; mov ecx,eax; add eax,4; dec edx; cmp edx,edi; mov [eax],edx; jne; call [vt+4]` + `mov [temp],0`.
- group null -> `*found = true; return 0` (via reloading found from its arg slot; no free).
- Loop `for (unsigned i=0; i < Entries.size(); ++i)` over group->Entries (+0x1c CArray<long>), unsigned compare, size re-read from the group local each iteration (`[esp+0x34]`).
  Body: id = entries[i]; info = (GetEntryDataType(id)==0 ? Entries[id] : 0) INLINED (GetTextInfo body, while the later uses CALL 0x9c7d00!); narrator = Narrators[info->NarratorIndex] (+4; PDB inline accessor ?GetNarratorIndex@CDataBankTextEntry@NGameText@@QBEJXZ).
  Narrator table base (edi = this->+0x204 first) and index*4 (ebp) held across BOTH compares; speaker.PStringData loaded once. Inline CCharString== (ptr equal -> true; both non-null + equal length -> Compare()==0 as neg/sbb/inc).
  bl = match; destroy temp if flag; if (bl) push_back(id) (ecx=&vec, push &id).
- After loop: group release `dec [ebx+4]; jne; call [vt+4]` — no null test, no zero store (dying, known non-null).
- start==finish -> jump to common `if (start) free(start); return 0`.
- count=(finish-start)>>2 (sar, signed), pick=0 (ebx), `*found = true`, id=start[0]; `if (count > 1)` (signed jle) do { GFRandom(count,&Seed) [inline: seed*0x24a1+0x24df stored, GFROR13 call, stored, `test ebp,ebp; je; div` UNSIGNED]; id=start[pick] } while (lastIndex && id == *lastIndex).
- AsyncUpdatePending() (0x9d68b0); GetTextInfo(id) CALLED -> `mov eax,[eax]` PText; IsLoaded INLINED: PText!=0 && (PFileData(+0x38)==0 || IsFinished()) -> success; else if (allowPending) success; else BeginLoadingText(id).
- Fallback: idx=pick (edi), tries=0 (found slot), `if (count>0)` loop { other=start[idx]; AsyncUpdatePending(); GetTextInfo(other)->IsLoaded() CALLED out of line (0x9c7be0); if loaded -> success(other); idx=(idx+1)%count (cdq/idiv SIGNED); } while (++tries < count) (jl signed). Then free + return 0.
- Success tail is shared by both success sites (tail-merged at 0x9c9581): `if (lastIndex) *lastIndex=id; esi = GetPTextEntry(id,true).Object (hidden temp in found slot, ret ptr = [eax]);` release temp (`mov edi,[eax+4]; mov ecx,eax; add eax,4; dec edi; mov [eax],edi; mov edx,edi; test edx,edx; jne; call [vt+4]; mov [slot],0`), then `free(start)` with NO null test (known non-empty), return esi.

## 2. Name / signature (two sources)
- FableWin.pdb (ghidra_out/fablewin_pdb_names.tsv 02f701b0) + Ego_r.pdb (egor_pdb_names.tsv 00b17cd0) + bsim_port_audit_final.tsv 9c9280 + labels_namepass_2026-10-26.tsv:
  `?GetTextEntryFromGroup@CDataBank@NGameText@@QBEPBVCTextEntry@2@KABVCCharString@@AA_NPAJ_N@Z`
  = `const NGameText::CTextEntry* NGameText::CDataBank::GetTextEntryFromGroup(unsigned long, const CCharString&, bool&, long*, bool) const`.
- Manifest label correct (class/leaf). Not virtual (QBE): no vtable slot.
- 5 landed callers all declare exactly this (bool&, const method, const CTextEntry*): CTCTalk_DoesSpeechExist_006e2910.cpp:15, CTCSoundPlayer_PlayAttachedDialogue_0066ba80.cpp:139, CTCOracleMinigame_GetTextWithNarrator_007e5ff0.cpp:37, CTCTavernGame_GetTextWithNarrator_005eb3a0.cpp:35, 01229de0.
- DRAFT WRONG: non-const method, `bool* found`, returns non-const CTextEntry*. Because the method is const, Seed (+0x210) must be `mutable` or written via const_cast; GetPGroupEntry/GetPTextEntry return CIVCountedPointer<const T>.
- PDB parameter names unknown; group id = PDB-free; use evidence-based names.

## 3. Callees
| addr | landed row | landed name | PDB name | draft disagreement |
|---|---|---|---|---|
| 0x009CC210 | 00/9c/CBankFile_GetEntryDataType_009cc210.cpp | CBankFile::GetEntryDataType | ?GetEntryDataType@CBankFile@@QBEKK@Z | ok |
| 0x009C8890 | 00/9c/NGameTextCDataBank_GetPGroupEntry_009c8890.cpp | GetPGroupEntry | ?GetPGroupEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCGroupEntry@NGameText@@@@K_N@Z | draft returns CIVCountedPointer<CGroupEntry> (missing const T) |
| 0x0099F600 | (none landed) | - | ??H@YI?AVCCharString@@ABV0@PBD@Z (operator+(const CCharString&,const char*) __fastcall, hidden result in ecx, string in edx, literal pushed) | ok (literal 0x012783C8, draft says "_1_5", not re-verified) |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ??1CCharString@@QAE@XZ | ok |
| 0x00411570 | 00/41/CBasicStringchar_Compare_00411570.cpp | CBasicString<char>::Compare | ?Compare@?$CBasicString@D@@KIJPBD0@Z (protected static, __fastcall, long) | draft: free `Compare` (spelling wrong) |
| 0x005178A0 | 00/51/global_PushBackdword005178a0_005178a0.cpp | folded vector<4-byte>::push_back COMDAT | STLport vector<long>::push_back (folded body, 10 instances) | name generic; called out of line |
| 0x00497890 | 00/49/global_GFROR13_00497890.cpp | GFROR13 | ?GFROR13@@YIJK@Z (returns long) | draft returns unsigned long; use engine/GFRandom.h GFRandom(range,&seed) |
| 0x009D68B0 | 00/9d/unattributed_CallTwo009d6730_009d68b0.cpp (row misnamed) | CallTwo_009d6730 | ?AsyncUpdatePending@CBankFileAsync@@QAEXXZ (non-const; spelled so by landed GetPTextEntry 0x9c9070 + reloc_bindings) | draft `FlushAsyncUpdates` WRONG (not in either PDB) -> NAME_FIXES candidate for 0x9d68b0 row too |
| 0x009C7D00 | 00/9c/NGameTextCDataBank_GetTextInfo_009c7d00.cpp | GetTextInfo | ?GetTextInfo@CDataBank@NGameText@@QBEPBVCDataBankTextEntry@2@K@Z | draft returns non-const ptr |
| 0x009C7BE0 | 00/9c/CDataBankTextEntry_IsLoaded_009c7be0.cpp | CDataBankTextEntry::IsLoaded (global ns in row) | ?IsLoaded@CDataBankTextEntry@NGameText@@QBE_NXZ | namespace NGameText missing in landed row; draft ok-ish |
| 0x009D54E0 | 00/9d/CBankFileAsyncData_IsFinished_009d54e0.cpp | IsFinished | ?IsFinished@CBankFileAsyncData@@QBE_NXZ | ok |
| 0x009C8630 | 00/9c/CDataBank_BeginLoadingText_009c8630.cpp | BeginLoadingText (declared NON-const, QAE) | ?BeginLoadingText@CDataBank@NGameText@@QBEXK@Z (const) | draft const = PDB; landed row's non-const is wrong |
| 0x009C9070 | 00/9c/NGameTextCDataBank_GetPTextEntry_009c9070.cpp | GetPTextEntry | ?GetPTextEntry@...QBE?AV?$CIVCountedPointer@$$CBVCTextEntry@NGameText@@@@K_N@Z | draft non-const T |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free | _free | ok |
(Side note: landed GetTextBySymbol 0x009C95E0 header comment calls 0x009C9070 "GetPGroupEntry" — wrong, it is GetPTextEntry.)

## 4. Members
- CDataBank (shared header rebuild/include/engine/NGameText_CDataBank.h: CDataBank : CBankFile, declares only methods, NO members past CBankFile; opt-ins FABLE_RETAIL_NGAMETEXT_GROUPENTRY (CGroupEntry + GetPGroupEntry), FABLE_RETAIL_CBANKFILE_ENTRYQUERY auto-defined (GetEntryDataType/EntryExists); needs FABLE_RETAIL_DEF_CLASS_BASE).
  PDB (struct_layouts_egor.tsv line 7537, CDataBank 680 bytes): Entries CArray<CDataBankEntry*> @PDB 596, TextEntryBuffer 612, GroupEntryBuffer 628, Narrators CArray<CCharString> 644, NarratorMap 660, `ulong Seed` 676.
  Retail offsets (landed rows): Entries +0x1e0 (GetTextInfo 9c7d00, GetPTextEntry 9c9070), Narrators +0x204 (ConvertNarratorNameToIndex 9c7ef0), Seed +0x210 (this row's LCG; PDB name Seed). Header must be extended (or local overlay as siblings do — every sibling models CDataBank locally with padding).
- CGroupEntry +0x1c `CArray<long> Entries` (header name, under FABLE_RETAIL_NGAMETEXT_GROUPENTRY; PDB accessor ?GetTextEntries@CGroupEntry@NGameText@@QBEABV?$CArray@J@@XZ inline). Refcount +4 (CIVCountedPointeeBase IVRefCount), Delete = vtable slot 1.
- CDataBankTextEntry (PDB 8 bytes): PText CIVCountedPointer<CTextEntry> +0, NarratorIndex +4 (draft calls it SpeakerIndex — WRONG; PDB NarratorIndex + GetNarratorIndex accessor).
- CTextEntry +0x38 PFileData = CCountedPointer<CBankFileAsyncData> (.Data first word), per landed 9c9070.
- CCharString: fable_string.h (shared); CIVCountedPointer: engine/CIVCountedPointer.h (opt-ins FABLE_RETAIL_CIVCOUNTEDPOINTER (copy ctor/Release/Reset), _ASSIGN, _DTOR_DIRECT, _RELEASE_MEMBER (count loaded before ecx copy: matches retail's temp releases), _RELEASE_INNER); CIVCountedPointeeBase.h opt-ins FABLE_RETAIL_DEF_CLASS_BASE, _CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES.
- CArray: engine/CArray.h (FABLE_RETAIL_CARRAY, _PUSHBACK, _PUSHBACK_NOINLINE); STLport vector push_back: fable_containers.h FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE / _OUTOFLINE, FABLE_RETAIL_STLP_VECTOR_INDEX_START.
- GFRandom: engine/GFRandom.h inline GFRandom(range, unsigned long* seed) (exact retail LCG body).
- Draft models LOCALLY (shared header exists): CCharString/CCharStringData (fable_string.h), CIVCountedPointer (engine/CIVCountedPointer.h), CTextEntry/CGroupEntry/CDataBank (engine/NGameText_CDataBank.h), CBankFileAsyncData (engine/CBankFileAsyncData.h), CLongVector (engine/CArray.h), CRefCounted/CResourceVirtual (engine/CIVCountedPointeeBase.h), GFROR13 (engine/GFRandom.h).

## 5. Visibility
- Same-TU neighbours (CDataBank.cpp, defined earlier): GetTextInfo 0x9c7d00 (inlined in the loop, called later — inline budget), IsLoaded 0x9c7be0 (inlined first site, called in fallback loop), BeginLoadingText 0x9c8630, GetPGroupEntry 0x9c8890, GetPTextEntry 0x9c9070 (called), ConvertNarratorNameToIndex 0x9c7ef0. GetGroupInfo and GetNarratorIndex have NO retail address -> pure inline.
- push_back 0x5178a0 called out of line (folded COMDAT) -> FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE or FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE (body visible, noinline: c2 knows &id not captured).
- vector dtor inline (`if (start) free(start)`), free direct.

## 6. Siblings (same TU, all plain, no pragmas/// Flags lines)
- NGameTextCDataBank_GetPTextEntry_009c9070.cpp (local CResource/CIVCountedPointer, const_cast on this for AsyncUpdatePending/BeginLoadingText).
- NGameTextCDataBank_GetTextInfo_009c7d00.cpp (engine/CArray.h, local CDataBank overlay with Entries at +0x1e0).
- CDataBank_ConvertNarratorNameToIndex_009c7ef0.cpp (FORCEINLINE CCharString operator== with Compare; Narrators +0x204).
- NGameTextCDataBank_GetTextBySymbol_009c95e0.cpp (counted pointer homed in dead arg slot; release written by hand).
- NGameTextCDataBank_GetPGroupEntry_009c8890.cpp (fable_game_text_bank.h + engine headers with #define renames).

## 7. Draft model errors vs disassembly
1. Signature: must be `const`, `bool& found`, return `const CTextEntry*`; Seed mutable.
2. Frame 0x18 v 0x20: retail has a distinct named `id` local (+0x20) written before the compares and a separate flags word; group local lives in the groupID arg slot and the text temp / try counter in the found slot — draft's scoping block around group plus CLongVector ctor order may shift these.
3. IsGroup: retail inline `GetGroupInfo(id)` (type==1 ? Entries[id] : 0) != 0 — draft has equivalent; keep after group/vector init.
4. Group release after loop must be a raw `dec/jne/Delete` with no null test and no zero store (draft's ReleaseTemporary dtor form re-tests null).
5. Narrator compare: retail keeps `&Narrators.first[idx]` (edi base + ebp idx*4) across both compares and reloads `[base+idx*4]` for the second compare — write `Narrators[info->GetNarratorIndex()]` as a const ref used twice; speaker data loaded once.
6. Random: unsigned `div` with zero test (GFRandom), pick unsigned; fallback index signed idiv; count compares signed (`jle`/`jl`).
7. Two success sites tail-merge; free on success path has no null test.
8. SpeakerIndex -> NarratorIndex; Speakers -> Narrators; RandomSeed -> Seed; FlushAsyncUpdates -> CBankFileAsync::AsyncUpdatePending; Compare -> CBasicString<char>::Compare.

## 0x00646F67 (not landed)

# DOSSIER 0x00646F67 (843 B) -- NPlayerGui::CDrawExperience::Draw

## 1. Retail shape (xdis 00646f67 0x35b)
- __thiscall, `ret 4` (one param), `push ebp; mov ebp,esp; sub esp,0x154` -> frame pointer: `#pragma optimize("y",off)` + `#pragma optimize("s",on)`
  (as siblings DrawIcon 0x645292, ScaleAlpha 0x643d5d, 2DSprite ctor 0x493ac2). Ends `leave; ret 4`.
- NO SEH prologue even though a CWideString local is destroyed at the end (call 0x99b510 at 0x6472a6) -> TU built without /EHsc
  (or no EH state needed). Save order: esi, edi, then ebx pushed AFTER the range guard (0x646f95).
- Early-out: `type = GetCurrentType(); if (type < 0 || type > 3) return;` (jl / jg to the shared epilogue 0x6472ac).
- switch(type) compiled as `sub eax,0 ; je ; dec ; je ; dec ; je ; dec ; jne` (default keeps ExperienceGraphic +0x3dc).
- Locals: alpha byte at [ebp-0x24]; pos X/Y kept at [ebp-0x38]/[-0x34]; colour bytes [-0x10..-0xd] with B pre-saved at [-0xc];
  three sprites at -0xac, -0x100, -0x154 (0x54 each); CWideString at -0x28; GFACoordToVCoord out at -0x48.
- Constants: 0x1230014 = 255.0f (Morality row notes), 0x125a2ac = 0.85f scale (Draw2DText), 0x122dcb4 = 2^32 fixup
  (`fild ; jge ; fadd` = unsigned long -> float conversion of GetGraphicFrameHeight's result).

## 2. Name / signature (two sources agree; manifest label RIGHT)
- Ego_r.pdb (egor_pdb_names.tsv 0x0068C109) and FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv 0x01842E93):
  `?Draw@CDrawExperience@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z`
- => `void NPlayerGui::CDrawExperience::Draw(const CEnginePrimitivePrepareInfo& info)` -- public, NON-virtual (QAE), non-const.
  Draft wrongly took `CEnginePrimitivePrepareInfo*` (same bytes, wrong mangling).
- Class: CDrawExperience : NPlayerGui::CDrawBase (ctor ??0CDrawExperience@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase...).
- Caller: one, 0x0043968d (xref.py) -- CPlayerGui draw path (via PDrawExperience +0x188).

## 3. Callees (landed file / landed name / PDB name)
| addr | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x646D90 | 00/64/CMultiStatBar_GetCurrentChange_00646d90.cpp | CMultiStatBar::GetCurrentChange  **WRONG** | ?GetCurrentType@CMultiStatBar@NPlayerGui@@QBE?AW4EStatUpdateType@@XZ (returns 6 default) | GetCurrentType (right, but returns long, non-const) |
| 0x646DA7 | 00/64/CMultiStatBar_GetCurrentChange_00646da7.cpp | free fn ?CMultiStatBar_GetCurrentChange@@YI... | ?GetCurrentChange@CMultiStatBar@NPlayerGui@@QBEJXZ | GetCurrentChange (right; must be const) |
| 0x438570 | 00/43/CPlayerGui_PeekGuiDef_00438570.cpp | CPlayerGui::PeekGuiDef | ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ (static fastcall, returns const&) | free `PeekGuiDef()` returning ptr: WRONG |
| 0xBFEA70 | 00/bf/global_ftol2_00bfea70.cpp | _ftol2 | CRT | implicit cast |
| 0x643D5D | 00/64/NPlayerGuiCDrawBase_ScaleAlpha_00643d5d.cpp | CDrawBase::ScaleAlpha | ?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z | `GetFadedAlpha`: WRONG name, non-const |
| 0x493AC2 | 00/49/CEnginePrimitive2DSprite_..._00493ac2.cpp | sprite ctor (C2DVector pos) | ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z | draft passes C2DCoordF: WRONG type (it is const C2DVector&) and long layer/blend/interp (are enums) |
| 0x41BEB0 | 00/41/CEnginePrimitive2DSprite_..._0041beb0.cpp | sprite ctor (C2DBoxF) | ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DBoxF@@W4EEngine2DLayer@@KKW4...@@W4...@@MMEEEEKABVC2DVector@@5@Z | ok shape, enums |
| [info+0x34]->vslot 23 (+0x5c) | CIEngine.h | CIEngine::SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) | (pure; CEngine 0x00B23BC0) | draft `Renderer->AddPrimitive`: WRONG (info.Engine is a CIEngine&) |
| 0x646DA7 then 0x405F00 | 00/40/global_GFIntToWideString_00405f00.cpp | GFIntToWideString | ?GFIntToWideString@@YI?AVCWideString@@J@Z | `IntToWideString`: WRONG |
| 0x9E21D0 | 00/9e/global_GFACoordToVCoord_009e21d0.cpp | GFACoordToVCoord | ?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z (const& in, & out) | pointers: WRONG mangling |
| 0x438010 | 00/43/CPlayerGui_Draw2DText_00438010.cpp | CPlayerGui::Draw2DText | ?Draw2DText@CPlayerGui@@QAEXABVC2DVector@@ABVCWideString@@ABVCFontBank@@ABVCRGBColour@@MW4EEngine2DLayer@@_N@Z | font as ptr: WRONG (const CFontBank&); called on `CPlayerGui::Get()` ([0x13B8790] = CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass), NOT on this->PlayerGui; draft's `g_pGameUI` global is a made-up name |
| [Bank]->vslot 22 (+0x58) | CGraphicDataBank.h | GetGraphicFrameHeight(unsigned long graphic, unsigned long frame) const -> unsigned long | ?GetGraphicFrameHeight@CGraphicDataBank@@UBEKKK@Z | `GetGraphicHeight` returning long: WRONG name+type (the 2^32 fixup proves unsigned) |
| 0x48DA62 | 00/48/CFontBank_GetStringWidth_0048da62.cpp | CFontBank::GetStringWidth | ?GetStringWidth@CFontBank@@QBEMABVCWideString@@@Z (const) | non-const in draft |
| 0x99B510 | 00/99/CWideString_Destructor_0099b510.cpp | CWideString::~CWideString | ??1CWideString@@QAE@XZ | ok |

NAME_FIXES candidates: 0x00646D90 landed `CMultiStatBar::GetCurrentChange` -> `NPlayerGui::CMultiStatBar::GetCurrentType` (QBE?AW4EStatUpdateType@@XZ);
0x00646DA7 landed as free `CMultiStatBar_GetCurrentChange` -> member `NPlayerGui::CMultiStatBar::GetCurrentChange() const` (QBEJXZ). Both PDBs.

## 4. Members
- this (CDrawBase, rebuild/include/engine/NPlayerGui_CDrawBase.h; NO CDrawExperience.h exists): PlayerGui +0x04 (CPlayerGui*), Component +0x10
  (CMainGameComponent*; draft calls it `Resources`/CGuiResources -- WRONG), Bank +0x14 (CGraphicDataBank*), Drawing +0x18 (bool),
  PrimitiveHandles +0x1c (vector<CEnginePrimitiveHandle>, 8-byte elements; handles [0],[1],[2] = Start, Start+8, Start+0x10).
  Opt-ins: FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING (inline SetAsDrawing(true) -> `mov byte [edi+18h],1`),
  FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES (ScaleAlpha decl + typed handle vector).
- [Component+0x1614c] = font (CFontBank*). Draft models a local CGuiResources; find the real CMainGameComponent member (struct_layouts_egor.tsv
  CMainGameComponent) -- likely a CCountedPointer<CFontBank>/font pointer; check engine/CMainGameComponent.h.
- CPlayerGui (engine/CPlayerGui.h, FABLE_RETAIL_CPLAYERGUI_MEMBERS): Alpha +0x2a0 (float; draft "GlobalAlpha"), PStatUpdates +0x2f4
  (CCountedPointer<NPlayerGui::CMultiStatBar>; reads its object word = inline operator->; draft "ExperienceBar"). Draw2DText needs
  FABLE_RETAIL_CPLAYERGUI_DRAWTEXT; PeekGuiDef needs FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF.
- CMultiStatBar : CStatBase (engine/NPlayerGui_CStatBase.h, no CMultiStatBar.h): GuiMode +0x28, FadeInTimer +0x2c (float).
  `fld [bar+2c] ; fmul 255.0 ; ftol` is the inline PDB `?GetFadeAlpha@CMultiStatBar@NPlayerGui@@QBEEXZ` (unsigned char, const)
  = (unsigned char)(FadeInTimer * 255.0f). Draft names it `Alpha` and inlines the expression by hand.
- CPlayerGuiDef (engine/CPlayerGuiDef.h, flattened _X/_Y floats; PDB names): +0x3d4 ExperiencePos, +0x3dc ExperienceGraphic,
  +0x3e0 ExperienceGeneralGraphic (type 3), +0x3e4 ExperienceSkillGraphic (type 1), +0x3e8 ExperienceStrengthGraphic (type 0),
  +0x3ec ExperienceWillGraphic (type 2), +0x3f0 ExperienceTextOffset, +0x3f8 ExperienceTextColour (CRGBColour, as uchar[4]),
  +0x3fc ExperienceMiddleOffset, +0x404 ExperienceMiddleMinWidth (float), +0x408 ExperienceMiddleGraphic, +0x40c ExperienceEndGraphic.
  Draft names Graphic0..3 / BackOffset / BackPadding / BackGraphic / BackEndGraphic are all WRONG. No opt-in gives C2DVector-typed
  Experience members (header is flattened; siblings use engine/_quarantine/CPlayerGuiDef.h or build C2DVector from X/Y).
- Locally modelled in the draft though shared headers exist: C2DVector, C2DBoxF, CRGBColour, CFontBank, CEnginePrimitive2DSprite,
  CEnginePrimitivePrepareInfo, CGraphicDataBank, CPlayerGui (as CGameUI), CPlayerGuiDef, CDrawBase (as CDrawExperience body).
  (CWideString / CMultiStatBar have no engine header.)

## 5. Visibility
- CRGBColour copy: retail reads all four bytes first (B[+0], A[+3], G[+1], R[+2]) then stores -> exactly the FABLE_RETAIL_CRGBCOLOUR copy ctor
  `CRGBColour(const CRGBColour& c){ Initialise(c.R,c.G,c.B,c.A); }` (CRGBColour.h). This is the "byte temp" the old notes could not
  reproduce -- try it first. Then `colour.A = (unsigned char)((float)colour.A * PlayerGui->Alpha)` (movzx A ; fild ; fmul [+2a0]).
- Inline in retail (no call): SetAsDrawing, CCountedPointer::operator-> on PStatUpdates, GetFadeAlpha, CTBaseSingleton::Get().
- Out of line: GetCurrentType/GetCurrentChange (0x646d90/0x646da7, same TU region, earlier addresses -- declare only), ScaleAlpha, sprite ctors,
  GFIntToWideString, GFACoordToVCoord, GetStringWidth, ~CWideString.
- C2DVector operator+ (pos + offset) inline: C2DVector.h FABLE_RETAIL_C2DVECTOR (+ _FORCEINLINE_CTOR if needed). C2DBoxF: FABLE_RETAIL_C2DBOXF(_CORNERS).
- Sprite ctors: FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS (enum params). Do NOT define _CRIME_CTOR (it makes the C2DVector ctor inline).
- Bank: FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE + FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE (slot 22 GetGraphicFrameHeight).
- Font: FABLE_RETAIL_CFONTBANK + FABLE_RETAIL_CFONTBANK_STRING_HELPERS (GetStringWidth const). PrepareInfo: FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
  (Engine is CIEngine&) + FABLE_RETAIL_CIENGINE (SetPersistentPrimitive slot 23).

## 6. Siblings
- 00/64/NPlayerGuiCCrimeStatBar_DrawStatBox_0064619f.cpp -- `// Flags: /O1 /Oy-`, full shared-header idiom list (C2DVECTOR, C2DBOXF(_CORNERS),
  CRGBCOLOUR, CPLAYERGUI(_MEMBERS), ENUM_CTORS, CIENGINE, PREPAREINFO...), `info.Engine.SetPersistentPrimitive(PrimitiveHandles[3],sprite,0xc0)`,
  `CPlayerGui::Get().ScaleAlpha(alpha)`. Best idiom template.
- 00/64/NPlayerGuiCDrawQuickAccessIcons_DrawIcon_00645292.cpp -- y off + s on, CDrawBase, calls ScaleAlpha.
- 00/64/NPlayerGuiCDrawMorality_Draw_006448a4.cpp -- y off + s on CDraw*::Draw (local models; idiom: Drawing=true; PeekGuiDef; 255*Alpha).
- 00/64/NPlayerGuiCMultiStatBar_DrawStatBarOnLeft_00646c44.cpp -- same class as callees, `#pragma optimize("s", on)`.
- 00/64/NPlayerGuiCStatBar_DrawStatBoxWithText_00646a46.cpp -- text+font+Draw2DText shape (named actual-screen text pos local).

## 7. Old draft model errors
- Prior grade DIFFER 824v843, frame 0x14c v 0x154 (8 bytes short): the missing slot is consistent with the colour copy (B saved at -0xc
  while the colour lives at -0x10) -> FABLE_RETAIL_CRGBCOLOUR Initialise copy ctor.
- Param pointer vs const&; ScaleAlpha misnamed; Draw2DText called on a fake global instead of CPlayerGui::Get(); GetGraphicFrameHeight typed long
  then cast `(float)(unsigned long)` -- use the real unsigned long return directly; font typed via fake CGuiResources.
- Graphic selection: draft starts `graphic = ExperienceGraphic` then switch -- matches retail (default keeps +0x3dc store at [-4]).
- Draft adds `GetFadedAlpha(alpha)` per sprite: correct (retail calls ScaleAlpha 3x with the same [ebp-0x24] byte).
- Text pos: retail computes (pos + TextOffset) into [-0x18] then GFACoordToVCoord(in=[-0x18], out=[-0x48]) -- a named out local.
- End sprite x = (backEnd.X - backPos.X) + backPos.X (`fld [-50] ; fsub [-8] ; fadd [-8]`) -- keep that odd expression.

## 0x00F3BA70 (not landed)

# DOSSIER 0x00F3BA70 CTriangleStripifier2::BuildCacheOptimizedVertexList (853 bytes)

## 1. Retail shape (xdis 00f3ba70 0x365)
- `__thiscall`, **`ret 18h`** = 6 stack args; `this` (ecx) UNUSED (ecx overwritten at 0xF3BA74 before any read).
  Callers set ecx: 0xF3E527 `mov ecx,esi`, 0xF3ECA9 `mov ecx,ebp` (third call 0xF3E319).
- No frame pointer (esp-relative, `sub esp,44h`, push ebx/esi/edi, later push ebp). No SEH frame (locals' dtors are
  just inline `if(p) free(p)`). Speed-optimised (/O2 style: `lea esp,[esp]` / `mov edi,edi` loop-alignment pads, strength
  reduction). No `optimize("s")` / `y off` signs.
- Entry E. Arg slots: a1 E+4, a2 E+8, a3 E+0Ch, a4 E+10h, a5 E+14h, a6 E+18h.
- Locals: CArray<bool> at E-0Ch (ctor this = E-0Ch), CArray<long> at E-18h. counter E-44h, s E-3Ch, vertex E-40h (dword),
  i E-28h, now E-2Ch, ahead E-38h, j E-30h, &strips[s+1] E-1Ch, p (next strip elem ptr) E-34h, a3.begin E-24h.
- Dead arg slots reused: a2 slot (E+8) holds the `false`/`-1` ctor value temporaries and later s*12; a6 slot (E+18h)
  holds the allocator temporary (empty `allocator<>` passed by reference as 3rd ctor arg).
- Teardown order: free(E-18h array) then free(E-0Ch array) (reverse construction), then `add esp,44h; ret 18h`.

## 2. Name / signature (two sources agree; manifest label is RIGHT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @00bd7470) and FableWin.pdb (ghidra_out/_fablewin_api.tsv +
  naming_stragglers/FableWin.exe.signatures.tsv @0331ec20) both:
  `?BuildCacheOptimizedVertexList@CTriangleStripifier2@@AAEXABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@V?$CArray@G@@@@02AAV2@@Z`
- Decoded: **private** non-const thiscall, returns void:
  `void BuildCacheOptimizedVertexList(const CArray<unsigned short>& a1, const CArray<CArray<long> >& a2,
   const CArray<CArray<unsigned short> >& a3, const CArray<unsigned short>& a4, const CArray<CArray<unsigned short> >& a5,
   CArray<unsigned short>& a6);`
  Note: a1..a5 are ALL `const&` (draft has them non-const); only a6 is non-const.
- No parameter names in any PDB dump on disk (no locals/params TSVs). Callers: 0xF3E319, 0xF3E529 (both in the
  0xF3E120.. region, next call is CalcSectionScore 0xF3D830), 0xF3ECAB (in 0xF3EA50 = public
  GenerateTriangleListAndStrip(CArray<G>&,CArray<G>&,CArray<G>&), Ego_r ?GenerateTriangleListAndStrip@...QAEXAAV?$CArray@G@@00@Z). None landed.
- Header rebuild/include/engine/CTriangleStripifier2.h does NOT declare this member (only FillInPolygons,
  GetMeshIslandSubsection under `private:`); the row must add/declare it (or the coordinator extends the header).

## 3. Callees
| addr | landed row | landed name | PDB/leaf | draft spelling |
|---|---|---|---|---|
| 0x00451583 | rebuild/src/compiled/00/45/stdvectorbool_vector_00451583.cpp | std::vector<bool>::vector(n,const bool&,const allocator&) | ??0?$vector@_NV?$allocator@_N@std@@@std@@QAE@IAB_NABV?$allocator@_N@1@@Z | draft: "CArray<bool>(n,value)" -- WRONG: retail calls the STLport vector fill ctor COMDAT (3 args incl. allocator), CArray(n,x) wrapper is inlined |
| 0x00512630 | rebuild/src/compiled/00/51/stdvectorunsignedlongstdallocatorunsignedlong_vector_00512630.cpp | std::vector<unsigned long>::vector(n,const&,alloc) (ICF: shared by all 4-byte POD) | ??0?$vector@... | draft "CArray<long>(n,-1)" -- same remark; local is CArray<long> (-1 value) |
| 0x00BFEAE6 | rebuild/src/compiled/00/bf/global_memmove_00bfeae6.cpp | memmove | MSVCR71 memmove | ok |
| 0x00BFEA0E | rebuild/src/compiled/00/bf/global_malloc_00bfea0e.cpp | malloc | malloc | ok |
| 0x00BFEA14 | rebuild/src/compiled/00/bf/global_free_00bfea14.cpp | free | free | ok |
No vtable calls. NOT called: 0x005135D0 (vector<unsigned short>::_M_insert_overflow) -- here the push_back growth is
fully expanded inline (malloc / memmove / free), unlike sibling StripUnusedVerts which calls 0x005135D0.

## 4. Members
- CTriangleStripifier2 has no data (header: "PDB base and derived records each have size four, no data" = vptr only);
  `this` never touched.
- Only CArray internals: {_M_start +0, _M_finish +4, _M_end_of_storage +8}, 12 bytes. Shared header
  rebuild/include/engine/CArray.h (`class CArray : public std::vector<T>`, from stlport/fable_containers.h).
  Draft models CArray LOCALLY (own struct with m_begin/m_end/m_capacity + hand push_back/clear) -- a shared header exists.
- CArray.h opt-ins: FABLE_RETAIL_CARRAY (operator[] via begin()), FABLE_RETAIL_CARRAY_AT, _SIZE_CTOR_OUTOFLINE,
  _SIZE_CTOR_FORCEINLINE, _GETASCARRAY_START, _DEALLOCATEUNUSEDMEMORY, _PUSHBACK_NOINLINE, _PUSHBACK. CArray(n,const T&)
  is always `: std::vector<T>(n,x)` (inline wrapper -> matches retail calling the vector ctor directly).
- size(): ptr-diff division -- `sar 1` for 2-byte elements, `imul 2AAAAAABh; sar; shr 1Fh; add` for 12-byte (CArray<CArray<>>).

## 5. Callee visibility / opt-ins (rebuild/include/stlport/fable_containers.h)
- Fill ctors called out of line: `FABLE_RETAIL_STLP_VECTOR_FILL_OUTOFLINE` (decl only, line ~402) or
  `FABLE_RETAIL_STLP_VECTOR_FILL_NOINLINE` (visible body, `__declspec(noinline)`, line ~396). NOINLINE likely better
  (c2 then knows the ctor does not capture the value temp; retail reuses arg slots E+8/E+18h for the temps).
- push_back growth inlined: needs `FABLE_RETAIL_STLP_VECTOR` (guards the `_M_insert_overflow(..., __true_type, ...)`
  definition at line ~699: len = size + max(size,fillLength); allocate; __copy_trivial(start,pos); fill_n(1); _M_clear;
  _M_set). Retail matches it exactly: `[esp+38h]=1` / `[esp+34h]=size` / `lea ecx; jb` = (max) over references;
  `test esi,esi; je` = placement-new null test in fast path; memmove only when pos!=start = `__copy_trivial`
  (`last==first?result:memmove(...)+n`, line 160). Do NOT use FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE/NOINLINE.
- clear(): STLport `erase(begin(),end())` inline -> `__copy_trivial(finish,finish,start)` gives retail's odd
  `mov eax,[ebx+4]; cmp eax,eax` at 0xF3BA97. Use a6.clear() (no FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE).
- Same-TU neighbours (same source file, retail order): StripUnusedVerts 0xF3B970 (immediately before), then this,
  CalcSectionPolyMask 0xF3CF50, GetMeshIslandSubsection 0xF3D320, CalcSectionScore 0xF3D830, FindBestStartVertex
  0xF3E120, GenerateTriangleListAndStrip 0xF3EA50/0xF3F3A0, ReorderTriangleListInPlace 0xF3F5A0. None of them is called here.

## 6. Sibling landed rows (idioms)
- rebuild/src/compiled/00/f3/CTriangleStripifier2_StripUnusedVerts_00f3b970.cpp -- local CArray model (no shared header),
  `#pragma auto_inline(off)` same-TU stand-in bodies for 0x005135D0 / 0x00F3B640, no Flags line; notes: a vertex held as
  `unsigned long` needs a separate homed `const unsigned short&` temp for push_back.
- rebuild/src/compiled/00/f3/CTriangleStripifier2_GetMeshIslandSubsection_00f3d320.cpp -- uses
  `#include "engine/CTriangleStripifier2.h"` (shared CArray), no pragmas/macros; `subsection.clear()`, CArray<bool>(n,false) locals.
- rebuild/src/compiled/00/f3/CTriangleStripifier2_CalcSectionScore_00f3d830.cpp -- local CArray model, auto_inline(off) stand-ins.
- rebuild/src/compiled/00/f3/CTriangleStripifier2_ReorderTriangleListInPlace_00f3f5a0.cpp, _GenerateTriangleListAndStrip_00f3f3a0.cpp
  -- local models + auto_inline(off) blocks. No `// Flags:` lines in any sibling (default lane flags).

## 7. Decoded control flow vs old draft (draft errors)
Retail, in source terms (a1 UNUSED; a2 only for size):
```
a6.clear();
CArray<bool> emitted(a2.size(), false);        // write-only
CArray<long> emittedAt(a2.size(), -1);
long counter = 0;
for (long s = 0; s < a5.size(); ++s) {                 // s SIGNED (`test edi,edi; jle` on s*12 for s>0)
  [now = counter+1 computed ONCE before the i loop, ++now each i]  (E-2Ch, 0xF3BB37 / 0xF3BD48)
  for (unsigned i = 0; i < a5[s].size(); ++i) {        // sizes re-read from a5 every pass
    vertex = a5[s][i];                                  // E-40h; push_back reads it as word -> likely `unsigned short vertex`
    ahead = now;
    if (s > 0 && s < a5.size() - 1) {                   // second test unsigned (jae)
      const CArray<G>& next = a5[s+1];
      distance = now - counter - 1;
      for (unsigned j = 0; j < next.size(); ++j) {
        w = next[j]; ++ahead; ++distance; bool needed = false;
        for (unsigned k = 0; k < a3[w].size(); ++k) {   // a3 (NOT a1) indexed by w; size re-read via [ecx]/[ecx+4]
          unsigned f = a3[w][k];
          if (a4[f] == s) {                              // a4 (NOT a1!) is the per-vertex strip index map
            long at = emittedAt[f];
            if (at >= 0) { if (ahead - at >= 16) goto next_strip; }   // signed jge
            else { if (f != vertex) continue; if (distance >= 16) goto next_strip; }
            needed = true;
          }
        }
        if (!needed) break;
      }
    }
    emitted[vertex] = true;
    a6.push_back(vertex);
    emittedAt[vertex] = ++counter;
    ++now;
  }
next_strip:;
}
```
Draft errors:
1. Argument roles: draft indexes `stripOfFace` = a1 for `a4[f]==s`; retail reads a4 ([esp+64h] after push ebp = E+10h at
   0xF3BBE2). a1 is never read. Draft names a4 "unused" -- swapped.
2. Constness: a1..a5 must be `const CArray<...>&` per mangled name.
3. `now` is a separate induction variable initialised once per strip (counter+1) and incremented per i, not
   `long now = counter + 1;` recomputed inside the i loop (retail 0xF3BB37 load counter only at strip start, 0xF3BD48 inc).
4. Strip index must be signed (`long`/`int`): retail tests `s>0` with `jle`; draft uses unsigned long.
5. Local CArray model + hand-written push_back/clear instead of the shared engine/CArray.h + STLport bodies (the
   ctor calls are std::vector fill ctors 0x451583/0x512630 with an allocator arg -- draft's 2-arg CArray ctor decls
   produce the wrong push count).
6. vertex type: draft `unsigned long vertex` would need a homed short temp for push_back; retail reads the word directly
   from the vertex slot (0xF3BCF0 `mov cx,[esp+14h]`), so `unsigned short vertex` (or equivalent) is indicated.
7. Pointer induction: j loop walks `next` with a pointer (E-34h, `add ebx,2`) and k loop with esi `add esi,2` -- natural
   strength reduction of `next[j]` / `a3[w][k]`, keep subscripts.


## 0x006B3FF0

# DOSSIER 0x006B3FF0 CEnvironment::Update (758 bytes)

## 1. Retail shape (xdis 006b3ff0 0x306)
- __thiscall, `ret 4` (one float arg `time` read at [esp+0x254] after 4 pushes). `sub esp,0x244; push ebx; push ebp; push edi; ... push esi` (esi pushed late, at 0x6B403B, popped at 0x6B42BC). NO frame pointer (ebp = this), NO SEH prologue even though a CBlendedEnvironmentTheme local with a dtor exists => TU built without /GX (the CEnvironment ctor 0x006B4900 has no SEH either). Speed-optimised (unrolled scaler copy, `lea esp,[esp]` loop-align pad at 0x6B403C): no optimize("s"), no /Oy-.
- Frame 0x244 (locals at esp+0x10..0x254 with esi pushed): [0x10] loop-saved edi / later `rain` float, [0x14] channel byte offset / later the CIntelligentPointer<CThing> temporary (8 bytes, 0x14..0x1c), position C3DVector 0x1c..0x28, CBlendedEnvironmentTheme `theme` 0x28..0x21c (500 bytes; ctor 0x008864A0 BEFORE the loop, dtor 0x006B5D80 at the end), **CCamera return temporary 0x21c..0x254 (0x38 bytes)** for the by-value virtual GetRenderCamera. This camera temp is the missing frame space in the old draft (0x210 vs 0x244).
- Loop: ebx = channel*12 (byte offset into PriorityStackChannels, cmp 0x48 = 6 channels), edi = &GameTickState.BlendedTheme[i].Scalers[1] (stride 0x1f4). Colours copy = `rep movsd` 0x4c dwords (19 CRGBFloatColour = memcpy-sized struct copy). Scalers: 30 floats unrolled x10 (eax index +10, ecx ptr +0x28). TextureSets: 3-iteration countdown (ebx=3) calling vector<long>::operator= 0x00454886 then vector<float>::operator= 0x0045EF2D out of line (the implicit CIDrawEnvironmentTextureSet::operator= inlined, its CArray<long>/CArray<float> members' vector operator= called).
- Rain: `mov al,[ebp+45h]; test; mov ecx,[ebp+1C4h]; mov [esp+10h],ecx; jne; mov [esp+10h],0` => `float rain = RainStrength(+0x1c4); if(!RainEnabled) rain = 0;` integer copy (no fld) — GetRainStrength (0x006B2BD0, fld [ecx+1C4h]) expanded or member read. Threshold const at 0x0129BA3C (0.0001f). bl=1 (bitfield mask for Dead) is reused for `PassedFirstUpdate = true` (`mov [ebp+44h],bl`).
- if (rain > thr && !IS_THING_ALIVE(PRainSplashParticle)) { position = CMainGameComponent::Get()->GetWorld().GetGameCamera()->GetRenderCamera(1.0f).PeekWorldPos(); PRainSplashParticle = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(DefinitionManager->[+0xd8]->[+0x100], position, false)); if (IS_THING_ALIVE(...)) { CTCDParticleEmitter* e=0; thing->GetTCDriver(&e) [inline: test [eax+28h] bit 0x800 -> [eax+5Ch]]; e->AttachToCamera(8, 0.0f); } } else if (rain <= thr (jp form) && IS_THING_ALIVE(...)) PRainSplashParticle->Kill(false);
- Then UpdateSunAndMoonPositions(); UpdateWind(); LightingFlash(); if(!PassedFirstUpdate){PrevGameTickState=GameTickState; PassedFirstUpdate=true;} theme dtor.
- Order at entry: PrevGameTickState = GameTickState (0x006B63C0 push &GameTickState, ecx=&PrevGameTickState); SpellEffectInterpolator_ptr->FrameUpdate(); GameTickState.Time(+0xc0c) = time; theme ctor.

## 2. Identity
- Ego_r.pdb `?Update@CEnvironment@@QAEXM@Z` (0x006EF9E0) and FableWin.pdb same (0x0244E500, 1614 b) — two sources. Manifest label CEnvironment::Update is CORRECT. Leaf: `?Update@CEnvironment@@QAEXM@Z` : public, void Update(float time), non-const.
- Callers: 0x004A5DF3 (CWorld::UpdateFrame, row 0x004A5A40, pushes the game time) and 0x006B4305 (CEnvironment::RenderUpdate 0x006B42F0: `if(!PassedFirstUpdate) Update(0)`). Declared already in engine/CEnvironment.h under FABLE_RETAIL_CENVIRONMENT.

## 3. Callees (landed file / landed name / PDB name; ! = draft spelling wrong)
- 0x006B63C0 CEnvironmentGameTickState_operator_006b63c0.cpp (top-level struct) / PDB `??4CEnvironmentGameTickState@CEnvironment@@QAEAAV01@ABV01@@Z` — NESTED class CEnvironment::CEnvironmentGameTickState (draft top-level; manifest says CEnvironmentThemeDef, wrong). No shared header; CEnvironment.h keeps GameTickState/PrevGameTickState opaque (unsigned char[0xc20]) — needs a typing opt-in.
- 0x006B8640 CEnvironmentSpellEffectInterpolator_FrameUpdate_006b8640.cpp / PDB `?FrameUpdate@CEnvironmentSpellEffectInterpolator@@QAEXXZ`. engine/CEnvironmentSpellEffectInterpolator.h does NOT declare FrameUpdate (needs adding/opt-in).
- 0x008864A0 NEnvironmentInternal::CBlendedEnvironmentTheme ctor (landed) / `??0CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ`. ! draft lacks namespace NEnvironmentInternal.
- 0x006B5D80 landed as `CBufferPairTriple_006b5d80::~CBufferPairTriple_006b5d80` (CBufferPairTriple006b5d80_..._006b5d80.cpp, made-up) — **NAME_FIX**: it is `??1CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ` (manifest ~CBlendedEnvironmentTheme; landed rows 0x0088A180 / 0x008886E0 / 0x00888410 bind it as the theme dtor; this function constructs/destructs the 500-byte theme at the same object).
- 0x00887B00 `?ProcessTransitions@CEnvironmentThemePriorityStack@NEnvironmentInternal@@QAEXXZ` (landed) ; 0x008889C0 `?CalculateTheme@CEnvironmentThemePriorityStack@NEnvironmentInternal@@QAEXAAVCBlendedEnvironmentTheme@2@MABVCEnvironmentThemeResourcePallette@2@@Z` (landed) — ! draft passes pointers; PDB takes (CBlendedEnvironmentTheme&, float, const CEnvironmentThemeResourcePallette&). Declared in engine/CEnvironmentThemePriorityStack.h.
- 0x00454886 `??4?$vector@JV?$allocator@J@std@@@std@@QAEAAV01@ABV01@@Z` (stdvector_operator_00454886.cpp) ; 0x0045EF2D `??4?$vector@MV?$allocator@M@std@@@std@@...` (vector<float>, not "CVectorTexture" as draft says!). Opt-in FABLE_RETAIL_STLP_VECTOR_ASSIGN (stlport/fable_containers.h:437, declaration-only operator=).
- 0x00A01B10 / 0x00A01C10 / 0x00A01B50 / 0x00A01B90 CBaseIntelligentPointer ctor/dtor/GetPItem/SetPItem (landed). Vtable 0x01238C6C = CIntelligentPointer<CThing>.
- 0x006E0880 `?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z` static __fastcall — ! draft calls it `CreateSpecialEffect`. Not landed; declared in engine/CTCDParticleEmitter.h under FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE.
- 0x006E0DB0 `?AttachToCamera@CTCDParticleEmitter@@QAEXJM@Z` (landed; header declares AttachToCamera(long,float)).
- 0x004C9B80 `?Kill@CThing@@QAEX_N@Z` (landed; CThing.h:248).
- 0x006B2CA0 — not landed; manifest label `CGameCamera_UpdateViewMatrix` WRONG, draft `UpdateViewMatrix` WRONG. It is `?UpdateSunAndMoonPositions@CEnvironment@@AAEXXZ` (Ego_r donor order UpdateSunAndMoonPositions 0x6EE710 < UpdateWind 0x6EEC00; body fcos/fsin of [esi+0xc0c] time; LightingFlash row comment already names it). Private.
- 0x006B3030 `?UpdateWind@CEnvironment@@AAEXXZ` (landed, private).
- 0x006B3B80 `?LightingFlash@CEnvironment@@AAEXXZ` (landed) — ! draft `UpdateThunder` wrong. Declared under FABLE_RETAIL_CENVIRONMENT_LIGHTINGFLASH (private).
- Virtual: [[[0x13B86A0]+0x24]+0x34] slot 1 (`call [edx+4]`, hidden return ptr, arg 1.0f) = CMainGameComponent::PMainGameComponent -> PWorld (scoped_ptr<CWorld> +0x24) -> CWorld::PCurrentGameCamera (+0x34, CAIGameCameraBase*) -> `virtual CCamera GetRenderCamera(float interpolation) const` slot 1 (engine/CAIGameCameraBase.h:28). Copies CCamera::WorldPosition (+0, 12 bytes; CCamera.h PeekWorldPos()). ! draft's `g_pEngine->Display->View->GetPosition` model is wrong (returns C3DVector, hence too-small frame).

## 4. Members (engine/CEnvironment.h, Ego_r names)
- +0x04 DefinitionManager (CGameDefinitionManager*): [+0xd8] -> [+0x100] = rain-splash effect id (draft names Effects/RainSplashEffect; verify in CGameDefinitionManager.h).
- +0x3c ForcedColours, +0x3d ForcedNonColourValues, +0x44 PassedFirstUpdate, +0x45 RainEnabled (bool).
- +0x48 PriorityStackChannels CArray<NEnvironmentInternal::CEnvironmentThemePriorityStack> — typed under FABLE_RETAIL_CENVIRONMENT_THEMESTACKS.
- +0x54 GameTickState / +0xc74 PrevGameTickState (CEnvironment::CEnvironmentGameTickState, retail 0xc20 bytes: BlendedTheme[6] 6x500, Time +0xbb8 -> abs 0xc0c). Opaque in header; FABLE_RETAIL_CENVIRONMENT_WEATHERSCALERS gives RainStrength at +0x1c4 (= BlendedTheme[0].Scalers[15]) but splits GameTickState into byte arrays — incompatible with a typed tick state; author will need a new opt-in typing the tick state (or use GetRainStrength under FABLE_RETAIL_CENVIRONMENT_RAINSTRENGTH with a same-TU body as LightingFlash does).
- +0x1954 ThemeSet (CEnvironmentThemeResourcePallette, typed under THEMESTACKS), +0x1964 SpellEffectInterpolator_ptr (scoped_ptr ptr; GetSpellEffectInterpolator() under FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR), +0x1968 PRainSplashParticle (opaque unsigned char[8] in header; real type CIntelligentPointer<CThing> — needs typing).
- CBlendedEnvironmentTheme (engine/CBlendedEnvironmentTheme.h, NEnvironmentInternal): TimeOfDay, Colours[19] (CRGBFloatColour), Scalers[30], TextureSets[3] (CIDrawEnvironmentTextureSet{CArray<long> Textures; CArray<float> Blends;}, 24 bytes). Do NOT define FABLE_RETAIL_CBLENDEDENVIRONMENTTHEME_ASSIGN (that makes whole-theme = call 0x006B5DF0; here members are copied piecewise).
- CThing +0x28 TC-interface bitmask (bit 0x800 = TCI_PARTICLE_EMITTER), +0x5c PTCDriver, +0x91 bit0 Dead (IsDead()). CThing.h: FABLE_RETAIL_CTHING_GETTCDRIVER gives template GetTCDriver<T>(T**) (plain inline; here it is expanded), IsDead().
- Locally-modelled classes in old draft that have shared headers: CEnvironment, CBlendedEnvironmentTheme, CIDrawEnvironmentTextureSet, CRGBFloatColour, CEnvironmentThemePriorityStack, CEnvironmentThemeResourcePallette, CEnvironmentSpellEffectInterpolator, CBaseIntelligentPointer/CIntelligentPointer, CThing, CTCDParticleEmitter, C3DVector, CArray (vector), CMainGameComponent/CWorld/CAIGameCameraBase/CCamera (draft's CEngine/CEngineDisplay/CCameraView are fictitious), CGameDefinitionManager.

## 5. Visibility / opt-ins
- CIntelligentPointer: FABLE_RETAIL_CINTELLIGENTPOINTER + _ASSIGN (inline GetPItem->SetPItem matches 0x6B420C..0x6B4218); the (T*) ctor is inline (base ctor, vptr store, SetPItem) — default branch, not _CTOR_OUTOFLINE.
- IS_THING_ALIVE: engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE (inline; do NOT use _NOINLINE here — all checks are expanded, GetPItem called twice per test).
- vector operator= out of line: FABLE_RETAIL_STLP_VECTOR_ASSIGN.
- Same-TU (CEnvironment.cpp) bodies retail could see: GetRainStrength 0x006B2BD0 (out-of-line copy exists, expanded here as in LightingFlash), UpdateSunAndMoonPositions/UpdateWind/LightingFlash/SetWeightedThemes defined earlier (called, not inlined — large). CEnvironmentGameTickState::operator= 0x006B63C0 and ~CBlendedEnvironmentTheme 0x006B5D80 are at neighbouring addresses (COMDAT/out-of-line), called.
- CTCDParticleEmitter::Create: FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE. CMainGameComponent::Get() (always) + FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD (GetWorld() inline reading +0x24); CWorld::GetGameCamera() inline (CWorld.h ~line 217, check its guarding macro); CCamera PeekWorldPos (CCamera.h:54).

## 6. Sibling rows
- rebuild/src/compiled/00/6b/CEnvironment_LightingFlash_006b3b80.cpp: no pragmas; macros CCHARSTRING, CARRAY, CTCBASE, CTHING, CTHING_GETTC_EXPANDED, CINTELLIGENTPOINTER, CENVIRONMENT, CENVIRONMENT_THEMESTACKS/_DEFAULTTHEME/_RAINSTRENGTH/_LIGHTINGFLASH/_WEATHERSCALERS, CMAINGAMECOMPONENT(+_PLAYERMANAGER...), IS_THING_ALIVE(+_NOINLINE); defines GetRainStrength body in-TU.
- CEnvironment_SetWeightedThemes_006b3e60.cpp: CENVIRONMENT_THEMESTACKS/_THEMEINDEX/_SETWEIGHTEDTHEMES, STLP_* out-of-line opt-ins, includes fable_string.h, stlport/fable_containers.h, engine/CEnvironment.h.
- CEnvironment_UpdateWind_006b3030.cpp: `// Flags:` empty; uses `#define CEnvironment CEnvironmentFields` wrapper over the header.
- CEnvironment_CEnvironment_006b4900.cpp: local tick-state model `#pragma pack(push,1)` struct CEnvironmentGameTickState; CINTELLIGENTPOINTER.
- 00/88/NEnvironmentInternalCEnvironmentThemePriorityStack_CalculateTheme_008889c0.cpp: uses engine/CEnvironmentThemePriorityStack.h.

## 7. Old draft model errors
- Missing CCamera by-value temporary (fake C3DVector GetPosition) -> frame 0x210 instead of 0x244 (the residue). Use GetRenderCamera(1.0f).PeekWorldPos() (or `.WorldPosition`) through CMainGameComponent/CWorld/CAIGameCameraBase.
- Wrong names: CreateSpecialEffect (=CTCDParticleEmitter::Create), UpdateViewMatrix (=UpdateSunAndMoonPositions), UpdateThunder (=LightingFlash), g_pEngine (=CMainGameComponent::PMainGameComponent), CVectorTexture (=vector<float>/CArray<float> Blends), Channels (=PriorityStackChannels), SpellEffectInterpolator (=SpellEffectInterpolator_ptr), CEnvironmentGameTickState is nested CEnvironment::.
- CalculateTheme args by reference per PDB; theme types in NEnvironmentInternal.
- RainEnabled zero store "out of line": retail has load rain into a local first, then conditional zero store fall-through (`jne skip; mov [esp+10h],0`) — try `float rain = GetRainStrength(); if (!RainEnabled) rain = 0.0f;` with GetRainStrength visible, vs the draft's member access; also the ternary.
- Emitter extraction should be the inline GetTCDriver<CTCDParticleEmitter>(&emitter) with emitter=0 pre-init (xor edi,edi before GetPItem).
- No /GX: keep EH off for this TU (no SEH frame despite dtor locals).

## 0x00727D00

# DOSSIER 0x00727D00 -- CTCRandomAppearanceMorph::OnAppearanceDraw (760 bytes)

## 1. Retail shape (xdis 00727d00 0x308)
- thiscall, `ret 8` (2 args), returns bool (`mov al,1` at 0x727FED, always true).
- NO frame pointer (`sub esp,0F8h; push ebx/ebp/esi/edi`), NO SEH prologue although a local std::set, a CPrimitive temp,
  a CEnginePrimitiveMorphedAnimatingMesh and a CIVCountedPointer temp all have dtors -> lane default (no /EH), like sibling
  CTCSpotLight::OnAppearanceDraw 0x00731EA0 (`// Flags: none`, same no-SEH prologue `sub esp,68h; push ebx..edi`).
- `this` = CAGraphicAppearanceComponent subobject at full+0x0C (vtable 0x01266520 slot 0). [this+4]=PMeshMorphSet(+0x10),
  [this+0x10]=PrimHandles(+0x1C), [this-8]=ParentThing(+0x04).
- Not size-optimised: normal speed idioms (`mov edi,edi` loop align at 0x727D8E).

## 2. Name / signature (two sources)
- Ego_r.pdb (egor_pdb_names.tsv / egor_signatures.tsv) + FableWin signatures:
  `?OnAppearanceDraw@CTCRandomAppearanceMorph@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z`
- rtti-vtables.tsv: CTCRandomAppearanceMorph vtable 01266520 slot 0 = 00727d00 (slot1 OnAppearanceClear 727bf0, slot2 GetMeshesUsed 727c20;
  matches NGraphicAppearance_CAGraphicAppearanceComponent.h order: OnAppearanceDraw, OnAppearanceClear, GetMeshesUsed).
- xref: only reference is that vtable (no direct callers).
- `virtual bool OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance&, const NGraphicAppearance::CGraphicAppearanceDrawInfo&)`.
- Manifest label "CTCRandomAppearanceMorph::CTCRandomAppearanceMorph::OnAppearanceDraw" has a doubled class; real is
  CTCRandomAppearanceMorph::OnAppearanceDraw. Class layout (Ego_r 48 bytes): CTCAppearanceMorphBase(CTCBase, 0xC) + CAGraphicAppearanceComponent@+0xC,
  PMeshMorphSet CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> +0x10, Seed +0x18, PrimHandles vector<NGraphicAppearance::CPrimitive*> +0x1C,
  PTCSkeletalMorph +0x2C. No shared header for CTCRandomAppearanceMorph / CTCAppearanceMorphBase / CGraphicAppearanceDrawInfo /
  CEnginePrimitiveMorphedAnimatingMesh (siblings model them locally).

## 3. Callees (landed file | landed name | PDB name) -- draft spelling flags
| addr | landed file | PDB / correct | draft spelling |
|---|---|---|---|
| 004BBFF0 | 00/4b/CMap_DrawGetSeaLevel_004bbff0.cpp (MISLABEL, fastcall free fn) | body `fld [ecx+4Ch]; fmul [ecx+34h]; ret` = Scale(+0x34) * MainGraphic.RenderSizeX(+0x44+8) -> Ego_r `?GetFullScale@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ` (descriptive match: name+body; row 0x0075A530 calls it GetScale, but GetScale is inline `fld [+34h]` per header FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SCALE) | GetAlpha -- WRONG |
| 004BC180 | 00/4b/CTCGraphicAppearance_GetFadeDistance_004bc180.cpp (fastcall free fn) | ?GetFadeDistance@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ (header decl under _FLARE) | ok |
| 00BFEA0E / 00BFEA14 | global_malloc / global_free | STLport node alloc of the set header (0x14 bytes) inline set ctor/dtor | "operator new"/free -- it is std::set ctor/dtor inline, not user code |
| 00642440 | 00/64/stdvector_Mfillinsert_00642440.cpp | vector<NGraphicAppearance::CPrimitive*>::_M_fill_insert(pos, n, const T&) | "Grow" |
| 00BFEAE6 | global_memmove | inlined STLport erase(first,last) copy (from resize) | -- |
| 0099A2D0 | CBaseClass_CBaseClass_0099a2d0.cpp | CBaseClass::CBaseClass (inside inline CPrimitive/CEnginePrimitiveHandle ctor, then vptr 0x0122F598) | CNodeParams ctor -- WRONG |
| 004C0050 | 00/4c/NGraphicAppearanceCTCGraphicAppearance_AddPersistentPrimitive_004c0050.cpp | CPrimitive* CTCGraphicAppearance::AddPersistentPrimitive(const CPrimitive&) | InsertAppearanceNode -- WRONG |
| 004C4160 | NGraphicAppearanceCPrimitive_CPrimitive_004c4160.cpp | ??1CPrimitive@NGraphicAppearance@@QAE@XZ | ~CNodeParams -- WRONG |
| 004BD040 | NGraphicAppearanceCTCGraphicAppearance_GetTintColour_004bd040.cpp | `const CRGBColour GetTintColour() const` (ret 4, hidden result) | GetLevel -- WRONG |
| 00709A50 | 00/70/CEnginePrimitiveMorphedAnimatingMesh_..._00709a50.cpp | ??0CEnginePrimitiveMorphedAnimatingMesh(unsigned __int64 id, unsigned long mesh, const CTextureMorphVector&, const CHiddenMaterialSet&, const C3DAnimationInterpolator&, const C3DPositionInterpolator&, const C3DOrientationInterpolator&, float scale, bool dynamic, unsigned char alpha, CRGBColour colour, float drawDistance, EEngineInterpolationSet, const CEnginePrimitiveThemeChannelDesc&) | CMorphedMesh, wrong arg list |
| 004BCF50 | NGraphicAppearanceCTCGraphicAppearance_GetPMeshEffect_004bcf50.cpp | CIVCountedPointer<CEngineMeshEffect> GetPMeshEffect() (ret 4, hidden result) | GetPrimitive -- WRONG |
| 004BD620 | ..._SetPrimitiveGraphics_004bd620.cpp | void SetPrimitiveGraphics(CPrimitive*, const CEnginePrimitive&, unsigned short) | ok-ish (draft passes CMorphedMesh*) |
| 00709B10 | ..._00709b10.cpp | ~CEnginePrimitiveMorphedAnimatingMesh | ok |
| 004BCFE0 | ..._RefreshPrimitiveAnimationAndPos_004bcfe0.cpp | void RefreshPrimitiveAnimationAndPos(CPrimitive*, const C3DAnimationInterpolator&, const C3DPositionInterpolator&, const C3DOrientationInterpolator&) | arg order WRONG in draft (orient,anim,pos) |
| 004ABA70 | 00/4a/stdRbtree_Merase_004aba70.cpp | _Rb_tree<CCharString,...>::_M_erase (set<CCharString> dtor's clear) | ok |
| vtbl[+4] of CEngineMeshEffect | -- | inline CIVCountedPointer release (dec [obj+4], call slot1 at 0) in operator= and temp dtor | -- |
NAME_FIXES candidate: 0x004BBFF0 landed as CMap_DrawGetSeaLevel (and called GetScale in 0x0075A530) -> CTCGraphicAppearance::GetFullScale.

## 4. Members read/written
- CTCGraphicAppearance (engine/NGraphicAppearance_CTCGraphicAppearance.h; layout only under FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT):
  +0x40 LightingChannel (CEnginePrimitiveThemeChannelDesc) -> `lea edx,[esi+40h]` = ctor's last arg `theme` (NOT Scale as draft says);
  +0x5A Alpha (unsigned char; MainGraphic +0x44..0x51, ActivePersistenceFlags 0x52, FrameUpdated 0x54, UpdatePrimitiveReasons 0x58, bits 0x59, Alpha 0x5A, ByPassColorFilter 0x5B confirmed by header note `[ebp+5Bh]`).
  Alpha is protected; PDB has `?GetAlpha@CTCGraphicAppearance@NGraphicAppearance@@QBEEXZ` (inline in retail: the byte read at 0x727E55) -- not in header; author must add an opt-in or local accessor.
  Methods: GetFadeDistance / AddPersistentPrimitive / SetPrimitiveGraphics via FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE; GetTintColour (needs _TINT fwd decl; no member decl seen -> add), GetPMeshEffect (_MESHEFFECT includes CIVCountedPointer, no decl -> add), GetFullScale (none -> add), RefreshPrimitiveAnimationAndPos (none -> add).
- CGraphicAppearanceDrawInfo (Ego_r 36 bytes, no header): Interpolate +0, Pos +4 (C3DPositionInterpolator*), Orientation +8, Anim +0xC,
  PTextureMorphs +0x10, PTextureDecals +0x14, HiddenMaterials +0x18, ReasonsForDraw +0x1C (ulong). Draft had Anim/Orient/Pos wrong.
- CThing (engine/CThing.h): UniqueID unsigned __int64 +0x18, `GetUniqueID() const` inline. CTCBase.h: ParentThing +4, inline GetParentThing() (FABLE_RETAIL_CTCBASE).
- CMeshMorphSet (Ego_r 32): MeshParts CArray<CMeshMorphSetParts> +0 (engine/CArray.h = STLport vector); parts are 16 bytes in retail: MeshIndex ulong +0, TextureMorphs (vector<CEngineTextureMorph>, 12) +4.
- CEnginePrimitiveAnimatingMesh (Ego_r): MeshEffect CIVCountedPointer<CEngineMeshEffect> +0x80 -> written via inline operator= (FABLE_RETAIL_CIVCOUNTEDPOINTER + FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN).
- NGraphicAppearance::CPrimitive (engine/NGraphicAppearance_CPrimitive.h, 16 bytes): Type +0, PrimitiveHandle +4 (CEnginePrimitiveHandle, CBaseClass ctor + vptr 0x0122F598, handle 0), BankIndex +0xC; ctor CPrimitive(EPrimitiveType type, long bank_index=0). Here type=2, bank_index=part.MeshIndex.
- Draft models LOCALLY but shared headers exist: CTCBase, CThing, CTCGraphicAppearance, CPrimitive (CNodeParams), CIVCountedPointer (CMeshRef/CEngineMesh), CCountedPointer, CArray, C3DVector, CRGBColour, CEnginePrimitiveThemeChannelDesc, CEnginePrimitive, NGraphicAppearance::CAGraphicAppearanceComponent.

## 5. Callee-body visibility
- Inline in retail (must be visible): std::set<CCharString> ctor/dtor (header alloc 0x14 + init, then _M_erase+free), vector<CPrimitive*>::resize (expanded: erase(begin()+n,end()) with memmove + `cmp eax,eax` self-compare, else _M_fill_insert out of line), CPrimitive ctor (CBaseClass ctor out of line), CIVCountedPointer operator= and dtor, GetUniqueID, GetParentThing, GetAlpha, CCountedPointer operator->, CArray begin/end.
- Out of line: _M_fill_insert 0x642440 (normal), _Rb_tree::_M_erase 0x4ABA70, GetFullScale 0x4BBFF0, GetFadeDistance 0x4BC180 (all in CTCGraphicAppearance's TU, not this one).
- Opt-ins: fable_containers.h resize is inline by default (do NOT define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE); resize(n, const T& x) takes x by const ref -> retail `[esp+34h]=0` is the `_Tp()` temp of resize(n) (one-arg, default, not _ONE_DECL). Tree ctor/dtor inline by default (do NOT define FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE).

## 6. Sibling rows
- 00/73/CTCSpotLight_OnAppearanceDraw_00731ea0.cpp (same pattern, landed, `// Flags: none`): defines FABLE_RETAIL_BASE_OBJECT, _CCLASS, _DEF_CLASS_BASE, _CTCBASE, _CTHING, _C3DVECTOR, _CIVCOUNTEDPOINTER, _CTCGRAPHICAPPEARANCE_MOVEMENT, _CTCGRAPHICAPPEARANCE_FLARE, _CENGINEPRIMITIVE_ID_CTOR, _CENGINEPERSISTENTPRIMITIVEBASE; includes engine/CThing.h, NGraphicAppearance_CTCGraphicAppearance.h, CEnginePrimitive.h; idiom `p = tc_appearance.AddPersistentPrimitive(NGraphicAppearance::CPrimitive((NGraphicAppearance::EPrimitiveType)3));` and `GetParentThing().GetUniqueID()`.
- 00/64/CTCLight_DrawFlare_00641ae0.cpp: same macro set; AddPersistentPrimitive(CPrimitive(...)) into a vector slot.
- 00/72/CTCRandomAppearanceMorph_OnSerialise_00727bc0.cpp: local class model `class CTCRandomAppearanceMorph : public CTCAppearanceMorphBase, public NGraphicAppearance::CAGraphicAppearanceComponent` with CCountedPointer PMeshMorphSet +0x10, Seed +0x18 (FABLE_RETAIL_CTCBASE, engine/CTCBase.h, CCountedPointer.h, NGraphicAppearance_CAGraphicAppearanceComponent.h).
- 00/72/CTCRandomAppearanceMorph_OnAppearanceClear_00727bf0.cpp: adds `std::vector<NGraphicAppearance::CPrimitive*> PrimHandles` +0x1C; uses engine/CArray.h.
- 00/72/CTCRandomAppearanceMorph_GetMeshesUsed_00727c20.cpp (preceding vslot; differ-grade local model): CMeshMorphSetParts {MeshIndex +0, TextureMorphs +4}, 16 bytes.

## 7. Old draft's model errors
1. Wrong callee identities (GetAlpha/GetLevel/GetPrimitive/InsertAppearanceNode/CNodeParams/CMorphedMesh) -- see table.
2. Ctor arg list wrong. Retail push order (reverse) proves: (ParentThing->UniqueID, part->MeshIndex, part->TextureMorphs, hidden_set, *info.Anim, *info.Pos, *info.Orientation, app.GetFullScale(), true, app.GetAlpha(), app.GetTintColour(), app.GetFadeDistance(), (EEngineInterpolationSet)0, app.LightingChannel).
   The `push ecx; push esp; call GetTintColour` is VC7.1 building the by-value CRGBColour arg in place (hidden result pointer = arg slot): pass `app.GetTintColour()` directly as the argument.
3. Missing local `std::set<CCharString> hidden_materials;` (CHiddenMaterialSet) constructed ONCE before the loop (after the empty test) and destroyed after it; passed by const ref to every mesh ctor.
4. After the ctor: `mesh.MeshEffect = app.GetPMeshEffect();` (temp CIVCountedPointer at esp+0x2C, inline assign into mesh+0x80, temp dtor) BEFORE SetPrimitiveGraphics(PrimHandles[i], mesh, 0). Draft had a dead GetPrimitive.
5. resize: draft hand-wrote `Last = First+n`; retail is STLport resize(n) inline: erase(begin()+n,end()) (memmove, `cmp eax,eax`) else _M_fill_insert(end(), n-size(), T()). Use real std::vector<CPrimitive*> PrimHandles; `PrimHandles.resize(i + 1);`.
6. Order: GetFullScale (stored [esp+40h]) then GetFadeDistance ([esp+38h]) then `bool first = PrimHandles.empty()` (begin==end via sete) then set ctor, then loop.
7. Loop: iterator over PMeshMorphSet->MeshParts, end() re-read each iteration ([this+4]->[+4]), stride 0x10, plus an index (two strength-reduced counters i+1 / i*4). Body: if (first) { resize(i+1); PrimHandles[i] = app.AddPersistentPrimitive(CPrimitive((EPrimitiveType)2, part->MeshIndex)); } else if (info.ReasonsForDraw == 2 || == 1 || == 3) { app.RefreshPrimitiveAnimationAndPos(PrimHandles[i], *info.Anim, *info.Pos, *info.Orientation); continue; } then build mesh path.
   ParentThing is re-read each iteration through GetParentThing() (`[this-8]`, UniqueID lo/hi at +0x18/+0x1C).
8. DrawInfo field offsets wrong (Anim +0xC, Pos +4, Orientation +8, ReasonsForDraw +0x1C).

## 0x007BF790

# DOSSIER 0x007BF790 (762 bytes) -- NThingShot::CTCShotDefinitionBase::Init (target-POSITION overload)

## 1. Retail shape (xdis 007bf790 0x30a)
- __thiscall, `ret 0x20` (8 dword args), NO frame pointer (esp frame, `sub esp,8`), NO SEH prologue.
- arg1 = CDefPointer<const CThingShotDef> BY VALUE (cached in edi for the cmp; released at the very end `[esp+14h]` dec/[vt+4]).
- 8-byte local = one temp CIntelligentPointer<CThing> (vtbl 0x01238C6C) at [esp+10h], reused for PShooter then PWeapon.
- Size-idiom hints: `mov ebx,2000h ; mov ebp,4Dh` constants cached in registers, `or byte ptr [eax+59h],2` RMW
  (CTCTrapBase_OnInitialActivate_00787d4f.cpp says that RMW appears only under `optimize("s",on)`; speed gives mov/or/mov).
  No landed sibling of this class uses optimize("s") though -- try both.
- Parent 0x4d (SolidToThing) lookup: TWO separate GetTC expansions cross-jumped (block A 0x7BF989 `lea edi,[eax+44h]; lea eax,[esp+38h]; push eax; jmp 7BF9CD`;
  block B 0x7BF9C5 `lea edx,[esp+38h]; lea edi,[eax+44h]; push edx` -> shared tail). Different register/order prefixes prove two source copies.
  `xor edi,edi` (solid=0) only on the AddTC path, edi then reused as the two zero AddTC args.

## 2. Name / signature (two sources)
- Ego_r.pdb (egor_pdb_names.tsv/egor_signatures.tsv @0x007BE730, 829 b) + FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv @0x017D75C1):
  `?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1ABVC3DVector@@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z`
- = `public: void NThingShot::CTCShotDefinitionBase::Init(CDefPointer<const CThingShotDef>, CThing*, CThing*, const C3DVector&, float, float, float, const CDefPointer<const CMeleeCombatAbilityDef>&)`
- Overload proof: arg4 is copied 12 bytes into +0x51 (TargetPos) -> the C3DVector overload. The CThing* overload is 0x007C0690 (PBV4@, Ego_r 0x007BEDC0), same order in both PDBs.
- Manifest label `CTCShotDefinitionBase::Init` is right, but the class must be NThingShot:: and the def types `const` (draft used CDefPointer<CThingShotDef> -- wrong mangling).
- Shared header already declares it: engine/NThingShot_CTCShotDefinitionBase.h under FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT + _INIT_POSITION
  (params named def, shooter, weapon, target, accuracy, damageMultiplier, buildupTime, ability).
- Callers (xref): 007bd4f7, 007bd76b, 007c0ab1, 0087fb66, 008c69b7 (landed CCreatureActionTrollThrowProjectile_PerformAction_008c6860.cpp:103 `shot->Init(shotDef, PCreature.Get(), 0, TargetPos, ...)`), 008f35e7, 008f38cc.

## 3. Callees (landed file / landed+PDB name / draft spelling)
- 0x00A01B10 / 0x00A01B90 / 0x00A01B50 / 0x00A01C10 CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (rebuild/src/compiled/00/a0/*) -- use shared engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER), not the local model.
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (00/40/CVectorMap_LowerBound_0040f020.cpp) -- via CThing::GetTC(id,&p) under FABLE_RETAIL_CTHING_GETTC_EXPANDED.
- 0x00768210 **?CopyAugmentationDataFromThing@CTCObjectAugmentations@@QAEXABVCThing@@@Z** (00/76/CTCObjectAugmentations_CopyAugmentationDataFromThing_00768210.cpp; header CTCObjectAugmentations.h:38 takes `const CThing&`). DRAFT WRONG: "InitAbilitySlot(CThing*)".
- 0x00770FA0 ?AddThingNotSolidTo@CTCSolidToThing@@QAEXPBVCThing@@@Z (CTCSolidToThing.h:17, `const CThing*`). Draft took non-const CThing*.
- 0x004C9D60 ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z (CThing.h:258, ungated). Arg "CTCSolidToThing" @0x0123B47C via CCharString(const char*, long=-1) 0x0099EBF0 / ~ 0x0099EAE0.
- 0x004C73D0 **?GetPos@CThing@@QBEABVC3DVector@@XZ** (00/4c/CThing_GetPosition_004c73d0.cpp). DRAFT WRONG: "GetPosition".
- 0x0076EFA0 ?UpdateWeaponTrails@CTCWeaponTrail@@QAEXXZ (CTCWeaponTrail.h:21) -- matches.
- 0x00686A70 ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ (CTCBase, returns CWorld&). Draft: CTCShotDefinitionBase::GetWorld returning CWorld* -- wrong owner/type.
- 0x0049D870 **?GetFrame@CWorld@@SIJXZ** (static __fastcall, CWorld.h:142). DRAFT WRONG: "GetWorldFrame". Source is `GetWorld().GetFrame()` (GetWorld still called: `mov ecx,esi; call 686A70; call 49D870`).
- Physics vslot +0xB0 = slot 44 = **?ReturnToDefaultInteractionFlags@CTCPhysicsBase@@UAEXXZ** (rtti CTCPhysicsBase vtbl 0x0125D2C4 slot 44 = 0x006B1120, landed CTCPhysicsBase_ReturnToDefaultInteractionFlags_006b1120.cpp; FableWin 0x0179F5B3; CTCPhysicsBase.h:70). DRAFT WRONG: "Reset". Reached via ParentThing->GetPhysicsTC() (PhysicsTC +0x60).
- Own vslot +0x84 = slot 33 InitMovement (rtti 0x0126E1A4 slot 33 = 0x007C2CE0); +0x88 = slot 34 = InitAttachedEffects (Ego_r ?InitAttachedEffects@CTCShotDefinitionBase@NThingShot@@MAEXXZ, landed 0x007C25A0). DRAFT WRONG: "InitGraphics". Header declares both under FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION (+_MOVEMENT). Call order: InitAttachedEffects() then InitMovement().
- +0x64 `or byte [eax+59h],2` = CThing::GetTCGraphicAppearance()->SetForceResendEveryRenderFrame(true) (FableWin ?SetForceResendEveryRenderFrame@CTCGraphicAppearance@NGraphicAppearance@@QAEX_N@Z 0x017DD73C; bit1 of the +0x59 byte = ForceResendEveryRenderFrame; landed CTCCarryable_SetAsCarriedBy_00705d40.cpp:128/203 models it locally). Shared NGraphicAppearance_CTCGraphicAppearance.h has the bitfield line 96 but no setter: inline `{ ForceResendEveryRenderFrame = b; }` may give mov/or/mov; check.

## 4. Members (shared header engine/NThingShot_CTCShotDefinitionBase.h, FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT tail; PDB names struct_layouts_egor.tsv = retail+4 after HitThingsList)
- +0x04 ParentThing (CTCBase.h), +0x28 PShooter, +0x30 PWeapon (CIntelligentPointer<CThing>), +0x38 PShotDef CDefPointer<const CThingShotDef>, +0x3c PShotAbilityDef CDefPointer<const CMeleeCombatAbilityDef>,
  +0x40 AccuracyPercentage, +0x44 DamageMultiplier, +0x48 ShotBuildupTime (float), +0x4c StartFrame long, +0x51 TargetPos (header: `unsigned char TargetPos[12]` opaque -- assign via *(C3DVector*)TargetPos or memcpy-shaped copy; retail copies through `lea ecx,[esi+51h]` 3 dwords),
  +0x60 DieOnFrame=-1, +0x64 PassedTarget, +0x66 KnockDown, +0x67 Decapitate (bools), +0x70 BestCollisionPos C3DVector, +0x90 BestCollisionType ECollideType (=0 -> needs a cast `(ECollideType)0`).
- MadeSound +0x65 is NOT written; CanBeDeflected +0x68 not written.
- KnockDown: retail stores 0 then `if (PShotAbilityDef) KnockDown = true` (test of [esi+3Ch] after the store) -- probably `KnockDown = !PShotAbilityDef.IsNull()`-ish written as two statements or `SetKnockDown`.
- Store schedule: 0x40 <- arg5, 0x48 <- arg7, 0x44 <- arg6 then 0x60/0x64/0x66/0x67.
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING): UsingComponents bitset +0x20 (0x4d word +0x28 bit13, 0x4c bit12 `test ch,10h`, 0xbf word +0x34 bit31 `jns`), Components +0x44, PhysicsTC +0x60, PTCGraphicAppearance +0x64, Dead bit0 of +0x91 (IsDead()).
- TCI enums: TCI_WEAPON_TRAIL=76, TCI_SOLID_TO_THING=77, TCI_OBJECT_AUGMENTATIONS=191 (ETCInterfaceType.h); StatGetInterfaceType exists in CTCSolidToThing.h:16, CTCObjectAugmentations.h:140, CTCWeaponTrail.h:14.
- Classes the draft models LOCALLY that have shared headers: C3DVector, CCharString, CBaseIntelligentPointer/CIntelligentPointer, CDefPointer/CDefBase, CThingShotDef, CThing (+CInterfaceMap), CTCPhysicsBase, CTCObjectAugmentations, CTCSolidToThing, CTCWeaponTrail, CWorld, CTCShotDefinitionBase (whole class, pack(1)), CTCGraphicAppearance (as CTCInterface5B).

## 5. Callee-body visibility / opt-ins
- CDefPointer operator= is __forceinline in engine/CDefPointer.h (needs FABLE_RETAIL_DEF_CLASS_BASE); its re-read-after-release form matches the arg8 reference (`cmp ecx,[edi]` ... `mov eax,[edi]`), and caches the by-value arg1 in a register (no alias) -- matches retail.
- PShooter = shooter: retail builds a temp CIntelligentPointer (ctor, vtbl store, SetPItem) then GetPItem(temp) -> SetPItem(member) -> ~temp. Needs inline T* ctor (FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR or default inline) + FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (`item=other.GetPItem(); SetPItem(item)`), all CBaseIntelligentPointer members out of line.
- IS_THING_ALIVE<T> (engine/ThingAlive.h, FABLE_RETAIL_IS_THING_ALIVE) fits both alive tests: weapon (`test ebp; je; test [ebp+91h],1; jne`) and PShooter (GetPItem for !=0, GetPItem again for ->IsDead()). Needs CIntelligentPointer operator!=(int)/operator-> opt-ins (see COMPARE_NULL_INT / DEREFERENCE) -- 3rd GetPItem at 0x7BF93D is the next `PShooter->GetTC(...)`.
- CThing::GetTC(id,&p) / GetTC<T>(&p) FABLE_FORCEINLINE expanded body: FABLE_RETAIL_CTHING_GETTC_EXPANDED (conditional-expression form gives `mov ecx,[eax+4]` direct, as the 0x4c WeaponTrail site; the 0xbf/0x4d sites go via eax because of interleaved pushes).
- CVectorMap::LowerBound out of line (0x0040F020). AddTC, GetPos, GetWorld, GetFrame, CopyAugmentationDataFromThing, AddThingNotSolidTo, UpdateWeaponTrails all out of line.
- Same TU neighbours (defined earlier in the .cpp, likely ShotDefinitionBase.cpp): 0x007BED10 IsToDie ... 0x007BF510 OnCollision, 0x007BFA90 GetWeaponTrailGraphic (next), 0x007C0690 Init(CThing*) overload.

## 6. Sibling landed rows
- rebuild/src/compiled/00/7b/NThingShotCTCShotDefinitionBase_OnCollision_007bf510.cpp: no pragma; defines CTCSHOTDEFINITIONBASE_COLLISION/_MOVEMENT/_ONHITBYMELEE, CINTELLIGENTPOINTER(+_COPY,_CONVERT_MUTABLE,_ASSIGN), C3DVECTOR, DEF_CLASS_BASE, CTCBASE, CTHING(+_ORIGINAL_VTABLE,_GETTC_EXPANDED,...), IS_THING_ALIVE, CWORLD...; includes engine/CThing.h, NThingShot_CTCShotDefinitionBase.h, CTCWeaponTrail.h, ThingAlive.h. Best template: copy its define block.
- 00/7b/NThingShotCTCShotDefinitionBase_CTCShotDefinitionBase_007bf460.cpp (ctor), _007bf2b0 (dtor, pack(1) local), OnRemove_007bf350, CTCShotDefinitionBase_HasPassedTarget_007bf3d0.cpp; 00/7c/CTCShotDefinitionBase_InitAttachedEffects_007c25a0.cpp & UpdateCreatureAwarenessOfMe_007c04a0.cpp use `#pragma auto_inline(off/on)` around helpers.
- Caller with INIT_POSITION opt-in: 00/8c/CCreatureActionTrollThrowProjectile_PerformAction_008c6860.cpp.

## 7. Old-draft model errors
- Wrong names: InitAbilitySlot->CopyAugmentationDataFromThing(const CThing&), GetPosition->GetPos, GetWorldFrame->CWorld::GetFrame (static, via GetWorld()), Reset->ReturnToDefaultInteractionFlags, InitGraphics->InitAttachedEffects; GetWorld belongs to CTCBase and returns CWorld&.
- Wrong types: CDefPointer<CThingShotDef> (needs const), class not in NThingShot.
- Parent-0x4d block: draft writes ONE GetInterface after `if(!HasTC) AddTC`; retail has TWO GetTC copies cross-jumped. Likely source:
  `CTCSolidToThing* solid = 0; if (!ParentThing->GetTC(&solid)) { ParentThing->AddTC(CCharString("CTCSolidToThing"), false, 0); ParentThing->GetTC(&solid); } solid->AddThingNotSolidTo(PShooter.GetPItem()/Get());`
  (final GetPItem re-derives `lea ecx,[esi+28h]`, not ebx).
- Shooter block: should be IS_THING_ALIVE(PShooter) + `PShooter->GetTC(&solidShooter)` (GetPItem x3), not a cached local.
- Weapon block: IS_THING_ALIVE(weapon) && ParentThing->GetTC(&aug) -> aug->CopyAugmentationDataFromThing(*weapon).
- Local pack(1) whole-class model and local CThing must go: shared headers exist.

## 0x006BFCD0

# DOSSIER 0x006BFCD0 CTCBuyableHouse::GetSignPostPos (764 bytes, retail 0x006BFCD0..0x006BFFCB)

## 1. Retail shape (xdis 006bfcd0 0x30c)
- __thiscall, `ret 0Ch` (3 args), returns bool in al (`mov al,1` / `xor al,al`, two separate epilogues 0x6BFE14 and 0x6BFFC0).
- NO frame pointer (`sub esp,6Ch ; push ebx ; push ebp ; push esi ; push edi`), NO SEH (CCharString temp is destroyed without EH state),
  no /GS cookie. Default optimisation: no pragmas needed (siblings 0x006BFB90 / 0x006C0160 / 0x006C0F00 use none).
- `this` spilled to [esp+10h] at entry, reloaded twice (GetDefinitionManager, and edi=this for PeekWorldMap / ParentThing at the end).
- `main` kept in bl; float side (2.0f main / 4.0f dressing) is written into the `main` ARGUMENT SLOT [esp+80h] (frame reuse of the param home).
- Locals (esp after the 4 pushes): +0x10 this, +0x14 CCharString temp, +0x18 key 0x21 then reused for the def index,
  +0x1C..0x24 candidate C3DVector (passed to GetGroundSizeZAt, .Z overwritten by its result),
  +0x28..0x30 ahead = Forward*3.0f (all 3 stored), +0x34..0x3C axis (0,0,1) for Rotate, +0x40/+0x44 C2DVector copy of Forward
  (copied with integer moves = 2D-from-3D copy), +0x4C..0x54 perp*side temp (only Y spilled at +0x50; Z at +0x54 = side*0.0f HOISTED
  before the loop at 0x6BFE28..3E), +0x58..0x60 (ahead+pos) temp with only Z spilled (+0x60), +0x64..0x78 CRightHandedSet copy (24 bytes).
- Loop over the parent's TCI 0x21 component's std::list: `mov ebx,[comp+0Ch] ; mov esi,[ebx]` (STLport list _M_node, begin = node->next),
  `lea ecx,[esi+8] ; call GetPItem`, `mov esi,[esi]`.
- Second phase: `for (i=0;i<2;++i)` counter in esi (`inc esi ; cmp esi,2 ; jl`); `if (i)` Rotate((0,0,1), 0.5f).
- End test: `cmp byte [ParentThing+10h],3 ; jne continue` (CClass::Type == CThingBuilding::StatGetType()=3); IsPointInside(pos) false -> return true.
- Order in the door branch: orientation copy (if non-null) BEFORE *pos store; signpost branch: *pos first, then orientation (draft matches both).
- Constants: 0x0122DEDC = 0.0f, 0x0122DED4 = 3.0f; 0.5f is the immediate 0x3F000000; strings 0x0125D73C "MARKER_BUYABLE_HOUSE_MAIN_SIGNPOST"
  (main), 0x0125D714 "MARKER_BUYABLE_HOUSE_DRESSING_SIGNPOST" (read from the retail .exe).

## 2. Identity (manifest label is RIGHT)
- Ego_r.pdb name set (ghidra_out/egor_pdb_names.tsv, Ego_r addr 0x006F8180) AND FableWin.pdb (_fablewin_api.tsv 0x017A5A12 / 0x024AEB80,
  naming_stragglers/FableWin.exe.signatures.tsv): `?GetSignPostPos@CTCBuyableHouse@@AAE_N_NAAVC3DVector@@PAVCRightHandedSet@@@Z`.
  (The egor_pdb_names row AT 0x006bfcd0 = SetAutomaticCameraControl is an Ego_r address, unrelated.)
- Signature: PRIVATE, non-const: `bool GetSignPostPos(bool main, C3DVector& pos, CRightHandedSet* orientation);`
  -> the draft's `C3DVector* pos` is WRONG (PDB `AAV` = reference). Same bytes, but the leaf must be the reference form.
- Callers (xref): 0x006C06DF in CTCBuyableHouse::GetRentMoneyPos 0x006C06D0 (?GetRentMoneyPos@CTCBuyableHouse@@AAE_NAAVC3DVector@@JJ@Z) and
  0x006C098A in CTCBuyableHouse::CreateSignPost 0x006C0970 (?CreateSignPost@CTCBuyableHouse@@AAEPAVCThing@@ABVCCharString@@_N@Z).
  Neither is landed. Not a virtual (no vtable slot).
- No header declares GetSignPostPos yet (grep rebuild/include, rebuild/src: none). CTCBuyableHouse.h opt-ins: FABLE_RETAIL_CTCBUYABLEHOUSE (+ _FRAMEUPDATE,
  _WIFE, _RECEIVEDCONFIRMATION, _VSLOTS); the row must declare GetSignPostPos itself (CreateMainSignPost 0x006C0F00 uses the
  `#define CTCBuyableHouse CTCBuyableHouseLayout` include pattern + local class; or add a nested opt-in).

## 3. Callees (landed file | landed name | PDB name | draft spelling)
| addr | landed row | PDB / landed name | draft | flag |
|---|---|---|---|---|
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound (non-const) | CInterfaceMap::LowerBound | local stand-in; use engine/CVectorMap.h via CThing GetTC |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString::CCharString(const char*, long) (fable_string.h) | same | ok |
| 0x006869D0 | 00/68/CTCBase_GetDefinitionManager_006869d0.cpp | ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ (protected const, returns const CGameDefinitionManager&) | GetDefinitions() returning CDefinitionManager* | WRONG name + return type |
| 0x009AD410 | 00/9a/CDefinitionManager_GetDefGlobalIndexFromName_009ad410.cpp | ?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z (const) | non-const | missing const (engine/CDefinitionManager.h line 74 declares it const) |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | same | ok |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (returns CBaseObject*) | returns CThing* | wrong return; use CIntelligentPointer<CThing>::Get() (static_cast of GetPItem, engine/CIntelligentPointer.h line 69) |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | GetPosition | WRONG name (CThing.h line 411 has GetPos) |
| [phys+0x120] | slot 72 of CTCPhysicsBase vtable 0x0125D2C4 -> 0x006B1290 (manifest GetRHSet) | virtual const CRightHandedSet& CTCPhysicsBase::GetRHSet() const (Ego_r ?GetRHSet@...UBEABVCRightHandedSet@@XZ); slot 71 = GetRHSet(float) 0x006B1450 | CPlacement::GetOrientation | WRONG class+name (object is CThing::PhysicsTC +0x60, via GetPhysicsTC()) |
| 0x00A22F80 | 00/a2/CRightHandedSet_Rotate_00a22f80.cpp | ?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z | RotateAboutAxis | WRONG name; not declared in engine/CRightHandedSet.h (row declares it) |
| 0x00686D20 | 00/68/CTCBase_PeekWorldMap_00686d20.cpp | ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ (const) | GetWorldMap | WRONG: GetWorldMap is 0x00686D30 (non-const); retail calls PeekWorldMap |
| 0x0063A300 | 00/63/CWorldMap_GetGroundSizeZAt_0063a300.cpp | ?GetGroundSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z | same | ok (const, via const CWorldMap&) |
| 0x0082D000 | 00/82/CThingBuilding_IsPointInside_0082d000.cpp | ?IsPointInside@CThingBuilding@@QBE_NABVC3DVector@@@Z (const) | non-const | missing const (engine/CThingBuilding.h line 43 const) |

## 4. Members
- this: CTCBuyableHouse : CTCBase; only ParentThing (+0x04, CTCBase, accessor GetParentThing()) is read. engine/CTCBase.h (FABLE_RETAIL_CTCBASE)
  declares PeekWorldMap 0x00686D20 and GetDefinitionManager 0x006869D0 unconditionally (protected).
- CThing (engine/CThing.h): UsingComponents bitset<274> +0x20 (HasTC; `test byte [eax+24h],2` = bit 0x21, `test byte [eax+24h],40h` = bit 0x26);
  Components CVectorMap +0x44 (GetTC; FABLE_RETAIL_CTHING_GETTC_EXPANDED gives the exact `lea esi,[eax+44h]` .. `mov esi,[esi+4]` end-reread shape seen
  here, key stored AFTER the presence test as retail does); PhysicsTC +0x60 (GetPhysicsTC()); DefGlobalIndex ushort +0x8C (GetDefGlobalIndex(),
  movzx); Dead bitfield +0x91 bit 0 (IsDead()/IsAlive()); CClass::Type uchar +0x10 (engine/CClass.h GetType(), CThingBuilding::StatGetType()=3).
  Opt-ins: FABLE_RETAIL_CTHING, FABLE_RETAIL_CTHING_TCDRIVER (HasTC branch), FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (created for sibling
  0x006C1250 FrameUpdate), FABLE_RETAIL_CTHING_GETTC_EXPANDED.
- Interface 0x21 = TCI_THING_OWNER (engine/ETCInterfaceType.h line 38; landed CTCThingOwner::GetInterfaceType 0x004DDB18 returns 0x21) ->
  the component is CTCThingOwner (engine/CTCThingOwner.h): std::list<CIntelligentPointer<CThing> > ObjectList +0x0C (struct_layouts_egor
  CTCThingOwner +12 ObjectList); accessor GetObjectList() under FABLE_RETAIL_CTCTHINGOWNER_OWNEDOBJECTS, StatGetInterfaceType under
  FABLE_RETAIL_CTCTHINGOWNER. The draft's "CTCBuildingContents" / ThingsHead is a MADE-UP name.
- Interface 0x26 = TCI_DOOR (ETCInterfaceType.h line 43; engine/CTCDoor.h). The thing the draft calls `marker` is the house's DOOR
  (sign goes 3 units in front of the door, 2/4 to the side). The def-index match is the sign-post MARKER thing.
- CRightHandedSet (engine/CRightHandedSet.h): protected Up +0, Forward +0xC, size 24; GetForward()/GetUp() inline. Copied by value (+0x64).
- C3DVector.h / C2DVector.h: C2DVector(const C3DVector&) under FABLE_RETAIL_C2DVECTOR_FROM3D; C3DVector::ToC2DVector under FABLE_RETAIL_C3DVECTOR_TO2D;
  operator+/operator* FABLE_VECTOR_INLINE; ADD_XY_GROUP / SCALE_X_PAREN variants exist for FP order.
- Classes the draft models LOCALLY that have shared headers: C3DVector, C2DVector, CRightHandedSet, CCharString (fable_string.h),
  CInterfaceMap (engine/CVectorMap.h), CPlacement (=CTCPhysicsBase.h), CThing, CThingBuilding, CBaseIntelligentPointer (CIntelligentPointer.h),
  CTCBuildingContents (=CTCThingOwner.h), CDefinitionManager, CWorldMap, CTCBuyableHouse, CTCBase.

## 5. Callee visibility
- All callees are out-of-line calls in retail; none of their bodies should be visible. GetTC/HasTC are __forceinline templates
  (no out-of-line copy in Fable.exe, owner decision 2026-11-01). LowerBound 0x0040F020 is out of line (declared in CVectorMap.h).
- GetDefinitionManager/PeekWorldMap are out-of-line CTCBase accessors in another TU (0x006869D0 / 0x00686D20): declarations only.
- CIntelligentPointer<CThing>::Get / operator-> are inline wrappers over out-of-line GetPItem.
- Same-TU neighbours (CTCBuyableHouse file, defined earlier): IsHouseBeingUsed 0x006BFB10, NeedsMainSignPost 0x006BFB90; later GetValueOfTrophies 0x006BFFD0.
  None is called here.

## 6. Sibling landed rows
- rebuild/src/compiled/00/6c/CTCBuyableHouse_GetCurrentCollectableRentValue_006c0160.cpp: same TCI 0x21 list walk + GetDefinitionManager/
  GetDefGlobalIndexFromName + GetPItem; local CThing model with forceinline HasTC/GetTC (still old-style local models); no pragmas.
  Its note: "the 0x21 key never shares the name's word" -- here retail DOES share +0x18 for key and def index.
- rebuild/src/compiled/00/6b/CTCBuyableHouse_NeedsMainSignPost_006bfb90.cpp: FABLE_RETAIL_CINTELLIGENTPOINTER; FABLE_FORCEINLINE helper idiom.
- rebuild/src/compiled/00/6c/CTCBuyableHouse_CreateMainSignPost_006c0f00.cpp: `#define CTCBuyableHouse CTCBuyableHouseLayout` + engine/CTCBuyableHouse.h.
- rebuild/src/compiled/00/6b/CTCBuyableHouse_GetValueOfTrophies_006bffd0.cpp: uses `#pragma auto_inline(off/on)` around helpers.
- Sibling 0x006C1250 FrameUpdate (pending) introduced FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE + FABLE_RETAIL_CTHING_HASTC_FORCEINLINE.
- None of the siblings carries a `// Flags:` line (default flags).

## 7. Draft model errors vs disassembly
- Names: GetPosition->GetPos, RotateAboutAxis->Rotate, GetOrientation->GetRHSet (CTCPhysicsBase slot 72), GetWorldMap->PeekWorldMap (0x00686D20,
  const), GetDefinitions->GetDefinitionManager (const CGameDefinitionManager&), CTCBuildingContents->CTCThingOwner::ObjectList, `C3DVector* pos`
  -> `C3DVector& pos`, const missing on GetDefGlobalIndexFromName / IsPointInside / GetGroundSizeZAt receiver.
- Local naming: 0x26 thing = door, def-index thing = sign-post marker.
- Frame residue (739 v 764, 0x5C v 0x6C): retail materialises ahead = Forward*3.0f as a full 3-float temp (+0x28) BEFORE GetPos is called,
  then (ahead + door->GetPos()) as a temp (+0x58, only Z spilled), then + (perp*side) (+0x4C, Y spilled, Z = hoisted side*0.0f).
  i.e. the expression is `(rh.GetForward()*3.0f + door->GetPos()) + C3DVector(-f.Y, f.X, 0.0f)*side` with f a C2DVector copy of Forward;
  the draft's `base = Forward*3; base += GetPos()` (in-place +=) merges ahead and the sum into one slot -> 16 fewer bytes.
  Try: `C3DVector candidate = set.GetForward()*3.0f + door->GetPos() + C3DVector(-forward.Y, forward.X, 0.0f)*side;` as one expression
  (operator+ returning by value creates the separate temporaries), the C2DVector built with FROM3D/ToC2DVector.
- Final x add order (retail `fxch st(2) ; fadd st(2)`): comes from the (ahead+pos).x + perp.x operand order; the one-expression form above
  should give retail's order (left = ahead+pos, right = perp*side).
- Draft control flow, loop forms (list walk with if/else-if on HasTC(0x26) then def index; `for (i<2)` with `if (i)` rotate), and the
  `Type==3 && !IsPointInside(pos)` test already match retail.

## 0x0055D180

# DOSSIER 0x0055D180 -- CTCPhotojournalCapture::TakeQuickScreenshot (772 bytes)

## 1. Retail shape (xdis 0055d180 0x314)
- thiscall member, `this` kept in esi (push esi; mov esi,ecx). Plain `ret` (two exits, both `add esp,274h ; ret`). No args.
- NO frame pointer (esp-based `sub esp,274h`), NO SEH/EH frame (TU built without /EHsc; siblings AddDescription 0x55CFB0 /
  GetDescription 0x55D070 also no EH frame). /GS: cookie 0x0139C8A8 stored at [esp+274h] after push esi, check
  0x00BFE9F9 at both exits -> `// Flags: /GS`. No `optimize("s")` / `("y",off)` signs (no ebp, no biased frame).
- Locals (offsets rel. to esp after `push esi`): name a +0x0C, clock b +0x14, path +0x10, Replace temps +4/+8,
  path-chain temps +4,+8,+0x1C,+0x20,+0x24, CCharString +0x18, FILETIME result +0x24, CDiskFileWin32 +0x2C (0x1C bytes,
  Pathname at +0x44), SYSTEMTIME +0x48, CDateAndTime +0x58, dateText wchar[128] +0x74, timeText wchar[128] +0x174, cookie +0x274.

## 2. Name / signature (two sources)
- Ego_r.pdb (egor_pdb_names.tsv 005d64a0) `?TakeQuickScreenshot@CTCPhotojournalCapture@@QAEXXZ`; FableWin
  (_fablewin_api.tsv 023e24c0, 1332 bytes debug). Order of Ego_r class methods (OnCreate, AddDescription, GetDescription,
  TakeQuickScreenshot, TakeQuickScreenshotNoHud, TakeTheBloodyScreenshot, GlobalUpdate) == retail order 0x55CFA0, 0x55CFB0,
  0x55D070, 0x55D180, 0x55D490, 0x55D4C0, 0x55D4D0. Callers (xref.py): 0x55D4C0 TakeTheBloodyScreenshot, 0x55D4E4
  GlobalUpdate (landed, declares `void TakeQuickScreenshot(); // 0x0055D180`), 0x55D510 (GlobalUpdate 2nd call),
  0x6880AE CInputProcessMain::ProcessInput (landed, declares it too). Manifest label is CORRECT.
- Mangled leaf: `?TakeQuickScreenshot@CTCPhotojournalCapture@@QAEXXZ` -> `public: void __thiscall ...(void)`, non-const, non-virtual.
- Side note: 0x0055D4C0 is landed as `global_Destval_0055d4c0.cpp` -- it is really TakeTheBloodyScreenshot
  (`call 55d180 ; mov ecx,[13b8790] ; jmp CPlayerGui::Open 4374b0`; Ego_r ?TakeTheBloodyScreenshot@CTCPhotojournalCapture@@QAEXXZ). NAME_FIXES candidate.

## 3. Callees (retail addr | landed file | PDB name | draft spelling problem?)
- 0x00437A00 | `ConsvalstdallocatorstdpairEHeroMorphType..._00437a00.cpp` (bogus propagated label, leaf ?Run@T@@) |
  Ego_r ?IsShowingScreenshotMessage@CPlayerGui@@QAE_NXZ (Ego_r 44a6c0/d0/e0 order Show,Hide,IsShowing == retail 9E0,9F0,A00) |
  DRAFT WRONG: `IsScreenshotMessageShowing`.
- 0x004379F0 | CPlayerGui_HideScreenshotMessage_004379f0.cpp | ?HideScreenshotMessage@CPlayerGui@@QAEXXZ | ok.
- 0x004379E0 | `Consval..._004379e0.cpp` (bogus) | ?ShowScreenshotMessage@CPlayerGui@@QAEXXZ | ok. All three are
  out-of-line forwarders (mov ecx,[ecx+248h]; jmp) -> declare non-inline.
- CPlayerGui global 0x013B8790 = `CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass` via inline `Get()` (see landed
  GlobalUpdate 0x55D4D0 / NoHud 0x55D490 idiom; engine/CTBaseSingletonNoDefaultAlloc.h + engine/CCountedPointer.h). Draft's
  `g_pPlayerGui` is a stale stand-in. Retail re-reads [13b8790] before each call.
- IAT [0x0143FE18] GetLocalTime(&SYSTEMTIME).
- 0x009E9A30 | CDateAndTime_CDateAndTime_009e9a30.cpp | ??0CDateAndTime@@QAE@ABU_SYSTEMTIME@@@Z | ok.
- 0x009E9D40 | CDateAndTime_ToFileTime_009e9d40.cpp | ?ToFileTime@CDateAndTime@@QBE?AU_FILETIME@@XZ (hidden result, ret 4) | ok.
- 0x00A0D980 | global_FormatDateTime_00a0d980.cpp (landed as free `__fastcall FormatDateTime(const _FILETIME&, unsigned short*,
  unsigned short*, long)`) | Ego_r ?FormatDateTime@CXBConfig@@SIXABU_FILETIME@@PA_W1H@Z (static member of CXBConfig, int flags) |
  draft spells free `FormatDateTime(..., wchar_t*, wchar_t*, int)`; bytes same (static __fastcall) but PDB owner is CXBConfig.
  Call: ecx=&filetime, edx=dateText, push timeText, push 0.
- 0x0099B6B0 CWideString(const wchar_t*) | 0x0099B510 ~CWideString | 0x0099B7D0 operator= | 0x0099BF30 operator+(CWideString,
  const wchar_t*) `??H@YI?AVCWideString@@ABV0@PBG@Z` | 0x0099BE70 operator+(CWideString,CWideString) | 0x0099AF00 operator
  const wchar_t* (`??BCWideString@@QBEPBGXZ`) -- all in rebuild/include/fable_string.h (FABLE_RETAIL_CWIDESTRING).
- 0x0099C1E0 | NOT landed | ?Replace@CWideString@@QAEXABV1@0@Z (Ego_r b03450; fable_string.h:205 `Replace(const CWideString&
  search, const CWideString& replacement)`) | ok.
- 0x0040D2A0 | CUserProfileManager_Get_0040d2a0.cpp | Ego_r ?Get@CUserProfileManager@@SIAAV1@XZ (returns reference) |
  DRAFT WRONG: `CProfileManager::Get()` returning pointer, member `SaveDir`. Note landed HasScreenshots 0x581B33 spells it
  CTBaseSingleton<CUserProfileManager>::Get (disagrees with Ego_r; the landed row/Ego_r spelling wins).
- 0x00406350 | global_GetValidProfileName_00406350.cpp | ?GetValidProfileName@@YI?AVCWideString@@ABV1@@Z | MISSING in draft.
- 0x0041A600 | NResourceDirectoryNames_GetPhotojournalDir_0041a600.cpp | ?GetPhotojournalDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ |
  MISSING in draft (HasScreenshots 0x581B33 calls the same address "GetSaveDir" -- misnamed there).
- 0x00999AB0 | CAFile_IsRoomForFile_00999ab0.cpp | ?IsRoomForFile@CAFile@@SI_NJABVCWideString@@@Z | DRAFT WRONG: free function;
  it is static CAFile::IsRoomForFile(long, const CWideString&) (ecx=10000000, edx=&path).
- 0x0099AD80 | CDiskFileWin32_CDiskFileWin32_0099ad80.cpp | ??0CDiskFileWin32@@QAE@ABVCWideString@@W4FileAccessType@@K@Z |
  args (path, FileAccessType 4 = FILE_ACCESS_CREATE (CAFile.h enum), 2). Draft used `long` for the enum.
- 0x0099A920 | CDiskFileWin32_Close_0099a920.cpp | Close (virtual; called directly). Explicit Close() then inline dtor Close() again.
- 0x0099A300 | CBaseClassNonCopyable_CBaseClassNonCopyable_0099a300.cpp | ??1CBaseClassNonCopyable@@MAE@XZ (end of inlined
  ~CDiskFileWin32: vtable store 0x0122D06C, Close, ~CWideString Pathname @file+0x18, ~CBaseClassNonCopyable).
- 0x00BFEFB6 | global_D3DXSaveSurfaceToFileW_00bfefb6.cpp (import thunk) | D3DXSaveSurfaceToFileW(path, 1 /*D3DXIFF_JPG*/,
  surface, NULL, NULL).
- 0x0041E5F2 | NUISystemCManager_GetInstance_0041e5f2.cpp | ?GetInstance@CManager@NUISystem@@SIPAV12@XZ | DRAFT WRONG: `CFrontEnd::GetInstance`.
- vtable slot +0xA4 (slot 41) on NUISystem::CManager (vtable 0x01230134 slot 41 = 0x0041CEB3) |
  NUISystemCManager_PlaySample_0041ceb3.cpp | ?PlaySample@CManager@NUISystem@@UAEKAAVCCharString@@J@Z (returns unsigned long,
  ignored; args CCharString& (non-const), long 1) | DRAFT WRONG: called it `PlaySound` slot 41; PlaySoundW is slot 40 (+0xA0).
  Use engine/NUISystem_CManager.h (`#define FABLE_RETAIL_CMANAGER`), which declares PlaySoundW / PlaySample.
- 0x0099EBF0 CCharString(const char*, long=-1) (push -1 explicit) | 0x0099EAE0 ~CCharString -- fable_string.h (FABLE_RETAIL_CCHARSTRING).
- 0x00BFE9F9 __security_check_cookie.

## 4. Members / globals
- this+0x40 = 4 : CTCPhotojournalCapture::TakeScreenshotAfterFrames (long) per landed GlobalUpdate/NoHud layout comment
  (retail 0x44 class; PDB struct_layouts has 72-byte layout with STLport vector difference). DRAFT WRONG name `State`.
  No engine/CTCPhotojournalCapture.h header exists; siblings model the class locally (copy their 0x44 layout block).
- CUserProfileManager+0x08 CWideString ProfileName (struct_layouts_egor.tsv); engine/CUserProfileManager.h exists
  (opt-ins: FABLE_RETAIL_CUSERPROFILEMANAGER_PROFILENAME (declares static m_strProfileName only) etc. -- check whether the
  member ProfileName/Get are exposed; HasScreenshots/CopyCustomTattooToUserDir model it locally with `CWideString ProfileName; // +0x08`).
- GPDisplayManager (CDisplayManager*, 0x013B8390) -> +0x2C CSurface BackBufferSurface, +0x30 = BackBufferSurface.PD3DSurface
  (IDirect3DSurface9*; CSurface: +0 vptr, +4 PD3DSurface). Headers engine/CDisplayManager.h (FABLE_RETAIL_CDISPLAYMANAGER) +
  engine/CSurface.h. DRAFT models a local `CDisplay g_pDisplay` -- wrong name.
- CDiskFileWin32: engine/CDiskFileWin32.h with `#define FABLE_RETAIL_CDISKFILEWIN32` gives the real class (CAFile base,
  inline `virtual ~CDiskFileWin32() { Close(); }`) -- exactly the inlined dtor retail shows. Needs FABLE_RETAIL_CAFILE too.
- CDateAndTime: engine/CDateAndTime.h (members protected; no SYSTEMTIME ctor/ToFileTime declared -> author must add / local-extend).
- Literals: L" " 0x0122D820, L"-" 0x0123175C, L"\\" 0x0122DE78, L"/" 0x0122DE74, L":" 0x0122DE70, L"." 0x0124C4A8,
  L".jpg" 0x0124C49C, "SND_MM_SNAPSHOT" 0x0124C48C.
- Locally modelled classes in the draft that have shared headers: CWideString/CCharString (fable_string.h), CDiskFileWin32,
  CDateAndTime, CDisplayManager (as CDisplay), NUISystem::CManager (as CFrontEnd), CUserProfileManager (as CProfileManager),
  CPlayerGui (engine/CPlayerGui.h; siblings still declare it locally with the singleton template).

## 5. Callee visibility
- CDiskFileWin32 dtor is INLINE in retail (vtable store + Close + ~CWideString + ~CBaseClassNonCopyable) -> shared header opt-in.
- CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get() inline (global read each time).
- Everything else is out of line: CWideString ctor/dtor/op=/op+/Replace/op const wchar_t*, CCharString ctor/dtor,
  CUserProfileManager::Get, GetValidProfileName, GetPhotojournalDir, IsRoomForFile, CDateAndTime ctor/ToFileTime,
  FormatDateTime, CPlayerGui Show/Hide/IsShowing (out-of-line forwarders at 0x4379E0..), CManager::GetInstance. No STLport.

## 6. Sibling landed rows (same TU, rebuild/src/compiled/00/55/)
- CTCPhotojournalCapture_GlobalUpdate_0055d4d0.cpp / _TakeQuickScreenshotNoHud_0055d490.cpp: no pragmas, no Flags line,
  #include "engine/CCountedPointer.h", local CTBaseSingletonNoDefaultAlloc<T> + CPlayerGui + 0x44 CTCPhotojournalCapture layout.
- CTCPhotojournalCapture_ClosePhotojournalCapture_0055d770.cpp: `// Flags: /GS`, esp-based frame, no size pragma,
  `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED`. DestroyScreen 0x55D6C0 also `// Flags: /GS`.
- CTCPhotojournalCapture_AddDescription_0055cfb0.cpp: #include "fable_string.h", local CDiskFileWin32 runtime model
  (enum FileAccessType local) -- predates the FABLE_RETAIL_CDISKFILEWIN32 opt-in.
- Same path idiom (other TU): rebuild/src/compiled/00/58/CTCInventory_HasScreenshots_00581b33.cpp (RELOCATION_MATCH):
  `CUserProfileManager& profiles = <Get>(); CWideString pattern = <Dir>() + GetValidProfileName(profiles.ProfileName) + s_backslash + s_jpgMask;`
  -- the singleton fetched into a REFERENCE local first is what makes retail call Get() before pushing the literals.
  (That row needed optimize("s") because its retail had a biased ebp frame; THIS row does not have one.)
  Pending twin with the same unresolved chain: 0x0055D810 OnCaptionEntered (hand304 NOTES).

## 7. Old draft model errors (vs disassembly)
- PATH EXPRESSION WRONG (main cause of 685v772 / frame 0x260 v 0x274): retail is
  `CUserProfileManager& profiles = CUserProfileManager::Get();`
  `CWideString path = NResourceDirectoryNames::GetPhotojournalDir() + GetValidProfileName(profiles.ProfileName) + L"\\" + name + L".jpg";`
  (pushes .jpg, &name, L"\\", then GetValidProfileName(edx=&mgr+8) into temp +0x24, GetPhotojournalDir into +0x20, then
  op+(W,W) ->+0x1C, op+(W,L"\\") ->+0x08, op+(W,W name) ->+0x04, op+(W,L".jpg") -> path +0x10; temps die +4,+8,+1C,+20,+24).
  Draft used `CProfileManager::Get()->SaveDir + L"\\" + name + L".jpg"` -- missing two calls and two temps.
- Front is right: `name = name + L" " + clock;` (op+(a,L" ")->+8, op+(tmp,b)->+4, op= into a, dtors +4,+8), and
  Replace(L"\\",L"-"), Replace(L"/",L"-"), Replace(L":",L".") with implicit CWideString temporaries (the "with" temp built first).
- `clock` (b) lives to function end (destroyed after path, before name): keep it a function-scope local.
- Early-exit branch: IsShowingScreenshotMessage -> HideScreenshotMessage; TakeScreenshotAfterFrames = 4; return.
- Sound: `NUISystem::CManager::GetInstance()->PlaySample(CCharString("SND_MM_SNAPSHOT"), 1)` -- a named CCharString
  local (non-const ref param; temp at +0x18, explicit -1 pushed by default arg), slot 41 PlaySample not PlaySound.
- Inside the if: `CDiskFileWin32 file(path, FILE_ACCESS_CREATE, 2); file.Close();` then D3DXSaveSurfaceToFileW(path,
  1, GPDisplayManager->BackBufferSurface.PD3DSurface, 0, 0) (surface loaded BEFORE pushing the 0,0), ShowScreenshotMessage,
  PlaySample; file destroyed at block end (inline dtor) -- draft's local CDiskFileWin32 had out-of-line virtual dtor.
- Names to fix: IsScreenshotMessageShowing->IsShowingScreenshotMessage, State->TakeScreenshotAfterFrames, CProfileManager->
  CUserProfileManager (reference return, ProfileName), CFrontEnd->NUISystem::CManager, PlaySound->PlaySample, g_pDisplay->
  GPDisplayManager, IsRoomForFile -> CAFile::IsRoomForFile, FormatDateTime (PDB CXBConfig::FormatDateTime; landed spells it global).

## 0x00A8F9E0

# DOSSIER 0x00A8F9E0 -- C3DPrimitive2::UploadToIndexBuffer(unsigned long)

## 1. Retail shape (xdis 00a8f9e0 0x315; oracle.tsv 773 bytes)
- __thiscall, `ret 4` (one 4-byte arg), returns bool (`mov al,1`). No frame pointer (`sub esp,1Ch ; push ebx/ebp/esi/edi`), no SEH,
  no /Os idioms (alignment fillers `lea ecx,[ecx]` / `lea esp,[esp]` before loops = /O2 speed). `this` spilled to [esp+18h].
- Old draft header says 784 bytes; real length is 773 (0x305 + int3 padding; next fn 0x00A8FCF0 GetUsedBones).
- Argument is tested as `test byte ptr [esp+30h],dl(=1)` -> `flags & 1`, NOT a bool. Later the arg slot [esp+30h] is reused
  as the animated-block byte offset (i*0x30).

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 00bd2140) and FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv 032fb7e0):
  `?UploadToIndexBuffer@C3DPrimitive2@@AAE_NK@Z` = private: bool __thiscall C3DPrimitive2::UploadToIndexBuffer(unsigned long).
- Callers (xref): 0x00A932E1 UploadRepeatedBuffers / 0x00A93341 UploadStaticBuffers (landed, both declare
  `bool UploadToIndexBuffer(unsigned long flags)` and pass the init flags), 0x00A9287F UploadAnimatedBuffers, 0x00A8FEEC.
- Manifest label is RIGHT (class+method); the draft's `bool dynamic` parameter type is WRONG (must be unsigned long, private).

## 3. Callees
| retail | landed row | landed name | PDB name | note |
|---|---|---|---|---|
| 0x00BFEA1A | 00/bf/global_operatornew_00bfea1a.cpp | operator new | operator new(size 0x14) | `new CIndexBuffer` x2 |
| 0x00AC1EC0 | 00/ac/CIndexBuffer_CIndexBuffer_00ac1ec0.cpp | CIndexBuffer::CIndexBuffer() | ??0CIndexBuffer@@QAE@XZ | ok |
| 0x00AC1F70 | 00/ac/CIndexBuffer_Initialise_00ac1f70.cpp | CIndexBuffer::Initialise | ?Initialise@CIndexBuffer@@QAEXKKW4EIndexBufferFormat@@W4EIndexBufferPool@@W4EPrimitiveType@@@Z (returns VOID) | draft declares `bool Initialise(..long,long,long)` -- wrong: void, enum params |
| 0x00AC1FB0 | 00/ac/CIndexBuffer_Lock_00ac1fb0.cpp | CIndexBuffer::Lock | ?Lock@CIndexBuffer@@QAEPAEKKK@Z (returns unsigned char*) | draft returns void* -- PDB: unsigned char* |
| 0x00AC1F00 | 00/ac/CVertexBufferWin32_DoUnlock_00ac1f00.cpp | "CVertexBufferWin32::DoUnlock" (propagated, wrong) | ?Unlock@CIndexBuffer@@QAEXXZ | call it CIndexBuffer::Unlock (Q16 draft 00a90260 also does). Possible NAME_FIXES item for the landed row |
| vtbl slot 0 | 0x00AC2070 | ~CIndexBuffer scalar deleting dtor (push 1) | virtual ~CIndexBuffer() | reached via boost::scoped_ptr::reset/checked_delete |
- Initialise args (push order 4, pool, 0x65, usage, length): Initialise(length=count*reps*2, usage 8|0x18, (EIndexBufferFormat)0x65,
  (EIndexBufferPool)1|2, (EPrimitiveType)4). flags&1 -> usage 0x18, pool 2. Lock(0,0,0). Exactly the same idiom as the Q16 draft
  0x00A90260 (rebuild/candidates/manual/pending/2026-11-11_linux_handQ16/differ/00a90260.cpp) which uses InitFlags&1.
- PDB has `?InitIndexBuffer@C3DPrimitive2@@AAE_NAAVCIndexBuffer@@KK@Z` (Ego_r 00bd0f20, FableWin 032fc7d0, 79 debug bytes): no
  retail out-of-line copy is called -> inlined here. Likely body: `bool InitIndexBuffer(CIndexBuffer& b, unsigned long length,
  unsigned long flags){ usage=8,pool=1; if(flags&1){usage=0x18;pool=2;} b.Initialise(length,usage,IBF16,pool,PT 4); return true; }`
  (return value unchecked by retail). Defined earlier in the TU (Ego_r 00bd0f20 < 00bd2140) => visible.
- Status write `IndexBufferStatus |= 1` = FableWin inline `?SetIndexBufferStatus@C3DPrimitive2@@AAEXW4EBufferStatus@1@@Z`.

## 4. Members (retail = Ego_r minus 4 after the 12-byte CArray ClothPrimitives; struct_layouts_egor.tsv)
C3DPrimitive2 (rebuild/include/engine/C3DPrimitive2.h):
- +0x08 long RepeatingMeshReps (header names it). `reps>1?reps:1` = FableWin inline `GetRepeatingMeshBatchSize()` (no header accessor).
- +0x2C scoped_ptr<N3DPrimitive::CDeviceIndependent> Data -- header has only `unsigned char Data[4]`; NO opt-in macro types it.
  Landed siblings (ConvertToVBFormatRepeatingMesh 0x00A8F770, GetUsedBones 0x00A8FCF0) model C3DPrimitive2 + CDeviceIndependent locally.
- +0x34 boost::scoped_ptr<CIndexBuffer> IndexBuffer -- `FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER` (needs FABLE_RETAIL_SCOPED_PTR,
  FABLE_RETAIL_SCOPED_PTR_RESET, FABLE_RETAIL_CINDEXBUFFER). reset = swap + checked_delete (retail: read old, store new, null test, vcall slot0(1)) -- matches.
- +0x3C ulong IndexBufferStatus (named), +0x44 CStaticBlock* StaticBlocks (named, stride 0x20), +0x48 ulong AnimatedBlockCount,
  +0x4C CAnimatedBlock* AnimatedBlocks (typed only with `FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS`, stride 0x30, VertexCount +0x10,
  accessor GetVertexCount()).
- Block head N3DPrimitive::CBaseBlock (engine/N3DPrimitive_CStaticBlock.h): PrimitiveCount +0, StartIndex +4, bool IsStrip +8;
  `FABLE_RETAIL_CBASEBLOCK_ACCESSORS` offers getters only (no setters needed; fields public in default branch).
  FableWin inline `?GetBaseBlock@C3DPrimitive2@@QAEPAVCBaseBlock@N3DPrimitive@@K@Z` = AnimatedBlockCount ? &AnimatedBlocks[i] : &StaticBlocks[i]
  (retail tests [this+48h] per iteration and keeps two running byte offsets i*0x30 ([esp+30h]) and i*0x20 ([esp+14h])).
- N3DPrimitive::CDeviceIndependent (no shared header): vptr +0 (CBaseClassNonCopyable), CArray<C3DVertex2> Vertices +4 (0x20 elems),
  CArray<CIVCountedPointer<CTriangleList>> TriangleLists +0x10 (element = 4-byte pointer, Object at +0).
- N3DPrimitive::CTriangleList (no shared header; CIVCountedPointeeBase: vptr+refcount): CArray<C3DTriangle2> Triangles +8 (6-byte),
  CArray<unsigned short> StripIndices +0x14.
- FableWin inline accessors to use (all exist as names): CDeviceIndependent::GetVertexCount, GetTriangleListCount, PeekTriangleList(K)
  (returns const CTriangleList&), GetTotalIndexCount, GetTotalTriangleCount; CTriangleList::GetStripSize, GetTriangleCount,
  PeekStrip() (const unsigned short*), PeekTriangleList() (const C3DTriangle2*); CArray<G>/CArray<C3DTriangle2>::GetAsCArray const.
  Shared engine/CArray.h already has `const T* GetAsCArray() const { size()>0 ? _M_start : (const T*)this; }` = retail's
  `ja keep start / else edi=&StripIndices` select. Use it instead of the draft's local GetData.
- Classes the draft models LOCALLY that have shared headers: CArray (engine/CArray.h), CIndexBuffer (engine/CIndexBuffer.h),
  scoped_ptr (boost/scoped_ptr.h), C3DPrimitive2 (engine/C3DPrimitive2.h), CStaticBlock/CBaseBlock (N3DPrimitive_CStaticBlock.h),
  CAnimatedBlock (C3DPrimitive2.h ANIMATEDBLOCKS), C3DVertex2 / C3DTriangle2 (engine/C3DVertex2.h, engine/C3DTriangle2.h),
  CIVCountedPointer (engine/CIVCountedPointer.h). Blocker: header has no Data typing -> either add an opt-in (e.g.
  FABLE_RETAIL_C3DPRIMITIVE2_DATA) or keep local like the landed siblings.

## 5. Callee visibility
- Inlined in retail (visible): InitIndexBuffer, SetIndexBufferStatus, GetRepeatingMeshBatchSize, GetBaseBlock, all CDeviceIndependent /
  CTriangleList / CArray accessors above, scoped_ptr::reset + checked_delete, ~CIndexBuffer not inlined (virtual call).
- Out of line: CIndexBuffer ctor/Initialise/Lock/Unlock (other TU, 0x00AC1xxx), operator new.
- No STLport out-of-line helpers involved.

## 6. Siblings (same TU, retail neighbourhood 0x00A8E5C0..0x00A93380)
- 00/a8/C3DPrimitive2_ConvertToVBFormatRepeatingMesh_00a8f770.cpp (immediately before): `// Flags:` empty, no pragmas, local
  boost::scoped_ptr + local N3DPrimitive::CDeviceIndependent with GetVertexCount()/PeekVertex inline.
- 00/a8/C3DPrimitive2_GetUsedBones_00a8fcf0.cpp (immediately after): no pragmas, local model.
- 00/a8/C3DPrimitive2_SetupVertexBuffer_00a8eda0.cpp: same flags&1 -> (24/pool 2 vs 8/pool 1) idiom, reset(new ...) then Initialise.
- Q16 pending 00a90260 CreateDegenerateTriangleIndexBuffer: macros FABLE_RETAIL_SCOPED_PTR, _SCOPED_PTR_RESET, FABLE_RETAIL_CINDEXBUFFER,
  FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER (+CARRAY_SIZE_CTOR_OUTOFLINE); enum casts (EIndexBufferFormat)101 etc.
- 00/a9/C3DPrimitive2_ConvertToVBFormatAnimated_00a91180.cpp uses `#pragma auto_inline(off/on)`. No optimize pragmas in the TU.

## 7. Old draft model errors vs disassembly
- Parameter `bool dynamic` -> must be `unsigned long flags` tested `& 1` (mangled K; retail `test byte,1`).
- Initialise declared bool / long params; PDB: void with enums. Lock returns unsigned char*. Unlock name.
- First reset + empty loop: retail emits `reset(new CIndexBuffer)` then a counted loop over TriangleLists with NO body (0xA8FA22..3B),
  loop bound computed once. Likely an inlined accessor whose result is dead (e.g. GetTotalTriangleCount() into an unused/ASSERT local)
  or InitIndexBuffer path; draft's literal empty `for` may not produce identical bound handling -- check.
- Second count loop = inlined GetTotalIndexCount(): strip>0 ? GetStripSize() : GetTriangleCount()*3 (retail `add ecx,eax ; lea ecx,[ecx+eax*2]`).
  Vertex count (Vertices.size()) is computed BEFORE the index count loop and kept in [esp+28h] (vertsPerRep).
- length = count*reps*2 computed before the 2nd `new` (ebx); reps kept in edi / [esp+10h], outer loop is a down-counter
  ([esp+24h] dec / jne) with repBase [esp+20h] += vertsPerRep -- a `for(rep=0;rep<reps;++rep)` was converted; fine but note.
- Strip path: block->IsStrip=1 written BEFORE PrimitiveCount = size-2 (retail order: start index, PeekStrip select, IsStrip, count);
  triangle path: PeekTriangleList select stored to spill [esp+1Ch], IsStrip=0 after count calc then PrimitiveCount = n, unrolled 3 stores,
  `dec edi ; jne` down-count loop.
- Animated offset: `if (AnimatedBlockCount) offset += AnimatedBlocks[i].VertexCount` reads this->AnimatedBlocks fresh (this reloaded from
  [esp+18h]) -- draft matches semantically. Residue noted in hand277 NOTES: register assignment (indices ebx, block esi, this spilled).

## 0x00B206B0

# DOSSIER 0x00B206B0 -- NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::OffsetSplinePositions (773 bytes)

## 1. Retail shape (xdis.py 00b206b0 0x315)
- __thiscall, plain `ret` (0xB209B4), no args. `sub esp,30h; push ebp; mov ebp,ecx` -> ebp is `this`, NOT a frame
  pointer (default /Oy). No SEH frame. Default /O2 (loop-top `nop` alignment at 0xB206DF). No pragma needed.
- Count = (ThisSpline.end-begin)/12 SIGNED (imul 0x2AAAAAAB, sar, shr 31) computed ONCE before the loop, `jle` exit
  -> `long count = ThisSpline.GetControlPointCount();` then `for (long i=0;i<count;++i)`; counter in [esp+18h] `dec`,
  index ebx += 12 strength-reduced. push ebx/esi/edi only after the count>0 test.
- Body guard: Limits[i] loaded Z,Y,X (fld [+8],[+4],[+0]) summed X*X+Y*Y+Z*Z, compared `> DELTA*DELTA`
  (0x129BA3C loaded+fmul'd at run time; `fcompp; test ah,5; jp skip`). This is C3DVector::IsNonZero.
- Then: eax=[this+4] (ThisSpline begin) loaded BEFORE edx=[this+0] (InitialSpline) / esi=&Initial[i];
  offset = This[i]-Initial[i] spilled to [esp+28h..30h]; edi=&Speed[i] (this+0x3C).
- Per axis: `fld 0.0(0x122DEDC); fld speed.k; fucompp; test ah,44h; jnp zero` -> `speed.k != 0.0f ? GFFloatRandom(speed.k*2 /* fadd st,st */, &seed) - speed.k : 0.0f`;
  ecx=&seed 0x013999C4, float pushed (__fastcall). Results to [esp+1Ch..24h]. ecx(&limit) re-read from spill [esp+14h] after each call.
- Sum: fld offset.X; fadd change.X (kept on x87 stack); Y,Z = offset.k + change.k stored back into CHANGE's slots
  [20h],[24h] -> operand order offset+change (draft's `change += offset` is reversed: change first).
- Limit squares: lx2 kept in st (fld X; fld st0; fmulp), ly2 -> [esp+10h], lz2 -> [esp+14h] (fld; fld st0; fmul st1; fstp; fstp st0).
- Branches: lx2<1e-4 (0x129F0BC, a SEPARATE constant, not DELTA) -> X=0 (constant fld 0.0 kept as moved.X),
  d = Y*Y/ly2 + Z*Z/lz2 (fld m; fmul m; fdiv l2  == (m*m)/l2); if (d > 1.0f) {s = 1.0f/d (fdivr 1.0); Y*=s; Z*=s}.
  else ly2<1e-4 -> Y=0, d = X*X/lx2 + Z*Z/lz2; else lz2<1e-4 -> Z=0, d = X*X/lx2 + Y*Y/ly2; else 3-term d, scale all.
  Shared tail 0xB20950 (Y*=s; Z*=s) and 0xB20964 pop.
- Store: fld moved.X; fadd [esi] (moved + initial, moved first), reload [this+4]+ebx, build temp at [esp+34h..3Ch], copy as 3 dwords
  -> `ThisSpline.SetControlPoint(i, moved + InitialSpline->GetControlPoint(i))` (or the [] equivalent).
- Constants: 0x129BA3C = DELTA_NOTIONAL_ZERO (1e-4f, ?DELTA_NOTIONAL_ZERO@@3MB, C3DVector.h line 26); 0x129F0BC = 1e-4f (another pool
  float); 0x122DEDC = 0.0f; 0x122DED8 = 1.0f.

## 2. Identity (two sources) -- manifest label CORRECT
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 00ae1990) and FableWin.pdb (_fablewin_api.tsv 018528cf/02f5d130; FableWin.exe.signatures.tsv):
  `?OffsetSplinePositions@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXXZ` = public, non-const, void(void), __thiscall.
- Ego_r TU order matches retail: WriteBinary (Ego 0xAE1920 / retail 0xB20630) -> OffsetSplinePositions (0xAE1990 / 0xB206B0) -> Initialise (0xAE2000).
- Callers (xref.py): 0xB20B00 (`call 0xB206B0; mov al,1; ret` = CSplineRandomOffsetData::Initialise ?Initialise@...@@QAE_NXZ, UNLANDED),
  CSplineRandomOffsetData::Update 0xB21160 (tail `jmp` at 0xB211A8, landed), CPSCSplineBase::Initialise 0xB215A0 (call at 0xB215B0, landed).

## 3. Callees
- 0x004978A0 GFFloatRandom: landed rebuild/src/compiled/00/49/global_GFFloatRandom_004978a0.cpp; PDB `?GFFloatRandom@@YIMMPAK@Z`
  (__fastcall float(float, unsigned long*)) -- Ego_r 004a1e20 + FableWin. Shared decl: rebuild/include/engine/GFFloatRandom.h
  (`float FABLE_FASTCALL GFFloatRandom(float,unsigned long*);`). Draft spelling agrees.
- Seed 0x013999C4: initialised .data, value 0x2A (42); the ONLY three references in Fable.exe are this function (0xB20767/7A0/7DA,
  full-image scan). So it is a TU-local static of this source file. It is NOT NParticleEngine::GetRandomSeed (that returns 0x013D2E10,
  retail 0x00AEAAD0). No PDB name found (Ego_r/FableWin grep "seed") -> descriptive name, mark in NOTES (draft's g_SplineRandomSeed ok;
  `static unsigned long X = 42;` defined in the row also gives the same imm32 reloc).
- No vtable calls. Inline (no out-of-line copy in retail, FableWin-only names): CCardinalSpline::GetControlPointCount ?GetControlPointCount@CCardinalSpline@@QBEJXZ (long),
  GetControlPoint ?GetControlPoint@CCardinalSpline@@QBEABVC3DVector@@J@Z (const C3DVector& (long) const),
  SetControlPoint ?SetControlPoint@CCardinalSpline@@QAEXJABVC3DVector@@@Z (void(long,const C3DVector&)), C3DVector::IsNonZero ?IsNonZero@C3DVector@@QBE_NXZ.

## 4. Members (retail offsets; struct_layouts_egor.tsv uses the 16-byte debug CArray: 0/4/28/52/53/56/72)
- this+0x00 CCardinalSpline* InitialSpline; +0x04 CCardinalSpline ThisSpline; +0x18 PreviousSpline (unused);
  +0x2C/+0x2D bool OffsetOnlyAtStartup/ResetEachFrame (unused); +0x30 CArray<C3DVector> SplinePosOffsetLimits; +0x3C SplinePosOffsetSpeed.
- CSplineRandomOffsetData / CPSCSplineBase: NO shared header (landed siblings model it locally, e.g. InterpolateTo 0xB211B0).
- CCardinalSpline: shared rebuild/include/engine/CCardinalSpline.h (members PRIVATE: ControlPoints +0, Tension +0xC, EndPointsConnected +0x10;
  accessors GetTension/SetTension/GetControlPointCount/GetControlPoints(by value)/SetControlPoints). It does NOT yet have
  GetControlPoint(long)/SetControlPoint(long,const C3DVector&) -- an author must add them (opt-in macro, e.g. FABLE_RETAIL_CCARDINALSPLINE_CONTROLPOINT,
  bodies `return ControlPoints[index];` / `ControlPoints[index]=p;`), or the draft cannot touch ControlPoints.
- CArray: shared engine/CArray.h (STLport vector; FABLE_RETAIL_CARRAY routes operator[] through begin()).
- C3DVector: shared engine/C3DVector.h. Opt-ins relevant: FABLE_RETAIL_C3DVECTOR (declares DELTA_NOTIONAL_ZERO, enables IsNonZero etc.);
  IsNonZero variants: default `squared=X*X; squared+=Z*Z; squared+=Y*Y`, FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ `X*X+Y*Y+Z*Z>D*D`
  (one-line; header notes the one-line sum loads Z,Y,X -- matches retail here), _ISNONZERO_ZYX(_PAREN/_SQUARED); operator+/-
  (_ADD_XY_GROUP/_SUBTRACT_XY_GROUP grouping variants), operator+= (line 266).
- Draft models LOCALLY (all have shared headers): C3DVector, CArray, CCardinalSpline. Also local GFFloatRandom decl (header exists).

## 5. Callee-body visibility
- All accessors (GetControlPoint/SetControlPoint/GetControlPointCount/IsNonZero/C3DVector ops) are expanded inline in retail.
- GFFloatRandom out of line (other TU, 0x004978A0). No STLport helper calls in this body.
- Same TU, earlier: WriteBinary 0xB20630 (landed). Later: GetApproximateSplineLength 0xB209C0 (landed, mislabelled file
  NParticleEngineCPSCSingleSprite_vfunc31_00b209c0.cpp), Initialise 0xB20B00 (unlanded).

## 6. Sibling landed rows (rebuild/src/compiled/00/b2/)
- ..._CSplineRandomOffsetData_InterpolateTo_00b211b0.cpp: shared engine/CCardinalSpline.h with FABLE_RETAIL_STLP_VECTOR_C,
  FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE, FABLE_RETAIL_STLP_VECTOR, FABLE_RETAIL_CARRAY; local nested CPSCSplineBase::CSplineRandomOffsetData; no pragmas.  <- best idiom to copy
- ..._WriteBinary_00b20630.cpp: engine/C3DVector.h + engine/CArray.h, local CCardinalSpline (public members), no macros.
- ..._Update_00b21160.cpp, ..._GetPointAtPos_00b20fd0.cpp, ..._GetPointAtIndex_00b21090.cpp: local CArray/CCardinalSpline stand-ins, no pragmas/flags.
- No `// Flags:` lines in any spline sibling: default flags.

## 7. Old draft's model errors (vs disassembly)
1. Local C3DVector/CArray/CCardinalSpline stand-ins; public ControlPoints. Use shared headers + inline GetControlPoint/SetControlPoint/GetControlPointCount.
2. Guard spelled as a local GetLengthSquared running sum (X,Y,Z statements) vs `g_epsilon*g_epsilon`: retail is IsNonZero with DELTA_NOTIONAL_ZERO,
   loads Z,Y,X then sums X*X+Y*Y+Z*Z -> try FABLE_RETAIL_C3DVECTOR + FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ (`if (limit.IsNonZero())`).
3. Draft binds initialPoint (InitialSpline) before reading ThisSpline; retail loads ThisSpline begin first: write
   `C3DVector offset = ThisSpline.GetControlPoint(i) - InitialSpline->GetControlPoint(i);` (InitialSpline's &[i] stays in esi for the final add).
4. `change += offset` (change first) vs retail offset.k + change.k (offset first): `offset + change` / `offset += change`-style; result lands in change's slots.
5. Ellipsoid terms: draft `m / l2 * m`; retail `(m*m)/l2` -> write `m*m/l2` (NOTES' own "next").
6. ly2/lz2 slot swap: retail puts ly2 at the lower slot [esp+10h], lz2 at [esp+14h]; lx2 never spilled. Declaration order of the three squares
   / whether they are const locals vs GFSqr(limit.k) may drive this.
7. 0x129F0BC is a distinct 1e-4f from DELTA_NOTIONAL_ZERO (0x129BA3C); keep it a literal 0.0001f (draft correct there).

## 0x007D46D0

# DOSSIER 0x007D46D0 CTCTavernGameCoinGolf::FadeOutTableOccupants (780 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 007d46d0 0x31c)
- thiscall, `ret 4` (one bool arg at [base+4]); NO frame pointer (`sub esp,0x28 ; push ebp ; mov ebp,ecx` -- ebp = this, so /Oy on);
  NO SEH prologue even though a local std::list with a destructor lives across calls (whole TU has no EH frames: siblings
  0x007D3700 GetAdditionalInfo / 0x007D4A90 LeaveGame likewise none). Callee-saved ebx/esi/edi pushed only AFTER the early-out.
- Frame 0x28: camPos (GetCamera arg1) at base-0x18..-0x10, lookAt (arg2, otherwise dead) at base-0xc..-0x4; lookAt slot is reused for
  the coin pos X/Y copy ([esp+0x2c]/[esp+0x30]). List head pointer slot base-0x1c is reused as the 0x5B key temp (creature loop)
  and as coin index `i` (coin loop). Empty filter object at base-0x25 (1 byte). Floats/alpha bytes share base-0x20/-0x24 (swapped
  between the two loops: creature dist -0x24 alpha -0x20; coin top/dist -0x20 alpha -0x24, key -0x20).
- Order: GetCamera(camPos, lookAt) FIRST, then `if (!ParentThing->HasTC(184)) return;` (test [ParentThing+0x34],0x1000000 =
  UsingComponents word 5 bit 24 = TCI_TAVERN_TABLE (ETCInterfaceType.h:189)). Draft's "Flags34 in-view" is this HasTC.
- list ctor inline: malloc(0xC), node->next=node->prev=node (header std::list default ctor). list dtor inline at the end: free loop,
  `mov [edi],edi ; push edi ; mov [edi+4],edi ; call free` (_List_base::clear + deallocate, default header bodies).
- Creature loop: `for it in list` node in ebx, data esi=[ebx+8]; IS_THING_ALIVE (null test + byte [+0x91]&1); GetPos; 2-D dist
  sqrt(dx*dx+dy*dy) (no Z); alpha: `restore` byte tested; `if (restore || 0.6f < dist) 0xFF; else if (dist < 0.4f) 0; else
  ftol((dist-0.4f)*1274.9998779f)`. Compare shapes: `fld 0.6 ; fcomp dist ; test ah,5 ; jnp ->0xff` and `fld dist ; fcomp 0.4 ;
  test ah,5 ; jp ->compute`. Then HasTC(91)+GetTC->SetAlpha(alpha) (test [esi+0x28],0x8000000; add esi,0x44; key 0x5B stored;
  LowerBound 0x40F020; `mov esi,[esi+4]` end; cmp it,end / cmp [eax],0x5b ; jle -> header GetTC<T> conditional-expression form).
- Coin loop: `for (unsigned i = 0; i < Coins.size(); ++i)` (size via /0x3C magic 0x88888889, sar 5; recomputed at loop bottom; jb
  unsigned; je to skip loop when size==0 at top). Strength-reduced byte offset ebx += 0x3C, `lea ecx,[ebx+start+0x25]` ->
  Coins[i].Thing.GetPItem(); thing alive test; `start+ebx != Held` ([this+0x108]) test AFTER alive test (reloads Coins._M_start
  each time). GetPos copied: X,Y to frame, top = pos.Z + 0.4f stored to slot; height = *min(top, camPos.Z) by reference
  (fld camZ ; fcomp top ; jnp -> &camZ else &top == STLport `b<a?b:a` with a=top b=camZ); dist = sqrt 3-D of (X-camX, Y-camY,
  height-camZ). Alpha: 0.3f (0x01238170) / 0.2f (0x012305A0) / 2549.9997559f (0x0126F490). Same SetAlpha tail.
- Constants: 0.6f 0x0123816C, 0.4f 0x0125A318 (used both as creature near and coin +Z offset), 1274.99988f 0x0126F494,
  0.3f 0x01238170, 0.2f 0x012305A0, 2549.99976f 0x0126F490 (bytes read from Fable.exe).

## 2. Name / signature (two sources)
- Ego_r (ghidra_out/egor_pdb_names.tsv, egor_signatures.tsv) AND FableWin (_fablewin_api.tsv 0x01824123, naming_stragglers):
  `?FadeOutTableOccupants@CTCTavernGameCoinGolf@@AAEX_N@Z` = private: void __thiscall FadeOutTableOccupants(bool). Manifest label is
  RIGHT. Draft leaf line is truncated: use the full mangled leaf above. Not virtual (no vtable slot).
- Callers (xref): 0x007D4A78 DrawPreparePrimitivesForRendering (passes false = fade), 0x007D4A9D LeaveGame (passes true = restore).
  Param name unproven (landed callers spell `fadeOut`, draft `restore`; semantics: true -> all 0xFF = restore).

## 3. Callees
| addr | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x007D3560 | 00/7d/CTCTavernGameCoinGolf_GetCamera_007d3560.cpp | GetCamera(C3DVector& camPos, C3DVector& lookAt) | ?GetCamera@CTCTavernGameCoinGolf@@QAEXAAVC3DVector@@0@Z | WRONG: draft `C3DVector GetCamera(float*)`; real is void, two C3DVector& out params |
| 0x00BFEA0E | 00/bf/global_malloc_00bfea0e.cpp | malloc (list node, STLport allocator) | CRT | draft malloc ok (but should come from header std::list) |
| 0x00686D90 | (bound in CTCBase.h:162) | CTCBase::PeekThingSearchTools() const | ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ | WRONG: draft `GetThingManager()`. (labels_callee_canon says GetThingSearchTools -- stale; CTCBase.h + chatgpt_pass10 say Peek, 0x00686D80 is Get) |
| 0x007D4CE0 | 00/7d/CThingSearchTools_GetAllThings_007d4ce0.cpp | CThingSearchTools::GetAllThings<CThingCreatureBase, std::list<CThingCreatureBase*>, CThingFilter_IsHuman>(const CThingFilter_IsHuman&, list&) const | ??$GetAllThings@VCThingCreatureBase@@V?$list@PAVCThingCreatureBase@@...@VCThingFilter_IsHuman@@@CThingSearchTools@@QBEX... | WRONG: draft class CWorldThingManager, arg ORDER reversed (list, filter), list<CThingAICreature*>, CThingFilter_None. Real: filter first, list second; element CThingCreatureBase*, filter CThingFilter_IsHuman |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const -> const C3DVector& | ?GetPos@CThing@@QBEABVC3DVector@@XZ | WRONG spelling: draft GetPosition |
| 0x00BFEA70 | 00/bf/global_ftol2_00bfea70.cpp | _ftol2 (float->uchar cast) | CRT | ok |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound | same | draft local CInterfaceMap model -- use CThing.h Components |
| 0x004BFAC0 | 00/4b/CTCGraphicAppearance_SetAlpha_004bfac0.cpp | NGraphicAppearance::CTCGraphicAppearance::SetAlpha(unsigned char) | ?SetAlpha@CTCGraphicAppearance@NGraphicAppearance@@QAEXE@Z | draft omits namespace NGraphicAppearance |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const -> CBaseObject* | same (both PDBs) | WRONG: draft returns CThing*; reach it via CIntelligentPointer<CThing>::Get() |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free | CRT | ok |

## 4. Members
- this (CTCTavernGameCoinGolf, no shared header; CTCTavernGame.h exists (FABLE_RETAIL_CTCTAVERNGAME, CTCBase-derived), no
  CTCTavernGameCoinBase/CoinGolf header):
  +0x04 CTCBase::ParentThing (CThing*) ; +0xFC/+0x100 CTCTavernGameCoinBase::Coins (std::vector<Coin>; Ego_r +0x114, retail 0x18
  smaller like GetCamera row notes) ; +0x108 CTCTavernGameCoinBase::Held (Coin*; Ego_r +0x124). Draft names CoinsStart/CoinsFinish/
  Held -> real `Coins` vector + `Held`. The GetCamera landed row calls +0x108 `PCoin` (descriptive) -- PDB name is Held.
- Coin (struct_layouts_egor.tsv, 60 bytes): Owned bool +0x24, Thing CIntelligentPointer<CThing> +0x25 (odd, pack(1) as in the
  GetCamera row's CCoinGolfCoin), Radius float +0x30, Owner int +0x34, Sound ulong +0x38.
- CThing (engine/CThing.h): UsingComponents bitset<274> +0x20 (words +0x28 = bit 91, +0x34 = bit 184); Components CVectorMap +0x44;
  Dead bit +0x91 bit0 (IsDead()). Opt-ins: FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (GetTC<T>/HasTC forceinline conditional-expr form,
  chunk Q23) or FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT; GetPos() declared at CThing.h:411.
- ETCInterfaceType.h: TCI_GRAPHIC_APPEARANCE_NEW = 91, TCI_TAVERN_TABLE = 184.
- NGraphicAppearance_CTCGraphicAppearance.h: FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETALPHA declares SetAlpha.
- CIntelligentPointer.h: FABLE_RETAIL_CINTELLIGENTPOINTER (Get() = static_cast<T*>(GetPItem())).
- ThingAlive.h: FABLE_RETAIL_IS_THING_ALIVE (template IS_THING_ALIVE(const T&): `thing!=0 && !thing->IsDead()`).
- C3DVector.h (shared) -- several copy-ctor variants; GetCamera row uses a local C3DVector with user copy ctor.
- Draft LOCAL models that have shared headers: C3DVector, CThing, CThingAICreature, CTCGraphicAppearance, CBaseIntelligentPointer,
  CInterfaceMap (=CVectorMap.h), CThingFilter_None (wrong filter anyway; CThingFilter_IsHuman), STLport list (CList/SNode ->
  stlport/fable_containers.h std::list), CThingSearchTools (engine/CThingSearchTools.h), CTCBase (engine/CTCBase.h).

## 5. Callee visibility
- GetAllThings<...IsHuman> 0x007D4CE0 sits right AFTER this function (0x007D46D0+0x30C .. 0x007D49E0 ctor .. 0x007D4CE0) -> same TU
  template instance emitted out of line; CThingSearchTools.h offers FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY (same-TU
  __declspec(noinline) visible body, chunk 768) -- worth trying (lets c2 know the list is not captured; may affect slot reuse).
  Note that body omits the IsThingOfType mask test that the landed 0x007D4CE0 row has.
- GetCamera 0x007D3560 is in the same TU, defined earlier (same class file); a visible noinline body could change slot reuse of lookAt.
- std::list ctor/dtor/clear: retail fully INLINED (no out-of-line COMDAT call) -> default header path, do NOT define
  FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE / _CLEAR_OUTOFLINE / _BASE_OUTOFLINE.
- GetPos, GetPItem, LowerBound, SetAlpha, PeekThingSearchTools: out of line calls (declarations suffice).

## 6. Siblings (rebuild/src/compiled/00/7d/)
- CTCTavernGameCoinGolf_GetCamera_007d3560.cpp: FABLE_RETAIL_CINTELLIGENTPOINTER, pack(1) CCoinGolfCoin with Thing at +0x25,
  local IS_THING_ALIVE(const CIntelligentPointer<CThing>&), local C3DVector with user copy ctor; no Flags line.
- CTCTavernGameCoinGolf_UpdateValues_007d4410.cpp: fable_compiler.h, engine/CVectorMap.h, local CThing with forceinline GetInterface.
- CTCTavernGameCoinGolf_LeaveGame_007d4a90.cpp / _DrawPreparePrimitivesForRendering_007d4a60.cpp (latter includes engine/CTCTavernGame.h
  via `#define CTCTavernGame CTCTavernGameDonor`).
- CTCTavernGameCoinGolf_GetAdditionalInfo_007d3700.cpp / _vfunc32_007d3510.cpp: `#pragma auto_inline(off/on)`.
- CThingSearchTools_GetAllThings_007d4ce0.cpp: local STLport list model with iterator user copy ctor.
- None of the CoinGolf rows carries a `// Flags:` line (default flags).

## 7. Old-draft model errors
1. GetCamera signature wrong (returns C3DVector + float*) -> real void GetCamera(C3DVector& camPos, C3DVector& lookAt); explains the
   "camera frame layout" residue (retail: two 12-byte locals, lookAt dead).
2. GetAllThings: wrong class (CWorldThingManager), wrong getter (GetThingManager -> PeekThingSearchTools, const), reversed argument
   order (retail pushes list first = LAST arg, filter second-pushed = FIRST arg), wrong element type and filter type.
3. List: hand-rolled CList with malloc -- replace by header std::list<CThingCreatureBase*> (ctor/dtor bodies inlined as retail).
4. ParentThing test spelled as a fake Flags34 word -> ParentThing->HasTC(TCI_TAVERN_TABLE).
5. Coin vector modelled as raw Coin* start/finish -> std::vector<Coin> Coins; size() + operator[] (unsigned index, size recomputed
   each iteration); `&Coins[i] != Held`.
6. GetPItem returns CThing* in draft; use Coins[i].Thing.Get() (CIntelligentPointer<CThing>), alive test IS_THING_ALIVE/IsDead.
7. Creature comparison written `dist > 0.6f`; retail operand order is `0.6f < dist` (fld const; fcomp dist) -- check emitted form.
8. Local GetTC/HasTC models -> CThing.h GetTC<NGraphicAppearance::CTCGraphicAppearance>(TCI_GRAPHIC_APPEARANCE_NEW,&tc) under
   FABLE_RETAIL_CTHING_HASTC_FORCEINLINE.

## 0x0094A3B0

# DOSSIER 0x0094A3B0  CAIStateGroup_GoToPub::ChooseAction(bool)   (785 bytes incl. tail; code ends 0x0094A6C0)

## 1. Retail shape (xdis 0094a3b0 0x321)
- __thiscall, `ret 4` (one 4-byte arg: bool canDrink at [esp+0x1C] after prologue). No frame pointer (/Oy on), no SEH,
  no /GS. Prologue `sub esp,8 ; push ebx ; push ebp ; push esi ; push edi ; mov edi,ecx`. Locals = 8 bytes only.
- Register roles: edi=this, ebp=&PTankard (this+0x34, set once at 0x94A3F8 and reused for every GetPItem / IS_THING_ALIVE),
  ebx=tankard component (xor ebx,ebx = `CTCTavernTankard* tankard = 0`, kept to the end), esi = map / seed pointer.
- Frame: [esp+0x10] = shared key slot for GetTC keys 0x23, 0xBA, 0xCF; [esp+0x14] = village (CTCVillage*);
  the ARG slot [esp+0x1C] (canDrink) is reused AFTER its last read (0x94A4A9) as the 0xB8 key slot (0x94A575) and as the
  never-written out-pointer of the 0xB8 GetTC miss path (0x94A598). The 0x23 miss path (0x94A3EF `mov ecx,[esp+1Ch]`) also
  reads the arg slot as the never-written villageMember out-pointer. Old draft: "+4 frame" = it gave the 0xcf key/out its own word.
- Not size-optimised (no `optimize("s")` idioms: imul/add/div inline, jcc chains). Same-TU siblings use no pragmas.
- Tail merging in retail: ONE Drink block 0x94A4EA (`mov ecx,edi ; call 0x948880 ; epilogue`) is the target of 0x94A4DC,
  0x94A52C, 0x94A558, 0x94A6AA; ONE SitIdle block 0x94A6B0/0x94A6B2 is reached from 0x94A496 (game busy), 0x94A5D5 (talk case 0,
  ecx already = edi), 0x94A64E, 0x94A68E (rand%5 default) and the fall-through after IS_THING_ALIVE false; Laugh block 0x94A5E1
  is shared by talk case 2 and the "someone laughing" branch (0x94A62A). Several jcc's jump with `mov ecx,edi` already done.
- Float tests: `Fullness != 0.0f` via `fld [0x0122DEDC] (0.0f literal, see 0x00B18AF0 notes) ; fld [ebx+0Ch] ; fucompp ;
  test ah,44h ; jp Drink` and the `return false` (xor al,al) inline at 0x94A4DE. `Fullness == 1.0f` is an INTEGER compare
  `cmp dword ptr [ebx+0Ch],3F800000h` (0x94A51A) with `mov ecx,edi` scheduled between cmp and jne.
- ThankBarmaid path (0x94A525..0x94A535): `call ThankBarmaid ; test al,al ; jne Drink ; epilogue` -- returns ThankBarmaid's
  own al (NO `xor al,al`), unlike the 0.0f path. Model as e.g. `if (ThankBarmaid()) return Drink(); return false;` only if
  VC7.1 reuses al; else try `bool r = ThankBarmaid(); if (r) return Drink(); return r;`.
- Randoms (inlined GFRandom: seed*0x24A1+0x24DF, store, call GFROR13 0x497890, store eax): `%4==0` -> `test al,3 ; je`;
  talk switch `and eax,3 ; sub eax,0 ; je / dec ; je / dec ; jne` (switch on GFRandom(4)); `%2` -> `test al,1`; final
  `div 5` + `sub edx,0 ; je / dec edx ; jne` (switch on GFRandom(5)). Each GFRandom call re-calls GetPWorldSeed (0x8FCFD0).
- Control flow (retail):
  1 villageMember = creature.GetTC(0x23) [inline HasTC bit test `test byte [eax+24h],8`]; village = villageMember->GetPVillage().
  2 tankard=0; if IS_THING_ALIVE(PTankard) [inline: GetPItem x2 + `test byte [eax+91h],1`] PTankard->GetTC(0xBA,&tankard)
    [`test dword [eax+34h],4000000h`].
  3 game: PTable.GetPItem() then `mov ecx,[eax+38h] ; test ch,ch ; jns` (HasTC 0xCF) -> GetTC(0xCF,&game); if game->IsBeingPlayed
    (+0x20) -> SitIdle.  (PTable GetPItem called ONCE for both test and lookup.)
  4 if (!village->IsTavernTime() && canDrink && IS_THING_ALIVE(PTankard) [inline]) return Fullness!=0 ? Drink() : false;
  5 if (IS_THING_ALIVE(PTankard) [inline] && tankard) { if Fullness==1.0f: ThankBarmaid -> Drink / return al;
    else if GFRandom(4)==0 -> Drink }
  6 PTable.GetPItem()->GetTC(0xB8,&table) [`test dword [eax+34h],1000000h`]; if table->NumPeopleSeated(+0x40) > 1:
    if SomeoneTalking(+0x44): switch GFRandom(4) {0 SitIdle,1 Listen,2 Laugh,default TellStory};
    if SomeoneLaughing(+0x45) && GFRandom(2)==0 -> Laugh; GFRandom(2)==0 -> SitIdle else TellStory.
  7 switch GFRandom(5) {0: if IS_THING_ALIVE(PTankard) [OUT OF LINE call 0x004CC340, ecx=ebp] -> Drink; break; 1: FallAsleep}
    -> SitIdle.

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, Ego_r 0x0096DC40) and FableWin.pdb (_fablewin_api.tsv / FableWin signatures
  0x017BD27A): `?ChooseAction@CAIStateGroup_GoToPub@@IAE_N_N@Z` = protected, non-virtual, non-const
  `bool CAIStateGroup_GoToPub::ChooseAction(bool)`. Manifest label is CORRECT. Mangled leaf: ?ChooseAction@CAIStateGroup_GoToPub@@IAE_N_N@Z
- TU order proves it too: Ego_r FallAsleep 96D960, Listen 96DA10, SlumpDrunk 96DA80, TellStory 96DB30, ChooseAction 96DC40,
  WalkToPoint 96DFB0  <->  retail 0x94A0E0, 0x94A190, 0x94A200, 0x94A2B0, 0x94A3B0, 0x94A6D0 (same order).
- Callers (xref.py): CAIStateGroup_GoToPub::Update 0x0094AB00 (`push 1 ; mov ecx,ebp ; call` at 0x94AFDE -> ChooseAction(true));
  0x0094C7D0 = CAIStateGroup_GameMaster::ChooseAction() (`push 0 ; call 0x94A3B0 ; ret`; Ego_r `?ChooseAction@CAIStateGroup_GameMaster@@MAE_NXZ`,
  virtual) -> `return CAIStateGroup_GoToPub::ChooseAction(false);`. Param name canDrink is descriptive (PDB has no param names here).
- No engine/CAIStateGroup_GoToPub.h header: siblings model the class locally on top of engine/CAIStateGroupBase.h.
  Ego_r layout: PPub +0x14 CIntelligentPointer<CThingBuilding>, PBar +0x1C <CThingObject>, PTable +0x24 <CThingObject>,
  PBarman +0x2C <CThingAICreature>, PTankard +0x34 CIntelligentPointer<CThing>, SeatIndex +0x3C, ... HandsOffMode +0xA0.

## 3. Callees (landed file / landed name / PDB name; DRAFT MISMATCHES flagged)
- 0x0040F020 rebuild/src/compiled/00/40/CVectorMap_LowerBound_0040f020.cpp  CVectorMap::LowerBound (non-const, ret 4) -- via engine/CThing.h GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED).
- 0x007EE540 .../00/7e/CTCVillageMember_GetPVillage_007ee540.cpp  `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` (thiscall const).
  DRAFT WRONG: "GFGetPVillageInterface(CTCVillager*) __fastcall". Declared in engine/CTCVillageMember.h:39.
- 0x00658C0A .../00/65/CTCVillage_IsTavernTime_00658c0a.cpp  `?IsTavernTime@CTCVillage@@QBE_NXZ`. DRAFT WRONG: "IsCookTime"
  (propagated label). engine/CTCVillage.h:44 under FABLE_RETAIL_CTCVILLAGE_TAVERNTIMES.
- 0x00A01B50 .../00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp  CBaseIntelligentPointer::GetPItem (reach via CIntelligentPointer<T>::Get()/operator->).
- 0x00948880 NOT LANDED; manifest CAIStateGroup_GoToPub::Drink; Ego_r `?Drink@CAIStateGroup_GoToPub@@IAE_NXZ`. Draft OK.
- 0x00949380 .../00/94/CAIStateGroupGoToPub_ThankBarmaid_00949380.cpp `?ThankBarmaid@CAIStateGroup_GoToPub@@IAE_NXZ`. OK.
- 0x008FCFD0 .../00/8f/CAIStateGroupBase_GetPWorldSeed_008fcfd0.cpp -- landed as FREE fastcall `CAIStateGroupBase_GetPWorldSeed`
  (NAME_FIXES candidate); PDB `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` (protected CONST member) = engine/CAIStateGroupBase.h:75
  `unsigned long* GetPWorldSeed() const`. Draft declared it non-const.
- 0x00497890 .../00/49/global_GFROR13_00497890.cpp `?GFROR13@@YIJK@Z` long __fastcall(unsigned long); use engine/GFRandom.h
  (inline GFRandom(range, seed) = the LCG; declares GFROR13). Draft's local WorldRandomRange duplicates it.
- 0x00947D30 Laugh, 0x0094A190 Listen, 0x0094A2B0 TellStory, 0x0094A0E0 FallAsleep, 0x00947CA0 SitIdle: all landed
  (.../00/94/CAIStateGroupGoToPub_<Name>_<addr>.cpp); PDB `?<Name>@CAIStateGroup_GoToPub@@IAE_NXZ` -- protected bool(). OK.
- 0x004CC340 .../00/4c/global_ISTHINGALIVECIntelligentPointerCThing_004cc340.cpp  `??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z`
  = engine/ThingAlive.h template instantiation. DRAFT used an ad-hoc `IS_THING_ALIVE_P` template + a local inline IS_THING_ALIVE.
- Side NAME_FIX (not a callee): 0x0094A200 landed as "FallAsleepAtTable" (marked descriptive) -- Ego_r TU order puts
  `?SlumpDrunk@CAIStateGroup_GoToPub@@IAE_NXZ` (96DA80) exactly there (between Listen and TellStory); its anim
  SLUMPED_DRUNK_AT_TABLE agrees. Real name very likely SlumpDrunk.

## 4. Members / components
- this+0x04 PBrain (CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE[_MEMBERS]); PBrain+0x18 CAIBrain::Creature
  (CThingAICreature*, engine/CAIBrain.h:66); GetCreature(): FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE
  (returns CThingAICreature&; landed FreeSeat 0x9486D0 uses `GetCreature().GetTC(TCI_CARRYING,&c)`).
- CThing +0x20 UsingComponents bitset, +0x44 Components CVectorMap, +0x91 Dead bit (IsDead) -- engine/CThing.h; GetTC(id,&p)
  via FABLE_RETAIL_CTHING_GETTC_EXPANDED (exact retail shape: key copied inside the test, map through a pointer local, conditional expr).
- 0x23 TCI_VILLAGE_MEMBER -> CTCVillageMember (engine/CTCVillageMember.h). DRAFT called it "CTCVillager".
- 0xBA TCI_TAVERN_TANKARD -> CTCTavernTankard (Ego_r: Fullness float +0x0C, RHSet +0x10, Pos +0x28, PBarmaid +0x34). No header;
  model locally (draft "CTCTankard" -> rename CTCTavernTankard). (Not CTCTavernJug, whose Fullness is int.)
- 0xCF TCI_TAVERN_GAME (207) -> CTCTavernGame::IsBeingPlayed bool +0x20 (engine/CTCTavernGame.h:140/220, FABLE_RETAIL_CTCTAVERNGAME
  for the polymorphic layout). DRAFT WRONG: "CTCEating / Busy".
- 0xB8 TCI_TAVERN_TABLE -> CTCTavernTable (engine/CTCTavernTable.h): NumPeopleSeated +0x40 long, SomeoneTalking +0x44, SomeoneLaughing
  +0x45 under FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT (needs FABLE_RETAIL_CINTELLIGENTPOINTER + CIntelligentPointer.h first).
- PTankard/PTable: CIntelligentPointer<T> (engine/CIntelligentPointer.h, FABLE_RETAIL_CINTELLIGENTPOINTER; for IS_THING_ALIVE:
  _DEREFERENCE, _GET_ORIGINAL_CHAIN, _COMPARE_NULL_INT as in the 0x4CC340 row).
- Classes the old draft models LOCALLY that have shared headers: CThing (CThing.h), CInterfaceMap (CVectorMap.h),
  CBaseIntelligentPointer/CIntelligentPointer (CIntelligentPointer.h), CTCVillage (CTCVillage.h), CTCVillageMember (as "CTCVillager"),
  CTCTavernTable (CTCTavernTable.h), CTCTavernGame (as "CTCEating"), CAIBrain (CAIBrain.h), CAIStateGroupBase (CAIStateGroupBase.h),
  CThingCreatureBase, IS_THING_ALIVE (ThingAlive.h), GFROR13/random (GFRandom.h).

## 5. Callee visibility
- Same TU (defined earlier, all called OUT of line in retail): SitIdle 0x947CA0, Laugh 0x947D30, Drink 0x948880, ThankBarmaid
  0x949380, FallAsleep 0x94A0E0, Listen 0x94A190, TellStory 0x94A2B0. Just declare them (bodies big; retail calls them).
- Inline in retail: HasTC/GetTC (forceinline, CThing.h), GetCreature, GFRandom (GFRandom.h), IS_THING_ALIVE x3 expanded inline
  (0x94A3F8, 0x94A4B1, 0x94A4FB), BUT the 4th IS_THING_ALIVE (rand%5 case 0, last in source) is the OUT-OF-LINE COMDAT 0x4CC340.
  Likely VC7.1 inline budget exhausted by then with the template from engine/ThingAlive.h (FABLE_RETAIL_IS_THING_ALIVE, NOT _NOINLINE).
  If the budget doesn't run out, alternatives: the first three are spelled differently (e.g. inline CIntelligentPointer test) or
  a per-call trick; FABLE_RETAIL_IS_THING_ALIVE_NOINLINE would make all four out of line (wrong).
- Out of line: GetPItem 0xA01B50, LowerBound 0x40F020, GetPVillage 0x7EE540, IsTavernTime 0x658C0A, GetPWorldSeed 0x8FCFD0, GFROR13.
- No STLport helpers called.

## 6. Sibling landed rows (same class, same TU neighbourhood; all no pragma, no // Flags line found)
- 0x009486D0 FreeSeat (.../00/94/CAIStateGroupGoToPub_FreeSeat_009486d0.cpp): BEST TEMPLATE -- fully shared headers:
  FABLE_RETAIL_CAIBRAIN_GETCREATURE, _CAISTATEGROUPBASE(+_MEMBERS,_GETCREATURE), _CINTELLIGENTPOINTER(+_ASSIGN,_COMPARE_NULL_INT),
  _CTHING, _CTHING_GETTC_EXPANDED, _CTHINGAICREATURE, _CTHINGCREATUREBASE, _CTHINGGAMEOBJECT, _CTHINGPHYSICAL, _CTCBASE, _CCLASS,
  _DEF_CLASS_BASE, _CTCTAVERNTABLE_FREESEAT, _IS_THING_ALIVE; includes CAIBrain.h, CAIStateGroupBase.h, CIntelligentPointer.h,
  CTCTavernTable.h, CThing.h, CThingAICreature.h, CThingObject.h, CVectorMap.h, ThingAlive.h. Local class CAIStateGroup_GoToPub :
  CAIStateGroupBase with PPub..ActionPointIndex typed members. Uses `IS_THING_ALIVE(PTankard)`, `PTankard.Get()->GetTC(TCI_TAVERN_TANKARD,&t)`.
- 0x0094A2B0 TellStory: GETTC_EXPANDED + CThing.h/CVectorMap.h/CIntelligentPointer.h, local GoToPub model, GFRandom LCG inline, 0xB8 out-param miss slot shared.
- 0x0094A840 PayMoney (RELOCATION_MATCH): CCHARSTRING, GETTC_EXPANDED, named locals ordering.
- 0x0094A6D0 WalkToPoint (next function in TU): fable_compiler.h + CAIStateGroupBase.h + C3DVector.h.
- 0x009490D0 GetBar / 0x00949320 FreeBarPoint / 0x00948060 InitCreaturePosition: GETTC_EXPANDED idiom.

## 7. Old draft model errors
- Wrong names: GFGetPVillageInterface (->CTCVillageMember::GetPVillage const), IsCookTime (->IsTavernTime const), CTCVillager
  (->CTCVillageMember), CTCEating/Busy (->CTCTavernGame/IsBeingPlayed), CTCTankard (->CTCTavernTankard), GetPWorldSeed non-const,
  local WorldRandomRange (->GFRandom), IS_THING_ALIVE_P hack (->ThingAlive.h template).
- Duplicated `return Drink()` / `return SitIdle()` tails; retail has one block each (tail-merged) -- 813 v 785. The tankard and
  game paths jump into the single Drink/SitIdle block; consider a structure where Drink()/SitIdle() calls are fewer or the shapes are
  identical so VC7.1 cross-jumps them (identical `mov ecx,edi; call; epilogue`), e.g. via ThingAlive.h / GetTC shapes changing frame.
- Frame +4: retail uses only 2 local words + arg slot; out-pointers for 0x23 and 0xB8 must be the never-written uninitialised locals
  (GetTC result ignored only when guarded the same way) -- draft's local GetInterface stores key differently for 0xcf.
- GetCreature in draft returns PBrain->PCreature pointer from local CAIBrain; shared header returns reference (`*Creature`).
- Draft's control flow otherwise matches retail (checked branch by branch above), including the switch cases.

## 0x00D17A80 (not landed)

# DOSSIER 0x00D17A80 NScript::CQ_BountyHuntScript::WatchForBoasts (785 bytes)

## 1. Retail shape (xdis 00d17a80 0x321)
- __thiscall, no args, plain `ret` (6 return blocks, each `pop edi/esi/ebp/ebx; add esp,1Ch; ret`).
- NO frame pointer (`sub esp,1Ch; push ebx,ebp,esi,edi`), NO SEH prologue despite CCharString/vector locals -> default lane build (no /EHsc); no optimize("s") idioms (uses `add esp,4` after cdecl calls, `lea` alignment pads before loop heads: speed/Ot).
- Frame: boastName CCharString @esp+0x14 (ctor 0x0099E4B0 at entry, lives across whole fn); boasts vector @esp+0x20 (start +0x20, finish +0x24, eos +0x28); allocator dummy @esp+0x13; spawn-name temp @esp+0x18, region "" temp @esp+0x1c; ebx = EH-style "temp constructed" flag bit 1 for the new-expression's name temp (`or ebx,1` .. `test bl,1; and ebx,-2`).
- Control flow: `CCharString s; while(!IsActiveThreadTerminating()) { { vector boasts; Game->MsgOnBoastsMade(boasts); for (i=0; i != size(); ++i) {...} if (IsActiveThreadTerminating()) return; } Game->NewScriptFrame(); }`
  * the vector is destroyed (inline element loop + free, 0x00D17C31..0x00D17C5D) BEFORE `NewScriptFrame` (slot 7 call at 0x00D17C65) -> vector must be in an inner `{}` block that closes before NewScriptFrame (the post-loop terminate test at 0x00D17C1E is inside that block).
  * while loop rotated: entry test at 0x00D17A9A, bottom test 0x00D17C6A `je 0xd17aa7`.
  * for loop: top guard `test ecx,0xFFFFFFF8` (size()!=0), bottom `inc ebp; sar eax,3; cmp ebp,eax; jne` -> `!=` compare (jne, not jb), index in ebp starting 0.
  * element read: `esi = boasts[i].first`, `boastName = boasts[i].second` (operator= 0x0099EFB0) every iteration, BEFORE the 15/16 compare.
  * `if (first==15) { if (!BoastActiveTimeLimit) { if(term) return; BoastActiveTimeLimit=true; spawn WatchForQuestInfoDisplayConditions } }` -- note retail stores `mov byte[edi+78h],1` interleaved after `push 3Ch` (the store happens before operator new: assignment precedes the spawn statement).
    After the spawn (and when BoastActiveTimeLimit was already set, jne 0xd17bcb skips the re-test) flow goes to the timer test.
  * `else if (first==16) { if (!BoastActiveDecapitation) { if(term) return; BoastActiveDecapitation=true; } }`
  * then `if (BoastActiveTimeLimit && BoastTime.GetTime() <= 0 && !MasterScriptData->BountyHuntTimeLimitExceeded) { if(term) return; MasterScriptData->BountyHuntTimeLimitExceeded = true; }` (`jg` on GetTimer result; GetTime is INLINE through the GLOBAL g_pGameScriptInterface 0x0143E8F8 slot 90 +0x168 with TimerIndex +0x80, not through Game).
- Spawn expression: `operator new(0x3C)` -> if non-null: CCharString("WatchForQuestInfoDisplayConditions" 0x012CA7A4, -1) temp, CSpawnedFuncBase(name,0) 0x00CDD450, vtable 0x012C9EDC, Func 0x00D17DA0 @+0x34, PClass=this @+0x38; else esi=0. THEN CCharString("" 0x0122D70E) temp, push it, push esi, AddSpawnedFunction 0x00CB7E50; dtor "" temp, then conditional dtor of name temp. Same expression form as landed Deputy2 Main line 149: `AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>(CCharString("WatchForQuestInfoDisplayConditions"), 0, this, &CQ_BountyHuntScript::WatchForQuestInfoDisplayConditions), CCharString(""));` (without the "ParentClass." concat -- this is the class's own SpawnFunction form).
- THE RESIDUE (6 exits destroy the vector differently):
  | exit | site | element destroy | storage free |
  |---|---|---|---|
  | for-top terminate (1st in source) | 0x00D17C88 | out-of-line `??_G?$pair@JVCCharString@@@std@@` 0x004DA68A with flag 0 (push 0) | out-of-line 0x004D9C5D (~_Vector_base, folded) |
  | time-limit arm terminate | 0x00D17CBE | inline ~CCharString(+4) loop | out-of-line 0x004D9C5D |
  | decap arm terminate | 0x00D17CF9 | inline loop | out-of-line 0x004D9C5D |
  | expiry-flag terminate | 0x00D17D2E | inline loop | inline free (tail shared with next) |
  | post-loop terminate | 0x00D17D51 | inline loop | inline free 0x00BFEA14 |
  | normal end-of-block | 0x00D17C31 | inline loop | inline free |
  Inline-ness grows toward the LATER source sites -> looks like VC7.1 inline-budget exhaustion with sites processed bottom-up; the draft only tried in-body inline_depth pragmas. Untried lever: TU flag sweep `// Flags: /d2inlSNN` (sibling 0x00D18DF0 uses `/d2inlS15` for exactly this kind of per-site inline/out-of-line split), and/or modelling pair's implicit dtor as non-inline only at -- not possible per site; so budget is the realistic lever. Also the out-of-line ??_G(0) means the inner `_Destroy(pair*)` was called as `p->~pair()` through the scalar-deleting dtor (row 0x004DA68A notes: VC7.1 emits ??_G flag 0 for a non-inlined implicit dtor).

## 2. Name / identity (manifest label CORRECT)
- Leaf `?WatchForBoasts@CQ_BountyHuntScript@NScript@@QAEXXZ` -> `public: void __thiscall NScript::CQ_BountyHuntScript::WatchForBoasts(void)`, non-const, void.
- Source 1: Ego_r.pdb (ghidra_out/egor_pdb_names.tsv:38675, @0x00D1F110 in Ego_r build) and FableWin.pdb (ghidra_out/_fablewin_api.tsv:312277 / naming_stragglers FableWin.exe.signatures.tsv:317938 @0x036B1F10).
- Source 2: only xref is retail CQ_BountyHuntScript::Main 0x00D13580 (not landed): at 0x00D138B0 it builds CCharString(0x012CA45C = "WatchForBoasts") and stores Func=0x00D17A80 into a CSpawnedFunc<CQ_BountyHuntScript> (vtable 0x012C9EDC) -> AddSpawnedFunction.
- Not virtual; no vtable slot.

## 3. Callees (landed row | landed name | PDB/real name)
- 0x0099E4B0 CCharString() | 00/99/CCharString_CCharString_0099e4b0.cpp | ??0CCharString@@QAE@XZ -- ok
- 0x00CB7940 | CScriptBase_IsActiveThreadTerminating_00cb7940.cpp | CScriptBase::IsActiveThreadTerminating -- ok (engine/CScriptBase.h:35)
- 0x0074F680 vector ctor | manifest label CIndexBuffer::CIndexBuffer (FOLDED COMDAT, wrong for us) | real: STLport `vector<pair<long,CCharString> >::vector(const allocator&)` (or _Vector_base(const _Alloc&)) emitted out of line: `ret 4`, three zero stores. Note it is CALLED out of line here (lea of dummy allocator esp+0x13 pushed). Draft models it as an out-of-line ctor: right idea. Shared stlport `vector(const _Alloc& a=_Alloc())` is inline -- need a row-side/opt-in out-of-line ctor declaration (no FABLE_RETAIL_STLP_* macro exists for the default vector ctor; only VECTOR_N/FILL/COPY_OUTOFLINE).
- vtable Game slot 34 +0x88 MsgOnBoastsMade(vector<pair<long,CCharString> >&) const 0x008A9AE0 (engine/CGameScriptInterfaceBase.h:57) -- ok
- 0x0099EFB0 | CCharString_operator_0099efb0.cpp | CCharString::operator=(const CCharString&) -- ok
- 0x00BFEA1A | global_operatornew_00bfea1a.cpp | operator new
- 0x0099EBF0 | CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long=-1)
- 0x00CDD450 | CSpawnedFuncBase_CSpawnedFuncBase_00cdd450.cpp | CSpawnedFuncBase(const CCharString&, ulong)
- 0x00CB7E50 | CScriptBase_AddSpawnedFunction_00cb7e50.cpp | CScriptBase::AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&)
- 0x0099EAE0 | ~CCharString
- g_pGameScriptInterface 0x0143E8F8 slot 90 +0x168 GetTimer(long) const 0x006E7710 (inline CTimer::GetTime)
- 0x00BFEA14 free (MSVCR71) | global_free_00bfea14.cpp
- 0x004DA68A | 00/4d/stdpairlongCCharString_scalardeletingdestructor_004da68a.cpp | ??_G?$pair@JVCCharString@@@std@@QAEPAXI@Z (optimize("s") TU) -- ok
- 0x004D9C5D | 00/4d/_unattributed_ReleaseFirstMember_004d9c5d_004d9c5d.cpp | landed as DESCRIPTIVE placeholder `CFirstMemberOwner_004d9c5d::ReleaseFirstMember_004d9c5d`; by this call site it is the folded STLport `~_Vector_base<pair<long,CCharString>,allocator<...> >` (free of _M_start). NAME_FIX candidate (one source only: shape + this call site; no PDB hit checked).
- Game slot 7 +0x1c NewScriptFrame() const 0x006E7410 -- ok
- 0x00D17DA0 (address-only) | NScriptCQBountyHuntScript_WatchForQuestInfoDisplayConditions_00d17da0.cpp | ?WatchForQuestInfoDisplayConditions@CQ_BountyHuntScript@NScript@@QAEXXZ; retail string 0x012CA7A4 confirms.
- Draft spelling disagreements: draft calls 0x004D9C5D "base dealloc" (fine), names the +0xDA flag `BoastTimeLimitExpired` (WRONG -- real `BountyHuntTimeLimitExceeded`), names master-data pointer `Data` and type `CQ_BountyHuntMasterData` (WRONG -- `MasterScriptData`, `NScript::CQ_SunnyvaleMasterData`), interface type `CGameScriptInterface` (shared header uses `CGameScriptInterfaceBase* Game`).

## 4. Members
- this+0x40 Game: CLevelScriptBase<CQ_SunnyvaleMasterData>::Game (CGameScriptInterfaceBase*; or reference under FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS) -- engine/CLevelScriptBase.h.
- this+0x44 MasterScriptData (T* = CQ_SunnyvaleMasterData*) -- engine/CLevelScriptBase.h.
- this+0x78 bool BoastActiveTimeLimit, +0x79 bool BoastActiveDecapitation, +0x80 CTimer BoastTime (TimerIndex +0) -- engine/CQ_BountyHuntScript.h:90-93; Ego_r struct_layouts 140/141/148 (-0x14 retail shift). (+0x7c BoastTimerID unused here.)
- MasterScriptData+0xDA bool BountyHuntTimeLimitExceeded -- engine/CQ_SunnyvaleMasterData.h:167; struct_layouts_egor CQ_SunnyvaleMasterData 218.
- Opt-ins: CQ_BountyHuntScript.h offers FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_{GANGDEPUTY1,GANGDEPUTY2,HOSTAGE1,HOSTAGE2,GANGLEADER}; NONE declares WatchForBoasts or WatchForQuestInfoDisplayConditions -> the shared header needs a new opt-in (e.g. FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS) declaring both (coordinator to add). Needs FABLE_RETAIL_NSCRIPT (CLevelScriptBase/CTimer/CScriptBase #error without it), FABLE_RETAIL_SUSPENDABLE_PROCESS for engine/CSpawnedFunc.h (__forceinline ctor, Func +0x34, PClass +0x38), CTimer::GetTime defined inline after the include (CTimer.h wave-8 convention: `inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }`).
- Classes the draft models LOCALLY that have shared headers: CCharString (fable_string.h / FABLE_RETAIL_CCHARSTRING), std::pair/vector (stlport/fable_containers.h), CGameScriptInterface (engine/CGameScriptInterface.h / CGameScriptInterfaceBase.h), CTimer (engine/CTimer.h), CSpawnedFuncBase/CSpawnedFunc (engine/CSpawnedFuncBase.h, CSpawnedFunc.h), CScriptBase (engine/CScriptBase.h), CQ_BountyHuntScript (engine/CQ_BountyHuntScript.h), master data (engine/CQ_SunnyvaleMasterData.h).

## 5. Callee visibility
- Vector ctor 0x0074F680: called out of line (COMDAT, aligned) -> declare without body.
- ~vector: retail mixes; at the fully-inline sites the body is STLport `_Destroy(start,finish)` (inline __destroy_aux loop calling ~CCharString(+4)) + `_Vector_base` dtor (if start free). The pair dtor and ~_Vector_base bodies were visible (inlined at later sites) but emitted out of line at earlier ones (0x004DA68A / 0x004D9C5D are UNALIGNED addresses = COMDAT copies from other TUs, folded) -> budget effect, not invisibility.
- Relevant stlport macros: FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE / _DESTROY_AUX_FASTCALL_DECL (wrong: retail has no __destroy_aux call here), nothing for _Vector_base dtor out-of-line or default ctor out-of-line. ~vector at 0x004E34B2 (used out of line by landed CHeroBoastsScript::Main 0x00CE1AF0) is NOT called here.
- WatchForQuestInfoDisplayConditions 0x00D17DA0 (next function, same TU) only address-taken.
- Same-TU neighbours (retail order): WatchForAllGruntsKilled 0x00D17340, SpawnBanditGangGrunts 0x00D17470, [0x00D179B0 CBH_GangLeader::OnPersist], WatchForBoasts 0x00D17A80, WatchForQuestInfoDisplayConditions 0x00D17DA0, RemoveBoastQuestInfo 0x00D17E70, AddBoastQuestInfo 0x00D17EA0, CBH_GangDeputy1::Main 0x00D17F20.

## 6. Sibling landed rows
- rebuild/src/compiled/00/d1/NScriptCQBountyHuntScriptCBHGangDeputy2_Main_00d18df0.cpp: `// Flags: /d2inlS15` (inline-budget for per-site dtor inline split), defines FABLE_RETAIL_CCHARSTRING, _NSCRIPT, _SUSPENDABLE_PROCESS, _CLEVELSCRIPTBASE, _STLP_VECTOR_PUSHBACK_OUTOFLINE, _CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2; includes engine/CQ_BountyHuntScript.h, CSpawnedFuncBase.h, CSpawnedFunc.h; spawn idiom line 149.
- rebuild/src/compiled/00/d1/NScriptCQBountyHuntScript_SpawnBanditGangGrunts_00d17470.cpp: FABLE_RETAIL_NSCRIPT + FABLE_RETAIL_CCHARSTRING, shared CQ_BountyHuntScript.h, local std::vector, `#pragma auto_inline(off/on)` around one template specialization; no Flags.
- rebuild/src/compiled/00/d1/CQBountyHuntScript_WatchForAllGruntsKilled_00d17340.cpp: local vector whose dtor is expanded at 4 returns (notes: plain `inline` dtor got emitted out of line and called once; __forceinline expanded all).
- 0x00D17DA0 / 0x00D17E70 / 0x00D17EA0 rows: older local class models (CScriptBase-derived local CQ_BountyHuntScript), FABLE_RETAIL_NSCRIPT, no flags.
- rebuild/src/compiled/00/ce/NScriptCHeroBoastsScript_Main_00ce1af0.cpp: only other MsgOnBoastsMade caller landed; boast vector in its own `{}` block destroyed before the frame call; local vector model with out-of-line ~vector 0x004E34B2 and out-of-line _STLP_alloc_proxy ctor 0x0074F4B0.

## 7. Old draft model errors
- Vector scope: draft keeps `boasts` alive across `Game->NewScriptFrame()`; retail destroys it first -> put `{ vector; ...; if(term) return; }` then NewScriptFrame (draft notes claim body matched to post-loop test, so check this ordering in the differ).
- Wrong member names/types (Data/BoastTimeLimitExpired/CQ_BountyHuntMasterData; local CGameScriptInterface with padded vslots) -- use shared headers.
- Local vector ~BoastVector modelled as one inline dtor with custom _Destroy -> cannot produce the ??_G(0) out-of-line call nor the out-of-line 0x004D9C5D; need STLport-shaped vector (vector : _Vector_base, ~vector = _Destroy(range); ~_Vector_base frees) so the two levels can independently inline / not inline, then tune budget (/d2inlSNN sweep).
- `free` vs deallocate: inline sites call free 0x00BFEA14 directly (node allocator = malloc/free).
- Everything else (15/16 arms, re-tests, GetTime via global, spawn temp ordering) matches the disassembly.


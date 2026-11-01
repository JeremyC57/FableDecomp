# Hand chunk 675 (zlib build_tree .. CEnginePrimitiveManagerMeshBase::PreloadReflectionTexture): 9 of 10 byte-exact

## What matched (9)
- **zlib 1.2.1 trees.c** -- 0x00C052C0 `build_tree` (RELOCATION_MATCH, /GS): the landed whole-trees.c TU (0x00C03F50) text with
  one change, `(uch)((s->depth[n] >= s->depth[m] ? (int)s->depth[n] : (int)s->depth[m]) + 1)`. zlib was C, where the `?:` over two
  `uch` arms promotes both to int; the lane builds the TU as C++ (`extern "C"`), so the casts restore the C promotion (p1 DIFFER 492v498).
- **CVertexGroupBlockOptimiser** -- 0x00AD8ED0 `CountRemainingGroups() const`: mark used groups in a `vector<bool>(MaxGroupIndex+1,false)`,
  count marked groups the current block lacks. Needed the real STLport text: in-class `_Vector_base(n,a)` body, a `__declspec(noinline)`
  `vector<bool>(n,val,a)` (retail never expands 0x00451583), the `uninitialized_fill_n`/`__type_traits` chain, verbatim `_Rb_tree::_M_find`
  and a NAMED const_iterator from `find` in `HasGroup` (re-reads `end()`). 15 probes.
- **QuadricOptimiserInternals::COptimisedPrimitive** -- 0x00AC8840 `GetRemainingVertices(const CIndexList&) const`: the GetIntersection
  0x00AC8960 STLport set text plus `insert_unique` (0x00514C10, declared); the block-scoped `unsigned long index = *it;` before
  `remaining.Indices.insert(index)` was the lever. COptimisedVertex real layout (Exists +0x90, sizeof 0xB8).
- **CTCInventoryItem** -- 0x005D8EF0 `GetRHSetFromOrientation` (private static __fastcall): a 12-case switch returning anonymous-namespace
  `extern const CRightHandedSet` constants by value (C3DVector memberwise copy ctor). First grade.
- **Free function** -- 0x00CBEBA7 `GetHeroTitleAsLHTSTag()` (__fastcall, hidden CCharString result): 21-case switch on
  `g_pGameScriptInterface->GetHeroTitle()`; needs `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (516v345 without).
  The odd start address is genuine (previous function ends flush).
- **CSubtitleRenderer** -- 0x00443E20 implicit `~CSubtitleRenderer()`: full PDB member list at STLport sizes (deque 0x28 with a declared
  `~deque` 0x004427D0, vectors 12) over `engine/CBaseClassNonCopyable.h`; an empty PDB ctor makes the TU emit vtable/??_G/??1. First grade.
- **CAIStateGroup_WanderAroundShops** -- 0x00946F90 `FindASimplyDelightfulShop()`: same-TU real `CThingSearchTools::GetAllLeafThings<CThingBuilding>`
  template body and STLport `_M_insert_overflow`/`push_back` so the `shops` vector does not escape; `CTCShop* tc; if (HasTC(0x53)) tc = GetTC(0x53);`
  reproduces retail's absent-path read.
- **CAudioSource (libpk/OpenAL)** -- 0x00C72E00 `ApplySound(CAudioObject*, __int64)` (descriptive): the landed InitializeProperties 0x00C72AF0 model
  (inline alSourcei resolver on g_AudioContext). Bit tests are `(unsigned char)(x >> N) & 1` (retail `shr ; test reg8,1`); the hardware/software
  stream ternaries go through named locals (otherwise the flags push precedes the virtual call). 3 grades.
- **libvorbis 1.0.1 floor1.c** -- 0x00CA20A0 `accumulate_fit`: verbatim vendor text with the 1.0.1 `vorbis_info_floor1`; the static needs
  `static __declspec(noinline) int __stdcall` (lane libvorbis convention, callee-pop `ret 1Ch`) and a same-TU caller.

## Parked for Fable 5.1 (1)
- 0x00BEBCA0 CEnginePrimitiveManagerMeshBase::PreloadReflectionTexture DIFFER(445v484): retail recomputes `(end-begin)/0x2c` for the second `index < size ? &GraphicList[index] : 0` (ours CSEs or folds it), keeps index in edi / bank in esi (ours swapped), and lays the GetCurrentLoadedMipmapLevel -1 arm + shared epilogue after the two early-return blocks | tried: 22 probes -- forceinline helpers, if/return / result-local / `>=` / commuted `Size() > i` GetEntry forms, named count locals, early returns, two record-type views, signed count, optimize("g",off), /Ot,/O2, STLport-style GraphicList, nesting and return variants, CManagedTexture::Touch + GetCurrentLoadedMipmapLevel inlines (fixed the dead `add eax,20h`) | next: the same double-size shape occurs in PreloadDiffuseTexture 0x00BEC000 and PreloadBumpTexture 0x00BEC470 (both landed files grade DIFFER in this lane), so it is one shared inline helper; likely an LTCG-inlined out-of-line CGraphicDataBank accessor (lane gap: cross-module inline CSE) -- solve the three together.

## Skipped
- none.

## Label corrections
- 0x00C052C0 `_global::build_tree` -> zlib `build_tree` (static C).
- 0x00AD8ED0 VertexListMarkConnected -> CVertexGroupBlockOptimiser::CountRemainingGroups.
- 0x00AC8840 LTextTreeWalkThrough_BuildFromTree -> QuadricOptimiserInternals::COptimisedPrimitive::GetRemainingVertices.
- 0x00443E20 TextureCache_Destructor -> CSubtitleRenderer::~CSubtitleRenderer.
- 0x00946F90 CAIStateGroup_WanderAroundShops_SelectRandomEnemy -> CAIStateGroup_WanderAroundShops::FindASimplyDelightfulShop.
- 0x00C72E00 Audio_InitializeSource_AL -> CAudioSource::ApplySound (descriptive; thiscall member, not a free function).
- 0x00CA20A0 FloatVector_Clamp -> libvorbis accumulate_fit.
- 0x00BEBCA0 _global::PreloadReflectionTexture -> CEnginePrimitiveManagerMeshBase::PreloadReflectionTexture.
- Callees: 0x00947780 GetAllLeafThings<CThingAICreature,...> -> <CThingBuilding,vector<CThing*>,CThingFilter_HasTC>; 0x00772110
  CShaderPreParser::GetConstantCount -> CTCShop::GetNumShoppers; 0x006A80A0 Bitset_Test -> CThingFilter_HasTC::operator();
  0x004427D0 CBankFileAsyncData::FreeBuffer -> ~deque<CSubtitleRenderer::CSetTextInfo>. Full list in NAME_FIXES.tsv.

## Proven
- 0x00C052C0 build_tree -- zlib 1.2.1 trees.c `local void build_tree(deflate_state *s, tree_desc *desc)`; vendor text RELOCATION_MATCH; sits inside the landed trees.c run (0x00C03F50).
- 0x00AD8ED0 ?CountRemainingGroups@CVertexGroupBlockOptimiser@@ABEKXZ -- Ego_r.pdb name + member walk (InputList/CurrentBlock/MaxGroupIndex); RELOCATION_MATCH with that symbol as leaf.
- 0x00AC8840 ?GetRemainingVertices@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVCIndexList@2@ABV32@@Z -- Ego_r.pdb + sibling GetIntersection 0x00AC8960 TU; RELOCATION_MATCH.
- 0x005D8EF0 ?GetRHSetFromOrientation@CTCInventoryItem@@CI?AVCRightHandedSet@@W4EInventoryObjectOrientation@@@Z -- Ego_r/FableWin pdb names; constants by FableWin initializer order; RELOCATION_MATCH.
- 0x00CBEBA7 ?GetHeroTitleAsLHTSTag@@YI?AVCCharString@@XZ -- Ego_r.pdb; string table "_NONE".."_ARSE"; RELOCATION_MATCH.
- 0x00443E20 ??1CSubtitleRenderer@@UAE@XZ -- sole caller ??_G 0x004942A2 = slot 0 of vtable 0x012384BC, RTTI COL CSubtitleRenderer; PDB member walk; RELOCATION_MATCH.
- 0x00946F90 ?FindASimplyDelightfulShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@XZ -- Ego_r.pdb; caller IsRunnable 0x00947280; RELOCATION_MATCH.
- 0x00CA20A0 accumulate_fit -- libvorbis 1.0.1 floor1.c verbatim; callers in floor1_fit (0x00CA276D/0x00CA27B4); RELOCATION_MATCH.
- 0x00BEBCA0 ?PreloadReflectionTexture@CEnginePrimitiveManagerMeshBase@@SIXPBVC3DMeshMaterial@@KAA_NPBVCGraphicDataBank@@@Z -- Ego_r.pdb + FableWin.pdb; reads C3DMeshMaterial.ReflectionMapBankIndex (+0x18); siblings 0x00BEC000/0x00BEC470 (parked, name proven).
- Descriptive (not proven): 0x00C72E00 CAudioSource::ApplySound (no PDB for libpk; landed caller binding).

## Header debt
- CVertexGroupBlockOptimiser / CVertexGroupBlock, COptimisedPrimitive / COptimisedVertex / CIndexList, CThingSearchTools, CTCShop,
  CAudioSource / CAudioContext, CTCInventoryItem constants: restated locally (no engine headers, or the engine headers carry Ego_r
  Dinkumware sizes: vector 16 / map 16 / deque 0x50 vs retail STLport 12 / 12 / 0x28).
- CSubtitleRenderer uses engine/CBaseClassNonCopyable.h; its own members are local (no CSubtitleRenderer.h).
- Parked 0x00BEBCA0: CFrameData is the nested CGraphicInfoFrame::CFrameData in the PDB (modelled top-level in differ/).

## Callee names
- Proven PDB spellings used: CTCShop::GetNumShoppers, CAIStateGroupBase::PeekThingSearchTools / GetPWorldSeed, CThingSearchTools::PeekTypeList,
  CThingFilter_HasTC::operator(), STLport vector<bool> ctor / _Vector_base / _Rb_tree members, CEnginePrimitiveManagerMeshBase::GetAcceptablePreloadMipmap,
  CGraphicInfoFrame::CFrameData::BeginLoading / ctor, CResource::ListInsert / ListRemove, CGraphicInfoFrame::GetCurrentLoadedMipmapLevel.
- Descriptive: CAudioSource helpers 0x00C71F80 ComputePitchScale (catalog label kept), 0x00C720C0 StartFade, 0x00C72200 UpdateGain, 0x00C880A0 ApplyHardware, 0x00C733D0 ApplySoftware
  (InitializeProperties 0x00C72AF0 and SetPitchProperties 0x00C71F10 are the landed names); g_MaxShoppersPerShop_0138ce30; CResource::Touch (inlined MRU move).
- Globals: 0x0143BDB0 written `g_AudioContext` (landed majority; `?g_audioContext@@3UCAudioContext@@A` also bound -- one must win), 0x0143BC2C `g_pAudioMaster`.

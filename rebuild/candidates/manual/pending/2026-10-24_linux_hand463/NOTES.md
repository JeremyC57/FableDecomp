# Hand chunk 463 (vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> .. vector<EWeaponClass>): 40 of 40 byte-exact

Every row grades MATCH/RELOCATION_MATCH from the file in src/ (grades.txt), every test passes under g++ (tall.sh)
and VC7.1 (tall.py), and calls.py shows every relocation going to retail's target (no import calls in this chunk).
The catalog labels of all 40 rows are propagated: the real names are below and in each source's header.

## What matched (40)
- **_STL::_Destroy (00cdb820 CHeroSoulsRoundDef*, 00f28f20 CFeatDef*)** -- run the virtual destructor of every def in
  [first, last). Shape: /Gr TU, `__fastcall` template; `__destroy` -> `__destroy_aux(__false_type())` -> `first->~T()`
  inlined, the virtual dtor called through the vtable with flag 0; stride 0x38 / 0x64 = the PDB size less 4 per STLport
  vector.
- **vector::size (0043cac0 CAbilityButtonGuiDef, 0075ee70 CTCCameraPointScriptedSpline::CKeyCamera)** -- `_M_finish -
  _M_start` over 92- / 56-byte elements (reciprocal multiply with the `add edx,esi` correction).
- **vector::push_back (0051ae60 CLevelLoader::CObject::CLevelFiles)** -- STLport push_back; `_Construct` must be
  `__forceinline`; the CCountedPointer copy constructor bumps the shared count; __false_type homed in x's slot.
- **vector::resize(n, x), out-of-line erase (0052bea0 CThingTypeInfo, 0051d350 CRegion, 007d2670
  CShoveHaPennyRegionDef, 0088a390 CEnvironmentThemePriorityStackEntry, 00ad8090 CUniqueVertex, 00ad87a0
  COptimisedVertex, 00b7edf0 CPreTransposedBoneMatrix, 00f385a0 CGPUScratchBufferTextureManager::CResourceEntry)** --
  `if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x);` with erase and _M_fill_insert
  declaration-only. The element size (from the divide constant) is the only difference between the rows; FableWin.pdb
  mangles CPreTransposedBoneMatrix and CResourceEntry as structs (U).
- **vector::resize(n, x), inline erase (006b6d90 CIDrawEnvironmentParams::CThemeChannel, 0088a520
  NEnvironmentInternal::CBlendedEnvironmentTheme)** -- erase inlined as `__copy_ptrs` -> out-of-line `__fastcall __copy(...,
  random_access_iterator_tag(), (int*)0)` and `_Destroy` -> `__destroy_aux(..., __false_type())`; both tags need user
  constructors and are homed in the dead n slot.
- **vector::resize(n) forwarders (18 rows)** -- STLport `resize(n) { resize(n, _Tp()); }`; the default element's
  constructor is what the bytes show:
  - POD value-initialisation zero-fills: 004433f0 vector<long>, 0081dfb0 CTextureMorphEntry.
  - user constructors zeroing members: 00619290 CTCInventoryMap::CUIRegionInfo, 009d4300 CBankFile::CRuntimeData,
    0067f3a0 CDegradableInfo (empty body, only the embedded CEngineGraphic constructor runs).
  - size-optimised TU (`#pragma optimize("y", off)` + `("s", on)`: ebp frame, `and/or` stores, fldz/fstp): 0046aca8
    CSoundBankEntry, 0046b65a and 004e9f20 CMusicEntry (two TUs), 004ea398 CEngineGraphic (`pack(2)`, 14 bytes), 00581284
    CHeroMarriageInfo, 00657069 C2DCoordI (empty constructor: `push ecx ; push ecx`, no stores).
  - STLport pair (`pair() : first(_T1()), second(_T2())`): 0081de90 (EHeroMorphType, CTextureMorphEntry) -- the POD second
    is copied from a zero temporary kept in eax/edx/esi/edi; 008701a0 (EExpressionType, CExpressionSet::CEntry); 00759770
    (EBodyLocation, CWoundMorphs::CEntry); 0081df20 (unsigned long, CSkeletalMorphs::CEntry, empty constructor: only the
    key 0 and the CDefString -1); 0081ded0 (EHeroMorphType, CParticleMorphs::CEntry).
  - zero-fill then a late `-1`: 0081dfe0 / 0081ded0 CParticleMorphs::CEntry = `CEntry() { memset(this, 0, sizeof(*this));
    LocationName = CDefString(); }` with the intrinsic memset (without a user constructor VC7.1 does not zero a non-POD
    value-initialisation: first grade 31v68).
  - 0088a4e0 CEnvironmentThemePriorityStackEntry: `: BaseWeight(1.0f), ThemeTransitionActive(false) { BaseWeightLocked =
    false; }` gives retail's store order; retail never destroys the default entry, so its Themes vector is modelled with
    its constructor only (see LESSONS / review).
- **CPersistContext_TransferableVector::Resize (005813d7 CHeroMarriageInfo, 00618fd0 unsigned __int64, 00619420
  CTCInventoryMap::CUIRegionInfo)** -- vtable slot 1 (vtables 0x0124F350, 0x0125718C, 0x01257178): `TransVector->resize(size)`
  with TransVector at +4; in the /Os TU the one-argument resize needs `__forceinline`. PDB-exact names in FableWin/Ego_r.
- **set::insert(const value_type&) (004ac860 set<CCharString>, 00514230 set<long>)** -- `pair<_Rep_iterator, bool> p =
  _M_t.insert_unique(x); return pair<iterator, bool>(p.first, p.second);` (8-byte frame temporary, node pointer and bool
  copied to the hidden result), the landed 0x005BAF94 model in a speed TU.
- **CMusicManager::SetOverridingMusicSet(long, bool) (006bdea0)** -- `CState s = CurrentState; s.OverrideSet = set;
  InternalUpdate(s, force);` (bitwise six-dword copy: CState has only a default constructor). PDB name.
- **CTCWeaponTrail::GetLastProjectileWeaponTrailPoints (0076e5d0)** -- `return GetLastWeaponTrailPoints(points,
  ProjectileWeaponTrail);` with the list at retail +0x60. PDB name.

## Parked for Fable 5.1 (0)
None.

## Skipped
None (no row holds two functions).

## Label corrections
- 00cdb820, 00f28f20: catalog `vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::_Destroy` ->
  `_STL::_Destroy<CHeroSoulsRoundDef*>` / `_STL::_Destroy<CFeatDef*>`.
- 0043cac0, 0075ee70: `vector<CScriptThing>::size` -> `vector<CAbilityButtonGuiDef>::size` /
  `vector<CTCCameraPointScriptedSpline::CKeyCamera>::size`.
- 006b6d90, 0088a520: `vector<CTCInventoryMap::CUIMiniMapMarker>::resize` -> `vector<CIDrawEnvironmentParams::CThemeChannel>
  ::resize(n, x)` / `vector<NEnvironmentInternal::CBlendedEnvironmentTheme>::resize(n, x)`.
- 0051d350, 0052bea0, 007d2670, 0088a390, 00ad8090, 00ad87a0, 00b7edf0, 00f385a0: `vector<CVectorMap<unsigned long,...>>::resize`
  -> vector<CRegion>, <CThingTypeInfo>, <CShoveHaPennyRegionDef>, <NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>,
  <QuadricOptimiserInternals::CUniqueVertex>, <QuadricOptimiserInternals::COptimisedVertex>, <CPreTransposedBoneMatrix>,
  <CGPUScratchBufferTextureManager::CResourceEntry> `::resize(n, x)`.
- The 25 `vector<EWeaponClass>::_Umove<EWeaponClass*>` rows: 18 `vector<T>::resize(n)` forwarders (types above),
  3 `CPersistContext_TransferableVector<T>::Resize(long)`, 004ac860 `set<CCharString>::insert`, 00514230 `set<long>::insert`,
  006bdea0 `CMusicManager::SetOverridingMusicSet`, 0076e5d0 `CTCWeaponTrail::GetLastProjectileWeaponTrailPoints`.
- 0051ae60: catalog `vector<CLevelLoader::CObject::CLevelFiles>::push_back` confirmed.
- Callees: 0x006BD9B0 "CMusicManager_CopyAssign" is `CMusicManager::InternalUpdate(const CState&, bool)`; 0x004AC380
  "StdSet_Insert" and 0x00512DA0 "BinarySearchTree_Insert" are `_Rb_tree::insert_unique` (CCharString / long keys);
  coverage.tsv "push_back" at 0x0046ACCE, 0x0046B681, 0x004E9F47, 0x004EA3C3, 0x005811F1, 0x00656FAB are the two-argument
  `vector<T>::resize(n, x)` bodies; 0x007D26E0 "_Dest_val" is vector<CShoveHaPennyRegionDef>::resize(n); 0x0081B300
  "CExpressionSet::TransferBinaryIn" is the CVectorMap<EHeroMorphType, CTextureMorphEntry> (CTextureMorphs) transfer-in.
- No descriptive names: every row carries an STLport or PDB name (the three Resize rows and 006bdea0 / 0076e5d0 are
  PDB-exact symbols).

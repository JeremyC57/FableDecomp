# Hand chunk 455 (list<CMessageEvent_const*> .. pair<CCharString,CSymbolMap>): 39 of 40 byte-exact

All 39 src/ files grade MATCH/RELOCATION_MATCH through gall.sh (grades.txt), every test prints `<addr>_TEST PASS`
under g++ (tall.sh) and VC7.1 (tall.py), every `// Leaf:` names exactly one object symbol (objdump -t), and every
call target was paired with retail's by `$S/w22/calls.py` (free 0xBFEA14, operator delete 0xBFE9BC, mixer IAT,
landed callees). STL rows are modelled as STLport 4.6 templates in namespace `std`; class keys follow the PDB
name sets (CShopItem, CEngineSceneRenderStats, CEngineTextureMorph, CCompositeBlendDesc, CFileObject,
CLipSyncEntry, CScriptInfo are `class`; CSubTitle, CPendingQuery, pair are `struct`).

## What matched (39)

- **CWorld::GetFrameAsString** (0049d880, catalog "list<CMessageEvent const*>" ctor): static member formatting the
  frame counter through `__fastcall` GFIntToCharString (hidden result in ecx, the result pointer kept in esi).
- **std::_List_iterator<CVsyncCallbackTimer::CPendingQuery>::operator-- / operator++** (009f7780, 009f78f0, catalog
  list ctor): the unoptimised CVsyncCallbackTimer TU, `#pragma optimize("", off)`; `this` homed at [ebp-4] around the
  shared `_M_decr`/`_M_incr` (0x0041C7B0 / 0x0041C7A0).
- **std::list erase / pop_front / remove_if** (00a17110 CNavQuadTreeMap, 004b87f0 CQuestManager::CBoast, 004b91a0
  and 004b9250 CQuestCard, 009be100 CSubTitle, 00a219e0 long, 00a16eb0 pair<long, const CNavigatorBase*>): the
  payloads declare no destructor (the implicit one runs the members in reverse); CCountedPointer's dtor re-reads Info at
  every step (00a17110); CIVCountedPointer's release through a local copy plus the `Data = 0` store gives
  `dec [ecx+4] ; jne` (009be100); 009be100 needs `__forceinline erase` (it is pop_front, not the catalog's clear);
  004b9250 needs the landed `Compare` 0x00411570 body in the TU under `auto_inline(off)` so the unlink reuses `next`.
- **std::__lower_bound** (0070fcf0) over list<CAnimSet>::const_iterator by CAnimSetItPriorityLessThan: `__fastcall`
  template, priority at +0x4C (TLC 12-byte vectors).
- **std::__final_insertion_sort** (00470d36, catalog make_heap) over pair<unsigned long, CSoundPair>: size TU, inline
  3-argument `__unguarded_insertion_sort` passing `(_Tp*)0`; callee identity checked with objdump -r.
- **CAudioMixer::SetVolume** (00c71640, catalog make_pair): `_ftol` of the scaled span, mixerSetControlDetails through
  the IAT, the value homed in the dead argument slot.
- **libpk PKCWideString::operator=(const wchar_t*)** (00c16970) and **PKCPlaybackTemplate::ApplyTo** (00c14a80;
  descriptive names, libpk has no PDB): ApplyTo matches as an if/else of two calls (the shared push hoisted).
- **NFractal::CPerlinNoise::CPerlinNoise(unsigned long)** (00f36be0, catalog operator&&): `Initialise(seed); return this`.
- **std::pair copy ctor / operator=** (00654dd4, 00654eb6 pair<C3DVector, CCharString>, size TU: C3DVector has a user
  memberwise copy ctor but no operator=, so the assignment block-copies with movsd; 00cb5f50 pair<CCharString,
  CScriptInfo>; 009e6050 pair<CCharString, CSymbolMap::CSymbolInfo>).
- **std::map<CCharString, T>::operator[]** (009ac700 CCharString, 00519230 long, 008adf10 bool, 006bd6a0 C2DVector,
  00721640 pair<long, CDateAndTime>, 00756be0 pair<long, long>, 007aebe0 vector<CCharString>, 009d4810
  vector<unsigned long>, 00a2e290 vector<CMeshDataBank::CLipSyncEntry>): STLport 4.6 verbatim (`_M_lower_bound`
  out of line, inline CCharString operator<, `__default_constructed((_Tp*)0)`, insert forwarding to insert_unique).
  Shape levers: C2DVector's empty user ctor (no zero stores); STLport pair with `pair()` and a USER copy ctor
  (memberwise copy instead of a whole-pair `rep movsd`, 00721640 / 00756be0); for vector-valued maps the vector copy
  ctor's body in the TU under `auto_inline(off)` (VC7.1 then folds the default vector temporary's destructor, as retail).
- **Size-TU map::operator[]** (00cd39b1 map<CCharString, unsigned long>, twin of landed 0x0042D131; 00cd3d2e
  map<CCharString, CScriptGameResourceObjectScriptedThingBase>; 005bbc4e map<EClothingSuitPart, bool>; 005b322b
  map<NUISystem::CComponent*, pair<long, long> >; 005bbfd6 map<long, CEnginePrimitiveHandle>): 00cd3d2e needs
  `__default_constructed` (re-addresses the out-of-line-built default with `lea`, insert result in its own word);
  005bbc4e needs the chunk-422 insert stand-in (late `false` store); 005b322b needs the STLport pair with user copy
  ctor (two `and [ebp-x],0` stores ahead of the key); 005bbfd6 reuses 00485d30's handle model.
- **std::vector<CEnginePrimitiveHandle>::vector(size_type)** (00485d30, catalog map<long, ...>::operator[]): size TU;
  handle ctor/dtor `__forceinline`; retail's uniqueness test here is `(Object ? Object->RefCount : 0) == 1`; the
  allocator / `__false_type` (user ctor) temporaries overlay the high byte of the live `n` slot because the
  _Vector_base ctor (0x00485779, same retail TU) and `__uninitialized_fill_n` (landed 0x0043F6C0) bodies are visible.
- **/O2 map<scalar, T>::operator[] with inline lower_bound** (00991350 map<CPendingObjectSortKey, CFileObject*>,
  00778280 map<long, CShopItem>, 00b97b60 map<long, CEngineSceneRenderStats>, 00b2f5e0 map<pair<long, long>,
  CEngineTextureMorph>): the landed twin 0x009913D0 template; 00991350 needs the sort-key operator< with the two
  physical sort keys in named locals (as landed find 0x0098FE70) plus the twin's insert stand-in; CShopItem's ctor
  defaults {0, 1.0f, 0, 1, 0, 0, 0}; the stats ctor zeroes from the walk's xor register; 00b2f5e0 uses STLport's
  pair operator< and the vector copy-ctor stand-in.

## Parked for Fable 5.1 (1)

- 004c5b90 map<void*,CMeshEffectPriorityEntry>::operator[] DIFFER(170v161): the value_type's PEffect release: retail `mov ecx,[esp+10h] ; test ecx,ecx ; je ; dec dword [ecx+4] ; jne ; mov eax,[ecx] ; call [eax+4]`, ours `mov eax,[esp+10h] ; test eax,eax ; je ; dec [eax+4] ; mov ecx,[esp+10h] ; mov eax,[ecx+4] ; test eax,eax ; jne` (everything before it, incl. the overlaid default/value_type read-back copy, matches) | tried: member-form CIVCountedPointer dtor (170v161), local pointer copy (173v161), local + `Object = 0` as landed _Dest_val 0x00403E20 / 009be100 (173v161), pointee inline `Release()` (173v161) | next: spell the CIVCountedPointer copy ctor / release as a landed CIVCountedPointer<CEngineMeshEffect> user does (CTCGraphicAppearance code 0x004BD690 / 0x004BFE20), or the release on a named pointee local with DecRef/GetRefCount accessors; else lane gap: register-dec selection

## Skipped

None (no row spans two functions).

## Label corrections

- 0049d880 is CWorld::GetFrameAsString (catalog: list<CMessageEvent const*> ctor).
- 009f7780 / 009f78f0 are _List_iterator<CVsyncCallbackTimer::CPendingQuery>::operator-- / operator++ (catalog: list<CMessageEvent const*> ctor).
- 00c16970 is libpk PKCWideString::operator=(const wchar_t*); 00f36be0 is NFractal::CPerlinNoise::CPerlinNoise(unsigned long) (catalog: operator&&<CPredicate_And<...CIsThingAlive...>>).
- 00654dd4 is the pair<C3DVector, CCharString> copy ctor (catalog: operator=; 00654eb6 is the operator=).
- 00c14a80 is libpk PKCPlaybackTemplate::ApplyTo (descriptive; catalog: pair<CCharString, C3DMeshVolume> ctor).
- 00c71640 is CAudioMixer::SetVolume (catalog: make_pair<CSkeletalMorphs::CEntry, float&>).
- 009be100 is list<CSubTitle>::pop_front (catalog: clear).
- 0070fcf0 is STLport `__lower_bound` (catalog: Dinkumware lower_bound spelling); 00470d36 is `__final_insertion_sort` (catalog: make_heap).
- 00485d30 is vector<CEnginePrimitiveHandle>::vector(size_type) (catalog: map<long, CEnginePrimitiveHandle>::operator[]).
- Mapped types: 00519230 long, 008adf10 bool, 00756be0 pair<long, long>, 009d4810 vector<unsigned long> (catalog: C2DVector);
  007aebe0 vector<CCharString> and 00a2e290 vector<CMeshDataBank::CLipSyncEntry> (catalog: CArray<CLipSyncEntry>, a later build);
  00cd3d2e CScriptGameResourceObjectScriptedThingBase (catalog: CCountedPointer<NUISystem::CComponent>).
- Callees: 0x007E72A0 "CCarriedReadableDef" is CScriptGameResourceObjectScriptedThingBase() (vtable 0x0127094C);
  0x00CD27C7 "pair<CCharString, CTCBase::CRegisteredTCInfo>" is pair<const CCharString, CScriptGameResourceObjectScriptedThingBase>(a, b);
  0x005BB008 "_Pair_base<CComponent* const, pair<CWideString, long>>" is pair<const long, CEnginePrimitiveHandle>(a, b);
  0x004DD728 "~CTCShotDefinitionProjectile" also serves as ~pair<const long, CEnginePrimitiveHandle> (handle at +4);
  0x005BB346 "std::vector::operator=" is the vector<CCompositeBlendDesc> copy ctor; 0x00535470 "Std_Vector_Construct_Copy" is
  vector<unsigned long>'s copy ctor; 0x00485779 "Container_Initialize_PointerArray" is _Vector_base<CEnginePrimitiveHandle>(n, a);
  0x009D4380 "CStringMap_FindInsertPos", 0x004C5480 "AbilityDataList::FindAndInsert" and the "TransferIn" rows 0x00CD367C,
  0x00CD3CAE, 0x005B31CF, 0x005BBB7D, 0x005BBF9D are the map's `insert(hint, v)` / `insert_unique(hint, v)`.

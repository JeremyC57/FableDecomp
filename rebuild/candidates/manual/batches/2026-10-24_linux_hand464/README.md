# Hand chunk 464 (_STL::vector<long> .. CGPUScratchBufferTextureManager::CResourceEntry): 39 of 40 byte-exact

Every catalog label in the chunk was propagated from byte-identical bodies and named the wrong function (the eight
"vector<EWeaponClass>::_Umove", the 22 "vector<std::pair<CCharString>>::_Destroy/resize", ...). The real names come from
callers, FableWin.pdb / Ego_r.pdb name sets, Ego_r layouts (retail offsets: STLport vectors 12 bytes, lists 4) and RTTI.
All 39 matched rows grade MATCH / RELOCATION_MATCH through gall.sh, their tests pass under g++ (tall.sh) and VC7.1
(tall.py), and calls.py shows every call going to retail's target (no import mismatch).

## What matched (39)

- **STLport `__destroy_aux` / `_Destroy` (5, /Gr):** 0x004ABD90 CTestQuest (six CCharStrings, last member first),
  0x0076AE90 ParticleAndEndPoints (`_Destroy(first, last)` -> `__destroy` -> `__destroy_aux` inline), 0x007817B0
  NFire::CFirePoint (two CIntelligentPointers then the name), 0x00A43A00 NGameText::CDataBankTextEntry and 0x00AF1D90
  NParticleEngine::CParticleEmitterInfo. Shape: the CIVCountedPointer release must copy `Object` into a local before the
  decrement (`T* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; }`), else VC7.1
  re-reads the member (57 v 50). PDB def types are `class` (V): CTestQuest as a struct matched no symbol.
- **STLport `__unguarded_insertion_sort_aux` (9, explicit `__fastcall`, `ret 8`):** 0x004D0420 pair<float, CCharString>,
  0x005E0F60 CCharString/less, 0x006FBD10 pair<unsigned long, CSmallVector<CEvent,32>>, 0x0077EA10 pair<CCharString,
  C3DVector>, 0x009B2AC0 CWideString/less, 0x009B5A20 pair<CCharString, long>, 0x00A407C0 pair<CCharString,
  CSoundBankLH*>, 0x00A40FF0 pair<pair<CCharString, CCharString>, CSoundBankLH*>, 0x00A710F0 pair<CCharString,
  C3DMeshVolume>. Shape: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);` with the by-value
  entry built in its outgoing slot; `__unguarded_linear_insert` declaration-only (a call) except 0x009B2AC0 where retail
  inlines it (define it in the TU). Pair copies: C3DVector needs a memberwise user copy constructor (the implicit one
  forms `lea` + `add edi,4`, 73 v 69); C3DMeshVolume's implicit copy (with the CCountedPointer add-ref) is right.
- **STLport `vector<T>::resize(n)` (17):** inline two-argument resize (erase / `_M_fill_insert` out of line):
  0x00639810 CThingMapManager::CThingMapInfo, 0x0066E6D0 CRegionMinimap::RegionMinimapCellData (POD, value-init),
  0x006C9D50 pair<long, COpinionDeed*>, 0x006DD4A0 CAttackHistoryCombo, 0x007D9FC0 KnickKnack, 0x0081DF50
  CSkeletalMorphs::CEntry, 0x0086DCB0 pair<long, CRandomAppearanceMorph::CSkeletalMorph>, 0x0086EA80
  CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess>, 0x00786410 pair<long, float>; the out-of-line
  two-argument body itself 0x0098DA90 (C4DVector); one-argument wrappers over an out-of-line resize(n, x): 0x00A01820
  CGraphicInfo, 0x00A3D570 C3DAnimationStateAlpha, 0x00A43FF0 CLipSyncDataBankEntry, 0x00AD7350 COptimisedFace,
  0x00B6CC50 CLandscapeTheme, 0x00F38640 CGPUScratchBufferTextureManager::CResourceEntry, and 0x008883C0
  NEnvironmentInternal::CEnvironmentThemePriorityStack(long) (`Ents.resize(count)` inside the constructor). Shape:
  the temporary's store order follows the element's constructor (user zero ctor vs POD value-init; members assigned in
  the constructor body store after the init-list ones: CLandscapeTheme::WaterType, the priority entry's
  BaseWeightLocked; chained `Edges[0] = Edges[1] = Edges[2] = -1` stores right to left). The leaf must be the exact
  one-argument symbol (the inline two-argument resize is emitted too). New this chunk: element types with a real
  destructor match once the out-of-line callee has a same-TU noinline stand-in body that never writes through the value
  reference -- VC7.1 then folds the temporary's destructor test after the call, as retail (0x006DD4A0, 0x0086EA80,
  0x00A43FF0, 0x00A3D570).
- **STLport string / vector constructors (4):** 0x0099CA40 basic_string<unsigned short>(s, n, alloc), 0x0099CC40
  CWideStringData(const wchar_t*, long) (the same constructor inlined + `NoRefs = 1`), 0x00568CE0 basic_string<char>
  copy constructor (namespace `std` as the landed 0x00567D00 family), 0x00453659 vector<long>::vector<int>(int, int,
  alloc) (STLport's integral dispatch to `_M_initialize_aux`, in the module's frame-pointer size TU:
  `#pragma optimize("y", off)` + `("s", on)`). Shape: `_String_base(a)` / `_Vector_base(a)` zero stores first, then the
  empty iterator / `__true_type` tag (user constructor) homed in a dead argument slot and pushed first; member
  constructor templates instantiate explicitly in VC7.1.
- **STLport `__uninitialized_fill_n` (1):** 0x00AD62E0 over QuadricOptimiserInternals::CUniqueVertex (unsigned count:
  `test edx,edx ; jbe` / `dec ; jne`; placement-new null test; implicit copy = the Exists byte + two set<unsigned long>
  copies through the shared _Rb_tree copy constructor 0x00AD3A10).
- **CThingManager (1):** 0x0051F040 GetNameForThing_OnlyCalledFromTCHasName(unsigned __int64):
  `return (*NPCNameMap.find(uid)).second.second;` (no end test; the caller checks first) -- NPCNameMap sits at retail
  +0xCC, `_M_find` 0x00527E00 out of line.
- **CSBCCorridor (1):** 0x00A21F50 constructor: empty base constructors (only the final vptr store survives), STLport
  `_List_base(a) : _M_node(a, 0) { n = allocate(1); n->_M_next = n->_M_prev = n; _M_node._M_data = n; }` (malloc 0x20),
  `CurrentEndPoint(0.0f, 0.0f)`.
- **CCamera (1):** 0x00A0C830 SetAngleXZ(float): `C3DVector v(0, 0, 1); v.Rotate(RHSet.Forward, angle);` -- unfinished
  in retail (the result is dropped).

## Parked for Fable 5.1 (1)

- 00cdd0a0 CPersistContext_TransferableVector::Resize DIFFER(28v27): `lea eax,[esp+3]` / `mov byte ptr [esp+0Bh],0` where retail has `lea eax,[esp]` / `mov byte ptr [esp+8],0` (the bool() resize temporary at byte 3 vs byte 0 of the push-ecx frame word; one byte longer) | tried: inline one-argument `TransVector->resize(count)`, `resize(count, T())`, named `T value = T()`, STLport `__default_constructed((_Tp*)0)` | next: a bool temporary inside a 1-byte struct/union frame object, or `/O1`/`/Os` for this template TU (the other bool resize callers 0x00D34973 / 0x00E54A22 also home the byte at offset 3, so retail's offset-0 home is TU-specific)

## Skipped

None (no row holds two functions; 0x00453659's 38-byte row is a size-optimised body with no padding).

## Label corrections

- 0x00453659 `vector<long>::vector<int>` -- right (spelled `_STL::vector<long>::vector<int>(int, int, const allocator&)`).
- 0x004ABD90, 0x0076AE90, 0x007817B0, 0x00A43A00, 0x00AF1D90 (`..._Destroy`) -> STLport `__destroy_aux` / `_Destroy` over
  CTestQuest, ParticleAndEndPoints, NFire::CFirePoint, NGameText::CDataBankTextEntry, NParticleEngine::CParticleEmitterInfo.
- 0x004D0420, 0x005E0F60, 0x006FBD10, 0x0077EA10, 0x009B2AC0, 0x009B5A20, 0x00A407C0, 0x00A40FF0, 0x00A710F0
  (`vector<pair<CCharString>>::_Destroy`) -> STLport `__unguarded_insertion_sort_aux` (element types above).
- 0x00639810, 0x0066E6D0, 0x006C9D50, 0x006DD4A0, 0x00786410, 0x007D9FC0, 0x0081DF50, 0x0086DCB0, 0x0086EA80,
  0x0098DA90 (`vector<pair<CCharString>>::resize`) and 0x00A01820, 0x00A3D570, 0x00A43FF0, 0x00AD7350, 0x00B6CC50,
  0x00F38640 (`vector<EWeaponClass>::_Umove`) -> STLport `vector<T>::resize` of the element types above.
- 0x008883C0 (`vector<long>::vector<int>`) -> NEnvironmentInternal::CEnvironmentThemePriorityStack::CEnvironmentThemePriorityStack(long).
- 0x0051F040 (`vector<bool>::push_back`) -> CThingManager::GetNameForThing_OnlyCalledFromTCHasName(unsigned __int64).
- 0x0099CC40 (`vector<bool>`) -> CWideStringData::CWideStringData(const wchar_t*, long); 0x0099CA40 / 0x00568CE0
  (`vector<long>::vector<int>`) -> STLport basic_string<unsigned short>(s, n, a) / basic_string<char> copy constructor.
- 0x00A21F50 (`vector<bool>`) -> CSBCCorridor::CSBCCorridor(); 0x00A0C830 (`_Umove`) -> CCamera::SetAngleXZ(float).
- 0x00AD62E0 (`vector<char>::_Ufill`) -> STLport `__uninitialized_fill_n<CUniqueVertex*, unsigned int, CUniqueVertex>`.
- 0x00CDD0A0 (`_Umove`) -> CPersistContext_TransferableVector<bool>::Resize(long) (parked).
- Already landed: 0x0051F010 `CKeyedSet_0051f010::Contains(unsigned long, unsigned long)` is
  CThingManager::DoesNPCNameEntryExistForThing(unsigned __int64) (PDB name; one 8-byte UID, not key + unused word).
- Coverage names of callees: 0x0099AF70 (`std::_Iterator_base12::_Getcont`) is CWideString::operator<, 0x0099B510
  (`~CCharString`) is ~CWideString, 0x0099EAE0 (`_Cons_val<...>`) is ~CCharString.

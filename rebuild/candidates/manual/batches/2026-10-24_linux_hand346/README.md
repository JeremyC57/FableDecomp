# Hand chunk 346 (SplineDataList .. _Adjust_heap<pair<float,EConversationAttitude>>): 37 of 40 byte-exact

## What matched (37)
- **Dinkumware `_Adjust_heap` (18 rows: 004cbed0, 004d04f0, 00680750, 00a20750, 00a5a580, 00ad1dd0, 0087de50, 00807520, 00497160, 009b5c70, 00a408e0, 00a6fb40, 0071f650, 0086f450, 00adb000, 0081aa50, 0086b070, 004d0590)** -- one body for all: `top = hole; idx = 2*hole+2; for (; idx < bottom; idx = 2*idx+2) { if (pred(first[idx], first[idx-1])) --idx; first[hole] = first[idx], hole = idx; } if (idx == bottom) { first[hole] = first[idx-1]; hole = idx-1; } _Push_heap(...)`, `__fastcall` free function, `// Flags: /Gr`. The bottom case must use `idx - 1`, not `bottom - 1` (register choice). What varies is the element model:
  - by-value slot built word by word through `mov eax,esp` = a user copy constructor on the element (pair-level for 0071f650/0086f450/00adb000/00680750; trivial copy = plain `push`es for 00ad1dd0).
  - `mov eax,esp ; add eax,4 ; mov [eax],..` (0081aa50, 0086b070) = the SECOND member's inline copy ctor written as `{ *this = other; }`; a member-initialiser list folds the +4 into each store.
  - CCharString keys (009b5c70/00a408e0) use the inline null-checking `operator<` of the landed 0x00CB68C0; 00a6fb40 calls the out-of-line `FastCompare`; 00497160 is `_wcsicmp(a,b) < 0` (include `<string.h>`: `wchar_t` is not a keyword in the lane).
  - 0087de50 reproduces a retail bug: `CTurningAnimCompare` calls `GetRotationInAnimation` twice on the FIRST argument (always-false compare).
  - 00807520's coin flip is `const unsigned long& bit = Random() & 1; return bit == 0;` -- the reference-bound temporary is retail's second frame dword.
- **SGI `__adjust_heap` / `__pop_heap` (0071ff00, 004dfdbb, 00af3430)** -- `secondChild = 2*(secondChild+1)`. 0071ff00 needs the same-TU noinline CCompositeBlendDesc copy ctor of the landed 0x00720610; 004dfdbb is a /Os TU (`#pragma optimize("s", on)`, ebp frame comes with it) with out-of-line pair copy/assignment and `__forceinline` comparator + CCharString::operator<; 00af3430 is the `*result = *first; __adjust_heap(first, 0, last-first, value, comp)` front end of 0x00AF3290.
- **std::map<void*, CBulletTimeInfo>::operator[] (004c6e00)** -- STLport tree, inline lower_bound; the insert result must go through a named local (`SBulletTimeIterator inserted = insert_unique(...); i = inserted;`) for retail's duplicated return tail and frame re-read.
- **vector<T>::resize(n) (004eda32 CPrizeScoreDef, 004edf0a CMultiStaticMeshEntryDef)** -- landed 0x0067D610 model under `optimize("y",off)` + `("s",on)`: the out-of-line CSubDefClassBase ctor 0x0044C0C0, implicit derived dtors, inline CDefPointeeBase dtor, out-of-line ~CResource. The element size (0x30/0x38) needs every PDB member (LightingChannel +0x34).
- **CPersistContext::TransferVectorOfSubComponents<NSpeechGainManager::CDialogueLayerDef> (00472a8d)** -- landed 0x007D2740 body verbatim, 0x40-byte element, `y` off + `s` on.
- **Destructors** -- 00742fd0 ~CModeDragonFlyingBase (`__declspec(novtable)`, user empty dtor; CArray member frees, inline ~CModeBase, filter's implicit dtor then ~CAnimFilterBase `{}`), 009fc5c0 ~CResourceList (user dtors at all four levels; inline ~CResource calls RemoveResource), 00c14490/00c29af0 (landed CBuffer twins), 009cbee0 (`delete[]` + null inside the test; declare `operator delete[]`).
- **CHandleBasedMemoryPool (00c27e60 ReleaseBuffer, 00c27f50 RemoveFreeArea)** -- engine header + `_Methods` struct; pool free is a `__fastcall` 0x00C16680.
- **CMediaType::ResetFormatBuffer (00cb19c0)** -- strmbase source verbatim (CoTaskMemFree import).
- **StreamReader::AdvancePositionRelative (00c426f0)** -- `offset += m_position` (a separate local swaps eax/edx).
- **TeleportData (00d15170 operator=, 00d18c50 copy ctor, 00f0ca60 32-byte copy ctor)** -- implicit members emitted by a one-line `a = b` / placement-new helper; leaf `??4TeleportData` / `??0TeleportData`.

## Parked for Fable 5.1 (3)
- 00978b90 _Adjust_heap<CThing**,...,CThingCompare_InDirectionOfAndNearestToHeuristicBitch> DIFFER(142v142): comparator copy into the _Push_heap argument area uses `mov ecx,esp` with edx/eax temps where retail uses `mov eax,esp` with ecx/edx temps (register rotation only) | tried: four floats, two C2DVector members, user memberwise copy ctor (floats / C2DVector), C2DVector user copy ctor (the copy-ctor forms get eax but load only one word before `sub esp,10h`) | next: lane gap: register allocation (scheduling-only)
- 0097af70 _Adjust_heap<CThing**,...> (byte twin of 00978b90 in another TU, callees 0x0097ABF0 / 0x0097ADF0) DIFFER(142v142): same register rotation | tried: the 00978b90 model | next: lane gap: register allocation (scheduling-only)
- 005e1210 _Adjust_heap<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase>>*> DIFFER(221v221): esi/edi swapped throughout (retail first=esi, idx=edi and the cached by-value pointer in edi) | tried: inline release in operator=, landed 0x00BBC310 form (cached `old` + pointee Release()), long refcount + explicit dtor | next: lane gap: register allocation (scheduling-only); maybe a CIVCountedPointer with a separate non-member release helper

## Skipped
- none. All 40 rows are single functions with correct bounds (004cbed0's catalog length 112 includes 6 int3 bytes; the body is 106).

## Label corrections
- 004c6e00 `SplineDataList::FindOrInsert` -> `std::map<void*, CBulletTimeManager::CBulletTimeInfo>::operator[]` (only caller CBulletTimeManager::SetBulletTimeSlowdownMultiplier 0x004C6170); 0x004C6BC0 `SplineDataList::FindAndInsert` -> `_Rb_tree::insert_unique(iterator, const value_type&)`.
- 009cbee0 `~Stack<unsigned long,64>` -> unattributed buffer destructor (no callers); PDB Stack<unsigned long,64> is an empty 1-byte struct, so every `Stack<...>` label in this chunk is propagated.
- 00742fd0 -> `NCreatureMode::CModeDragonFlyingBase::~CModeDragonFlyingBase` (called by CModeDragonStrafe's deleting dtor 0x00743960).
- 009fc5c0 -> `CResourceList::~CResourceList`; 0x009FC020 is CResourceList::RemoveResource (already corrected in its landed source).
- 00c14490 / 00c29af0 -> CBuffer family (vector `_Tidy` shape), as the landed twins.
- 00c27e60 -> `CHandleBasedMemoryPool::ReleaseBuffer` (descriptive; the landed ~CHandleBasedMemoryPool 0x00C287E0 calls it "Cleanup"); 00c27f50 -> `CHandleBasedMemoryPool::RemoveFreeArea`; 0x00C16680 is the engine free (`__fastcall`), not ~CTimer.
- 00cb19c0 -> `CMediaType::ResetFormatBuffer`.
- 00c426f0 keeps `StreamReader::AdvancePositionRelative` (class unconfirmed; caller is the RIFF chunk walker 0x00C427D0).
- 00af3430 `TeleportData::operator=` -> SGI `__pop_heap<pair<CCharString,NParticleEngine::CRegisteredPSCInfo>>`.
- 00d18c50 `TeleportData::operator=` -> `TeleportData::TeleportData(const TeleportData&)`; 00f0ca60 is the copy ctor of the 32-byte TeleportData (vector itemsCarried).
- 004eda32 / 004edf0a `~WaypointData` -> `vector<CPrizeScoreDef>::resize(n)` / `vector<CMultiStaticMeshEntryDef>::resize(n)`; their callees 0x004EDA6C / 0x004EDF44 (`CArray<pair<ulong,CSoundPair>>::push_back`) are `resize(n, const T&)`.
- 00a5a580 -> `_Adjust_heap<float*, less<float>>`; 00adb000 -> `_Adjust_heap<CVertexGroupBlockOptimiser::CBlockInfo*>`; 0086b070 -> `_Adjust_heap<pair<long,CRandomAppearanceMorph::CSkeletalMorph>*>`; 004cbed0 / 00680750 compare a signed long key, not a float.
- 0x0099AF10 (label CCharString::operator const char*) is also CWideString::operator const wchar_t* (byte twin); 0x0099EAE0 (label `_Cons_val<...>`) is ~CCharString.

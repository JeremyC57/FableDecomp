# Hand chunk 353 (_Dest_val<std::allocator<std::pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase>>>> .. _Destroy_range<std::allocator<std::...>>): 40 of 40 byte-exact

Every catalog label in this chunk is a propagated `_Dest_val` / `_Destroy_range` name. Each row was identified from
callers, vtable slots, Ego_r.pdb names and layouts, and its source header states the real function and the evidence.
The grades are 1 MATCH (009b31a0) and 39 RELOCATION_MATCH. All 40 tests pass under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (40)

**STLport range destroy (`_Destroy` / `__destroy_aux` / `_Destroy_range`, 14 rows)**
- 00518b80 `_Destroy(CLevelFiles*, CLevelFiles*)`, 006c3510 / 006e5360 (CCharString-led 20- and 12-byte records),
  009f6720 (52-byte records; the dtor 0xA03B30 is an ICF-folded empty `ret`), 005684a0 (pair<long, CWideString>).
  These are LTCG `__fastcall` loops with an explicit `first->~T()`, which give the guarded do-while with `last` in edi.
- 00515250 `_Destroy_range(CMapInfo*, CMapInfo*, alloc)`. The full Ego_r CMapInfo layout lets the implicit dtor
  inline: the counted map is released, then the three CCharStrings, with the element pointer strength-reduced to `&PMap.Block`.
- 009b8070 (pair<unsigned long, CDefinitionManager::CDefClassInfo>) and 00a5e900 (pair<unsigned long, CBaseClass-derived
  CArray holder>) match only with the real STLport vector structure: `_Vector_base` with the end pointer in
  `_STLP_alloc_proxy`, a trivial `_Destroy(start, finish)`, `deallocate(p, n)` and `CArray<T> : vector<T>`. A flat
  three-pointer vector loses the strength-reduced element pointer (52v55).
- 006b6ff0 `__destroy_aux(vector<SThemeBlock>*...)`: the inner `_Destroy` passes a NAMED `__false_type` local to the
  out-of-line element `__destroy_aux` 0x6B67A0.
- 00a37af0 {list<long>, 3 longs, CCharString}: STLport `_List_base` with an inline clear().
- 00b2efb0 CGraphicFrame and 009c6d60 CFileInstaller::CBufferChunk reuse the landed `__forceinline` destructors and
  GFFreeFromGlobalAllocator unchanged.
- 009b31a0 `ReleaseCountedPointerAt0C`: a __fastcall `record->~SCountedRecord10()` (CIVCountedPointer at +0xc).
- 00c91320 `FreeBufferHeader`: `free(p->data); memset(p, 0, 20)`. The memset schedules the `add esp,4` among the stores;
  explicit stores schedule it first.

**STLport insertion sorts (10 rows)**
- `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)` calling `__unguarded_linear_insert(i, _Tp(*i), comp)`:
  004ba100 (pair<CCharString, pair<CCharString, CCharString>>; a flat 3-string struct gives 79v81), 009e6d10
  (pair<CCharString, CSymbolInfo>), 00cbb2d0 (pair<CCharString, CCountedPointer<T>>, copy adds a reference), 00807320 /
  009267a0 (CIntelligentPointer<CThing> / <const CThing>; copy = base ctor, vtable store, SetPItem(GetPItem());
  009267a0's comparator is a function pointer passed in edx), and 007fa290 (CThing* with the 52-byte
  CThingCompare_BestForTargeting2, which needs `C3DVector`'s user copy ctor or the comparator copy becomes `rep movsd`).
- 005f85ab / 005f85f1 are twins over pair<long, long>, built under `#pragma optimize("s", on)`.
- `__insertion_sort(first, last, comp)` calling `__linear_insert(first, i, _Tp(*i), comp)`: 004e3686 and 00594b59,
  both under `optimize("s")` (jump-to-bottom loop test, `push ecx ; push ecx` argument slot), each with its
  out-of-line value copy ctor.

**Other STL**
- 004b9f10 `_Rb_tree<CCharString,...>::insert_unique(const CCharString* first, const CCharString* last)`: a member
  template, explicitly instantiated. pair<iterator, bool> and the iterator need user ctors, so the discarded result
  comes back through the 8-byte frame slot.
- 007f8760 `std::for_each(PThing*, PThing*, SKillThing)` over CTCParticleAttacher::ParticleList: a /Gr `__fastcall`
  template with the hidden result pointer in ecx. The functor derives from std::unary_function (non-POD), so it
  returns through that pointer.

**Classes**
- CPlayer: 00633670 UninitModeClickPastText and 00633bb0 UninitModeOracleMinigame. These use the engine/CPlayer.h
  carrier and two RemoveFromInterface calls in member order.
- 00768310 CTCObjectAugmentations::OnSetInLimbo: two member calls with `this` kept in esi.
- 009439d0 ~CAIFilter_ShouldStillBeWorkingInWarehouse and 00bc5f50 ~CEngineInternalPrimitiveChanger: `novtable`
  derived classes with empty user dtors, so only the base's inline dtor stores a vtable.
- 0099a3c0 CBaseObject::ClearIntelligentPointers (engine/CBaseObject.h carrier).
- 00a2cfe0 ~CLODStateBlock: an empty out-of-line dtor; the counted pointer is released, then the STLport map clear().
- 00a45410 C3DSkeletalMorph::Reset: the VC6-era `erase(begin(), end())`. The counted-pointer Clear nulls Info BEFORE Data.
- 00add330 CLHFile::Close: `m_data.Reset()` (block word first), then `delete this`.
- 00996b60 CMemoryDataOutputStream::Clear: the retail opt-in engine header plus the STLport vector `Clear()`:
  `if (capacity()) { clear(); swap(empty); }`.
- 008f8030 LHNetworkLib::LTimer::MSeconds: `(unsigned long)((float)(GetTickCount() - SystemClockAtLastAdjust) *
  SpeedUpFactor + (float)TimerTimeAtLastAdjust)`, with __ftol2 tail-jumped.
- 00a3a180 C2DBoxI::GetDistance: the clamp must test the LOCAL copy (`nearest.X < TLX`); testing `pos.X` re-reads the
  reference and duplicates the tail (108v87).
- 00ae3a30 CParticleProgram::RemapBoneIndices: a plain wrapper over the private static `__fastcall` helper 0xAE3920.
- 00ae15d0 CParticleProgram::GetUsedBones: retail reuses ecx across the first call. The fix is to compile the static
  helper 0xAE1210's body (written from its retail bytes) above it under `#pragma auto_inline(off)`; without it the
  result is 39v33.

## Parked for Fable 5.1 (0)

## Skipped
None. Every row is one function, and the scanner-verified extents agree with the bodies: every row ends at its
`ret` (008f8030 at its tail `jmp __ftol2`) followed only by int3 padding.

## Label corrections
Row labels (all 40 propagated):
- `_Dest_val<..., pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>>`: 0099a3c0 CBaseObject::ClearIntelligentPointers,
  009b31a0 a fastcall record destructor (class unidentified), 00bc5f50 CEngineInternalPrimitiveChanger::~CEngineInternalPrimitiveChanger.
- `_Dest_val<..., pair<long, CMusicManager::CRegisteredMusicEntry>>`: 00633670 / 00633bb0 CPlayer::UninitMode*,
  00768310 CTCObjectAugmentations::OnSetInLimbo, 008f8030 LHNetworkLib::LTimer::MSeconds, 009439d0
  ~CAIFilter_ShouldStillBeWorkingInWarehouse, 00a3a180 C2DBoxI::GetDistance, 00ae15d0 CParticleProgram::GetUsedBones,
  00ae3a30 CParticleProgram::RemapBoneIndices.
- `_Dest_val<..., pair<pair<long,long>, CEngineTextureDecal>>`: 00996b60 CMemoryDataOutputStream::Clear.
- `_Dest_val<..., pair<unsigned long, CCountedPointer<CFaction>>>`: 00a2cfe0 ~CLODStateBlock, 00a45410 C3DSkeletalMorph::Reset,
  00add330 CLHFile::Close (name inferred from slot 3 of vtable 0x129F094).
- `_Dest_val<..., pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32>>>`: 00c91320 FreeBufferHeader (C library).
- `_Destroy_range<std::allocator<CSpeechFrame>>` (21 rows) and the `_Destroy_range<std::allocator<std::...>>` /
  `<CFileInstaller...>` rows: the range destroys, insertion sorts, insert_unique and for_each listed above.
  Their real element types are in each header.

Callee labels found wrong while identifying the rows:
- 0x00806910 / 0x009265C0 "_Push_heap<...>" = `__unguarded_linear_insert` (the template arguments in the label are right).
- 0x006B67A0 "_Copy_backward" = `__destroy_aux<SThemeBlock>`.
- 0x0043020A "LTextTreeWalkThrough::BuildTreeArray" = `map<CCharString, long>::_M_erase`.
- 0x0099A2E0 = CBaseClass::~CBaseClass (vtable 0x01231710). 0x00A03B30 "$E2" = an ICF-folded empty destructor.
- 0x004AC380 "StdSet_Insert" = `_Rb_tree<CCharString, ...>::insert_unique(const CCharString&)` (set<CCharString>).
  0x004BA910 = set<CCharString>::insert(first, last), a tail jump to 0x004B9F10.
- 0x00AE1210 "GetUsedBones" (_global) = private static `CParticleProgram::GetUsedBones(CArray<bool>&, const CArray<C3DGroup2>&)`.
  0x00AE3920 "InsertFromVector" = private static `CParticleProgram::RemapBoneIndices(map&, CArray<C3DGroup2>&)`.
- 0x00AC3AE0 / 0x00AC3AF0 "_Cons_val<..., CParticleMorphs::CEntry>" are forwarding thunks (`mov ecx,[ecx+4] ; jmp`)
  to 0x00AE15D0 / 0x00AE3A30.
- LTimer cluster (Ego_r LTimer layout, GetTickCount import 0x0143FDFC):
  - 0x008FA000 "CAIStateGroup_MinionHasInformation::OnActivate" = LTimer::LTimer() (factor 1.0f, built on LSocket::Bind's stack).
  - 0x008F80D0 "CMusicManager_OnMusicEntryTimeout" = the LTimer set-speed-factor member.
  - 0x008F9CA0 "MusicTimer_CheckAndTrigger" = an LTimer member: a pause toggle through SaveSpeedUpFactor.
  - 0x008F9E60 "CTCKraken::StartHighAttackBeam" = an LTimer set-time member.
  - 0x008F9D00 = an LTimer member: it re-bases the clock, then restores the factor; Ego_r names `Stop` as a candidate.
- 0x007F8860 "Unwind@00fbdab0" = CTCParticleAttacher's detach path: for_each kill, then clear of ParticleList
  (likely DetachParticles).

# Hand chunk 528 (C3DMeshStats::PackNames .. GFIsPointInTriangle): 17 of 20 byte-exact

Re-queue chunk (20 rows, 241-502 bytes, 0x00A2EEF0..0x00A44420). Every row graded RELOCATION_MATCH under RTM 13.10.3077
(gall.sh, 17/17), tests PASS under g++ (tall.sh) and VC7.1 (tall.py), vland dry run 17 WINS with no RELOC_CONFLICT /
RELOC_DUPLICATE / RELOC_IDENTITY. No retail_asm rows.

## What matched (17)
- **STLport vector growth / insert (9 rows)**: `_M_insert_overflow` 0x00A33C50 (C3DGeneratorObject), 0x00A33E80
  (pair<unsigned long, C3DDummyObject>), 0x00A34F00 (pair<unsigned long, C3DMeshVolume>), 0x00A3D060
  (C3DAnimationStateAlpha); `_M_fill_insert` 0x00A33F80, 0x00A35010, 0x00A37F40 (CClassTypeInfo), 0x00A3D190;
  `_M_range_insert<pair*>` 0x00A39610 (pair<unsigned long, long>). Shape lesson: give every helper COMDAT that retail
  emitted in the same TU (`__uninitialized_copy`, `__uninitialized_fill_n` / `uninitialized_fill_n`, `__copy_backward`,
  `fill`, `copy`, `__destroy_aux`) a same-TU `#pragma auto_inline(off)` + `__declspec(noinline)` body. VC7.1 then homes
  new_start in the dead fill_len slot, puts the tags in the atend/x slots and tail-duplicates the epilogues, as retail
  does. Element details: C3DAnimationStateAlpha needs a user inline empty virtual `~CIVCountedPointeeBase() {}` (so
  x_copy needs no dtor call); pair<ulong, C3DMeshVolume> needs CCountedPointer's inline release so `~pair` 0x00A326D0
  stays out of line.
- **STLport sort family over pair<unsigned long, T> (2 rows)**: `__introsort_loop` 0x00A34B00 (C3DHelperPoint) and
  0x00A34C00 (C3DDummyObject), _algo.c verbatim with CKeyPairCompareLess<K,V> as a global class template.
- **STLport sort family over CSoundManagerLH's NamePairMap entry pair<pair<CCharString, CCharString>, CSoundBankLH*>
  (4 rows)**: `__push_heap` 0x00A409A0, `__unguarded_partition` 0x00A40DF0, `__adjust_heap` 0x00A410A0,
  `__introsort_loop` 0x00A41EA0. The compare is CKeyPairCompareLess -> STLport's inline `__cdecl` pair operator<. VC7.1
  inlines it everywhere except in the inlined `__median`, where it inlines only the first compare and calls the
  out-of-line instance 0x00A40250 for the rest, as retail does. CBasicString<char>::operator< 0x00429950 is a same-TU
  noinline body (GOTCHAS rule).
- **CSoundManagerLH::AddBank 0x00A3F140** (RTTI vtable 0x0129D2C4 slot 37): two separate `return false` tests (retail
  duplicates the epilogue), inline FindFirstSetBit(mask, 16), then the CVectorMap Add (push_back of a temporary pair)
  and the sort-when-dirty. Add and push_back must be `__forceinline`: VC7.1 will not inline them by itself here.
- **C3DMeshStats::PackNames 0x00A2EEF0**: two set<CCharString> walks with the static `_Rb_global<bool>::_M_increment`,
  GetLength() != 0 filter, terminators as the implicit `CCharString("")` conversion (an explicit `CCharString("")`
  temporary passes the ctor's eax instead of re-forming the address), and the inline STLport resize/erase
  (`__copy_trivial`) plus CArray::GetData and the memcpy intrinsic.

## Parked for Fable 5.1 (3)
- 00a3bf10 C3DAnimationBlendState::CreateInterpolate DIFFER(432v432): in the transition arm retail runs `mov ecx,[esi+4] ; fstp st(0) ; inc ecx`, ours runs `mov ecx,[esi+4] ; inc ecx ; fstp st(0)` (scheduling only) | tried: Ego_r CIVCountedPointer member set (adopting raw ctor, template converting ctor, Release / operator= / AddRef forms), pointee Release with Delete inside (fixed the temporaries' release), local / inline-Lerp / reassociated lerp, named result, /G5 /G6 /G7, QFE 4035 | next: lane gap: x87 scheduling
- 00a43cd0 std::vector<CLipSyncDataBankEntry>::_M_fill_insert DIFFER(300v288): x_copy's CIVCountedPointer<CData> release is retail `dec dword [ecx+4] ; jne ; mov edx,[ecx] ; call [edx+4]`, ours is `mov esi,[eax+4] ; add eax,4 ; dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi` | tried: local copies, mutable/long count, Release-returns-count, Release-with-Delete, CIVCountedPointer::Release member, explicit `--object->IVRefCount == 0` dtor | next: lane gap: register allocation (read-modify-write vs load/decrement/store); the same release appears as RMW in operator= (0xA3BF10) and as load/dec/store in the dtor there, so the element's dtor form is the open question
- 00a44420 GFIsPointInTriangle DIFFER(414v424): each edge test's dot product runs x*nx, z*nz, y*ny in retail, while every source form gives z, y, x (plus 10 bytes of operand reloads) | tried: all dot orders, GFDotProduct (FableWin ?GFDotProduct@@YIMABVC3DVector@@0@Z), member operator* and operator^, const returns, cross locals, ctor forms, QFE, /Op | next: C3DVector's real inline operator set from the FableWin header order

## Skipped
- none (no row covers two functions).

## Names
### Proven
- 00a2eef0 `?PackNames@C3DMeshStats@@AAEXAAV?$CArray@D@@ABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@1@Z`. Evidence: Ego_r name set (exact frame: private, CArray<char>& plus two const set<CCharString>&, `ret 0Ch`); caller 0xA2F740 is the C3DMeshStats file-data initialiser; the output format is what the landed UnpackNames 0xA2ED10 reads.
- 00a3bf10 `?CreateInterpolate@C3DAnimationBlendState@@CI?AV?$CIVCountedPointer@VC3DAnimationStateBase@@@@ABVC3DAnimationStateBase@@0M@Z` (parked). Evidence: Ego_r 0xB96750 (private static fastcall, hidden return); callers are itself and SetInterpolate 0xA3C3B0; the arms build C3DAnimationTransitionState (vtable 0x01265144) and C3DAnimationState (0x01249F50).
- 00a44420 `?GFIsPointInTriangle@@YI_NABVC3DVector@@0000@Z` (parked). Evidence: Ego_r 0xB99B80 and FableWin 0x320CC20, in donor order after the two GFGetShortestDistance*SquaredBetweenPointAndLine (retail 0xA442E0 / 0xA443A0).
- 00a34b00 / 00a34c00 `std::__introsort_loop<pair<unsigned long, C3DHelperPoint / C3DDummyObject>*, .., int, CKeyPairCompareLess<..> >`. Evidence: 20- and 56-byte strides; callees on the same instantiation (__partial_sort 0xA342F0 / 0xA34360, __unguarded_partition 0xA329B0 / 0xA32A70); callers are the sorts 0xA35150 / 0xA351B0; Ego_r names the CKeyPairCompareLess<unsigned long, C3DHelperPoint> heap family.
- 00a409a0 / 00a40df0 / 00a410a0 / 00a41ea0 `std::__push_heap / __unguarded_partition / __adjust_heap / __introsort_loop <pair<pair<CCharString, CCharString>, CSoundBankLH*>*, .., CKeyPairCompareLess<..> >`. Evidence: the chain introsort 0xA41EA0 -> partition 0xA40DF0, pop/make_heap 0xA41630 / 0xA416E0 -> adjust_heap 0xA410A0 -> push_heap 0xA409A0, all under sort 0xA42060, which CSoundManagerLH's commit 0xA3F368 calls on NamePairMap (+0x54). The element is the one the landed FindByNamePair 0xA3EE50, push_back 0xA41840 and _M_insert_overflow 0xA41390 use; STLport 4.6 _heap.c/_algo.c text. (CSoundBankLH itself is the landed descriptive name.)
- 00a33c50 / 00a33f80 `vector<C3DGeneratorObject>::_M_insert_overflow / _M_fill_insert`. Evidence: 0x40 stride with CCharString at +0x34 (Ego_r C3DGeneratorObject: C3DDummyObject + ObjectName / BankIndex / UseLocalOrigin, 0x40 bytes); the CCharString::operator= at +0x34 in its __copy_backward 0xA31060.
- 00a33e80 / 00a34f00 / 00a35010 `vector<pair<unsigned long, C3DDummyObject / C3DMeshVolume>>`. Evidence: the C3DMeshStats CVectorMap members (Ego_r) plus the strides and element bodies.
- 00a37f40 `vector<CClassTypeInfo>::_M_fill_insert`. Evidence: the copy ctor 0xA37B50 is bound `??0CClassTypeInfo@@QAE@ABV0@@Z`; the 0x14 layout matches Ego_r CClassTypeInfo (list<CClass*>, NoAllocated, Used, Local, AllocFunc, Name).
- 00a39610 `vector<pair<unsigned long, long>>::_M_range_insert<pair*>`. Evidence: every __uninitialized_copy call goes to the single COMDAT 0x9B25C0, bound <pair<ulong,long>*, pair<ulong,long>*>; forward_iterator_tag arm of STLport 4.6.
- 00a3d060 / 00a3d190 `vector<C3DAnimationStateAlpha>`. Evidence: the vtable 0x01265128 store and the AlphaWeight +0x10 (Ego_r layout); the C3DAnimationBlendState CombinationBlends array.
- 00a43cd0 `vector<CLipSyncDataBankEntry>::_M_fill_insert` (parked). Evidence: resize 0xA43F20 <- CLipSyncDataBank::NotifyEntryAdded 0xA42DF0 / NotifyBankSize 0xA42E70; Ego_r CLipSyncDataBank::Entries is CArray<CLipSyncDataBankEntry>.
### Probable
- none.
### Descriptive
- 00a3f140 `CSoundManagerLH::AddBank(const CCharString&, unsigned long)`. The class comes from RTTI (vtable 0x0129D2C4, slot 37), but the audio layer is in neither PDB: Ego_r, Ego_d and FableWin hold no CSoundManagerLH / CSoundBankLH name, and the FSE manifest has none either. The method, the +0x40 device (slots 3 / 14), the bank's slot 5 and its slot 0 are named after what they do, as the landed FindByName / FindByNamePair rows are.

## Label corrections
- 00a2eef0 "DoublyLinkedList_MergeToString" -> C3DMeshStats::PackNames.
- 00a33c50 "Vector_Reallocate_C3DGeneratorObject_RangeShift" -> vector<C3DGeneratorObject>::_M_insert_overflow.
- 00a33e80 "Vector_Reallocate_CShopItem_0x38" (propagated) -> vector<pair<unsigned long, C3DDummyObject>>::_M_insert_overflow.
- 00a33f80 "Vector_InsertElements" -> vector<C3DGeneratorObject>::_M_fill_insert.
- 00a34b00 "Introsort_Partition" / 00a34c00 "Partition_With_Swap" -> std::__introsort_loop.
- 00a34f00 "Vector_Reallocate_Pair" / 00a35010 "Vector_Insert_Pair_16" -> the pair<unsigned long, C3DMeshVolume> vector members.
- 00a37f40 "Vector_Insert_CCharStringPair" -> vector<CClassTypeInfo>::_M_fill_insert.
- 00a39610 "Vector_Insert_Pair_Float_Attitude" (propagated) -> vector<pair<unsigned long, long>>::_M_range_insert.
- 00a3bf10 "CreateInterpolate" (right leaf, wrong module `_global`) -> C3DAnimationBlendState.
- 00a3d060 "Vector_Reallocate_CDisplayModeDesc" / 00a3d190 "Vector_Insert_CDisplayModeDesc" (propagated) -> vector<C3DAnimationStateAlpha>.
- 00a3f140 "CParticleMorphs_AddMorphEntry" (propagated) -> CSoundManagerLH vtable slot 37.
- 00a409a0 "Heap_AdjustDown_CCharStringTriple" -> __push_heap; 00a410a0 (same label) -> __adjust_heap.
- 00a40df0 "Introsort_Partition_CSymbolInfo" -> __unguarded_partition over the NamePairMap entry, not CSymbolInfo.
- 00a41ea0 "Introsort_Quicksort_CCharStringPair" -> __introsort_loop.
- 00a43cd0 "Vector_InsertRange_CDataBankTextEntry" -> vector<CLipSyncDataBankEntry>::_M_fill_insert (the whole 0xA43500..0xA43B20 chain is landed on NGameText::CDataBankTextEntry; NAME_FIXES.tsv).
- Callee labels: 0xA41E20 "Vector_ReallocAndShrink" / GuiVarSetBool, 0xA42060 "Heap_MakePriority_12byte" / SetGuiValue, 0xA41480 "Vector_InsertOrFill", 0xA3ED30 "VectorMap_LookupByKey", 0x994B40 "GetFramesRemaining", 0x4110D0 "CString_FillRange" are all propagated or descriptive (NAME_FIXES.tsv holds the proven spellings).

## Header debt
- engine/CMemoryDataOutputStream.h / engine/CDataOutputStream.h are flat layouts without the virtual dtor / SetPosition /
  GetPosition, so 0x00A2EEF0 models the stream locally (the same debt the landed PackBoneNames 0xA8B6C0 records).
- engine/CIVCountedPointeeBase.h models the vptr as a `__vftable` member, so the vector rows (0xA3D060 / 0xA3D190) and
  0xA3BF10 / 0xA43CD0 keep local virtual classes. vland warns SHARED_HEADER_AVAILABLE for it; a header with the real
  virtual dtor / Delete (slot 1) and Ego_r's AddRef / Release would clear the warning.
- No C3DMeshStats.h is generated; PackNames does not touch `this`, so the class is only declared.

# Hand chunk 418 (_global .. _global): 37 of 40 byte-exact

All 40 rows are `_global` catalog rows with propagated names; most were re-identified (see Label corrections).
Every matched row has a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (37)

- **GUID helpers (guiddef.h), 3 rows:** 00a3aef0 `IsEqualGUID`, 00a3af10 `operator==(const GUID&, const GUID&)`,
  00a3af30 `operator!=`. These are out-of-line copies of the guiddef.h inlines in a /Gr TU, written as explicit
  `__fastcall`. `#pragma intrinsic(memcmp)` with `!memcmp(a, b, sizeof(GUID))` gives retail's inline dword compare.
- **FindFirstSetBit, 00a3f860 (descriptive name):** the lowest set bit among the first `numBytes*8` bits, or -1. The
  loop is `for (i = 0; i < numBytes * 8; ++i) if (mask & (1 << i)) return i;`, `__fastcall`.
- **DirectShow helper, 00a3b000 `RenderOutputPins(IGraphBuilder*, IBaseFilter*)`:** the DirectX SDK dshowutil body,
  verbatim. It is `__fastcall` and the COM interfaces are written out by hand (no SDK headers).
- **md5, 00a3e010 `md5_finish`:** L. Peter Deutsch's md5.c, verbatim, as `extern "C" __fastcall`, with `// Flags: /GS`.
- **C3DAnimationStateAlpha STLport helpers, 7 rows:** 00a3cb90 / 00a3cc30 `__uninitialized_copy` (non-const and const
  source), 00a3cbe0 `__uninitialized_fill_n<.., unsigned int, ..>`, 00a3cd00 `__copy_backward`, 00a3cf20 `__copy`,
  00a3cee0 `fill`, 00a3d380 `vector::push_back`. SHAPE: the model is a local polymorphic chain
  (CIVCountedPointeeBase -> C3DAnimationState -> C3DAnimationStateAlpha, 0x14 bytes), so the implicit copy
  constructor stores all three vtables. The counted copy loops unroll 4x because their bodies have no calls.
  `push_back` needs a same-TU noinline `_M_insert_overflow` stand-in so the `__false_type` tag shares x's argument slot.
- **CClassFactory / CClassTypeInfo, 3 rows:** 00a37c40 `__copy<CClassTypeInfo*>`, 00a38150
  `vector<CClassTypeInfo>::resize(n)` (= `resize(n, _Tp())`), and 00a38080 `CClassFactory` scalar deleting destructor.
  SHAPE: CClassTypeInfo is 0x14 bytes in retail: a 4-byte STLport list sentinel, NoAllocated, Used, Local, AllocFunc
  and Name. The `??_G` matched only once `~CClassFactory()` was defined OUT OF LINE in the TU. The in-class
  `virtual ~CClassFactory() {}` turned every member destructor into an out-of-line call (54v78). It also needs a
  same-TU noinline `__destroy_aux`.
- **CCRCSymbolMap, 00a38420 `FindSymbol(const CCharString&, long&) const`:** the CRC of the name goes to the inline
  CVectorMap::Find. SHAPE: a same-TU noinline LowerBound stand-in lets VC keep the key in a register as retail's LTCG
  did. Find returns end() when the bound is end() or `key < bound->first`.
- **CCompressorZlib, 00a3ad20 `Compress(const void*, long, long, CArray<unsigned char>&)`:** an `__int64` overflow
  check, then resize, slot-5 Compress, and resize again. SHAPE: the size expression is repeated inline (named size
  locals swapped esi/edi). `GetAsCArray()` is the size-ternary and `using CACompressor::Compress;` is needed.
- **CSoundManagerLH (descriptive method names), 3 rows:**
  - 00a3ecb0 `vfunc_4(const C3DVector&)`: new `CSoundEmitterLH(this, pos)`, then push_back, `back()->Start()` and return
    `back()`. It uses the STLport list model with a by-value iterator, and `back()` is `*(--end())`. The
    `if (vfunc_6()) { ...; return back(); } return 0;` form gives retail's block order.
  - 00a3ed30 `FindByName` and 00a3ee50 `FindByNamePair`: `CVectorMap::Find` with `e = Data.end()` held in a local, so
    end() is not reloaded per arm. The `pair<CCharString, CCharString>` operator< is called out of line as `__cdecl`
    (0x00A40250).
- **CSoundManagerLH map sorting (STLport 4.6 over CCharString pairs), 16 rows:**
  - 8-byte `pair<CCharString, CSoundBankLH*>` entries: 00a40370 `__unguarded_linear_insert`, 00a40660
    `__unguarded_partition`, 00a40730 `__linear_insert`, 00a40ef0 `__insertion_sort`, 00a41200 `__make_heap`,
    00a41b60 `__partial_sort`, 00a418a0 `vector::push_back`, 00a41ad0 `CVectorMap::Insert`.
  - 12-byte `pair<pair<CCharString, CCharString>, CSoundBankLH*>` entries: 00a40820 `__unguarded_linear_insert`,
    00a40af0 `CVectorMap::LowerBound`, 00a40c10 `vector::erase(first, last)`, 00a40d80 `__median`, 00a40f50
    `__linear_insert`, 00a41630 `__pop_heap`, 00a416e0 `__make_heap`, 00a41cd0 `__partial_sort`.
  - SHAPE: the STLport `_algo.c` / `_heap.c` bodies are verbatim, as `__fastcall` templates in `_STL`. The CCharString
    model has the inline operator< over `CBasicString<char>::operator<` (a same-TU noinline body), and the pair has
    STLport's explicit copy constructor. Out-of-line callees are only declared. A tag homed in a dead slot needs
    nothing (the copy_backward tag in the comparator slot, 00a40730 / 00a40f50). The tag in erase's LIVE `first` slot
    needs a same-TU noinline `__copy` (00a40c10).
  - The pair operator< is inline only where retail inlined it (00a40820, 00a40af0). `__pop_heap` is inline in the
    8-byte `__partial_sort` and out of line in the 12-byte one, from the same source.
  - Insert is `Data.push_back(value_type(key, value)); Dirty = true;`, so the temporary dies before the Dirty store.
  - LowerBound is the landed 0x00447C80 loop (`count / 2`), with `count = _M_finish - _M_start` computed BEFORE
    `first = _M_start` (216v217 the other way round).

## Parked for Fable 5.1 (3)

- 00a385a0 CCRCSymbolMap::AddSymbolDirty DIFFER(203v195): the second CRC's chars select lands in eax + `mov edx,eax` (retail preloads the "" literal straight into edx), and the pair's value word is stored before the end-of-storage `cmp` and re-read for the placement copy (retail keeps it in edx) | tried: __forceinline CVectorMap::Insert, named pair local, STLport pair copy ctor (4 grades) | next: a same-TU stand-in for Calc 0x004014A0 (retail's LTCG knows Calc writes no memory, so the value need not be re-read)
- 00a3df30 md5_append DIFFER(209v209): scheduling only; at the first md5_process call retail emits `lea ecx,[ebx+18h]` before `push ebx` (ours pushes first) | tried: md5_process as a __thiscall member of a block type, named block local (2 grades) | next: lane gap: ltcg (md5_process uses an LTCG custom convention, data in ecx and pms on the stack)
- 00a3f340 CSoundManagerLH::vfunc_41 DIFFER(183v184): the registers are swapped in the inline NameMap erase. Retail keeps the copy source (end) in edi and the result in ebp, then re-reads `[esi+68h]` for the final sort; ours has begin in edi and end in ebp, and forwards edi | tried: template __copy_ptrs, named first/last locals, erase-argument probe (4 grades) | next: give the erase's `__copy` a same-TU noinline body (the live-slot tag lesson that fixed 00a40c10) or make vector::erase a same-TU member

## Skipped

None.

## Label corrections

All catalog names in this chunk were propagated `_global` labels. Where no PDB name exists, the new name is a
**descriptive name**.

- 00a37c40 `_Move<CLipSyncPhoneme*,CLipSyncPhoneme*>` -> `_STL::__copy<CClassTypeInfo*, CClassTypeInfo*, int>`
- 00a38080 `CGuiControlTreePane_Destructor` -> `CClassFactory::`scalar deleting destructor'` (vtable 0x0129CF70)
- 00a38150 `CTCInventoryMap_InitMiniMapMarkers` -> `_STL::vector<CClassTypeInfo>::resize(unsigned int)`
- 00a38420 `CVectorMap_LookupByCRC` -> `CCRCSymbolMap::FindSymbol(const CCharString&, long&) const` (FableWin.pdb name)
- 00a385a0 `EnumValue_InsertIntoMap` -> `CCRCSymbolMap::AddSymbolDirty(const CCharString&, long)` (FableWin.pdb name)
- 00a3ad20 `TextureConvert_ResizeBuffer` -> `CCompressorZlib::Compress(const void*, long, long, CArray<unsigned char>&)` (PDB name)
- 00a3aef0 / 00a3af10 / 00a3af30 `Compare_4Ints_Equal` / `_Equal_v2` / `_NotEqual` -> `IsEqualGUID` / GUID `operator==` / GUID `operator!=`
- 00a3cb90 / 00a3cc30 `_Copy_backward<CDisplayModeDesc*>` -> `_STL::__uninitialized_copy<C3DAnimationStateAlpha*>` / `<const C3DAnimationStateAlpha*>`
- 00a3cbe0 `fill<CTextureManagerPoolInfo*>` -> `_STL::__uninitialized_fill_n<C3DAnimationStateAlpha*, unsigned int, C3DAnimationStateAlpha>`
- 00a3cd00 `Vector_Insert_CDisplayModeDesc_Bulk` -> `_STL::__copy_backward<C3DAnimationStateAlpha*, C3DAnimationStateAlpha*, int>`
- 00a3cee0 `fill<CTextureManagerPoolInfo*>` -> `_STL::fill<C3DAnimationStateAlpha*, C3DAnimationStateAlpha>`
- 00a3cf20 `Vector_Erase_CDisplayModeDesc_Bulk` -> `_STL::__copy<C3DAnimationStateAlpha*, C3DAnimationStateAlpha*, int>`
- 00a3d380 `Vector_AddCombinationEntry` -> `_STL::vector<C3DAnimationStateAlpha>::push_back`
- 00a3df30 / 00a3e010 `md5_append` / `md5_finish`: correct
- 00a3ecb0 `PrimitiveStats_EnumerateAndInsert` -> `CSoundManagerLH::vfunc_4` (descriptive name, slot 4 of vtable 0x0129D2C4)
- 00a3ed30 `VectorMap_LookupByKey` -> `CSoundManagerLH::FindByName` (descriptive name)
- 00a3ee50 `Map_Insert_Sorted_CCharStringPair` -> `CSoundManagerLH::FindByNamePair` (descriptive name)
- 00a3f340 `CParticleMorphs_CommitChanges` -> `CSoundManagerLH::vfunc_41` (descriptive name)
- 00a3f860 `FindFirstSetBit`: kept (descriptive name)
- 00a40370 `Vector_Insert_CCharStringPair` -> `_STL::__unguarded_linear_insert<SNameEntry*, SNameEntry, CNameLess>`
- 00a40660 `Introsort_Partition_Pair` -> `_STL::__unguarded_partition<SNameEntry*, SNameEntry, CNameLess>`
- 00a40730 `Vector_Insert_Sorted_CCharStringPair` -> `_STL::__linear_insert<SNameEntry*, SNameEntry, CNameLess>`
- 00a40820 `Vector_Insert_Triple_CCharString` -> `_STL::__unguarded_linear_insert<SNamePairEntry*, SNamePairEntry, CNamePairLess>`
- 00a40af0 `Map_FindUpper_CCharStringPair` -> `CVectorMap<pair<CCharString, CCharString>, CSoundBankLH*>::LowerBound` (a lower bound, not an upper bound)
- 00a40c10 `Vector_Copy_CCharStringPair` -> `_STL::vector<SNamePairEntry>::erase(iterator, iterator)`
- 00a40d80 `Med3Comparator_CCharStringPairLess` -> `_STL::__median<SNamePairEntry, CNamePairLess>`
- 00a40ef0 `Vector_Shrink_Destroy_CCharString` -> `_STL::__insertion_sort<SNameEntry*, CNameLess>`
- 00a40f50 `Introsort_Quicksort_CCharStringTriple` -> `_STL::__linear_insert<SNamePairEntry*, SNamePairEntry, CNamePairLess>`
- 00a41200 `Heap_MakeHeap_CCharStringPair` -> `_STL::__make_heap<SNameEntry*, CNameLess, SNameEntry, int>`
- 00a41630 `_Pop_heap<CTestQuest*,int,CTestQuest,CTestQuestNameCompare>` -> `_STL::__pop_heap<SNamePairEntry*, SNamePairEntry, CNamePairLess, int>`
- 00a416e0 `Heap_MakeHeap_CCharStringTriple` -> `_STL::__make_heap<SNamePairEntry*, CNamePairLess, SNamePairEntry, int>`
- 00a418a0 `CCharStringPair_AppendToVector` -> `_STL::vector<SNameEntry>::push_back`
- 00a41ad0 `CCharStringPair_AppendWithPointer` -> `CVectorMap<CCharString, CSoundBankLH*>::Insert` (CVectorMap::Insert is a PDB member name)
- 00a41b60 `Heap_PopHeap_CCharStringPair` -> `_STL::__partial_sort<SNameEntry*, SNameEntry, CNameLess>`
- 00a41cd0 `Introsort_InsertSorted_CCharStringTriple` -> `_STL::__partial_sort<SNamePairEntry*, SNamePairEntry, CNamePairLess>`
- Outside the chunk: 0x00A40250, catalog `_Med3<std::pair<CCharString,CCharString>*,...>`, is the `__cdecl`
  `pair<CCharString, CCharString>` operator<. `CSoundBankLH` (the mapped object type) is a descriptive name.
  SNameEntry = `pair<CCharString, CSoundBankLH*>` and SNamePairEntry = `pair<pair<CCharString, CCharString>, CSoundBankLH*>`.

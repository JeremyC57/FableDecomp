# Hand chunk 417 (_STL::map .. CClassTypeInfo): 38 of 40 byte-exact

## What matched (38)

- **C3DMeshStats sort family, 20-byte `pair<unsigned long, C3DHelperPoint>`.** These rows are
  `__unguarded_linear_insert` 0x00A31280, `__uninitialized_fill_n` 0x00A32020, `__unguarded_partition` 0x00A329B0 and
  `__partial_sort` 0x00A342F0, plus `vector::_M_insert_overflow` 0x00A33D90 and `CVectorMap::Insert` 0x00A34950.
  - The element is the landed 0x00A32440 / 0x00A32C20 model: C3DVector has a user copy constructor and the pair has
    STLport's explicit copy constructor, so every copy is five dword moves.
  - The SGI bodies matched verbatim, all `__fastcall`.
  - `_M_insert_overflow` needed the landed 0x00569E30 lever on its third grade. With the two tag-taking callees given
    same-TU `__declspec(noinline)` bodies, VC7.1 homes every `__false_type()` temporary on the live `atend` slot.
    Without them the first temporary gets a fresh frame slot (`sub esp,8`).
- **C3DMeshStats sort family, 0x38-byte `pair<unsigned long, C3DDummyObject>`.** These rows are
  `__unguarded_linear_insert` 0x00A31320, `__unguarded_insertion_sort_aux` 0x00A323D0, `__adjust_heap` 0x00A32500,
  `__unguarded_partition` 0x00A32A70, `__make_heap` 0x00A32CC0, `__partial_sort` 0x00A34360 and `CVectorMap::Insert`
  0x00A349B0.
  - All matched first try with the landed 0x00A33800 model: the pair copy is key + `rep movsd` x13 and the
    assignment is `rep movsd` x14.
  - In 0x00A34360 `__pop_heap` is inline. Its by-value T is a frame local that `__adjust_heap` copies again.
  - In 0x00A32A70, `__iter_swap`'s temporary lands in the pivot's dead argument slot.
- **C3DMeshStats sort family, 16-byte `pair<unsigned long, C3DMeshVolume>`.** These rows are
  `__unguarded_linear_insert` 0x00A33250, `__copy` 0x00A33670, `__unguarded_insertion_sort_aux` 0x00A33870,
  `__unguarded_partition` 0x00A34260, `__make_heap` 0x00A344E0, `__linear_insert` 0x00A34820, `__insertion_sort`
  0x00A34D10, `__partial_sort` 0x00A35260 and `__introsort_loop` 0x00A35490. All matched first try.
  - The landed 0x00A333A0 model is a C3DMeshVolume `{CCharString Name; CCountedPointer ConvexVolume}`. Its
    in-class `operator=` is out of line at 0x00A32960; the counted pointer's assignment is ShareData's
    `Info != info` form; its destructor is `if (Info && --RefCount == 0) ...; Data = 0; Info = 0;`.
  - Each body is an explicitly instantiated `__fastcall` template.
  - VC7.1 inlines the record assignment in the loop but calls the COMDAT for the final `*last = val`.
  - `iter_swap` → `swap(*a, *b)` calls the landed `swap` 0x00A331B0 out of line.
  - `__median` is inlined over the keys, and the introsort threshold is tested on the byte distance
    (`and eax,-10h ; cmp eax,100h`).
- **C3DGeneratorObject (64 bytes).** These rows are `__uninitialized_copy` 0x00A31EC0, `fill` 0x00A32100, counted
  `__copy` 0x00A32170 and `vector::resize(n)` 0x00A34DF0.
  - The 13-dword `rep movsd` in the implicit copy and assignment is a **C3DDummyObject base class**. The PDB lists
    +0..+0x30 as padding, and FableWin.pdb's constructor takes a `const C3DDummyObject&`.
  - Separate `CMatrix3x4 Matrix; unsigned long GeneratorID;` members compile to `rep movsd` x12 plus one dword move
    (grade 1 was DIFFER for all three rows).
  - `resize` builds `T()` with CMatrix3x4's chained SetIdentity (the landed 0x00B32100 model) and a default
    CCharString. BoneIndex, BankIndex and UseLocalOrigin stay unset.
- **C3DMeshFile::GetRoot() const 0x00A32620** matched first try. The catalog called it "SharedPtr_Assign".
  - FableWin.pdb names the instantiations it needs: the `CCountedPointer<const CChunkedFileSuperChunk>` converting
    constructor and `SetStaticCast<const CChunkedFileSuperChunk>`, through ShareData and ReleaseData.
  - The body is `CCountedPointer<const Root> root; root.SetStaticCast(CCountedPointer<const Super>(RootChunk)); return root;`.
  - ShareData's `Info != info` test against the empty local leaves `{0,0}` for a null block even when Data is set.
  - C3DAnimFile::GetRootChunk 0x00A537F0 is the byte twin and is not landed.
- **C3DMeshObject (5 rows).**
  - GetRenderMesh 0x00A36090 matched first try.
  - GetNoHelperPoints 0x00A36140 and GetNoDummyObjects 0x00A36480 needed a named `last = FindXEnd(hash)` local
    first, so End is called before Begin and the member is re-read.
  - GetHelperPointPositionAnimated 0x00A36350 needed the helper lookup in its own statement, bound to a named
    reference.
  - HasDummyObject 0x00A36430 needed the positive `if (stats) return stats->HasDummyObject(..); return false;` form.
- **C3DMeshStats::GetDummyObjectNamesStartingWith 0x00A2F150** is a thiscall forwarder to UnpackNames.
- **Lip-sync map `_M_erase` 0x00A2DC20** is the landed 0x007AE0E0 twin with an 8-byte CLipSyncEntry.
- **CClassFactory (4 rows).**
  - RegisterAllocatedClass 0x00A371F0 and `list<CClass*>::_M_insert_dispatch` 0x00A37800 use STLport list
    insert inlined (malloc plus a null-checked placement copy, with `prev` read first).
  - `list::operator=` 0x00A37940 needs STLport's `_List_iterator(const iterator&)` as the only user copy
    constructor, so the by-value `iterator` is built in its slot while const_iterators are pushed.
  - The CClassTypeInfo copy constructor 0x00A37B50 is the inlined list copy constructor plus memberwise
    initialisation.

## Parked for Fable 5.1 (2)

- 00a2d810 _STL::map<unsigned long,CCountedPointer<C3DAnimation2> >::operator[] DIFFER(155v150): the pair's two `mov [esp+..],0` stores are scheduled before the iterator-slot store `mov [eax],edx` (retail: after `push edx`), and the release reads the block into eax and copies it to esi (`mov eax,[esp+14h] ; mov esi,eax ; dec [eax] ; cmp [esi],0`) where retail does `mov esi,[esp+14h] ; dec [esi] ; jne` | tried: local-block nested release with zeroing, member-form release (161v150, Info re-read), `info && --RefCount == 0` local, static Release(SCountBlock*) helper (158v150) | next: release through a Fable-style `ReleaseData()` member, or the T() temporary as a named local; else lane gap: register-copy/scheduling
- 00a358f0 C3DMeshObject::GetPositionMatrixFromRHSet DIFFER(185v185): the z cross-product term loads `fld [edx+4] ; fmul [edx+0Ch]` where retail has `fld [edx+0Ch] ; fmul [edx+4]` (one x87 operand order, everything else identical) | tried: four-row by-value ctor with negated Forward, named up/forward/right locals into const-ref and by-value ctors, static `__fastcall` with a named local matrix and a by-value axis setter | next: write the z term as `b.Y * a.X` in CrossProduct (right operand loaded first), or a member CrossProduct

## Skipped

- None. No row in this chunk holds two functions.

## Label corrections

All 40 catalog labels are propagated or descriptive. The real identities are:

| Address | Catalog label | Real identity |
|---|---|---|
| 0x00A2D810 | rbtree_lookup_or_insert | `_STL::map<unsigned long, CCountedPointer<C3DAnimation2> >::operator[]` (C3DAnimationInfo::CAnimationData::Animations) |
| 0x00A2DC20 | Utility_DestroyMapNodeWithAllocator | `_Rb_tree<CCharString, pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry> > >::_M_erase` |
| 0x00A2F150 | ConstructString | `C3DMeshStats::GetDummyObjectNamesStartingWith` |
| 0x00A31280 | std_deque_rotate | `__unguarded_linear_insert<pair<ulong,C3DHelperPoint>>` |
| 0x00A31320 | _Copy_impl<CDegradableInfo*> | `__unguarded_linear_insert<pair<ulong,C3DDummyObject>>` |
| 0x00A31EC0 | _Copy_impl<CDegradableInfo*> | `__uninitialized_copy<C3DGeneratorObject>` |
| 0x00A32020 | fill<C3DAnimationStateAlpha*> | `__uninitialized_fill_n<pair<ulong,C3DHelperPoint>>` |
| 0x00A32100 | _Fill<CHeroMarriageInfo*> | `fill<C3DGeneratorObject>` |
| 0x00A32170 | _Copy_impl<CActionInputControl*> | `__copy<C3DGeneratorObject>` |
| 0x00A323D0 | _Fill<CHeroMarriageInfo*> | `__unguarded_insertion_sort_aux<pair<ulong,C3DDummyObject>>` |
| 0x00A32500 | _Adjust_heap<CDebugBox*> | `__adjust_heap<pair<ulong,C3DDummyObject>>` |
| 0x00A32620 | SharedPtr_Assign | `C3DMeshFile::GetRoot() const` |
| 0x00A329B0 | partition_swap_5uint | `__unguarded_partition<pair<ulong,C3DHelperPoint>>` |
| 0x00A32A70 | partition_swap_14uint | `__unguarded_partition<pair<ulong,C3DDummyObject>>` |
| 0x00A32CC0 | make_heap_generic | `__make_heap<pair<ulong,C3DDummyObject>>` |
| 0x00A33250 | std_deque_insert | `__unguarded_linear_insert<pair<ulong,C3DMeshVolume>>` |
| 0x00A33670 | _Move<CScriptThing*> | `__copy<pair<ulong,C3DMeshVolume>>` |
| 0x00A33870 | _Copy_impl<CComplexNumber*> | `__unguarded_insertion_sort_aux<pair<ulong,C3DMeshVolume>>` |
| 0x00A33D90 | Vector_Reallocate_C3DAnimationStateAlpha | `vector<pair<ulong,C3DHelperPoint>>::_M_insert_overflow` |
| 0x00A34260 | Introsort_Partition_Pair | `__unguarded_partition<pair<ulong,C3DMeshVolume>>` |
| 0x00A342F0 | Heap_PartialSort_Pair | `__partial_sort<pair<ulong,C3DHelperPoint>>` |
| 0x00A34360 | swap<CDebugSphere> | `__partial_sort<pair<ulong,C3DDummyObject>>` |
| 0x00A344E0 | Heap_MakeHeap_CFaction | `__make_heap<pair<ulong,C3DMeshVolume>>` |
| 0x00A34820 | Deque_Insert_At_WithRefCount | `__linear_insert<pair<ulong,C3DMeshVolume>>` |
| 0x00A34950 | Vector_PushBack_20bytes | `CVectorMap<ulong,C3DHelperPoint>::Insert` |
| 0x00A349B0 | Vector_PushBack_56bytes | `CVectorMap<ulong,C3DDummyObject>::Insert` |
| 0x00A34D10 | Vector_Destruct_Range | `__insertion_sort<pair<ulong,C3DMeshVolume>>` |
| 0x00A34DF0 | Vector_InsertReserve | `vector<C3DGeneratorObject>::resize(n)` |
| 0x00A35260 | Heap_PopHeap | `__partial_sort<pair<ulong,C3DMeshVolume>>` |
| 0x00A35490 | Introsort_Partition_CCharString | `__introsort_loop<pair<ulong,C3DMeshVolume>>` |
| 0x00A36090 | C3DMeshInfo_GetMesh | `C3DMeshObject::GetRenderMesh` |
| 0x00A36140 | CTCInventoryAbilities_Helper_CRC | `C3DMeshObject::GetNoHelperPoints` |
| 0x00A36350 | C3DMesh_TransformPoint | `C3DMeshObject::GetHelperPointPositionAnimated` |
| 0x00A36430 | HasDummyObject_Wrapper | `C3DMeshObject::HasDummyObject` |
| 0x00A36480 | CTCInventoryAbilities_Helper_GetCount | `C3DMeshObject::GetNoDummyObjects` |
| 0x00A371F0 | CClass_AddAllocation | `CClassFactory::RegisterAllocatedClass` |
| 0x00A37800 | List_TransferNodes | `list<CClass*>::_M_insert_dispatch` |
| 0x00A37940 | List_CopyOrMoveNodes | `list<CClass*>::operator=` |
| 0x00A37B50 | list_node_allocate | `CClassTypeInfo::CClassTypeInfo(const CClassTypeInfo&)` |

Callees named on the way:

- 0x00A2E900 / 0x00A2E910 / 0x00A2E960 / 0x00A2E970 are C3DMeshStats::FindHelperPointsBegin/End and
  FindDummyObjectsBegin/End. The catalog has them as CTCInventoryAbilities::AddLearnedExpression.
- 0x00A32960 is `C3DMeshVolume::operator=`.
- 0x00A537F0 is `C3DAnimFile::GetRootChunk() const`.
- 0x00A33E80 is `vector<pair<ulong,C3DDummyObject>>::_M_insert_overflow`.
- 0x00A31FD0 is `__uninitialized_copy<pair<ulong,C3DHelperPoint>>`.
- 0x00A33510 and 0x00A33F80 are `vector<C3DGeneratorObject>::erase` and `_M_fill_insert`.

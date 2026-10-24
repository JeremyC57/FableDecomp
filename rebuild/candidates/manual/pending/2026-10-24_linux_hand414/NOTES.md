# Hand chunk 414 (CMemoryAllocator .. CScopedMemoryHandle): 40 of 40 byte-exact

All 40 rows grade MATCH / RELOCATION_MATCH with the default flags (`/O2 /Oy /W3`; the CVsyncCallbackTimer rows carry
`#pragma optimize("", off)` for their /Od translation unit). Every row has a behaviour test that prints `<addr>_TEST PASS`
under g++ (tall.sh) and VC7.1 under Wine (tall.py).

## What matched (40)

- **CMemoryAllocator** (0x009F2870 Create, 0x009F2B20 DeallocateAligned, 0x009F2BE0 AllocateAligned). Create builds the
  allocator with an inline class `operator new` over VirtualAlloc(size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE) and stores
  total - available physical memory in two globals. The aligned pair keeps the real block in the dword below the aligned
  pointer. Shape: the static Deallocate/Destroy/Allocate helpers need `__forceinline`; `delete Allocator` goes through an
  inline class `operator delete` (VirtualFree), and VC drops its null test because the inlined count check just tested
  the same register.
- **CInputManager / vector<CInputEvent>** (0x009F5600 AddEventToStore, 0x009F63B0 __copy_ptrs, 0x009F65E0
  _M_allocate_and_copy, 0x009F66A0 copy constructor, 0x009F67E0 erase). STLport 4.x bodies over the 52-byte event.
  Shape: `__copy` must be `__forceinline` inside the out-of-line `__fastcall __copy_ptrs`; the copy constructor goes through
  `_Vector_base(n, a)` with `_STLP_alloc_proxy(a, 0)`.
- **vector<CInputEventPackage>** (0x009F6ED0 _M_insert_overflow). The 48-byte non-POD overflow path. Shape: the tag
  temporaries are homed on the `__atend` slot only when the tag-taking callees are same-TU `auto_inline(off)` stand-ins.
- **CVsyncCallbackTimer and its list<CPendingQuery>** (0x009F7250 ClearAllQueries, 0x009F73A0 OnPreDeviceReset, 0x009F76F0
  allocator::allocate, 0x009F77B0 _STLP_alloc_proxy ctor, 0x009F7830 _List_base::clear, 0x009F79D0 erase, 0x009F7A30 and
  0x009F7BB0 _List_base ctors, 0x009F7AD0 _M_create_node, 0x009F7B60 __distance, 0x009F7C50 pop_back, 0x009F7D70 distance,
  0x009F7DC0 size, 0x009F7E40 push_front, 0x009F7EC0 push_back). One `#pragma optimize("", off)` TU with the iterator-base
  methods out of line (0x0041C780/7A0/7B0/7C0). Shape: in-class inline members still expand under optimize off, and a
  declined inline candidate still reserves frame slots; frame-slot order follows the local NAMES (STLport `__name` spelling);
  const_iterator needs `_Const_traits` so its copy stays implicit while iterator has the user copy constructor; ClearAllQueries
  needs `back()` as `__forceinline` in that source only.
- **Graphic bank list** (0x009F8A20, descriptive name). The out-of-line copy of the CGraphicBankManager::Sizeof body
  (0x009F8380) with the list as `this`. Shape: the landed two-loop form, unchanged.
- **CProfileManager** (0x009F8FC0 AddProfile, 0x009F9930 vector<CProfileTreeEntry*> copy constructor, 0x009F9BA0
  list<CProfileTreeEntry>::insert(pos, x), 0x009F9C40 insert(pos)). Shape: STLport's insert link order (`__n`, `__prev`
  locals) avoids a reload; `insert(pos) { return insert(pos, _Tp()); }` builds the by-value position in its slot only with
  the iterator's user copy constructor; the POD copy constructor is the inline `__copy_trivial` memmove with x re-read
  after malloc.
- **CResourceList** (0x009FC210 ShrinkResources). Unloads while AllocatedMemory + size > MaximumMemory, bounded by
  ResourceCount. Shape: `if (count++ >= ResourceCount) break;` inside the while; uses engine/CResourceList.h.
- **CGraphicInfoFrame algorithms** (0x00A004D0 __uninitialized_fill_n, 0x00A00750 __uninitialized_copy, 0x00A00840
  __copy_backward, 0x00A00A90 fill, 0x00A00B20 __copy). `__fastcall` STLport helpers over the 16-byte frame. Shape: the
  implicit copy constructor stores only the derived vtable; the implicit assignment uses CIVCountedPointer::operator= with
  the release and zero store inside the null test. All five matched on the first grade.
- **CCRC** (0x00A00950 Calc). Zero-seed table CRC-32 (standard 0xEDB88320 table at 0x0129A168, verified from the image).
  Shape: the explicit `if (length == 0) return 0;` keeps the early `ret 8` and the loop's own guard.
- **vector<CGraphicInfo> / vector<CGraphicInfoFrame>** (0x00A01650 push_back(), 0x00A016F0 resize(n)). The STLport
  anachronism `push_back(_Tp())` with the default CGraphicInfo (FrameCount 1, PixelFormat -1, Index -1, bitfield cleared),
  and `resize(n, _Tp())` with erase / _M_fill_insert called out of line. Shape: the same CIVCountedPointer destructor
  model gives retail's odd load/dec/store release.
- **CScopedMemoryHandle** (0x00A03870 Clear, 0x00A039F0 Initialise). Free the handle to its pool and reset to
  (no pool, -1); Initialise = Clear() + assign. Shape: the inlined Clear keeps its dead resets and VC duplicates the tails.

## Parked for Fable 5.1 (0)

None.

## Skipped

None.

## Label corrections

Names come from the PDB where it has them (Ego_d / Ego_r function-name tables). Otherwise they come from the STLport
shape and the callers. "Descriptive name" marks a name the PDB does not have.

- 0x009F5600 CInputEventVector_PushBackAt0x14 -> CInputManager::AddEventToStore (PDB)
- 0x009F63B0 CInputEvent_Copy -> _STL::__copy_ptrs<CInputEvent*, CInputEvent*>
- 0x009F65E0 CInputEvent_AllocateCopy -> _STL::vector<CInputEvent>::_M_allocate_and_copy
- 0x009F66A0 Array_CopyConstruct -> _STL::vector<CInputEvent>::vector(const vector&)
- 0x009F67E0 EventContainer_Erase -> _STL::vector<CInputEvent>::erase(first, last)
- 0x009F6ED0 Vector_InsertElementRange -> _STL::vector<CInputEventPackage>::_M_insert_overflow
- 0x009F7250 CVsyncCallbackTimer_FlushPendingQueries -> CVsyncCallbackTimer::ClearAllQueries
- 0x009F73A0 operator_new -> CVsyncCallbackTimer::OnPreDeviceReset
- 0x009F76F0 AllocArray_0x18Elements -> _STL::allocator<_List_node<CPendingQuery> >::allocate
- 0x009F77B0 __EH_epilog3 -> _STL::_STLP_alloc_proxy constructor
- 0x009F7830 LinkedList_Clear -> _STL::_List_base<CPendingQuery>::clear
- 0x009F79D0 StdList_EraseNode -> _STL::list<CPendingQuery>::erase
- 0x009F7A30 StdList_DefaultCtor_9f7a30 -> _STL::_List_base<CPendingQuery> constructor
- 0x009F7AD0 CFourierAnalysis_CreateHeapInstance -> _STL::list<CPendingQuery>::_M_create_node
- 0x009F7B60 MapIterator_CountDistance -> _STL::__distance (list const_iterator)
- 0x009F7BB0 StdList_DefaultCtor_9f7bb0 -> _STL::_List_base<16-byte element> constructor (element unidentified)
- 0x009F7C50 List_RemoveNode_Free -> _STL::list<CPendingQuery>::pop_back
- 0x009F7D70 MapIterator_CountDistance_9f7d70 -> _STL::distance (list const_iterator)
- 0x009F7DC0 OnPostSwapScreens_UpdateNUIListCount -> _STL::list<CPendingQuery>::size
- 0x009F7E40 Link_FourierAnalysisNode -> _STL::list<CPendingQuery>::push_front
- 0x009F7EC0 LinkedList_InsertNode -> _STL::list<CPendingQuery>::push_back
- 0x009F8A20 ComputeGraphicDataBankListSize -> descriptive name: Sizeof over list<CGraphicBankManager::CGraphicDataBankInfo>
  (no callers; the body CGraphicBankManager::Sizeof 0x009F8380 inlines)
- 0x009F8FC0 DoublyLinkedList_InsertNode -> CProfileManager::AddProfile(CProfile&) (PDB)
- 0x009F9930 std_vector_copy_constructor -> _STL::vector<CProfileManager::CProfileTreeEntry*>::vector(const vector&)
- 0x009F9BA0 ProfileEntry_Insert -> _STL::list<CProfileManager::CProfileTreeEntry>::insert(iterator, const T&)
- 0x009F9C40 ProfileList_AddEntry -> _STL::list<CProfileManager::CProfileTreeEntry>::insert(iterator)
- 0x009FC210 CResourceList_MakeFreeSpace -> CResourceList::ShrinkResources(unsigned long) (PDB; MakeFreeSpace is its caller 0x009FC4F0)
- 0x00A004D0 EntityArray_CopyInit -> _STL::__uninitialized_fill_n<CGraphicInfoFrame*>
- 0x00A00750 _Copy_impl<CGameMapCell*,CGameMapCell*> -> _STL::__uninitialized_copy<CGraphicInfoFrame*>
- 0x00A00840 _Copy_backward<CPhysicsMeshInfo*,...> -> _STL::__copy_backward<CGraphicInfoFrame*>
- 0x00A00A90 _Copy_backward<CDefPointer<CThingBaseDef const>*,...> -> _STL::fill<CGraphicInfoFrame*>
- 0x00A00B20 _Move<CGraphicInfoFrame*,...> -> _STL::__copy<CGraphicInfoFrame*> (element right, algorithm name STLport's)
- 0x00A00950 Calc -> CCRC::Calc (PDB class; retail takes stack arguments)
- 0x00A01650 CGraphicDataBank_AddEntry -> _STL::vector<CGraphicInfo>::push_back()
- 0x00A016F0 Vector_Resize -> _STL::vector<CGraphicInfoFrame>::resize(unsigned int)
- 0x00A03870 DiscardAsyncData -> CScopedMemoryHandle::Clear (PDB)
- 0x00A039F0 CHandleBuffer_Reset -> CScopedMemoryHandle::Initialise(CHandleBasedMemoryPool*, long) (PDB)

Callees identified on the way: 0x009FC170 CResourceList::UnloadNextResource(unsigned long); 0x00A02B30
CHandleBasedMemoryPool::Free(long); 0x00A01120 vector<CGraphicInfo>::_M_insert_overflow (__false_type, element loops);
0x00A00ED0 vector<CGraphicInfoFrame>::erase(first, last); 0x00A00F50 vector<CGraphicInfoFrame>::_M_fill_insert.

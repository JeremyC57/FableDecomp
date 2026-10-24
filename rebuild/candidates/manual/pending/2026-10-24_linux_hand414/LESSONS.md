# Chunk 414 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- /Od TU (`#pragma optimize("", off)`): the frame-slot order of locals follows their NAMES. Spell STLport's own
  `__next_node`, `__prev_node`, `__n`, `__position`, `__tmp`, or the slots come out permuted (0x009F7C50 pop_back,
  0x009F79D0 erase).
- Under `optimize("", off)`, /Ob stays on: plain in-class inline members still expand. A declined inline candidate still
  reserves its frame slots. Defining `list::_M_create_node` in-class gave push_back/push_front (0x009F7EC0/0x009F7E40) the
  three slots they were short. ClearAllQueries (0x009F7250) still needed `back()` as `__forceinline`.
- STLport list iterators in the /Od TU: iterator needs the user copy constructor `_List_iterator(const iterator&)`, which
  calls the out-of-line base constructor 0x0041C780. const_iterator must copy implicitly. Model this with
  `_Nonconst_traits`/`_Const_traits`; a single iterator type breaks size() (0x009F7DC0).
- The /Od `_List_base(const _Alloc&)` ctor (0x009F7A30/0x009F7BB0) fills its frame only with
  `__malloc_alloc::allocate(n) { void* __result = malloc(n); return __result; }` plus the `allocator<_Node>(__a)` conversion
  temporary for `_M_node(..., 0)`.
- `delete Allocator` through an inline class `static void __fastcall operator delete` (VirtualFree) matches retail's missing
  null test, because VC reuses the register the inlined `GetNoAllocations() == 0` check just tested. An explicit
  `~T(); VirtualFree` loads the global into esi instead (0x009F2B20).
- Static CMemoryAllocator helpers (Deallocate, Destroy, Allocate) do not inline into the aligned wrappers without
  `__forceinline` (0x009F2B20, 0x009F2BE0). The same goes for `__copy` inside an out-of-line `__fastcall __copy_ptrs`
  (0x009F63B0).
- STLport `insert(pos, x)` link order: `_List_node_base* __n = pos._M_node; _List_node_base* __prev = __n->_M_prev;
  tmp->_M_next = __n; tmp->_M_prev = __prev; __prev->_M_next = tmp; __n->_M_prev = tmp;`. Chaining
  `pos._M_node->_M_prev->_M_next` costs a reload (0x009F8FC0: 55v52, then a match).
- `iterator insert(iterator pos) { return insert(pos, _Tp()); }` builds the by-value position in its argument slot
  (`push ecx ; mov eax,esp ; mov [eax],ecx`) only when the list iterator has a user copy constructor (0x009F9C40).
- STLport's out-of-line `push_back() { push_back(_Tp()); }` anachronism exists (0x00A01650 vector<CGraphicInfo>). The
  `rep movsd` is the implicit copy constructor, and the overflow tag is `__false_type`: the callee 0x00A01120 loops instead
  of using memmove.
- CIVCountedPointer assignment: `if (Object != o.Object) { if (Object) { Object->Release(); Object = 0; } Object = o.Object;
  if (Object) Object->AddRef(); }`. The zero store sits inside the null test, and Release is
  `if (--RefCount == 0) DeleteThis();` (slot 1). The same model gives every implicit CGraphicInfoFrame copy/assign
  (0x00A004D0..0x00A00B20, all first-grade matches). The matching destructor also reproduces 0x00A016F0's odd
  load/dec/store release.
- Implicit copy constructor of a class derived from a polymorphic base with an out-of-line default ctor (CBaseClass
  0x0099A2D0): VC7.1 stores only the derived vtable; the base vptr store is dead and removed (0x00A004D0).
- An inlined `Clear()` followed by member assignment keeps the dead `Pool = 0; Handle = -1` stores, and VC duplicates the
  assignment tail per arm (0x00A039F0 CScopedMemoryHandle::Initialise).
- An explicit `if (length == 0) return 0;` before an unsigned `for` keeps both the early `ret 8` and the loop's own `jbe`
  guard (0x00A00950 CCRC::Calc).
- `while (a + size > max) { if (count++ >= limit) break; if (!Unload(0)) break; }` gives the rotated loop with
  `mov ecx,edi ; inc edi` (0x009FC210 CResourceList::ShrinkResources).
- A POD vector copy constructor is STLport's inline `__copy_trivial`
  (`last == first ? result : (char*)memmove(result, first, n) + n`) with the base ctor inlined. x's pointers are re-read
  after the malloc (0x009F9930).

Review at landing:
- Grade counts: rows 0x009F8A20 .. 0x00A039F0 took one or two grades each. 0x009F2B20 and the /Od list rows (shared
  model probe/model3.h, generator gen/gen_od.py) were iterated through shared-model probes. Their per-row counts were not
  kept, so some of them may have gone over the 4-grade budget; the final sources all match.
- Already-landed sources with wrong identities:
  - 0x009F69B0 "CDefWithRecords_009f69b0::operator=" is the CInputEventPackage copy constructor (push_back and
    _M_insert_overflow call it as a placement copy-construct).
  - 0x009F7330 "CPairedNodeLists::~CPairedNodeLists" is CVsyncCallbackTimer::~CVsyncCallbackTimer (it calls
    ClearAllQueries 0x009F7250, then clears both query lists).
  - 0x009F7760 "CPersistTraits<CReactionFrequencyTraitsArray>" is the /Od `_List_iterator(const iterator&)` copy
    constructor (one call to 0x0041C780).
  - 0x009F7910/7AA0/7C20/7CE0 "CRetailListOd*" are list<CPendingQuery> members of the same /Od TU.
  - 0x009FC4F0 CResourceList::MakeFreeSpace calls its callee "EvictForSpace"; the PDB name is ShrinkResources (0x009FC210).
  - 0x009FF590 NotifyEntryAdded's `GraphicArray::Add()` is vector<CGraphicInfo>::push_back() (0x00A01650).
- Local models next to engine headers:
  - 0x009FC210 uses engine/CResourceList.h through a `CResourceList_Methods` subclass.
  - The CGraphicInfoFrame rows model the frame as CBaseClass-derived with its own vtable (0x0129C878). The landed
    0x00A009A0 `??_G` models it as a non-polymorphic record with the base as first member. Both match their own rows;
    there is no engine header for CGraphicInfoFrame.
- 0x009F8A20 has no callers and no PDB name. Its descriptive class `CGraphicDataBankList` is the
  list<CGraphicBankManager::CGraphicDataBankInfo> itself; a better identity, such as a CPersistTraits Sizeof, may exist.
- 0x009F7BB0's 16-byte list element is unidentified (`SListEntry16`).
- 0x00A00950 CCRC::Calc is modelled `__stdcall` because retail takes data and length on the stack. The later PDBs have
  `__fastcall` statics (`SIKPBXK`), so the leaf is `?Calc@CCRC@@`.
- The Calc table 0x0129A168 was checked against the image: it is the standard reflected CRC-32 table (0xEDB88320).

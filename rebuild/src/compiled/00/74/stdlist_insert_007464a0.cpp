// std::list<CEnginePrimitiveHandle>::insert(iterator) -- STLport 4.6 _list.h `iterator insert(iterator __pos) { return insert(__pos, _Tp()); }`
//   (the no-default-parameter overload): build a default CEnginePrimitiveHandle, copy it into a new node before `__pos`, return the node
// Leaf: ?insert@?$list@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAE?AU?$_List_iterator@VCEnginePrimitiveHandle@@U?$_Nonconst_traits@VCEnginePrimitiveHandle@@@std@@@2@U32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007464A0, 177 bytes (+15 int3), `ret 8` (hidden result pointer + the iterator by value).
// Identity (NF201, 2026-11-20): the STLport instance, descriptive-by-library (the PDB name sets hold no STL instance names; the spelling
//   is the one stlport/fable_containers.h produces). Callers: list<CEnginePrimitiveHandle>::push_back() 0x007467D0 (the STLport inline
//   `insert(end())`, the 0x009E4AA0 / 0x009E4A20 CMaintainedEvent pair's shape) and 0x00833C00 (FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT,
//   the iterator copied in place into the argument slot, the result through a hidden pointer, then back()).
// Callees: CBaseClass::CBaseClass() 0x0099A2D0 / ~CBaseClass() 0x0099A2E0, the CRT malloc import 0x00BFEA0E (STLport's
//   __malloc_alloc node allocator), the CEnginePrimitiveHandle vtable 0x0122F598 and its pointee's NotifyReleased / Delete slots.
// Was the descriptive stand-in CEnginePrimitiveHandleList::InsertDefault over local CEnginePrimitiveHandle / list-node models.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_COUNTED_COPY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT_NOINLINE
#include "stlport/fable_containers.h"
#include "engine/CEnginePrimitiveHandle.h"

template std::list<CEnginePrimitiveHandle, std::allocator<CEnginePrimitiveHandle> >::iterator
    std::list<CEnginePrimitiveHandle, std::allocator<CEnginePrimitiveHandle> >::insert(
        std::list<CEnginePrimitiveHandle, std::allocator<CEnginePrimitiveHandle> >::iterator);

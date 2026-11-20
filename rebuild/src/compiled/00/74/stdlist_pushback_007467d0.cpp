// std::list<CEnginePrimitiveHandle>::push_back() -- STLport 4.6 _list.h `void push_back() { insert(end()); }`: append a default-constructed
//   handle through insert(iterator)
// Leaf: ?push_back@?$list@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007467D0, 20 bytes: push ecx (the discarded result's slot) ; end() = [ecx] copied into its argument
//   slot ; the hidden result slot's address pushed ; call 0x007464A0 ; pop ecx ; ret.
// Identity (NF201, 2026-11-20): the same 20 bytes as the landed std::list<CMaintainedEvent>::push_back 0x009E4AA0 (call target aside),
//   whose callee 0x009E4A20 is list<CMaintainedEvent>::insert(iterator); here the callee is the list<CEnginePrimitiveHandle> instance
//   0x007464A0 (default CEnginePrimitiveHandle, vtable 0x0122F598). Descriptive-by-library spelling (STLport header instance; no STL
//   instance names in the PDB name sets). Was the placeholder CKeyedOwner_007467d0::Register calling CKeyedOwner_007467d0::Insert.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT
#include "stlport/fable_containers.h"
#include "engine/CEnginePrimitiveHandle.h"

template void std::list<CEnginePrimitiveHandle, std::allocator<CEnginePrimitiveHandle> >::push_back();

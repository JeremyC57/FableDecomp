// std::_List_base<CCountedPointer<NUISystem::CAction> >::_List_base(const allocator&) -- STLport list base constructor: allocate the
//   0x10-byte sentinel node and link it to itself
// Leaf: ??0?$_List_base@V?$CCountedPointer@VCAction@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCAction@NUISystem@@@@@std@@@std@@QAE@ABV?$allocator@V?$CCountedPointer@VCAction@NUISystem@@@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00535140, 32 bytes, thiscall `ret 4`.
// Identity (NF93 q11435, hand4f b3 claim): the body is STLport _List_base(const allocator&) (zero the node pointer, malloc a 0x10 sentinel:
//   two links + an 8-byte CCountedPointer<NUISystem::CAction>, self-link, store); its callers 0x0053848F / 0x005384DD (an inline list
//   constructor of a NUISystem::CAction list) ignore its return, and the landed caller row binds this spelling. Descriptive-by-library
//   (STLport; the PDBs are Dinkumware builds). Was the address-named std::list_T_0x8_00535140 stand-in.
// Callees: malloc 0x00BFEA0E (the node allocator).
// Shape: stlport/fable_containers.h's default _List_base(const _Alloc&) body, emitted through an explicit instantiation.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace NUISystem { class CAction; }

template std::_List_base<CCountedPointer<NUISystem::CAction>, std::allocator<CCountedPointer<NUISystem::CAction> > >::_List_base(
    const std::allocator<CCountedPointer<NUISystem::CAction> >&);

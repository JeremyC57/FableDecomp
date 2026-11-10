// PKCTRList<CUnprovenListItem_012b39b8>::Reserve(long count) -- make room for count items: forwards to its container implementation's Reserve (+4, container slot 4)
// Leaf: ?Reserve@?$PKCTRList@VCUnprovenListItem_012b39b8@@@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C23820, 8 bytes, thiscall (`ret 4` in the callee), a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax+0x10]`;
//   a new function start exposed by splitting the gap-scan row 0x00C237F0.
// Identity (descriptive; libpk has no PDB or RTTI): slot 14 of the abstract list vtable 0x012B39B8 (cell 0x012B39F0: slots 0..3 _purecall,
//   slot 4 its deleting destructor), the PKCTRList instance whose slot-14 forwarder this copy is; the concrete lists
//   0x012B3AC8, 0x012B3B10, 0x012B3C88, 0x012B3CD0, 0x012B3D18, 0x012B3F58 inherit it unchanged (their own cells at the same slot). The body reads
//   container slot 4 (+0x10) of the +4 implementation, the landed PKCContainer_00c17b70 Reserve; the class family, slot map and
//   name evidence are in libpk/PKCTRList.h. Every list instance has its own copy of the five forwarders 0x2C / 0x30 / 0x24 /
//   0x10 / 0x28 (slots 11..15, 29 instances), byte-identical copies: one template instance per item type.
#include "libpk/PKCTRList.h"

class CUnprovenListItem_012b39b8;           // the item type of this list instance (unproven: named by the instance's abstract vtable)

template <class T> void PKCTRList<T>::Reserve(long count)
{
    m_pImpl->Reserve(count);
}

template void PKCTRList<CUnprovenListItem_012b39b8>::Reserve(long);
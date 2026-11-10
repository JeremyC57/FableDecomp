// PKCTRList<CUnprovenListItem_012b50f0>::GetCount() -- the number of items: asks its container implementation (+4) for its count (container slot 10)
// Leaf: ?GetCount@?$PKCTRList@VCUnprovenListItem_012b50f0@@@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36880, 8 bytes, thiscall, a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax+0x28]`;
//   a new function start exposed by splitting the gap-scan row 0x00C36840.
// Identity (descriptive; libpk has no PDB or RTTI): slot 15 of the abstract list vtable 0x012B50F0 (cell 0x012B512C: slots 0..3 _purecall,
//   slot 4 its deleting destructor), the PKCTRList instance whose slot-15 forwarder this copy is; the concrete lists
//   0x012B5138, 0x012B5180 inherit it unchanged (their own cells at the same slot). The body reads
//   container slot 10 (+0x28) of the +4 implementation, the landed PKCContainer_00c17b70 GetCount; the class family, slot map and
//   name evidence are in libpk/PKCTRList.h. Every list instance has its own copy of the five forwarders 0x2C / 0x30 / 0x24 /
//   0x10 / 0x28 (slots 11..15, 29 instances), byte-identical copies: one template instance per item type.
#include "libpk/PKCTRList.h"

class CUnprovenListItem_012b50f0;           // the item type of this list instance (unproven: named by the instance's abstract vtable)

template <class T> long PKCTRList<T>::GetCount()
{
    return m_pImpl->GetCount();
}

template long PKCTRList<CUnprovenListItem_012b50f0>::GetCount();
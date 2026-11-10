// PKCTRList<CUnprovenListItem_012b7ea8>::GetItemByIndex(long index) -- the item at an index: asks its container implementation (+4) for the index-th item (container slot 9)
// Leaf: ?GetItemByIndex@?$PKCTRList@VCUnprovenListItem_012b7ea8@@@@UAEPAVCUnprovenListItem_012b7ea8@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C477D0, 8 bytes, thiscall (`ret 4` in the callee), a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax+0x24]`;
//   a new function start exposed by splitting the gap-scan row 0x00C477B0.
// Identity (descriptive; libpk has no PDB or RTTI): slot 13 of the abstract list vtable 0x012B7EA8 (cell 0x012B7EDC: slots 0..3 _purecall,
//   slot 4 its deleting destructor), the PKCTRList instance whose slot-13 forwarder this copy is; the concrete lists
//   0x012B7F40, 0x012B7FD0 inherit it unchanged (their own cells at the same slot). The body reads
//   container slot 9 (+0x24) of the +4 implementation, the landed PKCContainer_00c17b70 GetItemByIndex; the class family, slot map and
//   name evidence are in libpk/PKCTRList.h. Every list instance has its own copy of the five forwarders 0x2C / 0x30 / 0x24 /
//   0x10 / 0x28 (slots 11..15, 29 instances), byte-identical copies: one template instance per item type.
#include "libpk/PKCTRList.h"

class CUnprovenListItem_012b7ea8;           // the item type of this list instance (unproven: named by the instance's abstract vtable)

template <class T> T* PKCTRList<T>::GetItemByIndex(long index)
{
    return static_cast<T*>(m_pImpl->GetItemByIndex(index));
}

template CUnprovenListItem_012b7ea8* PKCTRList<CUnprovenListItem_012b7ea8>::GetItemByIndex(long);
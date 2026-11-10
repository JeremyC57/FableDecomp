// PKCTRList<CUnprovenListItem_012b3a38>::GetCount() -- the number of items: asks its container implementation (+4) for its count (container slot 10)
// Leaf: ?GetCount@?$PKCTRList@VCUnprovenListItem_012b3a38@@@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C23000, 8 bytes, thiscall, a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax+0x28]`;
//   a new function start exposed by splitting the gap-scan row 0x00C22FC0.
// Identity (descriptive; libpk has no PDB or RTTI): slot 15 of the abstract list vtable 0x012B3A38 (cell 0x012B3A74: slots 0..3 _purecall,
//   slot 4 its deleting destructor), the PKCTRList instance whose slot-15 forwarder this copy is; the concrete lists
//   0x012B3860, 0x012B38A8, 0x012B38F0, 0x012B3BA8, 0x012B3DA8, 0x012B3DF0, 0x012B3E38, 0x012B3E80 inherit it unchanged (their own cells at the same slot). The body reads
//   container slot 10 (+0x28) of the +4 implementation, the landed PKCContainer_00c17b70 GetCount; the class family, slot map and
//   name evidence are in libpk/PKCTRList.h. Every list instance has its own copy of the five forwarders 0x2C / 0x30 / 0x24 /
//   0x10 / 0x28 (slots 11..15, 29 instances), byte-identical copies: one template instance per item type.
#include "libpk/PKCTRList.h"

class CUnprovenListItem_012b3a38;           // the item type of this list instance (unproven: named by the instance's abstract vtable)

template <class T> long PKCTRList<T>::GetCount()
{
    return m_pImpl->GetCount();
}

template long PKCTRList<CUnprovenListItem_012b3a38>::GetCount();
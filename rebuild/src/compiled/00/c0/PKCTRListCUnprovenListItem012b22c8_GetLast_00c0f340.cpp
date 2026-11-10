// PKCTRList<CUnprovenListItem_012b22c8>::GetLast() -- the list's last item: asks its container implementation (+4) for its last item (container slot 12)
// Leaf: ?GetLast@?$PKCTRList@VCUnprovenListItem_012b22c8@@@@UAEPAVCUnprovenListItem_012b22c8@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0F340, 8 bytes, thiscall, a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax+0x30]`;
//   a new function start exposed by splitting the gap-scan row 0x00C0F330.
// Identity (descriptive; libpk has no PDB or RTTI): slot 12 of the abstract list vtable 0x012B22C8 (cell 0x012B22F8: slots 0..3 _purecall,
//   slot 4 its deleting destructor), the PKCTRList instance whose slot-12 forwarder this copy is; the concrete lists
//   0x012B23A0, 0x012B24D0 inherit it unchanged (their own cells at the same slot). The body reads
//   container slot 12 (+0x30) of the +4 implementation, the landed PKCContainer_00c17b70 GetLast; the class family, slot map and
//   name evidence are in libpk/PKCTRList.h. Every list instance has its own copy of the five forwarders 0x2C / 0x30 / 0x24 /
//   0x10 / 0x28 (slots 11..15, 29 instances), byte-identical copies: one template instance per item type.
#include "libpk/PKCTRList.h"

class CUnprovenListItem_012b22c8;           // the item type of this list instance (unproven: named by the instance's abstract vtable)

template <class T> T* PKCTRList<T>::GetLast()
{
    return static_cast<T*>(m_pImpl->GetLast());
}

template CUnprovenListItem_012b22c8* PKCTRList<CUnprovenListItem_012b22c8>::GetLast();
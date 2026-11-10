// PKCTRList<CUnprovenListItem_012b4a18>::GetFirst() -- the list's first item: asks its container implementation (+4) for its first item (container slot 11)
// Leaf: ?GetFirst@?$PKCTRList@VCUnprovenListItem_012b4a18@@@@UAEPAVCUnprovenListItem_012b4a18@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2F760, 8 bytes, thiscall, a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax+0x2c]`;
//   split from the 72-byte gap-scan row 0x00C2F760 (five 8-byte forwarders 16 bytes apart, int3 padding between).
// Identity (descriptive; libpk has no PDB or RTTI): slot 11 of the abstract list vtable 0x012B4A18 (cell 0x012B4A44: slots 0..3 _purecall,
//   slot 4 its deleting destructor), the PKCTRList instance whose slot-11 forwarder this copy is; the concrete lists
//   0x012B4B00, 0x012B4BD8 inherit it unchanged (their own cells at the same slot). The body reads
//   container slot 11 (+0x2C) of the +4 implementation, the landed PKCContainer_00c17b70 GetFirst; the class family, slot map and
//   name evidence are in libpk/PKCTRList.h. Every list instance has its own copy of the five forwarders 0x2C / 0x30 / 0x24 /
//   0x10 / 0x28 (slots 11..15, 29 instances), byte-identical copies: one template instance per item type.
#include "libpk/PKCTRList.h"

class CUnprovenListItem_012b4a18;           // the item type of this list instance (unproven: named by the instance's abstract vtable)

template <class T> T* PKCTRList<T>::GetFirst()
{
    return static_cast<T*>(m_pImpl->GetFirst());
}

template CUnprovenListItem_012b4a18* PKCTRList<CUnprovenListItem_012b4a18>::GetFirst();
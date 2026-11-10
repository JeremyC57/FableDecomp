// PKCTRList<CListItem>::GetItemByIndex(long index) -- the item at an index: asks its container implementation (+4) for the index-th item (container slot 9)
// Leaf: ?GetItemByIndex@?$PKCTRList@UCListItem@@@@UAEPAUCListItem@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C09C60, 8 bytes, thiscall (`ret 4` in the callee), a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax+0x24]`;
//   a new function start exposed by splitting the gap-scan row 0x00C09C40.
// Identity (descriptive; libpk has no PDB or RTTI): slot 13 of the abstract list vtable 0x012B1BC0 (cell 0x012B1BF4: slots 0..3 _purecall,
//   slot 4 its deleting destructor), the PKCTRList instance whose slot-13 forwarder this copy is; the concrete lists
//   0x012B1B78 (CItemList), 0x012B1C40, 0x012B1D50, 0x012B1D98 inherit it unchanged (their own cells at the same slot). The body reads
//   container slot 9 (+0x24) of the +4 implementation, the landed PKCContainer_00c17b70 GetItemByIndex; the class family, slot map and
//   name evidence are in libpk/PKCTRList.h. Every list instance has its own copy of the five forwarders 0x2C / 0x30 / 0x24 /
//   0x10 / 0x28 (slots 11..15, 29 instances), byte-identical copies: one template instance per item type.
#include "libpk/PKCTRList.h"

struct CListItem;                              // the item type of this list instance: the item CItemList (0x012B1B78)
                                               // holds (landed CCollectAllItems::Visit(CListItem*) 0x00C10720, CItemDispatcher::
                                               // RunAllItems 0x00C10400; reloc binding ??0CItemAction@@QAE@PAUCItemDispatcher@@PAUCListItem@@@Z)

template <class T> T* PKCTRList<T>::GetItemByIndex(long index)
{
    return static_cast<T*>(m_pImpl->GetItemByIndex(index));
}

template CListItem* PKCTRList<CListItem>::GetItemByIndex(long);
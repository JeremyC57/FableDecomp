// PKCTRList<CListItem>::GetCount() -- the number of items: asks its container implementation (+4) for its count (container slot 10)
// Leaf: ?GetCount@?$PKCTRList@UCListItem@@@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C09C80, 8 bytes, thiscall, a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax+0x28]`;
//   a new function start exposed by splitting the gap-scan row 0x00C09C40.
// Identity (descriptive; libpk has no PDB or RTTI): slot 15 of the abstract list vtable 0x012B1BC0 (cell 0x012B1BFC: slots 0..3 _purecall,
//   slot 4 its deleting destructor), the PKCTRList instance whose slot-15 forwarder this copy is; the concrete lists
//   0x012B1B78 (CItemList), 0x012B1C40, 0x012B1D50, 0x012B1D98 inherit it unchanged (their own cells at the same slot). The body reads
//   container slot 10 (+0x28) of the +4 implementation, the landed PKCContainer_00c17b70 GetCount; the class family, slot map and
//   name evidence are in libpk/PKCTRList.h. Every list instance has its own copy of the five forwarders 0x2C / 0x30 / 0x24 /
//   0x10 / 0x28 (slots 11..15, 29 instances), byte-identical copies: one template instance per item type.
#include "libpk/PKCTRList.h"

struct CListItem;                              // the item type of this list instance: the item CItemList (0x012B1B78)
                                               // holds (landed CCollectAllItems::Visit(CListItem*) 0x00C10720, CItemDispatcher::
                                               // RunAllItems 0x00C10400; reloc binding ??0CItemAction@@QAE@PAUCItemDispatcher@@PAUCListItem@@@Z)

template <class T> long PKCTRList<T>::GetCount()
{
    return m_pImpl->GetCount();
}

template long PKCTRList<CListItem>::GetCount();
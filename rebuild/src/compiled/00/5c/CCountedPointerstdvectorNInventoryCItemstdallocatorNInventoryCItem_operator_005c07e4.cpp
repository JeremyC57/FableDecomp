#pragma optimize("s",on)
// CCountedPointer::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this (T = std::vector<NInventory::CItem, std::allocator<NInventory::CItem> >)
// Leaf: ??4?$CCountedPointer@V?$vector@VCItem@NInventory@@V?$allocator@VCItem@NInventory@@@std@@@std@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C07E4, 23 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Assign_005c07e4 (stand-in name).
// T = std::vector<NInventory::CItem, std::allocator<NInventory::CItem> >, proven by: NInventory::CTCInventoryBase::Copy 0x005BFB96 assigns SerialisedItems through it; it calls ShareData 0x005C053D, whose release is ReleaseData<vector<CItem>> 0x005C0511 (~CTCInventoryBase releases SerialisedItems there; Ego_r CTCInventoryBase CCountedPointer<std::vector<NInventory::CItem>> SerialisedItems); FableWin.pdb ??4 name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NInventory { class CItem; }
namespace std { template <class _Tp> class allocator; template <class _Tp, class _Alloc> class vector; }
template CCountedPointer<std::vector<NInventory::CItem, std::allocator<NInventory::CItem> > >& CCountedPointer<std::vector<NInventory::CItem, std::allocator<NInventory::CItem> > >::operator=(const CCountedPointer<std::vector<NInventory::CItem, std::allocator<NInventory::CItem> > >&);

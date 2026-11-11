#pragma optimize("s",on)
// CCountedPointer::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info (T = std::vector<NInventory::CItem, std::allocator<NInventory::CItem> >)
// Leaf: ?ReleaseData@?$CCountedPointer@V?$vector@VCItem@NInventory@@V?$allocator@VCItem@NInventory@@@std@@@std@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C0511, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_005c0511 (stand-in name).
// T = std::vector<NInventory::CItem, std::allocator<NInventory::CItem> >, proven by: ~CTCInventoryBase 0x005C00A0 releases +0x78 = SerialisedItems (Ego_r CTCInventoryBase +0x80 CCountedPointer<std::vector<NInventory::CItem>>, +0x88 SerialisedSelectedItems: same order, retail 8 low) from a size TU; ShareData 0x005C053D and Reset 0x005C29CE call it; Reset 0x005C29CE binds ReleaseData<vector<CItem>>; FableWin.pdb ReleaseData name.
// Member: ReleaseData, not the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }` (Ego_r.pdb 5-byte ??1<NUISystem::CComponent>
//   = retail jmp 0x00429C53 -> ReleaseData 0x004291DE); VC7.1 expands that destructor at the call site, so a size-TU caller and
//   every ShareData / Reset call ReleaseData itself (probe: /O1 scalar deleting dtor and scope end call ReleaseData, /O2 ones call ??1).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NInventory { class CItem; }
namespace std { template <class _Tp> class allocator; template <class _Tp, class _Alloc> class vector; }
template void CCountedPointer<std::vector<NInventory::CItem, std::allocator<NInventory::CItem> > >::ReleaseData();

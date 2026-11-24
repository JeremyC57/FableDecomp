#pragma optimize("s",on)
// CCountedPointer<const CFontBank>::ReleaseData() -- release the shared count (run the deleter and free the block when it drops to zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@$$CBVCFontBank@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042F948, 44 bytes, thiscall `ret`. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ReleaseData@?$CCountedPointer@$$CBVCFontBank@@@@QAEXXZ; destructor thunk 0x0042FC0E (`jmp 0x0042F948`, not landed), as the
//   CComponent pair 0x004291DE / 0x00429C53 (`~CCountedPointer() { ReleaseData(); }`); T = const CFontBank: FableWin.pdb ReleaseData / ShareData / Reset / ??4<CFontBank> member template of CCountedPointer<const CFontBank>; called by ShareData<const CFontBank> 0x0042F9F2 (reached from 0x0042DDB3, which assigns CFontManager::GetFont's CCountedPointer<CFontBank> into a CCountedPointer<const CFontBank> member) and Reset<const CFontBank> 0x0042FC13.
//   Was landed as ~CCountedPointer<CUnprovenPointee_0042f948>.
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data zeroed before Info). This size-optimised TU keeps ReleaseData out of line by itself.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CFontBank;
template void CCountedPointer<const CFontBank>::ReleaseData();

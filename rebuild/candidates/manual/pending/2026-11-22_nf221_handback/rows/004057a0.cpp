// CCountedPointer<CFontBank>::ReleaseData() -- release the shared count (run the deleter and free the block when it drops to zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCFontBank@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004057A0, 53 bytes, thiscall `ret`. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ReleaseData@?$CCountedPointer@VCFontBank@@@@QAEXXZ; destructor thunk none (no E9 into it), as the
//   CComponent pair 0x004291DE / 0x00429C53 (`~CCountedPointer() { ReleaseData(); }`); called by ShareData<CFontBank> 0x004190E2 and Reset<CFontBank> 0x0041947A (both call ReleaseData in the header body); no CCountedPointer<CFont> exists in Ego_d / Ego_r / FableWin (CFontManager::GetFont returns CCountedPointer<CFontBank>).
//   Was landed as ~CCountedPointer<CFontBank>.
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data zeroed before Info). Speed-shaped copy (the /O2 body), no size pragma.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CFontBank;
template void CCountedPointer<CFontBank>::ReleaseData();

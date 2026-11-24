#pragma optimize("s",on)
// CCountedPointer<CASoundBank>::ReleaseData() -- release the shared count (run the deleter and free the block when it drops to zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCASoundBank@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042F9A0, 44 bytes, thiscall `ret`. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ReleaseData@?$CCountedPointer@VCASoundBank@@@@QAEXXZ; destructor thunk 0x0042FBCB (`jmp 0x0042F9A0`, not landed), as the
//   CComponent pair 0x004291DE / 0x00429C53 (`~CCountedPointer() { ReleaseData(); }`); Reset<CASoundBank> 0x0042FD6F already binds this ReleaseData spelling.
//   Was landed as ~CCountedPointer<CASoundBank>.
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data zeroed before Info). This size-optimised TU keeps ReleaseData out of line by itself.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CASoundBank;
template void CCountedPointer<CASoundBank>::ReleaseData();

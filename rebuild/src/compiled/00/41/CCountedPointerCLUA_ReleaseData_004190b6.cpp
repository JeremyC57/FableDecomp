#pragma optimize("s",on)
// CCountedPointer<CLUA>::ReleaseData() -- release the shared count (run the deleter and free the block when it drops to zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCLUA@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004190B6, 44 bytes, thiscall `ret`. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ReleaseData@?$CCountedPointer@VCLUA@@@@QAEXXZ; destructor thunk 0x00419423 (`jmp 0x004190B6`, not landed), as the
//   CComponent pair 0x004291DE / 0x00429C53 (`~CCountedPointer() { ReleaseData(); }`); callers: CMainGameComponent ~ (0x00418B7A) / Shutdown (0x004175E5) and Reset<CLUA> (0x00419428) call it on live pointers.
//   Was landed as ~CCountedPointer<CLUA>.
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data zeroed before Info). This size-optimised TU keeps ReleaseData out of line by itself.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CLUA;
template void CCountedPointer<CLUA>::ReleaseData();

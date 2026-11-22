#pragma optimize("s",on)
// CCountedPointer<CMeshDataBank>::ReleaseData() -- release the shared count (run the deleter and free the block when it drops to zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCMeshDataBank@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042918C, 44 bytes, thiscall `ret`. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ReleaseData@?$CCountedPointer@VCMeshDataBank@@@@QAEXXZ; destructor thunk 0x00429BF9 (`jmp 0x0042918C`), as the
//   CComponent pair 0x004291DE / 0x00429C53 (`~CCountedPointer() { ReleaseData(); }`); called by ShareData<CMeshDataBank> 0x004291B8 and Reset<CMeshDataBank> 0x0042FC86.
//   Was landed as ~CCountedPointer<CMeshDataBank>.
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data zeroed before Info). This size-optimised TU keeps ReleaseData out of line by itself.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CMeshDataBank;
template void CCountedPointer<CMeshDataBank>::ReleaseData();

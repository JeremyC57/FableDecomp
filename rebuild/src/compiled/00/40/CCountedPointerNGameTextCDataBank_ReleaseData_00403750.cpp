// CCountedPointer<NGameText::CDataBank>::ReleaseData() -- release the shared count (run the deleter and free the block when it drops to zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCDataBank@NGameText@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403750, 53 bytes, thiscall `ret`. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ReleaseData@?$CCountedPointer@VCDataBank@NGameText@@@@QAEXXZ; destructor thunk none (no E9 into it), as the
//   CComponent pair 0x004291DE / 0x00429C53 (`~CCountedPointer() { ReleaseData(); }`); its three /Os callers (0x004175E5, 0x00418B7A, 0x0042F50B) call the other T's ReleaseData bodies in the same member sequence (NF220 Gate A: this text, no size pragma, grades RELOCATION_MATCH).
//   Was landed as ~CCountedPointer<NGameText::CDataBank>.
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data zeroed before Info). Speed-shaped copy (the /O2 body), no size pragma.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NGameText { class CDataBank; }
template void CCountedPointer<NGameText::CDataBank>::ReleaseData();

// CCountedPointer<NUISystem::CAction>::ReleaseData() -- release the shared count (delete the action and the block when it drops to zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCAction@NUISystem@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429160, 44 bytes, thiscall `ret`. Was landed as ~CCountedPointer<NUISystem::CAction> (FAM12, queue
//   land1011dr RELOC_NAME claim).
// Identity: FableWin.pdb ?ReleaseData@?$CCountedPointer@VCAction@NUISystem@@@@QAEXXZ; the body is the twin of the landed
//   CCountedPointer<NUISystem::CComponent>::ReleaseData 0x004291DE (Ego_r.pdb ??1<CComponent> is a 5-byte function = retail's `jmp`
//   COMDAT 0x00429C53), and CAction has the same pair: 0x00429B72 is a 5-byte `jmp 0x00429160` (the destructor COMDAT, called at
//   scope ends 0x00544E37.. 0x0060AAFB), while 0x00429160 is called directly on live pointers and by the landed binder
//   CTCInventory::AssignThingPC 0x00584B86 as ReleaseData. Data is zeroed before Info (the _RELEASEDATA order).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA, as 0x004291DE.
#pragma optimize("s",on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NUISystem { class CAction; }
template void CCountedPointer<NUISystem::CAction>::ReleaseData();

// CCountedPointer<NUISystem::CComponent>::CCountedPointer(NUISystem::CComponent* data) -- adopt the component: store it and, when non-null, allocate its 12-byte count block {1, DeleteData, Data}
// Leaf: ??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429C15, 62 bytes, thiscall `ret 4`; size-optimised TU (odd address).
// Name: FableWin.pdb ??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z; the count block stores
//   0x00428B10 = ?DeleteData@?$CCountedPointer@VCComponent@NUISystem@@@@SIXPAX@Z (FableWin; its own row: `delete (CComponent*)p`,
//   a virtual deleting destructor call); 43 landed NUISystem / inventory callers bind this address under this spelling.
//   Was landed as a descriptive CCountedPointer_00428b10::CCountedPointer_00428b10 with a Destroy_00428b10 deleter.
// Body: engine/CCountedPointer.h (T*) constructor: Data stored, the argument tested, the block built from the stored Data
//   (`mov ecx,[esi]`), Info = 0 when new fails or the pointer is null.
#pragma optimize("s",on)
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent { public: virtual ~CComponent(); }; }   // polymorphic (RTTI); DeleteData = virtual deleting dtor
template CCountedPointer<NUISystem::CComponent>::CCountedPointer(NUISystem::CComponent* data);

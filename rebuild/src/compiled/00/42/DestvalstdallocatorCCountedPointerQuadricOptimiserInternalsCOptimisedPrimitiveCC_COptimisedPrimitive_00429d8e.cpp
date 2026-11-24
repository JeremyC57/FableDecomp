// CCountedPointer<NUISystem::CList>::~CCountedPointer() -- the destructor: ReleaseData(), tail-jumped
// Leaf: ??1?$CCountedPointer@VCList@NUISystem@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429D8E, 5 bytes (`jmp 0x00429288`). Was landed as a descriptive extern "C" Forward/_ForwardTarget_00429288
//   under the propagated catalog label _Dest_val<allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>>> (NF383).
// Identity (no ICF in retail, GOTCHAS landing nf380: one name per address): the FableWin (debug) caller CTCBoastUI::Activate names
//   ??1?$CCountedPointer@VCList@NUISystem@@@@QAE@XZ and its retail twin 0x0056AD40 calls this address at 0x0056BBEF (the landed row's own
//   leaf relocation maps the destructor here); the 44-byte body 0x00429288 it jumps to is ?ReleaseData@?$CCountedPointer@VCList@NUISystem@@@@QAEXXZ
//   (FableWin ShareData<CList> calls ReleaseData, retail ShareData 0x00429486 calls 0x00429288). Same destructor / release pair as
//   CComponent (0x00429C53 / 0x004291DE) and CAction (0x00429B72 / 0x00429160), engine/CCountedPointer.h.
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY (~CCountedPointer() { ReleaseData(); },
//   ReleaseData out of line in its own row 0x00429288).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NUISystem { class CList; }                  // template argument only (the destructor never names T)
template CCountedPointer<NUISystem::CList>::~CCountedPointer();

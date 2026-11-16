// CCountedPointer<NUISystem::CAction>::~CCountedPointer() -- the destructor: ReleaseData(), tail-jumped
// Leaf: ??1?$CCountedPointer@VCAction@NUISystem@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429B72, 5 bytes (`jmp 0x00429160`, ReleaseData<NUISystem::CAction>).
// NF37 (2026-11-16): was the descriptive extern "C" Forward / ForwardTarget_00429160 pair. Named by what it jumps to and who calls it: the
// target 0x00429160 is CCountedPointer<NUISystem::CAction>::ReleaseData (its own landed row), and the 12 landed call sites
// (0x00544D20 x4, 0x00563920, 0x0056AD40 x3, 0x006096E0 x4) destroy CCountedPointer<NUISystem::CAction> locals and already bind this
// symbol. Same shape as the landed CComponent twin 0x00429C53 (engine/CCountedPointer.h RELEASEDATA: inline dtor { ReleaseData(); }).
#pragma optimize("s",on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NUISystem { class CAction; }
template CCountedPointer<NUISystem::CAction>::~CCountedPointer();

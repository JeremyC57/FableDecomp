// CCountedPointer<NUISystem::CComponent>::Reset(NUISystem::CComponent* data) -- drop the current reference, take ownership of data with a fresh shared-count record
// Leaf: ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z
// Retail: Fable.exe @ 0x00429C6F, 59 bytes, `ret 4`; called from 0x0041ECE3, 0x0041ED1B, 0x0041ED50, 0x0041ED85, 0x004235D6, ....
// Identity (Codex name fix, 2026-11-10): T = NUISystem::CComponent. Ego_r.pdb has ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@
// QAEXPAVCComponent@NUISystem@@@Z, and the landed caller 0x00596A68 binds this address under that name; the release call
// is 0x004291DE, ~CCountedPointer<NUISystem::CComponent> (landed). The deleter 0x00428B10 (`test ecx,ecx / mov eax,[ecx] /
// push 1 / call [eax]`) is the virtual delete of the polymorphic CComponent (the old catalog label, a vector<CCharString>,
// was wrong; this row was first landed with the pointee named by address).
// Body: the destructor's release (0x004291DE); Data = data; if (data) Info = new CCPPointerInfo(1, DeleteData, Data)
// (operator new 12 bytes, null-checked). engine/CCountedPointer.h, FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CALLS_DTOR.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CALLS_DTOR
#include "engine/CCountedPointer.h"
namespace NUISystem {
class CComponent { public: virtual ~CComponent(); };   // polymorphic (the deleter's virtual delete)
}
template void CCountedPointer<NUISystem::CComponent>::Reset(NUISystem::CComponent* data);

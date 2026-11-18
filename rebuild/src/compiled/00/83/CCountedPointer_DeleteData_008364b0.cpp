// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CAnimFilterBase>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 2, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCAnimFilterBase@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008364B0, 12 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax+8]; ret` -- a static __fastcall
//   member (data in ecx); only its address is used: it is stored as the count block's DeleteFunc by CCountedPointer<CAnimFilterBase>::Reset
//   0x00836580 (landed, NF82), which binds this exact spelling.
// Names (NF85, q11060; was the gap-scan placeholder sub_008364b0 / extern "C" CallIfSet): engine/CCountedPointer.h DeleteData (static
//   __fastcall, `delete static_cast<T*>(data)`; the Ego_r.pdb SIXPAX@Z deleter of every CCountedPointer<T>) for T = CAnimFilterBase, whose
//   virtual destructor is slot 2 (engine/CAnimFilterBase.h: operator() 0, Clone 1, ~ 2) -- the `call [eax+8]` here; the explicit
//   instantiation emits the COMDAT, as the landed sibling 0x00834A10 (CCountedPointer<CAIBrain>) does.
#include "engine/CAnimFilterBase.h"
#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CAnimFilterBase>::DeleteData(void*);

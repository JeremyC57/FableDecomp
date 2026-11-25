// CCountedPointer::Reset(T* data) -- (CCountedPointer<CEnginePrimitiveManagerWeatherMask>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCEnginePrimitiveManagerWeatherMask@@@@QAEXPAVCEnginePrimitiveManagerWeatherMask@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B38320, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00B361B0.
// Names (NF428, 2026-11-25; was the stand-in CCountedPointer_00b361b0::Reset with a local count-block model, self
//   ?Reset@CCountedPointer_00b361b0@@QAEXPAX@Z): T from two sources: (1) the retail callers bind the PDB name (table, at 00b33b50), a whole
//   FableWin.pdb entry; (2) retail RTTI: a retail call site passes a freshly constructed CEnginePrimitiveManagerWeatherMask (its vftable RTTI or ??0 call
//   just before the call; NF428 rtti428). The deleter the body stores, 0x00B361B0, is bound to
//   ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerWeatherMask@@@@SIXPAX@Z, a name derived from (1), consistent with it.
// Form (NF371 R1 form b, landed 0x00A299C0): the sibling explicit instantiation of engine/CCountedPointer.h's member, because a
//   landed binder emits a pick-any copy of this Reset (a strong definition would be an equal-body LNK2005);
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY leaves the stored deleter an external reference to its owner row.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CEnginePrimitiveManagerWeatherMask;

template void CCountedPointer<CEnginePrimitiveManagerWeatherMask>::Reset(CEnginePrimitiveManagerWeatherMask*);

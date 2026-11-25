// CCountedPointer::Reset(T* data) -- (CCountedPointer<CLipSyncSampleData>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCLipSyncSampleData@@@@QAEXPAVCLipSyncSampleData@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A44170, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00A44130.
// Names (NF428, 2026-11-25; was the stand-in CCountedPointer_00a44130::Reset with a local count-block model, self
//   ?Reset@CCountedPointer_00a44130@@QAEXPAX@Z): T from one source: the retail callers bind the PDB name (table, at 00a42ea0), a whole FableWin.pdb
//   entry. No retail call site shows a freshly constructed T (NF428 rtti428), so there is no second source; it lands under
//   the owner's best-guess rule (2026-11-25) on the name the table already binds at this address. The deleter the body
//   stores, 0x00A44130, is bound to ?DeleteData@?$CCountedPointer@VCLipSyncSampleData@@@@SIXPAX@Z, a name derived from the same callers (not counted).
// Form (NF371 R1 form b, landed 0x00A299C0): the sibling explicit instantiation of engine/CCountedPointer.h's member, because a
//   landed binder emits a pick-any copy of this Reset (a strong definition would be an equal-body LNK2005);
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY leaves the stored deleter an external reference to its owner row.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CLipSyncSampleData;

template void CCountedPointer<CLipSyncSampleData>::Reset(CLipSyncSampleData*);

// CCountedPointer::Reset(T* data) -- (CCountedPointer<CRegionConnectionGraph>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCRegionConnectionGraph@@@@QAEXPAVCRegionConnectionGraph@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051CEA0, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x0051CCD0.
// Names (NF428, 2026-11-25; was the stand-in CCountedPointer_0051ccd0::Reset with a local count-block model, self
//   ?Reset@CCountedPointer_0051ccd0@@QAEXPAX@Z): T from one source: the retail callers bind the PDB name (table, at 00506d40), a whole FableWin.pdb
//   entry. No retail call site shows a freshly constructed T (NF428 rtti428), so there is no second source; it lands under
//   the owner's best-guess rule (2026-11-25) on the name the table already binds at this address. The deleter the body
//   stores, 0x0051CCD0, is bound to ?DeleteData@?$CCountedPointer@VCRegionConnectionGraph@@@@SIXPAX@Z, a name derived from the same callers (not counted).
// Form (NF371 R1 form b, landed 0x00A299C0): the sibling explicit instantiation of engine/CCountedPointer.h's member, because a
//   landed binder emits a pick-any copy of this Reset (a strong definition would be an equal-body LNK2005);
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY leaves the stored deleter an external reference to its owner row.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CRegionConnectionGraph;

template void CCountedPointer<CRegionConnectionGraph>::Reset(CRegionConnectionGraph*);

// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CNavQuadTree>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCNavQuadTree@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A16270, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00A16270`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00a16270, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCNavQuadTree@@@@SIXPAX@Z; stored by CCountedPointer<CNavQuadTree>::Reset 0x00A16640 (reloc_bindings, caller 0x00A156D0); Ego_r.pdb CNavQuadTreeMap::PQuadTree.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CNAVQUADTREE
#include "engine/CNavQuadTree.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CNavQuadTree>::DeleteData(void*);

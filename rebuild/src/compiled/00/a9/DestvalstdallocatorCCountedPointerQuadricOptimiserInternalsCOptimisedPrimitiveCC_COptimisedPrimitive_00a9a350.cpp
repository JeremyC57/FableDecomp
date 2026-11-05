// CCountedPointer<CUnprovenPointee_00a9a350>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCUnprovenPointee_00a9a350@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9A350, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T unresolved: no typed caller with PDB evidence names the pointee, so a stand-in pointee class carries it (CUnproven<Kind>_<addr>, EXTRA_RULES 2026-10-28).
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
class CUnprovenPointee_00a9a350;
template CCountedPointer<CUnprovenPointee_00a9a350>::~CCountedPointer();

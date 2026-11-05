#pragma optimize("s",on)
// CCountedPointer<CIEngine>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCIEngine@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042F974, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = CIEngine: catalog T CIEngine + caller binding (0042dd28,0042f50b) + PDB fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
class CIEngine;
template CCountedPointer<CIEngine>::~CCountedPointer();

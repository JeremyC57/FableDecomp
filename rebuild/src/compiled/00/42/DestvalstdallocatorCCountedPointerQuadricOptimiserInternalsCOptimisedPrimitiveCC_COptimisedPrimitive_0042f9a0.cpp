#pragma optimize("s",on)
// CCountedPointer<CASoundBank>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCASoundBank@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042F9A0, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = CASoundBank: catalog T CASoundBank + caller binding (0042f50b) + PDB fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
class CASoundBank;
template CCountedPointer<CASoundBank>::~CCountedPointer();

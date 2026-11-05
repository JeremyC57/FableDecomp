#pragma optimize("s",on)
// CCountedPointer<CVillageTaskBase>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCVillageTaskBase@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065DA6B, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = CVillageTaskBase: caller binding CVillageTaskBase (0065ea75,0065ead6,0065eb30,0065eb8a,0065ebe4,0065ec3e,0065ec98,00660240,00660298,006602f0,00660348,00660787,00660894,0066090a,00660980,006609f6,00660a6c) + PDB fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
class CVillageTaskBase;
template CCountedPointer<CVillageTaskBase>::~CCountedPointer();

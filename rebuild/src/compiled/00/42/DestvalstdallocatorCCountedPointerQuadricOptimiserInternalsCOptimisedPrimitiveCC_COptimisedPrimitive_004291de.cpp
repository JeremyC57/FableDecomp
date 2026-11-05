#pragma optimize("s",on)
// CCountedPointer<NUISystem::CComponent>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004291DE, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = NUISystem::CComponent: catalog T NUISystem::CComponent + caller binding (0042d45d,0053b321,005729d8,00573792,005820e7,00582673,00582ace,0058315b,00584f15,005907ab,00596a68,00596cce,00597006,0059cb90,0059d0b8,0059d65a,0059ed42,0059f51e,005ae6af,005ae9de,005aeaee,005b358c,005b3613,005b70b8,005c323a,005c3444,005ccf0e,005ccffc,005cdd86,005d64dd) + PDB egor/fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent; }
template CCountedPointer<NUISystem::CComponent>::~CCountedPointer();

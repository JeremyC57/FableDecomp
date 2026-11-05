#pragma optimize("s",on)
// CCountedPointer<NInventory::CPerItemData>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCPerItemData@NInventory@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C04BF, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = NInventory::CPerItemData: catalog T NInventory::CPerItemData + caller binding (005c12a1,005c19a3,005c2831,005c2aa9,005c2ad8) + PDB fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
namespace NInventory { class CPerItemData; }
template CCountedPointer<NInventory::CPerItemData>::~CCountedPointer();

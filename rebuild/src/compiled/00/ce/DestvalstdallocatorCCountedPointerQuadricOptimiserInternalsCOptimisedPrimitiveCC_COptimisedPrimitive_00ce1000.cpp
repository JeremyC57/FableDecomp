// CCountedPointer<CEntityScriptBase>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCEntityScriptBase@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE1000, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = CEntityScriptBase: caller bindings (170 typed callers bind CCountedPointer<CEntityScriptBase>::~CCountedPointer here) + PDB fablewin symbol ??1?$CCountedPointer@VCEntityScriptBase@@@@QAE@XZ.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
class CEntityScriptBase;
template CCountedPointer<CEntityScriptBase>::~CCountedPointer();

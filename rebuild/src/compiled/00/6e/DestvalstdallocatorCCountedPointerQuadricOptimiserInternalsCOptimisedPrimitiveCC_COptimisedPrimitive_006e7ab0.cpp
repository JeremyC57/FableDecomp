// CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7AB0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = CScriptGameResourceObjectMovieBase: catalog T CScriptGameResourceObjectMovieBase + caller binding (00cff9a0,00d27050,00d7aa30,00d7c470,00d7fc70,00d848d0,00d872d0,00d89ad0,00da2ca0,00df3e50,00df9e00,00dfded0,00e0a820,00e12330,00e137b0,00e1d7b0,00e295e0,00e63890,00e8a2b0,00e9bbc0,00eaafe0,00eabb30,00eaf1e0,00eb2900,00eb2dc0,00eb3280,00eea020,00efdc40,00f170c0) + PDB fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();

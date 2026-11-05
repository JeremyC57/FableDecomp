// CCountedPointer<NCameraModes::CScriptModuleBase>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ED2C0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = NCameraModes::CScriptModuleBase: catalog T NCameraModes::CScriptModuleBase + caller binding (0072a090,0089e190) + PDB fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
namespace NCameraModes { class CScriptModuleBase; }
template CCountedPointer<NCameraModes::CScriptModuleBase>::~CCountedPointer();

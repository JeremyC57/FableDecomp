// CCountedPointer<CScriptThing>::ShareData(T* data, CCPPointerInfo* info) -- share another pointer's count block
// Leaf: ?ShareData@?$CCountedPointer@VCScriptThing@@@@QAEXPAVCScriptThing@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007044D0, 67 bytes. Callees: the old block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// When the block differs from the current one: release the current block (RefCount drop; at zero DeleteFunc on its Data
// and the block freed), then adopt data / info and take a reference on the new block (engine/CCountedPointer.h).
// Name: the binding table already binds this PDB name here (a landed caller); the self now spells it (one address, one name).
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
class CScriptThing;
template void CCountedPointer<CScriptThing>::ShareData(CScriptThing*, CCPPointerInfo*);

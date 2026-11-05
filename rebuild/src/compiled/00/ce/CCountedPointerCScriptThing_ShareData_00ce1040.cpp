// CCountedPointer<CEntityScriptBase>::ShareData(T* data, CCPPointerInfo* info) -- share another pointer's count block
// Leaf: ?ShareData@?$CCountedPointer@VCEntityScriptBase@@@@QAEXPAVCEntityScriptBase@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE1040, 67 bytes. Callees: the old block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// When the block differs from the current one: release the current block (RefCount drop; at zero DeleteFunc on its Data
// and the block freed), then adopt data / info and take a reference on the new block (engine/CCountedPointer.h).
// T = CEntityScriptBase: caller bindings (36 typed callers bind CCountedPointer<CEntityScriptBase>::ShareData here) + PDB fablewin symbol ?ShareData@?$CCountedPointer@VCEntityScriptBase@@@@QAEXPAVCEntityScriptBase@@PAVCCPPointerInfo@@@Z. The catalog label CCountedPointer<CScriptThing> (and engine/CCountedPointer.h line 29) is propagated; its neighbour 0x00CE1000 is the CEntityScriptBase destructor.
#include "engine/CCountedPointer.h"
class CEntityScriptBase;
template void CCountedPointer<CEntityScriptBase>::ShareData(CEntityScriptBase*, CCPPointerInfo*);

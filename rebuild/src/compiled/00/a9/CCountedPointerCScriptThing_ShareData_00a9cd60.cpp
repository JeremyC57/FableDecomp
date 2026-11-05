// CCountedPointer<CUnprovenPointee_00a9cd60>::ShareData(T* data, CCPPointerInfo* info) -- share another pointer's count block
// Leaf: ?ShareData@?$CCountedPointer@VCUnprovenPointee_00a9cd60@@@@QAEXPAVCUnprovenPointee_00a9cd60@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9CD60, 67 bytes. Callees: the old block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// When the block differs from the current one: release the current block (RefCount drop; at zero DeleteFunc on its Data
// and the block freed), then adopt data / info and take a reference on the new block (engine/CCountedPointer.h).
// T unresolved: the catalog's CCountedPointer<CScriptThing> label is propagated across 149 byte-identical ShareData bodies, and no typed caller names this one, so a stand-in pointee class carries it (CUnproven<Kind>_<addr>, EXTRA_RULES 2026-10-28).
#include "engine/CCountedPointer.h"
class CUnprovenPointee_00a9cd60;
template void CCountedPointer<CUnprovenPointee_00a9cd60>::ShareData(CUnprovenPointee_00a9cd60*, CCPPointerInfo*);

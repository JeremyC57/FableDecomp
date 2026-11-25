// CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::ShareData(T* data, CCPPointerInfo* info) -- share another pointer's count block
// Leaf: ?ShareData@?$CCountedPointer@VCScriptGameResourceObjectScriptedThingBase@@@@QAEXPAVCScriptGameResourceObjectScriptedThingBase@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA8E0, 67 bytes. Callees: the old block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// When the block differs from the current one: release the current block (RefCount drop; at zero DeleteFunc on its Data
// and the block freed), then adopt data / info and take a reference on the new block (engine/CCountedPointer.h).
// Name: the binding table already binds this PDB name here (a landed caller); the self now spells it (one address, one name).
// The release ShareData expands is the ReleaseData<T> body retail emits (Data zeroed before Info); its zero stores are
// overwritten by the adoption, so the leaf is the same, and the pick-any ReleaseData<T> copy this TU emits is retail's body.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectScriptedThingBase;
template void CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::ShareData(CScriptGameResourceObjectScriptedThingBase*, CCPPointerInfo*);

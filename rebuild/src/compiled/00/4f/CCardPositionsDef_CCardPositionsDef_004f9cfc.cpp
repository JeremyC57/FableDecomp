// CCardPositionsDef::~CCardPositionsDef() -- release the Offset vector's storage (C2DVector elements: nothing to destroy), then the def base destructor as the tail jump
#pragma optimize("s",on)
// Leaf: ??1CCardPositionsDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F9CFC, 25 bytes, thiscall; size-optimised def TU (`pop ecx` after free).
// Name (agentNF3, 2026-11-10): Ego_r.pdb and FableWin.pdb spell only ??1CCardPositionsDef@@UAE@XZ (virtual through
//   CParentDefClassBase); engine/CCardPositionsDef.h places it at this address (0x004F9CBA / 0x004F9CD0 build and destroy the
//   default element of TransferVectorOfSubComponents<CCardPositionsDef> with it). Was landed as a free
//   `CTCInventoryItem_OnDie(CTCInventoryItem*)` stand-in (catalog _global::Dtor_OnDieBase_004f9cfc).
// Callees: free 0x00BFEA14 (the inlined STLport ~_Vector_base of Offset +0x3C), ~CParentDefClassBase 0x00430300 (tail jump).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCARDPOSITIONSDEF_DTOR_NOVTABLE           // hdr overlay: no vtable store in this TU (as retail)
#include "engine/CCardPositionsDef.h"

CCardPositionsDef::~CCardPositionsDef()
{
}

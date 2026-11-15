#ifndef FABLE_ENGINE_SCRIPT_DISTANCE_H
#define FABLE_ENGINE_SCRIPT_DISTANCE_H
#include "rebuild_abi.h"
class CScriptThing;
// Both original PDBs; native 00cbe3ea, fastcall ECX/EDX references and float on stack.
bool FABLE_FASTCALL IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing&, float);
// Both original PDBs; native00cbe2ff, fastcall two references and float.
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);
#ifdef FABLE_RETAIL_SCRIPT_GETDISTANCEBETWEENTHINGS
// Retail opt-in (wfA1, row 0x00DFCA90 CBanditExtra::Main): Ego_r.pdb ?GetDistanceBetweenThings@@YIMABVCScriptThing@@0@Z, out of line at
// 0x00CBE512 (reloc_bindings; fastcall ECX/EDX references, float result; landed callers 0x00E64210 / 0x00E0F8A0 declare it locally).
float FABLE_FASTCALL GetDistanceBetweenThings(const CScriptThing&, const CScriptThing&);
#endif
#endif

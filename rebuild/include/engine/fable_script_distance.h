#ifndef FABLE_ENGINE_SCRIPT_DISTANCE_H
#define FABLE_ENGINE_SCRIPT_DISTANCE_H
#include "rebuild_abi.h"
class CScriptThing;
// Both original PDBs; native 00cbe3ea, fastcall ECX/EDX references and float on stack.
bool FABLE_FASTCALL IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing&, float);
// Both original PDBs; native00cbe2ff, fastcall two references and float.
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);
#endif

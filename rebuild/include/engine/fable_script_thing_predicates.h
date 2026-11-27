#ifndef FABLE_ENGINE_SCRIPT_THING_PREDICATES_H
#define FABLE_ENGINE_SCRIPT_THING_PREDICATES_H
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CScriptThing;
// Ego_r and FableWin both declare the const vector reference and fastcall convention.
bool FABLE_FASTCALL AreAllThingsInVectorDead(const std::vector<CScriptThing>&);
#endif

#ifndef FABLE_ENGINE_NCREATUREAI_H
#define FABLE_ENGINE_NCREATUREAI_H
#include "fable_compiler.h"
class CThingAICreature;class CThing;
namespace NCreatureAI {
// Both original PDBs, native008645e0; original IsRunnable donor02c2c6dd.
bool FABLE_FASTCALL IsAwareOfThing(const CThingAICreature*,const CThing*,bool);
}
#endif

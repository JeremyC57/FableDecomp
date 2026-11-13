#ifndef FABLE_ENGINE_NCREATUREAI_H
#define FABLE_ENGINE_NCREATUREAI_H
#include "fable_compiler.h"
class CThingAICreature;class CThing;
#ifdef FABLE_RETAIL_NCREATUREAI_SPEAKING
class CThingCreatureBase;
#endif
namespace NCreatureAI {
// Both original PDBs, native008645e0; original IsRunnable donor02c2c6dd.
bool FABLE_FASTCALL IsAwareOfThing(const CThingAICreature*,const CThing*,bool);
#ifdef FABLE_RETAIL_NCREATUREAI_SPEAKING
// Retail opt-in (chunk 2023, row 0x0094C050): Ego_r.pdb / FableWin.pdb ?IsSpeaking@NCreatureAI@@YI_NABVCThingCreatureBase@@@Z
// (0x008644E0) and ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z (0x00864580, landed), free __fastcall functions.
bool FABLE_FASTCALL IsSpeaking(const CThingCreatureBase&);
void FABLE_FASTCALL StopSpeaking(CThingCreatureBase&);
#endif
}
#endif

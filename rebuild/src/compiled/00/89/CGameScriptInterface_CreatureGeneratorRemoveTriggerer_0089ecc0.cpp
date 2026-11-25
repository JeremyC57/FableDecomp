// CGameScriptInterface::CreatureGeneratorRemoveTriggerer(const CScriptThing& creature_generator, const CScriptThing& creature) const -- a creature stops being one of the generator's triggerers
// Leaf: ?CreatureGeneratorRemoveTriggerer@CGameScriptInterface@@UBEXABVCScriptThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089ECC0. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: both GetPThing() (CScriptThing slot 11, +0x2c; the IsAlive / IS_THING_ALIVE checks are a debug-only assertion), then
//   ??$GetTC@VCTCCreatureGenerator@@@CThing@@QAE_NPAPAVCTCCreatureGenerator@@@Z (inline in retail: bit 0x3D, LowerBound 0x0040F020), the inline
//   ?GetTypeFlags@CThing@@QBEKXZ (& 8), ??$GFDebugPDynamicCast@PAVCThing@@PAVCThingCreatureBase@@@@YIPAVCThingCreatureBase@@PAVCThing@@PAV0@@Z and
//   ?RemoveCreatureTriggerer@CTCCreatureGenerator@@QAEXAAVCThingCreatureBase@@@Z (retail 0x0074A1E0, the call target).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CTCCREATUREGENERATOR_SCRIPTTRIGGERERS
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCreatureGenerator.h"
#include "engine/GFDebugPDynamicCast.h"

void CGameScriptInterface::CreatureGeneratorRemoveTriggerer(const CScriptThing& creature_generator, const CScriptThing& creature) const
{
    CThing* generator_thing = creature_generator.GetPThing();
    CThing* thing = creature.GetPThing();
    CTCCreatureGenerator* generator;
    if (generator_thing->GetTC(&generator) && (thing->GetTypeFlags() & 8))
        generator->RemoveCreatureTriggerer(*GFDebugPDynamicCast(thing, (CThingCreatureBase*)0));
}

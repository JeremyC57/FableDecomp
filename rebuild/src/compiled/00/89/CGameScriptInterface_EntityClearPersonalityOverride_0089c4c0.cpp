// CGameScriptInterface::EntityClearPersonalityOverride(const CScriptThing& thing) const -- script command: the creature's opinion of the hero drops its personality override
// Leaf: ?EntityClearPersonalityOverride@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089C4C0, 70 bytes.
// Identity: Ego_r.pdb 0x008DAB90 + FableWin.pdb 0x02A90220 ?EntityClearPersonalityOverride@CGameScriptInterface@@UBEXABVCScriptThing@@@Z,
//   slot 620 of engine/CGameScriptInterface.h (vtable 0x01260F0C).
// Callees: CScriptThing::GetPThing (vtable slot 11), the inlined CThing::GetTC (presence bit 0x4F = TCI_CREATURE_OPINION_OF_HERO at
//   +0x28 bit 15, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020), then CTCCreatureOpinionOfHero::ClearPersonalityOverride
//   @ 0x00682310 (tail jump; Ego_r.pdb ?ClearPersonalityOverride@CTCCreatureOpinionOfHero@@QAEXXZ).
// Name fix (agentNF55, 2026-11-17): re-authored on the shared headers (the EntitySetOpinionAttitudeEnabled 0x0089C380 form). It was the
//   free stand-in CGameScriptInterface_EntityClearPersonalityOverride(IGSI*) over Ent / KeyPtr overlays, calling a made-up
//   EntVec_LowerBound and Obj_Release.
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_FLAT_CLEARPERSONALITY
#include "engine/CScriptThing.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntityClearPersonalityOverride(const CScriptThing& thing) const
{
    CThing* p = thing.GetPThing();
    if (p) {
        CTCCreatureOpinionOfHero* component;
        if (p->GetTC(TCI_CREATURE_OPINION_OF_HERO, &component))
            component->ClearPersonalityOverride();
    }
}

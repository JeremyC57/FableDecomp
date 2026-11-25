// CGameScriptInterface::EntitySetAlwaysBlockAttacksFromThing(const CScriptThing& entity, const CScriptThing& attacker, bool block) const -- the entity's combat component always blocks (or stops blocking) one attacker
// Leaf: ?EntitySetAlwaysBlockAttacksFromThing@CGameScriptInterface@@UBEXABVCScriptThing@@0_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089DFF0. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: entity.GetPThing() and attacker.GetPThing() (CScriptThing slot 11, +0x2c), ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z on
//   both, ??$GetTC@VCTCCombat@@@CThing@@QAE_NPAPAVCTCCombat@@@Z (inline in retail: bit 0x30, LowerBound 0x0040F020), then by block
//   ?AddThingToAlwaysBlock@CTCCombat@@QAEXPBVCThing@@@Z (retail 0x007352D0) or ?RemoveThingToAlwaysBlock@CTCCombat@@QAEXPBVCThing@@@Z (0x00734190).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_ALWAYSBLOCK
#include "engine/CGameScriptInterface.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CTCCombat.h"

void CGameScriptInterface::EntitySetAlwaysBlockAttacksFromThing(const CScriptThing& entity, const CScriptThing& attacker, bool block) const
{
    CThing* thing = entity.GetPThing();
    CThing* attacker_thing = attacker.GetPThing();
    if (IS_THING_ALIVE(thing) && IS_THING_ALIVE(attacker_thing)) {
        CTCCombat* combat;
        if (thing->GetTC(&combat)) {
            if (block)
                combat->AddThingToAlwaysBlock(attacker_thing);
            else
                combat->RemoveThingToAlwaysBlock(attacker_thing);
        }
    }
}

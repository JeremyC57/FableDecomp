// Leaf: ?Initialise@CAIStateGroup_IceTrollIcicleAssault@@MAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// CAIStateGroup_IceTrollIcicleAssault::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) -- forwards to CAIStateGroupBase::Initialise (0x008FCEC0; retail tail-jumps to it).
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   _CAIStateGroup_IceTrollIcicleAssault_Initialise becomes the virtual member of the polymorphic class in
//   engine/CAIStateGroup_IceTrollIcicleAssault.h (model FABLE_RETAIL_CAISTATEGROUP_ICETROLLICICLEASSAULT_VIRTUAL_MODEL (generated
//   here); defines FABLE_RETAIL_CAISTATEGROUP_ICETROLLICICLEASSAULT_VIRTUAL_MODEL, FABLE_RETAIL_CAISTATEGROUPBASE,
//   FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS). NF429 G2 (JMP shape); callee: the target's RTTI-port label = its landed self.
#define FABLE_RETAIL_CAISTATEGROUP_ICETROLLICICLEASSAULT_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_IceTrollIcicleAssault.h"
void CAIStateGroup_IceTrollIcicleAssault::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
}

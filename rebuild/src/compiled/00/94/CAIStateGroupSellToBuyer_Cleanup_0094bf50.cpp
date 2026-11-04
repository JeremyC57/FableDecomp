// CAIStateGroup_SellToBuyer::Cleanup() -- when the state group ends, make the brain's creature stop speaking
// Leaf: ?Cleanup@CAIStateGroup_SellToBuyer@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094BF50, 11 bytes: `mov eax,[ecx+4] ; mov ecx,[eax+18h] ; jmp 0x00864580`
//   (NCreatureAI::StopSpeaking(*PBrain->Creature), __fastcall creature in ecx, tail call).
// Identity (two sources): (1) slot 9 of the CAIStateGroup_SellToBuyer vtable 0x0128BF84 (RTTI .?AVCAIStateGroup_SellToBuyer@@ :
//   CAIStateGroup_WorkBase<CAIStateGroupBase> : CAIStateGroupBase), the base Cleanup slot (engine/CAIStateGroupBase.h); (2) Ego_r.pdb
//   ?Cleanup@CAIStateGroup_SellToBuyer@@UAEXXZ (public virtual, donor 0x00970310). The tail target 0x00864580 is the landed
//   ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z (both PDBs: a free __fastcall function in namespace NCreatureAI).
// Callees: ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z 0x00864580.
// Header: engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS, _GETCREATURE), engine/CAIBrain.h (Creature +0x18,
//   FABLE_RETAIL_CAIBRAIN_GETCREATURE), engine/CThingAICreature.h. engine/CAIStateGroup_SellToBuyer.h is the flat Ego_r layout, so the
//   derived class is declared here (only the override this row defines).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
namespace NCreatureAI {
void __fastcall StopSpeaking(CThingCreatureBase& creature);   // 0x00864580
}
class CAIStateGroup_SellToBuyer : public CAIStateGroupBase {   // retail base CAIStateGroup_WorkBase<CAIStateGroupBase> adds no storage here
public:
    virtual void Cleanup();                             // slot 9
};
void CAIStateGroup_SellToBuyer::Cleanup()
{
    NCreatureAI::StopSpeaking(GetCreature());
}
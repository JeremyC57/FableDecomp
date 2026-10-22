#include "engine/CAIStateGroup_BanditKingLunge.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIBrain;
struct CThingAICreature;
struct CCharString;

typedef long EScriptAIPriority;


extern "C" void __fastcall CAIStateGroupBase_Initialise(CAIStateGroup_BanditKingLunge* stateGroup, void* edx, CAIBrain& brain, CThingAICreature& creature, long param3, EScriptAIPriority priority, CCharString const& name);

extern "C" void __fastcall CAIStateGroup_BanditKingLunge_Initialise(CAIStateGroup_BanditKingLunge* stateGroup, void* edx, CAIBrain& brain, CThingAICreature& creature, long param3, EScriptAIPriority priority, CCharString const& name)
{
    stateGroup->TimeOfLastLunge = 0;
    CAIStateGroupBase_Initialise(stateGroup, edx, brain, creature, param3, priority, name);
}
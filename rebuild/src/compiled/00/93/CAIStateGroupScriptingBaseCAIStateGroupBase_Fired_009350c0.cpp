// CAIStateGroup_ScriptingBase<CAIStateGroupBase>::Fired -- schedule the next scripted action.
// Flags:
// Retail: 009350c0,159 bytes,protected thiscall with no parameters.
// Leaf: ?Fired@?$CAIStateGroup_ScriptingBase@VCAIStateGroupBase@@@@IAEXXZ
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"

float FABLE_FASTCALL GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

#include "engine/CTCAIScratchpad.h"
#include "engine/CAIStateGroup_ScriptingBase.h"
template<> void CAIStateGroup_ScriptingBase<CAIStateGroupBase>::Fired(){
 CTCAIScratchpad* ai_scratchpad;
 GetCreature().GetTC(&ai_scratchpad);
 int frame_offset=ai_scratchpad->GetActionFrequency()*CMainGameComponent::GetConstantFPS();
 frame_offset*=(1.0f+ai_scratchpad->GetActionFrequencyVariation())-GFFloatRandom(ai_scratchpad->GetActionFrequencyVariation()*2.0f,GetPWorldSeed());
 NextFrameFireable=CWorld::GetFrame()+frame_offset;
}
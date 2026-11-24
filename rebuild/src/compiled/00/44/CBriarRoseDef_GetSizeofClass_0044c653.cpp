// Leaf: ?GetSizeofClass@CBriarRoseDef@@UAEKXZ
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#include "engine/CThingAICreature.h"
#include "engine/CBriarRoseDef.h"
#pragma optimize("s",on)
unsigned long CBriarRoseDef::GetSizeofClass()
{
    return 0x54;
}
// Flatten route (flatten_pilot v4, NF324): CBriarRoseDef_GetSizeofClass -> CBriarRoseDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CBriarRoseDef.h under FABLE_RETAIL_BRIAR_ROSE (preamble of CAIStateGroupBriarRose_Update_00956e70.cpp).

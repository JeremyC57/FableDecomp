// CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList(std::vector<CThing*>&) -- collect unoccupied preferred fire points near the creature
// Leaf: ?GetFirePointList@CAIStateGroup_RangedCombatUsePreferredFirePoint@@IAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: 0x0095D4C0,130 bytes,thiscall
#define FABLE_RETAIL_CAISTATEGROUP_RANGEDCOMBATUSEPREFERREDFIREPOINT
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CENTRE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFirePointFilters.h"
#include "engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h"
void CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList(std::vector<CThing*>& fire_points){
 PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxF(GetCreature().GetPos(),30.0f).ToC2DBoxI(),GFFilter_Bind2(CThingFilter_IsPreferredFirePoint(),std::not1(CThingFilter_IsOccupiableOccupied())),fire_points);
}
// 007f70d0 CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind4<CThingFilter_IsAvailableForCreatureInteraction, CThingFilter_In2DRangeLimit, CThingFilter_CanSeeEachOther, std::unary_negate<CThingFilter_IsUnconscious> >, CThingCompare_Nearest, CThingAICreature>(..) const -- the nearest AI creature under the area that is available for the interaction, within the range limits, mutually visible and conscious
// Leaf: ??$GetPBestThingInRoughArea@V?$CFilter_Bind4@VCThingFilter_IsAvailableForCreatureInteraction@@VCThingFilter_In2DRangeLimit@@VCThingFilter_CanSeeEachOther@@V?$unary_negate@VCThingFilter_IsUnconscious@@@std@@@@VCThingCompare_Nearest@@VCThingAICreature@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind4@VCThingFilter_IsAvailableForCreatureInteraction@@VCThingFilter_In2DRangeLimit@@VCThingFilter_CanSeeEachOther@@V?$unary_negate@VCThingFilter_IsUnconscious@@@std@@@@ABVCThingCompare_Nearest@@PAPAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F70D0, thiscall `ret 10h`.
// Identity: Ego_r.pdb 0x007F2390 + FableWin.pdb 0x02537170 name this instance; the landed caller CTCTargetingAI::
//   GetPBestCreatureForInteractionTypeWithinRangeLimit 0x007F67C0 binds it. The body calls CThingFilter_IsAvailableForCreatureInteraction 0x006A7430,
//   CThingFilter_In2DRangeLimit 0x006A7700, CThingFilter_CanSeeEachOther 0x006A78D0 and CThingFilter_IsUnconscious 0x006A73E0 (negated) in the
//   Bind4 order, then CThingCompare_Nearest::operator()<CThingAICreature> 0x0065DA97.
// NF107 (round 10, q11280 / q11283): ported off its local model (a CThingSearchTools_Calls stand-in and its local filters / Filters) onto the
//   shared headers as an explicit instantiation of the engine/CThingSearchTools.h GetPBestThingInRoughArea template (BESTROUGH_BODY + the NF107
//   opt-in BESTROUGH_THISEND with the byte-offset cell expression, the 0x007F7340 form), with the define / include set of its landed caller
//   0x007F67C0 (the same retail TU); the filters with a shared header come from it (TargetingInteractionFilters_pass24.h, CThingFilter_CanSeeEachOther.h,
//   CFilter_Bind4.h). It now binds the Ego_r.pdb spellings of 0x0049C6F0 / 0x0049C700.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_TARGETINGINTERACTION_FILTERS_PASS24
#define FABLE_RETAIL_KID_HURT_SELF_PASS23
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTargetingDef.h"
#include "engine/CTCTargetingAI.h"
#include "engine/CWorldMap.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/TargetingInteractionFilters_pass24.h"
#include "engine/CThingFilter_CanSeeEachOther.h"
#include "engine/CFilter_Bind4.h"
// Kept from the landed caller 0x007F67C0 (no shared header for these classes):
class CThingFilter_In2DRangeLimit {
public:
 CThingFilter_In2DRangeLimit(const C3DVector& pos,float min_range,float max_range):Pos(pos),MinRange(min_range),MaxRange(max_range){}
 bool operator()(const CThing&)const;                           // 0x006A7700
private:
 C3DVector Pos;
 float MinRange;
 float MaxRange;
};
class CThingFilter_IsUnconscious : public std::unary_function<CThing,bool> {
public:
 bool operator()(const CThing&)const;                           // 0x006A73E0
};
template bool CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind4<CThingFilter_IsAvailableForCreatureInteraction,CThingFilter_In2DRangeLimit,CThingFilter_CanSeeEachOther,std::unary_negate<CThingFilter_IsUnconscious> >,CThingCompare_Nearest,CThingAICreature>(const C2DBoxI&,const CFilter_Bind4<CThingFilter_IsAvailableForCreatureInteraction,CThingFilter_In2DRangeLimit,CThingFilter_CanSeeEachOther,std::unary_negate<CThingFilter_IsUnconscious> >&,const CThingCompare_Nearest&,CThingAICreature**) const;

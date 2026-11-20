// 007f7340 CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind3<CThingFilter_IsAvailableForCreatureInteraction, CThingFilter_IsAICreatureAwareOf, std::unary_negate<CThingFilter_IsIncapacitated> >, CThingCompare_Nearest, CThingAICreature>(const C2DBoxI&, const Filter&, const CThingCompare_Nearest&, CThingAICreature**) const -- the nearest AI creature under the area that is available for the interaction, aware of the source and not incapacitated
// Leaf: ??$GetPBestThingInRoughArea@V?$CFilter_Bind3@VCThingFilter_IsAvailableForCreatureInteraction@@VCThingFilter_IsAICreatureAwareOf@@V?$unary_negate@VCThingFilter_IsIncapacitated@@@std@@@@VCThingCompare_Nearest@@VCThingAICreature@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsAvailableForCreatureInteraction@@VCThingFilter_IsAICreatureAwareOf@@V?$unary_negate@VCThingFilter_IsIncapacitated@@@std@@@@ABVCThingCompare_Nearest@@PAPAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F7340, thiscall `ret 10h`.
// Identity: Ego_r.pdb 0x007F25F0 + FableWin.pdb 0x025378D0 name this instance; the landed caller CTCTargetingAI::GetPBestCreatureForInteractionType
//   0x007F68B0 binds it. The body calls CThingFilter_IsAvailableForCreatureInteraction 0x006A7430, CThingFilter_IsAICreatureAwareOf 0x006A7610 and
//   CThingFilter_IsIncapacitated 0x006A73F0 (negated) in the Bind3 order, and CThingCompare_Nearest::operator()<CThingAICreature> 0x0065DA97.
// NF107 (round 10, q11280 / q11283): ported off its local model (a CThingSearchTools_Calls stand-in and its local filters / Filters) onto the
//   shared headers as an explicit instantiation of the engine/CThingSearchTools.h GetPBestThingInRoughArea template (BESTROUGH_BODY), with
//   the define / include set of its landed caller 0x007F68B0 (the same retail TU). It now binds the Ego_r.pdb spellings of 0x0049C6F0 / 0x0049C700.
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
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_GETSIGHTRADIUS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
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
#define FABLE_RETAIL_TARGETINGINTERACTION_FILTERS_PASS24
#include "engine/TargetingInteractionFilters_pass24.h"
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#include "engine/CFilter_Bind3.h"
#include "engine/CTCPerceiveThing.h"
template bool CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind3<CThingFilter_IsAvailableForCreatureInteraction,CThingFilter_IsAICreatureAwareOf,std::unary_negate<CThingFilter_IsIncapacitated> >,CThingCompare_Nearest,CThingAICreature>(const C2DBoxI&,const CFilter_Bind3<CThingFilter_IsAvailableForCreatureInteraction,CThingFilter_IsAICreatureAwareOf,std::unary_negate<CThingFilter_IsIncapacitated> >&,const CThingCompare_Nearest&,CThingAICreature**) const;

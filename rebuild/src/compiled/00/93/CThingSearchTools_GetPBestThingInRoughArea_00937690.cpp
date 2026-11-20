// CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind3<CThingFilter_IsAICreatureAwareOf, CThingFilter_IsUnconscious, CThingFilter_IsAnAlly>, CThingCompare_Nearest, CThingCreatureBase>(const C2DBoxI& area, const CFilter_Bind3<...>& filter, const CThingCompare_Nearest& compare, CThingCreatureBase** out) const -- the nearest creature in the mapwho cells under the area (each thing once per search id) that the AI creature is aware of, unconscious and an ally, through the out pointer; whether one was found (the scratch map list is cleared)
// Leaf: ??$GetPBestThingInRoughArea@V?$CFilter_Bind3@VCThingFilter_IsAICreatureAwareOf@@VCThingFilter_IsUnconscious@@VCThingFilter_IsAnAlly@@@@VCThingCompare_Nearest@@VCThingCreatureBase@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsAICreatureAwareOf@@VCThingFilter_IsUnconscious@@VCThingFilter_IsAnAlly@@@@ABVCThingCompare_Nearest@@PAPAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00937690, thiscall `ret 0x10`; caller 0x00937C20 (binds a non-template stand-in: NAME_FIXES).
// Identity: Ego_r.pdb 0x0095A940 names this instance; the body calls CThingFilter_IsAICreatureAwareOf::operator() 0x006A7610, CThingFilter_IsUnconscious
//   0x006A73E0, CThingFilter_IsAnAlly 0x006A7A40 (engine/CFilter_Bind3.h order) and CThingCompare_Nearest::operator()<CThingCreatureBase> 0x00782560 out of line.
// NF110 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / the thing classes / the filters / the thing map / a local
//   CThingSearchTools, which bound the GetMapsInArea / GetCellExtents stand-ins, a non-template CThingCompare_Nearest::operator() stand-in and a QAE
//   self leaf) onto the shared headers as an explicit instantiation of the engine/CThingSearchTools.h GetPBestThingInRoughArea template (BESTROUGH_BODY
//   + NF107 BESTROUGH_THISEND with the byte-offset cell branch), on the define / include set of the landed NF107 port 0x00937420 (the same retail TU,
//   CFilter_Bind3 for its Bind4). It now binds the Ego_r.pdb spellings of 0x0049C6F0 / 0x0049C700 and CThingCompare_Nearest::operator()<CThingCreatureBase>.
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
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE
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
#include "engine/CFilter_Bind3.h"
// Kept from the landed rows (no shared header for these classes; the NF107 0x00937420 spelling of the same TU):
class CThingFilter_IsUnconscious : public std::unary_function<CThing,bool> {
public:
 bool operator()(const CThing&)const;                           // 0x006A73E0
};
class CThingFilter_IsAnAlly { public:CThing* Thing;bool operator()(const CThing&)const;}; // 0x006a7a40
template bool CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind3<CThingFilter_IsAICreatureAwareOf,CThingFilter_IsUnconscious,CThingFilter_IsAnAlly>,CThingCompare_Nearest,CThingCreatureBase>(const C2DBoxI&,const CFilter_Bind3<CThingFilter_IsAICreatureAwareOf,CThingFilter_IsUnconscious,CThingFilter_IsAnAlly>&,const CThingCompare_Nearest&,CThingCreatureBase**) const;

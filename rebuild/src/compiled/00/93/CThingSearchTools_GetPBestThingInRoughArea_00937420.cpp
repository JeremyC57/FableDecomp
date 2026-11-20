// 00937420 CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind4<CThingFilter_IsAICreatureAwareOf, CThingFilter_IsUnconscious, CThingFilter_IsAnAlly, std::unary_negate<CThingFilter_HasBeenDiscovered> >, CThingCompare_Nearest, CThingCreatureBase>(..) const -- the nearest creature under the area the AI creature is aware of, unconscious, an ally and not yet discovered
// Leaf: ??$GetPBestThingInRoughArea@V?$CFilter_Bind4@VCThingFilter_IsAICreatureAwareOf@@VCThingFilter_IsUnconscious@@VCThingFilter_IsAnAlly@@V?$unary_negate@VCThingFilter_HasBeenDiscovered@@@std@@@@VCThingCompare_Nearest@@VCThingCreatureBase@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind4@VCThingFilter_IsAICreatureAwareOf@@VCThingFilter_IsUnconscious@@VCThingFilter_IsAnAlly@@V?$unary_negate@VCThingFilter_HasBeenDiscovered@@@std@@@@ABVCThingCompare_Nearest@@PAPAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00937420, thiscall `ret 10h`.
// Identity: Ego_r.pdb 0x0095A6E0 + FableWin.pdb 0x02B70F70 name this instance; the body calls CThingFilter_IsAICreatureAwareOf 0x006A7610,
//   CThingFilter_IsUnconscious 0x006A73E0, CThingFilter_IsAnAlly 0x006A7A40 and CThingFilter_HasBeenDiscovered 0x006A84B0 (negated) in the Bind4
//   order, then CThingCompare_Nearest::operator()<CThingCreatureBase> 0x00782560. The landed caller 0x00937B10 binds a non-template stand-in
//   spelling (NAME_FIXES).
// NF107 (round 10, q11280 / q11283): ported off its local model (a CThingSearchTools_Calls stand-in and its local filters / Filters) onto the
//   shared headers as an explicit instantiation of the engine/CThingSearchTools.h GetPBestThingInRoughArea template (BESTROUGH_BODY + the NF107
//   opt-in BESTROUGH_THISEND with the byte-offset cell expression), on the define / include set of 0x007F7340 (the CTCTargetingAI search
//   TU). It now binds the Ego_r.pdb spellings of 0x0049C6F0 / 0x0049C700.
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
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#include "engine/CFilter_Bind4.h"
// Kept from the landed row (no shared header for these classes); CThingFilter_IsUnconscious as the landed caller 0x007F67C0 declares it and
// CThingFilter_HasBeenDiscovered over std::unary_function<CThing,bool> (std::unary_negate needs its argument_type):
class CThingFilter_IsUnconscious : public std::unary_function<CThing,bool> {
public:
 bool operator()(const CThing&)const;                           // 0x006A73E0
};
class CThingFilter_IsAnAlly { public:CThing* Thing;bool operator()(const CThing&)const;}; // 0x006a7a40
class CThingFilter_HasBeenDiscovered : public std::unary_function<CThing,bool> { public:bool operator()(const CThing&)const;}; // 0x006a84b0
template bool CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind4<CThingFilter_IsAICreatureAwareOf,CThingFilter_IsUnconscious,CThingFilter_IsAnAlly,std::unary_negate<CThingFilter_HasBeenDiscovered> >,CThingCompare_Nearest,CThingCreatureBase>(const C2DBoxI&,const CFilter_Bind4<CThingFilter_IsAICreatureAwareOf,CThingFilter_IsUnconscious,CThingFilter_IsAnAlly,std::unary_negate<CThingFilter_HasBeenDiscovered> >&,const CThingCompare_Nearest&,CThingCreatureBase**) const;

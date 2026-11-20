// CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_ScaresBirds, CThingCompare_Nearest, CThingCreatureBase>(const C2DBoxI& area, const CThingFilter_ScaresBirds& filter, const CThingCompare_Nearest& compare, CThingCreatureBase** out) const -- the nearest creature in the mapwho cells under the area (each thing once per search id) that scares the bird, through the out pointer; whether one was found (the scratch map list is cleared)
// Leaf: ??$GetPBestThingInRoughArea@VCThingFilter_ScaresBirds@@VCThingCompare_Nearest@@VCThingCreatureBase@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABVCThingFilter_ScaresBirds@@ABVCThingCompare_Nearest@@PAPAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009147E0, thiscall `ret 0x10`; caller 0x00914650 (binds this spelling).
// Identity: Ego_r.pdb 0x00938430 names this instance; the body expands CThingFilter_ScaresBirds::operator() (engine/CThingFilter_ScaresBirds.h) and calls
//   CThingCompare_Nearest::operator()<CThingCreatureBase> 0x00782560 out of line (the 0x00914650 set does not define CTHINGCOMPARE_NEAREST_OUTOFLINE).
// NF110 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / CThingDefView / the thing map / a local CThingSearchTools, which
//   bound the GetMapsInArea / GetCellExtents stand-ins, a non-template CThingCompare_Nearest::operator() stand-in and a QAE self leaf) onto the shared
//   headers as an explicit instantiation of the engine/CThingSearchTools.h GetPBestThingInRoughArea template (BESTROUGH_BODY + NF107 BESTROUGH_THISEND
//   with the byte-offset cell branch), on the define / include set of its landed caller 0x00914650 (CAIStateGroup_BirdStartled::IsRunnable, the
//   retail TU-mate: this instance follows it) + FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS (T's type flags 0x0F). It now binds the Ego_r.pdb
//   spellings of 0x0049C6F0 / 0x0049C700 and CThingCompare_Nearest::operator()<CThingCreatureBase> (0x00782560, the callee row's own spelling).
// Fidelity: the inline CThingFilter_ScaresBirds::operator() text is the landed TU-mate 0x00914650's, verbatim (FableWin.pdb ??RCThingFilter_ScaresBirds@@QBE_NABVCThingCreatureBase@@@Z
//   0x02B8E030; expanded here in retail: no filter call in 0x009147E0); engine/CThingFilter_ScaresBirds.h declares it only.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_TYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGDEADCREATURE_STATGETTYPE
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGATORTYPE
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingDeadCreature.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFWorldDistance.h"
#include "engine/CThingFilter_ScaresBirds.h"
#include "engine/CAIStateGroup_BirdStartled.h"
inline bool CThingFilter_ScaresBirds::operator()(const CThingCreatureBase& thing) const
{
    return thing.GetType() != CThingDeadCreature::StatGetType() && thing.GetDefGlobalIndex() != Bird->GetDefGlobalIndex();
}
template bool CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_ScaresBirds,CThingCompare_Nearest,CThingCreatureBase>(const C2DBoxI&,const CThingFilter_ScaresBirds&,const CThingCompare_Nearest&,CThingCreatureBase**) const;

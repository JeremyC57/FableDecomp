// CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind3<CThingFilter_IsInterestingToCreature, CThingFilter_IsInCreatureFOV, CThingFilter_IsPerceptible>, CThingCompare_Nearest, CThingPhysical>(const C2DBoxI& area, const CFilter_Bind3<...>& filter, const CThingCompare_Nearest& compare, CThingPhysical** out) const -- the nearest physical thing in the mapwho cells under the area (each thing once per search id) that is interesting to the creature, in its field of view and perceptible, through the out pointer; whether one was found (the scratch map list is cleared)
// Leaf: ??$GetPBestThingInRoughArea@V?$CFilter_Bind3@VCThingFilter_IsInterestingToCreature@@VCThingFilter_IsInCreatureFOV@@VCThingFilter_IsPerceptible@@@@VCThingCompare_Nearest@@VCThingPhysical@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsInterestingToCreature@@VCThingFilter_IsInCreatureFOV@@VCThingFilter_IsPerceptible@@@@ABVCThingCompare_Nearest@@PAPAVCThingPhysical@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F5D20, thiscall `ret 0x10`; caller 0x007F5AE0 (CTCTargetingBase::UpdateNearestInterestingThing, binds this spelling).
// Identity: Ego_r.pdb 0x007F1330 names this instance; the body calls CThingFilter_IsInterestingToCreature::operator() 0x006A7120, CThingFilter_IsInCreatureFOV
//   0x006A73C0, CThingFilter_IsPerceptible 0x006A8450 (engine/NTargeting_ThingFilters.h, engine/CFilter_Bind3.h order) and
//   CThingCompare_Nearest::operator()<CThingPhysical> 0x007F5730 out of line.
// NF110 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / the thing classes / the thing map / a local CThingSearchTools,
//   which bound the GetMapsInArea / GetCellExtents stand-ins and a QAE self leaf) onto the shared headers as an explicit instantiation of the
//   engine/CThingSearchTools.h GetPBestThingInRoughArea template. It now binds the Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE...
//   (0x0049C6F0) and ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE... (0x0049C700); the self leaf is the Ego_r.pdb instance spelling.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_CTHINGPHYSICAL_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NTargeting_ThingFilters.h"
#include "engine/CFilter_Bind3.h"
template bool CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind3<CThingFilter_IsInterestingToCreature,CThingFilter_IsInCreatureFOV,CThingFilter_IsPerceptible>,CThingCompare_Nearest,CThingPhysical>(const C2DBoxI&,const CFilter_Bind3<CThingFilter_IsInterestingToCreature,CThingFilter_IsInCreatureFOV,CThingFilter_IsPerceptible>&,const CThingCompare_Nearest&,CThingPhysical**) const;

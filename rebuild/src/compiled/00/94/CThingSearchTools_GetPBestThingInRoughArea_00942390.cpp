// CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>, CThingCompare_Nearest, CThingCreatureBase>(const C2DBoxI& area, const CFilter_Bind2<...>& filter, const CThingCompare_Nearest& compare, CThingCreatureBase** out) const -- the nearest creature in the mapwho cells under the area (each thing once per search id) that is not the excluded thing and is very close and motionless, through the out pointer; whether one was found (the scratch map list is cleared)
// Leaf: ??$GetPBestThingInRoughArea@V?$CFilter_Bind2@VCThingFilter_IsNot@@VCIsVeryCloseMotionlessCreature@@@@VCThingCompare_Nearest@@VCThingCreatureBase@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_IsNot@@VCIsVeryCloseMotionlessCreature@@@@ABVCThingCompare_Nearest@@PAPAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00942390, thiscall `ret 0x10`; caller 0x00941FC0 (binds this spelling).
// Identity: Ego_r.pdb 0x009661F0 names this instance; the body calls CThingFilter_IsNot::operator() 0x006A76A0, CIsVeryCloseMotionlessCreature::operator()
//   0x00942270 (both by const reference, as both PDBs spell them) and CThingCompare_Nearest::operator()<CThingCreatureBase> 0x00782560 out of line.
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
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE
class CThingAICreature;
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
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CIsVeryCloseMotionlessCreature.h"
template bool CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind2<CThingFilter_IsNot,CIsVeryCloseMotionlessCreature>,CThingCompare_Nearest,CThingCreatureBase>(const C2DBoxI&,const CFilter_Bind2<CThingFilter_IsNot,CIsVeryCloseMotionlessCreature>&,const CThingCompare_Nearest&,CThingCreatureBase**) const;

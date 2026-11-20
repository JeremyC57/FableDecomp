// CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_IsPotentialCustomer, CThingCompare_Nearest, CThingCreatureBase>(const C2DBoxI& area, const CThingFilter_IsPotentialCustomer& filter, const CThingCompare_Nearest& compare, CThingCreatureBase** out) const -- the nearest creature in the mapwho cells under the area (each thing once per search id) that is a potential customer, through the out pointer; whether one was found (the scratch map list is cleared)
// Leaf: ??$GetPBestThingInRoughArea@VCThingFilter_IsPotentialCustomer@@VCThingCompare_Nearest@@VCThingCreatureBase@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABVCThingFilter_IsPotentialCustomer@@ABVCThingCompare_Nearest@@PAPAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079A050, 554 bytes, thiscall `ret 0x10`; caller 0x00799900 (CTCShopKeeper::AcquireNewCustomer, binds this spelling).
// Identity: Ego_r.pdb 0x0079FA40 names this instance; the body calls CThingFilter_IsPotentialCustomer::operator() 0x00799E30 and
//   CThingCompare_Nearest::operator()<CThingCreatureBase> 0x00782560 out of line.
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
// Kept from the landed row (no shared header defines this filter; its operator() is out of line, Ego_r.pdb / FableWin.pdb spelling):
class CThingFilter_IsPotentialCustomer { public:
    bool operator()(const CThing& thing) const;   // 0x00799E30
};
template bool CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_IsPotentialCustomer,CThingCompare_Nearest,CThingCreatureBase>(const C2DBoxI&,const CThingFilter_IsPotentialCustomer&,const CThingCompare_Nearest&,CThingCreatureBase**) const;

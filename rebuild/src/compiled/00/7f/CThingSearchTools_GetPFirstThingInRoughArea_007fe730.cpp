// CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNot, CThingFilter_In2DRange>, CThingCreatureBase>(const C2DBoxI& area, const CFilter_Bind3<...>& filter_func, CThingCreatureBase** thing) const -- the first creature in the mapwho cells under the area (each thing once per search id) that is neither excluded thing and lies in the 2D range, through the out pointer (true; the scratch map list is left as is); else the scratch list is cleared and false
// Leaf: ??$GetPFirstThingInRoughArea@V?$CFilter_Bind3@VCThingFilter_IsNot@@V1@VCThingFilter_In2DRange@@@@VCThingCreatureBase@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsNot@@V1@VCThingFilter_In2DRange@@@@PAPAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FE730, 605 bytes, thiscall `ret 0xc`; callers 0x007FF0C0 and 0x007FEAF0 (respelled to this spelling by NF110).
// Identity: Ego_r.pdb 0x007EF030 names this instance; the body calls CThingFilter_IsNot::operator() 0x006A76A0 twice, then CThingFilter_In2DRange::operator()
//   0x006A76B0 (engine/CFilter_Bind3.h order).
// NF110 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / the thing classes / the thing map / a local CThingSearchTools,
//   which bound the GetMapsInArea / GetCellExtents stand-ins and a QAE self leaf) onto the shared headers as an explicit instantiation of the
//   engine/CThingSearchTools.h GetPFirstThingInRoughArea template (NF110 opt-in FIRSTROUGH_EGOD: the landed 0x009871C0 body), on the 0x009871C0
//   define / include set. It now binds the Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... (0x0049C6F0) and
//   ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE... (0x0049C700); the self leaf is the Ego_r.pdb instance spelling.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"
template bool CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind3<CThingFilter_IsNot,CThingFilter_IsNot,CThingFilter_In2DRange>,CThingCreatureBase>(const C2DBoxI&,const CFilter_Bind3<CThingFilter_IsNot,CThingFilter_IsNot,CThingFilter_In2DRange>&,CThingCreatureBase**) const;

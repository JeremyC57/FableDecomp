// CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, std::vector<CThing*>, CFilter_Bind4<CThingFilter_IsNot, CThingFilter_IsNot, CThingFilter_IsTargetable, CThingFilter_In2DRange> >(const C2DBoxI& area, const Filter& filter, std::vector<CThing*>& result) const -- every creature in the maps under the area that passes the two exclusions, the targetable test and the 2D range test, pushed into the result
// Leaf: ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@V?$CFilter_Bind4@VCThingFilter_IsNot@@V1@VCThingFilter_IsTargetable@@VCThingFilter_In2DRange@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind4@VCThingFilter_IsNot@@V1@VCThingFilter_IsTargetable@@VCThingFilter_In2DRange@@@@AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A3CA0, 646 bytes, `ret 0Ch`.
// Identity: Ego_r.pdb / FableWin.pdb name set (the only Bind4<IsNot,IsNot,IsTargetable,In2DRange> instance); the body's filters are exactly
//   these four (CThingFilter_IsNot 0x006A76A0, CThingFilter_IsTargetable 0x006A84F0, CThingFilter_In2DRange 0x006A76B0); row 0x007A0D90
//   passes the box, the GFFilter_Bind4 result and a vector<CThing*>. The landed file name (GetPBestThingInRoughArea<...>) is a propagated label.
// Body: engine/CThingSearchTools.h ORIGINAL_ROUGHAREA (as 0x0095C7C0): GetAllThingMapsInArea 0x0049C6F0, the search statics, per map
//   ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, per layer the scaled cells; push_back's growth path vector<CThing*>::_M_insert_overflow.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsTargetable.h"
#include "engine/CThingFilter_In2DRange.h"

template void CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, std::vector<CThing*>,
    CFilter_Bind4<CThingFilter_IsNot, CThingFilter_IsNot, CThingFilter_IsTargetable, CThingFilter_In2DRange> >(const C2DBoxI&,
    const CFilter_Bind4<CThingFilter_IsNot, CThingFilter_IsNot, CThingFilter_IsTargetable, CThingFilter_In2DRange>&, std::vector<CThing*>&) const;

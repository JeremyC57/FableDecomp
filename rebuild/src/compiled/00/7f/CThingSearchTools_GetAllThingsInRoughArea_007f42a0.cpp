// CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, std::vector<CThingGameObject*>, CFilter_Bind2<CThingFilter_In3DRange, std::unary_negate<CThingFilter_HasTC> > >(const C2DBoxI& area, const Filter& filter, Container& result) const -- every creature in the maps under the area that is in 3D range and has no TC of the filter's type, pushed into the game-object result
// Leaf: ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$vector@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_In3DRange@@V?$unary_negate@VCThingFilter_HasTC@@@std@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_In3DRange@@V?$unary_negate@VCThingFilter_HasTC@@@std@@@@AAV?$vector@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F42A0, `ret 0Ch`.
// Identity: Ego_r.pdb ??$GetAllThingsInRoughArea<CThingCreatureBase, vector<CThingGameObject*>, CFilter_Bind2<In3DRange, unary_negate<HasTC>>> (donor 0x007ED5F0);
//   the body calls CThingFilter_In3DRange 0x006A7780 and CThingFilter_HasTC 0x006A80A0 (negated); push_back grows through
//   vector<CThingGameObject*>::_M_insert_overflow 0x00788FFC.
// Body: engine/CThingSearchTools.h ORIGINAL_ROUGHAREA (as 0x0095C7C0 / 0x007A3CA0): GetAllThingMapsInArea 0x0049C6F0, the search statics,
//   per map ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, per layer the scaled cells. The landed file name (GetPBestThingInRoughArea<...>)
//   is a propagated label.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingGameObject.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_In3DRange.h"
#include "engine/CThingFilter_HasTC.h"

template void CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, std::vector<CThingGameObject*>,
    CFilter_Bind2<CThingFilter_In3DRange, std::unary_negate<CThingFilter_HasTC> > >(const C2DBoxI&,
    const CFilter_Bind2<CThingFilter_In3DRange, std::unary_negate<CThingFilter_HasTC> >&, std::vector<CThingGameObject*>&) const;

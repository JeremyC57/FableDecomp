// CThingSearchTools::GetAllThingsInRoughArea<const CThing, std::vector<const CThing*>, CFilter_Bind3<CThingFilter_HasPhysicsMesh, CThingFilter_RadiusIntersectsInfiniteLine, CThingFilter_IsNot> >(const C2DBoxI& area, const Filter& filter, Container& result) const -- every thing in the maps under the area that has a physics mesh, whose radius meets the infinite line and that is not the excluded thing, pushed into the const result
// Leaf: ??$GetAllThingsInRoughArea@$$CBVCThing@@V?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@V?$CFilter_Bind3@VCThingFilter_HasPhysicsMesh@@VCThingFilter_RadiusIntersectsInfiniteLine@@VCThingFilter_IsNot@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_HasPhysicsMesh@@VCThingFilter_RadiusIntersectsInfiniteLine@@VCThingFilter_IsNot@@@@AAV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00746560, `ret 0Ch`.
// Identity: Ego_r.pdb / FableWin.pdb instantiation; the body calls CThingFilter_HasPhysicsMesh 0x006A6F50, CThingFilter_RadiusIntersectsInfiniteLine 0x006A7050
//   and CThingFilter_IsNot 0x006A76A0; push_back grows through vector<const CThing*>::_M_insert_overflow 0x0049C370.
// Body: engine/CThingSearchTools.h ORIGINAL_ROUGHAREA (as 0x0095C7C0 / 0x007A3CA0): GetAllThingMapsInArea 0x0049C6F0, the search statics,
//   per map ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, per layer the scaled cells. The landed file name (GetPBestThingInRoughArea<...>)
//   is a propagated label.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_TYPEDLOCAL
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_HasPhysicsMesh.h"
#include "engine/CThingFilter_RadiusIntersectsInfiniteLine.h"
#include "engine/CThingFilter_IsNot.h"

template void CThingSearchTools::GetAllThingsInRoughArea<const CThing, std::vector<const CThing*>,
    CFilter_Bind3<CThingFilter_HasPhysicsMesh, CThingFilter_RadiusIntersectsInfiniteLine, CThingFilter_IsNot> >(const C2DBoxI&,
    const CFilter_Bind3<CThingFilter_HasPhysicsMesh, CThingFilter_RadiusIntersectsInfiniteLine, CThingFilter_IsNot>&, std::vector<const CThing*>&) const;

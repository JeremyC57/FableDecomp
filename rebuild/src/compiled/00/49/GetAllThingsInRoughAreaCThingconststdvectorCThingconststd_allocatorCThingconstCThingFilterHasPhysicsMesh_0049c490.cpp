// CThingSearchTools::GetAllThingsInRoughArea<const CThing, std::vector<const CThing*>, CThingFilter_HasPhysicsMesh>(...) -- finds physics mesh candidates in the rough area.
// Leaf: ??$GetAllThingsInRoughArea@$$CBVCThing@@V?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@VCThingFilter_HasPhysicsMesh@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasPhysicsMesh@@AAV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@Z
// Flags: default
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#include "engine/CThing.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasPhysicsMesh.h"

template void CThingSearchTools::GetAllThingsInRoughArea<const CThing, std::vector<const CThing*>, CThingFilter_HasPhysicsMesh>(
    const C2DBoxI&, const CThingFilter_HasPhysicsMesh&, std::vector<const CThing*>&) const;
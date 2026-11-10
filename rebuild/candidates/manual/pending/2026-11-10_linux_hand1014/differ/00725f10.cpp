// CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, vector<CThingCreatureBase*>, CThingFilter_IsNot>
// Grade: DIFFER(733v590) -- header ROUGHAREABODY instance (Leaf settled: <CThingCreatureBase, vector<CThingCreatureBase*>, CThingFilter_IsNot>): ours frame 0x58 v retail 0x50, this in ebx v retail edi; retail walks the layer table by a pointer (esp+18h = 0x0137DADC, &LayerInfo[0].ScaleShift) against g_NoMapwhoLevels 0x013BA848, ours keeps an index plus two extra zeroed counters; ours is 143 bytes longer overall, so the shared body is not this instance's source shape
// Leaf: ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$vector@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@VCThingFilter_IsNot@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_IsNot@@AAV?$vector@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_IsNot.h"
#include "stlport/fable_containers.h"

template void CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase,std::vector<CThingCreatureBase*>,CThingFilter_IsNot>(
    const C2DBoxI&,const CThingFilter_IsNot&,std::vector<CThingCreatureBase*>&) const;

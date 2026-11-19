// CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, std::vector<CThingCreatureBase*>, CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange> >(const C2DBoxI&, const CFilter_Bind2<..>&, std::vector<CThingCreatureBase*>&) const -- collect every creature in the mapwho cells under the area that passes both filters (not the excluded thing, inside the 2D range)
// Leaf: ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$vector@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_IsNot@@VCThingFilter_In2DRange@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_IsNot@@VCThingFilter_In2DRange@@@@AAV?$vector@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3C50, 607 bytes.
// Identity: Ego_r.pdb 0x00825D60 + FableWin.pdb 0x0286D1E0 (entry q11420, NF93 part 3 "claim holds"); the landed caller 0x008DD7E0 binds this
//   spelling.
// NF98 (round 8, q11420): ported off its local model (RetailVector / CThing / CThingCreatureBase / CThingAICreature / C2DBoxI / CMapwhoNode /
//   CThingMapCell / CThingMap / CMapwhoLevelInfo / CLayerInfo / the two filters / Filters / CThingSearchTools_Calls, which emitted a
//   CThingSearchTools_Calls stand-in) onto the shared headers as an explicit instantiation of the header template, on the landed
//   0x007A3CA0 recipe (the Bind4<IsNot, IsNot, IsTargetable, In2DRange> instance of the same template) plus ROUGHAREA_TYPEDLOCAL (0x00746560:
//   the typed pointer stored before the filter calls, 0x008E3DB6 mov [esp+38h],esi ahead of call 0x006A76A0).
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_TYPEDLOCAL
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
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"

template void CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, std::vector<CThingCreatureBase*>,
    CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange> >(const C2DBoxI&,
    const CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>&, std::vector<CThingCreatureBase*>&) const;

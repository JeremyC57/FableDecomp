// CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind2<CThingFilter_MinAllowedRadius, CThingFilter_OverlappingSphere> >(const C2DBoxI& area, const CFilter_Bind2<...>& filter_func, std::list<CThingGameObject*>& things) const -- collect every game object in the map cells under the area that is at least the filter's minimum radius and overlaps its sphere, each thing once per search
// Leaf: ??$GetAllThingsInRoughArea@VCThingGameObject@@V?$list@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_MinAllowedRadius@@VCThingFilter_OverlappingSphere@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_MinAllowedRadius@@VCThingFilter_OverlappingSphere@@@@AAV?$list@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FE7C0, 680 bytes, thiscall `ret 0Ch`; called from 0x006FDAE9.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
//   CThingFilter_MinAllowedRadius::operator() 0x006A7410, CTCPhysicsBase::GetBoundingInfo 0x006AF640 (the inline
//   CThingFilter_OverlappingSphere), operator new 0x00BFEA0E (list node), memmove 0x00BFEAE6 (the AreaMaps clear).
// Shape: the shared template body under FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD (Ego_d.pdb's locals and scopes, the empty
//   test through C2DBoxI::GetWidth / GetHeight, the inline GetMapwhoArea), instantiated explicitly with the include and opt-in set
//   of the landed twin 0x00878430 (same template, cm_helper_functions.obj). That body closed the post-node-loop reload order (retail
//   edx = BRX, edi = thing_map, esi = BRY after the node loop) that parked this row on the older ORIGINAL_ROUGHAREA +
//   ROUGHAREA_THISEND text: RELOCATION_MATCH at the first grade (hand4c 2026-11-19). No flags, no new opt-ins.
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_WORLD
#define FABLE_RETAIL_CGAMECAMERAMANAGER_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/CThingFilter_MinAllowedRadius.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CFilter_Bind2.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject,std::list<CThingGameObject*>,CFilter_Bind2<CThingFilter_MinAllowedRadius,CThingFilter_OverlappingSphere> >(const C2DBoxI&,const CFilter_Bind2<CThingFilter_MinAllowedRadius,CThingFilter_OverlappingSphere>&,std::list<CThingGameObject*>&)const;
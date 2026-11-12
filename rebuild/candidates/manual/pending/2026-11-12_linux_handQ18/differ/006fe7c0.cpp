// CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind2<CThingFilter_MinAllowedRadius, CThingFilter_OverlappingSphere> >(const C2DBoxI& area, const CFilter_Bind2<...>& filter, std::list<CThingGameObject*>& result) const -- collect every game object in the map cells under the area that is at least the filter's minimum radius and overlaps its sphere, each thing once per search
// Grade: DIFFER(ours v retail) -- DIFFER(680v680) -- ported to the shared headers (same template body as 0x008780D0); one residue: the reload order after the node loop at 0x006FE9BD (retail edx/edi/esi = cells.BRX / map / cells.BRY; ours edi/esi/edx)
// Leaf: ??$GetAllThingsInRoughArea@VCThingGameObject@@V?$list@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_MinAllowedRadius@@VCThingFilter_OverlappingSphere@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_MinAllowedRadius@@VCThingFilter_OverlappingSphere@@@@AAV?$list@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FE7C0, 680 bytes, thiscall `ret 0Ch`; called from 0x006FDAE9.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
//   CThingFilter_MinAllowedRadius::operator() 0x006A7410, CTCPhysicsBase::GetBoundingInfo 0x006AF640 (the inline
//   CThingFilter_OverlappingSphere), operator new 0x00BFEA0E (list node), memmove 0x00BFEAE6 (the AreaMaps clear).
// Shape: the shared header body (FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA) with the Q18 opt-ins ROUGHAREA_THISEND (the map
//   loop's end read from the member) and CTHINGMAP_PEEKCELL_INLINE (the cell from the inline PeekCell), instantiated explicitly;
//   byte twin of 0x008780D0 (the CThingBuilding / IsOfType instance) except the type mask and the first filter.
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_WORLD
#define FABLE_RETAIL_CGAMECAMERAMANAGER_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
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

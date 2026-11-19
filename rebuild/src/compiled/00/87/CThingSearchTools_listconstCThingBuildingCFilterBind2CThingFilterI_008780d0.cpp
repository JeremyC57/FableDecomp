// CThingSearchTools::GetAllThingsInRoughArea<CThingBuilding, std::list<const CThingBuilding*>, CFilter_Bind2<CThingFilter_IsOfType, CThingFilter_OverlappingSphere> >(const C2DBoxI& area, const CFilter_Bind2<...>& filter_func, std::list<const CThingBuilding*>& things) const -- collect every building in the map cells under the area that is of the filter's thing type and overlaps its sphere, each thing once per search
// Leaf: ??$GetAllThingsInRoughArea@VCThingBuilding@@V?$list@PBVCThingBuilding@@V?$allocator@PBVCThingBuilding@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_IsOfType@@VCThingFilter_OverlappingSphere@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_IsOfType@@VCThingFilter_OverlappingSphere@@@@AAV?$list@PBVCThingBuilding@@V?$allocator@PBVCThingBuilding@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008780D0, 680 bytes, thiscall `ret 0Ch`.
// Identity (corrected): the catalog's "GetPBestThingInRoughArea<CFilter_Bind2<CThingFilter_IsNot,CIsVeryCloseMotionlessCreature>,
//   CThingCompare_Nearest,CThingCreatureBase>" is wrong. The body pushes every hit onto a std::list (operator new 0xc node,
//   0x00BFEA0E) and returns nothing; its type test is `and ecx,47h ; cmp cl,47h` (CThingBuilding::StatGetTypeFlags), the filter
//   calls CThingFilter_IsOfType::operator() 0x006A7100 then the inline CThingFilter_OverlappingSphere (CTCPhysicsBase::
//   GetBoundingInfo 0x006AF640); its only caller is the landed NCameraModes::CTools::GetThingsOfType<CThingBuilding> 0x00878390
//   (`call 0x008780D0` at 0x00878421), whose source instantiates exactly this template (Ego_r.pdb name above, the const-pointer
//   list overload). Byte twin of 0x006FE7C0 (the CThingGameObject / MinAllowedRadius instance) except the type mask and first filter.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
//   CThingFilter_IsOfType::operator() 0x006A7100, CTCPhysicsBase::GetBoundingInfo 0x006AF640, operator new 0x00BFEA0E, memmove
//   0x00BFEAE6 (the AreaMaps clear).
// Shape: the shared template body under FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD (Ego_d.pdb's locals and scopes, the empty
//   test through C2DBoxI::GetWidth / GetHeight, the inline GetMapwhoArea), instantiated explicitly with the include and opt-in set
//   of the landed same-TU twin 0x00878430 (cm_helper_functions.obj). That body closed the post-node-loop reload order (retail
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
#define FABLE_RETAIL_CTHINGBUILDING_TYPEFLAGS
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
#include "engine/CThingBuilding.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/CThingFilter_IsOfType.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CFilter_Bind2.h"
class CThingBuilding;
template void CThingSearchTools::GetAllThingsInRoughArea<CThingBuilding,std::list<const CThingBuilding*>,CFilter_Bind2<CThingFilter_IsOfType,CThingFilter_OverlappingSphere> >(const C2DBoxI&,const CFilter_Bind2<CThingFilter_IsOfType,CThingFilter_OverlappingSphere>&,std::list<const CThingBuilding*>&)const;
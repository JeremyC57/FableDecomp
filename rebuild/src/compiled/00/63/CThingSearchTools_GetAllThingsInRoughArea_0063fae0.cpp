// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsNot>& filter, std::list<const CThingGameObject*>& things) -- (<CThingGameObject>) every CThingGameObject in the mapwho cells under the area passing both filters, each visited once per search stamp, appended to the list
// Leaf: ??$GetAllThingsInRoughArea@VCThingGameObject@@V?$list@PBVCThingGameObject@@V?$allocator@PBVCThingGameObject@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_IsNot@@V1@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_IsNot@@V1@@@AAV?$list@PBVCThingGameObject@@V?$allocator@PBVCThingGameObject@@@std@@@std@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063FAE0, 596 bytes, thiscall, `ret 0Ch`. Name: Ego_r.pdb / FableWin.pdb (the leaf above); the only
//   caller, CWorldMap::IsLineOfSight 0x0063E6F0, binds it.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0 / ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (protected const);
//   CThingFilter_IsNot::operator() 0x006A76A0 (both filters); malloc 0x00BFEA0E (the list node); memmove 0x00BFEAE6 (the AreaMaps clear).
// Port (NF101, 2026-11-19, q11280 / q11283): the landed row's local CThingSearchTools model (GetMapsInArea / GetCellExtents stand-ins for
//   0x0049C6F0 / 0x0049C700, local CThingMap / list / filter models) replaced by an explicit instantiation of the shared
//   engine/CThingSearchTools.h GetAllThingsInRoughArea body (ROUGHAREA_EGOD), the define set of the landed 0x00878430 instance.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
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
#include "fable_compiler.h"
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
#include "engine/CThingFilter_IsNot.h"
#include "engine/CFilter_Bind2.h"

template void CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject,std::list<const CThingGameObject*>,CFilter_Bind2<CThingFilter_IsNot,CThingFilter_IsNot> >(const C2DBoxI&,const CFilter_Bind2<CThingFilter_IsNot,CThingFilter_IsNot>&,std::list<const CThingGameObject*>&) const;

// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things) -- append every thing of type TThing in the mapwho cells under the area that passes the filter to the result list, each thing visited once per search stamp; instance <CThingBuilding, std::list<CThingBuilding*>, CThingFilter_OverlappingSphere>
// Leaf: ??$GetAllThingsInRoughArea@VCThingBuilding@@V?$list@PAVCThingBuilding@@V?$allocator@PAVCThingBuilding@@@std@@@std@@VCThingFilter_OverlappingSphere@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_OverlappingSphere@@AAV?$list@PAVCThingBuilding@@V?$allocator@PAVCThingBuilding@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00785DD0, length 581 bytes (`ret 0xc`).
// Identity (sweep3): Ego_r/FableWin name this instance ??$GetAllThingsInRoughArea@VCThingBuilding@@V?$list@PAVCThingBuilding@@...VCThingFilter_OverlappingSphere@@@CThingSearchTools@@QBEX....
//   The landed name FindThingsInRoughArea has no PDB match: CThingSearchTools' rough-area collector of things is
//   GetAllThingsInRoughArea (ForEachInRoughArea applies a functor, GetAllTCsInRoughArea / GetAllTCDsInRoughArea collect components).
//   The catalog label "GetPBestThingInRoughArea<CThingFilter_IsPotentialCustomer,CThingCompare_Nearest,CThingCreatureBase>" was propagated across byte-identical instances.
//   Mask 0x47 is CThingBuilding (CThingGameObject 7 | building bit 0x40; RTTI 0x0124509C); the landed CThingMask47 was invented.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), CThingFilter_OverlappingSphere::operator() 0x006689E0, std::list node allocation (malloc 0x00BFEA0E via the STLport node allocator), memmove 0x00BFEAE6 (the AreaMaps clear).
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF106 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / the thing class / the filter / CThingMap / CMapVector / an STLport list / a local CThingSearchTools, which bound the GetMapsInArea / GetCellExtents stand-ins and a QAE self leaf) onto the shared headers as an explicit
//   instantiation of the engine/CThingSearchTools.h template, on the landed 0x0085B2A0 / 0x006FE7C0 (std::list instances: ORIGINAL_ROUGHAREA + ROUGHAREA_EGOD + PEEKCELL_INLINE) define set. It now binds the
//   Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...;
//   the self leaf is the Ego_r.pdb / FableWin.pdb spelling.
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGBUILDING_TYPEFLAGS
#define FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGSPHERE_OUTOFLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include <stddef.h>
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_OverlappingSphere.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThingBuilding,std::list<CThingBuilding*>,CThingFilter_OverlappingSphere >(const C2DBoxI&,const CThingFilter_OverlappingSphere&,std::list<CThingBuilding*>&) const;

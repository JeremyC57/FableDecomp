// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things) -- append every thing of type TThing in the mapwho cells under the area that passes the filter to the result vector, each thing visited once per search stamp; instance <const CThingBuilding, std::vector<const CThingBuilding*>, CThingFilter_None>
// Leaf: ??$GetAllThingsInRoughArea@$$CBVCThingBuilding@@V?$vector@PBVCThingBuilding@@V?$allocator@PBVCThingBuilding@@@std@@@std@@VCThingFilter_None@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_None@@AAV?$vector@PBVCThingBuilding@@V?$allocator@PBVCThingBuilding@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B2720, length 570 bytes (`ret 0xc`).
// Identity (sweep3): Ego_r/FableWin name this instance ??$GetAllThingsInRoughArea@$$CBVCThingBuilding@@V?$vector@PBVCThingBuilding@@...VCThingFilter_None@@@CThingSearchTools@@QBEX... (FableWin).
//   The landed name FindThingsInRoughArea has no PDB match: CThingSearchTools' rough-area collector of things is
//   GetAllThingsInRoughArea (ForEachInRoughArea applies a functor, GetAllTCsInRoughArea / GetAllTCDsInRoughArea collect components).
//   The catalog label "GetPBestThingInRoughArea<CThingFilter_IsPotentialCustomer,CThingCompare_Nearest,CThingCreatureBase>" was propagated across byte-identical instances.
//   Mask 0x47 is CThingBuilding (CThingGameObject 7 | building bit 0x40; RTTI 0x0124509C); the landed CThingMask47 was invented.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), vector<const CThingBuilding*>::_M_insert_overflow (push_back), memmove 0x00BFEAE6 (the AreaMaps clear).
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF106 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / the thing class / the filter / CThingMap / CMapVector / an STLport vector / a local CThingSearchTools, which bound the GetMapsInArea / GetCellExtents stand-ins and a QAE self leaf) onto the shared headers as an explicit
//   instantiation of the engine/CThingSearchTools.h template, on the landed 0x0095C7C0 / 0x00959380 (vector, family K016) define set. It now binds the
//   Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...;
//   the self leaf is the FableWin.pdb spelling (0x024696C0; Ego_r.pdb has no instance) the landed caller 0x006B1F30 already binds (was QAE).
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGBUILDING_TYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_None.h"
template void CThingSearchTools::GetAllThingsInRoughArea<const CThingBuilding,std::vector<const CThingBuilding*>,CThingFilter_None>(const C2DBoxI&,const CThingFilter_None&,std::vector<const CThingBuilding*>&) const;

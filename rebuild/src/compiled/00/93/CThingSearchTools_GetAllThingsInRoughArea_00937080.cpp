// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things) -- append every thing of type TThing in the mapwho cells under the area that passes the filter to the result list, each thing visited once per search stamp; instance <CThingDeadCreature, std::list<CThingDeadCreature*>, CThingFilter_None>
// Leaf: ??$GetAllThingsInRoughArea@VCThingDeadCreature@@V?$list@PAVCThingDeadCreature@@V?$allocator@PAVCThingDeadCreature@@@std@@@std@@VCThingFilter_None@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_None@@AAV?$list@PAVCThingDeadCreature@@V?$allocator@PAVCThingDeadCreature@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00937080, length 570 bytes (`ret 0xc`).
// Identity (sweep3): Ego_r/FableWin name this instance ??$GetAllThingsInRoughArea@VCThingDeadCreature@@V?$list@PAVCThingDeadCreature@@...VCThingFilter_None@@@CThingSearchTools@@QBEX....
//   The landed name FindThingsInRoughArea has no PDB match: CThingSearchTools' rough-area collector of things is
//   GetAllThingsInRoughArea (ForEachInRoughArea applies a functor, GetAllTCsInRoughArea / GetAllTCDsInRoughArea collect components).
//   The catalog label "GetPBestThingInRoughArea<CThingFilter_IsPotentialCustomer,CThingCompare_Nearest,CThingCreatureBase>" was propagated across byte-identical instances.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), the list node malloc 0x00BFEA0E.
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF107 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / CThingDeadCreature / CThingFilter_None / the mapwho node and map structs / a local STLport list / a local CThingSearchTools template, which bound the GetMapsInArea / GetCellExtents stand-ins) onto the shared headers as an explicit
//   instantiation of the engine/CThingSearchTools.h template, on the landed 0x0079BF30 / 0x0080E8E0 (std::list, ROUGHAREA_EGOD) define set. It now binds the
//   Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...;
//   the self leaf is the Ego_r.pdb / FableWin.pdb spelling (Ego_r.pdb 0x00959DF0, FableWin.pdb; CThingDeadCreature::StatGetTypeFlags is the NF107 opt-in FABLE_RETAIL_CTHINGDEADCREATURE_STATTYPEFLAGS).
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
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
#define FABLE_RETAIL_CTHINGDEADCREATURE_STATTYPEFLAGS
#include <stddef.h>
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CThingFilter_None.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThingDeadCreature,std::list<CThingDeadCreature*>,CThingFilter_None >(const C2DBoxI&,const CThingFilter_None&,std::list<CThingDeadCreature*>&) const;

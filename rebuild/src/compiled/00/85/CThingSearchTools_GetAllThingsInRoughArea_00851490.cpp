// 00851490 CThingSearchTools::GetAllThingsInRoughArea, 596 retail bytes.
// Leaf: ??$GetAllThingsInRoughArea@VCThingMarker@@V?$vector@PAVCThingMarker@@V?$allocator@PAVCThingMarker@@@std@@@std@@VCThingFilter_HasTC@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasTC@@AAV?$vector@PAVCThingMarker@@V?$allocator@PAVCThingMarker@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00851490.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), CThingFilter_HasTC 0x006A80A0, vector<CThingMarker*>::_M_insert_overflow 0x0062E190.
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF107 (round 10, q11280 / q11283): ported off its local model (RetailVector / CThing / C2DBoxI / NThingMap::CThingMap / ThingVector / the filters / Filters / a CThingSearchTools_Calls stand-in, which bound the GetAllThingMapsInArea / ClipAndConvertWorldMapAreaToLocalArea stand-ins of CThingSearchTools_Calls and its own stand-in self leaf) onto the shared headers as an explicit
//   instantiation of the engine/CThingSearchTools.h template, on the landed 0x0074D710 / 0x00919780 (vector, family K016) define set. It now binds the
//   Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...;
//   the self leaf is the Ego_r.pdb / FableWin.pdb spelling (Ego_r.pdb 0x00851510, FableWin.pdb 0x029BCBB0; CThingMarker::StatGetTypeFlags is the NF107 opt-in FABLE_RETAIL_CTHINGMARKER_STATTYPEFLAGS).
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_TYPEDLOCAL
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGMARKER_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingMarker.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_HasTC.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThingMarker,std::vector<CThingMarker*>,CThingFilter_HasTC >(const C2DBoxI&,const CThingFilter_HasTC&,std::vector<CThingMarker*>&) const;

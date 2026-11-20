// 007f9c00 CThingSearchTools::GetAllThingsInRoughArea, 619 retail bytes.
// Leaf: ??$GetAllThingsInRoughArea@VCThing@@V?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@V?$CFilter_Bind3@VCThingFilter_IsNotList@@VCThingFilter_IsTargetable@@VCThingFilter_In3DRange@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsNotList@@VCThingFilter_IsTargetable@@VCThingFilter_In3DRange@@@@AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F9C00.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), CThingFilter_IsNotList 0x006A7EB0, CThingFilter_IsTargetable 0x006A84F0, CThingFilter_In3DRange 0x006A7780, vector<CThing*>::_M_insert_overflow 0x00436AC0.
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF107 (round 10, q11280 / q11283): ported off its local model (RetailVector / CThing / C2DBoxI / NThingMap::CThingMap / ThingVector / the filters / Filters / a CThingSearchTools_Calls stand-in, which bound the GetAllThingMapsInArea / ClipAndConvertWorldMapAreaToLocalArea stand-ins of CThingSearchTools_Calls and its own stand-in self leaf) onto the shared headers as an explicit
//   instantiation of the engine/CThingSearchTools.h template, on the landed 0x0074D710 / 0x007F4020 (vector, family K016) define set. It now binds the
//   Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...;
//   the self leaf is the Ego_r.pdb / FableWin.pdb spelling (Ego_r.pdb 0x007F4D60, FableWin.pdb 0x0272BFD0) the landed caller CTCTargetingSpirit::UpdateTargetList 0x007F9960 already binds.
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
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_IsNotList.h"
#include "engine/CThingFilter_IsTargetable.h"
#include "engine/CThingFilter_In3DRange.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThing,std::vector<CThing*>,CFilter_Bind3<CThingFilter_IsNotList,CThingFilter_IsTargetable,CThingFilter_In3DRange> >(const C2DBoxI&,const CFilter_Bind3<CThingFilter_IsNotList,CThingFilter_IsTargetable,CThingFilter_In3DRange>&,std::vector<CThing*>&) const;

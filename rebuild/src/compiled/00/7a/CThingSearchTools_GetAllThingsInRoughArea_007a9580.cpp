// 007a9580 CThingSearchTools::GetAllThingsInRoughArea, 628 retail bytes.
// Leaf: ??$GetAllThingsInRoughArea@VCThingPhysical@@V?$vector@PAVCThingPhysical@@V?$allocator@PAVCThingPhysical@@@std@@@std@@V?$CFilter_Bind3@VCThingFilter_In3DRange@@VCThingFilter_IsNot@@VCThingFilter_HasInstantiationName@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_In3DRange@@VCThingFilter_IsNot@@VCThingFilter_HasInstantiationName@@@@AAV?$vector@PAVCThingPhysical@@V?$allocator@PAVCThingPhysical@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A9580.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), CThingFilter_In3DRange 0x006A7780, CThingFilter_IsNot 0x006A76A0, CThingFilter_HasInstantiationName 0x006A7C00, vector<CThingPhysical*>::_M_insert_overflow 0x007A91A0.
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF107 (round 10, q11280 / q11283): ported off its local model (RetailVector / CThing / C2DBoxI / NThingMap::CThingMap / ThingVector / the filters / Filters / a CThingSearchTools_Calls stand-in, which bound the GetAllThingMapsInArea / ClipAndConvertWorldMapAreaToLocalArea stand-ins of CThingSearchTools_Calls and its own stand-in self leaf) onto the shared headers as an explicit
//   instantiation of the engine/CThingSearchTools.h template, on the landed 0x0074D710 / 0x0095C7C0 (vector, family K016) define set. It now binds the
//   Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...;
//   the self leaf is the Ego_r.pdb / FableWin.pdb spelling (Ego_r.pdb 0x007AE2C0, FableWin.pdb 0x026CBA20; filter order from the body's call order).
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
#define FABLE_RETAIL_CTHINGPHYSICAL_STATTYPEFLAGS
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_In3DRange.h"
#include "engine/CThingFilter_IsNot.h"
// Kept from the landed row (no shared header for this class):
class CThingFilter_HasInstantiationName { public:void* InstantiationName;bool operator()(const CThing&)const;}; // 0x006a7c00
template void CThingSearchTools::GetAllThingsInRoughArea<CThingPhysical,std::vector<CThingPhysical*>,CFilter_Bind3<CThingFilter_In3DRange,CThingFilter_IsNot,CThingFilter_HasInstantiationName> >(const C2DBoxI&,const CFilter_Bind3<CThingFilter_In3DRange,CThingFilter_IsNot,CThingFilter_HasInstantiationName>&,std::vector<CThingPhysical*>&) const;

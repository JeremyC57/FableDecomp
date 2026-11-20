// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind4<CanBeUsedAsCover, not1(IsOccupiableOccupied), IsNot, In2DRange>& filter, vector<CThing*>& result) -- the advance-in-cover search: walks every thing map overlapping the area, each mapwho layer and each cell of the scaled box, visits every mapwho once per search ID and appends the things that can be used as cover, are not occupied, are not the excluded thing and lie in 2D range
// Leaf: ??$GetAllThingsInRoughArea@VCThing@@V?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@V?$CFilter_Bind4@VCThingFilter_CanBeUsedAsCover@@V?$unary_negate@VCThingFilter_IsOccupiableOccupied@@@std@@VCThingFilter_IsNot@@VCThingFilter_In2DRange@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind4@VCThingFilter_CanBeUsedAsCover@@V?$unary_negate@VCThingFilter_IsOccupiableOccupied@@@std@@VCThingFilter_IsNot@@VCThingFilter_In2DRange@@@@AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009793D0, 640 bytes (incl. padding), `__fastcall`, `ret 0xc`; one caller
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), CThingFilter_CanBeUsedAsCover 0x006A7AB0, CThingFilter_IsOccupiableOccupied 0x006A8A30 (negated), CThingFilter_IsNot 0x006A76A0, CThingFilter_In2DRange 0x006A76B0, vector<CThing*>::_M_insert_overflow 0x00436AC0.
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF107 (round 10, q11280 / q11283): ported off its local model (RetailVector / CThing / C2DBoxI / NThingMap::CThingMap / ThingVector / the filters / Filters / a CThingSearchTools_Calls stand-in, which bound the GetAllThingMapsInArea / ClipAndConvertWorldMapAreaToLocalArea stand-ins of CThingSearchTools_Calls and its own stand-in self leaf) onto the shared headers as an explicit
//   instantiation of the engine/CThingSearchTools.h template, on the landed 0x0074D710 / 0x0095C7C0 (vector, family K016) define set. It now binds the
//   Ego_r.pdb spellings ?GetAllThingMapsInArea@CThingSearchTools@@IBE... / ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE...;
//   the self leaf is the Ego_r.pdb / FableWin.pdb spelling (Ego_r.pdb 0x0099E090, FableWin.pdb 0x02C15E40; the landed caller 0x00978FD0 binds a CThingSearchToolsRetail::FindInRoughArea stand-in, NAME_FIXES).
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
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#include <stddef.h>
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CThingFirePointFilters.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"
#include "stlport/fable_containers.h"
// Kept from the landed row (no shared header for this class):
class CThingFilter_CanBeUsedAsCover { public:bool operator()(const CThing&)const;}; // 0x006a7ab0
template void CThingSearchTools::GetAllThingsInRoughArea<CThing,std::vector<CThing*>,CFilter_Bind4<CThingFilter_CanBeUsedAsCover,std::unary_negate<CThingFilter_IsOccupiableOccupied>,CThingFilter_IsNot,CThingFilter_In2DRange> >(const C2DBoxI&,const CFilter_Bind4<CThingFilter_CanBeUsedAsCover,std::unary_negate<CThingFilter_IsOccupiableOccupied>,CThingFilter_IsNot,CThingFilter_In2DRange>&,std::vector<CThing*>&) const;

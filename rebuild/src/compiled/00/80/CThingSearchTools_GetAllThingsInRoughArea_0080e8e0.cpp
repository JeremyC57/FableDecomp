// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& things) -- append every thing of type TThing in the mapwho cells under the area that passes the filter to the result list, each thing visited once per search stamp; instance <CThing, std::list<CThing*>, CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNotList, CThingFilter_IsWithinArea >>
// Leaf: ??$GetAllThingsInRoughArea@VCThing@@V?$list@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@V?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_IsNotList@@VCThingFilter_IsWithinArea@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_IsNotList@@VCThingFilter_IsWithinArea@@@@AAV?$list@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080E8E0, length 602 bytes (`ret 0xc`).
// Identity (sweep3): Ego_r/FableWin name this instance ??$GetAllThingsInRoughArea@VCThing@@V?$list@PAVCThing@@...V?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_IsNotList@@VCThingFilter_IsWithinArea@@@@@CThingSearchTools@@QBEX....
//   The landed name FindThingsInRoughArea has no PDB match: CThingSearchTools' rough-area collector of things is
//   GetAllThingsInRoughArea (ForEachInRoughArea applies a functor, GetAllTCsInRoughArea / GetAllTCDsInRoughArea collect components).
//   The catalog label "GetPBestThingInRoughArea<CFilter_Bind2<CThingFilter_IsNot,CIsVeryCloseMotionlessCreature>,CThingCompare_Nearest,CThingCreatureBase>" was propagated across byte-identical instances.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), CThingFilter_IsNot::operator() 0x006A76A0, CThingFilter_IsNotList::operator() 0x006A7EB0, CThingFilter_IsWithinArea::operator() 0x007FD830, std::list node allocation (malloc 0x00BFEA0E via the STLport node allocator), memmove 0x00BFEAE6 (the AreaMaps clear).
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF106 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / the thing class / the filters / CFilter_Bind3 / CThingMap / CMapVector / an STLport list / a local CThingSearchTools, which bound the GetMapsInArea / GetCellExtents stand-ins and a QAE self leaf) onto the shared headers as an explicit
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
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
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
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsNotList.h"
// Kept from the landed row (no shared header for this class):
class CThingFilter_IsWithinArea {
public:
    bool operator()(const CThing& thing) const;   // 0x007FD830 (PDB ??RCThingFilter_IsWithinArea@@QBE_NABVCThing@@@Z)
};
template void CThingSearchTools::GetAllThingsInRoughArea<CThing,std::list<CThing*>,CFilter_Bind3<CThingFilter_IsNot,CThingFilter_IsNotList,CThingFilter_IsWithinArea> >(const C2DBoxI&,const CFilter_Bind3<CThingFilter_IsNot,CThingFilter_IsNotList,CThingFilter_IsWithinArea>&,std::list<CThing*>&) const;

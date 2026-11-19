// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind2<CThingFilter_HasTC, CThingFilter_In2DRange>& filter, std::list<CThingCreatureBase*>& things) -- (<CThingCreatureBase>) every CThingCreatureBase in the mapwho cells under the area passing both filters, each visited once per search stamp, appended to the list
// Leaf: ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$list@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_HasTC@@VCThingFilter_In2DRange@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_HasTC@@VCThingFilter_In2DRange@@@@AAV?$list@PAVCThingCreatureBase@@V?$allocator@PAVCThingCreatureBase@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091A750, 596 bytes, thiscall, `ret 0Ch`.
// Identity (proven): the type mask 0xF is CThingCreatureBase's and the filters called are CThingFilter_HasTC::operator() 0x006A80A0 and
//   CThingFilter_In2DRange::operator() 0x006A76B0; Ego_r.pdb/FableWin.pdb ??$GetAllThingsInRoughArea@VCThingCreatureBase@@V?$list@...V?$CFilter_Bind2@...
//   is the only list instantiation with that filter pair for CThingCreatureBase; its caller is CAIStateGroup_WorkBase<CAIStateGroupBase>::Acquire 0x0091A9B0. The manifest's
//   "GetPBestThingInRoughArea<CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>, ...>" is propagated.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (C2DBoxI by value), CThingFilter_HasTC::operator() 0x006A80A0, CThingFilter_In2DRange::operator() 0x006A76B0, std::list node allocation (malloc 0x00BFEA0E via the STLport node allocator), memmove 0x00BFEAE6 (the AreaMaps clear).
// Globals: CThingSearchTools::NoLookups / NoThingsScanned / SearchID 0x013B89A8 / AC / B0, NThingMap::MaxLayers 0x013BA848, NThingMap::LayerInfo 0x0137DAD8.
// NF106 (round 10, q11280 / q11283): ported off its local model (C2DBoxI / CThing / the thing class / the filters / CFilter_Bind2 / CThingMap / CMapVector / an STLport list / a local CThingSearchTools, which bound the GetMapsInArea / GetCellExtents stand-ins and a QAE self leaf) onto the shared headers as an explicit
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
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
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
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_In2DRange.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase,std::list<CThingCreatureBase*>,CFilter_Bind2<CThingFilter_HasTC,CThingFilter_In2DRange> >(const C2DBoxI&,const CFilter_Bind2<CThingFilter_HasTC,CThingFilter_In2DRange>&,std::list<CThingCreatureBase*>&) const;

// 0077adf0 CThingSearchTools::GetAllTCDsInRoughArea<CTCDExperienceOrb, CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>, std::vector<CThing*> >(const C2DBoxI&, const CFilter_Bind2<..>&, std::vector<CThing*>&) const -- every thing under the area whose driver is an experience orb, not the given thing and in 2D range, pushed into the vector
// Leaf: ??$GetAllTCDsInRoughArea@VCTCDExperienceOrb@@V?$CFilter_Bind2@VCThingFilter_IsNot@@VCThingFilter_In2DRange@@@@V?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_IsNot@@VCThingFilter_In2DRange@@@@AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z

// Retail: Fable.exe (TLC, VC7.1) @ 0x0077ADF0, 614 bytes, __thiscall, `ret 0Ch`.
// Identity: Ego_r.pdb 0x00596D90 + FableWin.pdb 0x01F59D70 name this instance; the body tests the parent thing's driver with
//   IsDriverType(26) (FableWin.pdb ?StatGetDriverType@CTCDExperienceOrb@@SIKXZ), then calls CThingFilter_IsNot 0x006A76A0 and
//   CThingFilter_In2DRange 0x006A76B0 in that order (CFilter_Bind2), and pushes into a vector<CThing*> (overflow 0x00436AC0).
//   The catalog label GetPBestThingInRoughArea<CFilter_Bind3<..IsAvailableForCreatureInteraction..>> was propagated across bodies.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, the two filters,
//   vector<CThing*>::_M_insert_overflow 0x00436AC0, memmove 0x00BFEAE6 (the AreaMaps clear).
// NF107 (round 10, q11280 / q11283): ported off its local model (RetailVector / CThing / C2DBoxI / NThingMap::CThingMap / ThingVector / the
//   two filters / Filters / a CThingSearchTools_Calls stand-in) onto the shared headers as an explicit instantiation of the
//   engine/CThingSearchTools.h GetAllTCDsInRoughArea template, on the define set of the landed 0x005185F0 (TCDSINROUGHAREA_EGOD) plus the
//   NF107 opt-in FABLE_RETAIL_CTCDEXPERIENCEORB_STATGETDRIVERTYPE. It now binds the Ego_r.pdb spellings of 0x0049C6F0 / 0x0049C700.
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCDSINROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_TCDRIVER
#define FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE
#define FABLE_RETAIL_CTCD_STATGETDRIVERTYPE
#define FABLE_RETAIL_CTCDEXPERIENCEORB_STATGETDRIVERTYPE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
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
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCDriverBase.h"
#include "engine/CTCDExperienceOrb.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"

template void CThingSearchTools::GetAllTCDsInRoughArea<CTCDExperienceOrb,CFilter_Bind2<CThingFilter_IsNot,CThingFilter_In2DRange>,std::vector<CThing*> >(const C2DBoxI&,const CFilter_Bind2<CThingFilter_IsNot,CThingFilter_In2DRange>&,std::vector<CThing*>&)const;

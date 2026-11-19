// 0091ba40 CThingSearchTools::GetAllTCsInRoughArea<CTCPerceiveThing, CFilter_Bind2<CThingFilter_IsPerceivingThing, CThingFilter_IsNot>, std::list<CTCPerceiveThing*> >(const C2DBoxI&, const CFilter_Bind2<..>&, std::list<CTCPerceiveThing*>&) const -- append the perceive-thing component of every thing in the mapwho cells under the area that has one and passes both filters, each thing once per search
// Leaf: ??$GetAllTCsInRoughArea@VCTCPerceiveThing@@V?$CFilter_Bind2@VCThingFilter_IsPerceivingThing@@VCThingFilter_IsNot@@@@V?$list@PAVCTCPerceiveThing@@V?$allocator@PAVCTCPerceiveThing@@@std@@@std@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_IsPerceivingThing@@VCThingFilter_IsNot@@@@AAV?$list@PAVCTCPerceiveThing@@V?$allocator@PAVCTCPerceiveThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091BA40, 683 bytes, thiscall `ret 0Ch`.
// Identity: Ego_r.pdb 0x0093F0D0 + FableWin.pdb 0x02BD4990 name this instance (the only GetAllTCsInRoughArea<CTCPerceiveThing, ..., list>);
//   the body tests interface 52 (TCI_PERCEIVE_THING) and calls CThingFilter_IsPerceivingThing::operator() 0x006A80E0 and
//   CThingFilter_IsNot::operator() 0x006A76A0 in that order (CFilter_Bind2 F1, F2), and pushes into a std::list (malloc'd nodes).
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, the two filters,
//   malloc 0x00BFEA0E (the list node), memmove 0x00BFEAE6 (the AreaMaps clear).
// NF106 (round 10, q11280 / q11283): ported off its CThingSearchTools_Calls stand-in model (RetailVector / InterfaceMap / CThing / ThingList /
//   C2DBoxI / NThingMap::CThingMap / Filters / CThingSearchTools_Calls) onto the shared headers as an explicit instantiation of the
//   TCSINROUGHAREA_EGOD template, on the landed 0x00518340 define set (its world_map TU twin <CTCCreatureGenerator, ..., vector>). Its self
//   leaf was the stand-in ?GetAllTCsInRoughArea@CThingSearchTools_Calls@@QBEXABVC2DBoxI@@ABUFilters@@AAUThingList@@@Z (no landed caller).
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCPERCEIVETHING_AWAREOFTHING
#define FABLE_RETAIL_CTHINGFILTER_ISPERCEIVINGTHING
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
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
#include "engine/CTCPerceiveThing.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsPerceivingThing.h"
#include "engine/CThingFilter_IsNot.h"
template void CThingSearchTools::GetAllTCsInRoughArea<CTCPerceiveThing,CFilter_Bind2<CThingFilter_IsPerceivingThing,CThingFilter_IsNot>,std::list<CTCPerceiveThing*> >(const C2DBoxI&,const CFilter_Bind2<CThingFilter_IsPerceivingThing,CThingFilter_IsNot>&,std::list<CTCPerceiveThing*>&)const;

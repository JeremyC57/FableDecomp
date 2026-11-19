// CThingSearchTools::GetAllTCDsInRoughArea<CTCDNavigationSeed, CThingFilter_IsInQuestOrNullQuest, std::vector<const CThing*> >(const C2DBoxI&
//   area, const CThingFilter_IsInQuestOrNullQuest& filter_func, std::vector<const CThing*>& things) const -- every thing linked into a mapwho
//   cell overlapping the area (each once per search) whose driver is a navigation seed and that passes the filter, appended to things
// Leaf: ??$GetAllTCDsInRoughArea@VCTCDNavigationSeed@@VCThingFilter_IsInQuestOrNullQuest@@V?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_IsInQuestOrNullQuest@@AAV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005185F0, 597 bytes, __thiscall, `ret 0Ch`. Name: Ego_r.pdb 0x004E56B0 / FableWin.pdb 0x01CCBD00 (the
//   leaf above); the landed caller 0x0050A650 CWorldMap::GetMapNavigationAreaInit binds this spelling.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0 / ClipAndConvertWorldMapAreaToLocalArea 0x0049C700 (protected const);
//   CTCDriverBase::IsDriverType (vtable slot 28); CThingFilter_IsInQuestOrNullQuest::operator() 0x006A7C60; the vector's
//   _M_insert_overflow 0x0049C370; memmove 0x00BFEAE6 (the AreaMaps clear).
// Port (NF101, 2026-11-19, q11280 / q11283): the landed row's local CThingSearchTools_Calls / RetailVector model replaced by an explicit
//   instantiation of the shared engine/CThingSearchTools.h body (NF101 overlay opt-in TCDSINROUGHAREA_EGOD, the TCSINROUGHAREA_EGOD walk of
//   the landed 0x00518340 with the driver test), the 0x00518340 define set.
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
#include "engine/CTCDNavigationSeed.h"
#include "engine/CThingFilter_IsInQuestOrNullQuest.h"

template void CThingSearchTools::GetAllTCDsInRoughArea<CTCDNavigationSeed,CThingFilter_IsInQuestOrNullQuest,std::vector<const CThing*> >(const C2DBoxI&,const CThingFilter_IsInQuestOrNullQuest&,std::vector<const CThing*>&)const;

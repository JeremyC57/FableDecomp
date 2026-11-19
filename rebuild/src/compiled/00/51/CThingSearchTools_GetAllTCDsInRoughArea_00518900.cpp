// CThingSearchTools::GetAllTCDsInRoughArea<CTCDRegionExit, CThingFilter_IsInQuestOrNullQuest, std::vector<const CThing*> >(const C2DBoxI& area,
//   const CThingFilter_IsInQuestOrNullQuest& filter_func, std::vector<const CThing*>& things) const -- every thing in the mapwho cells under the area
//   whose driver is a region exit (driver type 0x19) and that passes the quest filter, each visited once per search, appended to the vector
// Leaf: ??$GetAllTCDsInRoughArea@VCTCDRegionExit@@VCThingFilter_IsInQuestOrNullQuest@@V?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_IsInQuestOrNullQuest@@AAV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00518900, 597 bytes, __thiscall. Name: the landed caller 0x0050A650 CWorldMap::GetMapNavigationAreaInit binds
//   this spelling (Ego_r.pdb GetAllTCDsInRoughArea<CTCDNavigationSeed / CTCDRegionExit>, template order T, Filter, Container).
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0 (protected const), ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, the filter
//   0x006A7C60, vector<const CThing*>::_M_insert_overflow 0x0049C370, memmove 0x00BFEAE6 (the AreaMaps clear).
// Port (NF103, round 10, q11280 / q11283): the landed row's local CThingSearchTools_Calls model (stand-ins for 0x0049C6F0 / 0x0049C700, the
//   search statics, NThingMap and the vector) replaced by an explicit instantiation of the shared engine/CThingSearchTools.h
//   GetAllTCDsInRoughArea body (TCDSINROUGHAREA_EGOD), the define set of the landed twin 0x005185F0 (T = CTCDRegionExit).
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
#include "engine/CTCDRegionExit.h"
#include "engine/CThingFilter_IsInQuestOrNullQuest.h"

template void CThingSearchTools::GetAllTCDsInRoughArea<CTCDRegionExit,CThingFilter_IsInQuestOrNullQuest,std::vector<const CThing*> >(const C2DBoxI&,const CThingFilter_IsInQuestOrNullQuest&,std::vector<const CThing*>&)const;

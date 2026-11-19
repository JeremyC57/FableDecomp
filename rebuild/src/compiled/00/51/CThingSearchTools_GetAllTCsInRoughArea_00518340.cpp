// CThingSearchTools::GetAllTCsInRoughArea<CTCCreatureGenerator, CThingFilter_IsInQuestOrNullQuest, std::vector<const CTCCreatureGenerator*> >(const C2DBoxI& area, const CThingFilter_IsInQuestOrNullQuest& filter_func, std::vector<const CTCCreatureGenerator*>& tcs) const -- append the creature-generator component of every thing in the mapwho cells under the area that has one and passes the quest filter, each thing once per search
// Leaf: ??$GetAllTCsInRoughArea@VCTCCreatureGenerator@@VCThingFilter_IsInQuestOrNullQuest@@V?$vector@PBVCTCCreatureGenerator@@V?$allocator@PBVCTCCreatureGenerator@@@std@@@std@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_IsInQuestOrNullQuest@@AAV?$vector@PBVCTCCreatureGenerator@@V?$allocator@PBVCTCCreatureGenerator@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00518340, 686 bytes, thiscall `ret 0Ch`; one caller 0x0050AADB (CWorldMap::GetMapNavigationAreaInit 0x0050A650).
// Identity (corrected): the catalog label "GetAllTCsInRoughArea<CTCPerceiveThing, CFilter_Bind2<..>, list<..> >" is propagated; reloc_bindings
//   binds 0x00518340 to the name in the Leaf line (Ego_r.pdb; FableWin.pdb world_map.obj 0x01CCB630, Ego_d.pdb the same): interface 0x3D
//   (TCI_CREATURE_GENERATOR, `test [edi+24h],20000000h` + `cmp [esi+eax*8],3Dh`), the out-of-line CThingFilter_IsInQuestOrNullQuest::operator()
//   0x006A7C60, a std::vector<const CTCCreatureGenerator*> with STLport's _M_insert_overflow 0x00514F40 called.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
//   CThingFilter_IsInQuestOrNullQuest::operator() 0x006A7C60, vector<const CTCCreatureGenerator*>::_M_insert_overflow 0x00514F40,
//   memmove 0x00BFEAE6 (the AreaMaps clear).
// Shape: the shared egod rough-area walk (ROUGHAREA_EGOD: GetMapwhoArea, GetWidth / GetHeight, PeekCell) under the new overlay opt-in
//   FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_EGOD (Ego_d locals filter_func / tcs / loop_thing / tc / tc_type in FableWin's order:
//   GetParentThing, StatGetInterfaceType, `loop_thing && GetTC(tc_type, &tc) && filter_func(*loop_thing)`, push_back(tc)), with the
//   world_map TU's opt-ins of the landed 0x00501450 CWorldMap::GenerateRegionConnectivityGraph: CTHING_GETTC_EXPANDED and
//   CVECTORMAP_INLINE_LOWERBOUND (retail expands the component lookup in place, 0x005184A5..0x005184EE), plus
//   STLP_VECTOR_OVERFLOW_OUTOFLINE (STLport's _vector.c bodies visible but out of line: retail calls 0x00514F40). The old parked local
//   model's level-loop reload-order residue went with the port (RELOCATION_MATCH at the third grade). New overlay header
//   engine/CThingFilter_IsInQuestOrNullQuest.h (Ego_d class record, Ego_r layout). No flags.
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
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
#define FABLE_RETAIL_CTCCREATUREGENERATOR
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
#include "engine/CTCCreatureGenerator.h"
#include "engine/CThingFilter_IsInQuestOrNullQuest.h"
template void CThingSearchTools::GetAllTCsInRoughArea<CTCCreatureGenerator,CThingFilter_IsInQuestOrNullQuest,std::vector<const CTCCreatureGenerator*> >(const C2DBoxI&,const CThingFilter_IsInQuestOrNullQuest&,std::vector<const CTCCreatureGenerator*>&)const;
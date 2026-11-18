// CThingSearchTools::GetAllThingsInRoughArea<CThing, std::list<const CThing*>, CFilter_Bind2<CThingFilter_IsOfType, CThingFilter_OverlappingSphere> >(const C2DBoxI& area, const CFilter_Bind2<...>& filter_func, std::list<const CThing*>& things) const -- append every thing in the mapwho cells under the area that is of the filter's thing type and overlaps its sphere, each thing once per search
// Leaf: ??$GetAllThingsInRoughArea@VCThing@@V?$list@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_IsOfType@@VCThingFilter_OverlappingSphere@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_IsOfType@@VCThingFilter_OverlappingSphere@@@@AAV?$list@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00878430, 673 bytes, thiscall `ret 0Ch`.
// Identity (corrected): the catalog's "GetPBestThingInRoughArea<CThingFilter_HasTrackName,CThingCompare_Nearest,CThingTrackNode>" is
//   wrong (that one takes four arguments; this body returns nothing and `ret 0Ch`s). rebuild/oracles/reloc_bindings.tsv binds 0x00878430
//   to the name above (referenced from 0x00877720), the landed caller NCameraModes::CTools::GetStandardThings 0x00877720 calls it twice
//   with CThingFilter_IsOfType(3) / (5) + CThingFilter_OverlappingSphere into its std::list<const CThing*>, and Ego_d.pdb has the
//   instance in the same object (cm_helper_functions.obj, next to GetThingsOfType<CThingBuilding> and the CThingBuilding twin 0x008780D0).
//   The CThing type test folds (StatGetTypeFlags() == 0): retail keeps `xor cl,cl ; jne` (0x00878588).
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
//   CThingFilter_IsOfType::operator() 0x006A7100, CTCPhysicsBase::GetBoundingInfo 0x006AF640 (the inline OverlappingSphere test),
//   operator new 0x00BFEA0E (list node), memmove 0x00BFEAE6 (the AreaMaps clear).
// Shape: the shared template body under the new egod opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD (Ego_d.pdb's locals and
//   scopes, the empty test through C2DBoxI::GetWidth / GetHeight, the inline GetMapwhoArea), instantiated explicitly with the include
//   and opt-in set of the landed same-TU caller 0x00877720. What closed the post-node-loop reload order (edx = BRX, edi = thing_map,
//   esi = BRY at 0x00878626) that parked the twins: the GetWidth() / GetHeight() empty test with the expanded GetMapwhoArea.
// FITTED: the empty test through C2DBoxI::GetWidth() / GetHeight() (both accessors exist in Ego_d's C2DBoxI record, GetWidth also
//   FableWin ?GetWidth@C2DBoxI@@QBEJXZ; the spelling was chosen by grading). The header GetMapwhoArea uses Ego_d's locals layer_info /
//   clipped_area and returns the named box through C2DBoxI's user copy constructor (Ego_d C2DBoxI method list 0xAE3E; FableWin
//   ??0C2DBoxI@@QAE@ABV0@@Z = FABLE_RETAIL_C2DBOXI_COPY, under FABLE_RETAIL_C2DBOXI): the same text matches its out-of-line copy
//   0x00436500 and this row's expansion (without the copy constructor the named result grades 677v673 here, 68v66 there).
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_WORLD
#define FABLE_RETAIL_CGAMECAMERAMANAGER_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGBUILDING_TYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
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
#include "engine/CThingBuilding.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/CThingFilter_IsOfType.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CFilter_Bind2.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThing,std::list<const CThing*>,CFilter_Bind2<CThingFilter_IsOfType,CThingFilter_OverlappingSphere> >(const C2DBoxI&,const CFilter_Bind2<CThingFilter_IsOfType,CThingFilter_OverlappingSphere>&,std::list<const CThing*>&)const;
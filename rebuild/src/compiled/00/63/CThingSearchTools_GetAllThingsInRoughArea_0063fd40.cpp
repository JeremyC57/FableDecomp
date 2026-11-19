// CThingSearchTools::GetAllThingsInRoughArea<CThing, std::list<CThing*>, CFilter_Bind2<CThingFilter_OverlappingCircle, CWorldMap::CThingFilterBase> >(const C2DBoxI& area, const CFilter_Bind2<...>& filter_func, std::list<CThing*>& things) const -- append every thing in the mapwho cells under the area whose physics footprint circle overlaps the filter circle and that the world-map filter accepts, each thing once per search
// Leaf: ??$GetAllThingsInRoughArea@VCThing@@V?$list@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_OverlappingCircle@@VCThingFilterBase@CWorldMap@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_OverlappingCircle@@VCThingFilterBase@CWorldMap@@@@AAV?$list@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063FD40, 661 bytes, thiscall `ret 0Ch`; one caller 0x0063EA1C (in 0x0063E950).
// Identity (corrected): the catalog label "ForEachInRoughArea<CThingPhysical, CFilter_Bind3<SolidToThing, PhysicsFlagsSolidToThing,
//   OverlappingCircle>, CThingFindHeightZBelowAt>" is propagated (that instance is 0x0066A1E0), and reloc_bindings.tsv binds 0x0063FD40 to an
//   old local-model spelling (?ForEachInRoughArea@CThingSearchTools@@QAEXABVC2DBoxI@@ABUCBoundThingFilter@@AAUCThingList@@@Z). The body
//   returns nothing (`ret 0Ch`), pushes 12-byte list nodes, folds the CThing type test (`xor cl,cl ; jne`, 0x0063FE97), expands
//   CThingFilter_OverlappingCircle (GetMapwhoPosition 0x006AFCB0) and calls the CWorldMap::CThingFilterBase virtual (`call [edx]`,
//   0x0063FEEF): Ego_r.pdb's ??$GetAllThingsInRoughArea@VCThing@@V?$list@PAVCThing@@...CFilter_Bind2@VCThingFilter_OverlappingCircle@@
//   VCThingFilterBase@CWorldMap@@... in world_map_tools.obj (Ego_r 0x006EC000), FableWin.pdb the same name; Ego_d.pdb the same locals as the
//   egod body (filter_func, things, flags, it, local_area, thing_map, c1, clipped_area, cy, pcell, cx, ploop_node, pmapwho, loop_thing, tthing).
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
//   CTCPhysicsBase::GetMapwhoPosition 0x006AFCB0, CWorldMap::CThingFilterBase::operator() (virtual slot 0), operator new 0x00BFEA0E (list
//   node), memmove 0x00BFEAE6 (the AreaMaps clear).
// Shape: the shared egod body FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD with the include / opt-in set of the landed twins 0x00878430 /
//   0x008780D0 / 0x006FE7C0, plus the CThingFilter_OverlappingCircle::operator() body (new overlay opt-in
//   FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGCIRCLE_BODY, FableWin text, Ego_d local ptc_physics), expanded by VC7.1 as in retail. Without the
//   body the filter is a call (the old parked draft carried a local model of it). The one-expression GetSquaredMagnitude2D
//   (FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23, FableWin 0x01DAEC70 `return X*X+Y*Y`) gives retail's dy-first squares at
//   0x0063FEC8; the running sum squares dx first. CTCPHYSICSBASE_CALCULATERADIUS declares GetMapwhoPosition (out of line, 0x006AFCB0);
//   CTCPHYSICSBASE_IMPULSE (declarations only) is on only because its block ends with the `public:` that CALCULATERADIUS's `protected:`
//   needs for GetRadius. No flags.
// Helper at its own address: this TU's out-of-line copy of the circle operator() grades DIFFER(95v95) at 0x0063EF50 -- only the square
//   order (retail dx first there, 0x0063EF7E..84, as in its other out-of-line copy 0x0063F2C0, while the expanded copies 0x0063FD40 and
//   0x0066A1E0 square dy first). Every tested text flips both sites together (sum spellings `X*X+Y*Y`, `(X*X)+(Y*Y)`, `(X*X+Y*Y)`,
//   `Y*Y+X*X`, `X*X+(Y*Y)` = dy first; running sum, `GFSqr(X)+GFSqr(Y)`, `(X*X)+Y*Y` = dx first; five body shapes, four operator- opt-ins,
//   two GFSqr opt-ins, dead-local and compile-position diagnostics leave the helper unchanged), so retail's split is c2 context.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHT_BELOW
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_CALCULATERADIUS
#define FABLE_RETAIL_CTCPHYSICSBASE_IMPULSE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGCIRCLE_BODY
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
#include "engine/CWorldMap.h"
#include "engine/CThingFilter_OverlappingCircle.h"
#include "engine/CFilter_Bind2.h"
template void CThingSearchTools::GetAllThingsInRoughArea<CThing,std::list<CThing*>,CFilter_Bind2<CThingFilter_OverlappingCircle,CWorldMap::CThingFilterBase> >(const C2DBoxI&,const CFilter_Bind2<CThingFilter_OverlappingCircle,CWorldMap::CThingFilterBase>&,std::list<CThing*>&)const;
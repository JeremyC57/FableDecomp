// CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind3<CThingFilter_SolidToThing, CThingFilter_PhysicsFlagsSolidToThing, CThingFilter_OverlappingCircle>, `anonymous namespace'::CThingFindHeightZBelowAt>(const C2DBoxI& area, const CFilter_Bind3<...>& filter_func, const CThingFindHeightZBelowAt& op_func) const -- run the height-below functor on every physical thing in the mapwho cells under the area that is not the source thing, collides like the source and overlaps the filter circle, each thing once per search; true when any call returned true
// Leaf: ??$ForEachInRoughArea@VCThingPhysical@@V?$CFilter_Bind3@VCThingFilter_SolidToThing@@VCThingFilter_PhysicsFlagsSolidToThing@@VCThingFilter_OverlappingCircle@@@@VCThingFindHeightZBelowAt@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A1E0, 725 bytes, thiscall `ret 0Ch`; one caller 0x00666E8B (CThingCreatureBase TU, thing_creature_base.cpp).
// Identity: Ego_r.pdb / FableWin.pdb ??$ForEachInRoughArea@VCThingPhysical@@V?$CFilter_Bind3@VCThingFilter_SolidToThing@@
//   VCThingFilter_PhysicsFlagsSolidToThing@@VCThingFilter_OverlappingCircle@@@@VCThingFindHeightZBelowAt@?A0xeff05290@@@CThingSearchTools@@QBE_N
//   ABVC2DBoxI@@ABV?$CFilter_Bind3@...@@ABVCThingFindHeightZBelowAt@?A0xeff05290@@@Z (thing_creature_base.obj; the anonymous-namespace hash
//   is per compile, so the Leaf line stops before it). The catalog name is the display form of the same.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, CTCPhysicsBase virtuals
//   CanCollideWithThings (+0xE4) / CanCollideWithThingsOneWay (+0xE8), CTCPhysicsBase::GetMapwhoPosition 0x006AFCB0,
//   `anonymous namespace'::CThingFindHeightZBelowAt::operator() 0x00668AB0, memmove 0x00BFEAE6 (the AreaMaps clear).
// Shape: the egod rough-area walk under the new overlay opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD (Ego_d locals in
//   FableWin 0x01DAAC80's order: got_one / flags first, the empty test on the edges, `filter_func(*tthing) && op_func(*tthing)`), with
//   the three filter bodies visible as retail expands them: FABLE_RETAIL_CTHINGFILTER_SOLIDTOTHING_BODY (new overlay opt-in; the same text
//   grades MATCH at its out-of-line copy 0x00661640), FABLE_RETAIL_CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_BODY (new overlay header
//   engine/CThingFilter_PhysicsFlagsSolidToThing.h, Ego_d class record) and the landed FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGCIRCLE_BODY
//   (the one-expression GetSquaredMagnitude2D squares dy first here, 0x0066A3C4, as in 0x0063FD40). CTCPHYSICSBASE_CALCULATERADIUS declares
//   GetMapwhoPosition; CTCPHYSICSBASE_IMPULSE (declarations only) restores the `public:` GetRadius needs (as 0x0063FD40). The functor is the
//   TU's own anonymous-namespace class (Ego_d record 0x4EE6C: From / To const C3DVector&, Height float; constructor, operator() const,
//   GetHeight), declared here; its operator() is the out-of-line 0x00668AB0. RELOCATION_MATCH at the third grade (the GetWidth / GetHeight
//   empty test of the GetAllThingsInRoughArea copies gave 725v725 with a kept width; FableWin compares the edges). No flags.
// Not this row: the out-of-line Bind3 copy 0x00669040 expands all three filters in retail; emitted from this TU it calls the flags and
//   circle filters (the landed local model of 0x00669040 forces them inline). Recorded for the coordinator, no change made.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_CONSTPROC
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
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
#define FABLE_RETAIL_CTHINGFILTER_SOLIDTOTHING_BODY
#define FABLE_RETAIL_CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_BODY
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_SolidToThing.h"
#include "engine/CThingFilter_PhysicsFlagsSolidToThing.h"
#include "engine/CThingFilter_OverlappingCircle.h"
#include "engine/CFilter_Bind3.h"
namespace {
// Ego_d.pdb ``anonymous-namespace'::CThingFindHeightZBelowAt` (thing_creature_base.cpp's own class; no shared header).
class CThingFindHeightZBelowAt {
public:
    CThingFindHeightZBelowAt(const C3DVector& from, const C3DVector& to, float height);
    bool operator()(const CThing& thing) const;                       // 0x00668AB0
    float GetHeight() const;
private:
    const C3DVector& From;                                             // +0x00
    const C3DVector& To;                                               // +0x04
    mutable float Height;                                              // +0x08
};
}
template bool CThingSearchTools::ForEachInRoughArea<CThingPhysical,CFilter_Bind3<CThingFilter_SolidToThing,CThingFilter_PhysicsFlagsSolidToThing,CThingFilter_OverlappingCircle>,CThingFindHeightZBelowAt>(const C2DBoxI&,const CFilter_Bind3<CThingFilter_SolidToThing,CThingFilter_PhysicsFlagsSolidToThing,CThingFilter_OverlappingCircle>&,const CThingFindHeightZBelowAt&)const;
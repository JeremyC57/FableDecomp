// CThingCreatureBase::GetHeightZBelowAt(const C3DVector& pos, bool include_theme_height, bool include_things) -- ground (or theme) height under pos; with include_things, raise it to the highest solid physics mesh hit by a vertical segment from just above the creature's centre to 50 below it, among the solid things overlapping the creature's circle
// Leaf: ?GetHeightZBelowAt@CThingCreatureBase@@QBEMABVC3DVector@@_N1@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00666D40, 348 bytes, thiscall, `ret 0Ch`. Grade: RELOCATION_MATCH (348/348).
// Shape: Ego_d.pdb lines 2770..2798 and its locals area, radius, extra_size_area, height_z, { from, to_centre, to,
//   get_height_below_func } (to_centre: type 0x2460 C3DVector, bprel -64). Every line's VC7.1 /Od size maps onto Ego_d's (VC10 /Od)
//   with the calibrated deltas: +3 per float-variable argument (2788 23 = 20+3, 2793 79 = 76+3), +4 per 0.0f literal argument (VC7.1
//   `push 0`, VC10 fldz/push/fstp: landed 0x0075C420 line 206 `C3DVector offset(0.0f, 0.0f, 0.0f)` 14 -> 26), `x += 1.0f` equal in both
//   (fadd [const]: landed 0x00B187D0 line 74, 18 = 18). Block 2782..2786 = 27 / 16 / 12 / 12 / 12:
//   `to_centre(0.0f, 0.0f, radius)` 16+3+8 = 27; `from = pos + to_centre` 16 (hidden return slot + operand + this + call); `to = from`
//   12; `from.Z += 1.0f` 12 (`+= extra_size_area` would be 9); `to.Z -= 50.0f` 12. Same idiom (an offset vector named for where it
//   points, added to the position) as Ego_d's CTCPhysicsControlled::UpdateOnSurface locals `up` / `down`.
//   Release: from = pos + (0,0,radius) and to = from share one pos.Z+radius on the x87 stack (`fld st0; fxch st1; fadd [1.0]`).
// Same-TU functor: `anonymous namespace'::CThingFindHeightZBelowAt (Ego_d TPI 0x4EE6C: public ctor / operator() const / GetHeight,
//   private const C3DVector& From +0, const C3DVector& To +4, float Height +8; operator() is the landed 0x00668AB0). Its ctor and
//   GetHeight are one-line in-class bodies (Ego_d lines 146 / 170, 42 / 17 B; no Fable.exe copy). Header opt-ins (overlay, add-only): the two filter ctors are visible
//   inline bodies (FABLE_RETAIL_CTHINGFILTER_SOLIDTOTHING_CTOR_BODY / _PHYSICSFLAGSSOLIDTOTHING_CTOR_BODY, Ego_d ctor line tables).
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINESEGMENT
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_CONSTPROC
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTZBELOW
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_IMPULSE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_CTHINGFILTER_SOLIDTOTHING_CTOR_BODY
#define FABLE_RETAIL_CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_CTOR_BODY
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_SolidToThing.h"
#include "engine/CThingFilter_PhysicsFlagsSolidToThing.h"
#include "engine/CThingFilter_OverlappingCircle.h"
#include "engine/CFilter_Bind3.h"
namespace {
class CThingFindHeightZBelowAt {
public:
    CThingFindHeightZBelowAt(const C3DVector& from, const C3DVector& to, float height) : From(from), To(to), Height(height) {}
    bool operator()(const CThing& thing) const;                       // 0x00668AB0
    float GetHeight() const { return Height; }
private:
    const C3DVector& From;                                             // +0x00
    const C3DVector& To;                                               // +0x04
    mutable float Height;                                              // +0x08
};
}
float CThingCreatureBase::GetHeightZBelowAt(const C3DVector& pos, bool include_theme_height, bool include_things) const
{
    float extra_size_area = 1.0f;
    float radius = GetPhysicsTC()->GetRadius();
    C2DBoxI area((long)(pos.X - radius - extra_size_area), (long)(pos.Y - radius - extra_size_area),
                 (long)(pos.X + radius + extra_size_area), (long)(pos.Y + radius + extra_size_area));
    float height_z = include_theme_height ? PeekWorldMap().GetThemeSizeZAt(pos) : PeekWorldMap().GetGroundSizeZAt(pos);
    if (include_things) {
        C3DVector to_centre(0.0f, 0.0f, radius);
        C3DVector from = pos + to_centre;
        C3DVector to = from;
        from.Z += 1.0f;
        to.Z -= 50.0f;
        CThingFindHeightZBelowAt get_height_below_func(from, to, height_z);
        PeekThingSearchTools().ForEachInRoughArea<CThingPhysical>(area, GFFilter_Bind3(CThingFilter_SolidToThing(*this), CThingFilter_PhysicsFlagsSolidToThing(*this), CThingFilter_OverlappingCircle(pos, radius)), get_height_below_func);
        height_z = get_height_below_func.GetHeight();
    }
    return height_z;
}
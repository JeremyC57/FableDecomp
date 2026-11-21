// Bracket: RELOCATION_MATCH only without Ego_d's to_centre local (Ego_d 2782..2786 = 27/16/12/12/12 B; this text 12/9/12/9/12); held, not landed (advisor 2026-11-21, ADVISOR_DECISIONS_B).
// Leaf: ?GetHeightZBelowAt@CThingCreatureBase@@QBEMABVC3DVector@@_N1@Z
// Flags: none
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
        C3DVector from = pos;
        from.Z += radius;
        C3DVector to = from;
        from.Z += extra_size_area;
        to.Z -= 50.0f;
        CThingFindHeightZBelowAt get_height_below_func(from, to, height_z);
        PeekThingSearchTools().ForEachInRoughArea<CThingPhysical>(area, GFFilter_Bind3(CThingFilter_SolidToThing(*this), CThingFilter_PhysicsFlagsSolidToThing(*this), CThingFilter_OverlappingCircle(pos, radius)), get_height_below_func);
        height_z = get_height_below_func.GetHeight();
    }
    return height_z;
}

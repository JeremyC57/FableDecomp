// CAIStateGroup_BanditSitRoundCampfire::GetPNearCampfire -- select a free village or nearby campfire.
// Flags: none
// Leaf: ?GetPNearCampfire@CAIStateGroup_BanditSitRoundCampfire@@ABEPAVCThingObject@@XZ
// Inferred: assign DistanceSq after FromPos construction. The retail store runs after both position copies at 0093AD3E.
#define FABLE_RETAIL_CISNEARCAMPFIREWITHSPACE_RANGE_SCOPE_PASS26
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST
#define FABLE_RETAIL_CTHINGOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGOBJECT_STATGETTYPE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_PEEKCAMPFIRES_PASS26
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_DECL_PASS26
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
#define FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGOBJECT_HASPROPERTY
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE_ACTIONPOINTFREE_PASS26
#define FABLE_RETAIL_CTHINGOBJECT_HASPROPERTY_MASK_PASS26
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CAIStateGroup_BanditSitRoundCampfire.h"
#include "engine/CAIBrain.h"
#include "engine/CIsCampfireWithSpace.h"
#include "engine/CIsNearCampfireWithSpace.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CThingSearchTools.h"
#include "engine/ThingAlive.h"
#include "engine/CTCUserActionPointBase.h"

// FableWin 02B7F120, the original predicate reads the object argument and never captures its empty receiver.
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline bool CIsCampfireWithSpace::operator()(const CThingObject& campfire) const
{
    const CTCSingleUserActionPoints* ptc_action_points;
    if (campfire.HasProperty(0x4000UL) && campfire.PeekTC(&ptc_action_points))
        return ptc_action_points->IsActionPointFree();
    return false;
}

// FableWin 02B7A940, matching the predicate called at native 0093B698.
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline bool CIsNearCampfireWithSpace::operator()(const CThingObject& campfire) const
{
    if (campfire.HasProperty(0x4000UL) && (campfire.GetPos() - FromPos).GetSquaredMagnitude() <= DistanceSq) {
        const CTCSingleUserActionPoints* ptc_action_points;
        if (campfire.PeekTC(&ptc_action_points))
            return ptc_action_points->IsActionPointFree();
    }
    return false;
}

// FableWin 02B6CED0, matching the candidate type at native 0093B6A7.
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
CThingObject* CThingCompare_Nearest::operator()(CThingObject* first, CThingObject* second) const
{
    if (!first)
        return second;
    if (!second)
        return first;
    return (first->GetPos() - Pos).GetSquaredMagnitude() <
           (second->GetPos() - Pos).GetSquaredMagnitude() ? first : second;
}

CThingObject* CAIStateGroup_BanditSitRoundCampfire::GetPNearCampfire() const
{
    CThingObject* pcampfire = 0;
    const CTCVillageMember* ptc_vill_mem;
    if (!GetCreature().PeekTC(&ptc_vill_mem))
        FABLE_RETAIL_ASSERT_ARM(false);

    const CTCVillage* ptc_village = ptc_vill_mem->GetPVillage();
    if (ptc_village) {
        std::list<CIntelligentPointer<CThing> > village_campfires(ptc_village->PeekVillageCampfires());
        CIsCampfireWithSpace is_campfire_with_space;
        float best_sq_dist = 0.0f;
        {
            for (std::list<CIntelligentPointer<CThing> >::const_iterator it = village_campfires.begin(),
                 end_it = village_campfires.end(); it != end_it; ++it) {
                if (IS_THING_ALIVE(*it) && (*it)->GetType() == CThingObject::StatGetType()) {
                    CThingObject* pthis_campfire = GFDebugPDynamicCast((*it).Get(), (CThingObject*)0);
                    if (is_campfire_with_space(*pthis_campfire)) {
                        float this_sq_dist = (pthis_campfire->GetPos() - GetCreature().GetPos()).GetSquaredMagnitude();
                        if (!pcampfire || this_sq_dist < best_sq_dist) {
                            pcampfire = pthis_campfire;
                            best_sq_dist = this_sq_dist;
                        }
                    }
                }
            }
        }
    }
    if (!pcampfire) {
        PeekThingSearchTools().GetPBestThing(CIsNearCampfireWithSpace(22.0f, GetCreature().GetPos()),
                                            CThingCompare_Nearest(GetCreature().GetPos()), &pcampfire);
    }
    return pcampfire;
}
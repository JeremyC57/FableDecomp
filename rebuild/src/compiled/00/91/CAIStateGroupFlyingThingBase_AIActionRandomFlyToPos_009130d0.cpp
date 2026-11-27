// 009130d0 CAIStateGroup_FlyingThingBase::AIAction_RandomFlyToPos
// Leaf: ?AIAction_RandomFlyToPos@CAIStateGroup_FlyingThingBase@@IAE?AW4EAIActionReturn@@ABVC3DVector@@@Z
// Flags: none
// Inline-budget: The native deque insertion expands placement construction for CBezierCurve. Its separately emitted copies are checked in PICKANY.txt.
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22
#define FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_CAISTATEGROUP_FLYINGTHINGBASE
#define FABLE_STLPORT_DEQUE_PUSH_BACK
#define FABLE_RETAIL_STLP_DEQUE_CLEAR
#define FABLE_STLPORT_DEQUE_PUSH_BACK_AUX_NOINLINE
// 009130d0 CAIStateGroup_FlyingThingBase::AIAction_RandomFlyToPos
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTZBELOW
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINESEGMENT
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGATORTYPE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_SETNAVIGATORTOTYPE
#define FABLE_RETAIL_FLIGHT_PASS29
#define FABLE_RETAIL_CBEZIERCURVE
#define FABLE_RETAIL_CBEZIERCURVE_GETPOINTONCURVE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_STLP_DEQUE_PASS22_POP
#define FABLE_RETAIL_STLP_DEQUE_MSUBTRACT_BODY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#include "fable_assert.h"
#include <math.h>
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorldMap.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CAIStateGroup_FlyingThingBase.h"
#include "engine/CBrainUpdateZone.h"
#include "engine/CTCPhysicsBase.h"

#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/CBezierCurve.h"
#include "stlport/fable_deque.h"
namespace std {
template<> FABLE_FORCEINLINE void FABLE_FASTCALL _Construct<CBezierCurve,CBezierCurve>(CBezierCurve* p,const CBezierCurve& v) { new(p) CBezierCurve(v); }
}
extern const unsigned long NAVIGATOR_TYPE_FLYER;
extern const unsigned long NAVIGATOR_TYPE_A_STAR;
extern const float DELTA_NOTIONAL_ZERO;
namespace {
const C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM(0.0f, 0.0f, 25.0f);
}




namespace std {
template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::clear()
{
    for (_Tp** __node = this->_M_start._M_node + 1; __node < this->_M_finish._M_node; ++__node) {
        _Destroy(*__node, *__node + this->buffer_size());
        this->_M_map_size.deallocate(*__node, this->buffer_size());
    }
    if (this->_M_start._M_node != this->_M_finish._M_node) {
        _Destroy(this->_M_start._M_cur, this->_M_start._M_last);
        _Destroy(this->_M_finish._M_first, this->_M_finish._M_cur);
        this->_M_map_size.deallocate(this->_M_finish._M_first, this->buffer_size());
    }
    else
        _Destroy(this->_M_start._M_cur, this->_M_finish._M_cur);
    this->_M_finish = this->_M_start;
}
}
namespace std {
template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::_M_reallocate_map(size_type __nodes_to_add, bool __add_at_front)
{
    size_type __old_num_nodes = this->_M_finish._M_node - this->_M_start._M_node + 1;
    size_type __new_num_nodes = __old_num_nodes + __nodes_to_add;

    _Tp** __new_nstart;
    if (this->_M_map_size._M_data > 2 * __new_num_nodes) {
        __new_nstart = this->_M_map._M_data + (this->_M_map_size._M_data - __new_num_nodes) / 2
                       + (__add_at_front ? __nodes_to_add : 0);
        if (__new_nstart < this->_M_start._M_node)
            __copy_trivial(this->_M_start._M_node, this->_M_finish._M_node + 1, __new_nstart);
        else
            __copy_trivial_backward(this->_M_start._M_node, this->_M_finish._M_node + 1, __new_nstart + __old_num_nodes);
    } else {
        size_type __new_map_size =
            this->_M_map_size._M_data + (max)(this->_M_map_size._M_data, __nodes_to_add) + 2;

        _Tp** __new_map = this->_M_map.allocate(__new_map_size);
        __new_nstart = __new_map + (__new_map_size - __new_num_nodes) / 2
                       + (__add_at_front ? __nodes_to_add : 0);
        __copy_trivial(this->_M_start._M_node, this->_M_finish._M_node + 1, __new_nstart);
        this->_M_map.deallocate(this->_M_map._M_data, this->_M_map_size._M_data);

        this->_M_map._M_data = __new_map;
        this->_M_map_size._M_data = __new_map_size;
    }

    this->_M_start._M_set_node(__new_nstart);
    this->_M_finish._M_set_node(__new_nstart + __old_num_nodes - 1);
}
}
bool CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector& from_pos, float dist_to_keep_within, C3DVector& random_pos, bool avoid_themes) const
{
    if (PeekWorldMap().GetMapNumberAt(from_pos) != PeekWorldMap().GetMapNumberAt(GetCreature().GetPos()))
        return false;
    CTCCreatureNavigation* pnav;
    GetCreature().GetTC(TCI_NAVIGATION, &pnav);
    unsigned long nav_type = pnav->GetNavigatorType();
    pnav->SetNavigatorToType(NAVIGATOR_TYPE_FLYER);
    if (!pnav->GetNearbyWanderPosition(from_pos, dist_to_keep_within, random_pos, true))
        return false;
    pnav->SetNavigatorToType(nav_type);
    random_pos.Z = GetCreature().GetHeightZBelowAt(ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM + random_pos, true, true) + MinHeightToKeepAboveGround;
    float theme_height = PeekWorldMap().GetThemeSizeZAt(random_pos);
    if (theme_height > random_pos.Z)
        random_pos.Z = theme_height;
    return true;
}



bool CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath(std::deque<CBezierCurve>& beziers)
{
    CBezierCurve new_curve;
    C3DVector point1 = beziers.back().Point4;
    new_curve.Point1 = point1;
    C3DVector p1_to_p2_vector = point1 - beziers.back().Point3;
    C3DVector control_point = p1_to_p2_vector + point1;
    long map_no = PeekWorldMap().GetMapNumberAt(point1);
    float ground_height = GetCreature().GetHeightZBelowAt(ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM + control_point, true, true);
    long count = 10;
    while ((ground_height + MinHeightToKeepAboveGroundInFlight > control_point.Z || control_point.X < 0.0f || control_point.Y < 0.0f
            || PeekWorldMap().GetMapNumberAt(control_point) != map_no) && count > 0) {
        p1_to_p2_vector *= 0.5f;
        control_point = p1_to_p2_vector + point1;
        ground_height = GetCreature().GetHeightZBelowAt(ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM + control_point, true, true);
        --count;
    }
    new_curve.Point2 = control_point;
    for (unsigned long c1 = 0; c1 < 2; ++c1) {
        C3DVector next_pos;
        if (!GetRandomFlightPos(point1, FlightRange, next_pos, false))
            return false;
        switch (c1) {
        case 0:
            next_pos.Z = next_pos.Z + MinHeightToKeepAboveGroundInFlight;
            new_curve.Point3 = next_pos;
            break;
        case 1: {
            next_pos.Z = GFFloatRandom(MaxHeightAboveGround, GetPWorldSeed()) + next_pos.Z + MinHeightToKeepAboveGroundInFlight;
            new_curve.Point4 = next_pos;
            break;
        }
        }
    }
    beziers.push_back(new_curve);
    return true;
}



bool CAIStateGroup_FlyingThingBase::CalculatePrettyPath(std::deque<CBezierCurve>& beziers, const C3DVector& final_dest)
{
    beziers.clear();
    CBezierCurve first_curve;
    first_curve.SetPoint1(GetCreature().PeekPos());
    for (unsigned long c1 = 2; c1 <= 4; ++c1) {
        C3DVector next_pos;
        if (!GetRandomFlightPos(first_curve.GetPoint1(), FlightRange, next_pos, true))
            return false;
        if (c1 == 2 || c1 == 4)
            next_pos.Z += GFFloatRandom(MaxHeightAboveGround, GetPWorldSeed()) + MinHeightToKeepAboveGroundInFlight;
        else
            next_pos.Z += GFFloatRandom(MaxHeightAboveGround * 0.25f, GetPWorldSeed()) + MinHeightToKeepAboveGroundInFlight;
        switch (c1) {
        case 2: first_curve.SetPoint2(next_pos); break;
        case 3: first_curve.SetPoint3(next_pos); break;
        case 4: first_curve.SetPoint4(next_pos); break;
        }
    }
    beziers.push_back(first_curve);
    unsigned long no_of_mid_curves = GFRandom(3, GetPWorldSeed());
    for (unsigned long c1 = 0; c1 < no_of_mid_curves; ++c1)
        if (!AddRandomBezierCurveToPath(beziers))
            return false;
    CBezierCurve last_curve;
    const C3DVector last_point1 = beziers.back().GetPoint4();
    last_curve.SetPoint1(last_point1);
    C3DVector p1_to_p2_vector = last_point1 - beziers.back().GetPoint3();
    C3DVector control_point = last_point1 + p1_to_p2_vector;
    float ground_height = GetCreature().GetHeightZBelowAt(control_point + ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM, true, true);
    long count = 10;
    while (ground_height + MinHeightToKeepAboveGroundInFlight > control_point.Z && count > 0) {
        p1_to_p2_vector *= 0.5f;
        control_point = last_point1 + p1_to_p2_vector;
        ground_height = GetCreature().GetHeightZBelowAt(control_point + ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM, true, true);
        --count;
    }
    if (control_point.Z < ground_height + MinHeightToKeepAboveGroundInFlight)
        control_point.Z = ground_height + MinHeightToKeepAboveGroundInFlight;
    last_curve.SetPoint2(control_point);
    last_curve.SetPoint4(final_dest);
    C3DVector final_control_point;
    if (!GetRandomFlightPos(final_dest, FlightRange, final_control_point, true))
        return false;
    final_control_point.Z += MinHeightToKeepAboveGroundInFlight;
    last_curve.SetPoint3(final_control_point);
    beziers.push_back(last_curve);
    return true;
}
#include "engine/GFPower.h"
C3DVector CBezierCurve::GetPointOnCurve(float parameter_t) const
{
    C3DVector return_point=Point1 * GFPower(1.0f - parameter_t, 3.0f)
        + Point2 * 3.0f * parameter_t * GFSqr(1.0f - parameter_t)
        + Point3 * 3.0f * GFSqr(parameter_t) * (1.0f - parameter_t)
        + Point4 * GFPower(parameter_t, 3.0f);
    return return_point;
}

EAIActionReturn CAIStateGroup_FlyingThingBase::AIAction_RandomFlyToPos(const C3DVector& pos)
{
    if (IsStopping())
        return AI_ACTION_TERMINATE_PROCESS;

    CTCCreatureNavigation* pnav;
    if (!GetCreature().GetTC(&pnav)) {
        FABLE_RETAIL_ASSERT_ARM(false);
    }
    pnav->SetNavigatorToType(NAVIGATOR_TYPE_FLYER);
    std::deque<CBezierCurve> pretty_path;
    if (!CalculatePrettyPath(pretty_path, pos)) {
        pnav->SetNavigatorToType(NAVIGATOR_TYPE_A_STAR);
        return AI_ACTION_FAILED;
    }

    const CBrainUpdateZone* pupdate_zone = PBrain->PeekPCurrentUpdateZone();
    long muliply_speed_factor = 1;
    if (pupdate_zone)
        muliply_speed_factor = pupdate_zone->UpdateFrequency;
    C3DVector to_pos = GetCreature().GetPos() + GetCreature().GetPhysicsTC()->GetFacingVector();
    float nav_close_enough_sq = GFSqr(0.01f + GetCreature().GetPhysicsTC()->GetRadius()
        + GetCreature().GetMaxFlyingSpeed(to_pos) * muliply_speed_factor);
    while (!pretty_path.empty()) {
        if (false) {} else for (float parameter_t = 0.1f; parameter_t <= 1.0f + DELTA_NOTIONAL_ZERO; parameter_t += 0.1f) {
            bool final_point = pretty_path.size() == 1 && fabs(parameter_t - 1.0f) < DELTA_NOTIONAL_ZERO;
            C3DVector target_pos = pretty_path.front().GetPointOnCurve(parameter_t);
            if (final_point || (target_pos - GetCreature().PeekPos()).GetSquaredMagnitude2D() >= nav_close_enough_sq) {
                EAIActionReturn ret = AI_ACTION_FAILED;
                bool close_enough = false;
                bool nav_route_calculated = false;
                do {
                    ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, target_pos, 0.01f, nav_route_calculated,
                        AI_MOVEMENT_FLYING, true, false, true, 0, true, 0.0f);
                    close_enough = !final_point && (target_pos - GetCreature().PeekPos()).GetSquaredMagnitude2D() <= nav_close_enough_sq;
                } while (ret == AI_ACTION_IN_PROGRESS && !close_enough);
                if (ret != AI_ACTION_FINISHED && ret != AI_ACTION_IN_PROGRESS)
                    return ret;
            }
        }
        pretty_path.pop_front();
    }
    GetCreature().GetPhysicsTC()->SetFacingAngleYZ(0.0f);
    pnav->SetNavigatorToType(NAVIGATOR_TYPE_A_STAR);
    return AI_ACTION_FINISHED;
}
// 00a18fc0 CNavigatorFlyer::MoveToPosProximity -- Validates the destination and computes the next above-ground flight step.
// Grade: DIFFER(1127v1125) -- Frame and scratch-CPU operand assignment differ at entry. The normalisation and final altitude tests also schedule differently.
// Retail: 0x00A18FC0, 1125 bytes, thiscall.
// Leaf: ?MoveToPosProximity@CNavigatorFlyer@@UAE?AW4ENavigatorReturn@@ABVC3DVector@@0MMAAV3@_N22222M@Z
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CNAVIGATORBASE_HEIGHTTHEMES
#define FABLE_RETAIL_CNAVIGATORFLYER
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CNAVIGATORMANAGER_SAMEMAP
#define FABLE_RETAIL_CNAVIGATORMANAGER_GETNODEAT
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX
#include "engine/ENavigatorReturn.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CNavigatorFlyer.h"
#include "engine/CNavigatorManager.h"
#include <math.h>
extern const float DELTA_NOTIONAL_ZERO;
extern C3DVector ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM;
// Ego_d lib_navigator_flyer.cpp S_LDATA32, corroborated by the native altitude clamp at 0x0129CB48.
extern const float MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT;

ENavigatorReturn CNavigatorFlyer::MoveToPosProximity(const C3DVector& current_pos, const C3DVector& dest_pos, float proximity,
                                                     float current_speed, C3DVector& resulting_vector, bool avoid_dynamic_obstacles, bool avoid_static_obstacles, bool reduce_speed_when_near, bool fail_on_dynamic_blockage, bool high_priority, bool ignore_path_preferability, float distance_to_lod_centre)
{
    if (Interrupted || !OnRoute ||
        (float)fabs(dest_pos.X - DestinationPos.X) > DELTA_NOTIONAL_ZERO ||
        (float)fabs(dest_pos.Y - DestinationPos.Y) > DELTA_NOTIONAL_ZERO ||
        (float)fabs(dest_pos.Z - DestinationPos.Z) > DELTA_NOTIONAL_ZERO) {
        Interrupted = false;
        DestinationPos = dest_pos;
        if (!NavigationManager->GetNodeAt(dest_pos, false) ||
            PInterface->GetThemeHeightAt(dest_pos) - DELTA_NOTIONAL_ZERO > dest_pos.Z ||
            !NavigationManager->IsInSameMap(current_pos, dest_pos)) {
            OnRoute = false;
            return NAV_CANNOT_REACH_TARGET;
        }
        OnRoute = true;
    }
    float range = PInterface->PeekNavigatorRadius(false) + proximity;
    float prox_and_rad_sq = range * range;
    C3DVector straight_path = dest_pos - current_pos;
    if (straight_path.GetSquaredMagnitude() <= prox_and_rad_sq) {
        resulting_vector.X = 0.0f;
        resulting_vector.Y = 0.0f;
        resulting_vector.Z = 0.0f;
        OnRoute = false;
        return NAV_REACHED_TARGET;
    }
    if (straight_path.GetSquaredMagnitude2D() <= prox_and_rad_sq &&
        PInterface->GetThemeHeightAt(dest_pos) + DELTA_NOTIONAL_ZERO < dest_pos.Z) {
        resulting_vector.X = 0.0f;
        resulting_vector.Y = 0.0f;
        resulting_vector.Z = 0.0f;
        OnRoute = false;
        return NAV_REACHED_TARGET;
    }
    float speed_sq = current_speed * current_speed;
    if (straight_path.GetSquaredMagnitude() > speed_sq) {
        straight_path.Normalise();
        straight_path *= current_speed;
    }
    C3DVector new_pos = straight_path + current_pos;
    float ground_height = FindPosZAtIncludingThemes(new_pos + ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM, true);
    C2DVector flat = C2DVector(dest_pos) - C2DVector(current_pos);
    if (flat.X * flat.X + flat.Y * flat.Y > speed_sq) {
        if (new_pos.Z - ground_height < MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT) {
            new_pos.Z = MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT + ground_height;
            straight_path = new_pos - current_pos;
            straight_path.Normalise();
            straight_path *= current_speed;
        }
    }
    if (new_pos.Z < ground_height) {
        new_pos.Z = ground_height + MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT;
        straight_path = new_pos - current_pos;
    }
    resulting_vector = straight_path;
    return NAV_MOVING_TO_TARGET;
}

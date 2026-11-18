// 00877840 NCameraModes::CTools::IsLineOfSight -- Tests the camera ray and returns the nearest blocking hit.
// Leaf: ?IsLineOfSight@CTools@NCameraModes@@QBE_NABV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@ABVC3DVector@@1AAV5@2AAMPAPBVCThing@@W4ELOSTest@12@_N@Z
// Retail: 0x00877840, 1023 bytes, thiscall.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_LANDSCAPEFROMTO
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_LINEOFSIGHT_PASS24
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/NCameraModes_CTools.h"
extern const float DELTA_NOTIONAL_ZERO;

namespace NCameraModes {
bool CTools::IsLineOfSight(const std::vector<const CThing*>& thing_list, const C3DVector& from, const C3DVector& to, C3DVector& collision_pos,
                           C3DVector& normal_vector, float& cam_radius, const CThing** pphit_thing, ELOSTest test_flags, bool camera_test) const
{
    if (!((from - to).GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO))
        return true;
    bool intersection_occured = false;
    float best_cam_rad = 0.0f;
    const CThing* pbest_thing = 0;
    C3DVector best_pos;
    C3DVector best_normal;
    {
        C3DVector test_pos, test_normal;
        const CThing* ptest_thing;
        if ((test_flags & 9) && GetThingPointIntersectionFromTo(thing_list, from, to, test_pos, test_normal, &ptest_thing, camera_test, test_flags)) {
            float test_cam_rad = CameraMeshOffsetDistance;
            if ((ptest_thing->GetTypeFlags() & 8) && (test_flags & 8))
                test_cam_rad = CameraCreatureOffsetDistance;
            best_cam_rad = test_cam_rad;
            best_pos = test_pos;
            best_normal = test_normal;
            pbest_thing = ptest_thing;
            intersection_occured = true;
        }
    }
    if (test_flags & 4) {
        C3DVector test_pos, test_normal;
        if (GetThemePointIntersectionFromTo(from, to, test_pos, test_normal, true)) {
            float test_cam_rad = CameraThemeOffsetDistance;
            if (!intersection_occured || (test_pos - from).GetSquaredMagnitude() < (best_pos - from).GetSquaredMagnitude()) {
                best_pos = test_pos;
                best_normal = test_normal;
                best_cam_rad = test_cam_rad;
            }
            intersection_occured = true;
        }
    }
    if (test_flags & 0x10) {
        C3DVector test_pos, test_normal;
        if (GetThemePointIntersectionFromTo(from, to, test_pos, test_normal, false)) {
            float test_cam_rad = CameraThemeOffsetDistance;
            if (!intersection_occured || (test_pos - from).GetSquaredMagnitude() < (best_pos - from).GetSquaredMagnitude()) {
                best_pos = test_pos;
                best_normal = test_normal;
                best_cam_rad = test_cam_rad;
            }
            intersection_occured = true;
        }
    }
    if (test_flags & 2) {
        C3DVector test_pos, test_normal;
        if (GetLandscapePointIntersectionFromTo(from, to, test_pos, test_normal, camera_test)) {
            float test_cam_rad = CameraLandscapeOffsetDistance;
            if (!intersection_occured || (test_pos - from).GetSquaredMagnitude() < (best_pos - from).GetSquaredMagnitude()) {
                best_pos = test_pos;
                best_normal = test_normal;
                best_cam_rad = test_cam_rad;
            }
            intersection_occured = true;
        }
    }
    if (intersection_occured) {
        collision_pos = best_pos;
        normal_vector = best_normal;
        cam_radius = best_cam_rad;
        *pphit_thing = pbest_thing;
        return false;
    }
    return true;
}
}

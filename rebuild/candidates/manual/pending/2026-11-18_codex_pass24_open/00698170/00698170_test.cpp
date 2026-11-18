#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#ifndef _MSC_VER
#define __fastcall
#endif
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_NORMALISE
#include "engine/C3DVector.h"
#include <cassert>
#include <cmath>
#include <cstdio>

struct CameraToolsFixture
{
    mutable C3DVector seen[4];
    C3DVector expected_position, expected_look;
    mutable unsigned calls;
    unsigned mask;
    float fov;

    bool IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& cam_look_at,
                               const C3DVector& cam_pos, float input_fov) const
    {
        assert(cam_pos.X == expected_position.X && cam_pos.Y == expected_position.Y &&
               cam_pos.Z == expected_position.Z);
        assert(cam_look_at.X == expected_look.X && cam_look_at.Y == expected_look.Y &&
               cam_look_at.Z == expected_look.Z && input_fov == fov);
        assert(calls < 4);
        seen[calls] = pos;
        return (mask & (1U << calls++)) != 0;
    }
};

const float DELTA_NOTIONAL_ZERO=0.0001f;

C3DVector& C3DVector::AccurateNormalise()
{
    *this *= 1.0f/std::sqrt(Dot(*this));
    return *this;
}

struct CameraFixture
{
    float fov;
    float GetFOV() const { return fov; }
};
struct ModeFixture
{
    C3DVector position;
    struct Look { C3DVector Position; } look;
    const C3DVector& GetPosition() const { return position; }
    const Look& GetLookAtPhase() const { return look; }
};
struct CGameCameraManager
{
    CameraFixture Camera;
    ModeFixture* CurrentMode;
    CameraToolsFixture CamTools;
    bool IsVolumeInViewingFrustrum(const C3DVector& origin,float radius) const;
};

// The selected implementation is reproduced verbatim for this standalone fixture.
bool CGameCameraManager::IsVolumeInViewingFrustrum(const C3DVector& origin, float radius) const
{
    float fov = Camera.GetFOV();
    C3DVector cam_pos = CurrentMode->GetPosition();
    C3DVector cam_look_at = CurrentMode->GetLookAtPhase().Position;
    C3DVector line = cam_look_at - cam_pos;
    C3DVector perp;
    if (fabsf(line.Y) > DELTA_NOTIONAL_ZERO) {
        perp.X = 1.0f;
        perp.Z = 1.0f;
        perp.Y = -((line.X + line.Z) / line.Y);
    } else if (fabsf(line.Z) > DELTA_NOTIONAL_ZERO) {
        perp.X = 1.0f;
        perp.Y = 1.0f;
        perp.Z = -((line.X + line.Y) / line.Z);
    } else {
        return false;
    }
    perp.AccurateNormalise();
    C3DVector other_perp = perp ^ line;
    other_perp.AccurateNormalise();
    bool one_line_in = CamTools.IsPosInViewingFrustrum(origin + perp * radius, cam_look_at, cam_pos, fov) && CamTools.IsPosInViewingFrustrum(origin - perp * radius, cam_look_at, cam_pos, fov);
    bool other_line_in = CamTools.IsPosInViewingFrustrum(origin + other_perp * radius, cam_look_at, cam_pos, fov) && CamTools.IsPosInViewingFrustrum(origin - other_perp * radius, cam_look_at, cam_pos, fov);
    return one_line_in && other_line_in;
}

int main()
{
    const C3DVector center(3, 5, 7);
    for (unsigned axis = 0; axis < 3; ++axis) {
        for (unsigned mask = 0; mask < 16; ++mask) {
            CGameCameraManager manager;
            CameraToolsFixture& tool=manager.CamTools;
            tool.expected_position = C3DVector(1, 2, 3);
            const C3DVector line(axis == 0 ? 2.0f : 0.0f,
                                 axis == 1 ? 2.0f : 0.0f,
                                 axis == 2 ? 2.0f : 0.0f);
            tool.expected_look = tool.expected_position + line;
            tool.fov = 0.125f;
            tool.mask = mask;
            tool.calls = 0;
            ModeFixture mode;
            mode.position=tool.expected_position;
            mode.look.Position=tool.expected_look;
            manager.CurrentMode=&mode;
            manager.Camera.fov=tool.fov;
            bool result=manager.IsVolumeInViewingFrustrum(center,3.0f);
            if (axis == 0) {
                assert(!result && tool.calls == 0);
                continue;
            }
            const unsigned first_count = mask & 1 ? 2 : 1;
            const unsigned second_count = mask & (1U << first_count) ? 2 : 1;
            assert(tool.calls == first_count + second_count);
            assert(result == (mask == 15));
            for (unsigned i = 0; i < tool.calls; ++i) {
                const C3DVector delta = tool.seen[i] - center;
                assert(std::fabs(delta.Dot(line)) < 0.00001f);
                assert(std::fabs(delta.Dot(delta) - 9.0f) < 0.00001f);
            }
            if (first_count == 2) {
                assert(std::fabs(tool.seen[0].X + tool.seen[1].X - 2 * center.X) < 0.00001f);
                assert(std::fabs((tool.seen[0] - center).Dot(tool.seen[2] - center)) < 0.00001f);
            }
        }
    }
    std::puts("00698170_TEST PASS");
    return 0;
}

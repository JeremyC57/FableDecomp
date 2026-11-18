#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include "engine/CMatrix3x4.h"
#include <cassert>
#include <cmath>
#include <cstdio>

struct CPlane
{
    C3DVector Normal;
    float Distance;
};

class CEngineCamera
{
public:
    CPlane FrustrumPlanes[4];

    bool ClipBox(const CMatrix3x4& unit_to_world) const;

};

// The selected implementation is reproduced verbatim for this standalone fixture.
bool CEngineCamera::ClipBox(const CMatrix3x4& unit_to_world) const
{
    C3DVector points[8];
    points[0].Initialise(-1.0f, -1.0f, -1.0f);
    unit_to_world.TransformPoint(points[0]);
    points[1].Initialise(1.0f, -1.0f, -1.0f);
    unit_to_world.TransformPoint(points[1]);
    points[2].Initialise(-1.0f, 1.0f, -1.0f);
    unit_to_world.TransformPoint(points[2]);
    points[3].Initialise(1.0f, 1.0f, -1.0f);
    unit_to_world.TransformPoint(points[3]);
    points[4].Initialise(-1.0f, -1.0f, 1.0f);
    unit_to_world.TransformPoint(points[4]);
    points[5].Initialise(1.0f, -1.0f, 1.0f);
    unit_to_world.TransformPoint(points[5]);
    points[6].Initialise(-1.0f, 1.0f, 1.0f);
    unit_to_world.TransformPoint(points[6]);
    points[7].Initialise(1.0f, 1.0f, 1.0f);
    unit_to_world.TransformPoint(points[7]);

    for (unsigned long c_plane = 0; c_plane < 4; ++c_plane) {
        bool outside = true;
        const CPlane& plane = FrustrumPlanes[c_plane];
        for (unsigned long c_point = 0; c_point < 8; ++c_point) {
            if (plane.Normal.Dot(points[c_point]) - plane.Distance < 0.0f) {
                outside = false;
                break;
            }
        }
        if (outside)
            return false;
    }
    return true;
}

int main()
{
    CEngineCamera camera;
    camera.FrustrumPlanes[0].Normal = C3DVector(1, 0, 1);
    camera.FrustrumPlanes[1].Normal = C3DVector(-1, 0, 1);
    camera.FrustrumPlanes[2].Normal = C3DVector(0, 1, 1);
    camera.FrustrumPlanes[3].Normal = C3DVector(0, -1, 1);
    for (unsigned i = 0; i < 4; ++i)
        camera.FrustrumPlanes[i].Distance = 2.0f;

    CMatrix3x4 m;
    m.E11 = 2; m.E12 = -1; m.E13 = 0;
    m.E21 = 1; m.E22 = 3; m.E23 = 0;
    m.E31 = 0; m.E32 = 1; m.E33 = 2;
    for (int x = -10; x <= 10; ++x) {
        for (int y = -10; y <= 10; ++y) {
            for (int z = -4; z <= 4; ++z) {
                m.E41 = float(x); m.E42 = float(y); m.E43 = float(z);
                bool expected = true;
                for (unsigned i = 0; i < 4; ++i) {
                    const CPlane& p = camera.FrustrumPlanes[i];
                    const float nx = p.Normal.X, ny = p.Normal.Y, nz = p.Normal.Z;
                    const float radius = std::fabs(nx * 2 - ny) +
                                         std::fabs(nx + ny * 3) +
                                         std::fabs(ny + nz * 2);
                    if (nx * x + ny * y + nz * z - radius >= p.Distance)
                        expected = false;
                }
                assert(camera.ClipBox(m) == expected);
            }
        }
    }
    std::puts("00b30610_TEST PASS");
    return 0;
}

// CEngineCamera::ClipBox(const CMatrix3x4& unit_to_world) const -- test the eight corners of a transformed unit cube against the camera side planes
// Grade: DIFFER(678v710) -- The shared matrix operator produces fewer repeated corner sums and shrinks the leaf by 32 bytes.
// Leaf: ?ClipBox@CEngineCamera@@QBE_NABVCMatrix3x4@@@Z
// Flags: none
// Retail: 0x00B30610, 710 bytes, const thiscall.
// Ego_d names and scopes from engine_camera.obj.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPBOX
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CEngineCamera.h"

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

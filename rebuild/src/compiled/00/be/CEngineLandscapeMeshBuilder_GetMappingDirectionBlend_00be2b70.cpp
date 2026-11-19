// CEngineLandscapeMeshBuilder::GetMappingDirectionBlend -- blend weight of one texture-mapping direction for a landscape normal.
// Leaf: ?GetMappingDirectionBlend@CEngineLandscapeMeshBuilder@@QAEMW4LANDSCAPE_TEXTURE_MAPPING_DIRECTION@@ABVC3DVector@@@Z
// Flags: none
// Retail: 0x00BE2B70, 343 bytes, thiscall, ret 8.
// Evidence: Ego_d.pdb locals dir, normal, map_normal (C3DVector copy), vert_angle, base_blend, normal2d, dot, blend (blend_start folds);
//   FableWin.exe 0x02CAE000 copies MappingDirNormals[dir] with ??0C3DVector@@QAE@ABV0@@Z, builds normal2d from normal and dots it with
//   C2DVector(map_normal). Retail keeps that copy in the frame: X/Y at [esp+10h]/[esp+14h] read by the dot (0x00BE2C2C), Z's store dead
//   but its slot kept -- the `sub esp,14h` the parked drafts missed. The arithmetic is retail's (2/pi scale, x4 / x2), not the donor
//   revision's divisions. MappingDirNormals: Ego_r / FableWin ?MappingDirNormals@CEngineLandscapeMeshBuilder@@0PAVC3DVector@@A (new opt-in).
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGBLEND
#define FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGDIRNORMALS
#include <math.h>
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CEngineLandscapeMeshBuilder.h"

float CEngineLandscapeMeshBuilder::GetMappingDirectionBlend(LANDSCAPE_TEXTURE_MAPPING_DIRECTION dir, const C3DVector& normal)
{
    C3DVector map_normal = MappingDirNormals[dir];
    float vert_angle = asin(normal.Z);
    float base_blend = (vert_angle * 0.63661975f - 0.5f) * 4.0f;
    if (base_blend < 0.0f)
        base_blend = 0.0f;
    else if (base_blend > 1.0f)
        base_blend = 1.0f;
    if (dir == 0)
        return base_blend;
    if (base_blend == 1.0f)
        return 0.0f;
    C2DVector normal2d(normal);
    normal2d.AccurateNormalise();
    float dot = normal2d.Dot(C2DVector(map_normal));
    float blend = 1.0f - (acos(dot) * 0.63661975f - 0.25f) * 2.0f;
    if (blend < 0.0f)
        blend = 0.0f;
    else if (blend > 1.0f)
        blend = 1.0f;
    if (base_blend == 0.0f)
        return blend;
    if (blend == 0.0f)
        return 0.0f;
    return (1.0f - base_blend) * blend;
}
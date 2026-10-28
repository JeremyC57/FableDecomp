// CEngineLandscapeMeshBuilder::GetMappingDirectionBlend(LANDSCAPE_TEXTURE_MAPPING_DIRECTION direction, const C3DVector& normal) -- how much a landscape texture mapping direction applies to a surface normal: a vertical factor from asin(normal.Z), a horizontal factor from acos of the normalised XY normal against the direction's XY, combined (direction 0 = the vertical factor alone)
// Grade: DIFFER(343v343) -- frame only: retail reserves `sub esp,14h` (one dead 4-byte slot above the two C2DVector locals), VC7.1 `sub esp,10h`, so every frame offset differs by 4; a result variable, dot/angle locals, GFClamp, a temp reference, asinf-style overloads and a C3DVector local copy were inert
// Leaf: ?GetMappingDirectionBlend@CEngineLandscapeMeshBuilder@@QAEMW4LANDSCAPE_TEXTURE_MAPPING_DIRECTION@@ABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE2B70, 343 bytes, thiscall `ret 8`, float. 1 grade used.
// Identity (proven): Ego_r.pdb signature (thiscall, enum + const C3DVector&, float) in donor order before
//   BuildSharedIndexBufferGenerationTables 0x00BE2CD0; caller CEngineLandscapeRenderer::BuildBlendingTables (site 0x00B673B8).
//   Catalog label "BuildBlendingTables_ComputeBlendFactor" is descriptive.
// Callees: _CIasin 0x00BFEDC4 / _CIacos 0x00BFEBA2 (intrinsics), C2DVector::AccurateNormalise 0x00A14510; the five mapping
//   direction vectors at 0x0139C338.
extern "C" double __cdecl asin(double x);
extern "C" double __cdecl acos(double x);
#pragma intrinsic(asin, acos)
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector& AccurateNormalise();                      // 0x00A14510
    float Dot(const C2DVector& v) const { return X * v.X + Y * v.Y; }
};
enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION { };
extern C3DVector g_MappingDirectionVectors_0139c338[];   // 0x0139C338

class CEngineLandscapeMeshBuilder { public:
    float GetMappingDirectionBlend(LANDSCAPE_TEXTURE_MAPPING_DIRECTION direction, const C3DVector& normal);
};

float CEngineLandscapeMeshBuilder::GetMappingDirectionBlend(LANDSCAPE_TEXTURE_MAPPING_DIRECTION direction, const C3DVector& normal)
{
    const C3DVector& mappingDirection = g_MappingDirectionVectors_0139c338[direction];
    C2DVector direction2D(mappingDirection.X, mappingDirection.Y);
    float vertical = ((float)asin(normal.Z) * 0.63661975f - 0.5f) * 4.0f;
    if (vertical < 0.0f)
        vertical = 0.0f;
    else if (vertical > 1.0f)
        vertical = 1.0f;
    if (direction == 0)
        return vertical;
    if (vertical == 1.0f)
        return 0.0f;
    C2DVector normal2D(normal.X, normal.Y);
    normal2D.AccurateNormalise();
    float blend = 1.0f - ((float)acos(normal2D.Dot(direction2D)) * 0.63661975f - 0.25f) * 2.0f;
    if (blend < 0.0f)
        blend = 0.0f;
    else if (blend > 1.0f)
        blend = 1.0f;
    if (vertical == 0.0f)
        return blend;
    if (blend == 0.0f)
        return 0.0f;
    return (1.0f - vertical) * blend;
}

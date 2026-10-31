// CEngineInternalPrimitiveRepeatedStaticMesh::IsInsideFrustum(const CEngineCamera& camera, bool ignoreFrustum) -- distance cull (the box-to-camera distance against RepeatedMeshDrawDistanceFactor * the max draw distance; SSE or x87 path), then the 4-plane n-vertex frustum test
// Leaf: IsInsideFrustum
// Grade: RELOCATION_MATCH (475 bytes; parked 461v475 'inline-asm SSE lane gap' disproved): the landed retail_asm SSE
//   kernel of C3DBoundingBox::CalcShortestDistanceToPointSquared (0x00BF7DF0 form); the x87 fallback's difference vector
//   is a frame C3DVector for X/Y with the Z difference a scalar consumed by its square (retail's fld st0 / fmulp and the
//   0x24 frame); the plane normals as named locals.
// Retail-asm: 0x00B99BE5..0x00B99C2F `movss xmm0,[ecx+8] ; movhps xmm0,[ecx] ; ... ; maxps xmm2,xmm0 ; minps xmm2,xmm1 ; subps xmm2,xmm3 ; mulps xmm2,xmm2 ; shufps xmm2,xmm2,0B0h ; addps xmm2,xmm3 ; shufps xmm2,xmm2,3 ; addss xmm2,xmm3 ; movss [esp+0Ch],xmm2` -- C3DBoundingBox::CalcShortestDistanceToPointSquared's hand SSE kernel behind g_SSEAvailable 0x013D2880 (the box and point pointers spilled to [esp+8] / the dead camera slot [esp+30h] and reloaded into ecx/edx because the asm names them), the same kernel as landed 0x00BF7DF0; the x87 branch is the C++ fallback.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B99BB0, 475 bytes, `ret 8`. Ego_r.pdb:
//   ?IsInsideFrustum@CEngineInternalPrimitiveRepeatedStaticMesh@@UAE_NABVCEngineCamera@@_N@Z (virtual); the virtual is a
//   __fastcall free function, as the landed siblings 0x00B9DC60 / 0x00BAA4E0.
// Globals: g_pEnginePrimitiveRendererSharedData 0x01436E80 (Ego_r CEnginePrimitiveRendererSharedData
//   RepeatedMeshDrawDistanceFactor +0x88; the landed 0x00BAA4E0 binding), g_SSEAvailable 0x013D2880 (landed 0x00BF7DF0), 0.0f @0x0122DEDC.
// Layout: engine/CEngineInternalPrimitiveRepeatedStaticMesh.h (BoundingBox +0x58; the base's CEngineFadeDistance
//   MaxDrawDistance word +0x18), engine/CEngineCamera.h (CameraDesc +0x14: WorldPosition at camera +0x54; FrustrumPlanes +0x1c0).
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)
#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"
#include "engine/CEngineCamera.h"
extern bool g_SSEAvailable;                             // 0x013D2880

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { return Z * Z + Y * Y + X * X; }
};
class C3DBoundingBox {
public:
    C3DVector MinCoord;                                 // +0x00
    C3DVector MaxCoord;                                 // +0x0c
    __forceinline float CalcShortestDistanceToPointSquared(const C3DVector& point) const
    {
        if (g_SSEAvailable) {
            float result;
            __asm {
                mov     ecx, this
                mov     edx, point
                movss   xmm0, [ecx+8]
                movhps  xmm0, [ecx]
                movss   xmm1, [ecx+14h]
                movhps  xmm1, [ecx+0Ch]
                movss   xmm2, [edx+8]
                movhps  xmm2, [edx]
                movaps  xmm3, xmm2
                maxps   xmm2, xmm0
                minps   xmm2, xmm1
                subps   xmm2, xmm3
                mulps   xmm2, xmm2
                movaps  xmm3, xmm2
                shufps  xmm2, xmm2, 0B0h
                addps   xmm2, xmm3
                shufps  xmm2, xmm2, 3
                addss   xmm2, xmm3
                movss   result, xmm2
            }
            return result;
        }
        C3DVector closest(point);
        if (closest.X < MinCoord.X)
            closest.X = MinCoord.X;
        else if (closest.X > MaxCoord.X)
            closest.X = MaxCoord.X;
        if (closest.Y < MinCoord.Y)
            closest.Y = MinCoord.Y;
        else if (closest.Y > MaxCoord.Y)
            closest.Y = MaxCoord.Y;
        if (closest.Z < MinCoord.Z)
            closest.Z = MinCoord.Z;
        else if (closest.Z > MaxCoord.Z)
            closest.Z = MaxCoord.Z;
        C3DVector d;
        d.X = point.X - closest.X;
        d.Y = point.Y - closest.Y;
        float dz = point.Z - closest.Z;
        return dz * dz + d.X * d.X + d.Y * d.Y;
    }
    __forceinline float CalcShortestDistanceToPoint(const C3DVector& point) const
    {
        return (float)sqrt(CalcShortestDistanceToPointSquared(point));
    }
};
class CPlane { public: C3DVector Normal; float D; };

struct CEnginePrimitiveRendererSharedData {        // Ego_r.pdb (only the draw-distance factors)
    unsigned char _pad00[0x7c];
    float DrawDistanceFactor;                      // +0x7c
    float DrawDistanceMinimum;                     // +0x80
    float DrawDistanceMaximum;                     // +0x84
    float RepeatedMeshDrawDistanceFactor;          // +0x88
};
extern CEnginePrimitiveRendererSharedData* g_pEnginePrimitiveRendererSharedData;   // 0x01436E80

static __forceinline const C3DBoundingBox& Box(const CEngineInternalPrimitiveRepeatedStaticMesh* self) { return *(const C3DBoundingBox*)self->BoundingBox; }
static __forceinline float MaxDrawDistance(const CEngineInternalPrimitiveRepeatedStaticMesh* self) { return *(const float*)self->_pad_0x18; }
static __forceinline const C3DVector& CameraPosition(const CEngineCamera& c) { return *(const C3DVector*)(c.CameraDesc + 0x40); }
static __forceinline const CPlane* Planes(const CEngineCamera& c) { return (const CPlane*)c.FrustrumPlanes; }

bool __fastcall IsInsideFrustum(CEngineInternalPrimitiveRepeatedStaticMesh* self, int /*edx*/, const CEngineCamera& camera, bool ignoreFrustum)
{
    float maxDistance = g_pEnginePrimitiveRendererSharedData->RepeatedMeshDrawDistanceFactor * MaxDrawDistance(self);

    if (Box(self).CalcShortestDistanceToPoint(CameraPosition(camera)) > maxDistance)
        return false;
    if (ignoreFrustum)
        return true;
    const CPlane* planes = Planes(camera);
    const C3DBoundingBox& box = Box(self);
    for (int i = 0; i < 4; ++i) {
        const CPlane& plane = planes[i];
        float nx = plane.Normal.X, ny = plane.Normal.Y, nz = plane.Normal.Z;
        float x = nx > 0.0f ? box.MinCoord.X : box.MaxCoord.X;
        float y = ny > 0.0f ? box.MinCoord.Y : box.MaxCoord.Y;
        float z = nz > 0.0f ? box.MinCoord.Z : box.MaxCoord.Z;
        if (x * nx + y * ny + z * nz > plane.D)
            return false;
    }
    return true;
}
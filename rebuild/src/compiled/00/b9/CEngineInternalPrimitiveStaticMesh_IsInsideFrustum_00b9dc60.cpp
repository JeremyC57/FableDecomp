// CEngineInternalPrimitiveStaticMesh::IsInsideFrustum(const CEngineCamera& camera, bool ignoreFrustum) -- the mesh's world-space bounding sphere centre (local centre through the object transform) must lie within the primitive's fade distance of the camera; unless ignoreFrustum, the sphere (radius scaled by +0x58) must also overlap the near / far depth range along the look vector and lie inside the four side frustum planes
// Leaf: IsInsideFrustum
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9DC60, `ret 8`. Ego_r.pdb: ?IsInsideFrustum@CEngineInternalPrimitiveStaticMesh@@UAE_NABVCEngineCamera@@_N@Z (virtual).
// Callees: 0x00BEB660 (the primitive's fade distance: FadeDistance +0x18, or clamped to the global camera's range when OverrideFadeDistance).
// Layout: engine/CEngineInternalPrimitiveStaticMesh.h -- MeshInfo (C3DMeshInfo*) is the header's `_pad_0x50` word, the radius scale
//   the float at +0x58 (`field58`), ObjectTransform the opaque bytes at +0x70; engine/C3DMeshInfo.h BoundingSphere +0x28;
//   engine/CEngineCamera.h -- CameraDesc (+0x14) holds the camera position at +0x54 and the near / far depths at +0x6c / +0x70,
//   LookVector +0x1b4, FrustrumPlanes CPlane[4] +0x1c0. `this` is the shared header; the virtual is a __fastcall free function.
// Shape: the world centre is three plain sums (VC7.1 canonicalises them to z,y,x as retail), the camera-relative dots are
//   written x+y+z (emitted z,y,x); the radius is the double-widened product of the landed GetBoundingSphereWorldSpace 0x00B9DBD0,
//   stored into the dead bool argument slot. The near / far tests must be ONE `||` condition: two separate early returns
//   duplicate the `fstp st(0)` false exit inline instead of sharing retail's tail block.
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/C3DMeshInfo.h"
#include "engine/CEngineCamera.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float LengthSquared() const { return Z * Z + Y * Y + X * X; }
};
struct CPlane { C3DVector Normal; float D; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };

float __fastcall CEnginePrimitive_ComputeFadeDistance(const CEngineInternalPrimitiveStaticMesh* self);   // 0x00BEB660

static __forceinline const C3DBoundingSphere& LocalSphere(const CEngineInternalPrimitiveStaticMesh* self)
{
    const C3DMeshInfo* info = *(C3DMeshInfo* const*)self->_pad_0x50;
    return *(const C3DBoundingSphere*)info->BoundingSphere;
}
static __forceinline const float* Transform(const CEngineInternalPrimitiveStaticMesh* self)
{
    return (const float*)self->ObjectTransform;
}
static __forceinline const C3DVector& CameraPosition(const CEngineCamera& camera) { return *(const C3DVector*)(camera.CameraDesc + 0x40); }
static __forceinline float CameraNear(const CEngineCamera& camera) { return *(const float*)(camera.CameraDesc + 0x58); }
static __forceinline float CameraFar(const CEngineCamera& camera) { return *(const float*)(camera.CameraDesc + 0x5c); }
static __forceinline const CPlane* Planes(const CEngineCamera& camera) { return (const CPlane*)camera.FrustrumPlanes; }

static __forceinline bool SphereInFrustum(const CEngineCamera& camera, const C3DVector& centre, float radius)
{
    C3DVector d = centre - CameraPosition(camera);
    float depth = d.X * camera.LookVector_X + d.Y * camera.LookVector_Y + d.Z * camera.LookVector_Z;
    if (depth + radius < CameraNear(camera) || depth - radius > CameraFar(camera))
        return false;
    const CPlane* planes = Planes(camera);
    if (d.X * planes[2].Normal.X + d.Y * planes[2].Normal.Y + d.Z * planes[2].Normal.Z > radius)
        return false;
    if (d.X * planes[3].Normal.X + d.Y * planes[3].Normal.Y + d.Z * planes[3].Normal.Z > radius)
        return false;
    if (d.X * planes[0].Normal.X + d.Y * planes[0].Normal.Y + d.Z * planes[0].Normal.Z > radius)
        return false;
    if (d.X * planes[1].Normal.X + d.Y * planes[1].Normal.Y + d.Z * planes[1].Normal.Z > radius)
        return false;
    return true;
}

bool __fastcall IsInsideFrustum(CEngineInternalPrimitiveStaticMesh* self, int /*edx*/, const CEngineCamera& camera, bool ignoreFrustum)
{
    const C3DVector& c = LocalSphere(self).Center;
    float x = c.X, y = c.Y, z = c.Z;
    const float* m = Transform(self);
    C3DVector centre(x * m[0] + y * m[3] + z * m[6] + m[9],
                     x * m[1] + y * m[4] + z * m[7] + m[10],
                     x * m[2] + y * m[5] + z * m[8] + m[11]);
    C3DVector delta = centre - CameraPosition(camera);
    float fade = CEnginePrimitive_ComputeFadeDistance(self);
    if (delta.LengthSquared() > fade * fade)
        return false;
    if (ignoreFrustum)
        return true;
    double radius = LocalSphere(self).Radius;
    return SphereInFrustum(camera, centre, (float)(radius * *(const float*)&self->field58));
}
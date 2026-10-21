// CEnginePrimitiveAttachmentManager::IsVisible(const C3DBoundingSphere& sphere, float maxDistance, const vector<CEngineCamera*>& cameras) -- true when any camera is closer than maxDistance - radius to the sphere and sees it inside its near/far planes and four frustum planes
// Leaf: IsVisible
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B38ED0, 420 bytes (+int3 padding), `ret 0Ch`; `this` (ecx) is never read. Called from
//   0x00B3A068 (the attachment update, which passes a sphere local, the distance and the camera vector).
// Callees: none (fsqrt inline through the `sqrt` intrinsic).
// Layout: the camera is engine/CEngineCamera.h (CameraDesc +0x14 = CEngineCameraDesc: WorldPosition +0x54, NearZPlane +0x6c,
//   FarZPlane +0x70; LookVector +0x1b4; FrustrumPlanes +0x1c0, four 16-byte CPlane {Normal, D}); the vector is STLport's
//   12-byte vector<CEngineCamera*>; `this` is engine/CEnginePrimitiveAttachmentManager.h, as a __fastcall free function.
// Shape: the loop-invariant `maxDistance - radius` is hoisted into the dead `sphere` argument slot; the distance is
//   (camera - centre), the frustum test the same inline camera helper as CEngineInternalPrimitiveRepeatedZSprite::
//   IsInsideFrustum 0x00BAA4E0 (radius by value into the dead `cameras` slot, one `||` near/far condition, planes 2, 3, 0, 1).
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/CEnginePrimitiveAttachmentManager.h"
#include "engine/CEngineCamera.h"
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
struct CPlane { C3DVector Normal; float D; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };
struct CCameraVector {                             // STLport vector<CEngineCamera*> (12 bytes)
    CEngineCamera** _M_start;
    CEngineCamera** _M_finish;
    CEngineCamera** _M_end_of_storage;
    typedef CEngineCamera* const* const_iterator;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
struct CEngineCameraDesc {                         // Ego_r.pdb, 0x6c bytes (CEngineCamera::CameraDesc +0x14)
    float     Window[4];                           // +0x00 C2DBoxF
    float     RotationMatrix[12];                  // +0x10 CMatrix3x4
    C3DVector WorldPosition;                       // +0x40 (camera +0x54)
    float     HorizontalFOV;                       // +0x4c
    float     VerticalFOV;                         // +0x50
    bool      Use2DFOV;                            // +0x54
    float     NearZPlane;                          // +0x58 (camera +0x6c)
    float     FarZPlane;                           // +0x5c (camera +0x70)
    float     ZBufferStart;                        // +0x60
    float     ZBufferEnd;                          // +0x64
    bool      StateValid;                          // +0x68
};
static __forceinline const CEngineCameraDesc& Desc(const CEngineCamera& c) { return *(const CEngineCameraDesc*)c.CameraDesc; }
static __forceinline const C3DVector& CameraPosition(const CEngineCamera& c) { return Desc(c).WorldPosition; }
static __forceinline float NearZ(const CEngineCamera& c) { return Desc(c).NearZPlane; }
static __forceinline float FarZ(const CEngineCamera& c) { return Desc(c).FarZPlane; }
static __forceinline const C3DVector& LookVector(const CEngineCamera& c) { return *(const C3DVector*)&c.LookVector_X; }
static __forceinline const CPlane& FrustrumPlane(const CEngineCamera& c, int i) { return ((const CPlane*)c.FrustrumPlanes)[i]; }

static __forceinline bool IsSphereInFrustum(const CEngineCamera& camera, const C3DVector& centre, float radius)
{
    C3DVector d = centre - CameraPosition(camera);
    float z = d.Dot(LookVector(camera));
    if (z + radius < NearZ(camera) || z - radius > FarZ(camera)) return false;
    if (d.Dot(FrustrumPlane(camera, 2).Normal) > radius) return false;
    if (d.Dot(FrustrumPlane(camera, 3).Normal) > radius) return false;
    if (d.Dot(FrustrumPlane(camera, 0).Normal) > radius) return false;
    if (d.Dot(FrustrumPlane(camera, 1).Normal) > radius) return false;
    return true;
}

bool __fastcall IsVisible(CEnginePrimitiveAttachmentManager* self, int, const C3DBoundingSphere& sphere, float maxDistance, const CCameraVector& cameras)
{
    for (CCameraVector::const_iterator it = cameras.begin(); it != cameras.end(); ++it) {
        const CEngineCamera& camera = **it;
        if ((CameraPosition(camera) - sphere.Center).GetLength() < maxDistance - sphere.Radius) {
            if (IsSphereInFrustum(camera, sphere.Center, sphere.Radius))
                return true;
        }
    }
    return false;
}
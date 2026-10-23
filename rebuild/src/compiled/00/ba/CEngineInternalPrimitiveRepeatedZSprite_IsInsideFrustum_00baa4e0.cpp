// CEngineInternalPrimitiveRepeatedZSprite::IsInsideFrustum(const CEngineCamera& camera, bool ignoreFrustum) -- distance-cull the sprite group's bounding sphere against the scaled fade-out distance and the fade-in start, then (unless told to skip it) test the sphere against the camera's near/far planes and four frustum planes
// Leaf: IsInsideFrustum
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BAA4E0, 444 bytes (+2 int3), `ret 8`; vtable 0x012A524C slot 20. Ego_r.pdb:
//   ?IsInsideFrustum@CEngineInternalPrimitiveRepeatedZSprite@@UAE_NABVCEngineCamera@@_N@Z.
// Callees: the fade-in range getter 0x00BAA460 (`ret 4`: an 8-byte {start, end} returned through a hidden pointer, built from
//   FadeInStart/FadeInEnd and clamped against the engine's z range; manifest "engine_get_clamped_z_coordinate").
//   Global: the engine render object 0x01436E80 (float +0x8c = the fade-distance scale).
// Layout: `this` is engine/CEngineInternalPrimitiveRepeatedZSprite.h (BoundingSphere +0x64, radius +0x70; the base's
//   CEngineFadeDistance FadeDistance +0x14 is flattened, its far word +0x18 read from `_pad_0x18`); the camera is
//   engine/CEngineCamera.h (CameraDesc +0x14 = CEngineCameraDesc: WorldPosition +0x54, NearZPlane +0x6c, FarZPlane +0x70;
//   LookVector +0x1b4; FrustrumPlanes +0x1c0, four 16-byte CPlane {Normal, D}). The virtual is a __fastcall free function.
// Shape: the sphere test is an inline camera helper taking the radius BY VALUE (copied with integer moves into the dead
//   `ignoreFrustum` slot) and the centre difference as a frame C3DVector; the near/far test is ONE `||` condition (two
//   separate ifs put the near-plane failure inline instead of retail's shared `fstp st(0)` exit); the planes are tested in
//   retail's order 2, 3, 0, 1; the fade range needs a user constructor so it is returned through the hidden pointer.
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/CEngineInternalPrimitiveRepeatedZSprite.h"
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
struct CFadeInRange {
    float Start, End;
    CFadeInRange(float s, float e) : Start(s), End(e) {}
};
struct CEngineGlobals {
    unsigned char _pad[0x8c];
    float FadeDistanceScale;                       // +0x8c
};
extern CEngineGlobals* g_pEngineGlobals_01436e80;           // 0x01436E80

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

struct CZSprite : CEngineInternalPrimitiveRepeatedZSprite {
    CFadeInRange GetFadeInRange() const;           // 0x00BAA460
};

static __forceinline const C3DBoundingSphere& Sphere(const CEngineInternalPrimitiveRepeatedZSprite* self) { return *(const C3DBoundingSphere*)self->BoundingSphere; }
static __forceinline float FadeFar(const CEngineInternalPrimitiveRepeatedZSprite* self) { return *(const float*)&self->_pad_0x18; }

bool __fastcall IsInsideFrustum(CEngineInternalPrimitiveRepeatedZSprite* self, int, const CEngineCamera& camera, bool ignoreFrustum)
{
    float dist = (Sphere(self).Center - CameraPosition(camera)).GetLength();
    if (dist - Sphere(self).Radius > g_pEngineGlobals_01436e80->FadeDistanceScale * FadeFar(self)) return false;
    if (dist + Sphere(self).Radius < static_cast<const CZSprite*>(self)->GetFadeInRange().Start) return false;
    if (ignoreFrustum) return true;
    return IsSphereInFrustum(camera, Sphere(self).Center, Sphere(self).Radius);
}
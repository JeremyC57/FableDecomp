// CEnginePrimitiveAttachmentManager::IsVisible(const C3DBoundingSphere& sphere, float maxDistance, const CArray<CEngineCamera*>& cameras) const -- true when any camera is closer than maxDistance - radius to the sphere and sees it inside its near/far planes and four frustum planes
// Leaf: IsVisible
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLength respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B38ED0, 420 bytes (+int3 padding), `ret 0Ch`; `this` (ecx) is never read. Called from
//   0x00B3A068 (the attachment update, which passes a sphere local, the distance and the camera vector).
// Callees: none (fsqrt inline through the `sqrt` intrinsic).
// Layout: the camera is engine/CEngineCamera.h (CameraDesc +0x14 = CEngineCameraDesc: WorldPosition +0x54, NearZPlane +0x6c,
//   FarZPlane +0x70; LookVector +0x1b4; FrustrumPlanes +0x1c0, four 16-byte CPlane {Normal, D}); the vector is STLport's
//   12-byte vector<CEngineCamera*> (CArray<CEngineCamera*>); `this` is engine/CEnginePrimitiveAttachmentManager.h (the PDB private
//   const member, ?IsVisible@CEnginePrimitiveAttachmentManager@@ABE_NABVC3DBoundingSphere@@MABV?$CArray@PAVCEngineCamera@@@@@Z).
// Shape: the loop-invariant `maxDistance - radius` is hoisted into the dead `sphere` argument slot; the distance is
//   (camera - centre), the frustum test the same inline camera helper as CEngineInternalPrimitiveRepeatedZSprite::
//   IsInsideFrustum 0x00BAA4E0 (radius by value into the dead `cameras` slot, one `||` near/far condition, planes 2, 3, 0, 1).
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(sqrt)
#define CEnginePrimitiveAttachmentManager CEnginePrimitiveAttachmentManagerLayout
#include "engine/CEnginePrimitiveAttachmentManager.h"
#undef CEnginePrimitiveAttachmentManager
#include "engine/CEngineCamera.h"
#include "engine/C3DVector.h"
class CPlane { public: C3DVector Normal; float D; };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
#include "engine/CArray.h"
class CEnginePrimitiveAttachmentManager : public CEnginePrimitiveAttachmentManagerLayout {   // header layout (vptr word flattened)
    bool IsVisible(const C3DBoundingSphere& sphere, float maxDistance, const CArray<CEngineCamera*>& cameras) const;   // Ego_r.pdb: private const
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
static FABLE_FORCEINLINE const CEngineCameraDesc& Desc(const CEngineCamera& c) { return *(const CEngineCameraDesc*)c.CameraDesc; }
static FABLE_FORCEINLINE const C3DVector& CameraPosition(const CEngineCamera& c) { return Desc(c).WorldPosition; }
static FABLE_FORCEINLINE float NearZ(const CEngineCamera& c) { return Desc(c).NearZPlane; }
static FABLE_FORCEINLINE float FarZ(const CEngineCamera& c) { return Desc(c).FarZPlane; }
static FABLE_FORCEINLINE const C3DVector& LookVector(const CEngineCamera& c) { return *(const C3DVector*)&c.LookVector_X; }
static FABLE_FORCEINLINE const CPlane& FrustrumPlane(const CEngineCamera& c, int i) { return ((const CPlane*)c.FrustrumPlanes)[i]; }

static FABLE_FORCEINLINE bool IsSphereInFrustum(const CEngineCamera& camera, const C3DVector& centre, float radius)
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

bool CEnginePrimitiveAttachmentManager::IsVisible(const C3DBoundingSphere& sphere, float maxDistance, const CArray<CEngineCamera*>& cameras) const
{
    for (CArray<CEngineCamera*>::const_iterator it = cameras.begin(); it != cameras.end(); ++it) {
        const CEngineCamera& camera = **it;
        if ((CameraPosition(camera) - sphere.Center).GetMagnitude() < maxDistance - sphere.Radius) {
            if (IsSphereInFrustum(camera, sphere.Center, sphere.Radius))
                return true;
        }
    }
    return false;
}
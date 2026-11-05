// CEngineShadowRenderer::SetShadowClipVolumesToNeverShadow() -- empty both shadow clip volumes (no clip planes, no sphere test), then give each a bounding sphere of radius 0 at (-1e30, -1e30, -1e30) with the sphere test on, so nothing falls inside either volume
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4CE20, 173 bytes (+ int3 padding).
// Layout (Ego_r.pdb, retail agrees): SceneData (CShadowRendererSceneData) at +0xc; its CasterClipVolume at +0x11c and
//   RecieverClipVolume at +0x1b4 (CShadowClipVolume, 0x98 bytes: ClipPlanes[8] +0, ClipPlaneCount +0x80, BoundingSphere +0x84,
//   ClipToSphere +0x94). engine/CEngineShadowRenderer.h keeps SceneData as opaque bytes, so the scene data is modelled locally
//   with the PDB member names at the header's offset.
// Shape: two inline Clear() calls (count 0, flag false; the zero register shared), then two inline SetBoundingSphere() calls
//   taking a C3DBoundingSphere temporary built from a C3DVector temporary (memberwise user copy ctors: each word is stored
//   into the 16-byte frame temporary and read back into the member; -1e30f is the immediate 0xF149F2CA).
#include "engine/C3DVector.h"
class C3DBoundingSphere { public:
    C3DVector Center;                                  // +0x00
    float     Radius;                                  // +0x0c
    C3DBoundingSphere(const C3DVector& centre, float radius) : Center(centre), Radius(radius) { }
};
class CPlane { public: float A, B, C, D; };

class CShadowClipVolume { public:                             // 0x98 bytes
    CPlane            ClipPlanes[8];                   // +0x00
    long              ClipPlaneCount;                  // +0x80
    C3DBoundingSphere BoundingSphere;                  // +0x84
    bool              ClipToSphere;                    // +0x94
    void Clear()
    {
        ClipPlaneCount = 0;
        ClipToSphere = false;
    }
    void SetBoundingSphere(const C3DBoundingSphere& sphere)
    {
        BoundingSphere = sphere;
        ClipToSphere = true;
    }
};

class CShadowRendererSceneData { public:                      // 0x240 bytes
    unsigned char     _pad000[0x110];                  // matrices, fades, light vector, camera position
    CShadowClipVolume CasterClipVolume;                // +0x110
    CShadowClipVolume RecieverClipVolume;              // +0x1a8
};

class CEngineShadowRenderer { public:
    unsigned char            _pad00[0x0c];
    CShadowRendererSceneData SceneData;                // +0x0c
    void SetShadowClipVolumesToNeverShadow();
};

void CEngineShadowRenderer::SetShadowClipVolumesToNeverShadow()
{
    SceneData.CasterClipVolume.Clear();
    SceneData.RecieverClipVolume.Clear();
    SceneData.CasterClipVolume.SetBoundingSphere(C3DBoundingSphere(C3DVector(-1.0e30f, -1.0e30f, -1.0e30f), 0.0f));
    SceneData.RecieverClipVolume.SetBoundingSphere(C3DBoundingSphere(C3DVector(-1.0e30f, -1.0e30f, -1.0e30f), 0.0f));
}
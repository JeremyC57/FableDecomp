// CWaterPatchMesh::Render(long triangleCount) const -- put the camera-centred world transform, bind the patch vertex buffer as stream 0, flush the dirty shader state and draw the shared water index list
// Leaf: Render
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEACA0, 427 bytes + int3 padding, `ret 4`.
// Signature from Ego_r.pdb: ?Render@CWaterPatchMesh@@QBEXJ@Z.
// Callees: CShaderRenderManager::SetWorldTransform(const CMatrix3x4&) @ 0x009881F0, IDirect3DDevice9::SetStreamSource
// (COM slot 100, +0x190), the inlined CShaderRenderManager::PrepareForRender(5) (SetVSConstants 0x0098B930,
// UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760, UpdateFogColour
// 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateCombinedProjectionTransform 0x00988A50, UpdateCameraPos 0x00989700),
// CRenderManagerCore::DrawCurrentBuffersIndexed(unsigned long x4) @ 0x00A0AD40.
// Globals: NEngineInternal::GPShaderRenderManager 0x01436E14, g_pRenderManagerCore 0x01436E18, NEngineInternal::GPCamera 0x01436EA0.
// Layout: engine/CWaterPatchMesh.h for `this` (VertexBuffer +0x10). CVertexBufferBase as in the landed
// C3DPrimitive2::SetupVertexBuffer (PVertexBuffer +4, Stride unsigned char +0x10). CRenderManagerCore keeps its
// device at +0x3cf0; the camera world position is at +0x54 (landed decal RenderPrimitive model). The
// CShaderRenderManager view is the landed RenderRainBox model (retail layout, UpdateFlags +0x3d8).
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore at offset 0): the stream bind is
//   its PDB inline SelectVertexBuffer(0, buffer) (PD3DDevice +0x3CF0, SetStreamSource +0x190) on the shared CVertexBufferWin32, and the
//   draw ?DrawCurrentBuffersIndexed@CRenderManagerCore@@QAEXKKKK@Z 0x00A0AD40. Was g_pRenderManagerCore with local CRenderManagerCore /
//   IDirect3DDevice9 / CVertexBufferWin32 models.
#include "engine/CRenderManager.h"
#include "engine/CWaterPatchMesh.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

#include "engine/C3DVector.h"
class C4DVector { public: float X, Y, Z, W; };

class CMatrix3x4 { public:
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};

class CShaderConstantRange { public:
    void* _vptr;
    long Offset;
    unsigned long Count;
};

class CVertexShaderConstantLayout { public:
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10
    CArray<C4DVector> ReservedPresets;                  // +0x140
};

class CShaderRenderManager {
public:
    void SetWorldTransform(const CMatrix3x4& transform);   // 0x009881F0

    FABLE_FORCEINLINE void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetData(),
                           CurrentVSConstantLayout->ReservedPresets.Size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

    void* _vptr;                                        // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char _pad008[0x1e0];
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x1ef];
    unsigned long UpdateFlags;                          // +0x3d8

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);
    void UpdatePixelShader();
    void UpdatePixelShaderConstants();
    void UpdateBoneTransform();
    void UpdateWorldSpaceTextureTransform();
    void UpdateObjectSpaceTextureTransform();
    void UpdateWorldSpaceFogTransform();
    void UpdateObjectSpaceFogTransform();
    void UpdateAmbient();
    void UpdateFogColour();
    void UpdateLightGlobals();
    void UpdateObjectSpaceLighting(unsigned long flags);
    void UpdateWorldSpaceLighting();
    void UpdateLocalisedSphereMapping();
    void ResetWorldTransform();
    void UpdateCombinedProjectionTransform();
    void UpdateCameraPos();
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14

class CEngineCamera { public:
    unsigned char _pad0[0x54];
    C3DVector WorldPosition;                            // +0x54
};
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                  // 0x01436EA0

struct CWaterPatchMesh_Methods : CWaterPatchMesh {
    void Render(long triangleCount) const;
};

void CWaterPatchMesh_Methods::Render(long triangleCount) const
{
    if (!VertexBuffer)
        return;
    CMatrix3x4 transform;
    const CEngineCamera* camera = NEngineInternal::GPCamera;
    transform.Position.X = camera->WorldPosition.X;
    transform.Position.Y = camera->WorldPosition.Y;
    transform.Position.Z = camera->WorldPosition.Z;
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, VertexBuffer);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(5);
    NEngineInternal::GPRenderManager->DrawCurrentBuffersIndexed(0, 0x121, 0, triangleCount);
}
// CWaterPatchMesh::Render(long triangleCount) const -- put the camera-centred world transform, bind the patch vertex buffer as stream 0, flush the dirty shader state and draw the shared water index list
// Leaf: Render
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEACA0, 427 bytes + int3 padding, `ret 4`.
// Signature from Ego_r.pdb: ?Render@CWaterPatchMesh@@QBEXJ@Z.
// Callees: CShaderRenderManager::SetWorldTransform(const CMatrix3x4&) @ 0x009881F0, IDirect3DDevice9::SetStreamSource
// (COM slot 100, +0x190), the inlined CShaderRenderManager::PrepareForRender(5) (SetVSConstants 0x0098B930,
// UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760, UpdateFogColour
// 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateCombinedProjectionTransform 0x00988A50, UpdateCameraPos 0x00989700),
// CRenderManagerCore::DrawCurrentBuffersIndexed(unsigned long x4) @ 0x00A0AD40.
// Globals: g_pShaderRenderManager 0x01436E14, g_pRenderManagerCore 0x01436E18, g_pEngineCamera 0x01436EA0.
// Layout: engine/CWaterPatchMesh.h for `this` (VertexBuffer +0x10). CVertexBufferBase as in the landed
// C3DPrimitive2::SetupVertexBuffer (PVertexBuffer +4, Stride unsigned char +0x10). CRenderManagerCore keeps its
// device at +0x3cf0; the camera world position is at +0x54 (landed decal RenderPrimitive model). The
// CShaderRenderManager view is the landed RenderRainBox model (retail layout, UpdateFlags +0x3d8).
#include "engine/CWaterPatchMesh.h"

struct C3DVector { float X, Y, Z; };
struct C4DVector { float X, Y, Z, W; };

struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};

template <class T> struct CArray {
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};

struct CShaderConstantRange {
    void* _vptr;
    long Offset;
    unsigned long Count;
};

struct CVertexShaderConstantLayout {
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10
    CArray<C4DVector> ReservedPresets;                  // +0x140
};

class CShaderRenderManager {
public:
    void SetWorldTransform(const CMatrix3x4& transform);   // 0x009881F0

    __forceinline void PrepareForRender(unsigned long flags)
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
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14

struct IDirect3DVertexBuffer9;
struct IDirect3DDevice9 {
    virtual void __stdcall Slot0() = 0;
    virtual void __stdcall Slot1() = 0;
    virtual void __stdcall Slot2() = 0;
    virtual void __stdcall Slot3() = 0;
    virtual void __stdcall Slot4() = 0;
    virtual void __stdcall Slot5() = 0;
    virtual void __stdcall Slot6() = 0;
    virtual void __stdcall Slot7() = 0;
    virtual void __stdcall Slot8() = 0;
    virtual void __stdcall Slot9() = 0;
    virtual void __stdcall Slot10() = 0;
    virtual void __stdcall Slot11() = 0;
    virtual void __stdcall Slot12() = 0;
    virtual void __stdcall Slot13() = 0;
    virtual void __stdcall Slot14() = 0;
    virtual void __stdcall Slot15() = 0;
    virtual void __stdcall Slot16() = 0;
    virtual void __stdcall Slot17() = 0;
    virtual void __stdcall Slot18() = 0;
    virtual void __stdcall Slot19() = 0;
    virtual void __stdcall Slot20() = 0;
    virtual void __stdcall Slot21() = 0;
    virtual void __stdcall Slot22() = 0;
    virtual void __stdcall Slot23() = 0;
    virtual void __stdcall Slot24() = 0;
    virtual void __stdcall Slot25() = 0;
    virtual void __stdcall Slot26() = 0;
    virtual void __stdcall Slot27() = 0;
    virtual void __stdcall Slot28() = 0;
    virtual void __stdcall Slot29() = 0;
    virtual void __stdcall Slot30() = 0;
    virtual void __stdcall Slot31() = 0;
    virtual void __stdcall Slot32() = 0;
    virtual void __stdcall Slot33() = 0;
    virtual void __stdcall Slot34() = 0;
    virtual void __stdcall Slot35() = 0;
    virtual void __stdcall Slot36() = 0;
    virtual void __stdcall Slot37() = 0;
    virtual void __stdcall Slot38() = 0;
    virtual void __stdcall Slot39() = 0;
    virtual void __stdcall Slot40() = 0;
    virtual void __stdcall Slot41() = 0;
    virtual void __stdcall Slot42() = 0;
    virtual void __stdcall Slot43() = 0;
    virtual void __stdcall Slot44() = 0;
    virtual void __stdcall Slot45() = 0;
    virtual void __stdcall Slot46() = 0;
    virtual void __stdcall Slot47() = 0;
    virtual void __stdcall Slot48() = 0;
    virtual void __stdcall Slot49() = 0;
    virtual void __stdcall Slot50() = 0;
    virtual void __stdcall Slot51() = 0;
    virtual void __stdcall Slot52() = 0;
    virtual void __stdcall Slot53() = 0;
    virtual void __stdcall Slot54() = 0;
    virtual void __stdcall Slot55() = 0;
    virtual void __stdcall Slot56() = 0;
    virtual void __stdcall Slot57() = 0;
    virtual void __stdcall Slot58() = 0;
    virtual void __stdcall Slot59() = 0;
    virtual void __stdcall Slot60() = 0;
    virtual void __stdcall Slot61() = 0;
    virtual void __stdcall Slot62() = 0;
    virtual void __stdcall Slot63() = 0;
    virtual void __stdcall Slot64() = 0;
    virtual void __stdcall Slot65() = 0;
    virtual void __stdcall Slot66() = 0;
    virtual void __stdcall Slot67() = 0;
    virtual void __stdcall Slot68() = 0;
    virtual void __stdcall Slot69() = 0;
    virtual void __stdcall Slot70() = 0;
    virtual void __stdcall Slot71() = 0;
    virtual void __stdcall Slot72() = 0;
    virtual void __stdcall Slot73() = 0;
    virtual void __stdcall Slot74() = 0;
    virtual void __stdcall Slot75() = 0;
    virtual void __stdcall Slot76() = 0;
    virtual void __stdcall Slot77() = 0;
    virtual void __stdcall Slot78() = 0;
    virtual void __stdcall Slot79() = 0;
    virtual void __stdcall Slot80() = 0;
    virtual void __stdcall Slot81() = 0;
    virtual void __stdcall Slot82() = 0;
    virtual void __stdcall Slot83() = 0;
    virtual void __stdcall Slot84() = 0;
    virtual void __stdcall Slot85() = 0;
    virtual void __stdcall Slot86() = 0;
    virtual void __stdcall Slot87() = 0;
    virtual void __stdcall Slot88() = 0;
    virtual void __stdcall Slot89() = 0;
    virtual void __stdcall Slot90() = 0;
    virtual void __stdcall Slot91() = 0;
    virtual void __stdcall Slot92() = 0;
    virtual void __stdcall Slot93() = 0;
    virtual void __stdcall Slot94() = 0;
    virtual void __stdcall Slot95() = 0;
    virtual void __stdcall Slot96() = 0;
    virtual void __stdcall Slot97() = 0;
    virtual void __stdcall Slot98() = 0;
    virtual void __stdcall Slot99() = 0;
    virtual long __stdcall SetStreamSource(unsigned int stream, IDirect3DVertexBuffer9* data, unsigned int offset,
                                           unsigned int stride) = 0;    // slot 100
};

class CRenderManagerCore {
public:
    void DrawCurrentBuffersIndexed(unsigned long minVertexIndex, unsigned long vertexCount, unsigned long startIndex,
                                   unsigned long primitiveCount);   // 0x00A0AD40
    unsigned char _pad0[0x3cf0];
    IDirect3DDevice9* PD3DDevice;                       // +0x3cf0
};
extern CRenderManagerCore* g_pRenderManagerCore;        // 0x01436E18

struct CEngineCamera {
    unsigned char _pad0[0x54];
    C3DVector WorldPosition;                            // +0x54
};
extern CEngineCamera* g_pEngineCamera;                  // 0x01436EA0

struct CVertexBufferWin32 {
    void* _vptr;                                        // +0x00
    IDirect3DVertexBuffer9* PVertexBuffer;              // +0x04
    unsigned char _pad8[8];
    unsigned char Stride;                               // +0x10
};

struct CWaterPatchMesh_Methods : CWaterPatchMesh {
    void Render(long triangleCount) const;
};

void CWaterPatchMesh_Methods::Render(long triangleCount) const
{
    if (!VertexBuffer)
        return;
    CMatrix3x4 transform;
    const CEngineCamera* camera = g_pEngineCamera;
    transform.Position.X = camera->WorldPosition.X;
    transform.Position.Y = camera->WorldPosition.Y;
    transform.Position.Z = camera->WorldPosition.Z;
    g_pShaderRenderManager->SetWorldTransform(transform);
    CVertexBufferWin32* vertexBuffer = VertexBuffer;
    if (vertexBuffer)
        g_pRenderManagerCore->PD3DDevice->SetStreamSource(0, vertexBuffer->PVertexBuffer, 0, vertexBuffer->Stride);
    else
        g_pRenderManagerCore->PD3DDevice->SetStreamSource(0, 0, 0, 0);
    g_pShaderRenderManager->PrepareForRender(5);
    g_pRenderManagerCore->DrawCurrentBuffersIndexed(0, 0x121, 0, triangleCount);
}

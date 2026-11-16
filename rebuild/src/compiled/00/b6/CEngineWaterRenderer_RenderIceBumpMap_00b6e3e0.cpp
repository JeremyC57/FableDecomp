// CEngineWaterRenderer::RenderIceBumpMap() -- apply the ice bump-map shaders and draw the 14-vertex bump quad twice, first with the X/Y and then the Y/Z channel-select pixel constants, setting the blend states between the passes and clearing the first one after
// Leaf: RenderIceBumpMap
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6E3E0, length 1195 bytes (+ int3 padding), plain `ret`.
// Signature from Ego_r.pdb: ?RenderIceBumpMap@CEngineWaterRenderer@@AAEXXZ.
// Callees: CShaderRenderManager::ApplyVertexShader @ 0x00988020, ApplyPixelShader @ 0x00988140, the inlined
// SetPixelShaderConstant (PSConstTable +0x3cc, UpdateFlags |= 2; the landed RenderBackgroundPatch 0x00B6E010 model) and
// PrepareForRender(0) (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0,
// UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, ResetWorldTransform 0x00988290,
// UpdateCombinedProjectionTransform 0x00988A50), CRenderManagerCore::DrawCurrentBuffersAsIndexedTris @ 0x00A0AC60, and
// four inlined render-core SetState calls (+0x28b8 <- 1, +0x291c/+0x2930 <- [0x01396F6C], +0x28b8 <- 0).
// Layout: retail CEngineWaterRenderer: the three PDB CArray<CVertexShader> members at +0x12c..+0x15c are 12-byte STLport
// vectors in retail (16 in the PDB/engine header), so IceBumpmapVertexShader is at +0x188 (PDB +0x194) and
// IceBumpmapPixelShader at +0x190 (PDB +0x19c); a local view is used, as in the landed CEngineWaterRenderer rows.

#include "fable_compiler.h"
#include "engine/C3DVector.h"
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
class CMatrix3x4 { public: C3DVector XAxis, YAxis, ZAxis, Position; };

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

class CVertexShader;
class CPixelShader;

class CShaderRenderManager {
public:
    void ApplyVertexShader(const CVertexShader* shader, long flags);   // 0x00988020
    void ApplyPixelShader(const CPixelShader* shader);                 // 0x00988140
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00

    void SetPixelShaderConstant(unsigned long index, const C4DVector& value)
    {
        *(PSConstTable.First + index) = value;
        UpdateFlags |= 2;
    }

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
    unsigned char _pad1e9[0x1e3];
    CArray<C4DVector> PSConstTable;                     // +0x3cc
    unsigned long UpdateFlags;                          // +0x3d8

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                           // 0x00988A20
    void UpdatePixelShaderConstants();                  // 0x00989BF0
    void UpdateBoneTransform();                         // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();            // 0x009894B0
    void UpdateObjectSpaceTextureTransform();           // 0x00989230
    void UpdateWorldSpaceFogTransform();                // 0x00989640
    void UpdateObjectSpaceFogTransform();               // 0x00989540
    void UpdateAmbient();                               // 0x00989760
    void UpdateFogColour();                             // 0x009897C0
    void UpdateLightGlobals();                          // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();                    // 0x0098A540
    void UpdateLocalisedSphereMapping();                // 0x00988E80
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
public:
    void ResetWorldTransform();   // 0x00988290
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore / CRenderStateManager at
//   offset 0): DrawCurrentBuffersAsIndexedTris 0x00A0AC60 and the shared inline setters AlphaBlendEnable (+0x28B8) and Source /
//   DestAlphaBlendMode (+0x291C / +0x2930, D3DAlphaBlendModeTable 0x01396F68 [ALPHA_BLEND_ONE]). Was g_pRenderManagerCore with a
//   local CRenderManagerCore / CTrackedRenderState / SetState model and g_renderStateValue_01396f6c.
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

class CEngineWaterRenderer { public:                      // retail layout (three 12-byte CArrays before +0x15c)
    unsigned char _pad000[0x178];
    unsigned char BumpmapVertexShader[8];          // +0x178 (PDB +0x184)
    unsigned char BumpmapPixelShader[8];           // +0x180 (PDB +0x18c)
    unsigned char IceBumpmapVertexShader[8];       // +0x188 (PDB +0x194)
    unsigned char IceBumpmapPixelShader[8];        // +0x190 (PDB +0x19c)
    void RenderIceBumpMap();
};

void CEngineWaterRenderer::RenderIceBumpMap()
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(reinterpret_cast<const CVertexShader*>(IceBumpmapVertexShader), 0);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(reinterpret_cast<const CPixelShader*>(IceBumpmapPixelShader));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(1.0f, 0.0f, 0.0f, 0.0f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(0.0f, 1.0f, 0.0f, 0.0f));
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, 0xe, 0, 2);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(0.0f, 1.0f, 0.0f, 0.0f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(0.0f, 0.0f, 1.0f, 0.0f));
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, 0xe, 0, 2);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
}
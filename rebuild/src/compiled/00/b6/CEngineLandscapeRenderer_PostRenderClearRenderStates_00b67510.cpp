// CEngineLandscapeRenderer::PostRenderClearRenderStates() -- after the landscape pass: unbind the index buffer, stream 0 and texture stages 0-2 from the D3D device (through the render core's state cache), then restore the lighting manager's default primitive parameters
// Leaf: ?PostRenderClearRenderStates@CEngineLandscapeRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B67510, length 389 bytes, plain `ret`; `this` is never read. Callers 0x00B68F96
//   and 0x00B6B23E (CEngineLandscapeRenderer::Render). Grade: RELOCATION_MATCH (2nd grade).
// Name: Ego_r.pdb `?PostRenderClearRenderStates@CEngineLandscapeRenderer@@AAEXXZ` (private; the catalog binding spells QAE).
// Callees: none out of line -- IDirect3DDevice9 virtual slots 104 SetIndices (+0x1A0), 100 SetStreamSource (+0x190) and 65
//   SetTexture (+0x104) (d3d9.h order), all through the render core's inline state-cache helpers.
// Globals: g_pRenderManagerCore 0x01436E18 (CRenderManagerCore: TextureStagesInUse +0x3CB8, PD3DDevice +0x3CF0,
//   PSelectedIndexBuffer +0x3CF4, CurrentlySelectedTextures[8] +0x3D00 -- Ego_r.pdb names at retail offsets, engine header);
//   g_pEngineLightingManager 0x01436E9C (FadedShadowDiffuseFactorEnabled +0xCE, CurrentThemeChannel +0xE0, StateUpdateFlags
//   +0xE4, SelfIlluminationAmbientPercentage +0xE8 -- engine header).
// Shape: every render-core helper re-reads the global and keeps its `this` in esi across the device call (the texture-stage
//   bookkeeping after each call reads esi, the next helper reloads eax); the theme descriptor (0,0,0) is built in the push-ecx
//   slot and copied as a dword. The inline helpers are the FableWin.pdb methods SelectIndexBuffer / SelectVertexBuffer /
//   AttachTextureToStage -> AttachD3DTextureToStage (render core) and RestoreDefaultPrimitiveParameters ->
//   SetEnableFadedShadowFactor / SetCurrentEnvironmentThemeChannel / SetSelfIlluminationAmbient (lighting manager); their
//   bodies are reconstructed from this function (null arguments fold every buffer/texture lookup away).
// IDirect3DDevice9 (d3d9.h, DirectX 9.0) -- the COM interface in its SDK slot order; only the slots this TU calls carry
// parameter lists, the others keep their SDK names with no arguments (never called here).
struct IDirect3DBaseTexture9;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct IDirect3DDevice9 {
    virtual long __stdcall QueryInterface() = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
    virtual long __stdcall TestCooperativeLevel() = 0;
    virtual unsigned int __stdcall GetAvailableTextureMem() = 0;
    virtual long __stdcall EvictManagedResources() = 0;
    virtual long __stdcall GetDirect3D() = 0;
    virtual long __stdcall GetDeviceCaps() = 0;
    virtual long __stdcall GetDisplayMode() = 0;
    virtual long __stdcall GetCreationParameters() = 0;
    virtual long __stdcall SetCursorProperties() = 0;
    virtual void __stdcall SetCursorPosition() = 0;
    virtual int __stdcall ShowCursor() = 0;
    virtual long __stdcall CreateAdditionalSwapChain() = 0;
    virtual long __stdcall GetSwapChain() = 0;
    virtual unsigned int __stdcall GetNumberOfSwapChains() = 0;
    virtual long __stdcall Reset() = 0;
    virtual long __stdcall Present() = 0;
    virtual long __stdcall GetBackBuffer() = 0;
    virtual long __stdcall GetRasterStatus() = 0;
    virtual long __stdcall SetDialogBoxMode() = 0;
    virtual void __stdcall SetGammaRamp() = 0;
    virtual void __stdcall GetGammaRamp() = 0;
    virtual long __stdcall CreateTexture() = 0;
    virtual long __stdcall CreateVolumeTexture() = 0;
    virtual long __stdcall CreateCubeTexture() = 0;
    virtual long __stdcall CreateVertexBuffer() = 0;
    virtual long __stdcall CreateIndexBuffer() = 0;
    virtual long __stdcall CreateRenderTarget() = 0;
    virtual long __stdcall CreateDepthStencilSurface() = 0;
    virtual long __stdcall UpdateSurface() = 0;
    virtual long __stdcall UpdateTexture() = 0;
    virtual long __stdcall GetRenderTargetData() = 0;
    virtual long __stdcall GetFrontBufferData() = 0;
    virtual long __stdcall StretchRect() = 0;
    virtual long __stdcall ColorFill() = 0;
    virtual long __stdcall CreateOffscreenPlainSurface() = 0;
    virtual long __stdcall SetRenderTarget() = 0;
    virtual long __stdcall GetRenderTarget() = 0;
    virtual long __stdcall SetDepthStencilSurface() = 0;
    virtual long __stdcall GetDepthStencilSurface() = 0;
    virtual long __stdcall BeginScene() = 0;
    virtual long __stdcall EndScene() = 0;
    virtual long __stdcall Clear() = 0;
    virtual long __stdcall SetTransform() = 0;
    virtual long __stdcall GetTransform() = 0;
    virtual long __stdcall MultiplyTransform() = 0;
    virtual long __stdcall SetViewport() = 0;
    virtual long __stdcall GetViewport() = 0;
    virtual long __stdcall SetMaterial() = 0;
    virtual long __stdcall GetMaterial() = 0;
    virtual long __stdcall SetLight() = 0;
    virtual long __stdcall GetLight() = 0;
    virtual long __stdcall LightEnable() = 0;
    virtual long __stdcall GetLightEnable() = 0;
    virtual long __stdcall SetClipPlane() = 0;
    virtual long __stdcall GetClipPlane() = 0;
    virtual long __stdcall SetRenderState() = 0;
    virtual long __stdcall GetRenderState() = 0;
    virtual long __stdcall CreateStateBlock() = 0;
    virtual long __stdcall BeginStateBlock() = 0;
    virtual long __stdcall EndStateBlock() = 0;
    virtual long __stdcall SetClipStatus() = 0;
    virtual long __stdcall GetClipStatus() = 0;
    virtual long __stdcall GetTexture() = 0;
    virtual long __stdcall SetTexture(unsigned long Stage, IDirect3DBaseTexture9* pTexture) = 0;
    virtual long __stdcall GetTextureStageState() = 0;
    virtual long __stdcall SetTextureStageState() = 0;
    virtual long __stdcall GetSamplerState() = 0;
    virtual long __stdcall SetSamplerState() = 0;
    virtual long __stdcall ValidateDevice() = 0;
    virtual long __stdcall SetPaletteEntries() = 0;
    virtual long __stdcall GetPaletteEntries() = 0;
    virtual long __stdcall SetCurrentTexturePalette() = 0;
    virtual long __stdcall GetCurrentTexturePalette() = 0;
    virtual long __stdcall SetScissorRect() = 0;
    virtual long __stdcall GetScissorRect() = 0;
    virtual long __stdcall SetSoftwareVertexProcessing() = 0;
    virtual int __stdcall GetSoftwareVertexProcessing() = 0;
    virtual long __stdcall SetNPatchMode() = 0;
    virtual float __stdcall GetNPatchMode() = 0;
    virtual long __stdcall DrawPrimitive() = 0;
    virtual long __stdcall DrawIndexedPrimitive() = 0;
    virtual long __stdcall DrawPrimitiveUP() = 0;
    virtual long __stdcall DrawIndexedPrimitiveUP() = 0;
    virtual long __stdcall ProcessVertices() = 0;
    virtual long __stdcall CreateVertexDeclaration() = 0;
    virtual long __stdcall SetVertexDeclaration() = 0;
    virtual long __stdcall GetVertexDeclaration() = 0;
    virtual long __stdcall SetFVF() = 0;
    virtual long __stdcall GetFVF() = 0;
    virtual long __stdcall CreateVertexShader() = 0;
    virtual long __stdcall SetVertexShader() = 0;
    virtual long __stdcall GetVertexShader() = 0;
    virtual long __stdcall SetVertexShaderConstantF() = 0;
    virtual long __stdcall GetVertexShaderConstantF() = 0;
    virtual long __stdcall SetVertexShaderConstantI() = 0;
    virtual long __stdcall GetVertexShaderConstantI() = 0;
    virtual long __stdcall SetVertexShaderConstantB() = 0;
    virtual long __stdcall GetVertexShaderConstantB() = 0;
    virtual long __stdcall SetStreamSource(unsigned int StreamNumber, IDirect3DVertexBuffer9* pStreamData, unsigned int OffsetInBytes, unsigned int Stride) = 0;
    virtual long __stdcall GetStreamSource() = 0;
    virtual long __stdcall SetStreamSourceFreq() = 0;
    virtual long __stdcall GetStreamSourceFreq() = 0;
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* pIndexData) = 0;
};

#define CRenderManagerCore CRenderManagerCoreLayout
#include "engine/CRenderManagerCore.h"
#undef CRenderManagerCore
#define CEngineLightingManager CEngineLightingManagerLayout
#include "engine/CEngineLightingManager.h"
#undef CEngineLightingManager

class CVertexBufferWin32 { public:
    IDirect3DVertexBuffer9* GetD3DVertexBuffer() const;
    unsigned long GetStride() const;
};
class CIndexBuffer { public:
    IDirect3DIndexBuffer9* GetD3DIndexBuffer() const;
};
class CTexture { public:
    IDirect3DBaseTexture9* GetD3DTexture() const;
};

class CRenderManagerCore : public CRenderManagerCoreLayout {
public:
    __forceinline void SelectIndexBuffer(CIndexBuffer* buffer, unsigned long baseVertexIndex)
    {
        PSelectedIndexBuffer = buffer;
        PD3DDevice->SetIndices(buffer ? buffer->GetD3DIndexBuffer() : 0);
    }
    __forceinline void SelectVertexBuffer(unsigned long stream, CVertexBufferWin32* buffer)
    {
        PD3DDevice->SetStreamSource(stream, buffer ? buffer->GetD3DVertexBuffer() : 0, 0, buffer ? buffer->GetStride() : 0);
    }
    __forceinline void AttachTextureToStage(long stage, const CTexture* texture)
    {
        AttachD3DTextureToStage(stage, texture ? texture->GetD3DTexture() : 0);
    }
private:
    __forceinline void AttachD3DTextureToStage(long stage, IDirect3DBaseTexture9* texture)
    {
        if (CurrentlySelectedTextures[stage] != texture) {
            CurrentlySelectedTextures[stage] = texture;
            PD3DDevice->SetTexture(stage, texture);
        }
        if (TextureStagesInUse < stage + 1)
            TextureStagesInUse = stage + 1;
    }
};
extern CRenderManagerCore* g_pRenderManagerCore;           // 0x01436E18

class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0;
    unsigned char ChannelIndex1;
    unsigned char ChannelBlendFactor;
    unsigned char Pad;
    CEnginePrimitiveThemeChannelDesc(unsigned char index0, unsigned char index1, unsigned char blend)
        : ChannelIndex0(index0), ChannelIndex1(index1), ChannelBlendFactor(blend) { }
};
class CEngineLightingManager : public CEngineLightingManagerLayout {
public:
    CEnginePrimitiveThemeChannelDesc& CurrentThemeChannel()
    {
        return *reinterpret_cast<CEnginePrimitiveThemeChannelDesc*>(&CurrentThemeChannel_ChannelIndex0);
    }
    __forceinline void SetEnableFadedShadowFactor(bool enable)
    {
        if (FadedShadowDiffuseFactorEnabled != enable) {
            FadedShadowDiffuseFactorEnabled = enable;
            StateUpdateFlags |= 1;
        }
    }
    __forceinline void SetCurrentEnvironmentThemeChannel(const CEnginePrimitiveThemeChannelDesc& channel)
    {
        if (CurrentThemeChannel_ChannelIndex0 != channel.ChannelIndex0 || CurrentThemeChannel_ChannelIndex1 != channel.ChannelIndex1
            || CurrentThemeChannel_ChannelBlendFactor != channel.ChannelBlendFactor) {
            CurrentThemeChannel() = channel;
            if (CurrentThemeChannel_ChannelBlendFactor == 0xff) {
                CurrentThemeChannel_ChannelIndex0 = CurrentThemeChannel_ChannelIndex1;
                CurrentThemeChannel_ChannelBlendFactor = 0;
            }
            StateUpdateFlags |= 0xf;
        }
    }
    __forceinline void SetSelfIlluminationAmbient(long percentage)
    {
        if (SelfIlluminationAmbientPercentage != percentage) {
            SelfIlluminationAmbientPercentage = percentage;
            StateUpdateFlags |= 2;
        }
    }
    __forceinline void RestoreDefaultPrimitiveParameters()
    {
        SetEnableFadedShadowFactor(false);
        SetCurrentEnvironmentThemeChannel(CEnginePrimitiveThemeChannelDesc(0, 0, 0));
        SetSelfIlluminationAmbient(0);
    }
};
extern CEngineLightingManager* g_pEngineLightingManager;   // 0x01436E9C

#define CEngineLandscapeRenderer CEngineLandscapeRendererLayout
#include "engine/CEngineLandscapeRenderer.h"
#undef CEngineLandscapeRenderer
class CEngineLandscapeRenderer : public CEngineLandscapeRendererLayout {
private:
    void PostRenderClearRenderStates();
};

void CEngineLandscapeRenderer::PostRenderClearRenderStates()
{
    g_pRenderManagerCore->SelectIndexBuffer(0, 0);
    g_pRenderManagerCore->SelectVertexBuffer(0, 0);
    g_pRenderManagerCore->AttachTextureToStage(0, 0);
    g_pRenderManagerCore->AttachTextureToStage(1, 0);
    g_pRenderManagerCore->AttachTextureToStage(2, 0);
    g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
}
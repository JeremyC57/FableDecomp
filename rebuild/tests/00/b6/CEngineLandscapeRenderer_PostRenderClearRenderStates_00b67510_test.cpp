#include <stdio.h>
#ifdef _MSC_VER
#define STDCALL __stdcall
#else
#define STDCALL
#endif
// IDirect3DDevice9 (d3d9.h, DirectX 9.0) -- the COM interface in its SDK slot order; only the slots this TU calls carry
// parameter lists, the others keep their SDK names with no arguments (never called here).
struct IDirect3DBaseTexture9;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct IDirect3DDevice9 {
    virtual long STDCALL QueryInterface() { return 0; }
    virtual unsigned long STDCALL AddRef() { return 0; }
    virtual unsigned long STDCALL Release() { return 0; }
    virtual long STDCALL TestCooperativeLevel() { return 0; }
    virtual unsigned int STDCALL GetAvailableTextureMem() { return 0; }
    virtual long STDCALL EvictManagedResources() { return 0; }
    virtual long STDCALL GetDirect3D() { return 0; }
    virtual long STDCALL GetDeviceCaps() { return 0; }
    virtual long STDCALL GetDisplayMode() { return 0; }
    virtual long STDCALL GetCreationParameters() { return 0; }
    virtual long STDCALL SetCursorProperties() { return 0; }
    virtual void STDCALL SetCursorPosition() { }
    virtual int STDCALL ShowCursor() { return 0; }
    virtual long STDCALL CreateAdditionalSwapChain() { return 0; }
    virtual long STDCALL GetSwapChain() { return 0; }
    virtual unsigned int STDCALL GetNumberOfSwapChains() { return 0; }
    virtual long STDCALL Reset() { return 0; }
    virtual long STDCALL Present() { return 0; }
    virtual long STDCALL GetBackBuffer() { return 0; }
    virtual long STDCALL GetRasterStatus() { return 0; }
    virtual long STDCALL SetDialogBoxMode() { return 0; }
    virtual void STDCALL SetGammaRamp() { }
    virtual void STDCALL GetGammaRamp() { }
    virtual long STDCALL CreateTexture() { return 0; }
    virtual long STDCALL CreateVolumeTexture() { return 0; }
    virtual long STDCALL CreateCubeTexture() { return 0; }
    virtual long STDCALL CreateVertexBuffer() { return 0; }
    virtual long STDCALL CreateIndexBuffer() { return 0; }
    virtual long STDCALL CreateRenderTarget() { return 0; }
    virtual long STDCALL CreateDepthStencilSurface() { return 0; }
    virtual long STDCALL UpdateSurface() { return 0; }
    virtual long STDCALL UpdateTexture() { return 0; }
    virtual long STDCALL GetRenderTargetData() { return 0; }
    virtual long STDCALL GetFrontBufferData() { return 0; }
    virtual long STDCALL StretchRect() { return 0; }
    virtual long STDCALL ColorFill() { return 0; }
    virtual long STDCALL CreateOffscreenPlainSurface() { return 0; }
    virtual long STDCALL SetRenderTarget() { return 0; }
    virtual long STDCALL GetRenderTarget() { return 0; }
    virtual long STDCALL SetDepthStencilSurface() { return 0; }
    virtual long STDCALL GetDepthStencilSurface() { return 0; }
    virtual long STDCALL BeginScene() { return 0; }
    virtual long STDCALL EndScene() { return 0; }
    virtual long STDCALL Clear() { return 0; }
    virtual long STDCALL SetTransform() { return 0; }
    virtual long STDCALL GetTransform() { return 0; }
    virtual long STDCALL MultiplyTransform() { return 0; }
    virtual long STDCALL SetViewport() { return 0; }
    virtual long STDCALL GetViewport() { return 0; }
    virtual long STDCALL SetMaterial() { return 0; }
    virtual long STDCALL GetMaterial() { return 0; }
    virtual long STDCALL SetLight() { return 0; }
    virtual long STDCALL GetLight() { return 0; }
    virtual long STDCALL LightEnable() { return 0; }
    virtual long STDCALL GetLightEnable() { return 0; }
    virtual long STDCALL SetClipPlane() { return 0; }
    virtual long STDCALL GetClipPlane() { return 0; }
    virtual long STDCALL SetRenderState() { return 0; }
    virtual long STDCALL GetRenderState() { return 0; }
    virtual long STDCALL CreateStateBlock() { return 0; }
    virtual long STDCALL BeginStateBlock() { return 0; }
    virtual long STDCALL EndStateBlock() { return 0; }
    virtual long STDCALL SetClipStatus() { return 0; }
    virtual long STDCALL GetClipStatus() { return 0; }
    virtual long STDCALL GetTexture() { return 0; }
    virtual long STDCALL SetTexture(unsigned long Stage, IDirect3DBaseTexture9* pTexture) { ++calls; lastStage = Stage; return 0; }
    virtual long STDCALL GetTextureStageState() { return 0; }
    virtual long STDCALL SetTextureStageState() { return 0; }
    virtual long STDCALL GetSamplerState() { return 0; }
    virtual long STDCALL SetSamplerState() { return 0; }
    virtual long STDCALL ValidateDevice() { return 0; }
    virtual long STDCALL SetPaletteEntries() { return 0; }
    virtual long STDCALL GetPaletteEntries() { return 0; }
    virtual long STDCALL SetCurrentTexturePalette() { return 0; }
    virtual long STDCALL GetCurrentTexturePalette() { return 0; }
    virtual long STDCALL SetScissorRect() { return 0; }
    virtual long STDCALL GetScissorRect() { return 0; }
    virtual long STDCALL SetSoftwareVertexProcessing() { return 0; }
    virtual int STDCALL GetSoftwareVertexProcessing() { return 0; }
    virtual long STDCALL SetNPatchMode() { return 0; }
    virtual float STDCALL GetNPatchMode() { return 0; }
    virtual long STDCALL DrawPrimitive() { return 0; }
    virtual long STDCALL DrawIndexedPrimitive() { return 0; }
    virtual long STDCALL DrawPrimitiveUP() { return 0; }
    virtual long STDCALL DrawIndexedPrimitiveUP() { return 0; }
    virtual long STDCALL ProcessVertices() { return 0; }
    virtual long STDCALL CreateVertexDeclaration() { return 0; }
    virtual long STDCALL SetVertexDeclaration() { return 0; }
    virtual long STDCALL GetVertexDeclaration() { return 0; }
    virtual long STDCALL SetFVF() { return 0; }
    virtual long STDCALL GetFVF() { return 0; }
    virtual long STDCALL CreateVertexShader() { return 0; }
    virtual long STDCALL SetVertexShader() { return 0; }
    virtual long STDCALL GetVertexShader() { return 0; }
    virtual long STDCALL SetVertexShaderConstantF() { return 0; }
    virtual long STDCALL GetVertexShaderConstantF() { return 0; }
    virtual long STDCALL SetVertexShaderConstantI() { return 0; }
    virtual long STDCALL GetVertexShaderConstantI() { return 0; }
    virtual long STDCALL SetVertexShaderConstantB() { return 0; }
    virtual long STDCALL GetVertexShaderConstantB() { return 0; }
    virtual long STDCALL SetStreamSource(unsigned int StreamNumber, IDirect3DVertexBuffer9* pStreamData, unsigned int OffsetInBytes, unsigned int Stride) { ++calls; return 0; }
    virtual long STDCALL GetStreamSource() { return 0; }
    virtual long STDCALL SetStreamSourceFreq() { return 0; }
    virtual long STDCALL GetStreamSourceFreq() { return 0; }
    virtual long STDCALL SetIndices(IDirect3DIndexBuffer9* pIndexData) { ++calls; return 0; }
    int calls; unsigned long lastStage;
};

class CRenderManagerCore { public:
    long TextureStagesInUse;
    IDirect3DDevice9* PD3DDevice;
    void* PSelectedIndexBuffer;
    IDirect3DBaseTexture9* CurrentlySelectedTextures[8];
    void SelectIndexBuffer(void* buffer) { PSelectedIndexBuffer = buffer; PD3DDevice->SetIndices(0); }
    void SelectVertexBuffer(unsigned long stream) { PD3DDevice->SetStreamSource(stream, 0, 0, 0); }
    void AttachD3DTextureToStage(long stage, IDirect3DBaseTexture9* texture)
    {
        if (CurrentlySelectedTextures[stage] != texture) {
            CurrentlySelectedTextures[stage] = texture;
            PD3DDevice->SetTexture(stage, texture);
        }
        if (TextureStagesInUse < stage + 1)
            TextureStagesInUse = stage + 1;
    }
};
class CEngineLightingManager { public:
    bool FadedShadowDiffuseFactorEnabled;
    unsigned char Channel[4];
    unsigned long StateUpdateFlags;
    long SelfIlluminationAmbientPercentage;
    void RestoreDefaultPrimitiveParameters()
    {
        if (FadedShadowDiffuseFactorEnabled) { FadedShadowDiffuseFactorEnabled = false; StateUpdateFlags |= 1; }
        if (Channel[0] != 0 || Channel[1] != 0 || Channel[2] != 0) {
            Channel[0] = Channel[1] = Channel[2] = 0;
            StateUpdateFlags |= 0xf;
        }
        if (SelfIlluminationAmbientPercentage != 0) { SelfIlluminationAmbientPercentage = 0; StateUpdateFlags |= 2; }
    }
};
static CRenderManagerCore* g_pRenderManagerCore;
static CEngineLightingManager* g_pEngineLightingManager;
static void PostRenderClearRenderStates()
{
    g_pRenderManagerCore->SelectIndexBuffer(0);
    g_pRenderManagerCore->SelectVertexBuffer(0);
    g_pRenderManagerCore->AttachD3DTextureToStage(0, 0);
    g_pRenderManagerCore->AttachD3DTextureToStage(1, 0);
    g_pRenderManagerCore->AttachD3DTextureToStage(2, 0);
    g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
}
int main()
{
    static IDirect3DDevice9 device;
    device.calls = 0;
    static CRenderManagerCore core;
    core.PD3DDevice = &device;
    core.PSelectedIndexBuffer = &core;
    core.TextureStagesInUse = 0;
    for (int i = 0; i < 8; ++i) core.CurrentlySelectedTextures[i] = 0;
    core.CurrentlySelectedTextures[1] = (IDirect3DBaseTexture9*)&device;
    static CEngineLightingManager lm;
    lm.FadedShadowDiffuseFactorEnabled = true; lm.Channel[0] = 3; lm.Channel[1] = 0; lm.Channel[2] = 0;
    lm.StateUpdateFlags = 0; lm.SelfIlluminationAmbientPercentage = 0;
    g_pRenderManagerCore = &core; g_pEngineLightingManager = &lm;
    PostRenderClearRenderStates();
    int ok = device.calls == 3 && device.lastStage == 1 && core.PSelectedIndexBuffer == 0 && core.TextureStagesInUse == 3
        && core.CurrentlySelectedTextures[1] == 0 && !lm.FadedShadowDiffuseFactorEnabled && lm.Channel[0] == 0
        && lm.StateUpdateFlags == 0xf;
    if (!ok) { printf("00b67510_TEST FAIL\n"); return 1; }
    printf("00b67510_TEST PASS\n");
    return 0;
}
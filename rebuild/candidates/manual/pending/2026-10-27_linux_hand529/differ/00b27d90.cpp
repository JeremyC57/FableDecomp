// CEngine::Render(const CEngineInterpolationInfo& info, unsigned long renderFlags) -- the engine's per-frame render entry: if the engine is enabled, update the off-world-tick processing state, unbind the device's stream source, index buffer and selected textures, force vertex and pixel shaders on, load the new scene if one is pending, render the frame, then put the shader enables back as they were
// Grade: DIFFER(328v325) -- 4 grades; only the shader-enable save differs: retail reads VertexShadersEnabled / PixelShadersEnabled as separate bytes (al, then bl / [esp+1Dh]), this source makes VC7.1 widen each read to a word (`mov ax, word ptr`, `ah`). Grade 2 (two bool locals) had the byte reads but homed the pixel flag at byte 0 of the dead renderFlags slot and loaded the vertex flag straight into bl; grade 4 (inline Set*Enabled(bool) helpers) also flipped the restore compares. Next idea: the saved pair as a class with a user constructor, or as a CShaderRenderManager sub-object copy.
// Leaf: ?Render@CEngine@@UAEXABVCEngineInterpolationInfo@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B27D90, 325 bytes, thiscall `ret 8`.
// Identity (proven): Ego_r.pdb ?Render@CEngine@@UAEXABVCEngineInterpolationInfo@@K@Z (virtual; donor 0x009B39A0); the CEngine
//   vtable 0x012A0F5C holds it (its only reference; the landed CEngine constructor names it slot 8). Callees are the Ego_r
//   private members UpdateOffGameTickProcessInfo(bool) 0x00B23A90, LoadNewScene(const CEngineInterpolationInfo&)
//   0x00B277A0 and RenderFrame 0x00B25950 (this chunk), and CShaderRenderManager::EnableVertexShaders 0x00987FE0 /
//   DisableVertexShaders 0x009880E0 / EnablePixelShaders 0x00988110 / DisablePixelShaders 0x00988190 (Ego_r names; each
//   also landed under a wrong CShaderEnableState spelling -- NAME_FIXES).
// Members (retail offsets, Ego_r names): EnableEngine +0x11, NewSceneFlag +0x18, CutscenePreloadFlag +0x24,
//   CameraAngleChanged +0x39; CRenderManagerCore TextureStagesInUse +0x3CB8, PD3DDevice +0x3CF0,
//   CurrentlySelectedTextures[8] +0x3D00 (Ego_r offsets, equal in retail); CShaderRenderManager VertexShadersEnabled
//   +0x390 / PixelShadersEnabled +0x391 (Ego_r +0x3A0 / +0x3A1, less the four 16-to-12-byte CArrays before them).
// Globals (no PDB data names; bound spellings whose types the layouts prove): 0x01436E18 CRenderManagerCore*
//   (?g_pRenderManagerCore@@3PAVCRenderManagerCore@@A), 0x013BC470 the CShaderRenderManager object
//   (?g_ShaderRenderManager@@3VCShaderRenderManager@@A).
// Shape: the device unbinding is an inline CRenderManagerCore member (descriptive name: inlined, absent from the PDB;
//   __forceinline -- plain inline stayed a call, grade 1); the shader enables are saved in a two-bool local (retail homes
//   the pixel flag at byte 1 of the dead renderFlags slot), forced on, and restored after the frame.
struct IDirect3DBaseTexture9;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct IDirect3DDevice9 {            // d3d9.h method order (IUnknown first); only the three used carry their parameters
    virtual long __stdcall QueryInterface() = 0;
    virtual long __stdcall AddRef() = 0;
    virtual long __stdcall Release() = 0;
    virtual long __stdcall TestCooperativeLevel() = 0;
    virtual long __stdcall GetAvailableTextureMem() = 0;
    virtual long __stdcall EvictManagedResources() = 0;
    virtual long __stdcall GetDirect3D() = 0;
    virtual long __stdcall GetDeviceCaps() = 0;
    virtual long __stdcall GetDisplayMode() = 0;
    virtual long __stdcall GetCreationParameters() = 0;
    virtual long __stdcall SetCursorProperties() = 0;
    virtual long __stdcall SetCursorPosition() = 0;
    virtual long __stdcall ShowCursor() = 0;
    virtual long __stdcall CreateAdditionalSwapChain() = 0;
    virtual long __stdcall GetSwapChain() = 0;
    virtual long __stdcall GetNumberOfSwapChains() = 0;
    virtual long __stdcall Reset() = 0;
    virtual long __stdcall Present() = 0;
    virtual long __stdcall GetBackBuffer() = 0;
    virtual long __stdcall GetRasterStatus() = 0;
    virtual long __stdcall SetDialogBoxMode() = 0;
    virtual long __stdcall SetGammaRamp() = 0;
    virtual long __stdcall GetGammaRamp() = 0;
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
    virtual long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture) = 0;   // slot 65
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
    virtual long __stdcall GetSoftwareVertexProcessing() = 0;
    virtual long __stdcall SetNPatchMode() = 0;
    virtual long __stdcall GetNPatchMode() = 0;
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
    virtual long __stdcall SetStreamSource(unsigned int stream, IDirect3DVertexBuffer9* data, unsigned int offset, unsigned int stride) = 0;   // slot 100
    virtual long __stdcall GetStreamSource() = 0;
    virtual long __stdcall SetStreamSourceFreq() = 0;
    virtual long __stdcall GetStreamSourceFreq() = 0;
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* indexData) = 0;   // slot 104
};

class CRenderManagerCore {
public:
    __forceinline void ReleaseSelectedDeviceObjects()      // inline (no PDB entry)
    {
        PD3DDevice->SetStreamSource(0, 0, 0, 0);
        PD3DDevice->SetIndices(0);
        for (long stage = 0; stage < TextureStagesInUse; ++stage) {
            if (CurrentlySelectedTextures[stage] != 0) {
                CurrentlySelectedTextures[stage] = 0;
                PD3DDevice->SetTexture(stage, 0);
            }
        }
        TextureStagesInUse = 0;
    }
    unsigned char _pad_0x0000[0x3CB8];                     // +0x0000
    long TextureStagesInUse;                               // +0x3CB8
    unsigned char _pad_0x3cbc[0x34];                       // +0x3CBC NoZSortedPolys .. NoLastFrameCachedTextureUploads
    IDirect3DDevice9* PD3DDevice;                          // +0x3CF0
    void* PSelectedIndexBuffer;                            // +0x3CF4 CIndexBuffer*
    unsigned long CurrentVertexFormat;                     // +0x3CF8
    bool XBoxShaderConstantsEnabled;                       // +0x3CFC
    bool FixedFunctionPipelineEnabled;                     // +0x3CFD
    IDirect3DBaseTexture9* CurrentlySelectedTextures[8];   // +0x3D00
};
extern CRenderManagerCore* g_pRenderManagerCore;           // 0x01436E18

class CShaderRenderManager {
public:
    void EnableVertexShaders();                            // 0x00987FE0
    void DisableVertexShaders();                           // 0x009880E0
    void EnablePixelShaders();                             // 0x00988110
    void DisablePixelShaders();                            // 0x00988190
    unsigned char _pad_0x000[0x390];                       // +0x000
    bool VertexShadersEnabled;                             // +0x390
    bool PixelShadersEnabled;                              // +0x391
};
extern CShaderRenderManager g_ShaderRenderManager;         // 0x013BC470

class CEngineInterpolationInfo;

class CEngine {
public:
    virtual ~CEngine();
    virtual void Render(const CEngineInterpolationInfo& info, unsigned long renderFlags);
private:
    void UpdateOffGameTickProcessInfo(bool processThisFrame);   // 0x00B23A90
    void LoadNewScene(const CEngineInterpolationInfo& info);    // 0x00B277A0
    void RenderFrame(const CEngineInterpolationInfo& info);     // 0x00B25950
public:
    unsigned char _pad_0x04[0xC];                          // +0x04
    bool EnableWireFrame;                                  // +0x10
    bool EnableEngine;                                     // +0x11
    bool EnableLocalLights;                                // +0x12
    bool EnableTextures;                                   // +0x13
    void* CurrentCamera;                                   // +0x14 CCamera*
    bool NewSceneFlag;                                     // +0x18
    unsigned long NewSceneLoadFlags;                       // +0x1C
    float NewSceneTimeOut;                                 // +0x20
    bool CutscenePreloadFlag;                              // +0x24
    float CutscenePreloadTimeout;                          // +0x28
    float CutsceneFastPreloadTimeout;                      // +0x2C
    float ActorAdditionalLoadTimeout;                      // +0x30
    float ActorRadius;                                     // +0x34
    bool ActorsLoading;                                    // +0x38
    bool CameraAngleChanged;                               // +0x39
};

void CEngine::Render(const CEngineInterpolationInfo& info, unsigned long renderFlags)
{
    if (!EnableEngine)
        return;
    UpdateOffGameTickProcessInfo((renderFlags & 1) != 0);
    g_pRenderManagerCore->ReleaseSelectedDeviceObjects();

    struct {
        bool VertexShadersEnabled;
        bool PixelShadersEnabled;
    } previous;
    previous.VertexShadersEnabled = g_ShaderRenderManager.VertexShadersEnabled;
    if (g_ShaderRenderManager.VertexShadersEnabled != true)
        g_ShaderRenderManager.EnableVertexShaders();
    previous.PixelShadersEnabled = g_ShaderRenderManager.PixelShadersEnabled;
    if (g_ShaderRenderManager.PixelShadersEnabled != true)
        g_ShaderRenderManager.EnablePixelShaders();

    if (NewSceneFlag || CutscenePreloadFlag || CameraAngleChanged)
        LoadNewScene(info);
    RenderFrame(info);

    if (previous.VertexShadersEnabled != g_ShaderRenderManager.VertexShadersEnabled) {
        if (previous.VertexShadersEnabled)
            g_ShaderRenderManager.EnableVertexShaders();
        else
            g_ShaderRenderManager.DisableVertexShaders();
    }
    if (previous.PixelShadersEnabled != g_ShaderRenderManager.PixelShadersEnabled) {
        if (previous.PixelShadersEnabled)
            g_ShaderRenderManager.EnablePixelShaders();
        else
            g_ShaderRenderManager.DisablePixelShaders();
    }
}

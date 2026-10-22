// Grade: DIFFER(926v926) -- scheduling only: the per-primitive {0,0} info zero stores sit after the arg pushes in retail (before the vtable load), before them in ours; and the GlobalColours[0] copy uses eax/edx swapped.
// CEnginePrimitiveRenderer2D::Render(const CEngineComponentRenderParams& params) -- render the 2D primitive lists through an orthographic full-target camera: open a capture block, force shaders on, save/force two device flags, swap in a 2D camera desc and the renderer's own global lighting, walk the overlay (layer 0x200000) or the normal list calling each primitive manager's virtual RenderPrimitive, then restore everything
// Leaf: Render
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4A700, length 928 bytes, `ret 4`, ebp frame aligned to 8 (`and esp,-8`: the 8-byte
//   per-primitive params built inside the loop with their address taken -- GOTCHAS chunk 232).
// Callees: CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (0x01436E1C), device flag setters 0x009FD100/0x009FD0F0 on
//   (0x01436E98)+0x954, CEngineCameraDesc copy ctor 0x00B4AF50, CEngineCamera::SetCameraState 0x00B30B50 (0x01436EA0),
//   CEngineLightingManager::SetGlobalLightingParameters 0x00B47280 (0x01436E9C), CEngineSubPrimitiveRenderer::RenderType
//   0x00B849F0 (0x01436E74), the primitive-manager table 0x01436E84, Enable/Disable shaders 0x00987FE0/0x009880E0/0x00988110/
//   0x00988190 on 0x013BC470, RestoreCaptureBlock 0x00A05840.
// Layout: local model (engine/CEnginePrimitiveRenderer2D.h is PDB-only padding).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRGBFloatColour { float R, G, B, A; };
struct C2DExtentsI { long Width, Height; };
struct CMatrix3x4 { float M[12]; };
struct C2DBoxF {
    float X0, Y0, X1, Y1;
    C2DBoxF() {}
    C2DBoxF(float x0, float y0, float x1, float y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
struct CEngineCameraDesc {                               // 0x6c bytes (PDB)
    C2DBoxF    Window;                                   // +0x00
    CMatrix3x4 RotationMatrix;                           // +0x10
    C3DVector  WorldPosition;                            // +0x40
    float      HorizontalFOV;                            // +0x4c
    float      VerticalFOV;                              // +0x50
    bool       Use2DFOV;                                 // +0x54
    float      NearZPlane;                               // +0x58
    float      FarZPlane;                                // +0x5c
    float      ZBufferStart;                             // +0x60
    float      ZBufferEnd;                               // +0x64
    bool       StateValid;                               // +0x68
    CEngineCameraDesc() {}
    CEngineCameraDesc(const CEngineCameraDesc& other);   // 0x00B4AF50
};
struct CEngineCamera {
    unsigned char     _pad00[0x14];
    CEngineCameraDesc CameraDesc;                        // +0x14
    void SetCameraState(const CEngineCameraDesc* desc, bool a, bool b);   // 0x00B30B50
};
extern CEngineCamera* g_pEngineCamera;                   // 0x01436EA0
struct CEngineLightingManager {
    unsigned char    _pad00[0x30];
    C3DVector        GlobalLightDirection;               // +0x30
    CRGBFloatColour* GlobalColours;                      // +0x3c
    void SetGlobalLightingParameters(const C3DVector* direction, const CRGBFloatColour* a, const CRGBFloatColour* b,
                                     const CRGBFloatColour* c);   // 0x00B47280
};
extern CEngineLightingManager* g_pEngineLightingManager; // 0x01436E9C
struct CDisplayManager {
    C2DExtentsI GetRenderTargetDimensions() const;       // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayManager;               // 0x01436E1C
struct CRenderDevice {
    unsigned char _pad000[0x268];
    long          Flag268;                               // +0x268
    long          Flag26c;                               // +0x26c
    void SetFlag26c(long value);                         // 0x009FD100
    void SetFlag268(long value);                         // 0x009FD0F0
};
struct CEngineDevice {
    unsigned char  _pad000[0x954];
    CRenderDevice* Device;                               // +0x954
};
extern CEngineDevice* g_pEngineDevice;                   // 0x01436E98
struct CEngineSubPrimitiveRenderer { void RenderType(long type, bool flag); };   // 0x00B849F0
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

struct CEngineInternalPrimitiveBase;
struct SPrimitiveRenderInfo { long A; long B; SPrimitiveRenderInfo() : A(0), B(0) {} };
struct CEnginePrimitiveManagerBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void RenderPrimitive(CEngineInternalPrimitiveBase* primitive, const SPrimitiveRenderInfo& info, long pass);   // slot 10
};
struct CEnginePrimitiveManagers {
    unsigned char                _pad00[0x10];
    CEnginePrimitiveManagerBase* Managers[64];           // +0x10
};
extern CEnginePrimitiveManagers* g_pPrimitiveManagers;   // 0x01436E84
struct CEngineInternalPrimitiveBase {
    unsigned char                 _pad00[0x8];
    long                          ManagerType;           // +0x08
    unsigned char                 _pad0c[0x38 - 0xc];
    CEngineInternalPrimitiveBase* Next;                  // +0x38
};

struct CTrackedRenderState;
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;
    unsigned long        m_priorValue;
};
struct CRenderManagerCoreView {                          // the render-state manager @ 0x013CB508
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];              // +0x0008
    CTrackedRenderState* m_dirty[0x200];                 // +0x2008
    long                 m_capturedCount;                // +0x2808
    long                 m_dirtyCount;                   // +0x280C
    long                 m_unknown2810;                  // +0x2810
    unsigned long        m_captureMask;                  // +0x2814
    unsigned char        _pad2818[0x3a24 - 0x2818];
    unsigned char        m_captureBlockMarker[4];        // +0x3A24
    __forceinline void OpenCaptureBlock()
    {
        m_captured[m_capturedCount++].m_pState = (CTrackedRenderState*)m_captureBlockMarker;
        m_captureMask <<= 1;
    }
    void RestoreCaptureBlock();                          // 0x00A05840
};
extern CRenderManagerCoreView* g_pRenderStateManager;   // 0x013CB508
struct CShaderEnableState {                             // the static shader manager object @ 0x013BC470
    unsigned char _pad000[0x390];
    bool          VertexShadersEnabled;                 // +0x390
    bool          PixelShadersEnabled;                  // +0x391
    unsigned char _pad392[2];
    long          _pad394;
    void EnableVertexShaders();                         // 0x00987FE0
    void DisableVertexShaders();                        // 0x009880E0
    void EnablePixelShaders();                          // 0x00988110
    void DisablePixelShaders();                         // 0x00988190
    __forceinline void SetVertexShadersEnabled(bool enable)
    {
        if (enable != VertexShadersEnabled) {
            if (enable)
                EnableVertexShaders();
            else
                DisableVertexShaders();
        }
    }
    __forceinline void SetPixelShadersEnabled(bool enable)
    {
        if (enable != PixelShadersEnabled) {
            if (enable)
                EnablePixelShaders();
            else
                DisablePixelShaders();
        }
    }
};
extern CShaderEnableState g_ShaderRenderManager;        // 0x013BC470
struct CShaderEnableSaver {
    bool m_vertexShaders;
    bool m_pixelShaders;
    __forceinline CShaderEnableSaver(bool vertexShaders, bool pixelShaders)
    {
        m_vertexShaders = g_ShaderRenderManager.VertexShadersEnabled;
        g_ShaderRenderManager.SetVertexShadersEnabled(vertexShaders);
        m_pixelShaders = g_ShaderRenderManager.PixelShadersEnabled;
        g_ShaderRenderManager.SetPixelShadersEnabled(pixelShaders);
    }
    __forceinline ~CShaderEnableSaver()
    {
        g_ShaderRenderManager.SetVertexShadersEnabled(m_vertexShaders);
        g_ShaderRenderManager.SetPixelShadersEnabled(m_pixelShaders);
    }
};
struct CRenderStateCaptureBlock {
    __forceinline CRenderStateCaptureBlock() { g_pRenderStateManager->OpenCaptureBlock(); }
    __forceinline ~CRenderStateCaptureBlock() { g_pRenderStateManager->RestoreCaptureBlock(); }
};

struct CEngineComponentRenderParams {
    void* Params;                                        // +0x00
    long  RenderLayer;                                   // +0x04
};
struct CEnginePrimitiveRenderer2D {
    unsigned char                 _pad04[0x40 - 4];      // +0x04 (vtable at +0x00)
    CEngineInternalPrimitiveBase* Primitives;            // +0x40
    CEngineInternalPrimitiveBase* OverlayPrimitives;     // +0x44
    float                         HorizontalFOV;         // +0x48
    CRGBFloatColour               ColourA;               // +0x4c
    CRGBFloatColour               ColourB;               // +0x5c
    CRGBFloatColour               ColourC;               // +0x6c
    C3DVector                     LightDirection;        // +0x7c
    virtual void Render(const CEngineComponentRenderParams& params);
};

void CEnginePrimitiveRenderer2D::Render(const CEngineComponentRenderParams& params)
{
    CRenderStateCaptureBlock capture;
    CShaderEnableSaver shaders(true, true);
    CRenderDevice* device = g_pEngineDevice->Device;
    long saved26c = device->Flag26c;
    long saved268 = device->Flag268;
    device->SetFlag26c(1);
    device->SetFlag268(1);
    CEngineCameraDesc savedDesc(g_pEngineCamera->CameraDesc);
    CEngineCameraDesc desc;
    desc.HorizontalFOV = HorizontalFOV;
    desc.StateValid = true;
    desc.NearZPlane = 0.1f;
    desc.FarZPlane = 1.0f;
    desc.ZBufferStart = 0.0f;
    desc.ZBufferEnd = 0.1f;
    desc.Use2DFOV = false;
    desc.VerticalFOV = 0.0f;
    desc.WorldPosition.X = 0.0f;
    desc.WorldPosition.Y = 0.0f;
    desc.WorldPosition.Z = 0.0f;
    desc.RotationMatrix.M[11] = 0.0f;
    desc.RotationMatrix.M[10] = 0.0f;
    desc.RotationMatrix.M[9] = 0.0f;
    desc.RotationMatrix.M[7] = 0.0f;
    desc.RotationMatrix.M[6] = 0.0f;
    desc.RotationMatrix.M[5] = 0.0f;
    desc.RotationMatrix.M[3] = 0.0f;
    desc.RotationMatrix.M[2] = 0.0f;
    desc.RotationMatrix.M[1] = 0.0f;
    desc.RotationMatrix.M[8] = 1.0f;
    desc.RotationMatrix.M[0] = 1.0f;
    desc.RotationMatrix.M[4] = -1.0f;
    C2DExtentsI extents = g_pDisplayManager->GetRenderTargetDimensions();
    desc.Window = C2DBoxF(0.0f, 0.0f, (float)extents.Width, (float)extents.Height);
    g_pEngineCamera->SetCameraState(&desc, false, true);
    C3DVector savedDirection = g_pEngineLightingManager->GlobalLightDirection;
    CRGBFloatColour savedB, savedC, savedA;
    savedB = g_pEngineLightingManager->GlobalColours[1];
    savedA = g_pEngineLightingManager->GlobalColours[0];
    savedC = g_pEngineLightingManager->GlobalColours[2];
    g_pEngineLightingManager->SetGlobalLightingParameters(&LightDirection, &ColourA, &ColourB, &ColourC);
    if (params.RenderLayer == 0x200000) {
        for (CEngineInternalPrimitiveBase* p = OverlayPrimitives; p; p = p->Next) {
            g_pPrimitiveManagers->Managers[p->ManagerType]->RenderPrimitive(p, SPrimitiveRenderInfo(), 2);
        }
    } else {
        for (CEngineInternalPrimitiveBase* p = Primitives; p; p = p->Next) {
            g_pPrimitiveManagers->Managers[p->ManagerType]->RenderPrimitive(p, SPrimitiveRenderInfo(), 2);
        }
        g_pEngineSubPrimitiveRenderer->RenderType(2, true);
    }
    g_pEngineLightingManager->SetGlobalLightingParameters(&savedDirection, &savedB, &savedA, &savedC);
    g_pEngineCamera->SetCameraState(&savedDesc, false, true);
    device->SetFlag26c(saved26c);
    device->SetFlag268(saved268);
}

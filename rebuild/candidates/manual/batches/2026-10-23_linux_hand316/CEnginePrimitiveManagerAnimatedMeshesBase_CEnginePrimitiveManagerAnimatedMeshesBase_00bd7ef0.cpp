// CEnginePrimitiveManagerAnimatedMeshesBase::~CEnginePrimitiveManagerAnimatedMeshesBase() -- compiler-generated destructor: the palette-skin vertex shaders (the spotlight CVertexShader[4], the 2D and outline singles, the colour-buffer shadow tables, the two shadow-buffer-render singles and the depth-buffer/static tables) released in reverse declaration order, then the CEnginePrimitiveManagerOcclusionBase destructor (tail jump)
// Leaf: ??1CEnginePrimitiveManagerAnimatedMeshesBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD7EF0, length 1323 bytes (tail `jmp` to the base destructor).
// Callees: ~CEnginePrimitiveManagerOcclusionBase @ 0x00B38590 (landed); the shared shader data's virtual deleting destructor
//   (slot 0, flag 1). Vtables: 0x0129B3D0 CVertexShader, 0x0129A030 CShaderBase.
// Layout: Ego_r.pdb CEnginePrimitiveManagerAnimatedMeshesBase (no engine header); its offsets hold in retail (the released
//   shaders sit at +0x38..+0x32b exactly as the PDB places them). The shader classes are the model of the landed
//   ~CEnginePrimitiveManagerParticleDecalGroup 0x00BB99A0 (twin: ~CEnginePrimitiveManagerStaticMeshesBase 0x00BA8C70).
// Shape: no user destructor (no own vtable store); each CVertexShader[4] is the eh-vector destructor loop (`mov edi,4`,
//   pointer walked down by 8); the base destructor is tail-jumped.
struct CPixelShaderData {
    virtual ~CPixelShaderData();                   // slot 0: the deleting destructor is called with 1
    void* m_pShader;                               // +0x04
    long  m_refCount;                              // +0x08
};
struct CVertexShaderData {
    virtual ~CVertexShaderData();                  // slot 0
    unsigned char _pad04[0xc];
    long  m_refCount;                              // +0x10
};

struct CShaderBase {                               // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
struct CPixelShader : CShaderBase {                // vtable 0x0129A054
    CPixelShaderData* Data;                        // +0x04
    __forceinline virtual ~CPixelShader()
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};
struct CVertexShader : CShaderBase {               // vtable 0x0129B3D0
    CVertexShaderData* Data;                       // +0x04
    __forceinline virtual ~CVertexShader()
    {
        CVertexShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};

struct CEnginePrimitiveManagerOcclusionBase {      // 0x20 bytes (landed 0x00B38590)
    virtual ~CEnginePrimitiveManagerOcclusionBase();   // 0x00B38590
    unsigned char _pad04[0x1c];
};

struct CEnginePrimitiveManagerAnimatedMeshesBase : CEnginePrimitiveManagerOcclusionBase {
    unsigned long PointLightShaderIndex[6];                           // +0x20
    CVertexShader VertexShadersPalSkin[4];                            // +0x38
    CVertexShader VertexShadersPalSkinEnv[4];                         // +0x58
    CVertexShader VertexShadersPalSkinEnvBump[4];                     // +0x78
    CVertexShader VertexShadersPalSkinDepthBufferShadow[4];           // +0x98
    CVertexShader VertexShadersPalSkinEnvDepthBufferShadow[4];        // +0xb8
    CVertexShader VertexShadersPalSkinEnvBumpDepthBufferShadow[4];    // +0xd8
    CVertexShader VertexShadersPalSkinDP3BumpDepthBufferShadow[4];    // +0xf8
    CVertexShader VertexShadersPalSkinSpotDepthBufferShadow[4];       // +0x118
    CVertexShader VertexShadersPalSkinEnvSpotDepthBufferShadow[4];    // +0x138
    CVertexShader VertexShadersPalSkinEnvBumpSpotDepthBufferShadow[4];// +0x158
    CVertexShader VertexShadersPalSkinDP3BumpSpotDepthBufferShadow[4];// +0x178
    CVertexShader VertexShadersPalSkinDepthShadowBufferRender;        // +0x198
    CVertexShader VertexShadersPalSkinColourShadowBufferRender;       // +0x1a0
    CVertexShader VertexShadersPalSkinColourBufferShadow[4];          // +0x1a8
    CVertexShader VertexShadersPalSkinEnvColourBufferShadow[4];       // +0x1c8
    CVertexShader VertexShadersPalSkinEnvBumpColourBufferShadow[4];   // +0x1e8
    CVertexShader VertexShadersPalSkinDP3BumpColourBufferShadow[4];   // +0x208
    CVertexShader VertexShadersPalSkinSpotColourBufferShadow[4];      // +0x228
    CVertexShader VertexShadersPalSkinEnvSpotColourBufferShadow[4];   // +0x248
    CVertexShader VertexShadersPalSkinEnvBumpSpotColourBufferShadow[4];// +0x268
    CVertexShader VertexShadersPalSkinDP3BumpSpotColourBufferShadow[4];// +0x288
    CVertexShader VertexShaderPalSkinOutline;                         // +0x2a8
    CVertexShader VertexShaderPalSkinDP3BumpOutline;                  // +0x2b0
    CVertexShader VertexShaderPalSkinOutlineSmoothFill;               // +0x2b8
    CVertexShader VertexShaderPalSkinDP3BumpOutlineSmoothFill;        // +0x2c0
    CVertexShader VertexShaders2DPalSkin;                             // +0x2c8
    CVertexShader VertexShaders2DPalSkinEnv;                          // +0x2d0
    CVertexShader VertexShaders2DPalSkinEnvBump;                      // +0x2d8
    CVertexShader VertexShaders2DPalSkinDP3Bump;                      // +0x2e0
    CVertexShader* VertexShadersPalSkinShadowBufferRender;            // +0x2e8
    CVertexShader* VertexShadersPalSkinShadow;                        // +0x2ec
    CVertexShader* VertexShadersPalSkinEnvShadow;                     // +0x2f0
    CVertexShader* VertexShadersPalSkinEnvBumpShadow;                 // +0x2f4
    CVertexShader* VertexShadersPalSkinDP3BumpShadow;                 // +0x2f8
    CVertexShader* VertexShadersPalSkinSpotShadow;                    // +0x2fc
    CVertexShader* VertexShadersPalSkinEnvSpotShadow;                 // +0x300
    CVertexShader* VertexShadersPalSkinEnvBumpSpotShadow;             // +0x304
    CVertexShader* VertexShadersPalSkinDP3BumpSpotShadow;             // +0x308
    CVertexShader VertexShadersPalSkinSpotlight[4];                   // +0x30c
    float TextureOverlayFadeDistandeFactor;                           // +0x32c
    unsigned char BoneMatrixBuffer[18 * 0x30];   // CPreTransposedBoneMatrix[18]// +0x330
};

void DestroyAnimatedMeshesBase_00bd7ef0(CEnginePrimitiveManagerAnimatedMeshesBase* manager)   // emits the implicit destructor; not part of the retail body
{
    manager->CEnginePrimitiveManagerAnimatedMeshesBase::~CEnginePrimitiveManagerAnimatedMeshesBase();
}

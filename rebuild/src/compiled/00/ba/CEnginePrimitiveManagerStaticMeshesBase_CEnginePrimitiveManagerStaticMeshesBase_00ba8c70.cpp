// CEnginePrimitiveManagerStaticMeshesBase::~CEnginePrimitiveManagerStaticMeshesBase() -- compiler-generated destructor: the outline vertex shaders, the stipple-alpha pixel shader, the colour-buffer shadow CVertexShader[4] tables, the single 2D/shadow-render vertex shaders and the static CVertexShader[4] tables released in reverse declaration order, then the CEnginePrimitiveManagerOcclusionBase destructor (tail jump)
// Leaf: ??1CEnginePrimitiveManagerStaticMeshesBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA8C70, length 1307 bytes (tail `jmp` to the base destructor).
// Callees: ~CEnginePrimitiveManagerOcclusionBase @ 0x00B38590 (landed); the shared shader data's virtual deleting destructor
//   (slot 0, flag 1). Vtables: 0x0129B3D0 CVertexShader, 0x0129A054 CPixelShader, 0x0129A030 CShaderBase.
// Layout: engine/CEnginePrimitiveManagerStaticMeshesBase.h offsets (they hold in retail: the shader members +0x38..+0x2ef are
//   exactly the ones released); the header keeps every shader opaque, so the shader classes are modelled locally as in the
//   landed ~CEnginePrimitiveManagerParticleDecalGroup 0x00BB99A0.
// Shape: no user destructor (no own vtable store); each CVertexShader[4] is the eh-vector destructor loop (`mov ebx,4`,
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

class CShaderBase { public:                               // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
class CPixelShader : public CShaderBase { public:                // vtable 0x0129A054
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
class CVertexShader : public CShaderBase { public:               // vtable 0x0129B3D0
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

struct CEnginePrimitiveManagerStaticMeshesBase : CEnginePrimitiveManagerOcclusionBase {
    unsigned long PointLightShaderIndex[6];                          // +0x20
    CVertexShader VertexShadersStaticSpotlight[4];                    // +0x38
    CVertexShader VertexShadersStaticStippleAlpha[4];                 // +0x58
    CVertexShader VertexShadersStatic[4];                             // +0x78
    CVertexShader VertexShadersStaticEnv[4];                          // +0x98
    CVertexShader VertexShadersStaticEnvBump[4];                      // +0xb8
    CVertexShader VertexShadersStaticDepthBufferShadow[4];            // +0xd8
    CVertexShader VertexShadersStaticEnvDepthBufferShadow[4];         // +0xf8
    CVertexShader VertexShadersStaticEnvBumpDepthBufferShadow[4];     // +0x118
    CVertexShader VertexShadersStaticDP3BumpDepthBufferShadow[4];     // +0x138
    CVertexShader VertexShadersStaticSpotDepthBufferShadow[4];        // +0x158
    CVertexShader VertexShadersStaticEnvSpotDepthBufferShadow[4];     // +0x178
    CVertexShader VertexShadersStaticEnvBumpSpotDepthBufferShadow[4]; // +0x198
    CVertexShader VertexShadersStaticDP3BumpSpotDepthBufferShadow[4]; // +0x1b8
    CVertexShader VertexShaders2DStatic;                              // +0x1d8
    CVertexShader VertexShaders2DStaticEnv;                           // +0x1e0
    CVertexShader VertexShaders2DStaticEnvBump;                       // +0x1e8
    CVertexShader VertexShaders2DStaticDP3Bump;                       // +0x1f0
    CVertexShader VertexShaderDepthShadowBufferRender;                // +0x1f8
    CVertexShader VertexShaderColourShadowBufferRender;               // +0x200
    CVertexShader VertexShadersStaticColourBufferShadow[4];           // +0x208
    CVertexShader VertexShadersStaticEnvColourBufferShadow[4];        // +0x228
    CVertexShader VertexShadersStaticEnvBumpColourBufferShadow[4];    // +0x248
    CVertexShader VertexShadersStaticDP3BumpColourBufferShadow[4];    // +0x268
    CVertexShader VertexShadersStaticSpotColourBufferShadow[4];       // +0x288
    CVertexShader VertexShadersStaticEnvSpotColourBufferShadow[4];    // +0x2a8
    CPixelShader  PixelShaderDiffuseFogStippleAlpha;                  // +0x2c8
    CVertexShader VertexShaderOutline;                                // +0x2d0
    CVertexShader VertexShaderDP3BumpOutline;                         // +0x2d8
    CVertexShader VertexShaderOutlineSmoothFill;                      // +0x2e0
    CVertexShader VertexShaderDP3BumpOutlineSmoothFill;               // +0x2e8
    CVertexShader* VertexShadersStaticShadow;                        // +0x2f0
    CVertexShader* VertexShadersStaticEnvShadow;                     // +0x2f4
    CVertexShader* VertexShadersStaticEnvBumpShadow;                 // +0x2f8
    CVertexShader* VertexShadersStaticDP3BumpShadow;                 // +0x2fc
    CVertexShader* VertexShadersStaticSpotShadow;                    // +0x300
    CVertexShader* VertexShadersStaticEnvSpotShadow;                 // +0x304
    CVertexShader* VertexShaderShadowBufferRender;                   // +0x308
    long           OverridePrimitive;                                // +0x30c
    long           RenderOnlyNMeshes;                                // +0x310
    long           StippleAlphaSolidFactor;                          // +0x314
};

void DestroyStaticMeshesBase_00ba8c70(CEnginePrimitiveManagerStaticMeshesBase* manager)   // emits the implicit destructor; not part of the retail body
{
    manager->CEnginePrimitiveManagerStaticMeshesBase::~CEnginePrimitiveManagerStaticMeshesBase();
}
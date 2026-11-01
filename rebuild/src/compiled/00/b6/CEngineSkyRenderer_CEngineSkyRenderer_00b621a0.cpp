// CEngineSkyRenderer::~CEngineSkyRenderer() -- uninitialises the sky vertex/index buffers, then the members' destructors (def pointer, file-path strings, shaders, buffers) and ~CEngineComponent
// Leaf: ??1CEngineSkyRenderer@@UAE@XZ
// Retail 0x00B621A0, 660 bytes, thiscall. Stores vtable 0x012A293C, calls CVertexBufferWin32::Uninitialise 0x00A63200 and
//   CIndexBuffer::Uninitialise 0x00AC1EE0, then the inline member dtors (CWideString 0x0099B510, shaders, the buffers'
//   own ~T { Uninitialise(); } over ~CBaseClassNonCopyable 0x0099A300) and tail-jumps to ~CEngineComponent 0x00B59730.
// Header debt: CEngineSkyRenderer.h / CEngineComponent.h / CVertexBufferWin32.h / CIndexBuffer.h are flattened, so the
//   bases and buffers are local models with the Ego_r layout.
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CBaseClassNonCopyable.h"

class CEngineComponent {                                         // RTTI .?AVCEngineComponent@@ (no bases), 0x0C bytes
public:
    virtual ~CEngineComponent();                                 // 0x00B59730
    void** GlobalPointerAddress;                                 // +0x04
    bool RenderEnabled;                                          // +0x08
    unsigned char KeepAlive;                                     // +0x09
};

class CShaderBase {                                              // vtable 0x0129A030
public:
    virtual ~CShaderBase() { }
};
class CVertexShaderData {
public:
    virtual ~CVertexShaderData();
    void* VertexDeclaration;
    void* VertexShader;
    const void* FormatDecl;
    long ReferenceCount;                                         // +0x10
    long RemoveReference() { return --ReferenceCount; }
};
class CPixelShaderData {
public:
    virtual ~CPixelShaderData();
    void* PixelShader;
    long ReferenceCount;                                         // +0x08
    long RemoveReference() { return --ReferenceCount; }
};
class CVertexShader : public CShaderBase {                       // vtable 0x0129B3D0, 8 bytes
public:
    CVertexShaderData* Data;
    CVertexShader();                                             // 0x00A62FB0
    virtual ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
};
class CPixelShader : public CShaderBase {                        // vtable 0x0129A054, 8 bytes
public:
    CPixelShaderData* Data;
    CPixelShader();                                              // 0x00A5EC00
    virtual ~CPixelShader() { if (Data) { CPixelShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
};

class CMovableResource {                                         // vtable 0x0129B860
public:
    virtual ~CMovableResource() { }
};
class CIndexBuffer : public CMovableResource {                   // vtable 0x0129EF2C, 0x14 bytes
public:
    void* PD3DIndexBuffer;
    unsigned long ByteLength;
    long PrimitiveType;
    void* PAllocatedBuffer;
    CIndexBuffer();                                              // 0x00AC1EC0
    virtual ~CIndexBuffer() { Uninitialise(); }
    void Uninitialise();                                         // 0x00AC1EE0
};

class CVertexCompressionParams {
public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
};
class CVertexBufferBase : public CBaseClassNonCopyable {         // 0x34 bytes; implicit destructor
public:
    void* PVertexBuffer;                                         // +0x04
    unsigned long Length;                                        // +0x08
    long Type;                                                   // +0x0C
    unsigned char Stride;                                        // +0x10
    CVertexCompressionParams CompressionParams;                  // +0x14
};
class CVertexBufferWin32 : public CVertexBufferBase {            // vtable 0x0129BA20
public:
    CVertexBufferWin32();
    virtual ~CVertexBufferWin32() { Uninitialise(); }
    void Uninitialise();                                         // 0x00A63200
};

class C2DVector {
public:
    float X, Y;
};
class CSkyDef;

class CEngineSkyRenderer : public CEngineComponent {             // vtable 0x012A293C, 0x194 bytes
public:
    virtual ~CEngineSkyRenderer();
    float CloudHeightOffset;                                     // +0x0C
    float CloudTextureOffset;                                    // +0x10
    float CloudTextureMulitplier;                                // +0x14
    float CloudSpeedMultiplier;                                  // +0x18
    float CloudMaxSpeed;                                         // +0x1C
    float CloudMinSpeed;                                         // +0x20
    CVertexBufferWin32 StarFieldVertexBuffer;                    // +0x24
    CVertexBufferWin32 VertexBuffer;                             // +0x58
    CIndexBuffer IndexBuffer;                                    // +0x8C
    CVertexBufferWin32 SolidBaseBandVertexBuffer;                // +0xA0
    CIndexBuffer SolidBaseBandIndexBuffer;                       // +0xD4
    CVertexShader CloudVertexShader;                             // +0xE8
    CVertexShader SkyVertexShader;                               // +0xF0
    CVertexShader StarFieldVertexShader;                         // +0xF8
    CPixelShader StarFieldPixelShader;                           // +0x100
    CVertexShader StarFieldReflectionVertexShader;               // +0x108
    CVertexShader SkySpriteVertexShader;                         // +0x110
    CVertexShader BaseBandVertexShader;                          // +0x118
    CPixelShader CloudPixelShader;                               // +0x120
    CPixelShader SkyPixelShader;                                 // +0x128
    CPixelShader SkySpritePixelShader;                           // +0x130
    CPixelShader BaseBandPixelShader;                            // +0x138
    CVertexShader ScreenSpaceSpriteVertexShader;                 // +0x140
    CPixelShader ScreenSpaceSpritePixelShader;                   // +0x148
    long StarCount;                                              // +0x150
    CWideString StarBitmapFilePath;                              // +0x154
    CWideString StarVBFilePath;                                  // +0x158
    C2DVector CloudVector[2];                                    // +0x15C
    C2DVector CloudVectorHistory[2][2];                          // +0x16C
    bool EnableClouds;                                           // +0x18C
    CDefPointer<const CSkyDef> PSkyDef;                          // +0x190
};

CEngineSkyRenderer::~CEngineSkyRenderer()
{
    VertexBuffer.Uninitialise();
    IndexBuffer.Uninitialise();
}
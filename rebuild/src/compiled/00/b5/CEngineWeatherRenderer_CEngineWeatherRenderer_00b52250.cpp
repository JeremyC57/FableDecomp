// CEngineWeatherRenderer::CEngineWeatherRenderer() -- constructs the weather renderer: rain / snow / mist vertex and index buffers, shader members, settings and the three weather masks, looks up the mist / snow / rain shaders by name, builds the buffers and resets the rain / snow overrides
// Leaf: ??0CEngineWeatherRenderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B52250, length 1472 bytes (plain `ret`, no EH frame).
// Label correction: catalogued "CEnginePrimitiveManagerParticleLineGroup" (propagated); the body calls the CEngineComponent ctor
//   @ 0x00B59710, stores vtable 0x012A2434 and its member offsets are exactly engine/CEngineWeatherRenderer.h (0x1328 bytes).
// Callees: CEngineComponent() @ 0x00B59710, CBaseClassNonCopyable() @ 0x0099A2F0, CIndexBuffer() @ 0x00AC1EC0, CVertexShader() @ 0x00A62FB0,
//   CPixelShader() @ 0x00A5EC00, CEngineWeatherSettings() @ 0x00B50FB0, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//   GetVertexShader @ 0x00A5D5F0 / GetPixelShader @ 0x00A5D720, BuildBuffers (this) @ 0x00B51570.
// Layout: engine/CEngineWeatherRenderer.h offsets hold, but its members are opaque byte arrays; restated locally with the real
//   buffer / shader types (models as the landed 0x00BABB50). CWeatherMask's implicit ctor only zeroes its C3DBoundingSphere, which
//   VC7.1 emits as a fully unrolled 3-element loop.
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); the vertex shader resources are
// CEngineResourceManager::VShaderResources[bank] (retail +0x29C0 + 8*bank, FableWin.pdb inline GetVShaderResource) and the pixel one
// ShaderResourcePixel (+0x2A98, GetPShaderResource). Shader classes from the shared headers (CShaderResource::GetVertexShader /
// GetPixelShader const, Ego_r.pdb / FableWin.pdb); were a local CEngineData / g_pEngineData stand-in and local shader models.
#define FABLE_RETAIL_CENGINECOMPONENT_PROTECTED
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADER
#include "fable_string.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};
struct IDirect3DVertexBuffer9;
class CVertexCompressionParams { public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
struct CVertexBufferBase : CBaseClassNonCopyable {                 // 0x34 bytes
    IDirect3DVertexBuffer9*  PVertexBuffer;        // +0x04
    unsigned long            Length;               // +0x08
    long                     Type;                 // +0x0c
    unsigned char            Stride;               // +0x10
    CVertexCompressionParams CompressionParams;    // +0x14
    CVertexBufferBase() : Length(0), Type(0), CompressionParams() { }
};
class CVertexBufferWin32 : public CVertexBufferBase { public:    // vtable 0x0129BA20
    virtual void VSlot1();
    CVertexBufferWin32()
    {
        PVertexBuffer = 0;
        Stride = 0;
    }
};
class CIndexBuffer { public:                              // 0x14 bytes
    void* _vtable;
    unsigned char _body[0x10];
    CIndexBuffer();                                // 0x00AC1EC0
};
struct CEngineWeatherSettings {                    // 0x558 bytes
    unsigned char _body[0x558];
    CEngineWeatherSettings();                      // 0x00B50FB0
};
class C3DBoundingSphere { public:
    float X, Y, Z, Radius;
    C3DBoundingSphere() : X(0.0f), Y(0.0f), Z(0.0f), Radius(0.0f) { }
};
class CWeatherMask { public:                              // 0x5c bytes
    C3DBoundingSphere BSphere;                     // +0x00
    unsigned char     BBox[0x18];                  // +0x10 C3DBoundingBox
    unsigned char     InvTransform[0x30];          // +0x28 CMatrix3x4
    float             Scale;                       // +0x58
};
class CVolumeTexture;

class CEngineWeatherRenderer : public CEngineComponent { public: // vtable 0x012A2434, 0x1328 bytes
    CVertexBufferWin32 RainVertexBuffer;           // +0x0c
    CIndexBuffer       RainIndexBuffer;            // +0x40
    CVertexBufferWin32 SnowVertexBuffer;           // +0x54
    CVertexBufferWin32 MistVertexBuffer;           // +0x88
    CIndexBuffer       MistIndexBuffer;            // +0xbc
    CVertexShader      RainParticleVertexShader;   // +0xd0
    CVertexShader      RainParticleClippingVertexShader;   // +0xd8
    CPixelShader       RainParticlePixelShader;    // +0xe0
    CVertexShader      SnowParticleVertexShader;   // +0xe8
    CVertexShader      SnowParticleClippingVertexShader;   // +0xf0
    CPixelShader       SnowParticlePixelShader;    // +0xf8
    CVertexShader      MistVertexShader;           // +0x100
    CPixelShader       MistPixelShader;            // +0x108
    CVolumeTexture*    MistVolumeTexture;          // +0x110
    CEngineWeatherSettings Settings;               // +0x114
    CWeatherMask       WeatherMaskList[3];         // +0x66c
    long               CurrentMask;                // +0x780
    unsigned char      SavedBoxVisibility[0xb64];  // +0x784 EWeatherBoxVisible[729]
    float              OverridenRainStrength;      // +0x12e8
    bool               RainOverriden;              // +0x12ec
    float              OverridenSnowStrength;      // +0x12f0
    bool               SnowOverriden;              // +0x12f4
    float              MaximumWeatherParticlePercentage;   // +0x12f8
    unsigned char      _debug[0x1328 - 0x12fc];    // +0x12fc debug switches and colours
    CEngineWeatherRenderer();
    virtual ~CEngineWeatherRenderer();
    private: void BuildBuffers(); public:                           // 0x00B51570
};

CEngineWeatherRenderer::CEngineWeatherRenderer()
{
    MistVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)10)->GetVertexShader("VSHADER_MIST");
    MistPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_MIST");
    SnowParticleVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)10)->GetVertexShader("VSHADER_WEATHER_SNOW");
    SnowParticleClippingVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)10)->GetVertexShader("VSHADER_WEATHER_SNOW_CLIPPING");
    SnowParticlePixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_WEATHER_SNOW");
    RainParticleVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)10)->GetVertexShader("VSHADER_WEATHER_RAIN");
    RainParticleClippingVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)10)->GetVertexShader("VSHADER_WEATHER_RAIN_CLIPPING");
    RainParticlePixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_WEATHER_RAIN");
    BuildBuffers();
    OverridenRainStrength = -1.0f;
    OverridenSnowStrength = -1.0f;
    CurrentMask = 0;
    RainOverriden = false;
    SnowOverriden = false;
    MaximumWeatherParticlePercentage = 1.0f;
}
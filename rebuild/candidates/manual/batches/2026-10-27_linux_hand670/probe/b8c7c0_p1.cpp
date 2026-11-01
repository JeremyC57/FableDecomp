// CEngineBillboardGenerator::~CEngineBillboardGenerator() -- billboard generator destructor (compiler-generated): the PixelShader and VertexShader members' destructors (each drops its shader data reference, then the CShaderBase vtable), then the CEngineComponent base at +4 and the CEnginePrimitiveRenderUtil / CBaseClassNonCopyable base
// Leaf: ??1CEngineBillboardGenerator@@UAE@XZ
#include "engine/CBaseClassNonCopyable.h"
struct IDirect3DPixelShader9;
struct IDirect3DVertexShader9;
struct IDirect3DVertexDeclaration9;
struct _D3DVERTEXELEMENT9;
class CShaderBase {
protected:
    CShaderBase() { }
    virtual ~CShaderBase() { }                               // ??1CShaderBase@@MAE@XZ
};
class CPixelShaderData {
    friend class CPixelShader;
    IDirect3DPixelShader9* PixelShader;                      // +0x04
    unsigned long          ReferenceCount;                   // +0x08
    unsigned long          ConstCount;                       // +0x0c
    long*                  ConstIndices;                     // +0x10
    virtual ~CPixelShaderData();                             // ??1CPixelShaderData@@EAE@XZ
    void Release() { if (--ReferenceCount == 0) delete this; }   // ?Release@CPixelShaderData@@AAEXXZ
};
class CVertexShaderData {
    friend class CVertexShader;
    IDirect3DVertexDeclaration9* VertexDeclaration;          // +0x04
    IDirect3DVertexShader9*      VertexShader;               // +0x08
    _D3DVERTEXELEMENT9*          FormatDecl;                 // +0x0c
    unsigned long                ReferenceCount;             // +0x10
    virtual ~CVertexShaderData();                            // ??1CVertexShaderData@@EAE@XZ
    void Release() { if (--ReferenceCount == 0) delete this; }   // ?Release@CVertexShaderData@@AAEXXZ
};
class CPixelShader : public CShaderBase {
public:
    CPixelShader() : Data(0) { }
    virtual ~CPixelShader() { if (Data) Data->Release(); }   // ??1CPixelShader@@UAE@XZ
    CPixelShaderData* Data;                                  // +0x04
};
class CVertexShader : public CShaderBase {
public:
    CVertexShader() : Data(0) { }
    virtual ~CVertexShader() { if (Data) Data->Release(); }  // ??1CVertexShader@@UAE@XZ
    CVertexShaderData* Data;                                 // +0x04
};
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {
public:
    CEnginePrimitiveRenderUtil() { }
};
class CEngineComponent {
public:
    CEngineComponent();
    virtual ~CEngineComponent();                             // 0x00B59730
    void**        GlobalPointerAddress;                      // +0x04
    bool          RenderEnabled;                             // +0x08
    unsigned char KeepAlive;                                 // +0x09
};
class CEngineBillboardGenerator : public CEnginePrimitiveRenderUtil, public CEngineComponent {
public:
    CEngineBillboardGenerator();
    CVertexShader VertexShader;                              // +0x10
    CPixelShader  PixelShader;                               // +0x18
};
CEngineBillboardGenerator::CEngineBillboardGenerator()       // ??0CEngineBillboardGenerator@@QAE@XZ (emits the vtable, ??_G and the implicit ??1)
{
}

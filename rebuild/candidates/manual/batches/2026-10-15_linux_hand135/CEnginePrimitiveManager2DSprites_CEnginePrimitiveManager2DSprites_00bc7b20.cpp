// CEnginePrimitiveManager2DSprites::~CEnginePrimitiveManager2DSprites() -- releases the two pixel shaders and two vertex shaders (reverse declaration order), then the CBase destructor
// Leaf: ??1CEnginePrimitiveManager2DSprites
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7B20, length 130 bytes (a byte-identical twin of 0x00BB1900, another translation unit).
// Callees: the shader objects' virtual deleting destructors (slot 0, `push 1`) at zero count, CBase::~CBase @ 0x0099A300 (tail
// `jmp`). Vtables (rtti-vtables.tsv): 0x0129A054 = CPixelShader, 0x0129B3D0 = CVertexShader, 0x0129A030 = CShaderBase.
// Layout: the generated engine/CEnginePrimitiveManager2DSprites.h matches retail -- VertexShader2DSprite +0x0C,
// VertexShader2DClockSprite +0x14, PixelShader2DClockSprite +0x1C, PixelShader2DClockSpriteAdditive +0x24, each an 8-byte
// shader wrapper {vptr, counted shader object} -- but a destructor cannot be declared on it, so it is restated locally.
// Shape: each wrapper's inline destructor stores its own vtable, releases its object through a LOCAL copy of the pointer (`dec [ecx+N] ; jne`; count at +8 for pixel shaders, +0x10
// for vertex shaders; `delete` on the known-non-null object needs no second test) and the inline CShaderBase destructor
// stores the base vtable (kept in edi). The manager is `__declspec(novtable)` (no vtable store of its own) over the
// out-of-line CBase destructor.
struct CBase {
    virtual ~CBase();                              // 0x0099A300
};

struct CPixelShaderObject {
    virtual ~CPixelShaderObject();                 // slot 0
    long m_unknown04;
    long m_refCount;                               // +0x08
};

struct CVertexShaderObject {
    virtual ~CVertexShaderObject();                // slot 0
    long m_unknown04[3];
    long m_refCount;                               // +0x10
};

struct CShaderBase {                               // vtable 0x0129A030
    virtual ~CShaderBase() { }
};

struct CPixelShader : CShaderBase {                // vtable 0x0129A054, 8 bytes
    CPixelShaderObject* m_pShader;
    virtual ~CPixelShader()
    {
        CPixelShaderObject* shader = m_pShader;
        if (shader) {
            if (--shader->m_refCount == 0)
                delete shader;
        }
    }
};

struct CVertexShader : CShaderBase {               // vtable 0x0129B3D0, 8 bytes
    CVertexShaderObject* m_pShader;
    virtual ~CVertexShader()
    {
        CVertexShaderObject* shader = m_pShader;
        if (shader) {
            if (--shader->m_refCount == 0)
                delete shader;
        }
    }
};

struct __declspec(novtable) CEnginePrimitiveManager2DSprites : CBase {
    unsigned char  _pad_0x04[0x4];                 // +0x04
    unsigned char  active;                         // +0x08
    unsigned char  _pad_0x09[0x3];                 // +0x09
    CVertexShader  VertexShader2DSprite;           // +0x0C
    CVertexShader  VertexShader2DClockSprite;      // +0x14
    CPixelShader   PixelShader2DClockSprite;       // +0x1C
    CPixelShader   PixelShader2DClockSpriteAdditive;   // +0x24
    unsigned short Indices[6];                     // +0x2C
    virtual ~CEnginePrimitiveManager2DSprites();
};

CEnginePrimitiveManager2DSprites::~CEnginePrimitiveManager2DSprites()
{
}

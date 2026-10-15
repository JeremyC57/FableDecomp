// CPixelShader::CPixelShader(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString& name) -- allocate the shared CPixelShaderData (0x14 bytes) from the arguments and keep it at +4
// Leaf: ??0CPixelShader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5ECB0, length 65 bytes.
// Vtables: CPixelShader 0x0129A054 (the CShaderBase base's own store is dead and dropped).
// Callees: operator new @ 0x00BFEA1A (`add esp,4`), CPixelShaderData::CPixelShaderData(
// IDirect3DPixelShader9*, const CArray<long>&, const CCharString&) @ 0x00A5EC40.
// Shape: `Data(new CPixelShaderData(...))` in the initialiser list: the vtable store is scheduled
// before the allocation, the null-checked placement constructor call reloads the three argument
// slots, and the null arm stores the zeroed eax. No EH frame (the module is built without /EHsc).
// PDB: CPixelShader = vptr, Data +4; CPixelShaderData is 0x14 bytes (vptr, PixelShader,
// ReferenceCount, ConstCount, ConstIndices).
struct IDirect3DPixelShader9;
struct CCharString { void* m_pData; };
template <class T> struct CArray { T* m_pFirst; T* m_pLast; T* m_pEnd; };

struct CPixelShaderData {                          // 0x14 bytes, vtable 0x0129D58C
    virtual ~CPixelShaderData();                   // +0x00 vptr
    IDirect3DPixelShader9* PixelShader;            // +0x04
    unsigned long          ReferenceCount;         // +0x08
    unsigned long          ConstCount;             // +0x0c
    long*                  ConstIndices;           // +0x10
    CPixelShaderData(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString& name);   // 0x00A5EC40
};

struct CShaderBase {                               // vtable 0x0129A030
    virtual void v00();
    CShaderBase() { }
};

struct CPixelShader : CShaderBase {                // vtable 0x0129A054
    CPixelShaderData* Data;                        // +0x04
    virtual void v00();
    CPixelShader(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString& name);
};

CPixelShader::CPixelShader(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString& name)
    : Data(new CPixelShaderData(shader, constIndices, name))
{
}
// CShaderResource::GetPixelShader(unsigned long index) -- the pixel shader of the bank entry: the cached one when present, else create it from the bank's shader data and entry name, cache a heap copy and return it
// Leaf: GetPixelShader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5D620, length 251 bytes (`ret 8`: CPixelShader by hidden pointer + index).
// Callees: CShaderDataBank shader-data lookup @ 0x00A61FB0 (manifest "CTCInventoryAbilities::GetAbilityLevel" is propagated),
// CBankFile::GetEntrySymbol(unsigned long) @ 0x009CC1D0 (CCharString by hidden pointer, built in the dead `index` slot),
// CShaderRenderManager::CreatePixelShader(const CArray<unsigned long>&, const CArray<CCharString>&, const CCharString&)
// @ 0x0098A9A0 (CPixelShader by hidden pointer), operator new @ 0x00BFEA1A (8 bytes), ~CCharString @ 0x0099EAE0; the shader
// data's virtual deleting destructor (slot 0, `push 1`) when its count reaches zero.
// Vtables: 0x0129A054 CPixelShader, 0x0129A030 CShaderBase (rtti-vtables.tsv).
// Layout: Ego_r.pdb CShaderResource with the retail 12-byte CArray: ShaderBank +4, Shaders (CArray<CShaderBase*>) +8,
// ShaderRenderManager +0x14. CPixelShader = {vptr, counted shader data}; the data's count sits at +8 behind its vptr and +4.
// Shape: the cached path returns a copy (inline user copy ctor: vtable, data, add-ref); the build path keeps the created
// shader in an 8-byte local, copies it into `new CPixelShader` (the data re-read from the address-taken local) and into the
// result, then runs the local's inline destructor (own vtable, the data's inline Release() over the ref base at +8 with the
// virtual deleting destructor, then CShaderBase's vtable) and the name's destructor.
void* __cdecl operator new(unsigned int size);       // 0x00BFEA1A

class CCharString { public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};
template<class T> class CArray { public: T* m_pFirst; T* m_pLast; T* m_pEnd; };

struct CRefCount {
    long m_count;
    void AddRef() { ++m_count; }
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CShaderDataRoot {                             // polymorphic root of the shared shader data
    virtual ~CShaderDataRoot();
    void* m_pDevice;                                 // +0x04
};
struct CShaderData : CShaderDataRoot, CRefCount {    // count at +8
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            delete this;
    }
};

class CShaderBase { public:                                 // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
class CPixelShader : public CShaderBase { public:                  // vtable 0x0129A054, 8 bytes
    CShaderData* Data;                               // +0x04
    CPixelShader(const CPixelShader& other) : Data(other.Data)
    {
        if (Data)
            Data->AddRef();
    }
    virtual ~CPixelShader()
    {
        if (Data)
            Data->Release();
    }
};

struct CPixelShaderData {                            // the bank's per-shader data
    unsigned char             _pad00[8];
    CArray<unsigned long>     Code;                  // +0x08
    CArray<CCharString>       Constants;             // +0x14
};
class CBankFile
{
public:
    CCharString GetEntrySymbol(unsigned long) const;   // 0x009CC1D0
};

class CShaderDataBank : public CBankFile { public:
    CPixelShaderData* GetShaderData(unsigned long index);   // 0x00A61FB0
};
class CShaderRenderManager { public:
    CPixelShader CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constants,
                                   const CCharString& name);   // 0x0098A9A0
};

class CShaderResource { public:
    void*                  __vftable;                // +0x00
    CShaderDataBank*       ShaderBank;               // +0x04
    CArray<CShaderBase*>   Shaders;                  // +0x08
    CShaderRenderManager*  ShaderRenderManager;      // +0x14
    CPixelShader GetPixelShader(unsigned long index);
};

CPixelShader CShaderResource::GetPixelShader(unsigned long index)
{
    if (Shaders.m_pFirst[index])
        return *static_cast<CPixelShader*>(Shaders.m_pFirst[index]);
    CPixelShaderData* data = ShaderBank->GetShaderData(index);
    CCharString name = ShaderBank->GetEntrySymbol(index);
    CPixelShader shader = ShaderRenderManager->CreatePixelShader(data->Code, data->Constants, name);
    Shaders.m_pFirst[index] = new CPixelShader(shader);
    return shader;
}
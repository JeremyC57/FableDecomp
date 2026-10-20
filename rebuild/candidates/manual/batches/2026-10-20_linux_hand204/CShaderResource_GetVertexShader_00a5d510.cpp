// CShaderResource::GetVertexShader(unsigned long index) const -- the vertex shader for a bank entry, created and cached on first use (code, vertex declaration, entry symbol and constant layout from the bank), returned as a counted handle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5D510, 213 bytes (+ int3 padding), `ret 8` (hidden result + index). Ego_r.pdb
//   `?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@K@Z`.
// Leaf: ?GetVertexShader@CShaderResource@@QBE?AVCVertexShader@@K@Z
// Callees: CShaderDataBank::GetVertexShader(unsigned long) const @ 0x00A61FA0 (PDB; CShaderBankEntry*), operator new @ 0x00BFEA1A,
//   CCharString(const CCharString&) @ 0x0099EC30, CBankFile::GetEntrySymbol(unsigned long) const @ 0x009CC1D0 (by value; landed
//   name), CVertexShader::CVertexShader(const CArray<unsigned long>&, const _D3DVERTEXELEMENT9*, const CCharString&,
//   const CCharString&) @ 0x00A63020 (PDB, private), ~CCharString @ 0x0099EAE0.
// Shape: the cached handle is copied out through CVertexShader's inline copy constructor (vtable 0x0129B3D0, Data, add-ref
//   of Data's count at +0x10), the two returns sharing that tail. On a miss the constructor arguments are evaluated right to
//   left inside the `new` null test: the layout name through an inline by-value getter (its copy lands in the dead `index`
//   slot), then the entry symbol (a by-value result in the frame); the conditional temporaries carry the construction-flag
//   word (zeroed at entry, set to 3, tested bit by bit for the destructors).
// Retail layout: ShaderBank +0x04, Shaders +0x08 (12-byte vector of CShaderBase*; mutable cache), VertexShaderDeclaration
//   +0x34 (Ego_r.pdb +0x40: the vertex-input block's containers are smaller); CShaderBankEntry ShaderCode +0x08,
//   VSConstantLayout +0x20 (PDB +0x28).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
template <class T> struct CArray {                      // retail CArray (STLport vector): 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
struct _D3DVERTEXELEMENT9;

class CShaderBankEntry {
public:
    void*                   __vftable;                  // +0x00
    unsigned long           ShaderType;                 // +0x04
    CArray<unsigned long>   ShaderCode;                 // +0x08
    CArray<CCharString>     ShaderConstants;            // +0x14
    CCharString             VSConstantLayout;           // +0x20
    CCharString GetVSConstantLayout() const { return VSConstantLayout; }
};
class CShaderDataBank {
public:
    CShaderBankEntry* GetVertexShader(unsigned long index) const;   // 0x00A61FA0
    CCharString GetEntrySymbol(unsigned long index) const;          // 0x009CC1D0 (CBankFile)
};

struct CVertexShaderData {
    unsigned char _pad[0x10];
    long          RefCount;                             // +0x10
};
class CShaderBase {
public:
    virtual ~CShaderBase();
};
class CVertexShader : public CShaderBase {              // vtable 0x0129B3D0, 8 bytes
public:
    CVertexShaderData* Data;                            // +0x04
    CVertexShader(const CArray<unsigned long>& code, const _D3DVERTEXELEMENT9* declaration,
                  const CCharString& name, const CCharString& constantLayout);   // 0x00A63020
    CVertexShader(const CVertexShader& other) : Data(other.Data)
    {
        if (Data)
            ++Data->RefCount;
    }
};

class CShaderResource {
public:
    void*                        __vftable;             // +0x00
    const CShaderDataBank*       ShaderBank;            // +0x04
    mutable CArray<CShaderBase*> Shaders;               // +0x08
    unsigned char                _pad14[0x34 - 0x14];
    const _D3DVERTEXELEMENT9*    VertexShaderDeclaration;   // +0x34
    CVertexShader GetVertexShader(unsigned long index) const;
};

CVertexShader CShaderResource::GetVertexShader(unsigned long index) const
{
    CShaderBase* shader = Shaders[index];
    if (shader)
        return *static_cast<CVertexShader*>(shader);
    CShaderBankEntry* entry = ShaderBank->GetVertexShader(index);
    CVertexShader* created = new CVertexShader(entry->ShaderCode, VertexShaderDeclaration,
                                               ShaderBank->GetEntrySymbol(index), entry->GetVSConstantLayout());
    Shaders[index] = created;
    return *created;
}

// Behaviour test for CShaderResource::GetVertexShader (0x00A5D510): a cached shader is returned add-referenced; a miss builds it from the bank entry (code, declaration, symbol, layout), caches it and returns it.
#include <cstdio>
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
static int g_live = 0, g_built = 0; static const void* g_code; static const void* g_decl; static void* g_name; static void* g_layout;
CCharString::CCharString(const CCharString& o) { ++g_live; PStringData = o.PStringData; }
CCharString::~CCharString() { --g_live; }
static unsigned char g_entryMem[sizeof(CShaderBankEntry)];
#define g_entry (*(CShaderBankEntry*)g_entryMem)
static unsigned long g_lookups[4]; static int g_nlook = 0;
CShaderBankEntry* CShaderDataBank::GetVertexShader(unsigned long index) const { g_lookups[g_nlook++] = index; return &g_entry; }
static char g_symbolText[] = "VS_SKIN";
CCharString CShaderDataBank::GetEntrySymbol(unsigned long) const { CCharString s(g_entry.VSConstantLayout); s.PStringData = g_symbolText; return s; }
CShaderBase::~CShaderBase() { }
static CVertexShaderData g_data;
CVertexShader::CVertexShader(const CArray<unsigned long>& code, const _D3DVERTEXELEMENT9* decl, const CCharString& name, const CCharString& layout)
{ ++g_built; g_code = &code; g_decl = decl; g_name = name.PStringData; g_layout = layout.PStringData; Data = &g_data; g_data.RefCount = 1; }
int main()
{
    static char layoutText[] = "LAYOUT"; g_entry.VSConstantLayout.PStringData = layoutText;
    CShaderDataBank bank; CShaderBase* table[3] = { 0, 0, 0 };
    static unsigned char mem[sizeof(CShaderResource)]; CShaderResource* r = (CShaderResource*)mem;
    r->ShaderBank = &bank; r->Shaders._M_start = table; r->Shaders._M_finish = table + 3; r->VertexShaderDeclaration = (const _D3DVERTEXELEMENT9*)&bank;
    bool ok;
    {
        CVertexShader first = r->GetVertexShader(2);
        ok = g_built == 1 && g_nlook == 1 && g_lookups[0] == 2 && table[2] != 0 && first.Data == &g_data && g_data.RefCount == 2
            && g_code == &g_entry.ShaderCode && g_decl == (const void*)&bank && g_name == g_symbolText && g_layout == layoutText;
    }
    ok = ok && g_live == 0;
    {
        CVertexShader again = r->GetVertexShader(2);                  /* cached: no second build */
        ok = ok && g_built == 1 && g_nlook == 1 && again.Data == &g_data && g_data.RefCount == 3;
    }
    if (!ok) { std::printf("00a5d510_TEST FAIL\n"); return 1; }
    std::printf("00a5d510_TEST PASS\n");
    return 0;
}

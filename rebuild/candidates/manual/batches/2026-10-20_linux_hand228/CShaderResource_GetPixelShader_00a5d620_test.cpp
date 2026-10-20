#include <cstdio>
// Behaviour model of CShaderResource::GetPixelShader @ 0x00A5D620 (copy of the source logic, stand-in callees).
static int g_dataDeleted = 0, g_creates = 0;
struct CCharString { const char* text; };
template<class T> struct CArray { T* m_pFirst; T* m_pLast; T* m_pEnd; };
struct CRefCount { long m_count; void AddRef() { ++m_count; } void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
struct CShaderDataRoot { virtual ~CShaderDataRoot() { ++g_dataDeleted; } };
struct CShaderData : CShaderDataRoot, CRefCount { void Release() { DecRef(); if (GetRefCount() == 0) delete this; } };
struct CShaderBase { virtual ~CShaderBase() {} };
struct CPixelShader : CShaderBase {
    CShaderData* Data;
    explicit CPixelShader(CShaderData* d) : Data(d) {}
    CPixelShader(const CPixelShader& o) : Data(o.Data) { if (Data) Data->AddRef(); }
    virtual ~CPixelShader() { if (Data) Data->Release(); }
};
struct CPixelShaderData { CArray<unsigned long> Code; CArray<CCharString> Constants; };
static CPixelShaderData g_bankData;
struct CShaderDataBank {
    CPixelShaderData* GetShaderData(unsigned long) { return &g_bankData; }
    CCharString GetEntrySymbol(unsigned long) { CCharString s = { "PS_WATER" }; return s; }
};
struct CShaderRenderManager {
    CPixelShader CreatePixelShader(const CArray<unsigned long>&, const CArray<CCharString>&, const CCharString&)
    {
        ++g_creates; CShaderData* d = new CShaderData; d->m_count = 1; return CPixelShader(d);
    }
};
struct CShaderResource {
    CShaderDataBank* ShaderBank; CArray<CShaderBase*> Shaders; CShaderRenderManager* ShaderRenderManager;
    CPixelShader GetPixelShader(unsigned long index)
    {
        if (Shaders.m_pFirst[index])
            return *static_cast<CPixelShader*>(Shaders.m_pFirst[index]);
        CPixelShaderData* data = ShaderBank->GetShaderData(index);
        CCharString name = ShaderBank->GetEntrySymbol(index);
        CPixelShader shader = ShaderRenderManager->CreatePixelShader(data->Code, data->Constants, name);
        Shaders.m_pFirst[index] = new CPixelShader(shader);
        return shader;
    }
};
int main()
{
    CShaderBase* slots[3] = { 0, 0, 0 };
    CShaderDataBank bank; CShaderRenderManager mgr;
    CShaderResource r; r.ShaderBank = &bank; r.ShaderRenderManager = &mgr;
    r.Shaders.m_pFirst = slots; r.Shaders.m_pLast = slots + 3; r.Shaders.m_pEnd = slots + 3;
    int ok = 1;
    {
        CPixelShader a = r.GetPixelShader(1);
        CShaderData* d = a.Data;
        ok = ok && g_creates == 1 && slots[1] != 0 && static_cast<CPixelShader*>(slots[1])->Data == d && d->m_count == 2;
        CPixelShader b = r.GetPixelShader(1);                 // cached
        ok = ok && g_creates == 1 && b.Data == d && d->m_count == 3;
    }
    CShaderData* d = static_cast<CPixelShader*>(slots[1])->Data;
    ok = ok && d->m_count == 1 && g_dataDeleted == 0;         // only the cache holds it now
    delete slots[1];
    ok = ok && g_dataDeleted == 1;
    if (!ok) { std::printf("00a5d620_TEST FAIL\n"); return 1; }
    std::printf("00a5d620_TEST PASS\n");
    return 0;
}

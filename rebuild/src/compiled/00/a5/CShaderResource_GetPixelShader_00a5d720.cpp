struct CCharString;
struct CPixelShader;

struct CShaderResourceField4 {
    int ResolveHandle(CPixelShader* p);
};

struct CShaderResource {
    void* field0;
    CShaderResourceField4* ShaderBank;
    CCharString* GetPixelShader(CCharString* a, CPixelShader* b);
    void Fetch(CCharString* a, int handle);
};

CCharString* CShaderResource::GetPixelShader(CCharString* a, CPixelShader* b)
{
    int handle = this->ShaderBank->ResolveHandle(b);
    this->Fetch(a, handle);
    return a;
}
class CCharString;
class CPixelShader;

struct CShaderResourceField4 {
    int ResolveHandle(CPixelShader* p);
};

class CShaderResource { public:
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
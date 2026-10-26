#include <cstdio>

class CCharString { public: int v; };
class CPixelShader { public: int id; };

static int g_calls_sub1 = 0;
static int g_calls_sub2 = 0;
static int g_last_handle = 0;

struct CShaderResourceField4 {
    int base;
    int ResolveHandle(CPixelShader* p) { g_calls_sub1++; return base + p->id; }
};

class CShaderResource { public:
    void* field0;
    CShaderResourceField4* ShaderBank;
    CCharString* GetPixelShader(CCharString* a, CPixelShader* b);
    void Fetch(CCharString* a, int handle) { g_calls_sub2++; g_last_handle = handle; a->v = handle; }
};

CCharString* CShaderResource::GetPixelShader(CCharString* a, CPixelShader* b)
{
    int handle = this->ShaderBank->ResolveHandle(b);
    this->Fetch(a, handle);
    return a;
}

int main() {
    CShaderResourceField4 f; f.base = 100;
    CShaderResource r; r.field0 = 0; r.ShaderBank = &f;
    CCharString a; a.v = -1;
    CPixelShader b; b.id = 7;
    CCharString* ret = r.GetPixelShader(&a, &b);
    if (ret == &a && g_last_handle == 107 && a.v == 107 && g_calls_sub1 == 1 && g_calls_sub2 == 1)
        printf("PARITY_OK\n");
    else
        printf("FAIL v=%d h=%d\n", a.v, g_last_handle);
    return 0;
}
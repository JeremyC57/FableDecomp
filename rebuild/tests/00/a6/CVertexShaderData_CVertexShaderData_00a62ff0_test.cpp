// Behaviour test for 0x00A62FF0 ~CVertexShaderData: release VertexShader, then VertexDeclaration (each only when set), then ~CBaseClass.
#include <cstdio>
#include <cstring>
static char g_log[32];
static void Log(const char* s) { std::strcat(g_log, s); }
struct IUnk { const char* Tag; int Refs; IUnk(const char* t) : Tag(t), Refs(1) { } unsigned long Release() { Log(Tag); return --Refs; } };
class CBaseClass { public: virtual ~CBaseClass() { Log("B"); } };
class CVertexShaderData : public CBaseClass {
public:
    CVertexShaderData(IUnk* shader, IUnk* decl) : VertexShader(shader), VertexDeclaration(decl) { }
    virtual ~CVertexShaderData()
    {
        if (VertexShader) { VertexShader->Release(); VertexShader = 0; }
        if (VertexDeclaration) { VertexDeclaration->Release(); VertexDeclaration = 0; }
    }
    IUnk* VertexShader;
    IUnk* VertexDeclaration;
};
int main()
{
    IUnk s("S"), d("D");
    g_log[0] = 0;
    CBaseClass* p = new CVertexShaderData(&s, &d);
    delete p;
    bool ok = std::strcmp(g_log, "SDB") == 0 && s.Refs == 0 && d.Refs == 0;
    g_log[0] = 0;
    p = new CVertexShaderData(0, &d);
    delete p;
    ok = ok && std::strcmp(g_log, "DB") == 0;
    std::printf(ok ? "00a62ff0_TEST PASS\n" : "00a62ff0_TEST FAIL\n");
    return ok ? 0 : 1;
}
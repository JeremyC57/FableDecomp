// Behaviour test for CEnginePrimitiveManagerText::CEnginePrimitiveManagerText @ 0x00BC6B50
#include <cstdio>
#include <cstring>
struct ShaderData { int Id; long ReferenceCount; };
struct Shader {
    ShaderData* Data;
    Shader() : Data(0) { }
    Shader(const Shader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }
    Shader& operator=(const Shader& other)
    {
        if (Data) --Data->ReferenceCount;
        Data = other.Data;
        ++Data->ReferenceCount;
        return *this;
    }
    ~Shader() { if (Data) --Data->ReferenceCount; }
};
static ShaderData g_data[4];
static const char* g_names[4] = { "VSHADER_TEXT", "VSHADER_TEXT_SHADOW", "PSHADER_TEXT", "PSHADER_TEXT_SHADOW" };
static int g_lookups = 0;
struct Resource {
    Shader Get(const char* name)
    {
        ++g_lookups;
        Shader s;
        for (int i = 0; i < 4; ++i)
            if (std::strcmp(name, g_names[i]) == 0) { s.Data = &g_data[i]; ++g_data[i].ReferenceCount; }
        return s;
    }
};
struct Manager {
    Shader VertexShaderText, VertexShaderTextShadow, PixelShaderText, PixelShaderTextShadow;
    Manager(Resource* vs, Resource* ps)
    {
        VertexShaderText = vs->Get("VSHADER_TEXT");
        VertexShaderTextShadow = vs->Get("VSHADER_TEXT_SHADOW");
        PixelShaderText = ps->Get("PSHADER_TEXT");
        PixelShaderTextShadow = ps->Get("PSHADER_TEXT_SHADOW");
    }
};
int main()
{
    for (int i = 0; i < 4; ++i) { g_data[i].Id = i; g_data[i].ReferenceCount = 1; }
    Resource vs, ps;
    bool ok = true;
    {
        Manager m(&vs, &ps);
        ok = ok && g_lookups == 4;
        ok = ok && m.VertexShaderText.Data == &g_data[0] && m.VertexShaderTextShadow.Data == &g_data[1];
        ok = ok && m.PixelShaderText.Data == &g_data[2] && m.PixelShaderTextShadow.Data == &g_data[3];
        for (int i = 0; i < 4; ++i) ok = ok && g_data[i].ReferenceCount == 2;   // resource + member
    }
    for (int i = 0; i < 4; ++i) ok = ok && g_data[i].ReferenceCount == 1;
    if (ok) std::printf("00bc6b50_TEST PASS\n"); else std::printf("00bc6b50_TEST FAIL\n");
    return ok ? 0 : 1;
}

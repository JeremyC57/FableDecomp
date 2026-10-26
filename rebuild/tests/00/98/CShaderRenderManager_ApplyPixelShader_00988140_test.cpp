// Behaviour test for CShaderRenderManager::ApplyPixelShader (0x00988140): a different shader becomes current (old released, flags |= 3); the same one is a no-op.
#include <cstdio>
static int g_deleted = 0;
struct CPixelShaderData {
    unsigned long ReferenceCount;
    virtual ~CPixelShaderData() { ++g_deleted; }
};
class CPixelShader { public:
    CPixelShaderData* Data;
    virtual ~CPixelShader() {}
    void Assign(const CPixelShader& other)
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
            Data = 0;
        }
        Data = other.Data;
        ++Data->ReferenceCount;
    }
};
class CShaderRenderManager { public:
    CPixelShader CurrentPixelShader;
    unsigned long UpdateFlags;
    void ApplyPixelShader(const CPixelShader& shader)
    {
        if (shader.Data != CurrentPixelShader.Data) {
            CurrentPixelShader.Assign(shader);
            UpdateFlags |= 3;
        }
    }
};
int main()
{
    CPixelShaderData* first = new CPixelShaderData; first->ReferenceCount = 1;
    CPixelShaderData* second = new CPixelShaderData; second->ReferenceCount = 1;
    CShaderRenderManager m; m.CurrentPixelShader.Data = first; m.UpdateFlags = 0x10;
    CPixelShader s; s.Data = second;
    m.ApplyPixelShader(s);
    if (m.CurrentPixelShader.Data != second || second->ReferenceCount != 2 || g_deleted != 1 || m.UpdateFlags != 0x13) { std::printf("00988140_TEST FAIL\n"); return 1; }
    m.UpdateFlags = 0;
    m.ApplyPixelShader(s);
    if (second->ReferenceCount != 2 || m.UpdateFlags != 0) { std::printf("00988140_TEST FAIL\n"); return 1; }
    delete second;
    std::printf("00988140_TEST PASS\n");
    return 0;
}
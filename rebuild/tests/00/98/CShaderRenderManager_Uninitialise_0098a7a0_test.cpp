// Behaviour test for CShaderRenderManager::Uninitialise (0x0098A7A0): every resource manager is uninitialised once
// and the device pointer is cleared (also with an empty array).
#include <cstdio>
struct CShaderResource { int uninits; void Uninitialise() { ++uninits; } };
struct CPtr { CShaderResource* Data; void* Info; CShaderResource* operator->() const { return Data; } };
struct CArr { CPtr* m_pFirst; CPtr* m_pLast; CPtr* m_pEnd; unsigned int size() const { return (unsigned int)(m_pLast - m_pFirst); } CPtr& operator[](unsigned int i) { return m_pFirst[i]; } };
struct CShaderRenderManager {
    void* D3DDevice; CArr ResourceManagers;
    void Uninitialise()
    {
        for (unsigned int i = 0; i < ResourceManagers.size(); ++i)
            ResourceManagers[i]->Uninitialise();
        D3DDevice = 0;
    }
};
int main()
{
    CShaderResource r[2]; r[0].uninits = r[1].uninits = 0;
    CPtr p[2]; p[0].Data = &r[0]; p[1].Data = &r[1]; p[0].Info = p[1].Info = 0;
    int device = 1;
    CShaderRenderManager m; m.D3DDevice = &device; m.ResourceManagers.m_pFirst = p; m.ResourceManagers.m_pLast = p + 2; m.ResourceManagers.m_pEnd = p + 2;
    m.Uninitialise();
    bool ok = r[0].uninits == 1 && r[1].uninits == 1 && m.D3DDevice == 0;
    m.D3DDevice = &device; m.ResourceManagers.m_pLast = p; m.Uninitialise();
    ok = ok && r[0].uninits == 1 && m.D3DDevice == 0;
    if (!ok) { std::printf("0098a7a0_TEST FAIL\n"); return 1; }
    std::printf("0098a7a0_TEST PASS\n");
    return 0;
}
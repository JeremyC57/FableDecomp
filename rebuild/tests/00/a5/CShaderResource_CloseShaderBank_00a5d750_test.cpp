#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
static int g_deleted = 0;
class CShaderBase { public: virtual ~CShaderBase() { ++g_deleted; } };
class CShaderDataBank { public: int m_unused; };
template <class T> class CArray { public:
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    unsigned long Size() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long i) { return m_pFirst[i]; }
};
class CShaderResource { public:
    CShaderDataBank*     ShaderBank;
    CArray<CShaderBase*> Shaders;
    void CloseShaderBank()
    {
        for (unsigned long i = 0; i < Shaders.Size(); ++i) {
            if (Shaders[i]) { delete Shaders[i]; Shaders[i] = 0; }
        }
        ShaderBank = 0;
    }
};
int main()
{
    CShaderBase* list[4]; list[0] = new CShaderBase; list[1] = 0; list[2] = new CShaderBase; list[3] = new CShaderBase;
    CShaderDataBank bank; CShaderResource r; r.ShaderBank = &bank;
    r.Shaders.m_pFirst = list; r.Shaders.m_pLast = list + 4; r.Shaders.m_pEnd = list + 4;
    r.CloseShaderBank();
    if (g_deleted != 3 || r.ShaderBank != 0 || list[0] || list[1] || list[2] || list[3] || r.Shaders.Size() != 4) { std::printf("00a5d750_TEST FAIL\n"); return 1; }
    r.Shaders.m_pLast = list; r.ShaderBank = &bank; r.CloseShaderBank();
    if (g_deleted != 3 || r.ShaderBank != 0) { std::printf("00a5d750_TEST FAIL\n"); return 1; }
    std::printf("00a5d750_TEST PASS\n");
    return 0;
}
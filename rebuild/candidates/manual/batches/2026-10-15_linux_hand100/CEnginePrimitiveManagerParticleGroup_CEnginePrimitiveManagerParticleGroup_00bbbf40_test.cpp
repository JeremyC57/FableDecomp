#include <cstdio>
struct CEngine { int id; };
struct CEnginePrimitiveManager {
    CEngine* m_pEngine;
    CEnginePrimitiveManager(CEngine* engine) : m_pEngine(engine) { }
    virtual ~CEnginePrimitiveManager() { }
};
struct CEnginePrimitiveManagerParticleGroup : CEnginePrimitiveManager {
    bool m_flag20, m_enabled, m_flag22, m_flag23, m_flag24;
    CEnginePrimitiveManagerParticleGroup(CEngine* engine)
        : CEnginePrimitiveManager(engine), m_flag20(false), m_flag22(false), m_flag23(false), m_flag24(false)
    {
        m_enabled = true;
    }
};
int main()
{
    CEngine e;
    CEnginePrimitiveManagerParticleGroup g(&e);
    if (g.m_pEngine != &e || g.m_flag20 || !g.m_enabled || g.m_flag22 || g.m_flag23 || g.m_flag24) { std::printf("00bbbf40_TEST FAIL\n"); return 1; }
    std::printf("00bbbf40_TEST PASS\n");
    return 0;
}

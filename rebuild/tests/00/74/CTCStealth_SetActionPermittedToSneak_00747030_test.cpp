#include <cstdio>
class CIDrawEnvironment { public:};
class CWorld { public: CIDrawEnvironment m_env; CIDrawEnvironment* DrawGetEnvironment() { return &m_env; } };
struct CDrawEnvironmentUser_00747030 { unsigned char _pad[0x2c]; CIDrawEnvironment* m_pDrawEnvironment; void SetWorld(CWorld* w) { m_pDrawEnvironment = w->DrawGetEnvironment(); } };
int main()
{
    CWorld w; CDrawEnvironmentUser_00747030 u; u.m_pDrawEnvironment = 0; u.SetWorld(&w);
    if (u.m_pDrawEnvironment != &w.m_env) { std::printf("00747030_TEST FAIL\n"); return 1; }
    std::printf("00747030_TEST PASS\n"); return 0;
}
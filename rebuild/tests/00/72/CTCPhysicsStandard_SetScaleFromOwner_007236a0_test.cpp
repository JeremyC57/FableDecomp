#include <cstdio>
class CThingPhysicsMeshInfo { public: unsigned short m_type; };
static long g_seen = -1;
static int g_calls = 0;
struct CTCPhysicsStandard {
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;
    virtual ~CTCPhysicsStandard() {}
    virtual void vfunc_85(long meshType) { g_seen = meshType; ++g_calls; }
    void vfunc_86()
    {
        if (PPhysicsMeshInfo)
            vfunc_85(PPhysicsMeshInfo->m_type);
    }
};
int main()
{
    CTCPhysicsStandard p;
    p.PPhysicsMeshInfo = 0;
    p.vfunc_86();
    if (g_calls != 0) { std::printf("007236a0_TEST FAIL\n"); return 1; }
    CThingPhysicsMeshInfo info; info.m_type = 0xFFFE;
    p.PPhysicsMeshInfo = &info;
    p.vfunc_86();
    if (g_calls != 1 || g_seen != 0xFFFE) { std::printf("007236a0_TEST FAIL\n"); return 1; }
    std::printf("007236a0_TEST PASS\n");
    return 0;
}
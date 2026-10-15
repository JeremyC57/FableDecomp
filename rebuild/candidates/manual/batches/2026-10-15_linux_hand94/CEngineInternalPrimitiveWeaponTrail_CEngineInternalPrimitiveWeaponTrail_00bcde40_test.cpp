#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0x00BCDE40: creation-time block freed, then Trail destroyed, then the (empty) root destructor.
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct CTrailArray { void* _M_start; void* _M_finish; void* _M_end_of_storage; ~CTrailArray() { Log('T'); } };
struct CFloatArray { float* m_pData; long m_count; long m_capacity; ~CFloatArray() { if (m_pData) { Log('F'); std::free(m_pData); } } };
struct CIVCountedPointeeBase { virtual void v00() {} ~CIVCountedPointeeBase() { Log('B'); } };
struct CEngineInternalPrimitiveWeaponTrail : CIVCountedPointeeBase {
    CTrailArray Trail;
    CFloatArray TrailElementCreationTime;
    virtual void v00() {}
    ~CEngineInternalPrimitiveWeaponTrail();
};
CEngineInternalPrimitiveWeaponTrail::~CEngineInternalPrimitiveWeaponTrail()
{
}
int main()
{
    { CEngineInternalPrimitiveWeaponTrail w; w.TrailElementCreationTime.m_pData = (float*)std::malloc(16); }
    if (std::strcmp(g_log, "FTB") != 0) { std::printf("00bcde40_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    { CEngineInternalPrimitiveWeaponTrail w; w.TrailElementCreationTime.m_pData = 0; }
    if (std::strcmp(g_log, "TB") != 0) { std::printf("00bcde40_TEST FAIL\n"); return 1; }
    std::printf("00bcde40_TEST PASS\n");
    return 0;
}

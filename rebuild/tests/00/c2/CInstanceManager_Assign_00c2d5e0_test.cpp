#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fade = 0, g_start = 0, g_quiet = 0, g_ramp = 0, g_nm = 0, g_reports = 0; static long g_started = -1; static float g_fadeTime = -1.0f; static bool g_haveQuiet = true;
struct CName { unsigned char b[8]; CName& operator=(const CName& o) { ++g_nm; b[0] = o.b[0]; return *this; } };
struct SThemeRequest { long m_0; CName m_name; long m_c, m_10, m_14, m_18; };
struct CRampedParam { void RampTo(float target, float time) { if (target == 0.0f && time == 0.0f) ++g_ramp; } };
struct CInstance { unsigned char pad[0x138]; CRampedParam m_volume; unsigned char pad2[0x47]; bool idle; bool IsIdle() { return idle; } };
static CInstance g_quietest;
struct CInstanceManager {
    unsigned char pad[0x18]; CInstance m_instances[2]; long m_current; long m_31c; SThemeRequest m_pending; unsigned char m_bPending;
    void FadeOutAll(float time) { ++g_fade; g_fadeTime = time; }
    CInstance* FindQuietestInstance() { ++g_quiet; return g_haveQuiet ? &g_quietest : 0; }
    void StartInstance(long index, const SThemeRequest*) { ++g_start; g_started = index; }
    void Assign(float fadeTime, const SThemeRequest* request)
    {
        FadeOutAll(fadeTime);
        for (unsigned long index = 0; index < 2; ++index) {
            if (m_instances[index].IsIdle()) { StartInstance(index, request); m_current = index; return; }
        }
        CInstance* instance = FindQuietestInstance();
        if (!instance) ++g_reports;
        instance->m_volume.RampTo(0, 0);
        m_bPending = 1;
        m_pending = *request;
    }
};
static bool Check()
{
    CInstanceManager* m = (CInstanceManager*)std::calloc(1, sizeof(CInstanceManager));
    SThemeRequest t; t.m_0 = 4; t.m_name.b[0] = 9; t.m_c = 1; t.m_10 = 2; t.m_14 = 3; t.m_18 = 5;
    m->m_instances[1].idle = true;
    m->Assign(0.5f, &t);
    if (g_fade != 1 || g_fadeTime != 0.5f || g_start != 1 || g_started != 1 || m->m_current != 1 || g_quiet) return false;
    m->m_instances[1].idle = false;
    m->Assign(0.25f, &t);
    if (g_quiet != 1 || g_ramp != 1 || m->m_bPending != 1 || m->m_pending.m_0 != 4 || m->m_pending.m_name.b[0] != 9 || m->m_pending.m_18 != 5 || g_nm != 1 || g_reports) return false;
    std::free(m);
    return true;
}
int main()
{
    if (!Check()) { std::printf("00c2d5e0_TEST FAIL\n"); return 1; }
    std::printf("00c2d5e0_TEST PASS\n");
    return 0;
}

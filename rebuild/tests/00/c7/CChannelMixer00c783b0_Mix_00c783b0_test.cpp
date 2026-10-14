#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_calls = 0; static float* g_src[2]; static float* g_dst[2]; static long g_width[2], g_limit[2];
static void MixChannels(float* src, float* dst, long width, long limit) { if (g_calls < 2) { g_src[g_calls] = src; g_dst[g_calls] = dst; g_width[g_calls] = width; g_limit[g_calls] = limit; } ++g_calls; }
struct SChannelSet { unsigned char _pad[0x58]; float m_channels[1]; }; struct SLimits { unsigned char _pad[4]; long m_limit; };
struct CMixer { unsigned char _pad[0x10]; float m_quad[4]; float m_pair[2]; SLimits* m_pLimits;
    void Mix(SChannelSet* set) { MixChannels(set->m_channels, m_quad, 4, m_pLimits->m_limit); MixChannels(set->m_channels, m_pair, 2, m_pLimits->m_limit); } };

int main()
{
    SChannelSet set; SLimits lim; lim.m_limit = 3; CMixer m; m.m_pLimits = &lim; m.Mix(&set);
    if (g_calls != 2 || g_src[0] != set.m_channels || g_dst[0] != m.m_quad || g_width[0] != 4 || g_limit[0] != 3 || g_src[1] != set.m_channels || g_dst[1] != m.m_pair || g_width[1] != 2 || g_limit[1] != 3) { std::printf("00c783b0_TEST FAIL\n"); return 1; }
    std::printf("00c783b0_TEST PASS\n");
    return 0;
}
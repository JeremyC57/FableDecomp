// Behaviour test for 0x00C727B0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct SVoiceFlags { unsigned int m_bit0 : 1; unsigned int m_bits1to4 : 4; unsigned int m_bit5 : 1; unsigned int m_rest : 26; };
struct SVoiceRequests { unsigned int m_bits0to6 : 7; unsigned int m_bit7 : 1; unsigned int m_bit8 : 1; unsigned int m_bit9 : 1; unsigned int m_rest : 22; };

static unsigned int g_paused = 0; static int g_pauses = 0;
struct CAudioContext { void SourcePause(unsigned int s) { g_paused = s; g_pauses++; } };
CAudioContext g_AudioContext;
struct V { unsigned int m_positionBLow, m_positionBHigh; SVoiceRequests m_requests; unsigned int SourceId;
    bool SetPositionB(unsigned int low, unsigned int high)
    {
        if (low == 0 && high == 0) { m_requests.m_bit7 = 0; g_AudioContext.SourcePause(SourceId); return true; }
        m_requests.m_bit7 = 1; m_positionBLow = low; m_positionBHigh = high; return false;
    } };
int main()
{
    V v; std::memset(&v, 0, sizeof(v)); v.SourceId = 42;
    if (v.SetPositionB(5, 0) || !v.m_requests.m_bit7 || v.m_positionBLow != 5 || g_pauses != 0) { std::printf("00c727b0_TEST FAIL\n"); return 1; }
    if (!v.SetPositionB(0, 0) || v.m_requests.m_bit7 || g_pauses != 1 || g_paused != 42) { std::printf("00c727b0_TEST FAIL\n"); return 1; }
    std::printf("00c727b0_TEST PASS\n");
    return 0;
}
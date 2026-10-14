#include <cstdio>
static long g_framesPerSecond;
struct CTimedState_00878b00 { unsigned char _pad[0xa4]; long m_frames; void ResetDuration() { m_frames = (long)(g_framesPerSecond * 3.0f); } };
int main()
{
    CTimedState_00878b00 t; g_framesPerSecond = 30; t.ResetDuration();
    if (t.m_frames != 90) { std::printf("00878b00_TEST FAIL\n"); return 1; }
    g_framesPerSecond = 7; t.ResetDuration();
    if (t.m_frames != 21) { std::printf("00878b00_TEST FAIL\n"); return 1; }
    std::printf("00878b00_TEST PASS\n"); return 0;
}

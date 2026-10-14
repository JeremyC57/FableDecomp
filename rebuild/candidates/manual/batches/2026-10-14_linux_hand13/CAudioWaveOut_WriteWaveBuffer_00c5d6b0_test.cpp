#include <cstdio>
struct WAVEHDR_32 { unsigned char _body[0x20]; };
struct CRITICAL_SECTION_18 { unsigned char _body[0x18]; };
static void* g_hwo = 0; static WAVEHDR_32* g_pwh = 0; static unsigned long g_cb = 0; static int g_enter = 0, g_leave = 0; static CRITICAL_SECTION_18* g_cs = 0;
unsigned long __stdcall FakeWaveOutWrite(void* hwo, WAVEHDR_32* pwh, unsigned long cb) { g_hwo = hwo; g_pwh = pwh; g_cb = cb; return 0; }
void __stdcall FakeEnter(CRITICAL_SECTION_18* cs) { ++g_enter; g_cs = cs; }
void __stdcall FakeLeave(CRITICAL_SECTION_18* cs) { if (cs == g_cs) ++g_leave; }
struct CAudioWaveOut {
    void* __vftable; WAVEHDR_32* Headers; void* WaveOut; unsigned char _pad0[4]; long BufferCount; long QueuedCount; long WriteIndex; unsigned char _pad1[4]; CRITICAL_SECTION_18 Lock;
    void WriteWaveBuffer(long) { FakeWaveOutWrite(WaveOut, &Headers[WriteIndex], sizeof(WAVEHDR_32)); FakeEnter(&Lock); ++QueuedCount; FakeLeave(&Lock); ++WriteIndex; if (WriteIndex == BufferCount) WriteIndex = 0; }
};
int main()
{
    WAVEHDR_32 hdrs[3]; CAudioWaveOut w; w.Headers = hdrs; w.WaveOut = (void*)0x99; w.BufferCount = 3; w.QueuedCount = 0; w.WriteIndex = 1;
    w.WriteWaveBuffer(0);
    if (g_hwo != (void*)0x99 || g_pwh != &hdrs[1] || g_cb != 0x20 || g_enter != 1 || g_leave != 1 || g_cs != &w.Lock || w.QueuedCount != 1 || w.WriteIndex != 2) { std::printf("00c5d6b0_TEST FAIL\n"); return 1; }
    w.WriteWaveBuffer(0);
    if (g_pwh != &hdrs[2] || w.QueuedCount != 2 || w.WriteIndex != 0) { std::printf("00c5d6b0_TEST FAIL\n"); return 1; }
    std::printf("00c5d6b0_TEST PASS\n");
    return 0;
}

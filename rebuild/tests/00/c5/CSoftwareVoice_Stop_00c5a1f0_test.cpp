// Behaviour test for CSoftwareVoice::Stop (0x00C5A1F0): the ring is cleared (+0x04..+0x1C to 0), the buffer pointer dropped, and
// the base Stop sets the stopped bit (request word 0x80000000), in that order; a standalone model of the classes.
#include <cstdio>
struct Ring { void* vptr; unsigned long f[7]; void Clear() { for (int i = 0; i < 7; ++i) f[i] = 0; } };
struct BaseVoice {
    unsigned long buffer, requests; int stopCalls;
    void Stop() { requests = 0x80000000u; ++stopCalls; }
};
struct SoftwareVoice : BaseVoice {
    Ring ring;
    void Stop() { ring.Clear(); buffer = 0; BaseVoice::Stop(); }
};
int main()
{
    SoftwareVoice v;
    v.buffer = 0x1234; v.requests = 7; v.stopCalls = 0;
    for (int i = 0; i < 7; ++i) v.ring.f[i] = 100 + i;
    v.Stop();
    for (int i = 0; i < 7; ++i) if (v.ring.f[i] != 0) { std::printf("00c5a1f0_TEST FAIL\n"); return 1; }
    if (v.buffer != 0 || v.requests != 0x80000000u || v.stopCalls != 1) { std::printf("00c5a1f0_TEST FAIL\n"); return 1; }
    std::printf("00c5a1f0_TEST PASS\n");
    return 0;
}
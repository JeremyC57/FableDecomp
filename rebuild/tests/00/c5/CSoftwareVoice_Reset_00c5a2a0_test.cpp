// Behaviour test for CSoftwareVoice::Reset (0x00C5A2A0): the base Reset (positions +0x18..+0x2C, the request word to 1), then the
// phase / +0x1E4 / +0xD4 words cleared and both shared scratch rings flushed (read, write and queued counts to 0, data kept).
#include <cstdio>
struct Ring { unsigned long data, capacity, frameBytes, channels, readPos, writePos, queued;
    void Flush() { readPos = 0; writePos = 0; queued = 0; } };
static Ring g_monoRing = { 1, 0x80, 2, 1, 5, 6, 7 };
static Ring g_mixRing = { 2, 0x80, 2, 6, 8, 9, 10 };
struct BaseVoice { unsigned long pos[6]; int resets;
    void Reset() { pos[0] = pos[1] = pos[2] = pos[3] = pos[5] = 0; pos[4] = 1; ++resets; } };
struct SoftwareVoice : BaseVoice { unsigned long d4, w1e4, step, phase;
    void Reset() { BaseVoice::Reset(); phase = 0; w1e4 = 0; d4 = 0; g_monoRing.Flush(); g_mixRing.Flush(); } };
int main()
{
    SoftwareVoice v;
    for (int i = 0; i < 6; ++i) v.pos[i] = 9;
    v.resets = 0; v.d4 = 3; v.w1e4 = 4; v.step = 0x1000; v.phase = 5;
    v.Reset();
    if (v.resets != 1 || v.pos[4] != 1 || v.pos[0] != 0 || v.pos[5] != 0) { std::printf("00c5a2a0_TEST FAIL\n"); return 1; }
    if (v.d4 != 0 || v.w1e4 != 0 || v.phase != 0 || v.step != 0x1000) { std::printf("00c5a2a0_TEST FAIL\n"); return 1; }
    if (g_monoRing.readPos | g_monoRing.writePos | g_monoRing.queued | g_mixRing.readPos | g_mixRing.writePos | g_mixRing.queued) {
        std::printf("00c5a2a0_TEST FAIL\n"); return 1; }
    if (g_monoRing.data != 1 || g_mixRing.channels != 6 || g_mixRing.capacity != 0x80) { std::printf("00c5a2a0_TEST FAIL\n"); return 1; }
    std::printf("00c5a2a0_TEST PASS\n");
    return 0;
}
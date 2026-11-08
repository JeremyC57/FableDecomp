// Behaviour test for CAudioBuffer::Destroy (0x00C4D9B0): release the context reference, clear the AL error, delete each
// created AL buffer (flag bit 0: +0, bit 1: +4) inside a suspend / process bracket that keeps an outer suspension, clear the
// error again, then poison both handles with 0xC0DAF00D and clear the flags and description.
#include <cstdio>
static int g_errors = 0, g_suspends = 0, g_processes = 0, g_releases = 0;
static unsigned long g_deleted[4]; static int g_nDeleted = 0;
struct CContext {
    bool Suspended; long RefCount;
    void Release() { --RefCount; ++g_releases; }
    long GetError() { ++g_errors; return 0; }
    void Suspend() { ++g_suspends; Suspended = true; }
    void Process() { ++g_processes; Suspended = false; }
    void DeleteBuffers(long count, const unsigned long* b) { for (long i = 0; i < count; ++i) g_deleted[g_nDeleted++] = b[i]; }
};
static CContext g_AudioContext = { false, 2 };
struct CAudioBuffer {
    unsigned long Buffer, SecondBuffer, Flags; const void* Desc;
    void Destroy()
    {
        g_AudioContext.Release();
        g_AudioContext.GetError();
        if (Flags & 1) { bool was = g_AudioContext.Suspended; g_AudioContext.Suspend(); g_AudioContext.DeleteBuffers(1, &Buffer);
                         if (!was) g_AudioContext.Process(); g_AudioContext.GetError(); }
        if (Flags & 2) { bool was = g_AudioContext.Suspended; g_AudioContext.Suspend(); g_AudioContext.DeleteBuffers(1, &SecondBuffer);
                         if (!was) g_AudioContext.Process(); g_AudioContext.GetError(); }
        SecondBuffer = 0xC0DAF00D; Buffer = 0xC0DAF00D; Flags = 0; Desc = 0;
    }
};
int main()
{
    int dummy;
    CAudioBuffer b = { 11, 22, 3, &dummy };
    b.Destroy();
    bool ok = g_releases == 1 && g_AudioContext.RefCount == 1 && g_nDeleted == 2 && g_deleted[0] == 11 && g_deleted[1] == 22
              && g_suspends == 2 && g_processes == 2 && g_errors == 3 && !g_AudioContext.Suspended
              && b.Buffer == 0xC0DAF00D && b.SecondBuffer == 0xC0DAF00D && b.Flags == 0 && b.Desc == 0;
    CAudioBuffer c = { 5, 6, 2, 0 };
    g_AudioContext.Suspended = true;      // an outer suspension is kept
    c.Destroy();
    ok = ok && g_nDeleted == 3 && g_deleted[2] == 6 && g_processes == 2 && g_AudioContext.Suspended && g_errors == 5;
    if (!ok) { std::printf("00c4d9b0_TEST FAIL\n"); return 1; }
    std::printf("00c4d9b0_TEST PASS\n");
    return 0;
}
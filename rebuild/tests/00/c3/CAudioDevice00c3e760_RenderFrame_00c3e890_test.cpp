// Behaviour test for CAudioDevice_00c3e760::RenderFrame (0x00C3E890): listener applied, then (with a primary output) the
// pre-output hook sees the mixer buffers (format 32, stage 0), the mixer channels are interleaved in speaker order
// (5.1: 0,1,4,5,2,3), the post-output hook sees the 16-bit block (format 1, stage 1), the primary is unlocked, and the
// scratch block renders into the secondary output; returns 0.
#include <cstdio>
#include <cstring>
struct SAudioBlockDesc { long m_format, m_channels, m_sampleRate, m_blockSize, m_reserved0, m_reserved1; void* m_pData; };
typedef void (*AudioBlockHook)(SAudioBlockDesc*, long);
static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct COutput { short buf[8]; int locks, unlocks; short* Lock() { ++locks; return buf; } void Unlock(short* b) { if (b == buf) ++unlocks; } };
struct CMixer { long* m_channelBuffers[6]; void WriteChannel(short* dest, long ch) { *dest = (short)(100 + ch); } };
struct CListener { int applied; void Apply() { ++applied; } };
struct CScratch { void Render(short* d) { d[0] = 77; } };
static SAudioBlockDesc g_seen[2]; static long g_stage[2]; static int g_hooks = 0;
static void Hook(SAudioBlockDesc* d, long stage) { if (g_hooks < 2) { g_seen[g_hooks] = *d; g_stage[g_hooks] = stage; } ++g_hooks; }
struct CDevice {
    CMixer* m_pMixer; CScratch* m_pScratch; COutput* m_pPrimary; COutput* m_pSecondary;
    AudioBlockHook m_pPostOutputHook, m_pPreOutputHook; unsigned long m_channels; CListener* m_pListener;
    long RenderFrame()
    {
        SAudioBlockDesc desc;
        if (m_pListener) m_pListener->Apply();
        if (!m_pPrimary) return 0;
        if (m_pPreOutputHook) {
            desc.m_format = 32; desc.m_channels = m_channels; desc.m_sampleRate = 44100; desc.m_blockSize = m_channels << 8;
            desc.m_reserved0 = 0; desc.m_reserved1 = 0; desc.m_pData = m_pMixer->m_channelBuffers; m_pPreOutputHook(&desc, 0);
        }
        short* block = m_pPrimary->Lock();
        switch (m_channels) {
        case 6: m_pMixer->WriteChannel(block, 0); m_pMixer->WriteChannel(block + 1, 1); m_pMixer->WriteChannel(block + 2, 4);
                m_pMixer->WriteChannel(block + 3, 5); m_pMixer->WriteChannel(block + 4, 2); m_pMixer->WriteChannel(block + 5, 3); break;
        case 4: m_pMixer->WriteChannel(block, 0); m_pMixer->WriteChannel(block + 1, 1); m_pMixer->WriteChannel(block + 2, 2);
                m_pMixer->WriteChannel(block + 3, 3); break;
        case 2: m_pMixer->WriteChannel(block, 0); m_pMixer->WriteChannel(block + 1, 1); break;
        case 1: m_pMixer->WriteChannel(block, 0); break;
        }
        if (m_pPostOutputHook) {
            desc.m_format = 1; desc.m_channels = m_channels; desc.m_sampleRate = 44100; desc.m_blockSize = m_channels << 8;
            desc.m_reserved0 = 0; desc.m_reserved1 = 0; desc.m_pData = block; m_pPostOutputHook(&desc, 1);
        }
        m_pPrimary->Unlock(block);
        if (m_pSecondary && m_pScratch) { short* s = m_pSecondary->Lock(); m_pScratch->Render(s); m_pSecondary->Unlock(s); }
        return 0;
    }
};
int main()
{
    CMixer mixer; CScratch scratch; COutput primary, secondary; CListener listener = { 0 };
    std::memset(&primary, 0, sizeof primary); std::memset(&secondary, 0, sizeof secondary);
    CDevice d = { &mixer, &scratch, &primary, &secondary, Hook, Hook, 6, &listener };
    bool ok = d.RenderFrame() == 0;
    short want[6] = { 100, 101, 104, 105, 102, 103 };
    for (int i = 0; i < 6; ++i) ok = ok && primary.buf[i] == want[i];
    ok = ok && listener.applied == 1 && primary.locks == 1 && primary.unlocks == 1 && secondary.buf[0] == 77 && secondary.unlocks == 1;
    ok = ok && g_hooks == 2 && g_stage[0] == 0 && g_seen[0].m_format == 32 && g_seen[0].m_blockSize == 6 * 256
            && g_seen[0].m_pData == (void*)mixer.m_channelBuffers && g_stage[1] == 1 && g_seen[1].m_format == 1
            && g_seen[1].m_pData == (void*)primary.buf && g_seen[1].m_sampleRate == 44100;
    std::memset(primary.buf, 0, sizeof primary.buf);
    d.m_channels = 3; d.m_pPrimary = 0;
    ok = ok && d.RenderFrame() == 0 && listener.applied == 2 && g_hooks == 2;
    d.m_pPrimary = &primary; d.m_pSecondary = 0; d.m_pPreOutputHook = 0; d.m_pPostOutputHook = 0;
    ok = ok && d.RenderFrame() == 0 && primary.buf[0] == 0 && primary.unlocks == 2;
    d.m_channels = 2;
    d.RenderFrame();
    ok = ok && primary.buf[0] == 100 && primary.buf[1] == 101 && primary.buf[2] == 0;
    if (!ok) { std::printf("00c3e890_TEST FAIL\n"); return 1; }
    std::printf("00c3e890_TEST PASS\n");
    return 0;
}
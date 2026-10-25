// Behaviour test for CSoftwareVoice::SizeRing (0x00C5A3C0): no-op without a usable prefetch size; streaming grows the
// ring to the prefetch size only when fewer bytes than that are ahead; otherwise sizes it to the playable region.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 int64;
#else
typedef long long int64;
#endif
static unsigned int g_lastBytes = 0; static int g_calls = 0;
struct CAudioRingBuffer {
    unsigned char* m_pData; unsigned int m_capacity, m_frameBytes, m_readPos;
    void SetCapacity(unsigned int bytes, bool keep) { if (keep) { g_lastBytes = bytes; ++g_calls; } }
    unsigned char* ReadPointer() const { return m_pData + m_readPos * m_frameBytes; }
};
struct CSoftwareVoice {
    unsigned int m_state, m_dataBytes, m_playOffset, m_prefetchBytes; unsigned char* m_pSource;
    CAudioRingBuffer m_ring; unsigned int m_loopStart, m_loopEnd;
    unsigned int GetLoopStart() { return m_loopStart; }
    unsigned int GetLoopEnd() { return m_loopEnd; }
    void SizeRing(bool streaming)
    {
        if (m_prefetchBytes == 0 || m_prefetchBytes >= m_dataBytes) return;
        if (streaming) {
            if ((int64)(m_ring.ReadPointer() - m_pSource) < (int64)m_prefetchBytes) m_ring.SetCapacity(m_prefetchBytes, true);
            return;
        }
        unsigned int start, end;
        if (m_state > 0 && m_state <= 2) { start = 0; end = m_dataBytes; }
        else { start = GetLoopStart(); end = GetLoopEnd(); }
        if (m_pSource == m_ring.m_pData) m_ring.SetCapacity(end, true);
        else if (m_playOffset == 0) m_ring.SetCapacity(end - start, true);
        else m_ring.SetCapacity(end - m_playOffset, true);
    }
};
int main()
{
    static unsigned char mem[4096];
    CSoftwareVoice v; v.m_dataBytes = 4000; v.m_prefetchBytes = 1000; v.m_pSource = mem; v.m_playOffset = 0;
    v.m_ring.m_pData = mem + 2000; v.m_ring.m_frameBytes = 4; v.m_ring.m_readPos = 0;
    v.m_loopStart = 100; v.m_loopEnd = 900; v.m_state = 0;
    v.SizeRing(true);                                       // 2000 bytes ahead: enough
    int ok = g_calls == 0;
    v.m_ring.m_pData = mem + 500; v.SizeRing(true);         // 500 ahead < 1000: grow
    ok = ok && g_calls == 1 && g_lastBytes == 1000;
    v.SizeRing(false); ok = ok && g_lastBytes == 800;       // loop region end - start
    v.m_playOffset = 300; v.SizeRing(false); ok = ok && g_lastBytes == 600;
    v.m_state = 2; v.SizeRing(false); ok = ok && g_lastBytes == 3700;   // whole data less the play offset
    v.m_ring.m_pData = mem; v.SizeRing(false); ok = ok && g_lastBytes == 4000;
    v.m_prefetchBytes = 5000; v.SizeRing(false); ok = ok && g_calls == 5;
    if (!ok) { std::printf("00c5a3c0_TEST FAIL\n"); return 1; }
    std::printf("00c5a3c0_TEST PASS\n");
    return 0;
}
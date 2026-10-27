// Test for 0x00C556E0 CAudioSource::Setup: base failure passes through; else the ring is set up and the stream source opened.
#include <cstdio>
struct CDesc { bool streamed; void* source; bool IsStreamed() { return streamed; } };
struct CRing { unsigned int capacity, frameBytes, channels; void Init(unsigned int c, unsigned int f, unsigned int ch) { capacity = c; frameBytes = f; channels = ch; } };
struct CSource { long baseResult; CRing* m_pRing; CRing m_ring; void* opened; bool openResult;
    long BaseSetup(CDesc*) { return baseResult; }
    bool Open(void* s) { opened = s; return openResult; }
    long Setup(CDesc* desc) {
        long result = BaseSetup(desc);
        if (result < 0) return result;
        m_ring.Init(0x80, 2, 1); m_pRing = &m_ring;
        CDesc* stream = desc->IsStreamed() ? desc : 0;
        void* source = stream->source;
        return Open(source) ? 0 : -5; } };
int main()
{
    int dummy; CDesc d; d.streamed = true; d.source = &dummy;
    CSource s; s.baseResult = -7; s.m_pRing = 0; s.opened = 0; s.openResult = true;
    bool ok = s.Setup(&d) == -7 && s.m_pRing == 0;
    s.baseResult = 0; ok = ok && s.Setup(&d) == 0 && s.m_pRing == &s.m_ring && s.m_ring.capacity == 0x80 && s.opened == &dummy;
    s.openResult = false; ok = ok && s.Setup(&d) == -5;
    if (!ok) { std::printf("00c556e0_TEST FAIL\n"); return 1; }
    std::printf("00c556e0_TEST PASS\n");
    return 0;
}
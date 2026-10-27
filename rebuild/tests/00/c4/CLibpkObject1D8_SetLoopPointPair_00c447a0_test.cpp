// Test for 0x00C447A0 CLibpkObject1D8::SetLoopPointPair: both records updated; a loop-dirty object drops its volume records.
#include <cstdio>
struct SLoop { int start; float length; };
struct CObj { unsigned int m_flags, m_dirty; SLoop* m_pLoop; void* m_pVol; SLoop m_loopStore; SLoop* m_pLoop2; void* m_pVol2; SLoop m_loopStore2;
    void Update(SLoop** cur, SLoop* store, const SLoop* v) { if (v) { *store = *v; *cur = store; } else *cur = 0; m_dirty |= 0x80; }
    void SetLoopPointPair(const SLoop* a, const SLoop* b) {
        Update(&m_pLoop, &m_loopStore, a); Update(&m_pLoop2, &m_loopStore2, b);
        if (m_dirty & 0x80) { m_dirty = (m_dirty & ~0x100u) | 0x1000; m_pVol = 0; m_pVol2 = 0; m_flags |= 0x10000; } } };
int main()
{
    int dummy; SLoop a = {10, 1.0f};
    CObj o; o.m_flags = 0; o.m_dirty = 0x100; o.m_pVol = &dummy; o.m_pVol2 = &dummy;
    o.SetLoopPointPair(&a, 0);
    bool ok = o.m_pLoop == &o.m_loopStore && o.m_loopStore.start == 10 && o.m_pLoop2 == 0 && o.m_pVol == 0 && o.m_pVol2 == 0
        && o.m_dirty == 0x1080 && o.m_flags == 0x10000;
    if (!ok) { std::printf("00c447a0_TEST FAIL\n"); return 1; }
    std::printf("00c447a0_TEST PASS\n");
    return 0;
}
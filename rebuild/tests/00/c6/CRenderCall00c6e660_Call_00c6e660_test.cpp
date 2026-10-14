#include <cstdio>
struct SZeroPair_00c6e660 { float m_first, m_second; };
struct CRenderCall_00c6e660 {
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { }
    virtual void Submit(long a, long b, long c, long d, SZeroPair_00c6e660* r, long e, long f) { seen = a + b + c + d + e; last = f; pair = *r; ++calls; }
    long seen, last, calls; SZeroPair_00c6e660 pair;
    void Call() { SZeroPair_00c6e660 record; record.m_first = 0.0f; record.m_second = 0.0f; Submit(0, 0, 0, 0, &record, 0, 1); }
};
int main()
{
    CRenderCall_00c6e660 c; c.calls = 0; c.pair.m_first = 5.0f; c.Call();
    if (c.calls != 1 || c.seen != 0 || c.last != 1 || c.pair.m_first != 0.0f || c.pair.m_second != 0.0f) { std::printf("00c6e660_TEST FAIL\n"); return 1; }
    std::printf("00c6e660_TEST PASS\n"); return 0;
}
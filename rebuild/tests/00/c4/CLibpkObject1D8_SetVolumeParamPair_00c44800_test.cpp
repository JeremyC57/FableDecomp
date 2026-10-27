// Test for 0x00C44800 CLibpkObject1D8::SetVolumeParamPair: both records updated; a volume-dirty object drops its loop records.
#include <cstdio>
struct SVol { int a; int b; };
struct CObj { unsigned int m_flags, m_dirty; void* m_pLoop; SVol* m_pVol; SVol m_volStore; void* m_pLoop2; SVol* m_pVol2; SVol m_volStore2;
    void Update(SVol** cur, SVol* store, const SVol* v) { if (v) { *store = *v; *cur = store; } else *cur = 0; m_dirty |= 0x100; }
    void SetVolumeParamPair(const SVol* x, const SVol* y) {
        Update(&m_pVol, &m_volStore, x); Update(&m_pVol2, &m_volStore2, y);
        if (m_dirty & 0x100) { m_dirty = (m_dirty & ~0x80u) | 0x1000; m_pLoop = 0; m_pLoop2 = 0; m_flags |= 0x10000; } } };
int main()
{
    int dummy; SVol a = {1, 2}, b = {3, 4};
    CObj o; o.m_flags = 1; o.m_dirty = 0x80; o.m_pLoop = &dummy; o.m_pLoop2 = &dummy;
    o.SetVolumeParamPair(&a, &b);
    bool ok = o.m_pVol == &o.m_volStore && o.m_pVol2 == &o.m_volStore2 && o.m_volStore2.b == 4 && o.m_pLoop == 0 && o.m_pLoop2 == 0
        && o.m_dirty == 0x1100 && o.m_flags == 0x10001;
    if (!ok) { std::printf("00c44800_TEST FAIL\n"); return 1; }
    std::printf("00c44800_TEST PASS\n");
    return 0;
}
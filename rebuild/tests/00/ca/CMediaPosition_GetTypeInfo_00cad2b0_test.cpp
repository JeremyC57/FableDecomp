#include <cstdio>
// CMediaPosition::GetTypeInfo: m_basedisp.GetTypeInfo(IID_IMediaPosition, itinfo, lcid, pptinfo).
static const int IID_IMediaPosition = 0x56A868B2;
struct BaseDispatch {
    const int* riid; unsigned it; unsigned lc; void** pp;
    int GetTypeInfo(const int& r, unsigned i, unsigned l, void** p) { riid = &r; it = i; lc = l; pp = p; *p = this; return 0; }
};
struct MediaPosition { BaseDispatch m_basedisp; int GetTypeInfo(unsigned i, unsigned l, void** p) { return m_basedisp.GetTypeInfo(IID_IMediaPosition, i, l, p); } };
int main()
{
    MediaPosition m; void* ti = 0;
    if (m.GetTypeInfo(0, 0x409, &ti) != 0 || ti != &m.m_basedisp || m.m_basedisp.riid != &IID_IMediaPosition || m.m_basedisp.lc != 0x409 || m.m_basedisp.pp != &ti) { std::printf("00cad2b0_TEST FAIL\n"); return 1; }
    std::printf("00cad2b0_TEST PASS\n");
    return 0;
}
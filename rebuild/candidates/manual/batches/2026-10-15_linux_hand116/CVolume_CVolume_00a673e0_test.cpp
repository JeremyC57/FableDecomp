#include <cstdio>
#include <cstring>
struct D3DVOLUME_DESC { long Format; long Type; unsigned long Usage; long Pool; unsigned long Width; unsigned long Height; unsigned long Depth; };
struct CFakeVolume {
    long m_refs; long m_descCalls;
    CFakeVolume() : m_refs(1), m_descCalls(0) {}
    unsigned long AddRef() { return ++m_refs; }
    unsigned long Release() { return --m_refs; }
    long GetDesc(D3DVOLUME_DESC* d) { ++m_descCalls; d->Format = 21; d->Type = 0; d->Usage = 0; d->Pool = 0; d->Width = 4; d->Height = 5; d->Depth = 6; return 0; }
};
static int g_base;
struct CBase { virtual ~CBase() {} CBase() { ++g_base; } };
struct CVolume : CBase {
    CFakeVolume* m_pVolume;
    CVolume() : m_pVolume(0) {}
    CVolume(const CVolume& other) : m_pVolume(other.m_pVolume) { if (m_pVolume) m_pVolume->AddRef(); }
};
int main()
{
    int ok = 1;
    CFakeVolume a; CVolume x; x.m_pVolume = &a;
    CVolume y(x);
    if (y.m_pVolume != &a || a.m_refs != 2 || g_base != 2) ok = 0;
    CVolume e; CVolume f(e);
    if (f.m_pVolume != 0) ok = 0;
    if (!ok) { std::printf("00a673e0_TEST FAIL\n"); return 1; }
    std::printf("00a673e0_TEST PASS\n");
    return 0;
}

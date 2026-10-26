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
class CVolume : public CBase { public:
    CFakeVolume* m_pVolume;
    CVolume& operator=(const CVolume& other)
    {
        if (m_pVolume) { m_pVolume->Release(); m_pVolume = 0; }
        m_pVolume = other.m_pVolume;
        if (m_pVolume) m_pVolume->AddRef();
        return *this;
    }
};
int main()
{
    int ok = 1;
    CFakeVolume a, b; CVolume x, y; x.m_pVolume = &a; y.m_pVolume = &b;
    CVolume& r = (x = y);
    if (&r != &x || x.m_pVolume != &b || a.m_refs != 0 || b.m_refs != 2) ok = 0;
    CVolume z; z.m_pVolume = 0;
    x = z;
    if (x.m_pVolume != 0 || b.m_refs != 1) ok = 0;
    if (!ok) { std::printf("00a67730_TEST FAIL\n"); return 1; }
    std::printf("00a67730_TEST PASS\n");
    return 0;
}
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
struct C3DVectorI { long X, Y, Z; C3DVectorI(long x, long y, long z) : X(x), Y(y), Z(z) {} };
class CVolume : public CBase { public:
    CFakeVolume* m_pVolume;
    C3DVectorI GetDimensions() const { D3DVOLUME_DESC desc; m_pVolume->GetDesc(&desc); return C3DVectorI(desc.Width, desc.Height, desc.Depth); }
};
int main()
{
    int ok = 1;
    CFakeVolume v; CVolume vol; vol.m_pVolume = &v;
    C3DVectorI d = vol.GetDimensions();
    if (d.X != 4 || d.Y != 5 || d.Z != 6 || v.m_descCalls != 1) ok = 0;
    if (!ok) { std::printf("00a674b0_TEST FAIL\n"); return 1; }
    std::printf("00a674b0_TEST PASS\n");
    return 0;
}
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
struct CBase { virtual ~CBase() {} CBase() { ++g_base; } };struct CVolumeTexture : CBase {
    CFakeVolume* m_pTexture; long m_format; long m_levels; int m_uninits;
    CVolumeTexture() : m_pTexture(0), m_format(0), m_levels(0), m_uninits(0) {}
    CVolumeTexture(const CVolumeTexture& other) : m_pTexture(other.m_pTexture), m_format(other.m_format), m_levels(other.m_levels), m_uninits(0)
    { if (other.m_pTexture) m_pTexture->AddRef(); }
    void Uninitialise() { ++m_uninits; if (m_pTexture) { m_pTexture->Release(); m_pTexture = 0; m_format = 0; m_levels = 0; } }
    CVolumeTexture& operator=(const CVolumeTexture& other)
    {
        Uninitialise();
        m_pTexture = other.m_pTexture; m_format = other.m_format; m_levels = other.m_levels;
        if (other.m_pTexture) m_pTexture->AddRef();
        return *this;
    }
};
int main()
{
    int ok = 1;
    CFakeVolume t; CVolumeTexture a; a.m_pTexture = &t; a.m_format = 3; a.m_levels = 2;
    CVolumeTexture b(a);
    if (b.m_pTexture != &t || b.m_format != 3 || b.m_levels != 2 || t.m_refs != 2) ok = 0;
    if (!ok) { std::printf("00a67dd0_TEST FAIL\n"); return 1; }
    std::printf("00a67dd0_TEST PASS\n");
    return 0;
}

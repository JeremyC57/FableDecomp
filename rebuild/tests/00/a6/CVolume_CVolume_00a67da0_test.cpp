// Behaviour test for 00a67da0 CVolume::CVolume(IDirect3DVolume9*) (copy of the model; Initialise is a working
// stand-in with the retail release/store/AddRef order and a fake COM volume).
#include <cstdio>

struct IUnknownLike {
    virtual long QueryInterface(const void*, void**) { return 0; }
    virtual unsigned long AddRef() = 0;
    virtual unsigned long Release() = 0;
};
struct IDirect3DVolume9 : IUnknownLike {
    unsigned long refs;
    IDirect3DVolume9() : refs(1) {}
    unsigned long AddRef() { return ++refs; }
    unsigned long Release() { return --refs; }
};
static int g_baseCtor = 0;
struct CBase {
    virtual ~CBase() {}
    CBase() { ++g_baseCtor; }
};
static int g_init = 0;
static IDirect3DVolume9* g_heldAtInit = (IDirect3DVolume9*)1;
struct CVolume : CBase {
    IDirect3DVolume9* m_pVolume;
    CVolume(IDirect3DVolume9* volume);
    void InitialiseFromVolumeTextureMipmap(IDirect3DVolume9* volume)
    {
        ++g_init; g_heldAtInit = m_pVolume;
        if (m_pVolume) { m_pVolume->Release(); m_pVolume = 0; }
        m_pVolume = volume;
        m_pVolume->AddRef();
    }
};
CVolume::CVolume(IDirect3DVolume9* volume) : m_pVolume(0)
{
    InitialiseFromVolumeTextureMipmap(volume);
}

int main()
{
    IDirect3DVolume9 vol;
    CVolume v(&vol);
    int ok = 1;
    if (g_baseCtor != 1 || g_init != 1 || g_heldAtInit != 0) ok = 0;   // member zeroed before Initialise
    if (v.m_pVolume != &vol || vol.refs != 2) ok = 0;
    if (!ok) { std::printf("00a67da0_TEST FAIL\n"); return 1; }
    std::printf("00a67da0_TEST PASS\n");
    return 0;
}
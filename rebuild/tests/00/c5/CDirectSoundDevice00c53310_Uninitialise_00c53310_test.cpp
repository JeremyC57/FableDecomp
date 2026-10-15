#include <cstdio>
static int g_releases = 0, g_uninits = 0;
static void CoUninitializeStub() { ++g_uninits; }
struct IDirectSound8 {
    virtual unsigned long Release() { ++g_releases; return 0; }
    virtual ~IDirectSound8() {}
};
struct CDirectSoundDevice {
    long m_reserved00; unsigned long m_status; IDirectSound8* m_pDirectSound;
    void Uninitialise()
    {
        if (m_pDirectSound) {
            m_pDirectSound->Release();
            CoUninitializeStub();
            m_pDirectSound = 0;
            m_status = 0x80000000;
        }
    }
};
int main()
{
    IDirectSound8 ds;
    CDirectSoundDevice d; d.m_reserved00 = 0; d.m_status = 0; d.m_pDirectSound = &ds;
    d.Uninitialise();
    if (d.m_pDirectSound != 0 || d.m_status != 0x80000000 || g_releases != 1 || g_uninits != 1) { std::printf("00c53310_TEST FAIL\n"); return 1; }
    d.m_status = 5; d.Uninitialise();
    if (d.m_status != 5 || g_releases != 1 || g_uninits != 1) { std::printf("00c53310_TEST FAIL\n"); return 1; }
    std::printf("00c53310_TEST PASS\n");
    return 0;
}
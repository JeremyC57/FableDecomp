#include <cstdio>
#include <cstring>
static int g_uninit, g_shutdown;
void CoUninitialize() { ++g_uninit; }
struct CFakeCom { long m_refs; virtual unsigned long Release() { return --m_refs; } };
struct CSoundDevice {
    virtual void Shutdown() { ++g_shutdown; }
    long m_x; CFakeCom* m_pDevice;
    void ReleaseVolume()
    {
        Shutdown();
        if (m_pDevice) { m_pDevice->Release(); CoUninitialize(); m_pDevice = 0; }
    }
};
int main()
{
    int ok = 1;
    CFakeCom com; com.m_refs = 1;
    CSoundDevice d; d.m_x = 0; d.m_pDevice = &com;
    d.ReleaseVolume();
    if (g_shutdown != 1 || com.m_refs != 0 || g_uninit != 1 || d.m_pDevice != 0) ok = 0;
    d.ReleaseVolume();
    if (g_shutdown != 2 || g_uninit != 1) ok = 0;
    if (!ok) { std::printf("00c57010_TEST FAIL\n"); return 1; }
    std::printf("00c57010_TEST PASS\n");
    return 0;
}
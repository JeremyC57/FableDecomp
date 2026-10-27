// Behaviour test for 0x00C56F70 CDSoundStreamBuffer::OpenDevice
#include <cstdio>
static int g_initHr = 0, g_createHr = 0, g_initDevHr = 0, g_coopHr = 0, g_uninits = 0, g_releases = 0; static unsigned int g_coInit = 99;
struct Dev { int Initialize(const void*) { return g_initDevHr; } int SetCooperativeLevel(void*, unsigned int l) { return l == 2 ? g_coopHr : -1; }
    void Release() { ++g_releases; } };
static Dev g_dev;
static int CoInit(unsigned int c) { g_coInit = c; return g_initHr; }
static int CoCreate(Dev** out) { if (g_createHr == 0) *out = &g_dev; return g_createHr; }
struct Buf {
    Dev* m_pDevice;
    bool OpenDevice(const void* device, void* window, bool mt)
    {
        int hr = CoInit(mt ? 0 : 2);
        if (hr != 0 && hr != 1) return false;
        if (CoCreate(&m_pDevice) != 0) { ++g_uninits; m_pDevice = 0; return false; }
        if (m_pDevice->Initialize(device) < 0) { m_pDevice->Release(); ++g_uninits; m_pDevice = 0; return false; }
        if (m_pDevice->SetCooperativeLevel(window, 2) < 0) { m_pDevice->Release(); ++g_uninits; m_pDevice = 0; return false; }
        return true;
    }
};
static int Fail() { std::printf("00c56f70_TEST FAIL\n"); return 1; }
int main()
{
    Buf b; b.m_pDevice = 0;
    if (!b.OpenDevice(0, 0, false) || g_coInit != 2 || b.m_pDevice != &g_dev) return Fail();
    g_initHr = 1; if (!b.OpenDevice(0, 0, true) || g_coInit != 0) return Fail();      // S_FALSE is fine
    g_initHr = -5; if (b.OpenDevice(0, 0, true) || g_uninits != 0) return Fail();
    g_initHr = 0; g_createHr = -1; if (b.OpenDevice(0, 0, true) || g_uninits != 1 || b.m_pDevice) return Fail();
    g_createHr = 0; g_coopHr = -2; if (b.OpenDevice(0, 0, true) || g_releases != 1 || g_uninits != 2 || b.m_pDevice) return Fail();
    std::printf("00c56f70_TEST PASS\n");
    return 0;
}
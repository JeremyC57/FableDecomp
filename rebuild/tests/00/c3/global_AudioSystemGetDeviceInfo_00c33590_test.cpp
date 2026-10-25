// Behaviour test for 00c33590: AudioSystem_GetDeviceInfo -- fills each non-null out word from the device getters
#include <cstdio>
struct CRecord { unsigned int a, b; };
struct CAudioDevice {
    CRecord* m_188;
    int calls;
    unsigned int GetFirstInfo() { ++calls; return m_188 ? m_188->b : 0x80000000u; }
    unsigned int GetSecondInfo() { ++calls; return 0; }
};
struct CAudioSystem { CAudioDevice* m_pDevice; };
static CAudioDevice g_device;
static CAudioSystem g_audioSystem = { &g_device };
static void AudioSystem_GetDeviceInfo(unsigned int* pFirst, unsigned int* pSecond)
{
    if (pFirst) *pFirst = g_audioSystem.m_pDevice->GetFirstInfo();
    if (pSecond) *pSecond = g_audioSystem.m_pDevice->GetSecondInfo();
}
int main()
{
    unsigned int x = 1, y = 1;
    AudioSystem_GetDeviceInfo(&x, &y);
    if (x != 0x80000000u || y != 0 || g_device.calls != 2) { std::printf("00c33590_TEST FAIL\n"); return 1; }
    CRecord r = { 3, 44 }; g_device.m_188 = &r;
    AudioSystem_GetDeviceInfo(&x, 0);
    if (x != 44 || g_device.calls != 3) { std::printf("00c33590_TEST FAIL\n"); return 1; }
    AudioSystem_GetDeviceInfo(0, 0);
    if (g_device.calls != 3) { std::printf("00c33590_TEST FAIL\n"); return 1; }
    std::printf("00c33590_TEST PASS\n");
    return 0;
}
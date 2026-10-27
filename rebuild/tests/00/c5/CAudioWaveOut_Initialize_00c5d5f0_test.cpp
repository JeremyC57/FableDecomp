// Behaviour test for 0x00C5D5F0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

static unsigned int g_devices = 0;
unsigned int waveOutGetNumDevs() { return g_devices; }
bool Initialize(const void* device, void* window, bool multithreaded)
{
    if (waveOutGetNumDevs() == 0)
        return false;
    return true;
}
int main()
{
    if (Initialize(0, 0, false)) { std::printf("00c5d5f0_TEST FAIL\n"); return 1; }
    g_devices = 3;
    if (!Initialize(0, 0, true)) { std::printf("00c5d5f0_TEST FAIL\n"); return 1; }
    std::printf("00c5d5f0_TEST PASS\n");
    return 0;
}
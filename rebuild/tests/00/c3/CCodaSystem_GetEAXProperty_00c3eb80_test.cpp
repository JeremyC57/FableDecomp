// Behaviour test for 00c3eb80: CCodaSystem::GetEAXProperty -- -9 without a hardware device, else the device's GetProperty with all four arguments
#include <cstdio>
static const void* g_set = 0; static unsigned long g_id = 0; static void* g_data = 0; static unsigned long g_size = 0;
class CEAXDevice {
public:
    virtual long SetProperty(const void* set, unsigned long id, void* data, unsigned long size) { g_set = set; g_id = id; g_data = data; g_size = size; return 41; }
    virtual long GetProperty(const void* set, unsigned long id, void* data, unsigned long size) { g_set = set; g_id = id; g_data = data; g_size = size; return 42; }
};
class CCodaSystem {
public:
    CEAXDevice* m_pEAXDevice;
    long GetEAXProperty(const void* set, unsigned long id, void* data, unsigned long size)
    {
        if (m_pEAXDevice)
            return m_pEAXDevice->GetProperty(set, id, data, size);
        return -9;
    }
};
int main()
{
    CEAXDevice d; CCodaSystem s; s.m_pEAXDevice = 0; int guid = 0, value = 0;
    if (s.GetEAXProperty(&guid, 5, &value, 4) != -9 || g_id) { std::printf("00c3eb80_TEST FAIL\n"); return 1; }
    s.m_pEAXDevice = &d;
    long r = s.GetEAXProperty(&guid, 5, &value, 4);
    if (r != 42 || g_set != &guid || g_id != 5 || g_data != &value || g_size != 4) { std::printf("00c3eb80_TEST FAIL\n"); return 1; }
    std::printf("00c3eb80_TEST PASS\n");
    return 0;
}

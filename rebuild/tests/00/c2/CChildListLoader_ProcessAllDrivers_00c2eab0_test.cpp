// Behaviour test for 00c2eab0: CChildListLoader::ProcessAllDrivers -- ProcessDriver(id, count) for every driver of the +0x10 list
#include <cstdio>
struct STriggerDriver { long m_id; unsigned short m_count; };
struct CDriverList {
    STriggerDriver items[3]; unsigned int n;
    virtual STriggerDriver* GetAt(unsigned int index) { return &items[index]; }
    virtual unsigned int GetCount() { return n; }
};
struct CLoadSource { CDriverList m_drivers; };
struct CChildListLoader {
    CLoadSource* m_pSource;
    long ids[4]; unsigned int counts[4]; int calls;
    void ProcessDriver(long id, unsigned int count) { ids[calls] = id; counts[calls] = count; ++calls; }
    void ProcessAllDrivers()
    {
        for (unsigned int i = 0; i < m_pSource->m_drivers.GetCount(); ++i) {
            STriggerDriver* driver = m_pSource->m_drivers.GetAt(i);
            ProcessDriver(driver->m_id, driver->m_count);
        }
    }
};
int main()
{
    CLoadSource src; src.m_drivers.n = 3;
    for (int i = 0; i < 3; ++i) { src.m_drivers.items[i].m_id = 100 + i; src.m_drivers.items[i].m_count = (unsigned short)(0xFFF0 + i); }
    CChildListLoader l; l.m_pSource = &src; l.calls = 0;
    l.ProcessAllDrivers();
    if (l.calls != 3 || l.ids[0] != 100 || l.ids[2] != 102 || l.counts[1] != 0xFFF1u) { std::printf("00c2eab0_TEST FAIL\n"); return 1; }
    src.m_drivers.n = 0; l.calls = 0; l.ProcessAllDrivers();
    if (l.calls != 0) { std::printf("00c2eab0_TEST FAIL\n"); return 1; }
    std::printf("00c2eab0_TEST PASS\n");
    return 0;
}
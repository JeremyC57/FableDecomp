// Behaviour test for 00c2eab0: CBankProcessor::ProcessAllDrivers -- AddDriverToGroup(number, group) for every driver record of the +0x10 list
#include <cstdio>
struct SDriverRecord { unsigned long m_index; unsigned short m_group; };
struct CDriverList {
    SDriverRecord items[3]; unsigned int n;
    virtual SDriverRecord* GetAt(unsigned int index) { return &items[index]; }
    virtual unsigned int GetCount() { return n; }
};
struct CBankData { CDriverList m_drivers; };
struct CBankProcessor {
    CBankData* m_pData;
    long ids[4]; unsigned int counts[4]; int calls;
    void AddDriverToGroup(unsigned long number, unsigned int group) { ids[calls] = (long)number; counts[calls] = group; ++calls; }
    void ProcessAllDrivers()
    {
        for (unsigned int i = 0; i < m_pData->m_drivers.GetCount(); ++i) {
            SDriverRecord* driver = m_pData->m_drivers.GetAt(i);
            AddDriverToGroup(driver->m_index, driver->m_group);
        }
    }
};
int main()
{
    CBankData src; src.m_drivers.n = 3;
    for (int i = 0; i < 3; ++i) { src.m_drivers.items[i].m_index = 100 + i; src.m_drivers.items[i].m_group = (unsigned short)(0xFFF0 + i); }
    CBankProcessor l; l.m_pData = &src; l.calls = 0;
    l.ProcessAllDrivers();
    if (l.calls != 3 || l.ids[0] != 100 || l.ids[2] != 102 || l.counts[1] != 0xFFF1u) { std::printf("00c2eab0_TEST FAIL\n"); return 1; }
    src.m_drivers.n = 0; l.calls = 0; l.ProcessAllDrivers();
    if (l.calls != 0) { std::printf("00c2eab0_TEST FAIL\n"); return 1; }
    std::printf("00c2eab0_TEST PASS\n");
    return 0;
}
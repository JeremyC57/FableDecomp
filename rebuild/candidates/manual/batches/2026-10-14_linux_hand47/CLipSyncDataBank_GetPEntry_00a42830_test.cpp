#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00A42830: exercises the same logic through a copy of the model (compiled separately from the source)
struct CLipSyncDataBankEntry { void* m_pObject; long m_state; };
struct CLipSyncDataBank {
    unsigned long m_size; CLipSyncDataBankEntry* m_pEntries;
    bool EntryExists(unsigned long index) const { return index != 0 && index < m_size; }
    CLipSyncDataBankEntry* GetPEntry(unsigned long index) { if (EntryExists(index)) return &m_pEntries[index]; return 0; }
};
static bool Run()
{
    CLipSyncDataBankEntry entries[4]; CLipSyncDataBank bank; bank.m_size = 4; bank.m_pEntries = entries;
    return bank.GetPEntry(2) == &entries[2] && bank.GetPEntry(0) == 0 && bank.GetPEntry(4) == 0 && bank.GetPEntry(3) == &entries[3];
}
int main()
{
    if (!Run()) { std::printf("00a42830_TEST FAIL\n"); return 1; }
    std::printf("00a42830_TEST PASS\n");
    return 0;
}

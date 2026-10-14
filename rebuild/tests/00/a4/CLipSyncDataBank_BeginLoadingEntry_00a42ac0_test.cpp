#include <cstdio>
struct CLipSyncDataBank; static int g_loads = 0; static unsigned long g_index = 99; static CLipSyncDataBank* g_bank = 0;
struct CLipSyncDataBankEntry { void* m_pObject; long m_state; void BeginLoading(unsigned long i, CLipSyncDataBank* b) { ++g_loads; g_index = i; g_bank = b; } };
struct CLipSyncDataBank { void* __vftable; unsigned char _bankFile[0x164 - 4]; CLipSyncDataBankEntry* m_pEntries; bool exists[4];
    bool EntryExists(unsigned long i) const { return exists[i]; }
    void BeginLoadingEntry(unsigned long i) { if (EntryExists(i)) { CLipSyncDataBankEntry& e = m_pEntries[i]; if (e.m_pObject == 0) e.BeginLoading(i, this); } } };
int main() { int obj; CLipSyncDataBankEntry e[3] = { { 0, 0 }, { &obj, 0 }, { 0, 0 } }; CLipSyncDataBank b; b.m_pEntries = e; b.exists[0] = true; b.exists[1] = true; b.exists[2] = false;
    b.BeginLoadingEntry(0); if (g_loads != 1 || g_index != 0 || g_bank != &b) { std::printf("00a42ac0_TEST FAIL\n"); return 1; } b.BeginLoadingEntry(1); b.BeginLoadingEntry(2); if (g_loads != 1) { std::printf("00a42ac0_TEST FAIL\n"); return 1; } std::printf("00a42ac0_TEST PASS\n"); return 0; }
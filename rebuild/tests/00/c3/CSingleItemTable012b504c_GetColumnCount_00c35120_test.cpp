// Test for 0x00C35120 CSingleItemTable_012b504c::GetColumnCount: 1 with an entry, else 0.
#include <cstdio>
struct CTableEntry { int id; };
struct CSingle { CTableEntry* m_pEntry; unsigned int GetColumnCount(unsigned int) { return m_pEntry != 0; } };
int main()
{
    CTableEntry a = {1}; CSingle s; s.m_pEntry = 0;
    unsigned int n0 = s.GetColumnCount(0); s.m_pEntry = &a; unsigned int n1 = s.GetColumnCount(5);
    if (n0 != 0 || n1 != 1) { std::printf("00c35120_TEST FAIL\n"); return 1; }
    std::printf("00c35120_TEST PASS\n");
    return 0;
}
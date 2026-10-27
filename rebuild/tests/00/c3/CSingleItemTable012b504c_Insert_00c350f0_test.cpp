// Test for 0x00C350F0 CSingleItemTable_012b504c::Insert: -2 past row 0, -1 when occupied, else 0 and stored.
#include <cstdio>
struct CTableEntry { int id; };
struct CSingle { CTableEntry* m_pEntry;
    long Insert(unsigned int row, CTableEntry* e) { if (row > 0) return -2; if (m_pEntry) return -1; m_pEntry = e; return 0; } };
int main()
{
    CTableEntry a = {1}, b = {2}; CSingle s; s.m_pEntry = 0;
    if (s.Insert(1, &a) != -2 || s.m_pEntry != 0 || s.Insert(0, &a) != 0 || s.m_pEntry != &a || s.Insert(0, &b) != -1 || s.m_pEntry != &a)
    { std::printf("00c350f0_TEST FAIL\n"); return 1; }
    std::printf("00c350f0_TEST PASS\n");
    return 0;
}
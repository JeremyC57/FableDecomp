#include <cstdio>

static int g_clears = 0, g_names = 0, g_order = 0, g_nameOrder = 0, g_clearOrder = 0; static void* g_freed = 0;
static void FakeFree(void* p) { g_freed = p; }
struct CWideString { void* m_p; ~CWideString() { ++g_names; g_nameOrder = ++g_order; } };
struct CRetailList_009be060 { struct SNode; SNode* m_pHead; unsigned long m_size;
    void Clear() { ++g_clears; g_clearOrder = ++g_order; }
    ~CRetailList_009be060() { Clear(); if (m_pHead) FakeFree(m_pHead); } };
struct CNamedEntryList_00a3bde0 { CRetailList_009be060 m_entries; unsigned char _pad[0x14 - 8]; CWideString m_name; ~CNamedEntryList_00a3bde0() { } };
int main()
{
    int head;
    { CNamedEntryList_00a3bde0 l; l.m_entries.m_pHead = (CRetailList_009be060::SNode*)&head; l.m_name.m_p = 0; }
    if (g_clears != 1 || g_names != 1 || g_freed != &head || g_nameOrder != 1 || g_clearOrder != 2) { std::printf("00a3bde0_TEST FAIL\n"); return 1; }
    std::printf("00a3bde0_TEST PASS\n"); return 0;
}

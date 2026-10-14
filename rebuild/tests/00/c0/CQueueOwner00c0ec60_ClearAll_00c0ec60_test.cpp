#include <cstdio>
static void* g_removed[4]; static int g_n = 0;
struct CQueueOwner { unsigned char _pad[0x2c]; void** m_pEntries; unsigned char _pad30[4]; long m_entryCount;
    void Remove(void* e) { g_removed[g_n++] = e; --m_entryCount; } void ClearAll() { while (m_entryCount != 0) Remove(m_pEntries[m_entryCount - 1]); } };
int main() { int a, b, c; void* entries[3] = { &a, &b, &c }; CQueueOwner q; q.m_pEntries = entries; q.m_entryCount = 3; q.ClearAll();
    if (q.m_entryCount != 0 || g_n != 3 || g_removed[0] != &c || g_removed[2] != &a) { std::printf("00c0ec60_TEST FAIL\n"); return 1; } std::printf("00c0ec60_TEST PASS\n"); return 0; }
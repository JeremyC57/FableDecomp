#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct SEntryNode_00a96da0 { long m_0; SEntryNode_00a96da0* m_pFirst; SEntryNode_00a96da0* m_pNext; SEntryNode_00a96da0* m_pPrev; };
static int g_erased = 0; static void* g_erasedFrom = 0;
struct CRetailEntryList_00a96da0 { SEntryNode_00a96da0* m_pHead; long m_count; void EraseFrom(SEntryNode_00a96da0* n) { ++g_erased; g_erasedFrom = n; }
    void clear() { if (m_count) { EraseFrom(m_pHead->m_pFirst); m_pHead->m_pNext = m_pHead; m_pHead->m_pFirst = 0; m_pHead->m_pPrev = m_pHead; m_count = 0; } }
    ~CRetailEntryList_00a96da0() { clear(); if (m_pHead) free(m_pHead); } };
struct CBuffer_00a96da0 { void* m_pBlock; ~CBuffer_00a96da0() { if (m_pBlock) free(m_pBlock); } };
struct CEntryListBuffer_00a96da0 { CBuffer_00a96da0 m_buffer; unsigned char _pad04[8]; CRetailEntryList_00a96da0 m_entries; ~CEntryListBuffer_00a96da0() { } };
int main() {
    SEntryNode_00a96da0 head, first; head.m_pFirst = &first; int x; { CEntryListBuffer_00a96da0 b; b.m_buffer.m_pBlock = &x; b.m_entries.m_pHead = &head; b.m_entries.m_count = 1; }
    if (g_erased != 1 || g_erasedFrom != &first || head.m_pNext != &head || g_freed != 2 || g_freedPtr[0] != &head || g_freedPtr[1] != &x) { std::printf("00a96da0_TEST FAIL\n"); return 1; }
    std::printf("00a96da0_TEST PASS\n"); return 0;
}

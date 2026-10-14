#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct SEntryNode_00884b60 { long m_0; SEntryNode_00884b60* m_pFirst; SEntryNode_00884b60* m_pNext; SEntryNode_00884b60* m_pPrev; };
static int g_erased = 0; static void* g_erasedFrom = 0;
struct CRetailEntryList_00884b60 { SEntryNode_00884b60* m_pHead; long m_count; void EraseFrom(SEntryNode_00884b60* n) { ++g_erased; g_erasedFrom = n; }
    void clear() { if (m_count) { EraseFrom(m_pHead->m_pFirst); m_pHead->m_pNext = m_pHead; m_pHead->m_pFirst = 0; m_pHead->m_pPrev = m_pHead; m_count = 0; } }
    ~CRetailEntryList_00884b60() { clear(); if (m_pHead) free(m_pHead); } };
namespace NScreenEffect { struct CScreenEffectPriorityStack { struct CStackEntry { virtual void v00() { } unsigned char _pad[0x2c]; ~CStackEntry() { } };
    struct CScreenFilterEntry : CStackEntry { CRetailEntryList_00884b60 m_entries; ~CScreenFilterEntry() { } }; }; }
int main() {
    SEntryNode_00884b60 head, first; head.m_pFirst = &first; head.m_pNext = &first; head.m_pPrev = &first;
    { NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry e; e.m_entries.m_pHead = &head; e.m_entries.m_count = 2; }
    if (g_erased != 1 || g_erasedFrom != &first || head.m_pNext != &head || head.m_pPrev != &head || head.m_pFirst || g_freed != 1 || g_freedPtr[0] != &head) { std::printf("00884b60_TEST FAIL\n"); return 1; }
    std::printf("00884b60_TEST PASS\n"); return 0;
}

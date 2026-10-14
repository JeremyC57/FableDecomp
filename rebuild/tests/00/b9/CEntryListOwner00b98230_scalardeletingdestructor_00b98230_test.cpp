#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct SEntryNode_00b98230 { long m_0; SEntryNode_00b98230* m_pFirst; SEntryNode_00b98230* m_pNext; SEntryNode_00b98230* m_pPrev; };
static int g_erased = 0; static void* g_erasedFrom = 0;
struct CRetailEntryList_00b98230 { SEntryNode_00b98230* m_pHead; long m_count; void EraseFrom(SEntryNode_00b98230* n) { ++g_erased; g_erasedFrom = n; }
    void clear() { if (m_count) { EraseFrom(m_pHead->m_pFirst); m_pHead->m_pNext = m_pHead; m_pHead->m_pFirst = 0; m_pHead->m_pPrev = m_pHead; m_count = 0; } }
    ~CRetailEntryList_00b98230() { clear(); if (m_pHead) free(m_pHead); } };
struct CEntryListOwner_00b98230 { long m_0; CRetailEntryList_00b98230 m_entries; ~CEntryListOwner_00b98230() { } };
static void Destroy_CEntryListOwner_00b98230(CEntryListOwner_00b98230* o) { delete o; }
int main() {
    SEntryNode_00b98230 head, first; head.m_pFirst = &first; CEntryListOwner_00b98230* o = new CEntryListOwner_00b98230; o->m_entries.m_pHead = &head; o->m_entries.m_count = 1; Destroy_CEntryListOwner_00b98230(o);
    if (g_erased != 1 || g_erasedFrom != &first || head.m_pNext != &head || g_freed != 1 || g_freedPtr[0] != &head) { std::printf("00b98230_TEST FAIL\n"); return 1; }
    std::printf("00b98230_TEST PASS\n"); return 0;
}
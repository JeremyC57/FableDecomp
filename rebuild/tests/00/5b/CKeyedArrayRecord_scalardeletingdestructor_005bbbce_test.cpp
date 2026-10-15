// Behaviour test for CKeyedArrayRecord's scalar deleting destructor @ 0x005BBBCE: deleting the record
// frees its array buffer (when present) and then the record's own storage.
#include <cstdio>
#include <cstdlib>
#include <new>

static void* g_freed[4];
static int g_freeCount = 0;
static void FreeStub(void* p) { if (g_freeCount < 4) g_freed[g_freeCount] = p; ++g_freeCount; }
static int g_deleteCount = 0;

template<class T>
struct CArray {
    T* m_first;
    T* m_last;
    T* m_end;
    ~CArray() { if (m_first) FreeStub(m_first); }
};

struct CKeyedArrayRecord {
    long         m_key;
    long         m_value;
    CArray<long> m_items;
    static void operator delete(void* p) { ++g_deleteCount; std::free(p); }
};

int main()
{
    static long items[3];
    bool ok = true;
    CKeyedArrayRecord* r = new CKeyedArrayRecord;
    r->m_key = 1; r->m_value = 2;
    r->m_items.m_first = items; r->m_items.m_last = items + 3; r->m_items.m_end = items + 3;
    delete r;
    if (g_freeCount != 1 || g_freed[0] != items || g_deleteCount != 1) ok = false;

    CKeyedArrayRecord* e = new CKeyedArrayRecord;
    e->m_items.m_first = 0; e->m_items.m_last = 0; e->m_items.m_end = 0;
    delete e;
    if (g_freeCount != 1 || g_deleteCount != 2) ok = false;

    // Destroy in place (the flag-0 path used by _Destroy_range): frees the array, keeps the storage.
    CKeyedArrayRecord local;
    local.m_items.m_first = items; local.m_items.m_last = items; local.m_items.m_end = items;
    local.~CKeyedArrayRecord();
    if (g_freeCount != 2 || g_deleteCount != 2) ok = false;
    new (&local) CKeyedArrayRecord;
    local.m_items.m_first = 0;

    if (!ok) { std::printf("005bbbce_TEST FAIL\n"); return 1; }
    std::printf("005bbbce_TEST PASS\n");
    return 0;
}
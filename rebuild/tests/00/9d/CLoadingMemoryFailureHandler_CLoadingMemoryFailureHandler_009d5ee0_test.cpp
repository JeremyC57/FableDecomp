// Behaviour test for 009d5ee0 (CLoadingMemoryFailureHandler::~CLoadingMemoryFailureHandler): the critical section is deleted, then the Banks storage is freed (only when allocated).
#include <cstdio>

struct SCriticalSection { long m_words[6]; };

static int   g_order = 0;
static int   g_deletedAt = 0, g_freedAt = 0;
static void* g_freed = 0;
static SCriticalSection* g_deleted = 0;

static void FreeBlock(void* p) { g_freed = p; g_freedAt = ++g_order; }
static void DeleteSection(SCriticalSection* s) { g_deleted = s; g_deletedAt = ++g_order; }

struct CBankFileAsync;

template <class T> struct CRetailArray {
    T* first;
    T* last;
    T* end_of_storage;
    ~CRetailArray()
    {
        if (first != 0)
            FreeBlock(first);
    }
};

struct CFailedAllocationHandler {
    virtual ~CFailedAllocationHandler() { }
};

struct CLoadingMemoryFailureHandler : CFailedAllocationHandler {
    CRetailArray<CBankFileAsync*> Banks;
    SCriticalSection              CriticalSection;
    ~CLoadingMemoryFailureHandler() { DeleteSection(&CriticalSection); }
};

int main()
{
    static CBankFileAsync* storage[4];
    SCriticalSection* section;
    {
        CLoadingMemoryFailureHandler h;
        h.Banks.first = storage; h.Banks.last = storage; h.Banks.end_of_storage = storage + 4;
        section = &h.CriticalSection;
    }
    if (g_deleted != section || g_freed != (void*)storage || g_deletedAt != 1 || g_freedAt != 2) { std::printf("009d5ee0_TEST FAIL\n"); return 1; }
    g_freed = 0;
    {
        CLoadingMemoryFailureHandler h;
        h.Banks.first = 0; h.Banks.last = 0; h.Banks.end_of_storage = 0;
    }
    if (g_freed != 0 || g_order != 3) { std::printf("009d5ee0_TEST FAIL\n"); return 1; }
    std::printf("009d5ee0_TEST PASS\n");
    return 0;
}
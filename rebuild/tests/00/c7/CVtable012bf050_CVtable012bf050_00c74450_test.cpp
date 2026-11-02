#include <cstdio>
// Behaviour test for CVtable_012BF050::~CVtable_012BF050 (retail 0x00C74450): the last instance
// frees the shared table and clears the pointer; earlier ones only drop the count.
static int g_frees = 0;
static void* g_lastFreed = 0;
void FreeSharedTable(void* table) { ++g_frees; g_lastFreed = table; }

struct CVtable_012BF050 {
    virtual void v00() { }
    CVtable_012BF050() { ++s_refCount; }
    ~CVtable_012BF050();
    static void* s_pSharedTable;
    static long  s_refCount;
};
void* CVtable_012BF050::s_pSharedTable = 0;
long  CVtable_012BF050::s_refCount = 0;

CVtable_012BF050::~CVtable_012BF050()
{
    if (--s_refCount == 0) {
        FreeSharedTable(s_pSharedTable);
        s_pSharedTable = 0;
    }
}

int main()
{
    static int table;
    CVtable_012BF050::s_pSharedTable = &table;
    {
        CVtable_012BF050 a;
        {
            CVtable_012BF050 b;
            if (CVtable_012BF050::s_refCount != 2) { std::printf("00c74450_TEST FAIL\n"); return 1; }
        }
        if (g_frees != 0 || CVtable_012BF050::s_refCount != 1 || CVtable_012BF050::s_pSharedTable != &table) { std::printf("00c74450_TEST FAIL\n"); return 1; }
    }
    if (g_frees != 1 || g_lastFreed != &table || CVtable_012BF050::s_refCount != 0 || CVtable_012BF050::s_pSharedTable != 0) { std::printf("00c74450_TEST FAIL\n"); return 1; }
    std::printf("00c74450_TEST PASS\n");
    return 0;
}
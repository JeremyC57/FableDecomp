#include <cstdio>
// Behaviour test for CVtable_012BF050's scalar deleting destructor (retail 0x00C74480): `delete`
// runs the shared-table release and then frees the object.
static int g_frees = 0;
static void* g_lastFreed = 0;
void FreeSharedTable(void* table) { ++g_frees; g_lastFreed = table; }

struct CVtable_012BF050 {
    CVtable_012BF050() { ++s_refCount; }
    virtual ~CVtable_012BF050();
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
    CVtable_012BF050* a = new CVtable_012BF050;
    CVtable_012BF050* b = new CVtable_012BF050;
    delete a;
    if (g_frees != 0 || CVtable_012BF050::s_refCount != 1) { std::printf("00c74480_TEST FAIL\n"); return 1; }
    delete b;
    if (g_frees != 1 || g_lastFreed != &table || CVtable_012BF050::s_pSharedTable != 0 || CVtable_012BF050::s_refCount != 0) { std::printf("00c74480_TEST FAIL\n"); return 1; }
    std::printf("00c74480_TEST PASS\n");
    return 0;
}
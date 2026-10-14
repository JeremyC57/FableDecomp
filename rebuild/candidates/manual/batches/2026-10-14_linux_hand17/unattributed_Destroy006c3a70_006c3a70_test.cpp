#include <cstdio>
#include <cstdlib>
static int g_stringDtors = 0, g_frees = 0, g_deletes = 0; static void* g_freed = 0; static void* g_deleted = 0;
class CCharString { public: void* PStringData; ~CCharString() { ++g_stringDtors; } };
struct SNamedEntry { CCharString Key; unsigned char Data[0x10]; };
struct CRetailEntryArray {
    SNamedEntry* Begin; SNamedEntry* End; SNamedEntry* Capacity;
    ~CRetailEntryArray() { SNamedEntry* first = Begin; SNamedEntry* last = End; for (; first != last; ++first) first->~SNamedEntry(); if (Begin != 0) { ++g_frees; g_freed = Begin; } }
};
struct CNamedEntryList_006c3a70 {
    CCharString Name; CRetailEntryArray Entries;
    void operator delete(void* p) { ++g_deletes; g_deleted = p; std::free(p); }
};
static void __fastcall Destroy_006c3a70(CNamedEntryList_006c3a70* object) { delete object; }
int main()
{
    CNamedEntryList_006c3a70* list = (CNamedEntryList_006c3a70*)std::malloc(sizeof(CNamedEntryList_006c3a70));
    SNamedEntry* entries = (SNamedEntry*)std::malloc(3 * sizeof(SNamedEntry));
    list->Entries.Begin = entries; list->Entries.End = entries + 3; list->Entries.Capacity = entries + 4;
    Destroy_006c3a70(list);
    if (g_stringDtors != 4 || g_frees != 1 || g_freed != entries || g_deletes != 1 || g_deleted != list) { std::printf("006c3a70_TEST FAIL\n"); return 1; }
    std::free(entries);
    Destroy_006c3a70(0);
    if (g_stringDtors != 4 || g_frees != 1 || g_deletes != 1) { std::printf("006c3a70_TEST FAIL\n"); return 1; }
    CNamedEntryList_006c3a70* bare = (CNamedEntryList_006c3a70*)std::malloc(sizeof(CNamedEntryList_006c3a70));
    bare->Entries.Begin = 0; bare->Entries.End = 0; bare->Entries.Capacity = 0;
    Destroy_006c3a70(bare);
    if (g_stringDtors != 5 || g_frees != 1 || g_deletes != 2 || sizeof(SNamedEntry) != 0x14) { std::printf("006c3a70_TEST FAIL\n"); return 1; }
    std::printf("006c3a70_TEST PASS\n"); return 0;
}

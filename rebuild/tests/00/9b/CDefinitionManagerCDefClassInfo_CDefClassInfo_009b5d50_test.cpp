// Behaviour test for 0x009B5D50 CDefinitionManager::CDefClassInfo::~CDefClassInfo (compiler-generated):
// the GlobalIndices block is freed when allocated, then Name is destroyed.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
static int g_order[4];
static int g_n = 0;
static void Note(int e) { if (g_n < 4) g_order[g_n++] = e; }
static void FreeBlock(void* p) { if (p != 0) { ++g_frees; std::free(p); } }
struct CharString { void* PStringData; CharString() : PStringData(0) { } ~CharString() { Note(2); } };
struct ULongArray {
    unsigned int* Start; unsigned int* Finish; unsigned int* EndOfStorage;
    ULongArray() : Start(0), Finish(0), EndOfStorage(0) { }
    ~ULongArray() { if (Start != 0) Note(1); FreeBlock(Start); }
};
struct DefClassInfo {
    CharString Name;
    void* (*AllocFunc)();
    ULongArray GlobalIndices;
};
int main()
{
    {
        DefClassInfo info;
        info.GlobalIndices.Start = (unsigned int*)std::malloc(3 * sizeof(unsigned int));
    }
    if (g_frees != 1 || g_n != 2 || g_order[0] != 1 || g_order[1] != 2) { std::printf("009b5d50_TEST FAIL\n"); return 1; }
    g_n = 0; g_frees = 0;
    { DefClassInfo info; }
    if (g_frees != 0 || g_n != 1 || g_order[0] != 2) { std::printf("009b5d50_TEST FAIL\n"); return 1; }
    std::printf("009b5d50_TEST PASS\n");
    return 0;
}
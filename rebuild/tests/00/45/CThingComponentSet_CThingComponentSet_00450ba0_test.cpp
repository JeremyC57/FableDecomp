// Behaviour test for CThingComponentSet::~CThingComponentSet (0x00450BA0): the Entries small vector frees its storage and is
// left empty (Start 0, Size 0).
#include <stdio.h>
#include <stdlib.h>
static void* g_freed;
static void FreeBlock(void* p) { g_freed = p; free(p); }
template <class T, int N> struct SmallVector {
    T* Start; signed char Size;
    void Release() { FreeBlock(Start); Start = 0; Size = 0; }
};
struct Entry { int Id; };
struct ComponentSet {
    SmallVector<Entry, 8> Entries;
    void Destroy() { Entries.Release(); }
};
int main()
{
    ComponentSet set;
    set.Entries.Start = (Entry*)malloc(8 * sizeof(Entry));
    set.Entries.Size = 3;
    Entry* block = set.Entries.Start;
    set.Destroy();
    if (g_freed != (void*)block || set.Entries.Start != 0 || set.Entries.Size != 0) { printf("00450ba0_TEST FAIL\n"); return 1; }
    printf("00450ba0_TEST PASS\n");
    return 0;
}

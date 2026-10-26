// Behaviour test for 0x00a80e40: std::__destroy_aux over pair<long, CCountedPointer<CNavConnectorNode> > -- each element's counted pointer is
// released (count dropped, the block's delete function and the block freed at zero) and both words cleared.
#include <stdio.h>
static int g_deleted;
struct Info { int RefCount; void (*DeleteFunc)(void*); void* Data; };
struct Elem { int key; void* data; Info* info; };
static void del(void*) { ++g_deleted; }
static void destroy(Elem* first, Elem* last)
{
    for (; first != last; ++first) {
        if (first->info) { if (--first->info->RefCount == 0) first->info->DeleteFunc(first->info->Data); }
        first->data = 0; first->info = 0;
    }
}
int main()
{
    Info a = { 1, del, 0 }, b = { 3, del, 0 };
    Elem v[3] = { { 1, &a, &a }, { 2, 0, 0 }, { 3, &b, &b } };
    destroy(v, v + 3);
    bool ok = g_deleted == 1 && b.RefCount == 2 && v[0].info == 0 && v[2].data == 0 && v[1].key == 2;
    destroy(v, v);
    if (!ok) { printf("00a80e40_TEST FAIL\n"); return 1; }
    printf("00a80e40_TEST PASS\n");
    return 0;
}

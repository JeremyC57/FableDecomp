#include <cstdio>

struct Info { long count; void (*del)(void*); void* data; };
static int g_deleted = 0, g_discarded = 0; static void Del(void*) { ++g_deleted; }
struct Ptr { void* d; Info* i; void Reset() { if (i) { --i->count; if (i->count == 0) { i->del(i->data); delete i; } } i = 0; d = 0; } };
static void Discard(Ptr&) { ++g_discarded; }
struct Owner { long removed; Owner() : removed(-1) {} void RemoveLOD(unsigned long lod, bool) { removed = (long)lod; } };
struct Lod { Ptr file; Owner* owner; unsigned char lod;
    void OnResourceLoadAborted() { Discard(file); file.Reset(); owner->RemoveLOD(lod, false); } };
int main()
{
    Owner o; Lod l = { { &o, new Info }, &o, 2 }; l.file.i->count = 1; l.file.i->del = Del; l.file.i->data = 0; l.OnResourceLoadAborted();
    if (g_deleted != 1 || g_discarded != 1 || l.file.d || l.file.i || o.removed != 2) { std::printf("00a254a0_TEST FAIL\n"); return 1; }
    std::printf("00a254a0_TEST PASS\n");
    return 0;
}

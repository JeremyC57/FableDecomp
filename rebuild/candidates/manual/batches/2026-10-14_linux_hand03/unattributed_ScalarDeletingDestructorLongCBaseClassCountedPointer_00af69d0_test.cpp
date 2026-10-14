#include <cstdio>

struct Info { long count; void (*del)(void*); void* data; };
static int g_deleted = 0, g_base = 0; static void Del(void*) { ++g_deleted; }
struct Ptr { void* d; Info* i; ~Ptr() { if (i) { --i->count; if (i->count == 0) { i->del(i->data); delete i; } } d = 0; i = 0; } };
struct Base { void* vt; ~Base() { ++g_base; } };
struct Rec { long key; Base base; Ptr p; };
int main()
{
    Rec* r = new Rec; r->p.d = 0; r->p.i = new Info; r->p.i->count = 1; r->p.i->del = Del; r->p.i->data = 0;
    delete r; if (g_deleted != 1 || g_base != 1) { std::printf("00af69d0_TEST FAIL\n"); return 1; }
    std::printf("00af69d0_TEST PASS\n");
    return 0;
}

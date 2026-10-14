#include <cstdio>

struct Info { long count; void (*del)(void*); void* data; };
static int g_deleted = 0; static void Del(void*) { ++g_deleted; }
struct Ptr { void* d; Info* i; ~Ptr() { if (i) { --i->count; if (i->count == 0) { i->del(i->data); delete i; } } d = 0; i = 0; } };
struct Str { char* s; ~Str() { delete[] s; } };
struct Rec { unsigned char init[0x2c]; Str name; Ptr bank; };
int main()
{
    Rec* r = new Rec; r->name.s = new char[4]; r->bank.d = r; r->bank.i = new Info; r->bank.i->count = 1; r->bank.i->del = Del; r->bank.i->data = 0;
    delete r; if (g_deleted != 1) { std::printf("009f87c0_TEST FAIL\n"); return 1; }
    std::printf("009f87c0_TEST PASS\n");
    return 0;
}
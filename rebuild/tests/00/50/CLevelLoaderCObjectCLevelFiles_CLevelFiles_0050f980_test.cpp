// Behaviour test for 0x0050F980 (CLevelFiles::~CLevelFiles): three counted pointers released last member first.
#include <cstdio>
static int g_order[8]; static int g_n;
struct Block { long refs; void (*destroy)(void*); void* obj; };
static void Destroy(void* p) { g_order[g_n++] = *(int*)p; }
struct Ptr {
    void* obj; Block* cnt;
    ~Ptr()
    {
        if (cnt) { --cnt->refs; if (cnt->refs == 0) { cnt->destroy(cnt->obj); delete cnt; } }
        obj = 0; cnt = 0;
    }
};
struct LevelFiles { long m0; Ptr a, b, c; };
int main()
{
    int ids[3] = { 1, 2, 3 };
    Block* shared = new Block; shared->refs = 2; shared->destroy = Destroy; shared->obj = &ids[1];
    {
        LevelFiles f; f.m0 = 0;
        f.a.obj = &ids[0]; f.a.cnt = new Block; f.a.cnt->refs = 1; f.a.cnt->destroy = Destroy; f.a.cnt->obj = &ids[0];
        f.b.obj = &ids[1]; f.b.cnt = shared;
        f.c.obj = &ids[2]; f.c.cnt = new Block; f.c.cnt->refs = 1; f.c.cnt->destroy = Destroy; f.c.cnt->obj = &ids[2];
    }
    int ok = g_n == 2 && g_order[0] == 3 && g_order[1] == 1 && shared->refs == 1;
    delete shared;
    if (!ok) { std::printf("0050f980_TEST FAIL\n"); return 1; }
    std::printf("0050f980_TEST PASS\n");
    return 0;
}
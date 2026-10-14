#include <cstdio>
static int g_destroyed = 0, g_base = 0;
struct Block { long refs; void (__fastcall* destroy)(void*); void* obj; };
static void __fastcall Destroy(void*) { ++g_destroyed; }
struct CP { void* obj; Block* cnt; ~CP() { if (cnt) { --cnt->refs; if (cnt->refs == 0) { cnt->destroy(cnt->obj); delete cnt; } } obj = 0; cnt = 0; } };
struct Base { virtual ~Base() { ++g_base; } };
struct T : Base { CP p; virtual ~T() {} };
int main()
{
    T* t = new T; Block* b = new Block; b->refs = 1; b->destroy = Destroy; b->obj = t; t->p.obj = t; t->p.cnt = b;
    delete t;
    if (g_destroyed != 1 || g_base != 1) { std::printf("004ab170_TEST FAIL\n"); return 1; }
    std::printf("004ab170_TEST PASS\n");
    return 0;
}

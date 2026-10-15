// Behaviour test for 0x0084F570: vector<CCountedPointer<T> >::erase: shift the tail down, release and clear the vacated last slot, return the position
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Block { long refs; int* destroyed; };
struct CP {
    void* data; Block* info;
    void Release() { if (info) { --info->refs; if (info->refs == 0) { ++*info->destroyed; delete info; } } data = 0; info = 0; }
};
struct Vec {
    CP* start; CP* finish;
    CP* erase(CP* pos)
    {
        if (pos + 1 != finish) for (CP* s = pos + 1, *d = pos; s != finish; ++s, ++d) *d = *s;
        --finish; finish->Release();
        return pos;
    }
};
static void TestErase()
{
    int destroyed = 0;
    CP v[3]; Block* b[3];
    for (int i = 0; i < 3; ++i) { b[i] = new Block; b[i]->refs = 2; b[i]->destroyed = &destroyed; v[i].data = (void*)(v + i); v[i].info = b[i]; }
    Vec vec; vec.start = v; vec.finish = v + 3;
    CP* r = vec.erase(v);
    // plain copies (the shifted elements share blocks) then the vacated last slot releases one reference
    CHECK(r == v && vec.finish == v + 2 && v[0].info == b[1] && v[1].info == b[2] && v[2].info == 0 && v[2].data == 0);
    CHECK(b[2]->refs == 1 && destroyed == 0);
    CP* r2 = vec.erase(v + 1);   // erasing the last element: no copy, release only (last reference: destroyed)
    CHECK(r2 == v + 1 && vec.finish == v + 1 && destroyed == 1 && v[1].info == 0 && v[0].info == b[1]);
    delete b[0]; delete b[1];
}
static void Run() { TestErase(); }

int main()
{
    Run();
    if (g_fail) { std::printf("0084f570_TEST FAIL\n"); return 1; }
    std::printf("0084f570_TEST PASS\n");
    return 0;
}
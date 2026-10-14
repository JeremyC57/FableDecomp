#include <cstdio>

struct Chunk { void* a; void* b; Chunk() : a(0), b(0) {} };
struct Factory { Chunk* last; Factory() : last(0) {}
    virtual Chunk* Register(Chunk* c) { last = c; return c; }
    Chunk* CreateNewInstance() { return Register(new Chunk); } };
int main()
{
    Factory f; Chunk* c = f.CreateNewInstance();
    if (!c || c != f.last || c->a || c->b) { std::printf("00ac29c0_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("00ac29c0_TEST PASS\n");
    return 0;
}
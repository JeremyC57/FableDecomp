#include <cstdio>

struct Chunk { long a; Chunk() : a(0) {} };
struct Factory { Chunk* last; Factory() : last(0) {}
    virtual Chunk* Register(Chunk* c) { last = c; return c; }
    Chunk* CreateNewInstance() { return Register(new Chunk); } };
int main()
{
    Factory f; Chunk* c = f.CreateNewInstance();
    if (!c || c != f.last || c->a != 0) { std::printf("00aa5b30_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("00aa5b30_TEST PASS\n");
    return 0;
}
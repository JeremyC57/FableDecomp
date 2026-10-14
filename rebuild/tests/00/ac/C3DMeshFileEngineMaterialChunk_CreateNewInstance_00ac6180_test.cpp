#include <cstdio>

struct Chunk { long a; Chunk(); };
Chunk::Chunk() : a(5) {}
struct Factory { Chunk* last; Factory() : last(0) {}
    virtual Chunk* Register(Chunk* c) { last = c; return c; }
    Chunk* CreateNewInstance() { return Register(new Chunk); } };
int main()
{
    Factory f; Chunk* c = f.CreateNewInstance();
    if (!c || c != f.last || c->a != 5) { std::printf("00ac6180_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("00ac6180_TEST PASS\n");
    return 0;
}
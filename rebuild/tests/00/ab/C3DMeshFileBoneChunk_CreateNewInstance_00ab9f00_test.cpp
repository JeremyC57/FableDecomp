#include <cstdio>

struct Chunk { long BoneIndex; Chunk() : BoneIndex(-1) {} };
struct Factory { Chunk* last; Factory() : last(0) {}
    virtual Chunk* Register(Chunk* c) { last = c; return c; }
    Chunk* CreateNewInstance() { return Register(new Chunk); } };
int main()
{
    Factory f; Chunk* c = f.CreateNewInstance();
    if (!c || c != f.last || c->BoneIndex != -1) { std::printf("00ab9f00_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("00ab9f00_TEST PASS\n");
    return 0;
}
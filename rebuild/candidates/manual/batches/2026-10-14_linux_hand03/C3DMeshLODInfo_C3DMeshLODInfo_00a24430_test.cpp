#include <cstdio>

struct Ptr { void* d; void* i; Ptr() : d(0), i(0) {} };
struct Lod { Ptr mesh, file; void* owner; unsigned char lod; bool used; Lod(void* o, unsigned char l) : owner(o), lod(l), used(false) {} };
int main()
{
    int x; Lod l(&x, 3); if (l.owner != &x || l.lod != 3 || l.used || l.mesh.d || l.file.i) { std::printf("00a24430_TEST FAIL\n"); return 1; }
    std::printf("00a24430_TEST PASS\n");
    return 0;
}

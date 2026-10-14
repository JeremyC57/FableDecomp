#include <cstdio>

struct Prim { bool r; bool UploadBuffers() { return r; } };
struct Mesh { unsigned long n; Prim* p; bool UploadBuffers() { bool ok = true; for (unsigned long i = 0; i < n; ++i) ok &= p[i].UploadBuffers(); return ok; } };
int main()
{
    Prim prims[3] = { { true }, { true }, { true } }; Mesh m = { 3, prims };
    if (!m.UploadBuffers()) { std::printf("00a89200_TEST FAIL\n"); return 1; }
    prims[1].r = false; if (m.UploadBuffers()) { std::printf("00a89200_TEST FAIL\n"); return 1; }
    m.n = 0; if (!m.UploadBuffers()) { std::printf("00a89200_TEST FAIL\n"); return 1; }
    std::printf("00a89200_TEST PASS\n");
    return 0;
}
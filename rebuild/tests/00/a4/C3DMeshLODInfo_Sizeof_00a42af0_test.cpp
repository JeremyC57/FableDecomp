#include <cstdio>

struct Mesh { unsigned long Sizeof() const { return 100; } };
struct File { unsigned long Sizeof() const { return 10; } };
struct Lod { Mesh* mesh; File* file;
    unsigned long Sizeof() const { unsigned long m = mesh ? mesh->Sizeof() + 8 : 8; unsigned long f = file ? file->Sizeof() + 8 : 8; return 0x34 + m + f; } };
int main()
{
    Mesh m; File f; Lod a = { &m, &f }, b = { 0, 0 }, c = { &m, 0 };
    if (a.Sizeof() != 0x34 + 108 + 18 || b.Sizeof() != 0x34 + 16 || c.Sizeof() != 0x34 + 108 + 8) { std::printf("00a42af0_TEST FAIL\n"); return 1; }
    std::printf("00a42af0_TEST PASS\n");
    return 0;
}
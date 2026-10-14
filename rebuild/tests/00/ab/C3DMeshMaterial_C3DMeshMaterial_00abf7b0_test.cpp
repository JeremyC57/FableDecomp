#include <cstdio>

struct Mat { long MaterialIndex; const char* Name; long DecalID; unsigned long d, b, r, i, flags, self; bool t1, t2, t3, t4; void* Filenames;
    Mat() : MaterialIndex(-1), Name(""), DecalID(0), d(0), b(0), r(0), i(0), flags(0), self(0), t1(false), t2(false), t3(false), t4(false), Filenames(0) {} };
int main()
{
    Mat m; if (m.MaterialIndex != -1 || m.Name[0] || m.DecalID || m.self || m.t4 || m.Filenames) { std::printf("00abf7b0_TEST FAIL\n"); return 1; }
    std::printf("00abf7b0_TEST PASS\n");
    return 0;
}
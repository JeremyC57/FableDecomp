#include <cstdio>

struct HP { const char* type; float x, y, z; long SubMeshIndex, BoneIndex;
    HP(const char* t) : type(t), x(0), y(0), z(0), SubMeshIndex(-1), BoneIndex(-1) {} };
struct DO : HP { float dx, dy, dz; bool UseLocalOrigin; DO() : HP("HDMY"), dx(0), dy(0), dz(0), UseLocalOrigin(false) {} };
int main()
{
    DO d; if (d.type[1] != 'D' || d.dx != 0 || d.UseLocalOrigin || d.BoneIndex != -1) { std::printf("00aa9e10_TEST FAIL\n"); return 1; }
    std::printf("00aa9e10_TEST PASS\n");
    return 0;
}
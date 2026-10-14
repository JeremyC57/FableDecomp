#include <cstdio>

struct HP { const char* type; float x, y, z; long SubMeshIndex, BoneIndex; const char* Name;
    HP() : type("HPNT"), x(0), y(0), z(0), SubMeshIndex(-1), BoneIndex(-1), Name("") {} };
int main()
{
    HP h; if (h.x != 0 || h.z != 0 || h.SubMeshIndex != -1 || h.BoneIndex != -1 || h.type[0] != 'H') { std::printf("00aa9b20_TEST FAIL\n"); return 1; }
    std::printf("00aa9b20_TEST PASS\n");
    return 0;
}
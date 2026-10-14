#include <cstdio>

struct M { float m[12]; };
struct Bone { const char* type; long BoneIndex, ParentIndex, FirstChildIndex, SiblingIndex; M Transform; const char* Name;
    Bone() : type("BONE"), BoneIndex(-1), ParentIndex(-1), FirstChildIndex(-1), SiblingIndex(-1), Name("") {}
    Bone(long i, const M& t) : type("BONE"), BoneIndex(i), ParentIndex(-1), FirstChildIndex(-1), SiblingIndex(-1), Transform(t), Name("") {} };
int main()
{
    M m; for (int i = 0; i < 12; ++i) m.m[i] = (float)i;
    Bone b(7, m);
    if (b.BoneIndex != 7 || b.ParentIndex != -1 || b.SiblingIndex != -1 || b.Transform.m[11] != 11.0f) { std::printf("00ab9d50_TEST FAIL\n"); return 1; }
    std::printf("00ab9d50_TEST PASS\n");
    return 0;
}
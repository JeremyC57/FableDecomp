#include <cstdio>

struct M { float m[12]; };
struct Bone { const char* type; long BoneIndex, ParentIndex, FirstChildIndex, SiblingIndex; M Transform; const char* Name;
    Bone() : type("BONE"), BoneIndex(-1), ParentIndex(-1), FirstChildIndex(-1), SiblingIndex(-1), Name("") {}
    Bone(long i, const M& t) : type("BONE"), BoneIndex(i), ParentIndex(-1), FirstChildIndex(-1), SiblingIndex(-1), Transform(t), Name("") {} };
int main()
{
    Bone b;
    if (b.BoneIndex != -1 || b.ParentIndex != -1 || b.FirstChildIndex != -1 || b.SiblingIndex != -1 || b.type[0] != 'B') { std::printf("00ab9da0_TEST FAIL\n"); return 1; }
    std::printf("00ab9da0_TEST PASS\n");
    return 0;
}

// Behaviour test for 0x00A44C90 C3DSkeletalBoneMorph::CompareCRCs
#include <cstdio>
class C3DSkeletalBoneMorph {
public:
    char*         BoneName;
    unsigned long BoneNameCRC;
    float         X, Y, Z;
    static bool CompareCRCs(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b) { return a.BoneNameCRC < b.BoneNameCRC; }
};
int main()
{
    C3DSkeletalBoneMorph a, b;
    a.BoneNameCRC = 5; b.BoneNameCRC = 0xF0000000UL;
    if (!C3DSkeletalBoneMorph::CompareCRCs(a, b)) { std::printf("00a44c90_TEST FAIL\n"); return 1; }
    if (C3DSkeletalBoneMorph::CompareCRCs(b, a)) { std::printf("00a44c90_TEST FAIL\n"); return 1; }
    if (C3DSkeletalBoneMorph::CompareCRCs(a, a)) { std::printf("00a44c90_TEST FAIL\n"); return 1; }
    std::printf("00a44c90_TEST PASS\n");
    return 0;
}
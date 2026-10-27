// Behaviour test for 0x00AC6B40 QuadricOptimiserInternals::COptimisedVertex::CGroup::CompareWeight -- heavier bone weight sorts first
#include <cstdio>
struct CGroup {
    unsigned long BoneIndex;
    float BoneWeight;
    static bool CompareWeight(const CGroup& a, const CGroup& b) { return a.BoneWeight > b.BoneWeight; }
};
int main()
{
    CGroup a, b; a.BoneIndex = 1; a.BoneWeight = 0.75f; b.BoneIndex = 2; b.BoneWeight = 0.25f;
    if (!CGroup::CompareWeight(a, b) || CGroup::CompareWeight(b, a) || CGroup::CompareWeight(a, a)) { std::printf("00ac6b40_TEST FAIL\n"); return 1; }
    std::printf("00ac6b40_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x0044C26D COpinionPersonalityTraits::COpinionPersonalityTraits()
#include <cstdio>
class COpinionPersonalityTraits {
public:
    float BaseOffset, HeroStatScaling, VillageOpinionScaling, TimeScalingRunIn, TimeScalingRunOut;
    float PeakScalingIfPos, PeakScalingIfNeg, PersistScalingIfPos, PersistScalingIfNeg;
    COpinionPersonalityTraits()
        : BaseOffset(0.0f), HeroStatScaling(1.0f), VillageOpinionScaling(1.0f), TimeScalingRunIn(1.0f),
          TimeScalingRunOut(1.0f), PeakScalingIfPos(1.0f), PeakScalingIfNeg(1.0f), PersistScalingIfPos(1.0f),
          PersistScalingIfNeg(1.0f) { }
};
int main()
{
    COpinionPersonalityTraits t[5];
    for (int i = 0; i < 5; ++i) {
        const float* f = &t[i].BaseOffset;
        if (f[0] != 0.0f) { std::printf("0044c26d_TEST FAIL\n"); return 1; }
        for (int k = 1; k < 9; ++k)
            if (f[k] != 1.0f) { std::printf("0044c26d_TEST FAIL\n"); return 1; }
    }
    if (sizeof(COpinionPersonalityTraits) != 0x24) { std::printf("0044c26d_TEST FAIL\n"); return 1; }
    std::printf("0044c26d_TEST PASS\n");
    return 0;
}
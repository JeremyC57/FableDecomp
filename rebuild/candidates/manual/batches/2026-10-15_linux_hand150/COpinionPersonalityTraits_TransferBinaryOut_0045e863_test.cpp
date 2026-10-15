// behaviour test for 0x0045E863 COpinionPersonalityTraits::TransferBinaryOut
#include <cstdio>
struct CDataOutputStream {
    float out[16]; int n;
    void WriteFloat(float value) { out[n++] = value; }
};
struct COpinionPersonalityTraits {
    float BaseOffset, HeroStatScaling, VillageOpinionScaling, TimeScalingRunIn, TimeScalingRunOut,
          PeakScalingIfPos, PeakScalingIfNeg, PersistScalingIfPos, PersistScalingIfNeg;
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        stream.WriteFloat(BaseOffset);
        stream.WriteFloat(HeroStatScaling);
        stream.WriteFloat(VillageOpinionScaling);
        stream.WriteFloat(TimeScalingRunIn);
        stream.WriteFloat(TimeScalingRunOut);
        stream.WriteFloat(PeakScalingIfPos);
        stream.WriteFloat(PeakScalingIfNeg);
        stream.WriteFloat(PersistScalingIfPos);
        stream.WriteFloat(PersistScalingIfNeg);
    }
};
int main()
{
    COpinionPersonalityTraits t = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    CDataOutputStream s; s.n = 0;
    t.TransferBinaryOut(s);
    int bad = s.n != 9;
    for (int i = 0; i < 9 && !bad; ++i) if (s.out[i] != (float)(i + 1)) bad = 1;
    if (bad) { std::printf("0045e863_TEST FAIL\n"); return 1; }
    std::printf("0045e863_TEST PASS\n");
    return 0;
}

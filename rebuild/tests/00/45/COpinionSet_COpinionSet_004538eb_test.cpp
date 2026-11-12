// behaviour test for 0x004538EB COpinionSet::COpinionSet(): a new opinion set holds five opinion values, all zero
#include <cstdio>
#include <vector>
struct OpinionSet {
    std::vector<float> OpinionValues;
    OpinionSet() : OpinionValues(5) { }                     // the function under test: five value-initialised floats
};
int main()
{
    OpinionSet s;
    int fail = 0;
    if (s.OpinionValues.size() != 5) fail |= 1;
    for (unsigned i = 0; i < s.OpinionValues.size(); ++i)
        if (s.OpinionValues[i] != 0.0f) fail |= 2;
    if (fail) { std::printf("CApprenticeSpeedTest_004538eb_TEST FAIL %d\n", fail); return 1; }
    std::printf("CApprenticeSpeedTest_004538eb_TEST PASS\n");
    return 0;
}

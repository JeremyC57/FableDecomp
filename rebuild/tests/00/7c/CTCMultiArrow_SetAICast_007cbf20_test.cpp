// Behaviour test for CTCMultiArrow::SetAICast @ 0x007CBF20: per-level tables, volleys = shots / per volley.
#include <cstdio>
class CSpecialAbilitiesMultiArrowDef { public: long NumberOfShots[4]; long NumberInVolley[4]; };
class CTCMultiArrow { public:
    const CSpecialAbilitiesMultiArrowDef* PDef;
    long NumberOfShots, NumberInVolley, NumOfVolleys, SpellLevel;
    unsigned char RemovedEffect;
    long CreateAmmoEffectCount;
    void SetAICast(long level)
    {
        NumberOfShots = PDef->NumberOfShots[level];
        NumberInVolley = PDef->NumberInVolley[level];
        NumOfVolleys = NumberOfShots / NumberInVolley;
        CreateAmmoEffectCount = NumOfVolleys;
    }
};
int main()
{
    CSpecialAbilitiesMultiArrowDef d = { { 3, 6, 9, 12 }, { 1, 2, 3, 5 } };
    CTCMultiArrow m; m.PDef = &d;
    m.SetAICast(3);
    if (m.NumberOfShots != 12 || m.NumberInVolley != 5 || m.NumOfVolleys != 2 || m.CreateAmmoEffectCount != 2) { std::printf("007cbf20_TEST FAIL\n"); return 1; }
    m.SetAICast(1);
    if (m.NumOfVolleys != 3 || m.CreateAmmoEffectCount != 3) { std::printf("007cbf20_TEST FAIL\n"); return 1; }
    std::printf("007cbf20_TEST PASS\n");
    return 0;
}
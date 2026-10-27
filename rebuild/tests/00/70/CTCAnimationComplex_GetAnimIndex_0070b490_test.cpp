// Behaviour test for CTCAnimationComplex::GetAnimIndex (0x0070B490): the sign-extended first short of the main anim set's Anims.
#include <cstdio>
struct ShortVector { short* start; short* finish; short* eos; };
class CAnimSet { public: int UpdateMode; ShortVector Anims; int GetAnimIndex() const { return Anims.start[0]; } };
struct CCompleteAnimation { CAnimSet MainAnimSet; };
class CAnimateThing { public: CCompleteAnimation CurrentAnim; };
class CTCAnimationComplex { public:
    CAnimateThing* PAnimation;
    int GetAnimIndex() const { return PAnimation->CurrentAnim.MainAnimSet.GetAnimIndex(); }
};
int main()
{
    short anims[2] = { -3, 9 };
    CAnimateThing a; a.CurrentAnim.MainAnimSet.Anims.start = anims;
    CTCAnimationComplex c; c.PAnimation = &a;
    if (c.GetAnimIndex() != -3) { std::printf("0070b490_TEST FAIL\n"); return 1; }
    anims[0] = 1234;
    if (c.GetAnimIndex() != 1234) { std::printf("0070b490_TEST FAIL\n"); return 1; }
    std::printf("0070b490_TEST PASS\n");
    return 0;
}
// Behaviour test for CTCCreatureOpinionOfHero::IsWillingToAttackHeroOnOpinion (0x006817F0): a model of the row -- an opinion
// enemy always attacks; otherwise attitudes 2, 4, 5, 6 and 12..17 refuse and every other attitude attacks.
#include <stdio.h>
static bool Willing(bool enemy, long attitude)
{
    if (enemy)
        return true;
    unsigned long index = attitude - 2;
    if (index > 15)
        return true;
    switch (index) {
    case 0: case 2: case 3: case 4: case 10: case 11: case 12: case 13: case 14: case 15:
        return false;
    default:
        return true;
    }
}
int main()
{
    int ok = Willing(true, 2);
    for (long a = -1; a < 25; ++a) {
        bool refuse = a == 2 || (a >= 4 && a <= 6) || (a >= 12 && a <= 17);
        ok = ok && Willing(false, a) == !refuse;
    }
    printf(ok ? "006817f0_TEST PASS\n" : "006817f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
// Behaviour test for 0x005926CE std::__copy<CFadeInFadeOutBase*> random-access copy (model copy)
#include <cstdio>
struct Fade {
    bool Active; float FadeInTime; float FadeOutTime; bool Closing; bool Opening; float OpenTimer; float CloseTimer;
    unsigned char ToColour[4];
};
struct RandomTag { RandomTag() {} };
static Fade* copy_ra(Fade* first, Fade* last, Fade* result, const RandomTag&, int*)
{
    for (int n = int(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    Fade f[4];
    for (int i = 0; i < 4; ++i) {
        f[i].Active = (i & 1) != 0; f[i].FadeInTime = 1.0f * i; f[i].FadeOutTime = 2.0f * i; f[i].Closing = false;
        f[i].Opening = true; f[i].OpenTimer = 0.5f * i; f[i].CloseTimer = 0.25f * i; f[i].ToColour[0] = (unsigned char)i;
    }
    Fade* end = copy_ra(f + 1, f + 4, f, RandomTag(), (int*)0);
    if (end != f + 3 || f[0].FadeInTime != 1.0f || !f[0].Active || f[2].ToColour[0] != 3 || f[2].CloseTimer != 0.75f) {
        std::printf("005926ce_TEST FAIL\n"); return 1;
    }
    std::printf("005926ce_TEST PASS\n");
    return 0;
}
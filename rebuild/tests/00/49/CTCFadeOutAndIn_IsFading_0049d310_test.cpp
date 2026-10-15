#include <cstdio>
struct CTCFadeOutAndIn {
    bool FadingOut; bool FadingIn; float Alpha; long FramesLeftForFade;
    bool IsFading() const { return FadingIn || FadingOut; }
};
int main()
{
    CTCFadeOutAndIn f; int n = 0;
    for (int i = 0; i < 4; ++i) { f.FadingOut = (i & 1) != 0; f.FadingIn = (i & 2) != 0; if (f.IsFading() != (i != 0)) { std::printf("0049d310_TEST FAIL\n"); return 1; } ++n; }
    if (n != 4) { std::printf("0049d310_TEST FAIL\n"); return 1; }
    std::printf("0049d310_TEST PASS\n");
    return 0;
}
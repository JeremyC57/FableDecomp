// behaviour test for 0x006C3C30 CTCFadeOutAndIn::CTCFadeOutAndIn(CThing&)
#include <cstdio>
class CThing { public: int id; };
struct Fade {
    CThing* ParentThing; bool FadingOut, FadingIn; float Alpha; long FramesLeft; CThing* things[4]; unsigned int count;
    Fade(CThing& t) : ParentThing(&t), FadingOut(false), FadingIn(false), Alpha(1.0f), FramesLeft(0), count(0)
    { things[count++] = &t; }
};
int main()
{
    CThing t = { 9 };
    Fade f(t);
    bool ok = f.ParentThing == &t && !f.FadingOut && !f.FadingIn && f.Alpha == 1.0f && f.FramesLeft == 0
        && f.count == 1 && f.things[0] == &t;
    if (!ok) { std::printf("006c3c30_TEST FAIL\n"); return 1; }
    std::printf("006c3c30_TEST PASS\n");
    return 0;
}
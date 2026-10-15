#include <cstdio>
static int g_updates = 0;
class CTCBase {
public:
    virtual ~CTCBase() { }
    virtual void FinalCleanup() { }
    virtual void FrameUpdate() { }
};
class CTCFadeOutAndIn : public CTCBase {
public:
    bool FadingOut; bool FadingIn; float Alpha; long FramesLeftForFade;
    bool IsFading() const { return FadingIn || FadingOut; }
    virtual void FrameUpdate() { ++g_updates; if (--FramesLeftForFade == 0) { FadingIn = false; FadingOut = false; } }
    virtual void FinalCleanup() { if (IsFading()) { FramesLeftForFade = 1; FrameUpdate(); } }
};
int main()
{
    CTCFadeOutAndIn f; f.FadingOut = false; f.FadingIn = false; f.FramesLeftForFade = 9;
    CTCBase* b = &f;
    b->FinalCleanup();
    bool idle = g_updates == 0 && f.FramesLeftForFade == 9;
    f.FadingOut = true; f.FramesLeftForFade = 40;
    b->FinalCleanup();
    if (!idle || g_updates != 1 || f.FramesLeftForFade != 0 || f.IsFading()) { std::printf("006c3b50_TEST FAIL\n"); return 1; }
    std::printf("006c3b50_TEST PASS\n");
    return 0;
}

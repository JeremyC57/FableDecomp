#include <cstdio>
static long g_frame = 0;
static long GetWorldFrame() { return g_frame; }
struct CTCWife {
    long FrameGotMarriedToThePlayer; bool NeedsToChangeBrain; bool iconShown; float iconScale; unsigned char iconAlpha;
    bool IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= GetWorldFrame(); }
    void SetAttractionIcon(bool show, float scale, unsigned char alpha) { iconShown = show; iconScale = scale; iconAlpha = alpha; }
    void InitWifeNonpersistentState() { if (IsMarriedToThePlayer()) { SetAttractionIcon(true, 0.5f, 255); NeedsToChangeBrain = true; } }
    void OnInitialActivate() { InitWifeNonpersistentState(); }
};
int main()
{
    CTCWife w; w.FrameGotMarriedToThePlayer = 100; w.NeedsToChangeBrain = false; w.iconShown = false; w.iconScale = 0; w.iconAlpha = 0;
    g_frame = 50; w.OnInitialActivate();
    if (w.NeedsToChangeBrain || w.iconShown) { std::printf("006d0800_TEST FAIL\n"); return 1; }
    g_frame = 100; w.OnInitialActivate();
    if (!w.NeedsToChangeBrain || !w.iconShown || w.iconScale != 0.5f || w.iconAlpha != 255) { std::printf("006d0800_TEST FAIL\n"); return 1; }
    std::printf("006d0800_TEST PASS\n");
    return 0;
}

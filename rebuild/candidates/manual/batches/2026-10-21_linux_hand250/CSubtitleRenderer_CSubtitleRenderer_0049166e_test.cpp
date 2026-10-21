// Behaviour test for CSubtitleRenderer::CSubtitleRenderer(CGraphicDataBank*) (0x0049166E): the member defaults.
#include <cstdio>
struct CGraphicDataBank { int dummy; };
static int g_vecCounts[4]; static int g_nVec = 0; static int g_nFade = 0; static int g_nStr = 0;
struct CWideString { void* PStringData; CWideString() : PStringData(0) { ++g_nStr; } };
struct CHandleVector { void* f; void* l; void* e; CHandleVector(unsigned long n) : f(0), l(0), e(0) { g_vecCounts[g_nVec++] = (int)n; } };
struct CVec { void* f; void* l; void* e; CVec() : f(0), l(0), e(0) {} };
struct CFadeTimer { int x; CFadeTimer() : x(0) { ++g_nFade; } };
struct CSubtitleRenderer {
    long TextCharactersShowing; bool TextGroupHasEntriesRemaining; long TextGroupIndex; CWideString TextGroupTag;
    CWideString Text, NameText, Button1Text, Button2Text, Button3Text, Button1OverrideText, Button2OverrideText;
    unsigned char Button1Alpha, Button2Alpha; unsigned long DisplayFlags; void* PFont; CGraphicDataBank* Graphics;
    float MaxDisplayTime, TextTimer; long PlayerModeAdded; CHandleVector PrimitiveHandles, ButtonPrimitiveHandles;
    CVec EmbeddedPrimitiveHandles, EmbeddedGraphics, EmbeddedHighlights, EmbeddedHighlightActionNames;
    bool PausedGame, AllCharactersDisplayed, InCutscene, CutsceneSkipped, CutsceneSkippable, CutsceneSkippableWhilePaused,
         CutsceneActionMode, Button1TextOverridden, Button2TextOverridden, DisplayingText, UpdateWhilePaused, UpdateTimer;
    CFadeTimer SkipCutsceneTimer, ButtonPulseTimer;
    CSubtitleRenderer(CGraphicDataBank* graphics)
        : TextCharactersShowing(0), TextGroupHasEntriesRemaining(false), TextGroupIndex(-1),
          Button1Alpha(0xff), Button2Alpha(0xff), DisplayFlags(0), PFont(0), Graphics(graphics),
          MaxDisplayTime(0.0f), TextTimer(0.0f), PlayerModeAdded(0), PrimitiveHandles(9), ButtonPrimitiveHandles(3),
          PausedGame(false), AllCharactersDisplayed(false), InCutscene(false), CutsceneSkipped(false),
          CutsceneSkippable(true), CutsceneSkippableWhilePaused(false), CutsceneActionMode(false),
          Button2TextOverridden(false), DisplayingText(false), UpdateWhilePaused(false), UpdateTimer(true) {}
};
int main()
{
    CGraphicDataBank bank;
    CSubtitleRenderer r(&bank);
    if (r.TextGroupIndex != -1 || r.Graphics != &bank || r.Button1Alpha != 0xff || r.Button2Alpha != 0xff || r.DisplayFlags != 0
        || r.MaxDisplayTime != 0.0f || !r.CutsceneSkippable || !r.UpdateTimer || r.PausedGame || r.DisplayingText
        || g_nVec != 2 || g_vecCounts[0] != 9 || g_vecCounts[1] != 3 || g_nFade != 2 || g_nStr != 8 || r.EmbeddedGraphics.f != 0)
    { std::printf("0049166e_TEST FAIL\n"); return 1; }
    std::printf("0049166e_TEST PASS\n");
    return 0;
}

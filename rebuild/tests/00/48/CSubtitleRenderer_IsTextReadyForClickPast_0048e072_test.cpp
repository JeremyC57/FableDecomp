// Behaviour test for 0x0048E072 CSubtitleRenderer::IsTextReadyForClickPast (model copy; the skippable test is stubbed).
#include <cstdio>

struct CSubtitleRendererModel {
    unsigned long DisplayFlags;
    float         TextTimer;
    bool          m_skippable;          // stands in for IsCutsceneSkippable() (0x0048DE44)
    bool IsCutsceneSkippable() const { return m_skippable; }
    bool IsTextReadyForClickPast() const
    {
        if (!IsCutsceneSkippable() && !(DisplayFlags & 0x48))
            return false;
        return (DisplayFlags & 2) && TextTimer > 0.2f;
    }
};

static int check(bool skippable, unsigned long flags, float timer, bool expect)
{
    CSubtitleRendererModel r;
    r.m_skippable = skippable;
    r.DisplayFlags = flags;
    r.TextTimer = timer;
    return r.IsTextReadyForClickPast() == expect;
}

int main()
{
    int ok = check(true, 2, 0.5f, true)
          && check(true, 2, 0.1f, false)
          && check(true, 0, 0.5f, false)
          && check(false, 2, 0.5f, false)
          && check(false, 2 | 0x40, 0.5f, true)
          && check(false, 2 | 0x08, 0.3f, true)
          && check(false, 0x48, 0.3f, false)
          && check(true, 2, 0.2f, false);
    if (!ok) { std::printf("0048e072_TEST FAIL\n"); return 1; }
    std::printf("0048e072_TEST PASS\n");
    return 0;
}
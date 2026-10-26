// Behaviour test for CTCWife::GetAppearanceChangedNoticed (0x006CED50): only categories noticed (frame > 0) within the
// def window count; among them the highest-priority one is reported (first wins on ties); false and the out value
// untouched when none qualifies.
#include <cstdio>

typedef long EWifeAppearanceCategories;
static long g_now = 1000;
long GFGetCurrentFrame() { return g_now; }
long g_FramesPerSecond = 10;
const long g_AppearanceCategoryPriority[16] = { 1, 5, 3, 5, 2, 9, 4, 4, 7, 1, 1, 8, 6, 2, 3, 9 };

class CHeroMarriageDef { public: long AppearanceChangeNoticeSeconds; };
class CTCWife { public:
    CHeroMarriageDef* PHeroMarriageDef;
    long FrameNoticedChangeInAppearance[16];
    bool GetAppearanceChangedNoticed(EWifeAppearanceCategories* category);
};

bool CTCWife::GetAppearanceChangedNoticed(EWifeAppearanceCategories* category)
{
    long now = GFGetCurrentFrame();
    long best = 0;
    bool found = false;
    for (long i = 0; i < 16; ++i) {
        long noticed = FrameNoticedChangeInAppearance[i];
        if (noticed > 0 && now - noticed < PHeroMarriageDef->AppearanceChangeNoticeSeconds * g_FramesPerSecond
            && g_AppearanceCategoryPriority[i] > best) {
            best = g_AppearanceCategoryPriority[i];
            *category = i;
            found = true;
        }
    }
    return found;
}

int main()
{
    CHeroMarriageDef def = { 5 };                       // window 50 frames
    CTCWife w;
    w.PHeroMarriageDef = &def;
    for (int i = 0; i < 16; ++i) w.FrameNoticedChangeInAppearance[i] = 0;
    EWifeAppearanceCategories c = -7;
    int ok = !w.GetAppearanceChangedNoticed(&c) && c == -7;
    w.FrameNoticedChangeInAppearance[1] = 990;          // priority 5, in window
    w.FrameNoticedChangeInAppearance[3] = 980;          // priority 5, tie: first stays
    w.FrameNoticedChangeInAppearance[5] = 900;          // priority 9 but out of window
    w.FrameNoticedChangeInAppearance[15] = -3;          // not noticed
    ok = ok && w.GetAppearanceChangedNoticed(&c) && c == 1;
    w.FrameNoticedChangeInAppearance[11] = 960;         // priority 8, in window
    ok = ok && w.GetAppearanceChangedNoticed(&c) && c == 11;
    w.FrameNoticedChangeInAppearance[5] = 951;          // now inside (1000-951 = 49 < 50)
    ok = ok && w.GetAppearanceChangedNoticed(&c) && c == 5;
    w.FrameNoticedChangeInAppearance[5] = 950;          // exactly 50: outside
    ok = ok && w.GetAppearanceChangedNoticed(&c) && c == 11;
    if (!ok) { std::printf("006ced50_TEST FAIL\n"); return 1; }
    std::printf("006ced50_TEST PASS\n");
    return 0;
}
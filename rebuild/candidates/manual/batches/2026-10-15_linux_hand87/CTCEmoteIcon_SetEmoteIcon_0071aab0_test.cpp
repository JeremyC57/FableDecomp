#include <cstdio>
struct CTCEmoteIcon {
    void* vptrSlot; void* ParentThing; bool m_flags[4];
    long ActiveTimer; void* EmoteText; long EmoteIcon; float EmoteScale; long EmoteType; long EmotePriority;
    unsigned char EmoteAlpha; bool Replaceable; bool Enabled;
    void SetActiveTimer(float seconds) { ActiveTimer = seconds == -1.0f ? -1 : (long)(seconds * 30.0f); }
    void SetEmoteIcon(long priority, long icon, float seconds, float scale, unsigned char alpha)
    {
        if (priority > EmotePriority)
            return;
        if (icon > 0) {
            EmoteIcon = icon; EmoteScale = scale; EmotePriority = priority; EmoteType = 1; EmoteAlpha = alpha;
            SetActiveTimer(seconds);
        } else {
            EmotePriority = 8; EmoteIcon = 0;
            SetActiveTimer(0.0f);
        }
    }
};
int main()
{
    CTCEmoteIcon e; e.EmotePriority = 3; e.EmoteIcon = 9; e.EmoteType = 0; e.EmoteAlpha = 0; e.EmoteScale = 0.0f; e.ActiveTimer = 5;
    e.SetEmoteIcon(4, 2, 1.0f, 2.0f, 200);                 // lower priority (higher number): ignored
    bool ignored = e.EmoteIcon == 9 && e.EmotePriority == 3 && e.ActiveTimer == 5;
    e.SetEmoteIcon(2, 11, 2.0f, 1.5f, 128);
    bool set = e.EmoteIcon == 11 && e.EmotePriority == 2 && e.EmoteType == 1 && e.EmoteAlpha == 128 && e.EmoteScale == 1.5f && e.ActiveTimer == 60;
    e.SetEmoteIcon(2, 0, 3.0f, 1.0f, 1);                   // no icon: clear
    bool cleared = e.EmoteIcon == 0 && e.EmotePriority == 8 && e.ActiveTimer == 0 && e.EmoteAlpha == 128;
    if (!ignored || !set || !cleared) { std::printf("0071aab0_TEST FAIL\n"); return 1; }
    std::printf("0071aab0_TEST PASS\n");
    return 0;
}

// Behaviour test for 0x006D5050 CTCFollowed::RemoveEmoteIcon: a follower with an emote-icon component that still
// shows the global follower icon gets SetEmoteIcon(1, 0, 0.0f, 1.0f, 0xFF); any other icon, or no component, is left.
#include <cstdio>

struct CTCEmoteIcon {
    long EmoteIcon;
    int  calls; int lastPriority; long lastIcon; float lastTime; float lastScale; unsigned char lastAlpha;
    void SetEmoteIcon(int priority, long icon, float t, float scale, unsigned char alpha)
    { ++calls; lastPriority = priority; lastIcon = icon; lastTime = t; lastScale = scale; lastAlpha = alpha; EmoteIcon = icon; }
};
struct CGlobalGameDef { long FollowerEmoteIcon; };
struct CDefinitions { CGlobalGameDef* PGlobalDef; };
struct CThing {
    CTCEmoteIcon* m_icon;
    bool HasInterface(long) const { return m_icon != 0; }
    CTCEmoteIcon* GetInterface(long) { return m_icon; }
};
struct CTCFollowed {
    CDefinitions* m_defs;
    CDefinitions* GetDefinitions() { return m_defs; }
    void RemoveEmoteIcon(CThing& follower)
    {
        if (follower.HasInterface(0x45)) {
            CTCEmoteIcon* icon = follower.GetInterface(0x45);
            long shown = icon->EmoteIcon;
            if (shown == GetDefinitions()->PGlobalDef->FollowerEmoteIcon)
                icon->SetEmoteIcon(1, 0, 0.0f, 1.0f, 0xff);
        }
    }
};

int main()
{
    CGlobalGameDef g; g.FollowerEmoteIcon = 12;
    CDefinitions d; d.PGlobalDef = &g;
    CTCFollowed f; f.m_defs = &d;
    CTCEmoteIcon icon; icon.calls = 0; icon.EmoteIcon = 12;
    CThing t; t.m_icon = &icon;
    f.RemoveEmoteIcon(t);
    if (icon.calls != 1 || icon.lastPriority != 1 || icon.lastIcon != 0 || icon.lastTime != 0.0f || icon.lastScale != 1.0f || icon.lastAlpha != 0xff) { std::printf("006d5050_TEST FAIL\n"); return 1; }
    icon.EmoteIcon = 5; f.RemoveEmoteIcon(t);
    if (icon.calls != 1) { std::printf("006d5050_TEST FAIL\n"); return 1; }
    CThing none; none.m_icon = 0; f.RemoveEmoteIcon(none);
    std::printf("006d5050_TEST PASS\n");
    return 0;
}
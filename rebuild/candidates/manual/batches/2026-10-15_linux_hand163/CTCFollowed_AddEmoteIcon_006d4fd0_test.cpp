// behaviour test for 0x006D4FD0 CTCFollowed::AddEmoteIcon
#include <cstdio>
struct Icon { long EmoteIcon; int calls; float t, s; unsigned char alpha;
    void SetEmoteIcon(int prio, long icon, float time, float scale, unsigned char a) { if (prio == 1) { EmoteIcon = icon; t = time; s = scale; alpha = a; ++calls; } } };
struct Thing { Icon* icon; unsigned char Flags6c; };
static long g_followerIcon = 77;
static void AddEmoteIcon(Thing* parent, Thing& follower)
{
    if (follower.icon)
    {
        Icon* icon = follower.icon;
        if (parent->Flags6c & 0x20)
        {
            long followerIcon = g_followerIcon;
            if (icon->EmoteIcon != followerIcon) icon->SetEmoteIcon(1, followerIcon, -1.0f, 0.5f, 0x80);
        }
    }
}
int main()
{
    Icon i = { 0, 0, 0, 0, 0 };
    Thing parent = { 0, 0x20 }, f = { &i, 0 };
    AddEmoteIcon(&parent, f);
    bool ok = i.calls == 1 && i.EmoteIcon == 77 && i.t == -1.0f && i.s == 0.5f && i.alpha == 0x80;
    AddEmoteIcon(&parent, f); ok = ok && i.calls == 1;
    i.EmoteIcon = 1; parent.Flags6c = 0; AddEmoteIcon(&parent, f); ok = ok && i.calls == 1;
    if (!ok) { std::printf("006d4fd0_TEST FAIL\n"); return 1; }
    std::printf("006d4fd0_TEST PASS\n");
    return 0;
}

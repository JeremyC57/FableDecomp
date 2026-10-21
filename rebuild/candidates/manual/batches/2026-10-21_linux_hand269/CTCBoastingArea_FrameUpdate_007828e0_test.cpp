// Behaviour test for 0x007828E0 CTCBoastingArea::FrameUpdate: the hero-within-radius state machine.
#include <cstdio>

struct V { float X, Y, Z; };
static bool InRange(const V& a, const V& b, float r)
{
    float dx = a.X - b.X, dy = a.Y - b.Y, dz = a.Z - b.Z;
    return dx * dx + (dy * dy + dz * dz) < r * r;
}
static int g_enter = 0, g_leave = 0, g_approach = 0;
struct Area {
    long State; float Radius, AnticipationRadius; V pos;
    void FrameUpdate(const V* hero, bool hasCrowd)
    {
        if (!hero) { State = 0; return; }
        bool inAnticipation = InRange(pos, *hero, AnticipationRadius);
        bool inRadius = InRange(pos, *hero, Radius);
        switch (State) {
        case 0:
            if (!inAnticipation) return;
            if (inRadius) { State = 2; ++g_enter; return; }
            State = 1;
            if (hasCrowd) ++g_approach;
            return;
        case 1:
            if (!inAnticipation) { State = 0; return; }
            if (inRadius) { State = 2; ++g_enter; }
            return;
        case 2:
            if (inRadius) return;
            if (inAnticipation) { State = 1; ++g_leave; } else { State = 0; ++g_leave; }
            return;
        }
    }
};
int main()
{
    Area a = { 0, 2.0f, 5.0f, { 0, 0, 0 } };
    V far_ = { 10, 0, 0 }, mid = { 0, 3, 0 }, near_ = { 0, 0, 1 };
    a.FrameUpdate(&far_, true);  bool ok = a.State == 0;
    a.FrameUpdate(&mid, true);   ok = ok && a.State == 1 && g_approach == 1;
    a.FrameUpdate(&near_, true); ok = ok && a.State == 2 && g_enter == 1;
    a.FrameUpdate(&mid, true);   ok = ok && a.State == 1 && g_leave == 1;
    a.FrameUpdate(&far_, true);  ok = ok && a.State == 0;
    a.FrameUpdate(&near_, false); ok = ok && a.State == 2 && g_enter == 2;
    a.FrameUpdate(&far_, true);  ok = ok && a.State == 0 && g_leave == 2;
    a.State = 2; a.FrameUpdate(0, true); ok = ok && a.State == 0;
    if (!ok) { std::printf("007828e0_TEST FAIL\n"); return 1; }
    std::printf("007828e0_TEST PASS\n");
    return 0;
}

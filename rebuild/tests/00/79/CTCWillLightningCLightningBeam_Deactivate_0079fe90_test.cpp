#include <cstdio>
// Behaviour model of CLightningBeam::Deactivate (0x0079FE90): recursive state transitions and the hit-sound choice.
struct Beam {
    int state; float time, timeBase; float end, dest; Beam* children[3]; int childCount;
    bool temporary; bool targetAlive; int attachedPlays; int unattachedPlays;
};
static void Deactivate(Beam& b, bool playerAlive)
{
    if (b.state == 3 || b.state == 2 || b.state == 1) {
        b.dest = b.end;
        for (int i = 0; i < b.childCount; ++i) Deactivate(*b.children[i], playerAlive);
        if (b.state != 2 && b.childCount != 0) {
            b.timeBase = b.time;
            b.state = b.state == 1 ? 6 : 4;
            return;
        }
        b.timeBase = b.time;
        b.state = 5;
        if (b.targetAlive) ++b.attachedPlays;
        else if (playerAlive && !b.temporary) ++b.unattachedPlays;
    }
}
int main()
{
    Beam leaf = { 3, 2.0f, 0.0f, 7.0f, 0.0f, { 0, 0, 0 }, 0, false, true, 0, 0 };
    Beam leaf2 = { 2, 3.0f, 0.0f, 8.0f, 0.0f, { 0, 0, 0 }, 0, true, false, 0, 0 };
    Beam root = { 1, 5.0f, 0.0f, 9.0f, 0.0f, { &leaf, &leaf2, 0 }, 2, false, false, 0, 0 };
    Deactivate(root, true);
    int ok = root.state == 6 && root.timeBase == 5.0f && root.dest == 9.0f && root.attachedPlays == 0;
    ok = ok && leaf.state == 5 && leaf.attachedPlays == 1 && leaf.dest == 7.0f;
    ok = ok && leaf2.state == 5 && leaf2.unattachedPlays == 0;
    Beam mid = { 2, 1.0f, 0.0f, 1.0f, 0.0f, { &leaf, 0, 0 }, 1, false, false, 0, 0 };
    Deactivate(mid, true); ok = ok && mid.state == 5 && mid.unattachedPlays == 1;
    Beam idle = { 0, 1.0f, 4.0f, 1.0f, 0.0f, { 0, 0, 0 }, 0, false, true, 0, 0 };
    Deactivate(idle, true); ok = ok && idle.state == 0 && idle.timeBase == 4.0f;
    if (!ok) { std::printf("0079fe90_TEST FAIL\n"); return 1; }
    std::printf("0079fe90_TEST PASS\n");
    return 0;
}
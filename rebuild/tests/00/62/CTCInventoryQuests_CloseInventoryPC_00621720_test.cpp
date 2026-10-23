// Behaviour test for CTCInventoryQuests::CloseInventoryPC @ 0x00621720: UI teardown only when the info screen is inactive; always clears cards/handles
#include <cstdio>
#include <vector>
struct State { int screenActive; int soundCalls; int components; int cards; int handles; int hudEnabled; long closedFrame; int previousScreenSet; };
static void ClosePC(State& s, long frame)
{
    if (!s.screenActive) {
        s.soundCalls = 3;
        s.previousScreenSet = 1;
        s.components = 0;
    }
    s.handles = 0;
    s.cards = 0;
    s.hudEnabled = 1;
    s.closedFrame = frame;
}
int main()
{
    int ok = 1;
    State a = { 0, 0, 5, 3, 2, 0, 0, 0 };
    ClosePC(a, 77);
    if (a.soundCalls != 3 || a.components != 0 || a.cards != 0 || a.handles != 0 || !a.hudEnabled || a.closedFrame != 77 || !a.previousScreenSet) ok = 0;
    State b = { 1, 0, 5, 3, 2, 0, 0, 0 };
    ClosePC(b, 9);
    if (b.soundCalls != 0 || b.components != 5 || b.cards != 0 || b.handles != 0 || b.previousScreenSet) ok = 0;
    if (!ok) { std::printf("00621720_TEST FAIL\n"); return 1; }
    std::printf("00621720_TEST PASS\n");
    return 0;
}
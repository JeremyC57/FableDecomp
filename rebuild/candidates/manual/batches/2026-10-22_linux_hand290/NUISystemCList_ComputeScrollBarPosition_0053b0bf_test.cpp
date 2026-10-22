// Behaviour test for NUISystem::CList::ComputeScrollBarPosition (0x0053B0BF): with 6 children, 3 visible and 2 hidden before
// the selection, the thumb moves 2/3 of the free bar length; the up/down steps are (0, length / hidden) and counts 2 / 1;
// with nothing hidden before the selection the up step is (-1,-1) with count 0.
#include <cstdio>

struct Bar { float y; float upX, upY, downX, downY; unsigned upCount, downCount; bool moved; };
static void Compute(const unsigned char* vis, unsigned n, unsigned selected, float barHeight, float thumbHeight, float scaleY,
                    float baseY, Bar& bar)
{
    unsigned visible = 0, hiddenBefore = 0;
    for (unsigned i = 0; i < n; ++i) {
        if (vis[i]) ++visible;
        else if (i < selected) ++hiddenBefore;
    }
    unsigned hidden = n - visible;
    unsigned hiddenAfter = hidden - hiddenBefore;
    float y = baseY;
    float length = barHeight - thumbHeight * scaleY;
    if (hidden > 0) y += (float)hiddenBefore / (float)hidden * length;
    bar.y = y; bar.moved = true;
    if (visible < n) {
        float step = length / (float)hidden;
        if (hiddenBefore) { bar.upX = 0.0f; bar.upY = step; bar.upCount = hiddenBefore; }
        else { bar.upX = -1.0f; bar.upY = -1.0f; bar.upCount = 0; }
        if (hiddenAfter) { bar.downX = 0.0f; bar.downY = step; bar.downCount = hiddenAfter; }
        else { bar.downX = -1.0f; bar.downY = -1.0f; bar.downCount = 0; }
    }
}

int main()
{
    unsigned char vis[6] = { 0, 0, 1, 1, 1, 0 };
    Bar bar = { 0, 0, 0, 0, 0, 0, 0, false };
    Compute(vis, 6, 3, 100.0f, 40.0f, 0.5f, 10.0f, bar);   // length 80, hidden 3, before 2
    bool ok = bar.y > 63.3f && bar.y < 63.4f && bar.upCount == 2 && bar.downCount == 1 && bar.upY > 26.6f && bar.upY < 26.7f;
    unsigned char vis2[3] = { 1, 1, 0 };
    Bar bar2 = { 0, 0, 0, 0, 0, 0, 0, false };
    Compute(vis2, 3, 0, 100.0f, 40.0f, 1.0f, 5.0f, bar2);
    ok = ok && bar2.y == 5.0f && bar2.upX == -1.0f && bar2.upCount == 0 && bar2.downCount == 1 && bar2.downY == 60.0f;
    std::printf(ok ? "0053b0bf_TEST PASS\n" : "0053b0bf_TEST FAIL\n");
    return ok ? 0 : 1;
}

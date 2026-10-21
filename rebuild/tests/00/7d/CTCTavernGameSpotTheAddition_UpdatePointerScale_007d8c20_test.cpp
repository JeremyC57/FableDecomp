// Behaviour test for CTCTavernGameSpotTheAddition::UpdatePointerScale (0x007D8C20): no pointer, nothing happens;
// selecting (state 0x2A, nothing selected) grows the scale by the def's phase speed from at least 0.1 up to a cap of 1;
// otherwise the scale shrinks by the phase speed and snaps to 0 below 0.1; the scale is pushed to the appearance (0x5B)
// when the pointer has one.
#include <cstdio>

struct Game {
    bool hasPointer; bool hasAppearance; long state; long selected; float scale; float speed; float pushed; int defs;
    void Update()
    {
        if (!hasPointer) return;
        if (state == 0x2a && selected == -1) {
            if (scale != 1.0f) {
                if (scale < 0.1f) scale = 0.1f;
                else if (scale < 1.0f) { ++defs; scale *= speed; }
                if (scale > 1.0f) scale = 1.0f;
            }
        } else if (scale != 0.0f) {
            if (scale < 0.1f) scale = 0.0f;
            else { ++defs; scale /= speed; }
        }
        if (hasAppearance) pushed = scale;
    }
};

int main()
{
    int ok = 1;
    Game g = { true, true, 0x2a, -1, 0.0f, 2.0f, -1.0f, 0 };
    g.Update(); ok &= g.scale == 0.1f && g.pushed == 0.1f && g.defs == 0;
    g.Update(); ok &= g.scale == 0.2f && g.defs == 1;
    g.scale = 0.75f; g.Update(); ok &= g.scale == 1.0f;
    g.Update(); ok &= g.scale == 1.0f && g.defs == 2;
    g.selected = 3; g.Update(); ok &= g.scale == 0.5f && g.pushed == 0.5f;
    g.scale = 0.05f; g.Update(); ok &= g.scale == 0.0f;
    g.hasAppearance = false; g.pushed = 7; g.Update(); ok &= g.pushed == 7 && g.defs == 3;
    Game n = { false, true, 0x2a, -1, 0.0f, 2.0f, -1.0f, 0 };
    n.Update(); ok &= n.scale == 0.0f && n.pushed == -1.0f;
    std::printf(ok ? "007d8c20_TEST PASS\n" : "007d8c20_TEST FAIL\n");
    return ok ? 0 : 1;
}
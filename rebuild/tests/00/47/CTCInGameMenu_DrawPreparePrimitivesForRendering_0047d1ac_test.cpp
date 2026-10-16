#include <cstdio>
// Behaviour model of CTCInGameMenu::DrawPreparePrimitivesForRendering (0x0047D1AC): only while the menu is open, each
// component is ticked by (now - last render time) and then prepares its primitives; the render time is updated.
struct Comp { float ticked; int prepared; void* target; };
struct Menu {
    bool open; double last; Comp* comps; unsigned int n;
    void Draw(double now, void* target)
    {
        if (open) {
            float seconds = (float)(now - last);
            last = now;
            for (unsigned int i = 0; i < n; ++i) { comps[i].ticked = seconds; ++comps[i].prepared; comps[i].target = target; }
        }
    }
};
int main()
{
    Comp c[2] = { { 0, 0, 0 }, { 0, 0, 0 } };
    Menu m = { false, 1.0, c, 2 };
    int tgt;
    m.Draw(3.0, &tgt);
    if (c[0].prepared || m.last != 1.0) { std::printf("0047d1ac_TEST FAIL\n"); return 1; }
    m.open = true; m.Draw(3.5, &tgt);
    if (c[0].ticked != 2.5f || c[1].prepared != 1 || c[1].target != &tgt || m.last != 3.5) { std::printf("0047d1ac_TEST FAIL\n"); return 1; }
    std::printf("0047d1ac_TEST PASS\n");
    return 0;
}
#include <cstdio>
// Behaviour model of CTCInventoryBase::SelectPreviousCategory (0x005BDC08): with at least one category and no selection
// in progress, deselects the current one, steps back with wrap-around and starts a -1 slide.
struct Inv {
    unsigned int n; long state; long sel; bool selected[4]; float move, dir; bool moving; int changed;
    void Prev()
    {
        if (n >= 1 && state == 0) {
            if (selected[sel]) selected[sel] = false;
            if (--sel < 0) sel = (long)n - 1;
            move = 1.0f; moving = true; dir = -1.0f; ++changed;
        }
    }
};
int main()
{
    Inv v = { 3, 0, 0, { true, false, false, false }, 0, 0, false, 0 };
    v.Prev();
    if (v.sel != 2 || v.selected[0] || v.dir != -1.0f || !v.moving || v.changed != 1) { std::printf("005bdc08_TEST FAIL\n"); return 1; }
    v.Prev();
    if (v.sel != 1 || v.changed != 2) { std::printf("005bdc08_TEST FAIL\n"); return 1; }
    v.n = 0; v.Prev();
    if (v.changed != 2) { std::printf("005bdc08_TEST FAIL\n"); return 1; }
    std::printf("005bdc08_TEST PASS\n");
    return 0;
}

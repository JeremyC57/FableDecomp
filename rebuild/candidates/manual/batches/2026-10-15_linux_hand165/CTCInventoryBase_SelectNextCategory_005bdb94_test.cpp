#include <cstdio>
// Behaviour model of CTCInventoryBase::SelectNextCategory (0x005BDB94): with more than one category and no selection in
// progress, deselects the current one, advances with wrap-around and starts a +1 slide.
struct Inv {
    unsigned int n; long state; long sel; bool selected[4]; float move, dir; bool moving; int changed;
    void Next()
    {
        if (n > 1 && state == 0) {
            if (selected[sel]) selected[sel] = false;
            ++sel;
            if ((unsigned int)sel >= n) sel = 0;
            move = 1.0f; moving = true; dir = 1.0f; ++changed;
        }
    }
};
int main()
{
    Inv v = { 3, 0, 2, { false, false, true, false }, 0, 0, false, 0 };
    v.Next();
    if (v.sel != 0 || v.selected[2] || v.dir != 1.0f || !v.moving || v.changed != 1) { std::printf("005bdb94_TEST FAIL\n"); return 1; }
    v.state = 1; v.Next();
    if (v.sel != 0 || v.changed != 1) { std::printf("005bdb94_TEST FAIL\n"); return 1; }
    v.state = 0; v.n = 1; v.Next();
    if (v.changed != 1) { std::printf("005bdb94_TEST FAIL\n"); return 1; }
    std::printf("005bdb94_TEST PASS\n");
    return 0;
}

// Behaviour test for NUISystem::CList::ScrollDown(first, last, useStates, moveSelection, force) (0x005392FC):
// a copy of the shift on a five-child list model (target position Y, alpha, zoom per child plus the saved
// LIST_ACTIVE record) checks the selection move, the per-child hand-over and the head/tail fade-in.
#include <cstdio>
struct Child { float y; unsigned char a; float zoom; };
struct List {
    Child c[5]; Child saved[5]; long n; long sel; float offY; float alphaOffset; bool wrapping;
    void Up(unsigned long first, unsigned long last, bool useStates, bool move)
    {
        if (n <= 1) return;
        if (move) {
            if (sel == n - 1 && !wrapping) return;
            if ((unsigned long)sel >= first && (unsigned long)sel <= last) { ++sel; sel = sel < n ? sel : 0; }
        }
        Child fill = useStates ? saved[first == 0 ? last : first - 1] : c[first == 0 ? last : first - 1];
        for (long i = last; i > (long)first; --i) { c[i] = c[i - 1]; if (useStates) saved[i] = saved[i - 1]; else saved[i] = c[i]; }
        if (!wrapping && first == 0) {
            Child src = useStates ? saved[0] : c[0];
            float alpha = src.a - alphaOffset * 255.0f;
            fill.y = src.y - offY; fill.a = (unsigned char)(alpha < 0.0f ? 0.0f : alpha);
            c[0].y = fill.y; c[0].a = fill.a; saved[0].y = fill.y; saved[0].a = fill.a;
        } else { c[first] = fill; saved[first] = fill; }
    }
    void Down(unsigned long first, unsigned long last, bool useStates, bool move)
    {
        if (n <= 1) return;
        if (move) {
            if (sel == 0 && !wrapping) return;
            if ((unsigned long)sel >= first && (unsigned long)sel <= last) { --sel; sel = sel < 0 ? n - 1 : sel; }
        }
        Child fill = useStates ? saved[last == (unsigned long)(n - 1) ? first : last + 1] : c[last == (unsigned long)(n - 1) ? first : last + 1];
        for (unsigned long i = first; i < last; ++i) { c[i] = c[i + 1]; if (useStates) saved[i] = saved[i + 1]; else saved[i] = c[i]; }
        if (!wrapping && last == (unsigned long)(n - 1)) {
            Child src = useStates ? saved[last] : c[last];
            float alpha = src.a - alphaOffset * 255.0f;
            fill.y = src.y + offY; fill.a = (unsigned char)(alpha < 0.0f ? 0.0f : alpha);
            c[last].y = fill.y; c[last].a = fill.a; saved[last].y = fill.y; saved[last].a = fill.a;
        } else { c[last] = fill; saved[last] = fill; }
    }
};
int main()
{
    List l;
    l.n = 5; l.sel = 2; l.offY = 10.0f; l.alphaOffset = 0.5f; l.wrapping = false;
    for (int i = 0; i < 5; ++i) { l.c[i].y = 100.0f * i; l.c[i].a = 255; l.c[i].zoom = 1.0f + i; l.saved[i] = l.c[i]; }
    l.Down(0, 4, true, true);
    if (l.sel != 1 || l.c[0].y != 100.0f || l.c[3].y != 400.0f || l.c[4].y != 410.0f || l.c[4].a != 127) { std::printf("005392fc_TEST FAIL\n"); return 1; }
    l.sel = 0;
    l.Down(0, 4, false, true);
    if (l.sel != 0 || l.c[0].y != 100.0f) { std::printf("005392fc_TEST FAIL\n"); return 1; }
    std::printf("005392fc_TEST PASS\n");
    return 0;
}
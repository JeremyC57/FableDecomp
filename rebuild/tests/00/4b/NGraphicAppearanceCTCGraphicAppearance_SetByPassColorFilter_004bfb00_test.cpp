#include <cstdio>

static int g_render = 0;
struct CThing { void AddToRenderList() { ++g_render; } };
struct CTCGraphicAppearance {
    CThing* ParentThing;
    unsigned char UpdatePrimitiveReasons;
    signed char ByPassColorFilter;
    void SetByPassColorFilter(bool byPass)
    {
        if (byPass) {
            if (++ByPassColorFilter == 1) {
                ParentThing->AddToRenderList();
                UpdatePrimitiveReasons |= 4;
            }
        } else if (ByPassColorFilter > 0) {
            if (--ByPassColorFilter == 0) {
                ParentThing->AddToRenderList();
                UpdatePrimitiveReasons |= 4;
            }
        }
    }
};

int main()
{
    CThing t; CTCGraphicAppearance a; a.ParentThing = &t; a.UpdatePrimitiveReasons = 0; a.ByPassColorFilter = 0;
    a.SetByPassColorFilter(false);                       // already zero: no change
    if (a.ByPassColorFilter != 0 || g_render != 0) { std::printf("004bfb00_TEST FAIL\n"); return 1; }
    a.SetByPassColorFilter(true);                        // 0 -> 1 marks changed
    if (a.ByPassColorFilter != 1 || g_render != 1 || a.UpdatePrimitiveReasons != 4) { std::printf("004bfb00_TEST FAIL\n"); return 1; }
    a.UpdatePrimitiveReasons = 0;
    a.SetByPassColorFilter(true);                        // 1 -> 2 silent
    if (a.ByPassColorFilter != 2 || g_render != 1 || a.UpdatePrimitiveReasons != 0) { std::printf("004bfb00_TEST FAIL\n"); return 1; }
    a.SetByPassColorFilter(false);                       // 2 -> 1 silent
    a.SetByPassColorFilter(false);                       // 1 -> 0 marks changed
    if (a.ByPassColorFilter != 0 || g_render != 2 || a.UpdatePrimitiveReasons != 4) { std::printf("004bfb00_TEST FAIL\n"); return 1; }
    std::printf("004bfb00_TEST PASS\n");
    return 0;
}
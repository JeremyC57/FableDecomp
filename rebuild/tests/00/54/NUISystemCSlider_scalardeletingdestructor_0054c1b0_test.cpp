#include <cstdio>

struct CSlider {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CSlider* g_last_dtor = 0;

void __fastcall CSlider_dtor(CSlider* slider)
{
    g_last_dtor = slider;
    slider->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CSlider_scalar_deleting_destructor(CSlider* slider, int edx_unused, unsigned int flags)
{
    CSlider_dtor(slider);
    if (flags & 1)
        operator delete(slider);
    return slider;
}

int main()
{
    CSlider a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CSlider_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CSlider b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CSlider_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}
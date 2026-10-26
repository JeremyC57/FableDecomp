#include <cstdio>

class CEnginePrimitiveManagerWeatherMask { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerWeatherMask* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerWeatherMask_dtor(CEnginePrimitiveManagerWeatherMask* mask)
{
    g_last_dtor = mask;
    mask->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerWeatherMask_scalar_deleting_destructor(CEnginePrimitiveManagerWeatherMask* mask, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerWeatherMask_dtor(mask);
    if (flags & 1)
        operator delete(mask);
    return mask;
}

int main()
{
    CEnginePrimitiveManagerWeatherMask a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerWeatherMask_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerWeatherMask b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerWeatherMask_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bb6cc0_TEST PASS\n");
    return 0;
}
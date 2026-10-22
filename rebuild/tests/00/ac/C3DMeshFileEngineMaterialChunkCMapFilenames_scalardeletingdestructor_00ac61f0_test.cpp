#include <cstdio>

struct CMapFilenames {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CMapFilenames* g_last_dtor = 0;

void __fastcall CMapFilenames_dtor(CMapFilenames* filenames)
{
    g_last_dtor = filenames;
    filenames->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CMapFilenames_scalar_deleting_destructor(CMapFilenames* filenames, int edx_unused, unsigned int flags)
{
    CMapFilenames_dtor(filenames);
    if (flags & 1)
        operator delete(filenames);
    return filenames;
}

int main()
{
    CMapFilenames a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CMapFilenames_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CMapFilenames b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CMapFilenames_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}
#include <cstdio>

struct CEditBox {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEditBox* g_last_dtor = 0;

void __fastcall CEditBox_dtor(CEditBox* box)
{
    g_last_dtor = box;
    box->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEditBox_scalar_deleting_destructor(CEditBox* box, int edx_unused, unsigned int flags)
{
    CEditBox_dtor(box);
    if (flags & 1)
        operator delete(box);
    return box;
}

int main()
{
    CEditBox a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEditBox_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEditBox b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEditBox_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}
#include <cstdio>

struct CInkeyBuffer {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CInkeyBuffer* g_last_dtor = 0;

void __fastcall CInkeyBuffer_dtor(CInkeyBuffer* buffer)
{
    g_last_dtor = buffer;
    buffer->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CInkeyBuffer_scalar_deleting_destructor(CInkeyBuffer* buffer, int edx_unused, unsigned int flags)
{
    CInkeyBuffer_dtor(buffer);
    if (flags & 1)
        operator delete(buffer);
    return buffer;
}

int main()
{
    CInkeyBuffer a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CInkeyBuffer_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CInkeyBuffer b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CInkeyBuffer_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_009f49e0_TEST PASS\n");
    return 0;
}
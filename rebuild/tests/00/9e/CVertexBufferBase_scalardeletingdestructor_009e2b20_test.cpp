#include <cstdio>

struct CVertexBufferBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CVertexBufferBase* g_last_dtor = 0;

void __fastcall CVertexBufferBase_dtor(CVertexBufferBase* buffer)
{
    g_last_dtor = buffer;
    buffer->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CVertexBufferBase_scalar_deleting_destructor(CVertexBufferBase* buffer, int edx_unused, unsigned int flags)
{
    CVertexBufferBase_dtor(buffer);
    if (flags & 1)
        operator delete(buffer);
    return buffer;
}

int main()
{
    CVertexBufferBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CVertexBufferBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CVertexBufferBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CVertexBufferBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}
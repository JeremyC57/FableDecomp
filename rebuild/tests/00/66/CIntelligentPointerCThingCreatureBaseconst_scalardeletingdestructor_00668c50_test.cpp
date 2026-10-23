#include <cstdio>

struct CIntelligentPointer {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CIntelligentPointer* g_last_dtor = 0;

void __fastcall CIntelligentPointer_dtor(CIntelligentPointer* pointer)
{
    g_last_dtor = pointer;
    pointer->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CIntelligentPointer_scalar_deleting_destructor(CIntelligentPointer* pointer, int edx_unused, unsigned int flags)
{
    CIntelligentPointer_dtor(pointer);
    if (flags & 1)
        operator delete(pointer);
    return pointer;
}

int main()
{
    CIntelligentPointer a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CIntelligentPointer_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CIntelligentPointer b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CIntelligentPointer_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00668c50_TEST PASS\n");
    return 0;
}
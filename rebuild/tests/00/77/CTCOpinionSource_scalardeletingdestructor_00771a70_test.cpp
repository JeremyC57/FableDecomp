#include <cstdio>

struct CTCOpinionSource {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCOpinionSource* g_last_dtor = 0;

void __fastcall CTCOpinionSource_dtor(CTCOpinionSource* source)
{
    g_last_dtor = source;
    source->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCOpinionSource_scalar_deleting_destructor(CTCOpinionSource* source, int edx_unused, unsigned int flags)
{
    CTCOpinionSource_dtor(source);
    if (flags & 1)
        operator delete(source);
    return source;
}

int main()
{
    CTCOpinionSource a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCOpinionSource_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCOpinionSource b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCOpinionSource_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00771a70_TEST PASS\n");
    return 0;
}
#include <cstdio>

class CBankFile { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CBankFile* g_last_dtor = 0;

void __fastcall CBankFile_dtor_00c4f220(CBankFile* file)
{
    g_last_dtor = file;
    file->destroyed = 1;
}

void free_engine(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CBankFile_scalar_deleting_destructor(CBankFile* file, int edx_unused, unsigned int flags)
{
    CBankFile_dtor_00c4f220(file);
    if (flags & 1)
        free_engine(file);
    return file;
}

int main()
{
    CBankFile a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CBankFile_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CBankFile b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CBankFile_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00c4f200_TEST PASS\n");
    return 0;
}
#include <cstdio>

struct CEngineInternalPrimitiveAnimatedMeshFromFile {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineInternalPrimitiveAnimatedMeshFromFile* g_last_dtor = 0;

void __fastcall CEngineInternalPrimitiveAnimatedMeshFromFile_dtor(CEngineInternalPrimitiveAnimatedMeshFromFile* file)
{
    g_last_dtor = file;
    file->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineInternalPrimitiveAnimatedMeshFromFile_scalar_deleting_destructor(CEngineInternalPrimitiveAnimatedMeshFromFile* file, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveAnimatedMeshFromFile_dtor(file);
    if (flags & 1)
        operator delete(file);
    return file;
}

int main()
{
    CEngineInternalPrimitiveAnimatedMeshFromFile a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineInternalPrimitiveAnimatedMeshFromFile_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineInternalPrimitiveAnimatedMeshFromFile b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineInternalPrimitiveAnimatedMeshFromFile_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}
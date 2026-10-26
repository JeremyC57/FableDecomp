#include <cstdio>

class CManagedTextureData { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CManagedTextureData* g_last_dtor = 0;

void __fastcall CManagedTextureData_dtor(CManagedTextureData* texture)
{
    g_last_dtor = texture;
    texture->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CManagedTextureData_scalar_deleting_destructor(CManagedTextureData* texture, int edx_unused, unsigned int flags)
{
    CManagedTextureData_dtor(texture);
    if (flags & 1)
        operator delete(texture);
    return texture;
}

int main()
{
    CManagedTextureData a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CManagedTextureData_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CManagedTextureData b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CManagedTextureData_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a6b590_TEST PASS\n");
    return 0;
}
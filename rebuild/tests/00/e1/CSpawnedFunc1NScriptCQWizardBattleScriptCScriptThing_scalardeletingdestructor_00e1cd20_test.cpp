#include <cstdio>

class CSpawnedFunc1Model { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CSpawnedFunc1Model* g_last_dtor = 0;

void __fastcall CSpawnedFunc1Model_dtor_00cdd4c0(CSpawnedFunc1Model* self)
{
    g_last_dtor = self;
    self->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CSpawnedFunc1Model_vector_deleting_destructor(CSpawnedFunc1Model* self, int edx_unused, unsigned int flags)
{
    CSpawnedFunc1Model_dtor_00cdd4c0(self);
    if (flags & 1)
        operator delete(self);
    return self;
}

int main()
{
    CSpawnedFunc1Model a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CSpawnedFunc1Model_vector_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CSpawnedFunc1Model b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CSpawnedFunc1Model_vector_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00e1cd20_TEST PASS\n");
    return 0;
}
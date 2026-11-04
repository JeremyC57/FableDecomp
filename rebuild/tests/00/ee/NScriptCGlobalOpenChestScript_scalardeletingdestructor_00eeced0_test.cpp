#include <cstdio>

class CGlobal_OpenChestScript { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CGlobal_OpenChestScript* g_last_dtor = 0;

void __fastcall CGlobal_OpenChestScript_dtor_00cbd510(CGlobal_OpenChestScript* self)
{
    g_last_dtor = self;
    self->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CGlobal_OpenChestScript_vector_deleting_destructor(CGlobal_OpenChestScript* self, int edx_unused, unsigned int flags)
{
    CGlobal_OpenChestScript_dtor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}

int main()
{
    CGlobal_OpenChestScript a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CGlobal_OpenChestScript_vector_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CGlobal_OpenChestScript b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CGlobal_OpenChestScript_vector_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00eeced0_TEST PASS\n");
    return 0;
}
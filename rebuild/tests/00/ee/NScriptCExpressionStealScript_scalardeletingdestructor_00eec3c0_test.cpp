#include <cstdio>

class CExpression_StealScript { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CExpression_StealScript* g_last_dtor = 0;

void __fastcall CExpression_StealScript_dtor_00cbd510(CExpression_StealScript* self)
{
    g_last_dtor = self;
    self->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CExpression_StealScript_vector_deleting_destructor(CExpression_StealScript* self, int edx_unused, unsigned int flags)
{
    CExpression_StealScript_dtor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}

int main()
{
    CExpression_StealScript a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CExpression_StealScript_vector_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CExpression_StealScript b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CExpression_StealScript_vector_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00eec3c0_TEST PASS\n");
    return 0;
}
#include <cstdio>

class CQ_HangingTreeEvilScript { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CQ_HangingTreeEvilScript* g_last_dtor = 0;

void __fastcall CQ_HangingTreeEvilScript_dtor_00d6ed60(CQ_HangingTreeEvilScript* self)
{
    g_last_dtor = self;
    self->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CQ_HangingTreeEvilScript_vector_deleting_destructor(CQ_HangingTreeEvilScript* self, int edx_unused, unsigned int flags)
{
    CQ_HangingTreeEvilScript_dtor_00d6ed60(self);
    if (flags & 1)
        operator delete(self);
    return self;
}

int main()
{
    CQ_HangingTreeEvilScript a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CQ_HangingTreeEvilScript_vector_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CQ_HangingTreeEvilScript b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CQ_HangingTreeEvilScript_vector_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00d6ed40_TEST PASS\n");
    return 0;
}
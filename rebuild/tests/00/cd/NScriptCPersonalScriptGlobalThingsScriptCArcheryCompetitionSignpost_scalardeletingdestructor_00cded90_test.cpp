#include <cstdio>

class CArcheryCompetitionSignpost { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CArcheryCompetitionSignpost* g_last_dtor = 0;

void __fastcall CArcheryCompetitionSignpost_dtor(CArcheryCompetitionSignpost* self)
{
    g_last_dtor = self;
    self->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CArcheryCompetitionSignpost_vector_deleting_destructor(CArcheryCompetitionSignpost* self, int edx_unused, unsigned int flags)
{
    CArcheryCompetitionSignpost_dtor(self);
    if (flags & 1)
        operator delete(self);
    return self;
}

int main()
{
    CArcheryCompetitionSignpost a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CArcheryCompetitionSignpost_vector_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CArcheryCompetitionSignpost b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CArcheryCompetitionSignpost_vector_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00cded90_TEST PASS\n");
    return 0;
}
#include <cstdio>

class CV_AmbushScamScript { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CV_AmbushScamScript* g_last_dtor = 0;

void __fastcall CV_AmbushScamScript_dtor_00e32310(CV_AmbushScamScript* self)
{
    g_last_dtor = self;
    self->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CV_AmbushScamScript_vector_deleting_destructor(CV_AmbushScamScript* self, int edx_unused, unsigned int flags)
{
    CV_AmbushScamScript_dtor_00e32310(self);
    if (flags & 1)
        operator delete(self);
    return self;
}

int main()
{
    CV_AmbushScamScript a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CV_AmbushScamScript_vector_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CV_AmbushScamScript b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CV_AmbushScamScript_vector_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00e322f0_TEST PASS\n");
    return 0;
}
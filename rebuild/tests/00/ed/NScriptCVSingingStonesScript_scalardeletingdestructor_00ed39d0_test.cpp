#include <cstdio>

class CV_SingingStonesScript { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CV_SingingStonesScript* g_last_dtor = 0;

void __fastcall CV_SingingStonesScript_dtor_00ed39f0(CV_SingingStonesScript* self)
{
    g_last_dtor = self;
    self->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CV_SingingStonesScript_vector_deleting_destructor(CV_SingingStonesScript* self, int edx_unused, unsigned int flags)
{
    CV_SingingStonesScript_dtor_00ed39f0(self);
    if (flags & 1)
        operator delete(self);
    return self;
}

int main()
{
    CV_SingingStonesScript a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CV_SingingStonesScript_vector_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CV_SingingStonesScript b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CV_SingingStonesScript_vector_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00ed39d0_TEST PASS\n");
    return 0;
}
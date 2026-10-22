#include <cstdio>

struct CFirstPerson {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CFirstPerson* g_last_dtor = 0;

void __fastcall CFirstPerson_dtor(CFirstPerson* person)
{
    g_last_dtor = person;
    person->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CFirstPerson_scalar_deleting_destructor(CFirstPerson* person, int edx_unused, unsigned int flags)
{
    CFirstPerson_dtor(person);
    if (flags & 1)
        operator delete(person);
    return person;
}

int main()
{
    CFirstPerson a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CFirstPerson_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CFirstPerson b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CFirstPerson_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}
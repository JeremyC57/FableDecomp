#include <cstdio>
#include <cstdlib>

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

struct CCombatTypeDef {
    int f0;
    int Parent;
    void* f8;
    void* scalar_deleting_destructor(unsigned int flags);
};

static void __fastcall CCombatTypeDef_dtor(void* def) { (void)def; ++g_dtor_calls; }

void* CCombatTypeDef::scalar_deleting_destructor(unsigned int flags)
{
    CCombatTypeDef_dtor(this);
    if (flags & 1) {
        ++g_delete_calls;
    }
    return this;
}

int main()
{
    CCombatTypeDef obj;
    obj.f0 = 0x11223344;

    g_dtor_calls = 0; g_delete_calls = 0;
    void* r = obj.scalar_deleting_destructor(0);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) {
        std::printf("FAIL flags=0 dtor=%d del=%d r=%p\n", g_dtor_calls, g_delete_calls, r);
        return 1;
    }

    g_dtor_calls = 0; g_delete_calls = 0;
    r = obj.scalar_deleting_destructor(1);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 1) {
        std::printf("FAIL flags=1 dtor=%d del=%d r=%p\n", g_dtor_calls, g_delete_calls, r);
        return 1;
    }

    g_dtor_calls = 0; g_delete_calls = 0;
    r = obj.scalar_deleting_destructor(2);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) {
        std::printf("FAIL flags=2 dtor=%d del=%d r=%p\n", g_dtor_calls, g_delete_calls, r);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00455de3_TEST PASS\n");
    return 0;
}
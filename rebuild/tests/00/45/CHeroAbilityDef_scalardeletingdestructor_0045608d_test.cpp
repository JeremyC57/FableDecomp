#include <cstdio>

struct CHeroAbilityDef {
    int f0;
    void CHeroAbilityDef_dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

static int g_dtor_called = 0;
static int g_delete_called = 0;

void CHeroAbilityDef::CHeroAbilityDef_dtor()
{
    g_dtor_called++;
}

extern "C" void __cdecl fable_operator_delete(void* p)
{
    g_delete_called++;
    (void)p;
}

void* CHeroAbilityDef::scalar_deleting_destructor(unsigned int flags)
{
    this->CHeroAbilityDef_dtor();
    if (flags & 1)
        fable_operator_delete(this);
    return this;
}

int main()
{
    CHeroAbilityDef obj;
    obj.f0 = 42;

    g_dtor_called = 0; g_delete_called = 0;
    void* r = obj.scalar_deleting_destructor(0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL flags0 dtor=%d del=%d\n", g_dtor_called, g_delete_called);
        return 1;
    }

    g_dtor_called = 0; g_delete_called = 0;
    r = obj.scalar_deleting_destructor(1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL flags1 dtor=%d del=%d\n", g_dtor_called, g_delete_called);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0045608d_TEST PASS\n");
    return 0;
}
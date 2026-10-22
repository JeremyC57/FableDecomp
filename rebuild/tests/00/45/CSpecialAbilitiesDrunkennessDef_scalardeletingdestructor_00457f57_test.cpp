#include <cstdio>

struct CSpecialAbilitiesDrunkennessDef {
    void* vftable;
};

static int g_dtor_called = 0;
static int g_delete_called = 0;

extern "C" void __fastcall CSpecialAbilitiesDrunkennessDef_dtor(CSpecialAbilitiesDrunkennessDef* def) { (void)def; g_dtor_called++; }
extern "C" void __cdecl operator_delete_stub(void* p) { (void)p; g_delete_called++; }

void* __fastcall CSpecialAbilitiesDrunkennessDef_scalar_deleting_destructor(CSpecialAbilitiesDrunkennessDef* def, int /*edx*/, unsigned int flags)
{
    CSpecialAbilitiesDrunkennessDef_dtor(def);
    if (flags & 1)
        operator_delete_stub(def);
    return def;
}

int main()
{
    CSpecialAbilitiesDrunkennessDef obj;
    obj.vftable = 0;

    // flags with bit0 clear: dtor runs, delete does NOT run, returns self
    g_dtor_called = 0; g_delete_called = 0;
    void* r0 = CSpecialAbilitiesDrunkennessDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r0 != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL branch0 r=%p dtor=%d del=%d\n", r0, g_dtor_called, g_delete_called);
        return 1;
    }

    // flags with bit0 set: dtor runs AND delete runs, returns self
    g_dtor_called = 0; g_delete_called = 0;
    void* r1 = CSpecialAbilitiesDrunkennessDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL branch1 r=%p dtor=%d del=%d\n", r1, g_dtor_called, g_delete_called);
        return 1;
    }

    // upper bits set but bit0 clear -> no delete
    g_dtor_called = 0; g_delete_called = 0;
    void* r2 = CSpecialAbilitiesDrunkennessDef_scalar_deleting_destructor(&obj, 0, 2);
    if (r2 != &obj || g_delete_called != 0) {
        std::printf("FAIL branch2 del=%d\n", g_delete_called);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00457f57_TEST PASS\n");
    return 0;
}
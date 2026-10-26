#include <cstdio>

class COpinionDeedEffectsDef { public: int f0; };

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted_ptr = 0;

extern "C" void __fastcall COpinionDeedEffectsDef_dtor(COpinionDeedEffectsDef* def) { (void)def; g_dtor_called++; }
extern "C" void operator_delete_impl(void* p) { g_delete_called++; g_deleted_ptr = p; }

void* __fastcall COpinionDeedEffectsDef_scalar_deleting_destructor(COpinionDeedEffectsDef* def, int /*edx*/, unsigned int flags)
{
    COpinionDeedEffectsDef_dtor(def);
    if (flags & 1)
        operator_delete_impl(def);
    return def;
}

int main()
{
    COpinionDeedEffectsDef obj; obj.f0 = 7;
    g_dtor_called = 0; g_delete_called = 0; g_deleted_ptr = 0;
    void* r0 = COpinionDeedEffectsDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r0 != &obj) { std::printf("FAIL ret0\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor0\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL nodelete\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0; g_deleted_ptr = 0;
    void* r1 = COpinionDeedEffectsDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r1 != &obj) { std::printf("FAIL ret1\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor1\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL delete1\n"); return 1; }
    if (g_deleted_ptr != &obj) { std::printf("FAIL delptr\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00456da4_TEST PASS\n");
    return 0;
}
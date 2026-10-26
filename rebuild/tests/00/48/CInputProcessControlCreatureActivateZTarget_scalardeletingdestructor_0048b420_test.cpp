#include <stdio.h>

class CInputProcessControlCreatureActivateZTarget { public: int tag; };

static int g_dtor_called = 0;
static int g_delete_called = 0;

extern "C" void __fastcall LandscapeBackgroundPatch_dtor(CInputProcessControlCreatureActivateZTarget* process) {
    g_dtor_called++;
    process->tag = 0xDEAD;
}
extern "C" void operator_delete_impl(void* p) {
    g_delete_called++;
    (void)p;
}

void* __fastcall CInputProcessControlCreatureActivateZTarget_scalar_deleting_destructor(
        CInputProcessControlCreatureActivateZTarget* process, int edx, unsigned int flags);

int main() {
    CInputProcessControlCreatureActivateZTarget a; a.tag = 1;
    // flags bit0 clear: dtor runs, no delete, returns self
    g_dtor_called = g_delete_called = 0;
    void* r1 = CInputProcessControlCreatureActivateZTarget_scalar_deleting_destructor(&a, 0, 0);
    if (r1 != &a || g_dtor_called != 1 || g_delete_called != 0 || a.tag != 0xDEAD) {
        printf("FAIL case0\n"); return 1;
    }
    // flags bit0 set: dtor runs AND delete runs
    CInputProcessControlCreatureActivateZTarget b; b.tag = 2;
    g_dtor_called = g_delete_called = 0;
    void* r2 = CInputProcessControlCreatureActivateZTarget_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || g_dtor_called != 1 || g_delete_called != 1) {
        printf("FAIL case1\n"); return 1;
    }
    // even flags (bit0 clear) e.g. 2: no delete
    g_dtor_called = g_delete_called = 0;
    CInputProcessControlCreatureActivateZTarget c; c.tag = 3;
    CInputProcessControlCreatureActivateZTarget_scalar_deleting_destructor(&c, 0, 2);
    if (g_delete_called != 0) { printf("FAIL case2\n"); return 1; }

    printf("VDD_PASS_OK\n");
    return 0;
}
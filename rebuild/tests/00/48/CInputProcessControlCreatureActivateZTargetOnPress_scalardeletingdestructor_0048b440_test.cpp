#include <cstdio>

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_last_deleted = 0;

class CInputProcessControlCreatureActivateZTargetOnPress
{ public:
    int marker;
    void dtor();
};

void CInputProcessControlCreatureActivateZTargetOnPress::dtor() { g_dtor_calls++; }

extern void __cdecl fable_operator_delete(void* p);
void __cdecl fable_operator_delete(void* p) { g_delete_calls++; g_last_deleted = p; }

void* __fastcall CInputProcessControlCreatureActivateZTargetOnPress_scalar_deleting_destructor(
    CInputProcessControlCreatureActivateZTargetOnPress* process, int, unsigned int flags);

int main()
{
    CInputProcessControlCreatureActivateZTargetOnPress a;
    a.marker = 123;

    // flags bit0 clear: dtor runs, no delete
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r1 = CInputProcessControlCreatureActivateZTargetOnPress_scalar_deleting_destructor(&a, 0, 0);
    if (r1 != &a) { printf("FAIL r1\n"); return 1; }
    if (g_dtor_calls != 1) { printf("FAIL dtor1\n"); return 1; }
    if (g_delete_calls != 0) { printf("FAIL del1\n"); return 1; }

    // flags bit0 set: dtor runs, delete runs on self
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r2 = CInputProcessControlCreatureActivateZTargetOnPress_scalar_deleting_destructor(&a, 0, 1);
    if (r2 != &a) { printf("FAIL r2\n"); return 1; }
    if (g_dtor_calls != 1) { printf("FAIL dtor2\n"); return 1; }
    if (g_delete_calls != 1) { printf("FAIL del2\n"); return 1; }
    if (g_last_deleted != &a) { printf("FAIL deleted-ptr\n"); return 1; }

    // higher bits ignored aside from bit0
    g_dtor_calls = g_delete_calls = 0;
    CInputProcessControlCreatureActivateZTargetOnPress_scalar_deleting_destructor(&a, 0, 2);
    if (g_delete_calls != 0) { printf("FAIL del-bit\n"); return 1; }

    printf("VDTOR_OK\n");
    return 0;
}
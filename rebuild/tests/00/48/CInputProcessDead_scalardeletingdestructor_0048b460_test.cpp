#include <stdio.h>

class CInputProcessDead { public: int dummy; };

static int g_dtor_calls = 0;
static int g_free_calls = 0;
static void* g_freed = 0;

void __fastcall CInputProcessDead_Destroy(CInputProcessDead* process)
{
    g_dtor_calls++;
    (void)process;
}

 void operator delete(void* p)
{
    g_free_calls++;
    g_freed = p;
}

void* __fastcall CInputProcessDead_scalar_deleting_destructor(
    CInputProcessDead* process, int edx, unsigned int flags)
{
    CInputProcessDead_Destroy(process);
    if (flags & 1)
        operator delete(process);
    return process;
}

int main()
{
    CInputProcessDead a; a.dummy = 7;

    // flags with bit0 clear: dtor runs, no free, returns self
    g_dtor_calls = g_free_calls = 0; g_freed = 0;
    void* r1 = CInputProcessDead_scalar_deleting_destructor(&a, 0, 0);
    if (r1 != &a || g_dtor_calls != 1 || g_free_calls != 0) { printf("FAIL1\n"); return 1; }

    // flags with bit0 set: dtor runs, free(self), returns self
    g_dtor_calls = g_free_calls = 0; g_freed = 0;
    void* r2 = CInputProcessDead_scalar_deleting_destructor(&a, 0, 1);
    if (r2 != &a || g_dtor_calls != 1 || g_free_calls != 1 || g_freed != &a) { printf("FAIL2\n"); return 1; }

    // higher bits set but bit0 clear -> no free
    g_dtor_calls = g_free_calls = 0; g_freed = 0;
    void* r3 = CInputProcessDead_scalar_deleting_destructor(&a, 0, 2);
    if (r3 != &a || g_free_calls != 0) { printf("FAIL3\n"); return 1; }

    printf("VDD_PARITY_OK\n");
    return 0;
}
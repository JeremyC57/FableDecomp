#include <cstdio>

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_last_deleted = 0;

extern "C" void __fastcall CInputProcessHeroInformationScreens_dtor(void* process)
{
    (void)process;
    ++g_dtor_calls;
}

extern "C" void __cdecl operator_delete_LBP(void* p)
{
    ++g_delete_calls;
    g_last_deleted = p;
}

void* __fastcall CInputProcessHeroInformationScreens_scalar_deleting_destructor(void* process, int edx, unsigned int flags);

int main()
{
    int dummy = 0;
    void* obj = &dummy;

    // flags & 1 == 0 -> destructor runs, no delete
    g_dtor_calls = g_delete_calls = 0;
    g_last_deleted = 0;
    void* r0 = CInputProcessHeroInformationScreens_scalar_deleting_destructor(obj, 0, 0);
    bool ok0 = (r0 == obj) && (g_dtor_calls == 1) && (g_delete_calls == 0);

    // flags & 1 == 1 -> destructor runs, then operator delete(this)
    g_dtor_calls = g_delete_calls = 0;
    g_last_deleted = 0;
    void* r1 = CInputProcessHeroInformationScreens_scalar_deleting_destructor(obj, 0, 1);
    bool ok1 = (r1 == obj) && (g_dtor_calls == 1) && (g_delete_calls == 1) && (g_last_deleted == obj);

    // even flag with high bits set but bit0 clear -> no delete
    g_dtor_calls = g_delete_calls = 0;
    void* r2 = CInputProcessHeroInformationScreens_scalar_deleting_destructor(obj, 0, 2);
    bool ok2 = (r2 == obj) && (g_dtor_calls == 1) && (g_delete_calls == 0);

    if (ok0 && ok1 && ok2) {
        printf("VDD_PARITY_OK\n");
        return 0;
    }
    printf("VDD_PARITY_FAIL %d %d %d\n", (int)ok0, (int)ok1, (int)ok2);
    return 1;
}
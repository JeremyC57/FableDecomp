#include <stdio.h>

struct CActiveFile {
    void* m_thing;
};

static int g_baseCalled = 0;
static void* g_freed = 0;

void __fastcall CActiveFile_Base_006c7670(CActiveFile* self) { g_baseCalled++; (void)self; }
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed = p; }

void __fastcall CActiveFile__OnReadFinished(CActiveFile* self);

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    // Case 1: member non-null -> base called + freed
    int dummy = 5;
    CActiveFile a;
    a.m_thing = &dummy;
    g_baseCalled = 0; g_freed = 0;
    CActiveFile__OnReadFinished(&a);
    if (g_baseCalled != 1) { printf("FAIL base1\n"); return 0; }
    if (g_freed != &dummy) { printf("FAIL freed\n"); return 0; }

    // Case 2: member null -> base called, nothing freed
    CActiveFile b;
    b.m_thing = 0;
    g_baseCalled = 0; g_freed = (void*)1;
    CActiveFile__OnReadFinished(&b);
    if (g_baseCalled != 1) { printf("FAIL base2\n"); return 0; }
    if (g_freed != (void*)1) { printf("FAIL nofree\n"); return 0; }

    printf("006c7dc0_TEST PASS\n");
    return 0;
}
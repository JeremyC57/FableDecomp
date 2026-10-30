#include <stdio.h>

struct CActiveFile {
    void* field0;
};

static int g_base_called = 0;
static void* g_freed = (void*)0;

void __fastcall BaseReadDone_007a3910(CActiveFile* self) { (void)self; g_base_called++; }
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed = p; }

void __fastcall OnReadFinished(CActiveFile* self);

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    // case 1: field0 non-null -> base called + freed
    int slot = 0;
    void* ptr = &slot;
    CActiveFile a; a.field0 = ptr;
    g_base_called = 0; g_freed = (void*)0;
    OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_freed == ptr);

    // case 2: field0 null -> base called, not freed
    CActiveFile b; b.field0 = (void*)0;
    g_base_called = 0; g_freed = (void*)1;
    OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_freed == (void*)1);

    if (ok1 && ok2)
        printf("007a3c80_TEST PASS\n");
    else
        printf("FAIL b=%d f=%p\n", g_base_called, g_freed);
    return 0;
}
#include <cstdio>

struct CActiveFile { void* ptr; };

static int g_base_called = 0;
static void* g_freed = (void*)0;

extern "C" void __fastcall base_70b8a0(CActiveFile* self) { (void)self; g_base_called++; }
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed = p; }

void __fastcall OnReadFinished(CActiveFile* self);

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    // Case 1: ptr non-null -> base called, free called with ptr
    int dummy;
    CActiveFile a; a.ptr = &dummy;
    g_base_called = 0; g_freed = (void*)0;
    OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_freed == &dummy);

    // Case 2: ptr null -> base called, free NOT called
    CActiveFile b; b.ptr = (void*)0;
    g_base_called = 0; g_freed = (void*)-1;
    OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2)
        printf("0070b990_TEST PASS\n");
    else
        printf("FAIL b=%d f1=%p\n", g_base_called, g_freed);
    return 0;
}
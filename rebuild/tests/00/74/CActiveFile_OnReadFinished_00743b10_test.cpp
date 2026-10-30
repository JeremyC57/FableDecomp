#include <cstdio>
struct CActiveFile { void* member0; };

static int g_base_called = 0;
static void* g_freed = (void*)0;

extern "C" void __fastcall CActiveFile_base_00742290(CActiveFile* self) { g_base_called++; (void)self; }
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    int dummy = 0;
    CActiveFile a; a.member0 = &dummy;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_freed == &dummy);

    g_base_called = 0; g_freed = (void*)1;
    CActiveFile b; b.member0 = (void*)0;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_freed == (void*)1); // free not called

    if (ok1 && ok2) printf("00743b10_TEST PASS\n");
    else printf("FAIL b=%d f=%p\n", g_base_called, g_freed);
    return 0;
}
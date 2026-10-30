#include <cstdio>

struct CActiveFile { void* m_buffer; };

static int g_helper_calls = 0;
static void* g_freed = (void*)-1;

void __fastcall CActiveFile_Helper(void* self) { (void)self; ++g_helper_calls; }
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    // case 1: non-null buffer -> helper called, free(buffer)
    int dummy;
    CActiveFile a; a.m_buffer = &dummy;
    g_helper_calls = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_helper_calls == 1) && (g_freed == &dummy);

    // case 2: null buffer -> helper called, no free
    CActiveFile b; b.m_buffer = 0;
    g_helper_calls = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_helper_calls == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2) { printf("ONREADFINISHED_OK\n"); return 0; }
    printf("FAIL o1=%d o2=%d\n", ok1, ok2);
    return 1;
}
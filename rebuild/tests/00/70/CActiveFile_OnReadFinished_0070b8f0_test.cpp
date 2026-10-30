#include <stdio.h>

struct CActiveFile {
    void* field0;
};

static int g_base_called = 0;
static void* g_freed = (void*)0;

void __fastcall CActiveFile_base(CActiveFile* self) {
    g_base_called = 1;
}

static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return;
    g_freed = p;
}

void __fastcall OnReadFinished(CActiveFile* self);

int main() {
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    // case: non-null field
    int dummy = 0;
    CActiveFile a;
    a.field0 = &dummy;
    g_base_called = 0; g_freed = (void*)0;
    OnReadFinished(&a);
    if (!g_base_called) { printf("FAIL base\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free\n"); return 1; }

    // case: null field -> no free
    CActiveFile b;
    b.field0 = (void*)0;
    g_base_called = 0; g_freed = (void*)0x1;
    OnReadFinished(&b);
    if (!g_base_called) { printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)0x1) { printf("FAIL free2\n"); return 1; }

    printf("0070b8f0_TEST PASS\n");
    return 0;
}
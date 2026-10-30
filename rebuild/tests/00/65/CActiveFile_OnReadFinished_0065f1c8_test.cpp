#include <stdio.h>

struct CActiveFile {
    void* buf;
};

static int g_pre = 0;
static void* g_freed = (void*)0;

extern "C" void __fastcall CActiveFile_PreRead_0065e4d1(CActiveFile* self) { g_pre++; }
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed = p; }

void __fastcall OnReadFinished(CActiveFile* self);

int main() {
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    CActiveFile a;
    a.buf = (void*)0x1234;
    OnReadFinished(&a);
    if (g_pre != 1) { printf("FAIL pre\n"); return 1; }
    if (g_freed != (void*)0x1234) { printf("FAIL free\n"); return 1; }

    CActiveFile b;
    b.buf = (void*)0;
    g_pre = 0; g_freed = (void*)0x9;
    OnReadFinished(&b);
    if (g_pre != 1) { printf("FAIL pre2\n"); return 1; }
    if (g_freed != (void*)0x9) { printf("FAIL free2\n"); return 1; }

    printf("OK_0065f1c8\n");
    return 0;
}
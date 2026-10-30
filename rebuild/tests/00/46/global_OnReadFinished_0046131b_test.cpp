#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile
{
    void* field_0;
};

extern "C" int g_called;
int g_called = 0;
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; (void)p; g_called = 1; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    CActiveFile a;
    a.field_0 = 0;
    CActiveFile_OnReadFinished(&a);
    if (g_called != 0) { printf("FAIL_0046131b\n"); return 1; }

    int dummy = 7;
    CActiveFile b;
    b.field_0 = &dummy;
    CActiveFile_OnReadFinished(&b);
    if (g_called != 1) { printf("FAIL_0046131b\n"); return 1; }

    printf("OK_0046131b\n");
    return 0;
}
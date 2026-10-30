#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile
{
    void* m_ptr;
};


void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

static int g_freed = 0;
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return;
    if (p != 0) g_freed = 1;
}

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    CActiveFile a;
    a.m_ptr = (void*)0x12345678;
    CActiveFile_OnReadFinished(&a);

    CActiveFile b;
    b.m_ptr = 0;
    g_freed = 0;
    CActiveFile_OnReadFinished(&b);
    int nullcase_ok = (g_freed == 0);

    if (nullcase_ok)
        printf("PASS_460c33\n");
    return 0;
}
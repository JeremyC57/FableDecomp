#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile { void* ptr; };

static int g_freed = 0;
static void* g_last = 0;
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed++; g_last = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    // non-null case: callee invoked once with the pointer
    int dummy = 0;
    CActiveFile a; a.ptr = &dummy;
    CActiveFile_OnReadFinished(&a);

    // null case: callee not invoked
    CActiveFile b; b.ptr = 0;
    CActiveFile_OnReadFinished(&b);

    if (g_freed == 1 && g_last == &dummy)
        printf("OK_461009\n");
    else
        printf("FAIL_461009\n");
    return 0;
}
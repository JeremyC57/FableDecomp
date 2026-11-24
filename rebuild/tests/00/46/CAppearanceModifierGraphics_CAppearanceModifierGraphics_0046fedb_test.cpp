#include "rebuild_abi.h"
#include <stdio.h>

struct CActiveFile {
    void* field0;
};

static int g_freed = 0;
static void* g_last = 0;

extern void __fastcall OnReadFinished_0046fedb(CActiveFile* thisptr);

static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return;
    g_freed = 1;
    g_last = p;
}

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    int dummy = 0;
    CActiveFile a;
    a.field0 = &dummy;
    OnReadFinished_0046fedb(&a);

    CActiveFile b;
    b.field0 = 0;
    OnReadFinished_0046fedb(&b);

    if (g_freed == 1 && g_last == &dummy) {
        printf("PASS_0046fedb\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}
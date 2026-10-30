#include <stdio.h>

struct CActiveFile {
    void* buf;
};

static int g_pre_called = 0;
static void* g_freed = (void*)0;

extern "C" void __fastcall CActiveFile_PreRead_005f8197(CActiveFile* self) {
    g_pre_called = 1;
    (void)self;
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
    int mk = 0;
    int slot = 0;
    void* bufptr = (void*)(&slot);

    /* case 1: non-null buffer -> pre called, freed */
    CActiveFile a;
    a.buf = bufptr;
    g_pre_called = 0;
    g_freed = (void*)0;
    OnReadFinished(&a);
    if (g_pre_called == 1 && g_freed == bufptr) mk++;

    /* case 2: null buffer -> pre called, NOT freed */
    CActiveFile b;
    b.buf = (void*)0;
    g_pre_called = 0;
    g_freed = (void*)1;
    OnReadFinished(&b);
    if (g_pre_called == 1 && g_freed == (void*)1) mk++;

    if (mk == 2) printf("OK_005f8908\n");
    else printf("FAIL %d\n", mk);
    return 0;
}
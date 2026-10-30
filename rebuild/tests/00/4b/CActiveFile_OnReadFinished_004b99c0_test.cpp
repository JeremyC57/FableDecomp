#include <stdio.h>

struct CActiveFile {
    void* field0;
};

static int g_baseCalled = 0;
static void* g_freed = (void*)0;

void __fastcall CActiveFile_baseHandler(CActiveFile* self)
{
    (void)self;
    g_baseCalled = 1;
}

static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return;
    g_freed = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    // case 1: field0 non-null -> base called + freed
    int dummy = 0;
    CActiveFile a;
    a.field0 = &dummy;
    g_baseCalled = 0; g_freed = (void*)0;
    CActiveFile_OnReadFinished(&a);
    if (!(g_baseCalled == 1 && g_freed == &dummy)) { printf("FAIL1\n"); return 1; }

    // case 2: field0 null -> base called, no free
    CActiveFile b;
    b.field0 = (void*)0;
    g_baseCalled = 0; g_freed = (void*)1;  // sentinel
    CActiveFile_OnReadFinished(&b);
    if (!(g_baseCalled == 1 && g_freed == (void*)1)) { printf("FAIL2\n"); return 1; }

    printf("004b99c0_TEST PASS\n");
    return 0;
}
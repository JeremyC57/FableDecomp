#include <cstdio>

struct CActiveFile {
    void* m_ptr;
};

static int g_baseCalled = 0;
static void* g_freed = (void*)-1;

void __fastcall OnReadFinishedBase(CActiveFile* self) { g_baseCalled++; (void)self; }
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    int dummy = 7;
    CActiveFile a; a.m_ptr = &dummy;
    g_baseCalled = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&a);
    bool ok = (g_baseCalled == 1) && (g_freed == &dummy);

    CActiveFile b; b.m_ptr = 0;
    g_baseCalled = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&b);
    ok = ok && (g_baseCalled == 1) && (g_freed == (void*)-1);

    if (ok) printf("007cbc20_TEST PASS\n");
    else printf("FAIL base=%d\n", g_baseCalled);
    return 0;
}
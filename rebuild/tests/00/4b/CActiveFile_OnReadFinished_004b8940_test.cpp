#include <cstdio>

struct CActiveFile {
    void* field0;
};

static int g_baseCalled = 0;
static void* g_freed = (void*)-1;

extern "C" void __fastcall CActiveFile_base(CActiveFile* self) { (void)self; g_baseCalled++; }
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    // Case 1: field0 non-null -> freed
    int dummy = 7;
    CActiveFile a; a.field0 = &dummy;
    g_baseCalled = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_baseCalled == 1) && (g_freed == &dummy);

    // Case 2: field0 null -> not freed
    CActiveFile b; b.field0 = 0;
    g_baseCalled = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_baseCalled == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2) printf("004b8940_TEST PASS\n");
    else printf("FAIL b=%d\n", g_baseCalled);
    return 0;
}
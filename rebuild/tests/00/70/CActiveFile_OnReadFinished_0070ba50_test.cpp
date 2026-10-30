#include <cstdio>

struct CActiveFile { void* ptr; };

static int g_base_called = 0;
static void* g_freed = (void*)0;

extern void __fastcall base_OnReadFinished_impl(CActiveFile* self);

void __fastcall base_OnReadFinished_impl(CActiveFile* self) { g_base_called++; }
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed = p; }

void __fastcall OnReadFinished(CActiveFile* self);

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    int dummy = 0;
    CActiveFile a;
    a.ptr = &dummy;
    g_base_called = 0; g_freed = (void*)0;
    OnReadFinished(&a);
    if (g_base_called != 1) { std::printf("FAIL base\n"); return 1; }
    if (g_freed != &dummy) { std::printf("FAIL free\n"); return 1; }

    CActiveFile b;
    b.ptr = (void*)0;
    g_base_called = 0; g_freed = (void*)0;
    OnReadFinished(&b);
    if (g_base_called != 1) { std::printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)0) { std::printf("FAIL free2\n"); return 1; }

    std::printf("0070ba50_TEST PASS\n");
    return 0;
}
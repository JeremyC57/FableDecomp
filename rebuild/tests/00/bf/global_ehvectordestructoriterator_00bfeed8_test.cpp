// Behaviour test for __ehvec_dtor (0x00BFEED8): elements are destroyed last-to-first, each with `this` = the
// element, the unwind helper is not reached on normal completion, and a zero count destroys nothing.
#include <stdio.h>

typedef unsigned int size_t;

static int g_unwound = 0;
extern "C" void __stdcall __ArrayUnwind(void*, size_t, int, void (__fastcall *)(void*)) { ++g_unwound; }

extern "C" void __stdcall __ehvec_dtor(void* ptr, size_t size, int count, void (__fastcall *pDtor)(void*))
{
    int success = 0;

    ptr = (char*)ptr + size * count;

    __try
    {
        while (--count >= 0)
        {
            ptr = (char*)ptr - size;
            (*pDtor)(ptr);
        }
        success = 1;
    }
    __finally
    {
        if (!success)
            __ArrayUnwind(ptr, size, count, pDtor);
    }
}

struct SElement { int id; int pad[2]; };
static int g_order[8];
static int g_calls = 0;
static void __fastcall DestroyElement(void* self) { g_order[g_calls++] = ((SElement*)self)->id; ((SElement*)self)->id = -1; }

int main()
{
    SElement a[5];
    int i;
    for (i = 0; i < 5; ++i) a[i].id = i + 10;
    __ehvec_dtor(a, sizeof(SElement), 5, DestroyElement);
    if (g_calls != 5 || g_unwound != 0) { printf("FAIL count\n"); return 1; }
    for (i = 0; i < 5; ++i)
        if (g_order[i] != 14 - i || a[i].id != -1) { printf("FAIL order\n"); return 1; }
    g_calls = 0;
    __ehvec_dtor(a, sizeof(SElement), 0, DestroyElement);
    if (g_calls != 0 || g_unwound != 0) { printf("FAIL empty\n"); return 1; }
    printf("PASS_00BFEED8_OK\n");
    return 0;
}

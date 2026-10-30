// Behaviour test for 0x004C73B0 (DeleteArray_004c73b0): the __fastcall argument goes to operator delete[]
// (MSVCR71 ??_V@YAXPAX@Z, import thunk 0x00BFEB1C). The test replaces the global operator delete[] to observe it.
#include <cstdio>

static void* g_last = 0;
static int g_calls = 0;

void __cdecl operator delete[](void* p)
{
    g_last = p;
    ++g_calls;
}

void __fastcall DeleteArray_004c73b0(void* p);

int main()
{
    static char block[24];
    DeleteArray_004c73b0(block);
    if (g_last == block && g_calls == 1)
        std::printf("004c73b0_TEST PASS\n");
    else
        std::printf("FAIL last=%p calls=%d\n", g_last, g_calls);
    return 0;
}

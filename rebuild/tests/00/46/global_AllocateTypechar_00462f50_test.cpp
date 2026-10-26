#include "rebuild_abi.h"
#include <cstdio>
#include <cstdlib>

extern "C" void* FABLE_STDCALL AllocateType_char_462f50(fable_u32 count, void* p2, void* p3);

// The allocator is the CRT's malloc (0x00BFEA0E); a static-CRT link cannot replace it, so the test checks the
// real block and records what the init helper received.
static void* g_initMem = (void*)1; static void* g_initP2 = 0; static void* g_initP3 = 0; static int g_initCalls = 0;
void FABLE_FASTCALL fable_init_462f50(void* p2, void* p3, void* mem, char* localByte)
{
    (void)localByte; g_initP2 = p2; g_initP3 = p3; g_initMem = mem; ++g_initCalls;
}

int main()
{
    void* r1 = AllocateType_char_462f50(4, (void*)0x1, (void*)0x2); // count!=0 -> malloc'd block
    int ok1 = r1 != 0 && g_initMem == r1 && g_initP2 == (void*)0x1 && g_initP3 == (void*)0x2;
    void* r0 = AllocateType_char_462f50(0, (void*)0x1, (void*)0x2); // count==0 -> null, init still runs
    int ok0 = r0 == 0 && g_initMem == 0 && g_initCalls == 2;
    std::free(r1);
    if (ok1 && ok0)
        printf("OK_462f50_ALLOC\n");
    else
        printf("FAIL_462f50\n");
    return 0;
}

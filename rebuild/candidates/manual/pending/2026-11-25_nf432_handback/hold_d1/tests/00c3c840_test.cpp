// Behaviour test for 0x00C3C840 CCodaObjectBase's scalar deleting destructor (NF432): links the row object. The row's MakeOne builds the
// object (its vtable and deleting destructor come from the row object); CodaMalloc / CodaFree are defined here over a static block and
// record their calls. `delete` through the virtual destructor must free exactly the block MakeOne got, once.
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#include "engine/CCodaObjectBase.h"
#include <stdio.h>
void* MakeOne();
static double g_block[4]; static int g_mallocs = 0, g_frees = 0; static void* g_freed = 0;
extern "C" void* __cdecl CodaMalloc(unsigned int) { ++g_mallocs; return g_block; }
extern "C" void __cdecl CodaFree(void* p) { ++g_frees; g_freed = p; }
int main(void)
{
    CCodaObjectBase* p = (CCodaObjectBase*)MakeOne();
    int ok = (void*)p == (void*)g_block && g_mallocs == 1;
    delete p;
    ok = ok && g_frees == 1 && g_freed == (void*)g_block;
    if (ok) { printf("00c3c840_TEST PASS\n"); return 0; }
    printf("00c3c840_TEST FAIL\n");
    return 1;
}

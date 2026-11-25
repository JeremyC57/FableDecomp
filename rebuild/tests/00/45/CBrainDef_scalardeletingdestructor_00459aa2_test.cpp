#include "rebuild_abi.h"
#include <stdio.h>
// NF451 (2026-11-25): the row now calls ~CBrainDef (??1CBrainDef@@UAE@XZ, 0x00459ABE) through engine/CBrainDef.h's destructor-only
// opt-in; this test links the row object, names the class through the same header view (no local model) and stubs the destructor
// under its PDB name with a fastcall stub (this in ecx, no stack arguments: the same calling convention as the __thiscall dtor).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CBRAINDEF_DTOR_DECL_ONLY
#include "engine/CBrainDef.h"

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_dtor_this = 0;

extern "C" void __fastcall CBrainDef_dtor_stub(void* thisptr) {
    g_dtor_this = thisptr; g_dtor_called = 1;
}
#pragma comment(linker, "/alternatename:??1CBrainDef@@UAE@XZ=@CBrainDef_dtor_stub@4")
void __cdecl operator delete(void* p)
{
    (void)p; g_delete_called = 1;
}

void * __fastcall CBrainDef_vector_deleting_dtor(
        CBrainDef *thisptr, int edx, unsigned int flags);

int main() {
    void* storage[0x20] = { 0 };
    CBrainDef* obj = reinterpret_cast<CBrainDef*>(storage);

    g_dtor_called = 0; g_delete_called = 0; g_dtor_this = 0;
    void *r1 = CBrainDef_vector_deleting_dtor(obj, 0, 0);
    int ok1 = (r1 == obj && g_dtor_called == 1 && g_dtor_this == obj && g_delete_called == 0);

    g_dtor_called = 0; g_delete_called = 0; g_dtor_this = 0;
    void *r2 = CBrainDef_vector_deleting_dtor(obj, 0, 1);
    int ok2 = (r2 == obj && g_dtor_called == 1 && g_dtor_this == obj && g_delete_called == 1);

    if (ok1 && ok2) {
        printf("PASS_459aa2_OK\n");
    } else {
        printf("FAIL_459aa2\n");
    }
    return 0;
}

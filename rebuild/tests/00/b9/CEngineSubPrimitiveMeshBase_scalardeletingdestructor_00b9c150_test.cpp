#include <stdio.h>

class CEngineSubPrimitiveMeshBase;

static int g_dtor = 0;
static int g_del = 0;
static CEngineSubPrimitiveMeshBase* g_lastDel = 0;

void __fastcall SubPrim_ScalarDtor(CEngineSubPrimitiveMeshBase* mesh) { g_dtor++; }
void __fastcall SubPrim_Delete(CEngineSubPrimitiveMeshBase* mesh) { g_del++; g_lastDel = mesh; }

void* __fastcall CEngineSubPrimitiveMeshBase_scalar_deleting_destructor(
        CEngineSubPrimitiveMeshBase* mesh, int edx, unsigned int flags);

int main()
{
    CEngineSubPrimitiveMeshBase* obj = (CEngineSubPrimitiveMeshBase*)0x1234;

    // flags bit0 clear -> dtor only, no delete
    g_dtor = g_del = 0; g_lastDel = 0;
    void* r0 = CEngineSubPrimitiveMeshBase_scalar_deleting_destructor(obj, 0, 0);
    bool ok0 = (r0 == obj) && (g_dtor == 1) && (g_del == 0);

    // flags bit0 set -> dtor + delete
    g_dtor = g_del = 0; g_lastDel = 0;
    void* r1 = CEngineSubPrimitiveMeshBase_scalar_deleting_destructor(obj, 0, 1);
    bool ok1 = (r1 == obj) && (g_dtor == 1) && (g_del == 1) && (g_lastDel == obj);

    // even flag (bit0 clear, higher bits) -> no delete
    g_dtor = g_del = 0;
    void* r2 = CEngineSubPrimitiveMeshBase_scalar_deleting_destructor(obj, 0, 2);
    bool ok2 = (r2 == obj) && (g_dtor == 1) && (g_del == 0);

    if (ok0 && ok1 && ok2)
        printf("VDD_PARITY_OK\n");
    else
        printf("VDD_PARITY_FAIL d=%d del=%d\n", g_dtor, g_del);
    return (ok0 && ok1 && ok2) ? 0 : 1;
}
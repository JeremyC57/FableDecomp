#include <cstdio>

struct CInputProcessInventoryBase
{
    void ScalarDtor();
};

static int g_dtorCalls = 0;
static int g_deleteCalls = 0;
static void* g_lastDeleted = 0;

void CInputProcessInventoryBase::ScalarDtor() { g_dtorCalls++; }
void __cdecl EngineOperatorDelete(void* p) { g_deleteCalls++; g_lastDeleted = p; }

void* __fastcall CInputProcessInventoryBase_scalar_deleting_destructor(
    CInputProcessInventoryBase* process, int edx, unsigned int flags);

int main()
{
    CInputProcessInventoryBase obj;

    // flags bit0 clear: dtor runs, no delete, returns self
    g_dtorCalls = g_deleteCalls = 0; g_lastDeleted = 0;
    void* r1 = CInputProcessInventoryBase_scalar_deleting_destructor(&obj, 0, 0);
    bool ok1 = (r1 == &obj) && g_dtorCalls == 1 && g_deleteCalls == 0;

    // flags bit0 set: dtor runs, delete runs on self, returns self
    g_dtorCalls = g_deleteCalls = 0; g_lastDeleted = 0;
    void* r2 = CInputProcessInventoryBase_scalar_deleting_destructor(&obj, 0, 1);
    bool ok2 = (r2 == &obj) && g_dtorCalls == 1 && g_deleteCalls == 1 && g_lastDeleted == &obj;

    // flags bit1 set but bit0 clear: no delete
    g_dtorCalls = g_deleteCalls = 0;
    void* r3 = CInputProcessInventoryBase_scalar_deleting_destructor(&obj, 0, 2);
    bool ok3 = (r3 == &obj) && g_dtorCalls == 1 && g_deleteCalls == 0;

    if (ok1 && ok2 && ok3)
        printf("VDD_PARITY_OK\n");
    else
        printf("VDD_FAIL %d %d %d\n", ok1, ok2, ok3);
    return (ok1 && ok2 && ok3) ? 0 : 1;
}
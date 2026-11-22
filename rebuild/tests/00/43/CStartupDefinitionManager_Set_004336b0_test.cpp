// Behaviour test for 0x004336b0 (NF232, q12629) on the shared headers: CStartupDefinitionManager::Set(CStartupDefinitionManager*), the
// row's body on engine/CStartupDefinitionManager.h (SET + PCLASS) and engine/CCountedPointer.h (RESET_CAMERA_DECL: Reset declared only).
// The out-of-line CCountedPointer<CStartupDefinitionManager>::Reset (retail 0x00433D3A) is a recording stub; PClass is defined here.
// Cases: a new manager handed to PClass.Reset; a null pointer (Shutdown's Set(0)) handed on unchanged. Replaces the bare HEAD stub test.
#include <stdio.h>
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_SET
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_PCLASS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "engine/CCountedPointer.h"
#include "engine/CStartupDefinitionManager.h"

void __fastcall CStartupDefinitionManager::Set(CStartupDefinitionManager* manager)
{
    PClass.Reset(manager);
}

static int g_resets = 0; static const void* g_self = 0; static const void* g_arg = (const void*)1;
template <> void CCountedPointer<CStartupDefinitionManager>::Reset(CStartupDefinitionManager* data) { ++g_resets; g_self = this; g_arg = data; }
CCountedPointer<CStartupDefinitionManager> CStartupDefinitionManager::PClass;
struct PClassAccess : CStartupDefinitionManager { static const void* Address() { return &PClass; } };   // protected static, never constructed
static double g_manager_store[(0xd0 + 7) / 8];
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }

int main()
{
    CStartupDefinitionManager* manager = (CStartupDefinitionManager*)g_manager_store;
    CStartupDefinitionManager::Set(manager);
    Check(g_resets == 1 && g_self == PClassAccess::Address() && g_arg == manager, "Set(manager): PClass.Reset(manager)");
    CStartupDefinitionManager::Set(0);
    Check(g_resets == 2 && g_self == PClassAccess::Address() && g_arg == 0, "Set(0): PClass.Reset(0)");
    printf(g_fails ? "FAIL_004336b0\n" : "PASS_004336b0\n");
    return g_fails ? 1 : 0;
}

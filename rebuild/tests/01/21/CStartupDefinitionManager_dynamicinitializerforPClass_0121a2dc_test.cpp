// Test for 0x0121a2dc: the CStartupDefinitionManager::PClass initializer / atexit pair (one TU on the shared headers, NF234).
// Case 1 (0x0121A2DC, _$E1): PClass is zero before main (no ctor code; zero-initialised static).
// Case 2 (0x01228AF1, _$E2): the atexit destructor is ~CCountedPointer = ReleaseData on PClass, once.
// ReleaseData<CStartupDefinitionManager> is out of line in retail (0x00433CC0): stubbed here. Test-only: protected opened for PClass.
#include <cstdio>
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_PCLASS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
#include "engine/CDefinitionManager.h"
#define protected public
#include "engine/CStartupDefinitionManager.h"
#undef protected
static int g_released = 0;
template<> void CCountedPointer<CStartupDefinitionManager>::ReleaseData() { ++g_released; Data = 0; Info = 0; }
CCountedPointer<CStartupDefinitionManager> CStartupDefinitionManager::PClass;
int main()
{
    int ok = 1;
    if (!CStartupDefinitionManager::PClass.IsNull() || CStartupDefinitionManager::PClass.Info != 0) ok = 0;
    CStartupDefinitionManager::PClass.Data = (CStartupDefinitionManager*)0x10;
    CStartupDefinitionManager::PClass.~CCountedPointer<CStartupDefinitionManager>();
    if (g_released != 1 || CStartupDefinitionManager::PClass.Data != 0) ok = 0;
    std::printf(ok ? "0121a2dc_TEST PASS\n" : "0121a2dc_TEST FAIL\n");
    return ok ? 0 : 1;
}

#include <stdio.h>
#include <string.h>
// Behaviour test for 0x00B3B0A0 CEngineResourceManager::GetIndexBufferMemPool (links the row object).
// NF152 (2026-11-20, q4563): rewritten on the shared header the row now uses (was a host copy of the free stand-in AddChildPrimitive);
//   the PassPattern is unchanged. The resource manager is zeroed storage (`this` is unused).
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#include "engine/EIndexBufferPool.h"
#include "engine/CEngineResourceManager.h"

static double g_manager[(sizeof(CEngineResourceManager) + 7) / 8];

int main()
{
    memset(g_manager, 0, sizeof g_manager);
    CEngineResourceManager& manager = *(CEngineResourceManager*)g_manager;
    if (manager.GetIndexBufferMemPool() == IB_POOL_MANAGED && (long)manager.GetIndexBufferMemPool() == 1) {
        printf("AUTO_TINY_00b3b0a0_TEST PASS\n"); return 0;
    }
    printf("AUTO_TINY_00b3b0a0_TEST FAIL\n");
    return 1;
}

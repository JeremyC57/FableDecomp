#include <stdio.h>
#include <string.h>
// Behaviour test for 0x00B3B5B0 CEngineResourceManager::FreeExternalTextureFromResourcePools (links the row object).
// NF152 (2026-11-20, q4596): rewritten on the shared headers the row now uses (engine/CEngineResourceManager.h retail branch + the NF152
//   opt-in, engine/CTexture.h); the checks and the PassPattern are kept. CTexture::Uninitialise is a counting stand-in; the resource
//   manager is zeroed storage (`this` is unused).
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_FREEEXTERNALTEXTUREFROMRESOURCEPOOLS
#define FABLE_RETAIL_CTEXTURE
#include "engine/CTexture.h"
#include "engine/CEngineResourceManager.h"

static int g_uninit = 0;
void CTexture::Uninitialise() { ++g_uninit; }

static double g_manager[(sizeof(CEngineResourceManager) + 7) / 8];

int main()
{
    memset(g_manager, 0, sizeof g_manager);
    CEngineResourceManager& m = *(CEngineResourceManager*)g_manager;
    m.FreeExternalTextureFromResourcePools(0);
    if (g_uninit != 0) { printf("00b3b5b0_TEST FAIL\n"); return 1; }
    m.FreeExternalTextureFromResourcePools(new CTexture);
    if (g_uninit != 1) { printf("00b3b5b0_TEST FAIL\n"); return 1; }
    printf("00b3b5b0_TEST PASS\n");
    return 0;
}

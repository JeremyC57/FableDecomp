#include <stdio.h>
#include <string.h>
// Behaviour test for 0x00B3B1F0 CEngineResourceManager::AllocateExternalTextureFromResourcePools (links the row object).
// NF152 (2026-11-20, q4595): rewritten on the shared headers the row now uses (engine/CEngineResourceManager.h retail branch + the NF152
//   opt-in, engine/CTexture.h); the checks and the PassPattern are kept. CTexture::Initialise is a recording stand-in; the resource
//   manager is zeroed storage (`this` is unused).
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_ALLOCATEEXTERNALTEXTUREFROMRESOURCEPOOLS
#define FABLE_RETAIL_CTEXTURE
#include "engine/CTexture.h"
#include "engine/CEngineResourceManager.h"

static const void* g_size; static const void* g_format; static long g_args[5];
bool CTexture::Initialise(const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage, ESurfacePool pool,
                          bool dynamic, bool render_target)
{
    g_size = &size; g_format = &format; g_args[0] = levels; g_args[1] = (long)usage; g_args[2] = pool; g_args[3] = dynamic;
    g_args[4] = render_target; return true;
}

static double g_manager[(sizeof(CEngineResourceManager) + 7) / 8];

int main()
{
    memset(g_manager, 0, sizeof g_manager);
    CEngineResourceManager& m = *(CEngineResourceManager*)g_manager;
    C2DExtentsI e(64, 32); static double fbuf[1]; const CPixelFormat& f = *(const CPixelFormat*)fbuf;
    CTexture* t = m.AllocateExternalTextureFromResourcePools(99, e, 3, f);
    unsigned long words[2]; memcpy(words, t, sizeof words);
    bool ok = t && words[0] == 0 && words[1] == 0 && g_size == &e && g_format == &f && g_args[0] == 3 &&
              g_args[1] == 0 && g_args[2] == SURFACE_POOL_MANAGED && g_args[3] == 0 && g_args[4] == 0;
    if (!ok) { printf("00b3b1f0_TEST FAIL\n"); return 1; }
    printf("00b3b1f0_TEST PASS\n");
    return 0;
}

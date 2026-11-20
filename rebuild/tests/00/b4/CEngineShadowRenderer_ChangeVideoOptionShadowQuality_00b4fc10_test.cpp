#include <stdio.h>
#include <string.h>
// Behaviour test for 0x00B4FC10 CEngineShadowRenderer::ChangeVideoOptionShadowQuality (links the row object).
// NF152 (2026-11-20, q5011): rewritten on the shared header the row now uses (engine/CEngineShadowRenderer.h retail branch + the NF152
//   opt-ins); the PassPattern is kept. OnReleaseDefaultPoolResources / InitialiseTextures are recording stand-ins; the renderer is zeroed
//   storage and the two option words are read back at their retail offsets (+0x338 long, +0x33C float).
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_INITIALISETEXTURES
#define FABLE_RETAIL_CENGINESHADOWRENDERER_ONRELEASEDEFAULTPOOLRESOURCES
#define FABLE_RETAIL_CENGINESHADOWRENDERER_CHANGEVIDEOOPTIONSHADOWQUALITY
#include "engine/CEngineShadowRenderer.h"

static char g_seq[32] = "";
static bool g_init_result = true;
void CEngineShadowRenderer::OnReleaseDefaultPoolResources() { strcat(g_seq, "R"); }
bool CEngineShadowRenderer::InitialiseTextures() { strcat(g_seq, "I"); return g_init_result; }

static double g_renderer[(sizeof(CEngineShadowRenderer) + 7) / 8];

int main()
{
    memset(g_renderer, 0, sizeof g_renderer);
    unsigned char* raw = (unsigned char*)g_renderer;
    CEngineShadowRenderer& renderer = *(CEngineShadowRenderer*)raw;
    bool r1 = renderer.ChangeVideoOptionShadowQuality(512, 0.75f);
    long size; float scale; memcpy(&size, raw + 0x338, 4); memcpy(&scale, raw + 0x33c, 4);
    g_init_result = false;
    bool r2 = renderer.ChangeVideoOptionShadowQuality(256, 1.5f);
    long size2; float scale2; memcpy(&size2, raw + 0x338, 4); memcpy(&scale2, raw + 0x33c, 4);
    if (strcmp(g_seq, "RIRI") || !r1 || r2 || size != 512 || scale != 0.75f || size2 != 256 || scale2 != 1.5f) {
        printf("00b4fc10_TEST FAIL\n"); return 1;
    }
    printf("00b4fc10_TEST PASS\n");
    return 0;
}

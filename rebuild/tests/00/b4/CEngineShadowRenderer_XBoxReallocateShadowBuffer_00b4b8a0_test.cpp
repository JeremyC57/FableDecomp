#include <stdio.h>
#include <string.h>
// Behaviour test for 0x00B4B8A0 CEngineShadowRenderer::XBoxReallocateShadowBuffer (links the row object; was a print-only stub).
// NF152 (2026-11-20, q4598): on the shared header (engine/CEngineShadowRenderer.h retail branch + the NF152 opt-in); the PassPattern is kept.
//   The renderer is a zeroed block that must stay zeroed (the PC body is empty).
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_XBOXREALLOCATESHADOWBUFFER
#include "engine/CEngineShadowRenderer.h"

static double g_renderer[(sizeof(CEngineShadowRenderer) + 7) / 8];
static double g_zero[(sizeof(CEngineShadowRenderer) + 7) / 8];

int main(void)
{
    memset(g_renderer, 0, sizeof g_renderer);
    memset(g_zero, 0, sizeof g_zero);
    CEngineShadowRenderer& renderer = *(CEngineShadowRenderer*)g_renderer;
    renderer.XBoxReallocateShadowBuffer();
    if (memcmp(g_renderer, g_zero, sizeof g_renderer) != 0) { printf("FAIL_00b4b8a0\n"); return 1; }
    printf("PASS_00b4b8a0\n");
    return 0;
}

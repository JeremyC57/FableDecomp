#include <stdio.h>
#include <string.h>
// Behaviour test for 0x00B4B890 CEngineShadowRenderer::XBoxDeallocateShadowBuffer (links the row object; was a print-only stub).
// NF152 (2026-11-20, q4597): on the shared header (engine/CEngineShadowRenderer.h retail branch + the NF152 opt-in); the PassPattern is kept.
//   The renderer is zeroed storage (the PC body reads nothing).
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_XBOXDEALLOCATESHADOWBUFFER
#include "engine/CEngineShadowRenderer.h"

static double g_renderer[(sizeof(CEngineShadowRenderer) + 7) / 8];

int main(void)
{
    memset(g_renderer, 0, sizeof g_renderer);
    CEngineShadowRenderer& renderer = *(CEngineShadowRenderer*)g_renderer;
    if (renderer.XBoxDeallocateShadowBuffer()) { printf("FAIL_00b4b890\n"); return 1; }
    printf("PASS_00b4b890\n");
    return 0;
}

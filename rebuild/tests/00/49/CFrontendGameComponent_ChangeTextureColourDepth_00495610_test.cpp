// Test of 0x00495610 CFrontendGameComponent::ChangeTextureColourDepth(long) on engine/CFrontendGameComponent.h (NF227, 2026-11-22).
// Self-contained (C++03, /Od /W3, no /EHsc): the row's body is copied here and the out-of-line callee InitGraphics (0x00494840) is
// stubbed; no class is modelled. The object is raw storage of the header's size: the calls are qualified (no vtable is used).
// Cases: each call runs InitGraphics once on `this`, whatever the depth argument (two depths).
#include <stdio.h>
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS
#include "engine/CFrontendGameComponent.h"
static unsigned long g_InitGraphicsCalls;
static const void* g_InitGraphicsThis;
bool CFrontendGameComponent::InitGraphics()
{
    ++g_InitGraphicsCalls;
    g_InitGraphicsThis = this;
    return false;
}
void CFrontendGameComponent::ChangeTextureColourDepth(long)
{
    InitGraphics();
}
static union { double align; unsigned char raw[sizeof(CFrontendGameComponent)]; } g_storage;
int main()
{
    CFrontendGameComponent* component = reinterpret_cast<CFrontendGameComponent*>(g_storage.raw);
    component->CFrontendGameComponent::ChangeTextureColourDepth(16);
    if (g_InitGraphicsCalls != 1 || g_InitGraphicsThis != component) {
        printf("CFrontendGameComponent_ChangeTextureColourDepth_00495610_test FAIL (16)\n");
        return 1;
    }
    component->CFrontendGameComponent::ChangeTextureColourDepth(32);
    if (g_InitGraphicsCalls != 2 || g_InitGraphicsThis != component) {
        printf("CFrontendGameComponent_ChangeTextureColourDepth_00495610_test FAIL (32)\n");
        return 1;
    }
    printf("CFrontendGameComponent_ChangeTextureColourDepth_00495610_test PASS\n");
    return 0;
}

// Behaviour test for 0x00A0AA90 CRenderManagerCore::SetAWindow(const C2DBoxI&) (links the REAL row object). NF172 (q11977): rewritten for
// the renamed row (the old test drove the stand-in CTCInventoryBaseViewportView::SetViewport_00A0AA90 on local models); the catalog
// PassPattern is kept. The core is raw storage (DisplayManager pointer at +0x3A3C); the member is reached through a linker alias
// (thiscall = fastcall with a spare edx), and the callee CDisplayManager::SetViewport(const C2DBoxI&) is a recording stub aliased the
// same way.
#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CallSetAWindow(void* self, int spare, const void* window);
#pragma comment(linker, "/alternatename:@CallSetAWindow@12=?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxI@@@Z")
#pragma comment(linker, "/alternatename:?SetViewport@CDisplayManager@@QAEXABVC2DBoxI@@@Z=@StubSetViewport@12")

static const void* gObservedManager;
static const void* gObservedViewport;
static int gCalls;

extern "C" void __fastcall StubSetViewport(void* self, int, const void* viewport)
{
    gObservedManager = self;
    gObservedViewport = viewport;
    ++gCalls;
}

static unsigned char gCore[0x3a60];
static unsigned char gManager[0x10];

int main()
{
    long viewport[4] = { 10, 20, 630, 460 };
    void* manager = gManager;
    memset(gCore, 0, sizeof(gCore));
    memcpy(gCore + 0x3a3c, &manager, sizeof(manager));
    CallSetAWindow(gCore, 0, viewport);
    if (gCalls != 1 || gObservedManager != gManager)
    {
        printf("FABLETLC_INVENTORY_VIEWPORT_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    if (gObservedViewport != viewport)
    {
        printf("FABLETLC_INVENTORY_VIEWPORT_BEHAVIOR FAIL code=2\n");
        return 2;
    }
    printf("FABLETLC_INVENTORY_VIEWPORT_BEHAVIOR PASS\n");
    return 0;
}

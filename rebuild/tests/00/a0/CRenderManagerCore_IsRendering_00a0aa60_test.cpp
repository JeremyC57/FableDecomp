// Behaviour test for 0x00A0AA60 CRenderManagerCore::IsRendering (links the REAL row object). NF172 (q11976): rewritten for the renamed
// row (the old test drove the free stand-in FableGetCanItemBeQuickAssigned_00A0AA60 on a local view); the catalog PassPattern is kept.
// The core is raw storage (Rendering at +0x3A54, engine/CRenderManagerCore.h static assert); the member is reached through a linker
// alias (thiscall = fastcall with a spare edx).
#include <stdio.h>
#include <string.h>

extern "C" bool __fastcall CallIsRendering(void* self, int spare);
#pragma comment(linker, "/alternatename:@CallIsRendering@8=?IsRendering@CRenderManagerCore@@QAE_NXZ")

static unsigned char gCore[0x3a60];

int main()
{
    memset(gCore, 0, sizeof(gCore));
    gCore[0x3a54] = 1;
    if (!CallIsRendering(gCore, 0))
    {
        printf("FABLETLC_INVENTORY_QUICK_ASSIGN_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    gCore[0x3a54] = 0;
    gCore[0x3a53] = 1;
    gCore[0x3a55] = 1;
    if (CallIsRendering(gCore, 0))
    {
        printf("FABLETLC_INVENTORY_QUICK_ASSIGN_BEHAVIOR FAIL code=2\n");
        return 2;
    }
    printf("FABLETLC_INVENTORY_QUICK_ASSIGN_BEHAVIOR PASS\n");
    return 0;
}

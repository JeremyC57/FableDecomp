// Behaviour test for CRenderManagerCore::AttachTextureToStage 0x009A0CF0 (NF129: the row now defines the Ego_r.pdb spelling on the
// shared engine/CRenderManagerCore.h retail class, so the test drives that class). The manager and texture are zeroed raw storage
// of the header's asserted sizes; the device is a fake whose vtable slot 0x41 (IDirect3DDevice9::SetTexture) records the call.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CRENDERMANAGERCORE
#include "engine/CRenderManagerCore.h"

struct FableTextureDevice
{
    void** vtable;
};

static long g_Calls;
static FableTextureDevice* g_Device;
static unsigned long g_Stage;
static void* g_Texture;

static long __stdcall ObserveSetTexture(FableTextureDevice* device, unsigned long stage, void* texture)
{
    ++g_Calls;
    g_Device = device;
    g_Stage = stage;
    g_Texture = texture;
    return 0;
}

static unsigned char g_Manager[sizeof(CRenderManagerCore)];

int main()
{
    void* vtable[0x42];
    memset(vtable, 0, sizeof(vtable));
    vtable[0x41] = reinterpret_cast<void*>(&ObserveSetTexture);
    FableTextureDevice device = {vtable};
    memset(g_Manager, 0, sizeof(g_Manager));
    CRenderManagerCore* manager = reinterpret_cast<CRenderManagerCore*>(g_Manager);
    manager->PD3DDevice = reinterpret_cast<IDirect3DDevice9*>(&device);

    unsigned long textureStorage = 0x12345678;
    void* textureWords[2] = {&textureStorage, 0};
    const CTexture* texture = reinterpret_cast<const CTexture*>(textureWords);

    manager->AttachTextureToStage(2, texture);
    if (g_Calls != 1 || g_Device != &device || g_Stage != 2 || g_Texture != &textureStorage ||
        (void*)manager->CurrentlySelectedTextures[2] != &textureStorage || manager->TextureStagesInUse != 3)
    {
        printf("FABLETLC_ATTACH_TEXTURE_TO_STAGE_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    manager->AttachTextureToStage(2, texture);
    if (g_Calls != 1 || manager->TextureStagesInUse != 3)
    {
        printf("FABLETLC_ATTACH_TEXTURE_TO_STAGE_BEHAVIOR FAIL code=2\n");
        return 2;
    }
    manager->AttachTextureToStage(2, 0);
    if (g_Calls != 2 || g_Stage != 2 || g_Texture != 0 || manager->CurrentlySelectedTextures[2] != 0 ||
        manager->TextureStagesInUse != 3)
    {
        printf("FABLETLC_ATTACH_TEXTURE_TO_STAGE_BEHAVIOR FAIL code=3\n");
        return 3;
    }
    manager->AttachTextureToStage(5, texture);
    if (g_Calls != 3 || g_Stage != 5 || g_Texture != &textureStorage ||
        (void*)manager->CurrentlySelectedTextures[5] != &textureStorage || manager->TextureStagesInUse != 6)
    {
        printf("FABLETLC_ATTACH_TEXTURE_TO_STAGE_BEHAVIOR FAIL code=4\n");
        return 4;
    }
    printf("FABLETLC_ATTACH_TEXTURE_TO_STAGE_BEHAVIOR PASS\n");
    return 0;
}

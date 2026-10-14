#include <cstdio>
#include <stddef.h>
static int g_reduces = 0, g_sets = 0; static unsigned long g_sizeSet = 0, g_levels = 0; static void* g_frameArg = 0; static void* g_texArg = 0;
struct CGraphicFrameData { unsigned char _pad[0x14]; unsigned short Width; unsigned short Height; unsigned long Sizeof() { return 50; } };
struct CGraphicFrame_Retail { CGraphicFrameData* FrameData; };
struct CManagedTexture_Retail { void* __vftable; void* Data; };
struct CTextureManager_Calls { bool ReduceMipmapLevel(CGraphicFrame_Retail& frame, CManagedTexture_Retail& texture, unsigned long levels) { ++g_reduces; g_frameArg = &frame; g_texArg = &texture; g_levels = levels; return true; } };
struct CEngineSurfaceCompositionManager_Retail { unsigned char _pad0[0x9d4]; CTextureManager_Calls TextureManager; unsigned char _pad1[0x10cc - 0x9d4 - 1]; unsigned long MinSurfaceSize; };
struct CCompositeSurfaceCache_Retail {
    virtual void v00() { }
    unsigned char _resource[0x18]; CEngineSurfaceCompositionManager_Retail* Owner; unsigned char _morphs[0x2c]; CGraphicFrame_Retail GraphicFrame; CManagedTexture_Retail ManagedTexture;
    void SetResourceSize(unsigned long size) { g_sizeSet = size; ++g_sets; }
    void UpdateResourceSize() { SetResourceSize(GraphicFrame.FrameData != 0 ? GraphicFrame.FrameData->Sizeof() + sizeof(GraphicFrame) : sizeof(GraphicFrame)); }
    void ReleaseTexture()
    {
        CGraphicFrameData* data = GraphicFrame.FrameData;
        unsigned long width = data->Width; unsigned long height = data->Height;
        unsigned long smaller = width < height ? width : height;
        if (smaller > Owner->MinSurfaceSize) { Owner->TextureManager.ReduceMipmapLevel(GraphicFrame, ManagedTexture, 1); UpdateResourceSize(); }
    }
};
int main()
{
    static CEngineSurfaceCompositionManager_Retail owner; owner.MinSurfaceSize = 32;
    CCompositeSurfaceCache_Retail cache; CGraphicFrameData data; data.Width = 64; data.Height = 128;
    cache.Owner = &owner; cache.GraphicFrame.FrameData = &data;
    cache.ReleaseTexture();
    if (g_reduces != 1 || g_frameArg != &cache.GraphicFrame || g_texArg != &cache.ManagedTexture || g_levels != 1 || g_sets != 1 || g_sizeSet != 54
        || offsetof(CCompositeSurfaceCache_Retail, Owner) != 0x1c || offsetof(CCompositeSurfaceCache_Retail, ManagedTexture) != 0x50
        || offsetof(CEngineSurfaceCompositionManager_Retail, MinSurfaceSize) != 0x10cc) { std::printf("00b2b120_TEST FAIL\n"); return 1; }
    data.Width = 256; data.Height = 32; cache.ReleaseTexture();      // smaller side 32 is not above 32
    if (g_reduces != 1 || g_sets != 1) { std::printf("00b2b120_TEST FAIL\n"); return 1; }
    std::printf("00b2b120_TEST PASS\n"); return 0;
}
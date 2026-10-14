#include <cstdio>
#include <stddef.h>
static unsigned long g_sizeSet = 0; static int g_sets = 0;
struct CGraphicFrameData { unsigned long size; unsigned long Sizeof() { return size; } };
struct CGraphicFrame_Retail { CGraphicFrameData* FrameData; };
struct CCompositeSurfaceCache_Retail {
    virtual void v00() { }
    unsigned char _resource[0x18]; unsigned char _morphs[0x30]; CGraphicFrame_Retail GraphicFrame;
    void SetResourceSize(unsigned long size) { g_sizeSet = size; ++g_sets; }
    void UpdateResourceSize() { SetResourceSize(GraphicFrame.FrameData != 0 ? GraphicFrame.FrameData->Sizeof() + sizeof(GraphicFrame) : sizeof(GraphicFrame)); }
};
int main()
{
    CCompositeSurfaceCache_Retail cache; CGraphicFrameData data = { 100 };
    cache.GraphicFrame.FrameData = &data; cache.UpdateResourceSize();
    if (g_sets != 1 || g_sizeSet != 104 || offsetof(CCompositeSurfaceCache_Retail, GraphicFrame) != 0x4c) { std::printf("00b2afd0_TEST FAIL\n"); return 1; }
    cache.GraphicFrame.FrameData = 0; cache.UpdateResourceSize();
    if (g_sets != 2 || g_sizeSet != 4) { std::printf("00b2afd0_TEST FAIL\n"); return 1; }
    std::printf("00b2afd0_TEST PASS\n"); return 0;
}

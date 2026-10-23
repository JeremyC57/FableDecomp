// 00bac0f0 behaviour test: CEnginePrimitiveManagerRepeatedZSprites::RenderNonsortedPrimitives (match.cpp body, host-modelled types)
#include <stdio.h>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct Sprite { unsigned long MeshKey; float Depth; int Id; };
typedef Sprite* PSprite;
struct CSortList {
    std::vector<PSprite> v;
    unsigned long size() const { return (unsigned long)v.size(); }
    PSprite& operator[](unsigned long i) { return v[i]; }
    void resize(unsigned long n) { v.resize(n); }
};
static bool SortsBefore(const PSprite b, const PSprite a)
{
    if (a->MeshKey > b->MeshKey) return true;
    if (a->MeshKey == b->MeshKey && a->Depth > b->Depth) return true;
    return false;
}
static void Swap(PSprite& a, PSprite& b) { PSprite t = a; a = b; b = t; }
static int g_log[16]; static int g_logN;
struct Scratch {
    int Starts, Ends;
    long StartSurfaceUse(long h) { ++Starts; return h + 100; }
    long StartSurfaceUse2(long h) { ++Starts; return h + 200; }
    void EndSurfaceUse(long) { ++Ends; }
    void EndSurfaceUse2(long) { ++Ends; }
};
struct DM { Scratch ScratchTextures; };
static DM g_dm; static DM* g_pDisplayManager = &g_dm;
struct Light {
    bool PendingAmbient; unsigned char A, B, C; unsigned long DirtyFlags; long PendingLights;
    void Flush()
    {
        if (PendingAmbient) { PendingAmbient = false; DirtyFlags |= 1; }
        if (A != 0 || B != 0 || C != 0) { A = B = C = 0; DirtyFlags |= 0xf; }
        if (PendingLights != 0) { PendingLights = 0; DirtyFlags |= 2; }
    }
};
static Light g_light; static Light* g_pEngineLightingManager = &g_light;
struct Mgr {
    CSortList PrimitiveSortList; long BillboardSurface, ZBufferSurface, BillboardTextureHandle, ZBufferTextureHandle, RenderedCount;
    long Seen[16];
    void RenderFromSortList(PSprite s) { g_log[g_logN++] = s->Id; Seen[0] = BillboardSurface; Seen[1] = ZBufferSurface; }
    void RenderNonsortedPrimitives();
};
void Mgr::RenderNonsortedPrimitives()
{
    Scratch* scratch = &g_pDisplayManager->ScratchTextures;
    BillboardSurface = scratch->StartSurfaceUse(BillboardTextureHandle);
    ZBufferSurface = scratch->StartSurfaceUse2(ZBufferTextureHandle);
    for (long gap = PrimitiveSortList.size() - 1; gap > 0; --gap) {
        for (unsigned long j = 0; j < PrimitiveSortList.size() - gap; ++j) {
            if (SortsBefore(PrimitiveSortList[j + gap], PrimitiveSortList[j]))
                Swap(PrimitiveSortList[j], PrimitiveSortList[j + gap]);
        }
    }
    for (unsigned long i = 0; i < PrimitiveSortList.size(); ++i)
        RenderFromSortList(PrimitiveSortList[i]);
    PrimitiveSortList.resize(0);
    RenderedCount = 0;
    scratch->EndSurfaceUse(BillboardTextureHandle);
    BillboardSurface = 0;
    scratch->EndSurfaceUse2(ZBufferTextureHandle);
    ZBufferSurface = 0;
    g_pEngineLightingManager->Flush();
}
int main()
{
    Sprite s[5] = { { 3, 1.0f, 0 }, { 1, 5.0f, 1 }, { 1, 2.0f, 2 }, { 2, 0.5f, 3 }, { 3, 0.2f, 4 } };
    Mgr m; m.BillboardTextureHandle = 1; m.ZBufferTextureHandle = 2; m.RenderedCount = 9;
    for (int i = 0; i < 5; ++i) m.PrimitiveSortList.v.push_back(&s[i]);
    g_light.PendingAmbient = true; g_light.A = 1; g_light.B = 0; g_light.C = 0; g_light.DirtyFlags = 0; g_light.PendingLights = 3;
    m.RenderNonsortedPrimitives();
    CHECK(g_logN == 5);
    CHECK(g_log[0] == 2 && g_log[1] == 1 && g_log[2] == 3 && g_log[3] == 4 && g_log[4] == 0);   // by mesh, then depth
    CHECK(m.Seen[0] == 101 && m.Seen[1] == 202);
    CHECK(m.PrimitiveSortList.size() == 0 && m.RenderedCount == 0 && m.BillboardSurface == 0 && m.ZBufferSurface == 0);
    CHECK(g_dm.ScratchTextures.Starts == 2 && g_dm.ScratchTextures.Ends == 2);
    CHECK(!g_light.PendingAmbient && g_light.A == 0 && g_light.PendingLights == 0 && g_light.DirtyFlags == 0xf);
    g_logN = 0; m.RenderNonsortedPrimitives(); CHECK(g_logN == 0);
    if (!g_fail) printf("00bac0f0_TEST PASS\n");
    return g_fail;
}
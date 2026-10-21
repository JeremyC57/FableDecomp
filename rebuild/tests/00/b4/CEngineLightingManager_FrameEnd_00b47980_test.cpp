// behaviour test for 00b47980 CEngineLightingManager::FrameEnd
#include <cstdio>
#include <cstring>
struct CObjectEntry { CObjectEntry* Next; };
struct CHeap {
    long AllocCount; bool RefCountReleaseEnabled; CObjectEntry* FreeList; int Released;
    void RefCountedRelease() { ++Released; }
    void Free(void* object)
    {
        --AllocCount;
        ((CObjectEntry*)object)->Next = FreeList;
        FreeList = (CObjectEntry*)object;
        if (AllocCount == 0 && RefCountReleaseEnabled)
            RefCountedRelease();
    }
};
static CHeap g_link, g_local, g_spot;
struct CLink { CLink* Pad; CLink* Next; };
struct CVec {
    void** b; void** e;
    unsigned int size() const { return (unsigned int)(e - b); }
    void clear() { e = b; }
};
struct CMgr {
    CVec LocalLightList, ShadowedSpotLightList;
    CLink* LightLookupGrid[16][16];
    void* SmallObjectLookupGrid[64][64];
    int BaseCalled;
    void FrameEnd()
    {
        for (int x = 0; x < 16; ++x)
            for (int y = 0; y < 16; ++y)
                while (LightLookupGrid[y][x]) {
                    CLink* next = LightLookupGrid[y][x]->Next;
                    g_link.Free(LightLookupGrid[y][x]);
                    LightLookupGrid[y][x] = next;
                }
        std::memset(SmallObjectLookupGrid, 0, sizeof(SmallObjectLookupGrid));
        for (unsigned int i = 0; i < LocalLightList.size(); ++i)
            g_local.Free(LocalLightList.b[i]);
        LocalLightList.clear();
        for (unsigned int j = 0; j < ShadowedSpotLightList.size(); ++j)
            g_spot.Free(ShadowedSpotLightList.b[j]);
        ShadowedSpotLightList.clear();
        ++BaseCalled;
    }
};
static CMgr g_m;
int main()
{
    static CLink links[3];
    static void* lights[2][4];
    static void* spots[1][4];
    std::memset(&g_m, 0, sizeof(g_m));
    links[0].Next = &links[1]; links[1].Next = 0; links[2].Next = 0;
    g_m.LightLookupGrid[3][5] = &links[0];
    g_m.LightLookupGrid[15][0] = &links[2];
    g_m.SmallObjectLookupGrid[10][20] = &g_m;
    void* lv[2] = { lights[0], lights[1] }; void* sv[1] = { spots[0] };
    g_m.LocalLightList.b = lv; g_m.LocalLightList.e = lv + 2;
    g_m.ShadowedSpotLightList.b = sv; g_m.ShadowedSpotLightList.e = sv + 1;
    g_link.AllocCount = 3; g_link.RefCountReleaseEnabled = true;
    g_local.AllocCount = 5; g_local.RefCountReleaseEnabled = true;
    g_spot.AllocCount = 1; g_spot.RefCountReleaseEnabled = false;
    g_m.FrameEnd();
    int bad = 0;
    if (g_m.LightLookupGrid[3][5] || g_m.LightLookupGrid[15][0]) bad |= 1;
    if (g_link.AllocCount != 0 || g_link.Released != 1) bad |= 2;
    if (g_link.FreeList != (CObjectEntry*)&links[1] && g_link.FreeList != (CObjectEntry*)&links[2]) bad |= 4;
    if (g_m.SmallObjectLookupGrid[10][20]) bad |= 8;
    if (g_local.AllocCount != 3 || g_local.Released != 0 || g_local.FreeList != (CObjectEntry*)lights[1]) bad |= 16;
    if (g_spot.AllocCount != 0 || g_spot.Released != 0) bad |= 32;
    if (g_m.LocalLightList.size() != 0 || g_m.ShadowedSpotLightList.size() != 0 || g_m.BaseCalled != 1) bad |= 64;
    if (bad) { std::printf("00b47980_TEST FAIL\n"); return 1; }
    std::printf("00b47980_TEST PASS\n");
    return 0;
}
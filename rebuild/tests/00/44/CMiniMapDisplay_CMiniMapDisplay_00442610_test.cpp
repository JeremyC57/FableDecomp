// Behaviour test: CMiniMapDisplay::~CMiniMapDisplay (0x00442610) -- the body clears RegionMinimapTexture and PRegion before
// the members die in reverse declaration order (RegionMinimap, MinimapList, Markers, the handle vectors), then the base.
#include <cstdio>
static int g_order[8]; static int g_n = 0;
static void* g_seenTexture = (void*)1; static void* g_seenRegion = (void*)1;
struct Member { int id; Member(int i) : id(i) { } ~Member() { g_order[g_n++] = id; } };
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { g_order[g_n++] = 0; } };
class CMiniMapDisplay;
struct CRegionMinimap { CMiniMapDisplay* owner; ~CRegionMinimap(); };
class CMiniMapDisplay : public CBaseClassNonCopyable { public:
    Member MapPrimitiveHandles, MarkerPrimitiveHandles, Markers, MinimapList;
    void* RegionMinimapTexture;
    CRegionMinimap RegionMinimap;
    void* PRegion;
    CMiniMapDisplay() : MapPrimitiveHandles(1), MarkerPrimitiveHandles(2), Markers(3), MinimapList(4),
                        RegionMinimapTexture((void*)5), PRegion((void*)6) { RegionMinimap.owner = this; }
    virtual ~CMiniMapDisplay() { RegionMinimapTexture = 0; PRegion = 0; }
};
CRegionMinimap::~CRegionMinimap() { g_seenTexture = owner->RegionMinimapTexture; g_seenRegion = owner->PRegion; g_order[g_n++] = 5; }
int main()
{
    { CMiniMapDisplay d; }
    if (g_seenTexture != 0 || g_seenRegion != 0 || g_n != 6 || g_order[0] != 5 || g_order[1] != 4 || g_order[2] != 3 ||
        g_order[3] != 2 || g_order[4] != 1 || g_order[5] != 0) { std::printf("00442610_TEST FAIL\n"); return 1; }
    std::printf("00442610_TEST PASS\n");
    return 0;
}
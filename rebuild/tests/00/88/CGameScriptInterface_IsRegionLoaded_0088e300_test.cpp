// Behaviour test for CGameScriptInterface::IsRegionLoaded @ 0x0088e300 (standalone model): the world map is fetched twice through
// CWorld slot 13 (PeekWorldMap); the answer is GetRegionNumberFromName(name) == GetLoadedRegion().
#include <cstdio>

class CCharString;
static long g_region_from_name;
static long g_loaded_region;
static int g_peek_calls;

class CWorldMap { public:
    long GetRegionNumberFromName(const CCharString&) const { return g_region_from_name; }
    long GetLoadedRegion() const { return g_loaded_region; }
};
class CWorld { public:
    CWorldMap Map;
    const CWorldMap& PeekWorldMap() const { ++g_peek_calls; return Map; }
};
class CGameScriptInterface { public:
    CWorld* World;
    bool IsRegionLoaded(const CCharString& regionName) const
    {
        long region = World->PeekWorldMap().GetRegionNumberFromName(regionName);
        long current = World->PeekWorldMap().GetLoadedRegion();
        return (bool)(region == current);
    }
};

int main()
{
    CWorld world;
    CGameScriptInterface gsi;
    gsi.World = &world;
    const CCharString* name = 0;
    g_region_from_name = 7; g_loaded_region = 7; g_peek_calls = 0;
    bool loaded = gsi.IsRegionLoaded(*name);
    bool ok = loaded && g_peek_calls == 2;
    g_region_from_name = 7; g_loaded_region = 3;
    bool notLoaded = gsi.IsRegionLoaded(*name);
    ok = ok && !notLoaded;
    if (ok) std::printf("REGION_LOADED_OK\n");
    else std::printf("FAIL loaded=%d notLoaded=%d calls=%d\n", (int)loaded, (int)notLoaded, g_peek_calls);
    return 0;
}

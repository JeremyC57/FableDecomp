// Behaviour test for 0x00729CA0 CTCActionUseScriptedHook::GetRegionEntranceDisplayName: map number at the connected entrance's
// position -> region number of that map -> that region's display name.
#include <cstdio>
#include <cstring>

class C3DVector { public: float X, Y, Z; };
class CThing { public:
    C3DVector Pos;
    const C3DVector& GetPos() const { return Pos; }
};
class CRegion { public:
    const char* DisplayName;
    const char* const& GetDisplayName() const { return DisplayName; }
};
class CWorldMap { public:
    virtual long GetMapNumberAt(const C3DVector& pos) const { return pos.X < 100.0f ? 1 : 2; }
    long GetRegionNumberMapIsIn(long map) const { return map == 1 ? 0 : 1; }
    const CRegion& GetRegion(long region) const { return Regions[region]; }
    CRegion Regions[2];
};
static CWorldMap g_map;
static int g_peeks = 0;
class CTCBase { public:
    const CWorldMap& PeekWorldMap() const { ++g_peeks; return g_map; }
};
class CTCActionUseScriptedHook : public CTCBase { public:
    CThing* PEntranceConnectedTo;
    const char* const& GetRegionEntranceDisplayName() const
    {
        const CWorldMap& worldMap = PeekWorldMap();
        long mapNumber = worldMap.GetMapNumberAt(PEntranceConnectedTo->GetPos());
        return PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(mapNumber)).GetDisplayName();
    }
};

int main()
{
    g_map.Regions[0].DisplayName = "Oakvale";
    g_map.Regions[1].DisplayName = "Bowerstone";
    CThing entrance; entrance.Pos.X = 50.0f; entrance.Pos.Y = 0.0f; entrance.Pos.Z = 0.0f;
    CTCActionUseScriptedHook hook; hook.PEntranceConnectedTo = &entrance;
    int ok = std::strcmp(hook.GetRegionEntranceDisplayName(), "Oakvale") == 0 && g_peeks == 3;
    entrance.Pos.X = 500.0f;
    ok = ok && std::strcmp(hook.GetRegionEntranceDisplayName(), "Bowerstone") == 0 && g_peeks == 6;
    if (!ok) { std::printf("00729ca0_TEST FAIL\n"); return 1; }
    std::printf("00729ca0_TEST PASS\n");
    return 0;
}
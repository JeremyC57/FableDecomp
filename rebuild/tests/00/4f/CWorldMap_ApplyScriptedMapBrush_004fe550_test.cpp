// Behaviour test for 0x004FE550 CWorldMap::ApplyScriptedMapBrush(brush, apply): the brush name is looked up in the
// loaded-brush table; a registered brush forwards (name, apply) to the map record it lives in, an unknown brush does
// nothing.
#include <cstdio>
#include <cstring>

struct MapModel {
    int Applies;
    const char* LastName;
    bool LastApply;
    void ApplyScriptedMapBrush(const char* name, bool apply) { ++Applies; LastName = name; LastApply = apply; }
};

struct MapInfoModel { MapModel* PMap; };

struct BrushEntry { const char* Name; long MapNumber; };

struct WorldMapModel {
    BrushEntry Loaded[4];
    int LoadedCount;
    MapInfoModel Maps[3];
    const BrushEntry* find(const char* name) const
    {
        for (int i = 0; i < LoadedCount; ++i)
            if (std::strcmp(Loaded[i].Name, name) == 0) return &Loaded[i];
        return 0;                                   // end()
    }
    void ApplyScriptedMapBrush(const char* brushName, bool apply)
    {
        const char* name = brushName;
        const BrushEntry* it = find(name);
        if (it != 0)
            Maps[it->MapNumber].PMap->ApplyScriptedMapBrush(name, apply);
    }
};

int main()
{
    MapModel m0 = { 0, 0, false }, m1 = { 0, 0, false }, m2 = { 0, 0, false };
    WorldMapModel w;
    w.Loaded[0].Name = "BRIDGE_UP"; w.Loaded[0].MapNumber = 2;
    w.Loaded[1].Name = "GATE_OPEN"; w.Loaded[1].MapNumber = 0;
    w.LoadedCount = 2;
    w.Maps[0].PMap = &m0; w.Maps[1].PMap = &m1; w.Maps[2].PMap = &m2;
    w.ApplyScriptedMapBrush("BRIDGE_UP", true);
    w.ApplyScriptedMapBrush("GATE_OPEN", false);
    w.ApplyScriptedMapBrush("UNKNOWN", true);
    if (m2.Applies != 1 || !m2.LastApply || std::strcmp(m2.LastName, "BRIDGE_UP") != 0 || m0.Applies != 1 || m0.LastApply || m1.Applies != 0) {
        std::printf("004fe550_TEST FAIL\n");
        return 1;
    }
    std::printf("004fe550_TEST PASS\n");
    return 0;
}
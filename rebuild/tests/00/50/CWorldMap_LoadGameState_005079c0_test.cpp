// Behaviour test for 0x005079C0 CWorldMap::LoadGameState(CPersistContext&): binary contexts read the region count,
// resize Regions to it and load each region from the second on (name then state); text contexts parse
// `REGION "name" <state> END_REGION` blocks, loading the state of known regions (index > 0) and skipping unknown ones.
#include <cstdio>
#include <cstring>

struct CRegion {
    char Name[32];
    int  State;
};
struct CWorldMap {
    CRegion Regions[8];
    int     RegionCount;
    int GetRegionNumberFromName(const char* name) const
    {
        for (int i = 0; i < RegionCount; ++i)
            if (std::strcmp(Regions[i].Name, name) == 0)
                return i;
        return -1;
    }
};

// binary stream: count, then (name, state) per region from index 1
struct CBinaryIn {
    unsigned int count;
    const char* names[8];
    int states[8];
    int pos;
};
static void LoadBinary(CWorldMap& map, CBinaryIn& in)
{
    unsigned int count = 0;
    count = in.count;
    map.RegionCount = (int)count;
    for (unsigned int r = 0; r < count; ++r) { map.Regions[r].Name[0] = 0; map.Regions[r].State = 0; }
    for (unsigned int i = 1; i < count; ++i) {
        std::strcpy(map.Regions[i].Name, in.names[in.pos]);
        map.Regions[i].State = in.states[in.pos];
        ++in.pos;
    }
}
static void LoadText(CWorldMap& map, const char* text)
{
    const char* p = text;
    while (*p == ' ') ++p;
    while (*p) {
        char keyword[32], name[32];
        int n = 0;
        std::sscanf(p, "%31s \"%31[^\"]\"%n", keyword, name, &n);
        p += n;
        int index = map.GetRegionNumberFromName(name);
        const char* end = std::strstr(p, "END_REGION");
        if (index > 0) {
            int state = 0;
            std::sscanf(p, "%d", &state);
            map.Regions[index].State = state;
        }
        p = end ? end + 10 : p + std::strlen(p);
        while (*p == ' ') ++p;
    }
}

int main()
{
    int ok = 1;
    CWorldMap map;
    CBinaryIn in = { 3, { "Bowerstone", "Oakvale" }, { 7, 9 }, 0 };
    LoadBinary(map, in);
    if (map.RegionCount != 3 || std::strcmp(map.Regions[1].Name, "Bowerstone") != 0 || map.Regions[2].State != 9) ok = 0;
    if (map.Regions[0].State != 0 || in.pos != 2) ok = 0;
    LoadText(map, "REGION \"Oakvale\" 42 END_REGION REGION \"Nowhere\" 5 END_REGION REGION \"Bowerstone\" 11 END_REGION");
    if (map.Regions[2].State != 42 || map.Regions[1].State != 11) ok = 0;
    std::strcpy(map.Regions[0].Name, "Base");
    LoadText(map, "REGION \"Base\" 99 END_REGION");          // index 0 is never loaded from text
    if (map.Regions[0].State != 0) ok = 0;
    if (!ok) { std::printf("005079c0_TEST FAIL\n"); return 1; }
    std::printf("005079c0_TEST PASS\n");
    return 0;
}
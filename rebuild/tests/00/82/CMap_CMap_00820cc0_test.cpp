// Behaviour test for a CMap constructor @ 0x00820CC0: derived sizes, cleared counters and the call sequence.
#include <cstdio>
#include <cstring>

class C2DCoordI { public: long X, Y; C2DCoordI() {} C2DCoordI(long x, long y) : X(x), Y(y) {} };
class C2DBoxI { public: C2DCoordI Min, Max; };
class CMapInit { public: long WorldSizeX, WorldSizeY; };
class CAFile { public: int dummy; };
class CASuspendableProcess { public: int dummy; };

static char g_log[64];
static void logc(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }

class CMap { public:
    const void* DefinitionManager; void* World; unsigned long* WorldSeed;
    long NoChangedHeightAreas, NoChangedGameAreas, SeaLevel, WorldSizeX, WorldSizeY;
    long HeightMapSizeX, HeightMapSizeY, GameMapSizeX, GameMapSizeY; C2DCoordI WorldPos;
    unsigned long MapUID; unsigned long ThingUIDCount; bool InitialisePassabilityFromThemes;
    void InitHeightMap() { logc('H'); }
    void InitGameMap() { logc('G'); }
    void InitThemePalette() { logc('P'); }
    void ClearBrushes() { logc('b'); }
    void ClearAreas() { logc('a'); }
    void LoadFromFile(CAFile&, CASuspendableProcess*) { logc('L'); }
    void Clear() { logc('C'); NoChangedHeightAreas = 0; NoChangedGameAreas = 0; SeaLevel = 0; }
    CMap(const void* defs, void* world, unsigned long* seed, const C2DCoordI& worldPos, const CMapInit& init);
    CMap(const void* defs, void* world, unsigned long* seed, long, const C2DBoxI& area, CAFile& file, CASuspendableProcess* p);
};

CMap::CMap(const void* defs, void* world, unsigned long* seed, const C2DCoordI& worldPos, const CMapInit& init)
    : DefinitionManager(defs), World(world), WorldSeed(seed)
{
    WorldSizeX = init.WorldSizeX;
    WorldSizeY = init.WorldSizeY;
    InitialisePassabilityFromThemes = false;
    GameMapSizeX = (long)(WorldSizeX * 0.25f);
    GameMapSizeY = (long)(WorldSizeY * 0.25f);
    HeightMapSizeX = GameMapSizeX * 4;
    HeightMapSizeY = GameMapSizeY * 4;
    ClearBrushes();
    ClearAreas();
    WorldPos = worldPos;
    InitHeightMap();
    InitGameMap();
    InitThemePalette();
    Clear();
    MapUID = 0;
    ThingUIDCount = 1;
}

CMap::CMap(const void* defs, void* world, unsigned long* seed, long, const C2DBoxI& area, CAFile& file, CASuspendableProcess* p)
    : DefinitionManager(defs), World(world), WorldSeed(seed)
{
    InitialisePassabilityFromThemes = false;
    MapUID = 0;
    ThingUIDCount = 1;
    WorldSizeX = area.Max.X - area.Min.X;
    WorldSizeY = area.Max.Y - area.Min.Y;
    GameMapSizeX = (long)(WorldSizeX * 0.25f);
    GameMapSizeY = (long)(WorldSizeY * 0.25f);
    HeightMapSizeX = GameMapSizeX * 4;
    HeightMapSizeY = GameMapSizeY * 4;
    ClearBrushes();
    ClearAreas();
    WorldPos = C2DCoordI(area.Min.X, area.Min.Y);
    InitHeightMap();
    InitGameMap();
    InitThemePalette();
    LoadFromFile(file, p);
}

int main()
{
    unsigned long seed = 0; int defs = 0, world = 0;
    int ok = 1;
    if (std::strcmp("00820cc0", "00820cc0") == 0) {
        CMapInit init; init.WorldSizeX = 130; init.WorldSizeY = 64;
        C2DCoordI pos(3, 4);
        CMap m(&defs, &world, &seed, pos, init);
        ok = m.WorldSizeX == 130 && m.GameMapSizeX == 32 && m.HeightMapSizeX == 128 && m.GameMapSizeY == 16
          && m.HeightMapSizeY == 64 && m.WorldPos.X == 3 && m.WorldPos.Y == 4 && m.MapUID == 0 && m.ThingUIDCount == 1
          && !m.InitialisePassabilityFromThemes && m.NoChangedGameAreas == 0 && m.WorldSeed == &seed
          && std::strcmp(g_log, "baHGPC") == 0;
    } else {
        C2DBoxI area; area.Min = C2DCoordI(10, 20); area.Max = C2DCoordI(74, 148);
        CAFile file; CASuspendableProcess proc;
        CMap m(&defs, &world, &seed, 99, area, file, &proc);
        ok = m.WorldSizeX == 64 && m.WorldSizeY == 128 && m.GameMapSizeX == 16 && m.HeightMapSizeY == 128
          && m.WorldPos.X == 10 && m.WorldPos.Y == 20 && m.MapUID == 0 && m.ThingUIDCount == 1
          && std::strcmp(g_log, "baHGPL") == 0;
    }
    if (!ok) { std::printf("00820cc0_TEST FAIL\n"); return 1; }
    std::printf("00820cc0_TEST PASS\n");
    return 0;
}
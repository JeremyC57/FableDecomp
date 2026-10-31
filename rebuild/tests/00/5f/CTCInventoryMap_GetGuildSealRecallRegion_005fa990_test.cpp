// Behaviour test for CTCInventoryMap::GetGuildSealRecallRegion (0x005FA990): a copy of the model with a small world map whose
// map number is the sign of X and whose region is map number + 10.
#include <cstdio>

class C3DVector {
public:
    float X, Y, Z;
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
};

const float g_positionEpsilon = 1e-4f;

class CWorldMap {
public:
    virtual ~CWorldMap() {}
    long GetMapNumberAt(const C3DVector& pos) const { ++m_lookups; return pos.X < 0.0f ? 1 : 2; }
    long GetRegionNumberMapIsIn(long map) const { return map + 10; }
    mutable int m_lookups;
};

class CWorld {
public:
    virtual ~CWorld() {}
    const CWorldMap& PeekWorldMap() const { return Map; }
    const C3DVector& GetGuildSealRecallPos() const { return GuildSealRecallPos; }
    C3DVector GuildSealRecallPos;
    CWorldMap Map;
};

CWorld g_world;

class CTCInventoryMap {
public:
    const CWorld& PeekWorld() const { return g_world; }
    long GetGuildSealRecallRegion() const
    {
        const C3DVector& recallPos = PeekWorld().GetGuildSealRecallPos();
        if (recallPos.GetSquaredMagnitude() > g_positionEpsilon * g_positionEpsilon) {
            const CWorldMap& worldMap = PeekWorld().PeekWorldMap();
            return worldMap.GetRegionNumberMapIsIn(worldMap.GetMapNumberAt(recallPos));
        }
        return 0;
    }
};

int main()
{
    CTCInventoryMap map;
    g_world.Map.m_lookups = 0;
    g_world.GuildSealRecallPos.X = 0.0f; g_world.GuildSealRecallPos.Y = 0.0f; g_world.GuildSealRecallPos.Z = 0.0f;
    if (map.GetGuildSealRecallRegion() != 0 || g_world.Map.m_lookups != 0) { std::printf("005fa990_TEST FAIL\n"); return 1; }
    g_world.GuildSealRecallPos.X = 0.00001f;                  // below the 1e-4 length threshold: still "unset"
    if (map.GetGuildSealRecallRegion() != 0) { std::printf("005fa990_TEST FAIL\n"); return 1; }
    g_world.GuildSealRecallPos.X = -5.0f; g_world.GuildSealRecallPos.Y = 2.0f;
    if (map.GetGuildSealRecallRegion() != 11 || g_world.Map.m_lookups != 1) { std::printf("005fa990_TEST FAIL\n"); return 1; }
    g_world.GuildSealRecallPos.X = 0.0f; g_world.GuildSealRecallPos.Y = 0.0f; g_world.GuildSealRecallPos.Z = 3.0f;
    if (map.GetGuildSealRecallRegion() != 12) { std::printf("005fa990_TEST FAIL\n"); return 1; }
    std::printf("005fa990_TEST PASS\n");
    return 0;
}
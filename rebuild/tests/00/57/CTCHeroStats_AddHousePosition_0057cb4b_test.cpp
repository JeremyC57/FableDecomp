#include <cstdio>
// Behaviour of CTCHeroStats::AddHousePosition (0x0057CB4B): appends one (region, position) record.
struct CRegion { int id; };
struct C3DVector {
    float x, y, z;
    C3DVector() {}
    C3DVector(const C3DVector& other) : x(other.x), y(other.y), z(other.z) {}
};
struct CHousePosition {
    const CRegion* first;
    C3DVector second;
    CHousePosition(const CRegion* const& region, const C3DVector& position) : first(region), second(position) {}
};
static const CRegion* g_region = 0; static float g_x = 0, g_y = 0, g_z = 0; static int g_pushes = 0;
struct CHousePositionVector {
    void push_back(const CHousePosition& value) { ++g_pushes; g_region = value.first; g_x = value.second.x; g_y = value.second.y; g_z = value.second.z; }
};
struct CTCHeroStats_Retail {
    CHousePositionVector HousePositions;
    void AddHousePosition(const CRegion* region, const C3DVector& position)
    {
        HousePositions.push_back(CHousePosition(region, position));
    }
};
int main()
{
    CRegion region; C3DVector at; at.x = 1.5f; at.y = -2.0f; at.z = 8.25f;
    CTCHeroStats_Retail s;
    s.AddHousePosition(&region, at);
    if (g_pushes != 1 || g_region != &region || g_x != 1.5f || g_y != -2.0f || g_z != 8.25f) { std::printf("0057cb4b_TEST FAIL\n"); return 1; }
    std::printf("0057cb4b_TEST PASS\n");
    return 0;
}
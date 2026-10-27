// Behaviour test for 0x00838950 CThing::IsLineOfSightTo_00838950(const CThing&) const: asks the world map for line of
// sight from this thing to the other with flag true, returning its verdict.
#include <cstdio>

class Thing;
class WorldMap {
public:
    bool blocked;
    mutable const Thing* from;
    mutable const Thing* to;
    mutable bool flag;
    bool IsLineOfSight(const Thing& f, const Thing& t, bool fl) const { from = &f; to = &t; flag = fl; return !blocked; }
};
static WorldMap g_map;

class Thing {
public:
    bool IsLineOfSightTo(const Thing& other) const
    {
        if (!PeekWorldMap().IsLineOfSight(*this, other, true))
            return false;
        return true;
    }
protected:
    const WorldMap& PeekWorldMap() const { return g_map; }
};

int main()
{
    Thing a, b;
    g_map.blocked = false; g_map.flag = false;
    if (!a.IsLineOfSightTo(b) || g_map.from != &a || g_map.to != &b || !g_map.flag) { std::printf("00838950_TEST FAIL\n"); return 1; }
    g_map.blocked = true;
    if (a.IsLineOfSightTo(b)) { std::printf("00838950_TEST FAIL\n"); return 1; }
    std::printf("00838950_TEST PASS\n");
    return 0;
}
// Behaviour test for CWorldMap::AreaCoversOneLevel 0x004FB160: four corners in one non-zero map.
#include <cstdio>

class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };

class CWorldMap { public:
    virtual long GetMapNumberAt(const C2DCoordI& pos) const
    {
        // 2 maps side by side: x < 100 -> map 1, 100 <= x < 200 -> map 2, beyond -> 0
        if (pos.X < 0 || pos.Y < 0 || pos.Y >= 100) return 0;
        if (pos.X < 100) return 1;
        if (pos.X < 200) return 2;
        return 0;
    }
    virtual ~CWorldMap() {}
    bool AreaCoversOneLevel(const C2DBoxI& area) const;
};

bool CWorldMap::AreaCoversOneLevel(const C2DBoxI& area) const
{
    if (GetMapNumberAt(C2DCoordI(area.TLX, area.TLY)) == GetMapNumberAt(C2DCoordI(area.BRX, area.TLY))
        && GetMapNumberAt(C2DCoordI(area.BRX, area.TLY)) == GetMapNumberAt(C2DCoordI(area.TLX, area.BRY))
        && GetMapNumberAt(C2DCoordI(area.TLX, area.BRY)) == GetMapNumberAt(C2DCoordI(area.BRX, area.BRY))
        && GetMapNumberAt(C2DCoordI(area.BRX, area.BRY)) != 0)
        return true;
    return false;
}

int main()
{
    CWorldMap m; bool ok = true;
    C2DBoxI a = { 10, 10, 50, 50 };    if (!m.AreaCoversOneLevel(a)) ok = false;
    C2DBoxI b = { 110, 10, 150, 90 };  if (!m.AreaCoversOneLevel(b)) ok = false;
    C2DBoxI c = { 50, 10, 150, 50 };   if (m.AreaCoversOneLevel(c)) ok = false;   // spans two maps
    C2DBoxI d = { 10, 50, 50, 150 };   if (m.AreaCoversOneLevel(d)) ok = false;   // bottom outside
    C2DBoxI e = { 300, 10, 350, 50 };  if (m.AreaCoversOneLevel(e)) ok = false;   // all corners in map 0
    if (!ok) { std::printf("004fb160_TEST FAIL\n"); return 1; }
    std::printf("004fb160_TEST PASS\n");
    return 0;
}
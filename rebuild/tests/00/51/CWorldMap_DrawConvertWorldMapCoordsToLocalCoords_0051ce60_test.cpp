// Behaviour test (0x0051ce60): CWorldMap::DrawConvertWorldMapCoordsToLocalCoords (CIDrawWorldMap slot 6) forwards to the world-map conversion
#include <cstdio>
#include <cmath>
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CMapInfo { public: C2DBoxI Area; unsigned char _rest[0x38]; };
template <class T> class CArray {
public:
    T* Start; T* Finish; T* EndOfStorage;
    const T& operator[](unsigned long i) const { return Start[i]; }
};
class CBaseClass { public: virtual ~CBaseClass() { } };
class CIDrawWorldMap {
public:
    virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const = 0;
    virtual ~CIDrawWorldMap() { }
};
class CWorldMap : public CBaseClass, public CIDrawWorldMap {
public:
    CArray<CMapInfo> Maps;
    C3DVector ConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const
    {
        const C2DBoxI& area = Maps[map].Area;
        return C3DVector(pos.X - area.TLX, pos.Y - area.TLY, pos.Z);
    }
    virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const
    {
        return ConvertWorldMapCoordsToLocalCoords(map, pos);
    }
};
int main()
{
    CMapInfo infos[2];
    infos[1].Area.TLX = 40; infos[1].Area.TLY = 80; infos[1].Area.BRX = 0; infos[1].Area.BRY = 0;
    CWorldMap wm; wm.Maps.Start = infos; wm.Maps.Finish = infos + 2; wm.Maps.EndOfStorage = infos + 2;
    const CIDrawWorldMap& draw = wm;            // through the secondary interface, as the GUI calls it
    C3DVector r = draw.DrawConvertWorldMapCoordsToLocalCoords(1, C3DVector(41.5f, 90.0f, -2.0f));
    if (std::fabs(r.X - 1.5f) > 1e-4f || std::fabs(r.Y - 10.0f) > 1e-4f || std::fabs(r.Z + 2.0f) > 1e-4f)
        { std::printf("0051ce60_TEST FAIL\n"); return 1; }
    std::printf("0051ce60_TEST PASS\n");
    return 0;
}
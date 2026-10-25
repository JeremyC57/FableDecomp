// Behaviour test (0x004fc4a0): CWorldMap world-map <-> local coordinate conversion against a two-map table
#include <cstdio>
#include <cmath>
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CMapInfo {
public:
    C2DBoxI       Area;
    unsigned char _rest[0x38];
    CMapInfo() : Area(0, 0, 0, 0) { }
};
template <class T> class CArray {
public:
    T* Start;
    T* Finish;
    T* EndOfStorage;
    const T& operator[](unsigned long i) const { return Start[i]; }
};
class CWorldMap {
public:
    CArray<CMapInfo> Maps;
    long GetMapNumberAt(const C2DCoordI& pos) const { return pos.X >= 100 ? 1 : 0; }   // stand-in for slot 17
    C3DVector Conv(long map, const C3DVector& pos) const
    {
        const C2DBoxI& area = Maps[map].Area;
        return C3DVector(pos.X + area.TLX, pos.Y + area.TLY, pos.Z);
    }
};
static bool feq(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    CMapInfo infos[2];
    infos[0].Area = C2DBoxI(10, 20, 100, 200);
    infos[1].Area = C2DBoxI(100, 50, 300, 400);
    CWorldMap wm;
    wm.Maps.Start = infos; wm.Maps.Finish = infos + 2; wm.Maps.EndOfStorage = infos + 2;
    C3DVector r = wm.Conv(0, C3DVector(1.5f, 2.0f, 7.0f));
    if (!feq(r.X, 11.5f) || !feq(r.Y, 22.0f) || !feq(r.Z, 7.0f)) { std::printf("004fc4a0_TEST FAIL\n"); return 1; }
    std::printf("004fc4a0_TEST PASS\n");
    return 0;
}
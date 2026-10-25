// Behaviour test (0x004fc2d0): CWorldMap world-map <-> local coordinate conversion against a two-map table
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
        return C3DVector(pos.X - area.TLX, pos.Y - area.TLY, pos.Z);
    }
    C3DVector ConvPos(const C3DVector& pos) const
    {
        return Conv(GetMapNumberAt(C2DCoordI((long)pos.X, (long)pos.Y)), pos);
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
    C3DVector r = wm.ConvPos(C3DVector(150.5f, 70.0f, 3.0f));
    if (!feq(r.X, 50.5f) || !feq(r.Y, 20.0f) || !feq(r.Z, 3.0f)) { std::printf("004fc2d0_TEST FAIL\n"); return 1; }
    C3DVector s = wm.ConvPos(C3DVector(50.0f, 30.0f, 1.0f));
    if (!feq(s.X, 40.0f) || !feq(s.Y, 10.0f) || !feq(s.Z, 1.0f)) { std::printf("004fc2d0_TEST FAIL\n"); return 1; }
    std::printf("004fc2d0_TEST PASS\n");
    return 0;
}
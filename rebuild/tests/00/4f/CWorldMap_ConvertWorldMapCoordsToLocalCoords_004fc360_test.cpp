// Behaviour test (0x004fc360): CWorldMap world-map <-> local coordinate conversion against a two-map table
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
    C2DVector Conv(long map, const C2DVector& pos) const
    {
        const C2DBoxI& area = Maps[map].Area;
        return C2DVector(pos.X - area.TLX, pos.Y - area.TLY);
    }
    C2DVector ConvPos(const C2DVector& pos) const
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
    C2DVector r = wm.ConvPos(C2DVector(120.0f, 60.0f));
    if (!feq(r.X, 20.0f) || !feq(r.Y, 10.0f)) { std::printf("004fc360_TEST FAIL\n"); return 1; }
    C2DVector s = wm.ConvPos(C2DVector(99.5f, 21.0f));
    if (!feq(s.X, 89.5f) || !feq(s.Y, 1.0f)) { std::printf("004fc360_TEST FAIL\n"); return 1; }
    std::printf("004fc360_TEST PASS\n");
    return 0;
}
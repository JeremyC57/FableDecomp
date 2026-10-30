// Behaviour test for GetBestThingFromMarkerList 0x00CBF13C (body copied from differ/00cbf13c.cpp; CScriptThing mocked).
#include <cstdio>
#include <cmath>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
extern const float g_epsilon_0129ba3c;
const float g_epsilon_0129ba3c = 1e-4f;
class C3DVector { public:
    float X, Y, Z;
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline const C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    __forceinline float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
    __forceinline const float Dot(const C3DVector& other) const { return X * other.X + Y * other.Y + Z * other.Z; }
    C3DVector& AccurateNormalise();
    __forceinline C3DVector& Normalise()
    {
        if (GetSquaredMagnitude() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
            AccurateNormalise();
        return *this;
    }
};
static int g_normalised = 0;
C3DVector& C3DVector::AccurateNormalise()
{
    ++g_normalised;
    float m = std::sqrt(X * X + Y * Y + Z * Z);
    X /= m; Y /= m; Z /= m;
    return *this;
}
class CScriptThing {                                    // mock: id, alive flag, position
public:
    CScriptThing() : Id(-1), Alive(false), Pos(0, 0, 0) {}
    CScriptThing(int id, bool alive, float x, float y, float z) : Id(id), Alive(alive), Pos(x, y, z) {}
    virtual ~CScriptThing() {}
    virtual const C3DVector& GetPos() const { return Pos; }
    virtual bool IsAlive() const { return Alive; }
    int Id; bool Alive; C3DVector Pos;
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
};
}

CScriptThing __fastcall GetBestThingFromMarkerList(const std::vector<CScriptThing>& markers, const CScriptThing& target,
                                                   const CScriptThing& origin, bool preferLargest)
{
    if (!target.IsAlive() || !origin.IsAlive())
        return CScriptThing();
    C3DVector direction = target.GetPos() - origin.GetPos();
    direction.Z = 0.0f;
    direction.Normalise();
    float bestDot = -2.0f;
    CScriptThing best;
    for (const CScriptThing* it = markers.begin(); it != markers.end(); ++it) {
        C3DVector markerDirection = it->GetPos() - origin.GetPos();
        markerDirection.Z = 0.0f;
        markerDirection.Normalise();
        float dot = markerDirection.Dot(direction);
        if (preferLargest ? dot > bestDot : dot < bestDot) {
            bestDot = dot;
            best = *it;
        }
    }
    return best;
}

int main()
{
    CScriptThing items[4] = {
        CScriptThing(1, true, 0, 10, 50),     // straight ahead (+Y), height ignored
        CScriptThing(2, true, 10, 0, 0),      // to the side (+X)
        CScriptThing(3, true, 3, 9, -40),     // slightly off ahead
        CScriptThing(4, true, 0, -10, 0),     // behind
    };
    std::vector<CScriptThing> markers;
    markers._M_start = items; markers._M_finish = items + 4; markers._M_end_of_storage = items + 4;
    CScriptThing origin(10, true, 0, 0, 7);
    CScriptThing target(11, true, 0, 100, -3);
    CScriptThing dead(12, false, 0, 100, 0);

    CScriptThing r = GetBestThingFromMarkerList(markers, target, origin, true);
    if (r.Id != 1) { std::printf("00cbf13c_TEST FAIL largest %d\n", r.Id); return 1; }
    if (g_normalised != 5) { std::printf("00cbf13c_TEST FAIL normalise count %d\n", g_normalised); return 1; }
    // smallest-dot mode: bestDot starts at -2, so no marker ever beats it and the default (null) thing comes back
    r = GetBestThingFromMarkerList(markers, target, origin, false);
    if (r.Id != -1) { std::printf("00cbf13c_TEST FAIL smallest %d\n", r.Id); return 1; }
    // a dead target or origin returns the null thing without walking the markers
    g_normalised = 0;
    r = GetBestThingFromMarkerList(markers, dead, origin, true);
    if (r.Id != -1 || g_normalised != 0) { std::printf("00cbf13c_TEST FAIL dead target\n"); return 1; }
    r = GetBestThingFromMarkerList(markers, target, dead, true);
    if (r.Id != -1 || g_normalised != 0) { std::printf("00cbf13c_TEST FAIL dead origin\n"); return 1; }
    // a marker on top of the origin keeps its zero direction (no normalise) and scores 0
    CScriptThing onTop[2] = { CScriptThing(20, true, 0, 0, 99), CScriptThing(21, true, -5, -5, 0) };
    markers._M_start = onTop; markers._M_finish = onTop + 2;
    r = GetBestThingFromMarkerList(markers, target, origin, true);
    if (r.Id != 20 || g_normalised != 2) { std::printf("00cbf13c_TEST FAIL zero %d %d\n", r.Id, g_normalised); return 1; }
    // empty list
    markers._M_finish = markers._M_start;
    r = GetBestThingFromMarkerList(markers, target, origin, true);
    if (r.Id != -1) { std::printf("00cbf13c_TEST FAIL empty\n"); return 1; }
    std::printf("00cbf13c_TEST PASS\n");
    return 0;
}

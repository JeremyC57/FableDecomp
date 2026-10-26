// CTools::GetIntersectionWithWorldMap(const C3DVector& start, const C3DVector& end, C3DVector* position, C3DVector* normal, bool flag, bool (__fastcall *test)(CWorldMap*, const C3DVector*, const C3DVector*, bool)) -- march from start towards end in steps (0.5, shrinking by 10x on each hit) testing each step against the world map; each hit refines the end point and writes the midpoint and its ground normal; true when anything was hit (up to 100 steps / 4 hits)
// Leaf: GetIntersectionWithWorldMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008751D0, length 713 bytes (`ret 0x18`).
// Callees: the world map from this->[0]->+0x74 virtual slot 13 (+0x34); CWorldMap::GetGroundFaceNormalAt 0x0063AB10 (C3DVector by
//   hidden pointer); the caller's test (__fastcall). Constants: 1e-4f 0x0129BA3C, 1.0f 0x0122DED8, 0.1f 0x0122EE58, 0.5f 0x0122F59C,
//   0.0f 0x0122DEDC.
// Shape: `return hits > 0 ? true : false;` (the plain comparison zeroes eax before the setg); the hit arm resets
//   remaining to the initial start-end vector, so the loop runs on while the march has not passed the end.
// Layout: CTools first member the owner (+0x00) whose +0x74 object hands out the world map (slot 13).
#include <math.h>
#pragma intrinsic(sqrt)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
class CWorldMap { public:
    C3DVector GetGroundFaceNormalAt(const C3DVector* position) const;   // 0x0063AB10
};
struct CWorldMapOwner {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* GetWorldMap();                                   // slot 13 (+0x34)
};
struct CToolsOwner {
    unsigned char   _pad00[0x74];
    CWorldMapOwner* WorldMapOwner;                                      // +0x74
};
typedef bool (__fastcall *TWorldMapTest)(CWorldMap* map, const C3DVector* from, const C3DVector* to, bool flag);
struct CTools {
    CToolsOwner* Owner;                                                 // +0x00
    bool GetIntersectionWithWorldMap(const C3DVector& start, const C3DVector& end, C3DVector* position, C3DVector* normal,
                                     bool flag, TWorldMapTest test);
};

bool CTools::GetIntersectionWithWorldMap(const C3DVector& start, const C3DVector& end, C3DVector* position, C3DVector* normal,
                                         bool flag, TWorldMapTest test)
{
    float step = 0.5f;
    CWorldMap* map = Owner->WorldMapOwner->GetWorldMap();
    C3DVector direction = end - start;
    float lengthSquared = direction.GetLengthSquared();
    if (lengthSquared < 0.0001f)
        return false;
    float length = (float)sqrt(lengthSquared);
    C3DVector from = start;
    C3DVector to = end;
    long hits = 0;
    long steps = 0;
    float invLength = 1.0f / length;
    direction.X *= invLength;
    direction.Y *= invLength;
    direction.Z *= invLength;
    C3DVector remaining;
    C3DVector initial = from - to;
    do {
        ++steps;
        C3DVector next = from + direction * step;
        if (test(map, &from, &next, flag)) {
            step *= 0.1f;
            to = next;
            remaining = initial;
            *position = (to + from) * 0.5f;
            *normal = map->GetGroundFaceNormalAt(position);
            ++hits;
        } else {
            from = next;
            remaining = from - to;
        }
    } while (steps < 100 && Dot(remaining, initial) > 0.0f && hits < 4);
    return hits > 0 ? true : false;
}
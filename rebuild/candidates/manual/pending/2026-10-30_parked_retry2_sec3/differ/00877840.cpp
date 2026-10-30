// CTools::IsLineOfSight(const std::vector<const CThing*>& ignore, const C3DVector& from, const C3DVector& to, C3DVector* hitPos, C3DVector* hitNormal, float* hitValue, CThing** hitThing, ELOSTest tests, bool flag) -- true when nothing blocks from..to: test things (tests & 9), the theme/world map (4 then 16) and the landscape (2), keep the hit nearest to from and write it out (position, normal, per-kind value, thing) when blocked
// Grade: DIFFER(1021v1023): 100 diff lines (was 169) -- the hit-thing out local is block-scoped around the thing test, so VC homes it in the dead ignore argument slot and the frame is retail's 0x4c; residue: the four vector locals sit at different frame offsets (declaration order has no effect, 24 orders tried)
// Leaf: IsLineOfSight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00877840, length 1023 bytes (`ret 0x24`).
// Callees: CTools::GetThingPointIntersectionFromTo 0x008771A0, GetThemePointIntersectionFromTo 0x008754A0 (flag true for
//   test 4, false for test 16), GetLandscapePointIntersectionFromTo 0x00875550. Global: the epsilon 0x0129BA3C (0.0001f variable).
// Layout: CTools blocking values: thing +0x10, thing with flag 8 +0x14, landscape +0x18, theme +0x1c; CThing flags byte +0x6c.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
extern float g_fIntersectionEpsilon;                        // 0x0129BA3C (0.0001f)
struct CThing {
    unsigned char _pad00[0x6c];
    unsigned char Flags;                                    // +0x6c
};
namespace std {
template <class T> struct allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
enum ELOSTest {
    LOS_TEST_THINGS = 1, LOS_TEST_LANDSCAPE = 2, LOS_TEST_THEME = 4, LOS_TEST_THING_FLAG = 8, LOS_TEST_THEME_2 = 0x10
};
struct CTools {
    void*         Owner;                                    // +0x00
    unsigned char _pad04[0x10 - 0x04];
    float         ThingBlockValue;                          // +0x10
    float         FlaggedThingBlockValue;                   // +0x14
    float         LandscapeBlockValue;                      // +0x18
    float         ThemeBlockValue;                          // +0x1c
    bool GetThingPointIntersectionFromTo(const std::vector<const CThing*>& ignore, const C3DVector& from, const C3DVector& to,
                                         C3DVector& hitPos, C3DVector& hitNormal, CThing*& hitThing, bool flag, ELOSTest tests);   // 0x008771A0
    bool GetThemePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool flag);   // 0x008754A0
    bool GetLandscapePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool includeWater);   // 0x00875550
    bool IsLineOfSight(const std::vector<const CThing*>& ignore, const C3DVector& from, const C3DVector& to, C3DVector* hitPos,
                       C3DVector* hitNormal, float* hitValue, CThing** hitThing, ELOSTest tests, bool flag);
};

bool CTools::IsLineOfSight(const std::vector<const CThing*>& ignore, const C3DVector& from, const C3DVector& to, C3DVector* hitPos,
                           C3DVector* hitNormal, float* hitValue, CThing** hitThing, ELOSTest tests, bool flag)
{
    if (!((from - to).GetLengthSquared() > g_fIntersectionEpsilon * g_fIntersectionEpsilon))
        return true;
    bool hit = false;
    float bestValue = 0.0f;
    CThing* bestThing = 0;
    C3DVector bestPos;
    C3DVector bestNormal;
    C3DVector pos;
    C3DVector normal;
    {
        CThing* thing;
        if ((tests & 9) && GetThingPointIntersectionFromTo(ignore, from, to, pos, normal, thing, flag, tests)) {
            float value = ThingBlockValue;
            if ((thing->Flags & 8) && (tests & 8))
                value = FlaggedThingBlockValue;
            bestValue = value;
            bestPos = pos;
            bestNormal = normal;
            bestThing = thing;
            hit = true;
        }
    }
    if ((tests & 4) && GetThemePointIntersectionFromTo(from, to, pos, normal, true)) {
        float value = ThemeBlockValue;
        if (!hit || (pos - from).GetLengthSquared() < (bestPos - from).GetLengthSquared()) {
            bestPos = pos;
            bestNormal = normal;
            bestValue = value;
        }
        hit = true;
    }
    if ((tests & 0x10) && GetThemePointIntersectionFromTo(from, to, pos, normal, false)) {
        float value = ThemeBlockValue;
        if (!hit || (pos - from).GetLengthSquared() < (bestPos - from).GetLengthSquared()) {
            bestPos = pos;
            bestNormal = normal;
            bestValue = value;
        }
        hit = true;
    }
    if ((tests & 2) && GetLandscapePointIntersectionFromTo(from, to, pos, normal, flag)) {
        float value = LandscapeBlockValue;
        if (!hit || (pos - from).GetLengthSquared() < (bestPos - from).GetLengthSquared()) {
            bestPos = pos;
            bestNormal = normal;
            bestValue = value;
        }
        hit = true;
    }
    if (!hit)
        return true;
    *hitPos = bestPos;
    *hitNormal = bestNormal;
    *hitValue = bestValue;
    *hitThing = bestThing;
    return false;
}

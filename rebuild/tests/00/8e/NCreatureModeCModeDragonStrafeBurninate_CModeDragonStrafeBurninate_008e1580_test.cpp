#include <cstdio>
#include <cstring>
static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
class CThing { public: int id; };
class CCharString { public:
    const char* p;
    CCharString() : p(0) {}
    CCharString(const char* t, long = -1) : p(t) {}
    const CCharString& operator=(const CCharString& o) { p = o.p; return *this; }
    unsigned long GetLength() const { return p ? (unsigned long)std::strlen(p) : 0; }
    const char* GetPString() const { return p ? p : ""; }
};
static unsigned long Calc(unsigned long seed, const void* d, unsigned long n)
{
    const unsigned char* c = (const unsigned char*)d;
    unsigned long h = seed;
    for (unsigned long i = 0; i < n; ++i) h = h * 33 + c[i];
    return h;
}
class CAnimFilterOfGroup { public:
    CCharString   Group;
    unsigned long GroupCRC;
    CAnimFilterOfGroup() : Group(), GroupCRC(0) {}
    CAnimFilterOfGroup(const CCharString& g) : Group() { Group = g; GroupCRC = Calc(0, g.GetPString(), g.GetLength()); }
    void SetGroup(const CCharString& g) { Group = g; GroupCRC = Calc(0, g.GetPString(), g.GetLength()); }
};
static bool FilterIs(const CAnimFilterOfGroup& f, const char* s)
{
    return f.Group.p != 0 && std::strcmp(f.Group.p, s) == 0 && f.GroupCRC == Calc(0, s, (unsigned long)std::strlen(s));
}
struct CIP {
    CThing* PItem;
    int     built;
    CIP() : PItem(0), built(1) {}
    CIP(CThing* t) : PItem(0), built(1) { PItem = t; }
};
template <class T> struct CVector { T* a; T* b; T* c; CVector() : a(0), b(0), c(0) {} };
template <class T> class CDefPointer { public: T* Object; CDefPointer() : Object(0) {} };
class C3DVector { public: float X, Y, Z; };
struct CModeStandBase {
    CThing* ParentThing;
    CModeStandBase(CThing* t) : ParentThing(t) { Log(1); }
};
// behaviour copy of NCreatureMode::CModeDragonStrafeBurninate::CModeDragonStrafeBurninate(thing, path, destination, start%, stop%, startPos, endPos)
static float g_speed = 0.0f; static const void* g_path = 0;
struct CModeDragonFlying : CModeStandBase {
    CAnimFilterOfGroup Filter;
    CModeDragonFlying(CThing* thing, const void* path, const C3DVector&, float speed) : CModeStandBase(thing), Filter("FLY") { g_speed = speed; g_path = path; }
};
struct CModeDragonStrafeBurninate : CModeDragonFlying {
    long Timer; CIP PBeamWeaponEffect; long LastExplodeFrame; C3DVector FlameStartPos, FlameEndPos; bool FlameActive;
    float FlameStartPercentage, FlameStopPercentage;
    CModeDragonStrafeBurninate(CThing* thing, const void* path, const C3DVector& destination, float startPct, float stopPct,
                               const C3DVector& startPos, const C3DVector& endPos)
        : CModeDragonFlying(thing, path, destination, 30.0f)
    {
        FlameStartPos = startPos; FlameEndPos = endPos; FlameActive = false;
        FlameStartPercentage = startPct; FlameStopPercentage = stopPct;
        Filter.SetGroup("STRAFE_BURNINATE");
    }
};
int main()
{
    CThing t; int path = 0; C3DVector d = { 1, 2, 3 }, s = { 4, 5, 6 }, e = { 7, 8, 9 };
    CModeDragonStrafeBurninate m(&t, &path, d, 0.25f, 0.75f, s, e);
    if (g_n != 1 || g_speed != 30.0f || g_path != &path || !FilterIs(m.Filter, "STRAFE_BURNINATE") || m.PBeamWeaponEffect.PItem
        || m.FlameStartPos.Y != 5 || m.FlameEndPos.Z != 9 || m.FlameActive || m.FlameStartPercentage != 0.25f || m.FlameStopPercentage != 0.75f)
    { std::printf("008e1580_TEST FAIL\n"); return 1; }
    std::printf("008e1580_TEST PASS\n");
    return 0;
}
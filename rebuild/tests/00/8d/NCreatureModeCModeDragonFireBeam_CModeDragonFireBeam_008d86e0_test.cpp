#include <cstdio>
#include <cstring>
static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct CThing { int id; };
struct CCharString {
    const char* p;
    CCharString() : p(0) {}
    CCharString(const char* t, long = -1) : p(t) {}
    CCharString& operator=(const CCharString& o) { p = o.p; return *this; }
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
struct CAnimFilterOfGroup {
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
template <class T> struct CDefPointer { T* Object; CDefPointer() : Object(0) {} };
struct C3DVector { float X, Y, Z; };
struct CModeStandBase {
    CThing* ParentThing;
    CModeStandBase(CThing* t) : ParentThing(t) { Log(1); }
};
// behaviour copy of NCreatureMode::CModeDragonFireBeam::CModeDragonFireBeam(CThing*, CThing*, float)
static long g_framesPerSecond = 30;
struct CModeDragonHover : CModeStandBase {
    CAnimFilterOfGroup Filter; CIP PTarget; bool UseHeadTurning;
    CModeDragonHover(CThing* thing, CThing* target) : CModeStandBase(thing), Filter("HOVER"), PTarget(target), UseHeadTurning(false) { }
};
struct CModeDragonFireBeam : CModeDragonHover {
    long Timer; CIP PBeamWeaponEffect; long LastExplodeFrame; bool BeamActive;
    CModeDragonFireBeam(CThing* thing, CThing* target, float seconds) : CModeDragonHover(thing, target)
    {
        Filter = CAnimFilterOfGroup("FIRE_BEAM");
        Timer = (long)(seconds * g_framesPerSecond);
    }
};
int main()
{
    CThing t, u; CModeDragonFireBeam m(&t, &u, 2.5f);
    if (g_n != 1 || !FilterIs(m.Filter, "FIRE_BEAM") || m.Timer != 75 || m.PTarget.PItem != &u || m.PBeamWeaponEffect.PItem)
    { std::printf("008d86e0_TEST FAIL\n"); return 1; }
    std::printf("008d86e0_TEST PASS\n");
    return 0;
}
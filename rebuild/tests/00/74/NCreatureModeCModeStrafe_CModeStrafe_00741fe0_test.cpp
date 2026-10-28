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
// behaviour copy of NCreatureMode::CModeStrafe::CModeStrafe(CThing*, bool)
template <class T> class CCountedPointer { public: T* Object; void* Count; CCountedPointer() : Object(0), Count(0) {} };
struct CModeStrafe : CModeStandBase {
    float AnimMovementDistance; CCharString StrafeAnimName; long AnimIndex; bool LastUsable; CAnimFilterOfGroup Filter;
    long ActiveWeapon, AimingNumFrames, AimingCenterFrame; float RumbleParam, PullBackAmount; bool ForceAnimNoBlendIn;
    CCountedPointer<int> PAnimProjectileAim, PAnimProjectileBuildUp, PAnimProjectilePullBack, PAnimStrafeUpperBody;
    CModeStrafe(CThing* thing, bool forceAnimNoBlendIn)
        : CModeStandBase(thing), AnimMovementDistance(0.0f), StrafeAnimName(""), LastUsable(true), Filter(""),
          ActiveWeapon(-1), AimingNumFrames(1), ForceAnimNoBlendIn(forceAnimNoBlendIn) { }
};
int main()
{
    CThing t; CModeStrafe m(&t, true); CModeStrafe n(&t, false);
    if (g_n != 2 || m.AnimMovementDistance != 0.0f || std::strcmp(m.StrafeAnimName.GetPString(), "") != 0 || !m.LastUsable
        || !FilterIs(m.Filter, "") || m.ActiveWeapon != -1 || m.AimingNumFrames != 1 || !m.ForceAnimNoBlendIn || n.ForceAnimNoBlendIn
        || m.PAnimProjectileAim.Object || m.PAnimProjectileBuildUp.Count || m.PAnimProjectilePullBack.Object || m.PAnimStrafeUpperBody.Count)
    { std::printf("00741fe0_TEST FAIL\n"); return 1; }
    std::printf("00741fe0_TEST PASS\n");
    return 0;
}
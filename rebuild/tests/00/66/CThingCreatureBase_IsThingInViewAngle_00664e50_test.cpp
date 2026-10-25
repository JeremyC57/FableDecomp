// Behaviour test for 0x00664E50: CThingCreatureBase::IsThingInViewAngle(const CThing& thing, float viewAngle, bool useLookAngle) const -- true when a living thing lies within half of viewAngle (in turns) of this creature's XY facing, or of its head-look direction when useLookAngle and it has a CTCLook
// A copy of the source model and logic (compiled separately from src/00664e50.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
#pragma intrinsic(fabs, fmod, atan2)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

__forceinline float NormaliseAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
__forceinline float GetAngleXY(float x, float y) { return NormaliseAngle((float)atan2((double)x, (double)y) * 0.15915494f); }
__forceinline float AngleDifference(float a, float b)
{
    float difference = (float)fabs((double)(NormaliseAngle(a) - NormaliseAngle(b)));
    if (difference > 0.5f)
        difference = 1.0f - difference;
    return difference;
}

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing() const;               // slot 70 (+0x118)
};
class CTCLook {
public:
    float GetLookAngleRelToBodyXY() const;             // 0x00751A80
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                         // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                             // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;              // 0x004C73D0
    bool IsDead() const { return (Flags91 & 1) != 0; }
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (const T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CThingCreatureBase : public CThing {
public:
    bool IsThingInViewAngle(const CThing& thing, float viewAngle, bool useLookAngle) const;
};

bool CThingCreatureBase::IsThingInViewAngle(const CThing& thing, float viewAngle, bool useLookAngle) const
{
    if (thing.IsDead())
        return false;
    C3DVector toThing = thing.GetPosition() - GetPosition();
    C3DVector facing = PhysicsTC->GetFacing();
    float facingAngle = GetAngleXY(facing.X, facing.Y);
    float lookAngle = facingAngle;
    if (HasTC(0x43)) {
        const CTCLook* look = GetTC<CTCLook>(0x43);
        if (useLookAngle)
            lookAngle = NormaliseAngle(look->GetLookAngleRelToBodyXY() + facingAngle);
    }
    float thingAngle = GetAngleXY(toThing.X, toThing.Y);
    if (AngleDifference(thingAngle, lookAngle) <= viewAngle * 0.5f)
        return true;
    return false;
}

// ---- test stand-ins ----
#define DEFV(n) void CTCPhysicsBase::v##n() { }
DEFV(00) DEFV(01) DEFV(02) DEFV(03) DEFV(04) DEFV(05) DEFV(06) DEFV(07) DEFV(08) DEFV(09)
DEFV(10) DEFV(11) DEFV(12) DEFV(13) DEFV(14) DEFV(15) DEFV(16) DEFV(17) DEFV(18) DEFV(19)
DEFV(20) DEFV(21) DEFV(22) DEFV(23) DEFV(24) DEFV(25) DEFV(26) DEFV(27) DEFV(28) DEFV(29)
DEFV(30) DEFV(31) DEFV(32) DEFV(33) DEFV(34) DEFV(35) DEFV(36) DEFV(37) DEFV(38) DEFV(39)
DEFV(40) DEFV(41) DEFV(42) DEFV(43) DEFV(44) DEFV(45) DEFV(46) DEFV(47) DEFV(48) DEFV(49)
DEFV(50) DEFV(51) DEFV(52) DEFV(53) DEFV(54) DEFV(55) DEFV(56) DEFV(57) DEFV(58) DEFV(59)
DEFV(60) DEFV(61) DEFV(62) DEFV(63) DEFV(64) DEFV(65) DEFV(66) DEFV(67) DEFV(68) DEFV(69)
static C3DVector Vec(float x, float y, float z) { return C3DVector(x, y, z); }
C3DVector CTCPhysicsBase::GetFacing() const { return Vec(0.0f, 1.0f, 0.0f); }
class TestPhysics : public CTCPhysicsBase {
public:
    C3DVector Facing;
    C3DVector Position;
    virtual C3DVector GetFacing() const { return Facing; }
};
const C3DVector& CThing::GetPosition() const { return static_cast<TestPhysics*>(PhysicsTC)->Position; }
static float g_lookAngle = 0.0f;
static int g_lookCalls = 0;
float CTCLook::GetLookAngleRelToBodyXY() const { ++g_lookCalls; return g_lookAngle; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}

static int Fail(int line) { std::printf("00664e50_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)

int main()
{
    CTCLook look;
    TestPhysics heroPhysics, otherPhysics;
    heroPhysics.Facing = Vec(0.0f, 1.0f, 0.0f);                 // facing +Y: angle 0 turns
    heroPhysics.Position = Vec(3.0f, -2.0f, 7.0f);
    static CThingCreatureBase hero;
    static CThing other;
    std::memset(hero.m_presentBits, 0, sizeof hero.m_presentBits); hero.Flags91 = 0;
    std::memset(other.m_presentBits, 0, sizeof other.m_presentBits); other.Flags91 = 0;
    hero.PhysicsTC = &heroPhysics; other.PhysicsTC = &otherPhysics;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x43, (CTCBase*)&look } };
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;

    otherPhysics.Position = Vec(3.0f, 8.0f, 0.0f);              // straight ahead
    CHECK(hero.IsThingInViewAngle(other, 0.01f, false));
    otherPhysics.Position = Vec(13.0f, -2.0f, 0.0f);            // a quarter turn to the right
    CHECK(hero.IsThingInViewAngle(other, 0.6f, false) && !hero.IsThingInViewAngle(other, 0.4f, false));
    otherPhysics.Position = Vec(-7.0f, -2.0f, 0.0f);            // a quarter turn to the left (0.75 wraps to 0.25)
    CHECK(hero.IsThingInViewAngle(other, 0.6f, true) && !hero.IsThingInViewAngle(other, 0.4f, true));
    CHECK(hero.IsThingInViewAngle(other, 0.5f, false));         // exactly on the edge counts
    otherPhysics.Position = Vec(3.0f, -12.0f, 0.0f);            // behind
    CHECK(!hero.IsThingInViewAngle(other, 0.9f, false) && hero.IsThingInViewAngle(other, 1.0f, false));
    CHECK(g_lookCalls == 0);
    hero.m_presentBits[0x43 >> 5] |= 1UL << (0x43 & 31);         // the hero has a CTCLook turned half a turn
    g_lookAngle = 0.5f;
    CHECK(hero.IsThingInViewAngle(other, 0.02f, true) && g_lookCalls == 1);
    CHECK(!hero.IsThingInViewAngle(other, 0.9f, false) && g_lookCalls == 1);   // the look angle is ignored without useLookAngle
    g_lookAngle = 1.25f;                                         // look + facing wraps: 1.25 -> 0.25
    otherPhysics.Position = Vec(13.0f, -2.0f, 0.0f);
    CHECK(hero.IsThingInViewAngle(other, 0.02f, true));
    other.Flags91 = 1;                                           // dead things are never in view
    CHECK(!hero.IsThingInViewAngle(other, 1.0f, true));
    heroPhysics.Facing = Vec(-1.0f, 0.0f, 0.0f);                 // facing -X: -0.25 turns normalises to 0.75
    other.Flags91 = 0;
    otherPhysics.Position = Vec(-20.0f, -1.0f, 0.0f);
    CHECK(hero.IsThingInViewAngle(other, 0.1f, false));            // ~0.757 turns: inside
    CHECK(!hero.IsThingInViewAngle(other, 0.1f, true));            // the look (1.25 + 0.75 -> 0) points elsewhere
    std::printf("00664e50_TEST PASS\n");
    return 0;
}
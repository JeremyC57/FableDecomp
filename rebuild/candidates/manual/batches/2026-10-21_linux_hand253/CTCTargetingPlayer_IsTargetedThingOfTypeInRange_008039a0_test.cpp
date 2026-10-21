#include <cstdio>
// Behaviour test for CTCTargetingPlayer::vfunc_38 (0x008039A0): the body of src/ over stand-ins -- false with no target or
// a type the target rejects, true under free targeting (except type 2), otherwise the squared-distance range test.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CTargetingDef {
    long m_count;
    int destroyed;
    void Destroy() { ++destroyed; }
    void Release() { if (--m_count == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
};
static CTargetingDef g_tdef = { 1, 0 };
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTargetingDef>* out) { out->Object = &g_tdef; ++g_tdef.m_count; return true; }
};
struct CTCTargeted { unsigned long TargetingFlags; };
class CThing;
struct CTCCarrying {
    CThing* weapon;
    CThing* GetThingInPrimarySlot() { return weapon; }
};
class CThing {
public:
    CDefClassBase* PDef;
    C3DVector pos;
    CTCTargeted* targeted;
    CTCCarrying* carrying;
    bool GetInterface(const CTCTargeted** ppOut, long type) const { *ppOut = type == 9 ? targeted : 0; return true; }
    bool GetInterface(CTCCarrying** ppOut, long type) const { *ppOut = type == 0x46 ? carrying : 0; return true; }
    const C3DVector& GetPosition() const { return pos; }
};
static unsigned long g_rangeType; static const CTargetingDef* g_rangeDef; static CThing* g_rangeWeapon;
static float GetTargetingRangeForTargetType(unsigned long type, const CTargetingDef* def, CThing* owner, CThing* weapon)
{
    g_rangeType = type; g_rangeDef = def; g_rangeWeapon = weapon;
    return 5.0f;
}
struct CGameSettings { bool FreeTargeting; };
static CGameSettings g_settings = { false };
struct CWorld { CGameSettings* GetSettings() const { return &g_settings; } };
static CWorld g_world;
struct CTCTargetingPlayer {
    CThing* target;
    CThing* GetPTargetedThing() const { return target; }
    CThing* ParentThing;
    const CWorld* GetWorld() const { return &g_world; }
    bool vfunc_38(unsigned long typeMask);
};

bool CTCTargetingPlayer::vfunc_38(unsigned long typeMask)
{
    if (!GetPTargetedThing())
        return false;
    const CTCTargeted* targeted;
    GetPTargetedThing()->GetInterface(&targeted, 9);
    if (!(targeted->TargetingFlags & typeMask))
        return false;
    if (typeMask != 2 && GetWorld()->GetSettings()->FreeTargeting)
        return true;
    CTCCarrying* carrying;
    CDefPointer<CTargetingDef> def;
    ParentThing->PDef->GetSubDef(&def);
    ParentThing->GetInterface(&carrying, 0x46);
    float range = GetTargetingRangeForTargetType(typeMask, def.Object, ParentThing, carrying->GetThingInPrimarySlot());
    CThing* owner = ParentThing;
    const C3DVector& targetPos = GetPTargetedThing()->GetPosition();
    const C3DVector& ownerPos = owner->GetPosition();
    C3DVector offset(ownerPos.X - targetPos.X, ownerPos.Y - targetPos.Y, ownerPos.Z - targetPos.Z);
    bool inRange = offset.Y * offset.Y + offset.X * offset.X + offset.Z * offset.Z <= range * range;
    return inRange;
}

static int fail(int line) { printf("008039a0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CDefClassBase pdef;
    CThing weapon;
    CTCCarrying carrying = { &weapon };
    CTCTargeted targeted = { 6 };
    CThing owner; owner.PDef = &pdef; owner.pos = C3DVector(0, 0, 0); owner.carrying = &carrying;
    CThing target; target.pos = C3DVector(3, 4, 0); target.targeted = &targeted;
    CTCTargetingPlayer tp; tp.ParentThing = &owner; tp.target = 0;
    CHECK(!tp.vfunc_38(2));
    tp.target = &target;
    CHECK(!tp.vfunc_38(1));                         // type rejected by the target
    CHECK(tp.vfunc_38(4));                          // distance 5 <= range 5
    CHECK(g_rangeType == 4 && g_rangeDef == &g_tdef && g_rangeWeapon == &weapon && g_tdef.m_count == 1);
    target.pos = C3DVector(3, 4, 1);
    CHECK(!tp.vfunc_38(4));                         // out of range
    g_settings.FreeTargeting = true;
    CHECK(tp.vfunc_38(4));                          // free targeting
    CHECK(!tp.vfunc_38(2));                         // type 2 always range-checked
    CHECK(g_tdef.m_count == 1 && g_tdef.destroyed == 0);
    printf("008039a0_TEST PASS\n");
    return 0;
}

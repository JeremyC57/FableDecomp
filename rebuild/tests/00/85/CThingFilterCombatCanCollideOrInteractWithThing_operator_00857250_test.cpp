#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

class CThing;
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4)
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56();
    virtual bool GetCollidesWithThings() const;         // slot 57 (+0xe4)
    virtual bool GetCollidesWithThingsOneWay() const;   // slot 58 (+0xe8)
    virtual void v59();
    virtual bool GetInteractsWithThings() const;        // slot 60 (+0xf0)
    CThing* ParentThing;                        // +0x04 (CTCBase)
};

struct CTCSolidToThing {
    bool IsSolidToThing(const CThing* thing) const;     // 0x00770DE0
};
struct CTCMeleeHittable {                       // interface type 0x77 (class name not pinned)
    bool CanBeHitByMeleeStrike() const;         // 0x007BEFA0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;               // +0x6c
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

struct CThingFilter_CombatCanCollideOrInteractWithThing {
    CTCPhysicsBase* TCPhysics;                  // +0x00
    bool ExcludeCreatures;                      // +0x04
    bool ExcludeDeadCreatures;                  // +0x05
    bool operator()(CThing* thing) const;
};

bool CThingFilter_CombatCanCollideOrInteractWithThing::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (ExcludeCreatures && (thing->ThingTypeFlags & 8))
        return false;
    if (ExcludeDeadCreatures && (thing->ThingTypeFlags & 0x80))
        return false;
    if (TCPhysics->ParentThing->HasTC(0x4d)) {
        if (!TCPhysics->ParentThing->GetTC<CTCSolidToThing>(0x4d)->IsSolidToThing(thing))
            return false;
    }
    if (thing->HasTC(0x4d)) {
        CTCSolidToThing* solid = thing->GetTC<CTCSolidToThing>(0x4d);
        CThing* parent = TCPhysics->ParentThing;
        if (!solid->IsSolidToThing(parent))
            return false;
    }
    if (physics && physics != TCPhysics) {
        if (physics->GetCollidesWithThings() || physics->GetInteractsWithThings())
            return true;
    }
    if (thing->ThingTypeFlags & 0x80)
        return true;
    if (thing->HasTC(0x77) && thing->GetTC<CTCMeleeHittable>(0x77)->CanBeHitByMeleeStrike())
        return true;
    return false;
}// ---- test doubles ----
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static bool g_collides = false, g_interacts = false;
struct CTestPhysics : CTCPhysicsBase {
    bool collides, interacts;
    virtual bool GetCollidesWithThings() const { return collides; }
    virtual bool GetInteractsWithThings() const { return interacts; }
};
#define DEFV(n) void CTCPhysicsBase::v##n##0() {} void CTCPhysicsBase::v##n##1() {} void CTCPhysicsBase::v##n##2() {} \
  void CTCPhysicsBase::v##n##3() {} void CTCPhysicsBase::v##n##4() {} void CTCPhysicsBase::v##n##5() {} \
  void CTCPhysicsBase::v##n##6() {} void CTCPhysicsBase::v##n##7() {} void CTCPhysicsBase::v##n##8() {} void CTCPhysicsBase::v##n##9() {}
DEFV(1) DEFV(2) DEFV(3) DEFV(4)
void CTCPhysicsBase::v00() {} void CTCPhysicsBase::v01() {} void CTCPhysicsBase::v02() {} void CTCPhysicsBase::v03() {} void CTCPhysicsBase::v04() {}
void CTCPhysicsBase::v05() {} void CTCPhysicsBase::v06() {} void CTCPhysicsBase::v07() {} void CTCPhysicsBase::v08() {} void CTCPhysicsBase::v09() {}
void CTCPhysicsBase::v50() {} void CTCPhysicsBase::v51() {} void CTCPhysicsBase::v52() {} void CTCPhysicsBase::v53() {} void CTCPhysicsBase::v54() {}
void CTCPhysicsBase::v55() {} void CTCPhysicsBase::v56() {} void CTCPhysicsBase::v59() {}
bool CTCPhysicsBase::GetCollidesWithThings() const { return false; }
bool CTCPhysicsBase::GetCollidesWithThingsOneWay() const { return false; }
bool CTCPhysicsBase::GetInteractsWithThings() const { return false; }
static int g_solidCalls = 0; static const CThing* g_solidArg = 0; static bool g_mySolid = true, g_otherSolid = true;
static CTCSolidToThing g_solids[2];
bool CTCSolidToThing::IsSolidToThing(const CThing* t) const { ++g_solidCalls; g_solidArg = t; return this == &g_solids[0] ? g_mySolid : g_otherSolid; }
static bool g_meleeHit = false;
bool CTCMeleeHittable::CanBeHitByMeleeStrike() const { return g_meleeHit; }
int main()
{
    CThing me, other; std::memset(&me, 0, sizeof(me)); std::memset(&other, 0, sizeof(other));
    CTestPhysics myPhys, otherPhys; myPhys.ParentThing = &me; otherPhys.ParentThing = &other;
    myPhys.collides = myPhys.interacts = false; otherPhys.collides = true; otherPhys.interacts = false;
    me.PhysicsTC = &myPhys; other.PhysicsTC = &otherPhys;
    CTCMeleeHittable hittable;
    CInterfaceMap::SEntry e1[2] = { { 0x10, (CTCBase*)0 }, { 0x4d, (CTCBase*)&g_solids[0] } };
    CInterfaceMap::SEntry e2[3] = { { 0x10, (CTCBase*)0 }, { 0x4d, (CTCBase*)&g_solids[1] }, { 0x77, (CTCBase*)&hittable } };
    me.Components.first = e1; me.Components.last = e1 + 2; other.Components.first = e2; other.Components.last = e2 + 3;
    CThingFilter_CombatCanCollideOrInteractWithThing f; f.TCPhysics = &myPhys; f.ExcludeCreatures = false; f.ExcludeDeadCreatures = false;
    bool ok = f(&other) && g_solidCalls == 0;
    other.ThingTypeFlags = 8; f.ExcludeCreatures = true; ok = ok && !f(&other); f.ExcludeCreatures = false; ok = ok && f(&other);
    other.ThingTypeFlags = 0x80; f.ExcludeDeadCreatures = true; ok = ok && !f(&other); f.ExcludeDeadCreatures = false;
    otherPhys.collides = false; ok = ok && f(&other);                 // dead creature passes even without collision
    other.ThingTypeFlags = 0; ok = ok && !f(&other);
    other.UsingComponents[0x77 >> 5] = 1UL << (0x77 & 31); ok = ok && !f(&other);
    g_meleeHit = true; ok = ok && f(&other); g_meleeHit = false;      // melee-hittable passes
    otherPhys.interacts = true; ok = ok && f(&other); otherPhys.interacts = false; otherPhys.collides = true;
    me.UsingComponents[0x4d >> 5] = 1UL << (0x4d & 31); g_mySolid = false;
    ok = ok && !f(&other) && g_solidArg == &other; g_mySolid = true; ok = ok && f(&other);
    other.UsingComponents[0x4d >> 5] |= 1UL << (0x4d & 31); g_otherSolid = false;
    ok = ok && !f(&other) && g_solidArg == &me; g_otherSolid = true; ok = ok && f(&other);
    ok = ok && !f(&me);                                              // own physics component
    if (!ok) { std::printf("00857250_TEST FAIL\n"); return 1; }
    std::printf("00857250_TEST PASS\n");
    return 0;
}
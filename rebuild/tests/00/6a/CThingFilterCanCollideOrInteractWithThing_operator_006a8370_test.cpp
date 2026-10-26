// Behaviour test for 006a8370: body copied from src/006a8370.cpp (CThingFilter_CanCollideOrInteractWithThing::operator()), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing;
class CTCPhysicsBase { public:
    bool collides, interacts;
    virtual bool GetCollidesWithThings() const { return collides; }
    virtual bool GetCollidesWithThingsOneWay() const { return false; }
    virtual bool GetInteractsWithThings() const { return interacts; }
    CThing* ParentThing;
};
static int g_solidCalls = 0; static const CThing* g_solidArg = 0;
class CTCSolidToThing { public:
    bool solid;
    bool IsSolidToThing(const CThing* thing) const { ++g_solidCalls; g_solidArg = thing; return solid; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;
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
class CThingFilter_CanCollideOrInteractWithThing { public:
    CTCPhysicsBase* TCPhysics;
    bool ExcludeCreatures;
    bool operator()(CThing* thing) const;
};
bool CThingFilter_CanCollideOrInteractWithThing::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (ExcludeCreatures && (thing->ThingTypeFlags & 8))
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
    return false;
}
int main()
{
    CThing me, other; memset(&me, 0, sizeof(me)); memset(&other, 0, sizeof(other));
    CTCPhysicsBase myPhys, otherPhys; myPhys.ParentThing = &me; otherPhys.ParentThing = &other; myPhys.collides = myPhys.interacts = false; otherPhys.collides = true; otherPhys.interacts = false;
    me.PhysicsTC = &myPhys; other.PhysicsTC = &otherPhys;
    CTCSolidToThing mySolid, otherSolid; mySolid.solid = true; otherSolid.solid = true;
    CInterfaceMap::SEntry e1[2] = { { 0x10, (CTCBase*)0x99 }, { 0x4d, (CTCBase*)&mySolid } };
    CInterfaceMap::SEntry e2[2] = { { 0x10, (CTCBase*)0x99 }, { 0x4d, (CTCBase*)&otherSolid } };
    me.Components.first = e1; me.Components.last = e1 + 2; other.Components.first = e2; other.Components.last = e2 + 2;
    CThingFilter_CanCollideOrInteractWithThing f; f.TCPhysics = &myPhys; f.ExcludeCreatures = false;
    // 1. no solid components: other's physics collides -> true
    CHECK(f(&other) == true); CHECK(g_solidCalls == 0);
    // 2. creature excluded
    other.ThingTypeFlags = 8; f.ExcludeCreatures = true; CHECK(f(&other) == false); f.ExcludeCreatures = false; CHECK(f(&other) == true); other.ThingTypeFlags = 0;
    // 3. my solid component rejects other
    me.UsingComponents[0x4d >> 5] = 1UL << (0x4d & 31); mySolid.solid = false; CHECK(f(&other) == false); CHECK(g_solidCalls == 1); CHECK(g_solidArg == &other);
    mySolid.solid = true; CHECK(f(&other) == true); CHECK(g_solidCalls == 2);
    // 4. other's solid component rejects me
    other.UsingComponents[0x4d >> 5] = 1UL << (0x4d & 31); otherSolid.solid = false; CHECK(f(&other) == false); CHECK(g_solidCalls == 4); CHECK(g_solidArg == &me);
    otherSolid.solid = true; CHECK(f(&other) == true);
    // 5. physics: same as mine -> false; null -> false; neither collides nor interacts -> false; interacts only -> true
    CHECK(f(&me) == false);
    other.PhysicsTC = 0; CHECK(f(&other) == false); other.PhysicsTC = &otherPhys;
    otherPhys.collides = false; CHECK(f(&other) == false);
    otherPhys.interacts = true; CHECK(f(&other) == true);
    if (fails == 0) printf("006a8370_TEST PASS\n"); else printf("006a8370_TEST FAIL\n");
    return fails;
}
// Behaviour test for 0x007C2800 CFilter_Bind3<IsNot, ShotsToCheckForCollisionWith, SolidToThing>::operator(): a thing
// passes only when it is not the excluded thing, does not evade, its physics interacts or collides, it is a
// creature/object (flags 0xC) or has a type-0x69 driver, and it is not the source thing.
#include <cstdio>

class CTCBase {
public:
    long type;
    virtual long GetInterfaceType() const { return type; }
};
class CTCPhysicsBase {
public:
    bool collides, interacts;
    virtual bool GetCollidesWithThings() const { return collides; }
    virtual bool GetInteractsWithThings() const { return interacts; }
};
class CThing {
public:
    CTCBase*        PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    unsigned int    ThingTypeFlags;
    bool            evades;
};
namespace NCreatureMode {
    bool IsToEvadeAllHits(const CThing& thing) { return thing.evades; }
}
class CThingFilter_IsNot {
public:
    CThing* Thing;
    bool operator()(const CThing& thing) const { return &thing != Thing; }
};
class CThingFilter_SolidToThing {
public:
    CThing* SourceThing;
    void*   PTCSolidToThing;
    bool operator()(const CThing& thing) const { return &thing != SourceThing; }
};
class CThingFilter_ShotsToCheckForCollisionWith {
public:
    bool operator()(const CThing& thing) const
    {
        unsigned int flags = thing.ThingTypeFlags;
        CTCPhysicsBase* physics = thing.PhysicsTC;
        if (NCreatureMode::IsToEvadeAllHits(thing))
            return false;
        if (!physics)
            return false;
        if (!physics->GetInteractsWithThings() && !physics->GetCollidesWithThings())
            return false;
        if (flags & 0xC)
            return true;
        CTCBase* driver = thing.PTCDriver;
        return driver && driver->GetInterfaceType() == 0x69;
    }
};
template <class A, class B, class C> class CFilter_Bind3 {
public:
    const A* F1;
    const B* F2;
    const C* F3;
    template <class T> bool operator()(const T& thing) const
    {
        return (*F1)(thing) && (*F2)(thing) && (*F3)(thing);
    }
};

int main()
{
    CTCPhysicsBase phys; phys.collides = true; phys.interacts = false;
    CTCPhysicsBase dead; dead.collides = false; dead.interacts = false;
    CTCBase shotDriver; shotDriver.type = 0x69;
    CTCBase otherDriver; otherDriver.type = 0x10;
    CThing excluded, source, creature, shot, plain, evader, ghost;
    CThing* all[7] = { &excluded, &source, &creature, &shot, &plain, &evader, &ghost };
    for (int i = 0; i < 7; ++i) { all[i]->PTCDriver = 0; all[i]->PhysicsTC = &phys; all[i]->ThingTypeFlags = 8; all[i]->evades = false; }
    shot.ThingTypeFlags = 0x10; shot.PTCDriver = &shotDriver;
    plain.ThingTypeFlags = 0x10; plain.PTCDriver = &otherDriver;
    evader.evades = true;
    ghost.PhysicsTC = &dead;
    CThingFilter_IsNot isNot; isNot.Thing = &excluded;
    CThingFilter_ShotsToCheckForCollisionWith shots;
    CThingFilter_SolidToThing solid; solid.SourceThing = &source; solid.PTCSolidToThing = 0;
    CFilter_Bind3<CThingFilter_IsNot, CThingFilter_ShotsToCheckForCollisionWith, CThingFilter_SolidToThing> f;
    f.F1 = &isNot; f.F2 = &shots; f.F3 = &solid;
    bool ok = !f(excluded) && !f(source) && f(creature) && f(shot) && !f(plain) && !f(evader) && !f(ghost);
    phys.collides = false; phys.interacts = true;
    ok = ok && f(creature);
    creature.PhysicsTC = 0;
    ok = ok && !f(creature);
    if (!ok) { std::printf("007c2800_TEST FAIL\n"); return 1; }
    std::printf("007c2800_TEST PASS\n");
    return 0;
}
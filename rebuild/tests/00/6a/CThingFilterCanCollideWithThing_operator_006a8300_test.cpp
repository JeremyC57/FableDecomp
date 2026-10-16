// behaviour test for CThingFilter_CanCollideWithThing::operator() (0x006A8300)
#include <cstdio>
struct CThing;
struct CTCCollisionExclusion { CThing* excluded; bool CanCollideWith(CThing* t) { return t != excluded; } };
struct CTCPhysicsBase { bool collides; CThing* ParentThing; bool GetCollidesWithThings() { return collides; } };
struct CThing { CTCPhysicsBase* PhysicsTC; bool hasExcl; CTCCollisionExclusion* excl; };
struct CThingFilter_CanCollideWithThing {
    CTCPhysicsBase* TCPhysics;
    bool operator()(CThing* thing) const
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics && physics != TCPhysics && physics->GetCollidesWithThings()) {
            CThing* owner = TCPhysics->ParentThing;
            if (owner->hasExcl) return owner->excl->CanCollideWith(thing);
            return true;
        }
        return false;
    }
};
int main()
{
    CThing owner; CTCPhysicsBase mine; mine.collides = true; mine.ParentThing = &owner; owner.PhysicsTC = &mine; owner.hasExcl = false;
    CThing other; CTCPhysicsBase op; op.collides = true; op.ParentThing = &other; other.PhysicsTC = &op;
    CThingFilter_CanCollideWithThing f; f.TCPhysics = &mine;
    int bad = !f(&other) || f(&owner);
    op.collides = false; if (f(&other)) bad = 1;
    op.collides = true; CTCCollisionExclusion ex; ex.excluded = &other; owner.hasExcl = true; owner.excl = &ex; if (f(&other)) bad = 1;
    ex.excluded = 0; if (!f(&other)) bad = 1;
    other.PhysicsTC = 0; if (f(&other)) bad = 1;
    if (bad) { std::printf("006a8300_TEST FAIL\n"); return 1; }
    std::printf("006a8300_TEST PASS\n");
    return 0;
}
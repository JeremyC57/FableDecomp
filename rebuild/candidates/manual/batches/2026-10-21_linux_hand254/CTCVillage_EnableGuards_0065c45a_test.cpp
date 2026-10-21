// Behaviour test for CTCVillage::EnableGuards (0x0065C45A): only live dummies with a dummy-villager component whose
// Guard flag is set are switched; disabling kills a live guard; enabling re-creates a dead/missing guard at the owner's
// position; then the crimes are cleared and every live villager with an opinion component decriminalises.
#include <cstdio>
#include <list>

struct C3DVector { float X, Y, Z; };
struct CThing {
    bool alive;
    bool killed;
    C3DVector pos;
    struct CTCDummyVillager* dummy;     // interface 0xD6 (0 = absent)
    int decriminalised;                 // interface 0x4F present when >= 0
    void Kill(bool) { killed = true; alive = false; }
};
static bool IS_THING_ALIVE(CThing* t) { return t && t->alive; }

struct CTCVillage;
struct CTCDummyVillager {
    bool enabled;
    bool Guard;
    CThing* creature;
    int created;
    C3DVector createdAt;
    void SetEnabled(bool e) { enabled = e; }
    void CreateVillager(CTCVillage*, const C3DVector& p, bool) { ++created; createdAt = p; }
};

struct CTCVillage {
    CThing* ParentThing;
    std::list<CThing*> VillageDummies, VillagePeople;
    int crimesCleared;
    void ClearCrimes() { ++crimesCleared; }
    void EnableGuards(bool enable);
};

void CTCVillage::EnableGuards(bool enable)
{
    for (std::list<CThing*>::iterator it = VillageDummies.begin(); it != VillageDummies.end(); ++it) {
        CThing* dummy = *it;
        if (IS_THING_ALIVE(dummy) && dummy->dummy) {
            CTCDummyVillager* villager = dummy->dummy;
            if (villager->Guard) {
                villager->SetEnabled(enable);
                if (!enable) {
                    if (IS_THING_ALIVE(villager->creature))
                        villager->creature->Kill(false);
                } else if (!IS_THING_ALIVE(villager->creature))
                    villager->CreateVillager(this, ParentThing->pos, false);
            }
        }
    }
    ClearCrimes();
    for (std::list<CThing*>::iterator p = VillagePeople.begin(); p != VillagePeople.end(); ++p)
        if (IS_THING_ALIVE(*p) && (*p)->decriminalised >= 0)
            ++(*p)->decriminalised;
}

int main()
{
    int ok = 1;
    CThing owner = { true, false, { 1, 2, 3 }, 0, -1 };
    CThing guard = { true, false, { 0, 0, 0 }, 0, -1 };
    CTCDummyVillager gd = { true, true, &guard, 0 };
    CTCDummyVillager vd = { true, false, 0, 0 };          // not a guard
    CThing d1 = { true, false, { 0, 0, 0 }, &gd, -1 }, d2 = { true, false, { 0, 0, 0 }, &vd, -1 };
    CThing dead = { false, false, { 0, 0, 0 }, &gd, -1 };
    CThing p1 = { true, false, { 0, 0, 0 }, 0, 0 }, p2 = { false, false, { 0, 0, 0 }, 0, 0 }, p3 = { true, false, { 0, 0, 0 }, 0, -1 };
    CTCVillage v; v.ParentThing = &owner; v.crimesCleared = 0;
    v.VillageDummies.push_back(&d1); v.VillageDummies.push_back(&d2); v.VillageDummies.push_back(&dead);
    v.VillagePeople.push_back(&p1); v.VillagePeople.push_back(&p2); v.VillagePeople.push_back(&p3);

    v.EnableGuards(false);
    ok &= !gd.enabled && vd.enabled && guard.killed && gd.created == 0;
    ok &= v.crimesCleared == 1 && p1.decriminalised == 1 && p2.decriminalised == 0 && p3.decriminalised == -1;
    v.EnableGuards(true);
    ok &= gd.enabled && gd.created == 1 && gd.createdAt.X == 1 && gd.createdAt.Z == 3;
    ok &= v.crimesCleared == 2 && p1.decriminalised == 2;
    std::printf(ok ? "0065c45a_TEST PASS\n" : "0065c45a_TEST FAIL\n");
    return ok ? 0 : 1;
}

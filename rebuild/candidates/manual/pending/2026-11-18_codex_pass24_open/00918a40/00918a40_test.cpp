// Standalone C++03 fixture for the recovered workplace gaze predicate.
#include <cstdio>

struct CTCShop {
    bool Open;
    int Visits;
    CTCShop(bool open) : Open(open), Visits(0) {}
    bool IsOpeningHours() { ++Visits; return Open; }
};

struct CTCShopKeeper {
    CTCShop* Shop;
    int Visits;
    CTCShopKeeper(CTCShop* shop) : Shop(shop), Visits(0) {}
    CTCShop* GetShopTC() { ++Visits; return Shop; }
};

struct Creature {
    CTCShopKeeper* Keeper;
    int Calls;
    Creature() : Keeper(0), Calls(0) {}
    bool GetTC(CTCShopKeeper** result) {
        ++Calls;
        *result = Keeper;
        return Keeper != 0;
    }
};

struct Brain {
    struct Creature* Creature;
};

struct CAIStateGroup_GazeAtWork {
    Brain* PBrain;
    bool OwnerPresent;
    bool FreeGaze;
    int GazeCalls;
    CAIStateGroup_GazeAtWork(Brain& brain) : PBrain(&brain), OwnerPresent(false), FreeGaze(true), GazeCalls(0) {}
    bool GetOwner() const { return OwnerPresent; }
    bool FindFreeGazePos() { ++GazeCalls; return FreeGaze; }
    bool IsRunnable();
};

// Recovered method body, with game dependencies supplied by the fixture.
bool CAIStateGroup_GazeAtWork::IsRunnable()
{
    if (GetOwner()) {
        CTCShopKeeper* ptc_shopkeeper;
        if (PBrain->Creature->GetTC(&ptc_shopkeeper)) {
            CTCShop* ptc_shop = ptc_shopkeeper->GetShopTC();
            if (!ptc_shop || !ptc_shop->IsOpeningHours())
                return false;
        }
        return FindFreeGazePos() != 0;
    }
    return false;
}

int main()
{
    CTCShop closed(false), open(true);
    CTCShopKeeper keeper(0);
    Creature creature;
    Brain brain = { &creature };
    CAIStateGroup_GazeAtWork gaze(brain);
    bool ok = !gaze.IsRunnable() && creature.Calls == 0 && gaze.GazeCalls == 0;
    gaze.OwnerPresent = true;
    ok = ok && gaze.IsRunnable() && creature.Calls == 1 && gaze.GazeCalls == 1;
    creature.Keeper = &keeper;
    ok = ok && !gaze.IsRunnable() && keeper.Visits == 1 && gaze.GazeCalls == 1;
    keeper.Shop = &closed;
    ok = ok && !gaze.IsRunnable() && closed.Visits == 1 && gaze.GazeCalls == 1;
    keeper.Shop = &open;
    gaze.FreeGaze = false;
    ok = ok && !gaze.IsRunnable() && open.Visits == 1 && gaze.GazeCalls == 2;
    gaze.FreeGaze = true;
    ok = ok && gaze.IsRunnable() && open.Visits == 2 && gaze.GazeCalls == 3;
    std::puts(ok ? "00918a40_TEST PASS" : "00918a40_TEST FAIL");
    return ok ? 0 : 1;
}

// Behaviour test for 0x009658B0 CAIStateGroup_RecoverStealableItem::Update: a standalone model of the recover sequence
// (action point, move with an unreachable fallback, face, pick up killing a carried weapon, carry back and drop).
#include <cstdio>

struct World {
    bool itemAlive, actionPointFree, carriedByOther, reachPoint, reachNear, look, pickUp, targetAlive, carrying, reachTarget;
    bool previousIsWeapon;
    // outputs
    bool terminated, gotPoint, killedPrevious, dropped, reachedDestination;
};

static void Update(World& w)
{
    if (!w.itemAlive) { w.terminated = true; return; }
    if (!w.actionPointFree) { w.terminated = true; return; }
    w.gotPoint = true;
    if (w.carriedByOther) { w.terminated = true; return; }
    int result = w.reachPoint ? 2 : 3;
    if (result == 3) result = w.reachNear ? 2 : 1;
    if (result != 2) { w.terminated = true; return; }
    if (!w.look) { w.terminated = true; return; }
    if (w.previousIsWeapon) w.killedPrevious = true;
    if (w.targetAlive && w.pickUp && w.carrying && w.reachTarget) { w.reachedDestination = true; w.dropped = true; }
    w.terminated = true;
}

int main()
{
    World a = World();
    a.itemAlive = true; a.actionPointFree = true; a.reachPoint = false; a.reachNear = true; a.look = true; a.pickUp = true;
    a.targetAlive = true; a.carrying = true; a.reachTarget = true; a.previousIsWeapon = true;
    Update(a);
    if (!a.gotPoint || !a.dropped || !a.reachedDestination || !a.killedPrevious || !a.terminated) { std::printf("009658b0_TEST FAIL\n"); return 1; }
    World b = World();
    b.itemAlive = true; b.actionPointFree = true; b.carriedByOther = true;
    Update(b);
    if (!b.terminated || b.dropped) { std::printf("009658b0_TEST FAIL\n"); return 1; }
    std::printf("009658b0_TEST PASS\n");
    return 0;
}
// behaviour test for 0x009647C0 CAIStateGroup_StealStealableItem::Update (early-terminate guards on the stealable item)
#include <cstdio>
class CThing { public: unsigned char Dead : 1; bool IsDead() const { return Dead; } };
struct Group {
    CThing* item; bool freePoint; void* owner; int terminated; bool gotPoint;
    void Terminate() { ++terminated; }
    // the leading guards of Update: a missing/dead item or no free action point terminates, else the point is claimed
    void UpdateStart()
    {
        if (!item || item->IsDead()) { Terminate(); return; }
        if (!freePoint) { Terminate(); return; }
        gotPoint = true;
        if (!item || item->IsDead()) { Terminate(); return; }
        if (owner != 0) { Terminate(); return; }
    }
};
int main()
{
    CThing ok_ = { 0 }, dead = { 1 }; int x;
    Group a = { &ok_, true, 0, 0, false }, b = { &dead, true, 0, 0, false }, c = { &ok_, false, 0, 0, false }, d = { &ok_, true, &x, 0, false }, e = { 0, true, 0, 0, false };
    a.UpdateStart(); b.UpdateStart(); c.UpdateStart(); d.UpdateStart(); e.UpdateStart();
    bool ok = a.terminated == 0 && a.gotPoint && b.terminated == 1 && !b.gotPoint && c.terminated == 1 && !c.gotPoint
        && d.terminated == 1 && d.gotPoint && e.terminated == 1;
    std::printf(ok ? "009647c0_TEST PASS\n" : "009647c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
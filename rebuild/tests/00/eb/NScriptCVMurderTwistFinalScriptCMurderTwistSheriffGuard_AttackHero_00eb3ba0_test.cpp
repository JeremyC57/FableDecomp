// Behaviour test for CMurderTwistSheriffGuard::AttackHero (0x00EB3BA0): a model of the guard turning on the hero
// (five interface calls on Me) and then releasing his own handle and his slot in the parent's guard handle vector.
#include <cstdio>
#include <vector>
struct Handle {
    void* data; int releases;
    Handle() : data(0), releases(0) {}
    bool IsValid() const { return data != 0; }
    void Release() { ++releases; data = 0; }
};
struct Game {
    int log[5]; int n; int hero;
    Game() : n(0), hero(77) {}
    int GetHero() { return hero; }
    void GiveThingBestEnemyTarget(int me, int target) { if (target == hero) log[n++] = 1 + me * 0; }
    void EntitySetAsKillable(int, bool a, bool b) { if (a && b) log[n++] = 2; }
    void EntitySetAsDamageable(int, bool a) { if (a) log[n++] = 3; }
    void SetIsPushableByHero(int, bool a) { if (a) log[n++] = 4; }
    void EntitySetAsUseMovementInActions(int, bool a) { if (a) log[n++] = 5; }
};
struct Parent { std::vector<Handle> seh_Guards; };
struct Guard {
    Game& G; int Me; Parent* ParentClass; Handle MyEntityHandle; unsigned long ID;
    Guard(Game& g, Parent* p, unsigned long id) : G(g), Me(5), ParentClass(p), ID(id) {}
    void AttackHero()
    {
        G.GiveThingBestEnemyTarget(Me, G.GetHero());
        G.EntitySetAsKillable(Me, true, true);
        G.EntitySetAsDamageable(Me, true);
        G.SetIsPushableByHero(Me, true);
        G.EntitySetAsUseMovementInActions(Me, true);
        if (MyEntityHandle.IsValid())
            MyEntityHandle.Release();
        if (ParentClass->seh_Guards[ID].IsValid())
            ParentClass->seh_Guards[ID].Release();
    }
};
int main()
{
    int dummy = 0;
    Game g; Parent p; p.seh_Guards.resize(3);
    p.seh_Guards[2].data = &dummy;
    Guard guard(g, &p, 2);
    guard.MyEntityHandle.data = &dummy;
    guard.AttackHero();
    if (g.n != 5 || g.log[0] != 1 || g.log[4] != 5 || guard.MyEntityHandle.releases != 1 || p.seh_Guards[2].releases != 1
        || p.seh_Guards[2].IsValid() || p.seh_Guards[1].releases != 0) { std::printf("00eb3ba0_TEST FAIL\n"); return 1; }
    Guard other(g, &p, 1);
    other.AttackHero();   // both handles invalid: no releases
    if (other.MyEntityHandle.releases != 0 || p.seh_Guards[1].releases != 0) { std::printf("00eb3ba0_TEST FAIL\n"); return 1; }
    std::printf("00eb3ba0_TEST PASS\n");
    return 0;
}
// behaviour test for 0x0076D460 CTCEnemy::IsAllyOf
#include <cstdio>
struct Faction { int group; bool IsAlliedWith(const Faction* o) const { return o->group == group; } };
struct Enemy;
struct Thing { Enemy* enemy; bool hostile; };
struct Enemy {
    Thing* ParentThing; Faction* PFaction;
    bool IsEnemyOf(const Thing& t) const { return t.hostile; }
    bool IsAllyOf(const Thing& t) const
    {
        if (&t == ParentThing) return true;
        if (IsEnemyOf(t)) return false;
        Faction* f = PFaction;
        if (f) { Faction* o = t.enemy->PFaction; if (o) return f->IsAlliedWith(o); }
        return false;
    }
};
int main()
{
    Faction a = { 1 }, b = { 2 }, c = { 1 };
    Thing me = { 0, false }, other = { 0, false };
    Enemy mine = { &me, &a }, theirs = { &other, &c };
    me.enemy = &mine; other.enemy = &theirs;
    bool ok = mine.IsAllyOf(me) && mine.IsAllyOf(other);
    theirs.PFaction = &b; ok = ok && !mine.IsAllyOf(other);
    theirs.PFaction = 0; ok = ok && !mine.IsAllyOf(other);
    theirs.PFaction = &c; other.hostile = true; ok = ok && !mine.IsAllyOf(other);
    other.hostile = false; mine.PFaction = 0; ok = ok && !mine.IsAllyOf(other);
    if (!ok) { std::printf("0076d460_TEST FAIL\n"); return 1; }
    std::printf("0076d460_TEST PASS\n");
    return 0;
}

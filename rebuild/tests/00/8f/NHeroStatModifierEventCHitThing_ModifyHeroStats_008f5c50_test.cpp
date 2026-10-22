// Behaviour test for 0x008F5C50 (CHitThing::ModifyHeroStats): stat experience and morality changes for armed and unarmed hits.
#include <cstdio>
struct Experience { int stat, amount, calls; Experience() : stat(-1), amount(0), calls(0) {} void AddTrainableStatExperience(int s, int a) { stat = s; amount = a; ++calls; } };
struct HeroStats { Experience* experience; int Morality; void SetMorality(int m) { Morality = m; } };
struct StatsDef { int MoralityForPunch, MoralityForWeaponHit; };
struct HitThing {
    StatsDef* StatsDefOfHitThing; bool Unarmed; int StatToIncrement; bool DoStatIncrement; float IncrementAmount;
    void ModifyHeroStats(HeroStats* s)
    {
        Experience* e = s->experience;
        if (DoStatIncrement) e->AddTrainableStatExperience(StatToIncrement, (int)IncrementAmount);
        if (Unarmed) s->SetMorality(StatsDefOfHitThing->MoralityForPunch + s->Morality);
        else s->SetMorality(StatsDefOfHitThing->MoralityForWeaponHit + s->Morality);
    }
};
int main()
{
    Experience e; HeroStats s = { &e, 10 }; StatsDef def = { -2, -5 };
    HitThing punch = { &def, true, 3, true, 2.75f }; punch.ModifyHeroStats(&s);
    int ok = e.calls == 1 && e.stat == 3 && e.amount == 2 && s.Morality == 8;
    HitThing slash = { &def, false, 1, false, 1.0f }; slash.ModifyHeroStats(&s);
    ok = ok && e.calls == 1 && s.Morality == 3;
    if (!ok) { std::printf("008f5c50_TEST FAIL\n"); return 1; }
    std::printf("008f5c50_TEST PASS\n");
    return 0;
}
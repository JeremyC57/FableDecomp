// Behaviour test for CTCIdleScheduler::RegisterIdleStateGroup (0x00801920): the group is stored under its type; its weight is a
// random value below the default period, replaced by one below the def's period for the type when the thing has a def with an
// idle-scheduler sub def listing that type.
#include <cstdio>
#include <map>
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Random(unsigned long range, unsigned long* seed)
{
    *seed *= 0x24a1; *seed += 0x24df; *seed = Ror13(*seed);
    if (range) return *seed % range;
    return 0;
}
struct IdleDef { std::map<long, long> Period; };
struct Scheduler {
    std::map<long, int*> IdleStates; std::map<long, long> IdleStatesWeight;
    unsigned short DefIndex; const IdleDef* SubDef; unsigned long Seed; unsigned long DefaultPeriod;
    void RegisterIdleStateGroup(int* group, long type)
    {
        IdleStates[type] = group;
        IdleStatesWeight[type] = Random(DefaultPeriod, &Seed);
        if (DefIndex != 0) {
            if (SubDef) {
                const std::map<long, long>& period = SubDef->Period;
                if (period.find(type) != period.end())
                    IdleStatesWeight[type] = Random(period.find(type)->second, &Seed);
            }
        }
    }
};
int main()
{
    int g1, g2, g3; IdleDef def; def.Period[2] = 7; bool ok = true;
    Scheduler s; s.DefIndex = 0; s.SubDef = &def; s.Seed = 1234; s.DefaultPeriod = 100;
    unsigned long seed = 1234; unsigned long w1 = Random(100, &seed);
    s.RegisterIdleStateGroup(&g1, 2);                          // no def index: default period only, one draw
    ok = ok && s.IdleStates[2] == &g1 && (unsigned long)s.IdleStatesWeight[2] == w1 && w1 < 100 && s.Seed == seed;
    s.DefIndex = 5;
    Random(100, &seed); unsigned long w2 = Random(7, &seed);
    s.RegisterIdleStateGroup(&g2, 2);                          // def period 7 for type 2: second draw replaces the first
    ok = ok && s.IdleStates[2] == &g2 && (unsigned long)s.IdleStatesWeight[2] == w2 && w2 < 7 && s.Seed == seed;
    unsigned long w3 = Random(100, &seed);
    s.RegisterIdleStateGroup(&g3, 9);                          // type not in the def: default draw stays
    ok = ok && s.IdleStates[9] == &g3 && (unsigned long)s.IdleStatesWeight[9] == w3 && s.Seed == seed;
    s.DefaultPeriod = 0; s.SubDef = 0;
    s.RegisterIdleStateGroup(&g1, 4);                          // zero range gives 0 but still advances the seed
    Random(0, &seed);
    ok = ok && s.IdleStatesWeight[4] == 0 && s.Seed == seed;
    if (!ok) { std::printf("00801920_TEST FAIL\n"); return 1; }
    std::printf("00801920_TEST PASS\n");
    return 0;
}
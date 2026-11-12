// Behaviour test for 0x008C4FD0 CCreatureAction_ScorpionSummon::CreateBabies: no valid position -> nothing; otherwise the family's
// creature list less the boss's living helpers is created (do-while: at least one), each at a random valid position (GFRandom
// remainder, 0 for an empty range), named SUMMONED_CREATURE; live creatures rise up and become helpers.
#include <cstdio>
#include <vector>
#include <string>
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{ *seed *= 0x24a1; *seed += 0x24df; *seed = ror13(*seed); if (range) return *seed % range; return 0; }
struct Pos { float X, Y, Z; };
struct Creature { long Def; Pos P; std::string Name; bool Dead; bool RoseUp; };
struct Boss { long Living; std::vector<Creature*> Helpers; long CountLivingHelpers() { return Living; } void SetHelperCreature(Creature& c) { Helpers.push_back(&c); } };
struct World { unsigned long Seed; std::vector<Pos> Valid; std::vector<Creature*> Made; bool KillNext;
    Creature* Create(long def, const Pos& p, const std::string& name) { Creature* c = new Creature; c->Def = def; c->P = p; c->Name = name; c->Dead = KillNext; c->RoseUp = false; Made.push_back(c); return c; } };
static void CreateBabies(World& w, Boss& boss, const std::vector<long>& family)
{
    std::vector<Pos> positions = w.Valid;
    if (positions.size() == 0) return;
    long count = (long)family.size() - boss.CountLivingHelpers();
    long i = 0;
    do {
        Pos pos = positions[GFRandom((unsigned long)positions.size(), &w.Seed)];
        long def_index = family[i++];
        Creature* creature = w.Create(def_index, pos, "SUMMONED_CREATURE");
        if (creature && !creature->Dead) { creature->RoseUp = true; boss.SetHelperCreature(*creature); }
    } while (i < count);
}
int main()
{
    World w; w.Seed = 12345; w.KillNext = false;
    Boss boss; boss.Living = 1;
    std::vector<long> family; family.push_back(7); family.push_back(8); family.push_back(9);
    CreateBabies(w, boss, family);
    bool ok = w.Made.empty();
    Pos a = { 1, 2, 3 }, b = { 4, 5, 6 }; w.Valid.push_back(a); w.Valid.push_back(b);
    unsigned long seed = 12345; unsigned long r1 = GFRandom(2, &seed);
    CreateBabies(w, boss, family);
    ok = ok && w.Made.size() == 2 && w.Made[0]->Def == 7 && w.Made[1]->Def == 8 && w.Made[0]->Name == "SUMMONED_CREATURE" &&
         w.Made[0]->P.X == w.Valid[r1].X && boss.Helpers.size() == 2 && w.Made[1]->RoseUp;
    boss.Living = 5; w.KillNext = true;
    CreateBabies(w, boss, family);
    ok = ok && w.Made.size() == 3 && boss.Helpers.size() == 2 && !w.Made[2]->RoseUp;
    for (size_t k = 0; k < w.Made.size(); ++k) delete w.Made[k];
    if (!ok) { std::printf("008c4fd0_TEST FAIL\n"); return 1; }
    std::printf("008c4fd0_TEST PASS\n");
    return 0;
}
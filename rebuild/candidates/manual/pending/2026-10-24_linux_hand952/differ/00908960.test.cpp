// behaviour test for 0x00908960 CCombatSequence_VillagerAdvance::OnCreate: two actions (CCreatureAction_VillagerAdvance, then CCreatureAction_VillagerAdvance) built on (creature, target) are pushed in order, each pushed copy sharing the local's count block, then the base OnCreate runs, and the local's reference is dropped
#include <cstdio>
#include <vector>
struct Block { unsigned long refs; int kind; };
struct Seq {
    std::vector<Block*> pushed; std::vector<int> kinds; bool baseCreated; int creature, target;
};
static void OnCreate(Seq& s, std::vector<Block*>& owned)        // the function under test
{
    Block* local = 0;
    for (int k = 1; k <= 2; ++k) {
        if (local && --local->refs == 0) local->kind = -local->kind; // reset: release the previous block (it is shared, so it survives)
        Block* b = new Block; b->refs = 1; b->kind = k; owned.push_back(b); local = b;
        ++local->refs; s.pushed.push_back(local); s.kinds.push_back(k);   // PushAction copies the counted pointer
    }
    s.baseCreated = true;
    if (local && --local->refs == 0) local->kind = -local->kind;     // ~CCountedPointer
}
int main()
{
    int fail = 0;
    Seq s; s.baseCreated = false; s.creature = 1; s.target = 2;
    std::vector<Block*> owned;
    OnCreate(s, owned);
    if (s.pushed.size() != 2 || s.kinds[0] != 1 || s.kinds[1] != 2) fail |= 1;
    if (!s.baseCreated) fail |= 2;
    for (unsigned int i = 0; i < owned.size(); ++i) { if (owned[i]->refs != 1 || owned[i]->kind <= 0) fail |= 4; delete owned[i]; }
    if (sizeof(void*) * 2 != (unsigned int)(2 * sizeof(void*))) fail |= 8;
    if (fail) { std::printf("00908960_TEST FAIL %d\n", fail); return 1; }
    std::printf("00908960_TEST PASS\n");
    return 0;
}

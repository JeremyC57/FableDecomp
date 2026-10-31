// Behaviour test for GetActionAnimSetList (0x00693140): a copy of the logic over a small model -- a combination-anim action
// collects every type-1 combination set whose first anim is the action; otherwise the main anim set is added when it plays
// the action as a type-1 set.
#include <cstdio>

class CAnimSet { public: short FirstAnim; unsigned char Type; };

struct SetList {                                   // stand-in for vector<CAnimSet*>
    CAnimSet* Items[16];
    int Count;
    SetList() : Count(0) {}
    void push_back(CAnimSet* s) { Items[Count++] = s; }
};

class CAnimateThing { public:
    CAnimSet  MainAnim;
    CAnimSet* Combination[8];
    int       CombinationCount;
    void GetAllCombinationAnimsFromType(long type, SetList& out)
    {
        for (int i = 0; i < CombinationCount; ++i)
            if (Combination[i]->Type == type)
                out.push_back(Combination[i]);
    }
};

class CThingCreatureBase { public:
    CAnimateThing Animation;
    long          CombinationAction;
    bool IsAnimCombinationAnim(long action) const { return action == CombinationAction; }
};

static void GetActionAnimSetList(SetList& sets, CThingCreatureBase& creature, long action)
{
    CAnimateThing& animator = creature.Animation;
    if (creature.IsAnimCombinationAnim(action)) {
        SetList combinationAnims;
        animator.GetAllCombinationAnimsFromType(1, combinationAnims);
        for (int i = 0; i < combinationAnims.Count; ++i) {
            CAnimSet* set = combinationAnims.Items[i];
            if (set->FirstAnim == action)
                sets.push_back(set);
        }
    } else {
        CAnimSet& main = animator.MainAnim;
        if (main.FirstAnim == action && main.Type == 1)
            sets.push_back(&main);
    }
}

int main()
{
    CAnimSet a = { 7, 1 }, b = { 9, 1 }, c = { 7, 2 }, d = { 7, 1 };
    CThingCreatureBase creature;
    creature.Animation.MainAnim.FirstAnim = 3;
    creature.Animation.MainAnim.Type = 1;
    creature.Animation.Combination[0] = &a;
    creature.Animation.Combination[1] = &b;
    creature.Animation.Combination[2] = &c;
    creature.Animation.Combination[3] = &d;
    creature.Animation.CombinationCount = 4;
    creature.CombinationAction = 7;

    SetList r1;
    GetActionAnimSetList(r1, creature, 7);
    if (r1.Count != 2 || r1.Items[0] != &a || r1.Items[1] != &d) { std::printf("00693140_TEST FAIL\n"); return 1; }
    SetList r2;
    GetActionAnimSetList(r2, creature, 3);
    if (r2.Count != 1 || r2.Items[0] != &creature.Animation.MainAnim) { std::printf("00693140_TEST FAIL\n"); return 1; }
    creature.Animation.MainAnim.Type = 2;
    SetList r3;
    GetActionAnimSetList(r3, creature, 3);
    if (r3.Count != 0) { std::printf("00693140_TEST FAIL\n"); return 1; }
    std::printf("00693140_TEST PASS\n");
    return 0;
}
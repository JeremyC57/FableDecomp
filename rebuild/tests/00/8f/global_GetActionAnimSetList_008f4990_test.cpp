// Behaviour test for 0x008F4990 (anonymous namespace) GetActionAnimSetList(vector<CAnimSet*>&, CThingCreatureBase&, long)
// The model mirrors the source: for a combination action every type-1 combination anim set whose first anim is the
// action is appended; otherwise the main anim set is appended when it plays the action as a type-1 set.
#include <cstdio>
#include <vector>

class CAnimSet { public: std::vector<short> Anims; unsigned char Type;
    long GetAnimIndex() const { return Anims[0]; } long GetType() const { return Type; } };
class CAnimateThing { public:
    CAnimSet MainAnimSet;
    std::vector<CAnimSet> Combination;
    void GetAllCombinationAnimsFromType(long type, std::vector<CAnimSet*>& out)
    { for (size_t i = 0; i < Combination.size(); ++i) if (Combination[i].Type == type) out.push_back(&Combination[i]); }
};
struct Creature { CAnimateThing anim; long combinationAction; bool IsAnimCombinationAnim(long a) const { return a == combinationAction; } };

static void GetActionAnimSetList(std::vector<CAnimSet*>& list, Creature& creature, long action)
{
    CAnimateThing& animator = creature.anim;
    if (creature.IsAnimCombinationAnim(action)) {
        std::vector<CAnimSet*> combination;
        animator.GetAllCombinationAnimsFromType(1, combination);
        for (std::vector<CAnimSet*>::iterator it = combination.begin(); it != combination.end(); ++it) {
            CAnimSet* s = *it;
            if (s->GetAnimIndex() == action) list.push_back(s);
        }
    } else {
        CAnimSet& main = animator.MainAnimSet;
        if (main.GetAnimIndex() == action && main.GetType() == 1) list.push_back(&main);
    }
}

static CAnimSet Make(short anim, unsigned char type) { CAnimSet s; s.Anims.push_back(anim); s.Type = type; return s; }

int main()
{
    int fail = 0;
    Creature c;
    c.anim.MainAnimSet = Make(7, 1);
    c.anim.Combination.push_back(Make(5, 1));
    c.anim.Combination.push_back(Make(5, 2));       // wrong type: never collected
    c.anim.Combination.push_back(Make(6, 1));
    c.anim.Combination.push_back(Make(5, 1));
    c.combinationAction = 5;
    std::vector<CAnimSet*> out;
    GetActionAnimSetList(out, c, 5);
    if (out.size() != 2 || out[0] != &c.anim.Combination[0] || out[1] != &c.anim.Combination[3]) fail |= 1;
    out.clear();
    GetActionAnimSetList(out, c, 7);                // not a combination anim: the main set plays it
    if (out.size() != 1 || out[0] != &c.anim.MainAnimSet) fail |= 2;
    out.clear();
    c.anim.MainAnimSet.Type = 3;
    GetActionAnimSetList(out, c, 7);                // main set of another type
    if (!out.empty()) fail |= 4;
    GetActionAnimSetList(out, c, 8);                // nothing plays the action
    if (!out.empty()) fail |= 8;
    if (fail) { std::printf("008f4990_TEST FAIL %d\n", fail); return 1; }
    std::printf("008f4990_TEST PASS\n");
    return 0;
}
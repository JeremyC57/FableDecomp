// Behaviour test for CAIStateGroup_MinionWorkInInn::GetFreeBench 0x0090BC60: null without a workplace or without the owner
// component (interface 0x21); otherwise the owned objects are passed through CAddFreeBenchToList into a vector and one of
// them is picked with GFRandom(count, world seed); null when no bench is free.
#include <cstdio>
#include <vector>
#include <list>
#include <algorithm>

struct CThing { bool HasOwner; bool FreeBench; std::list<CThing*>* Owned; };
static unsigned long gSeed = 5;
static unsigned long gLastRange = 0;
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    gLastRange = range;
    *seed = *seed * 0x24a1 + 0x24df;
    return range ? *seed % range : 0;
}
class CAddFreeBenchToList {
public:
    CAddFreeBenchToList(std::vector<CThing*>& list) : BenchList(&list) { }
    void operator()(CThing* const& thing) const { if (thing && thing->FreeBench) BenchList->push_back(thing); }
private:
    std::vector<CThing*>* BenchList;
};
struct CCreature { CThing* WorkBuilding; CThing* GetWorkBuilding() const { return WorkBuilding; } };
struct CAIStateGroup_MinionWorkInInn {
    CCreature* Creature;
    CCreature& GetCreature() const { return *Creature; }
    unsigned long* GetPWorldSeed() { return &gSeed; }
    CThing* GetFreeBench()
    {
        CThing* workplace = GetCreature().GetWorkBuilding();
        if (workplace && workplace->HasOwner) {
            const std::list<CThing*>& objects = *workplace->Owned;
            std::vector<CThing*> benches;
            std::for_each(objects.begin(), objects.end(), CAddFreeBenchToList(benches));
            if (!benches.empty())
                return benches[GFRandom((unsigned long)benches.size(), GetPWorldSeed())];
        }
        return 0;
    }
};

int main()
{
    bool ok = true;
    CCreature creature = { 0 };
    CAIStateGroup_MinionWorkInInn group = { &creature };
    if (group.GetFreeBench() != 0) ok = false;                  // no workplace
    std::list<CThing*> owned;
    CThing inn = { false, false, &owned };
    creature.WorkBuilding = &inn;
    if (group.GetFreeBench() != 0) ok = false;                  // no owner component
    inn.HasOwner = true;
    if (group.GetFreeBench() != 0) ok = false;                  // nothing owned
    CThing a = { false, false, 0 }, b = { false, true, 0 }, c = { false, false, 0 }, d = { false, true, 0 };
    owned.push_back(&a); owned.push_back(&b); owned.push_back(0); owned.push_back(&c); owned.push_back(&d);
    for (int i = 0; i < 8; ++i) {
        CThing* bench = group.GetFreeBench();
        if (bench != &b && bench != &d) ok = false;
        if (gLastRange != 2) ok = false;
    }
    b.FreeBench = false; d.FreeBench = false;
    if (group.GetFreeBench() != 0) ok = false;                  // no free bench
    if (!ok) { std::printf("0090bc60_TEST FAIL\n"); return 1; }
    std::printf("0090bc60_TEST PASS\n");
    return 0;
}